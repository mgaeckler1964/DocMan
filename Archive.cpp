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

#include "ActionManager.h"
#include "Folder.h"
#include "Archive.h"
#include "PermissionsFrm.h"

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT-
#	pragma option -b
#	ifdef __WIN32__
#		pragma option -a4
#		pragma option -pc
#	else
#		pragma option -po
#		pragma option -a2
#	endif
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

class FACTORY_ARCHIVE_FOLDER : public FACTORY_ROOT_FOLDER
{
	virtual int getItemType( void ) const;
	virtual PTR_ITEM createItem( int id ) const;
	virtual const char *getName( void ) const;
};

class ACTION_ARCHIVE : public ACTION_BASE_MOVE
{
	public:
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
};

class ACTION_SHOW_ARCHIVE : public ACTION_BASE_PROPERTIES
{
	public:
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static ACTION_ARCHIVE			theArchiveAction;
static ACTION_SHOW_ARCHIVE		theShowArchiveAction;
static FACTORY_ARCHIVE_FOLDER	theArchiveFolderFactory;

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

int FACTORY_ARCHIVE_FOLDER::getItemType( void ) const
{
	return (int)TYPE_ARCHIVE_FOLDER;
}

PTR_ITEM FACTORY_ARCHIVE_FOLDER::createItem( int id ) const
{
	return new THE_ARCHIVE_FOLDER( id, this );
}

const char *FACTORY_ARCHIVE_FOLDER::getName( void ) const
{
	return "Archive Folder";
}

void THE_ARCHIVE_FOLDER::getItemFactories( Array<const FACTORY_BASE*> *factory ) const
{
	factory->clear();
}

TGraphic *THE_ARCHIVE_FOLDER::getItemPicture( void ) const
{
	static Graphics::TBitmap *thePic = NULL;

	if( !thePic )
	{
		thePic = new Graphics::TBitmap();
		thePic->LoadFromResourceName((unsigned int)HInstance, "COMPANY_FOLDER_BITMAP");
	}
	return thePic;
}


bool ACTION_ARCHIVE::acceptItem( THE_ITEM *theItem )
{
	if( !theItem->isMoveable() )
/***/	return false;

	int perms = theItem->getUserPermissions();
	return (perms & (ITEM_PERM_DELETE|ITEM_PERM_MOVE)) == (ITEM_PERM_DELETE|ITEM_PERM_MOVE)
		? ACTION_BASE::acceptItem( theItem ) && !theItem->isDeleted() && !theItem->isArchived()
		: false
	;
}

const char *ACTION_ARCHIVE::getLabel( void ) const
{
	return "Archive";
}

REFRESH_TYPE ACTION_ARCHIVE::perform( PTR_ITEM theItem )
{
	(*theItem).archive();

	return REFRESH_RELOAD;
}

bool ACTION_SHOW_ARCHIVE::acceptItem( THE_ITEM *theItem )
{
	if( !theItem->isMoveable() )
/***/	return false;

	int perms = theItem->getUserPermissions();
	return (perms & (ITEM_PERM_DELETE|ITEM_PERM_MOVE)) == (ITEM_PERM_DELETE|ITEM_PERM_MOVE)
		? ACTION_BASE::acceptItem( theItem ) && !theItem->isDeleted() && !theItem->isArchived()
		: false
	;
}

const char *ACTION_SHOW_ARCHIVE::getLabel( void ) const
{
	return "Show Archive";
}

REFRESH_TYPE ACTION_SHOW_ARCHIVE::perform( PTR_ITEM theItem )
{
//	(*theItem).archive();

	PTR_ITEM	theArchive = (*theItem).getArchive( false );
	if( theArchive )
		(*theArchive).open();

	return REFRESH_NONE;
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

