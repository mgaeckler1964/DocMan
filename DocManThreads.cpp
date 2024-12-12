/*
		Project:		DocMan
		Module:			
		Description:	
		Author:			Martin Gäckler
		Address:		Hofmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2024 Martin Gäckler

		This program is free software: you can redistribute it and/or modify  
		it under the terms of the GNU General Public License as published by  
		the Free Software Foundation, version 3.

		You should have received a copy of the GNU General Public License 
		along with this program. If not, see <http://www.gnu.org/licenses/>.

		THIS SOFTWARE IS PROVIDED BY Martin Gäckler, Austria, Linz ``AS IS''
		AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
		TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
		PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR
		CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
		SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
		LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
		USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
		ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
		OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
		OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
		SUCH DAMAGE.
*/

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include "DocManThreads.h"

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT-
#	pragma option -b
#	pragma option -a4
#	pragma option -pc
#endif

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class static data --------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- prototypes ---------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module functions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class inlines ------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class constructors/destructors -------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class static functions ---------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class privates ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //
   
const char *ThreadDeletion::getTitle( void ) const
{
	return "Delete";
}

void ThreadDeletion::perform( void )
{
	size_t	numAdditional = additionalItems.size();

	if( numAdditional )
	{
		for(
			ITEM_CONTENT::const_iterator it = additionalItems.cbegin(),
				endIT = additionalItems.cend();
			!terminated && it != endIT;
			++it
		)
		{
			theItemToHandle = *it;

			theItemToHandle->deleteItem();
		}
	}
	else
	{
		theItemToHandle->deleteItem();
	}
}

const char *ThreadCopying::getTitle( void ) const
{
	return "Copy";
}

void ThreadCopying::perform( void )
{
	THE_ITEM::ItemsCopiedMap itemsCopied;
	size_t	numAdditional = additionalItems.size();

	if( numAdditional )
	{
		for(
			ITEM_CONTENT::const_iterator it = additionalItems.cbegin(),
				endIT = additionalItems.cend();
			!terminated && it != endIT;
			++it
		)
		{
			theItemToHandle = *it;

			theItemToHandle->copy(
				itemsCopied, m_target, m_createLink, m_moveReserve,
				theItemToHandle->getName()
			);
		}
	}
	else
	{
		PTR_ITEM	newItem = theItemToHandle->copy(
			itemsCopied, m_target, m_createLink, m_moveReserve, m_newName
		);
		if( newItem )
		{
			if( !m_newDescription.isEmpty() )
			{
				newItem->setDescription( m_newDescription );
				newItem->updateDatabase();
			}
		}
	}
}

const char *ThreadMoving::getTitle( void ) const
{
	return "Move";
}

void ThreadMoving::perform( void )
{
	size_t	numAdditional = additionalItems.size();

	if( numAdditional )
	{
		for(
			ITEM_CONTENT::const_iterator it = additionalItems.cbegin(),
				endIT = additionalItems.cend();
			!terminated && it != endIT;
			++it
		)
		{
			theItemToHandle = *it;

			theItemToHandle->moveTo(
				target, theItemToHandle->getName()
			);
			theItemToHandle->updateDatabase();
		}
	}
	else
	{
		theItemToHandle->moveTo( target, newName );
		theItemToHandle->updateDatabase();
	}
}

const char *ThreadPurging::getTitle( void ) const
{
	return "Purge";
}

void ThreadPurging::perform( void )
{
	theItemToHandle->purgeItem();
}

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

