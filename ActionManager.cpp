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

#include <gak/array.h>

#include "ActionManager.h"
#include "ItemManager.h"

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT-
#	ifdef __WIN32__
#		pragma option -a4
#		pragma option -pc
#	else
#		pragma option -po
#		pragma option -a2
#	endif
#endif

using namespace gak;

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
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

/*
	this is dynamicaly generated, because otherwise, the contructor
	of ACTION_BASE may use this array, before the array itself is
	contructed
*/
static Array<ACTION_PTR> *allActions = NULL;

// --------------------------------------------------------------------- //
// ----- class static data --------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- prototypes ---------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module functions ---------------------------------------------- //
// --------------------------------------------------------------------- //

static int actionCompare(
	const ACTION_PTR &a1,
	const ACTION_PTR &a2
)
{
	int compareResult = a1->getGroup() - a2->getGroup();

	if( !compareResult )
		compareResult = strcmp( a1->getLabel(), a2->getLabel() );

	return compareResult;
}

// --------------------------------------------------------------------- //
// ----- class inlines ------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class constructors/destructors -------------------------------- //
// --------------------------------------------------------------------- //

ACTION_BASE::ACTION_BASE()
{
	if( !allActions )
	{
		allActions = new Array<ACTION_PTR>;
	}

	allActions->addElement( this );
}

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

bool ACTION_BASE::acceptItem( THE_ITEM *theItem )
{
	doEnterFunctionEx( gakLogging::llInfo, "ACTION_BASE::acceptItem()" );

	return theItem->getID() && !theItem->isDeleted();
}

bool ACTION_BASE::acceptUser( const UserOrGroup & )
{
	return true;
}

int ACTION_BASE_PROPERTIES::getGroup( void ) const
{
	return 0;
}

int ACTION_BASE_PERMISSIONS::getGroup( void ) const
{
	return 10;
}

int ACTION_BASE_MOVE::getGroup( void ) const
{
	return 20;
}

int ACTION_BASE_CHECK::getGroup( void ) const
{
	return 30;
}

int ACTION_BASE_VERSIONS::getGroup( void ) const
{
	return 40;
}

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

void getItemActions(
	const UserOrGroup &actUser,
	THE_ITEM *theItem,
	Array<ACTION_BASE *> *actions
)
{
	doEnterFunctionEx( gakLogging::llDetail, "getItemActions()" );
	doLogValueEx( gakLogging::llDetail, allActions->size() );

	actions->clear();

	for(
		Array<ACTION_BASE *>::const_iterator it = allActions->cbegin(),
			endIT = allActions->cend();
		it != endIT;
		++it
	)
	{
		ACTION_BASE *theAction = *it;

		if( theAction->acceptUser( actUser )
		&& theAction->acceptItem( theItem ) )
		{
			actions->addElement( theAction );
		}
	}

	actions->sort( actionCompare );
}

#ifdef __BORLANDC__
#	pragma option -p.
#	pragma option -a.
#endif

