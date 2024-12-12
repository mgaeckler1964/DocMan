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

#ifndef DOC_MAN_THREADS_H
#define DOC_MAN_THREADS_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include "StatusFrm.h"
#include "ItemManager.h"

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

#pragma option -RT-
class ThreadDocMan : public StatusThread
{
	protected:
	PTR_ITEM		theItemToHandle;
	ITEM_CONTENT	additionalItems;

	public:
	ThreadDocMan( const PTR_ITEM &itemToHandle=PTR_ITEM() )
	: StatusThread(), theItemToHandle( itemToHandle )
	{
	}
	void addItem( const PTR_ITEM &newItem )
	{
		additionalItems.addElement( newItem );
	}
};

class ThreadDeletion : public ThreadDocMan
{
	virtual const char *getTitle( void ) const;
	virtual void perform( void );

	public:
	ThreadDeletion( const PTR_ITEM &theItemToHandle ) : ThreadDocMan(theItemToHandle) {};
};

class ThreadPurging : public ThreadDocMan
{
	virtual const char *getTitle( void ) const;
	virtual void perform( void );

	public:
	ThreadPurging( const PTR_ITEM &theItemToHandle ) : ThreadDocMan(theItemToHandle) {};
};

class ThreadCopying : public ThreadDocMan
{
	PTR_ITEM	m_target;
	STRING		m_newName, m_newDescription;
	bool		m_createLink, m_moveReserve;

	virtual const char *getTitle( void ) const;
	virtual void perform( void );

	public:
	ThreadCopying(
		const PTR_ITEM &theItemToHandle,
		const PTR_ITEM &target, bool createLink, bool moveReserve,
		const STRING &newName,
		const STRING &descr = ""
	 )
	: m_newName( newName ), m_newDescription( descr ), ThreadDocMan(theItemToHandle)
	{
		m_target = target;
		m_createLink = createLink;
		m_moveReserve = moveReserve;
	};
};

class ThreadMoving : public ThreadDocMan
{
	PTR_ITEM	target;
	STRING		newName, newDescription;

	virtual const char *getTitle( void ) const;
	virtual void perform( void );

	public:
	ThreadMoving(
		const PTR_ITEM &theItemToHandle,
		const PTR_ITEM &target, const STRING &name,
		const STRING &descr = ""
	 )
	: newName( name ), newDescription( descr ), ThreadDocMan(theItemToHandle)
	{
		this->target = target;
	};
};
#pragma option -RT+

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

#endif	// DOC_MAN_THREADS_H