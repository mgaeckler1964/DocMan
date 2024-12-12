/*
		Project:
		Module:
		Description:
		Author:			Martin G�ckler
		Company:		CRESD GmbH
		Address:		Arminiusstr. 3a, D-8543 M�nchen
		Phone:			+49 - 89 - 65 30 95 63
		Telefax:		+49 - 89 - 65 30 95 64
		Copyright:		Siemens AG
		Date:			xx. xx. 20xx
		History:
*/

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include "Delete.h"

#include "ItemManager.h"
#include "Folder.h"
#include "PermissionsFrm.h"
#include "CopyMoveLinkFrm.h"

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

class FACTORY_PERSONAL_TRASH : public FACTORY_ROOT_FOLDER
{
	virtual int getItemType( void ) const;
	virtual PTR_ITEM createItem( int id ) const;
	virtual const char *getName( void ) const;
};

class ACTION_RESTORE : public ACTION_BASE_MOVE
{
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
};

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static FACTORY_PERSONAL_TRASH	thePersonalTrashFactory;
static ACTION_RESTORE			theRestoreAction;

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

ACTION_DELETE	theDeleteAction;
ACTION_PURGE	thePurgeAction;

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

int FACTORY_PERSONAL_TRASH::getItemType( void ) const
{
	return (int)TYPE_PERSONAL_TRASH;
}

PTR_ITEM FACTORY_PERSONAL_TRASH::createItem( int id ) const
{
	return new THE_PERSONAL_TRASH( id, this );
}

const char *FACTORY_PERSONAL_TRASH::getName( void ) const
{
	return "Personal Trash";
}

void THE_PERSONAL_TRASH::openItem( int  )
{
	// don't open any trashed items
}

void THE_PERSONAL_TRASH::getItemFactories( Array<const FACTORY_BASE*> *factory ) const
{
	factory->clear();
}

TGraphic *THE_PERSONAL_TRASH::getItemPicture( void ) const
{
	static Graphics::TBitmap *thePic = NULL;

	if( !thePic )
	{
		thePic = new Graphics::TBitmap();
		thePic->LoadFromResourceName((unsigned int)HInstance, "PERSONAL_TRASH_BITMAP");
	}
	return thePic;
}

void THE_PERSONAL_TRASH::purgeItem( void )
{
	ITEM_CONTENT	*theContent = getContent();

	for(
		ITEM_CONTENT::const_iterator it = theContent->cbegin(),
			endIT = theContent->cend();
		it != endIT;
		++it
	)
	{
		THE_ITEM	*child = *it;
		child->purgeItem();
	}
}

bool ACTION_DELETE::acceptItem( THE_ITEM *theItem )
{
	if( !theItem->isMoveable() )
/***/	return false;

	int perms = theItem->getUserPermissions();
	return perms & ITEM_PERM_DELETE
		? ACTION_BASE::acceptItem( theItem ) && !theItem->isDeleted()
		: false
	;
}

bool ACTION_RESTORE::acceptItem( THE_ITEM *theItem )
{
	if( theItem->isDeleted() )
		return true;

	int perms = theItem->getUserPermissions();
	return perms & ITEM_PERM_MOVE
		? theItem->isMoveable() && theItem->isArchived() 
		: false
	;
}

bool ACTION_PURGE::acceptItem( THE_ITEM *theItem )
{
	int perms = theItem->getUserPermissions();
	if( !(perms & ITEM_PERM_DELETE) )
/*@*/	return false;

	if( theItem->isDeleted() )
/***/	return true;

	THE_PERSONAL_TRASH *trash = dynamic_cast<THE_PERSONAL_TRASH *>(theItem);
	if( trash )
/***/	return true;

	if( theItem->getID() == theItem->getVolumeID() )
/*@*/	return false;

	if( !theItem->isMoveable() )
/***/	return true;

	return false;
}

const char *ACTION_DELETE::getLabel( void ) const
{
	return "Delete";
}

const char *ACTION_RESTORE::getLabel( void ) const
{
	return "Restore...";
}

const char *ACTION_PURGE::getLabel( void ) const
{
	return "Purge";
}

REFRESH_TYPE ACTION_DELETE::perform( PTR_ITEM theItem )
{
	ThreadDeletion *theThread = new ThreadDeletion( theItem );
	theThread->StartThread();

	return REFRESH_RELOAD;
}

REFRESH_TYPE ACTION_RESTORE::perform( PTR_ITEM theItem )
{
	REFRESH_TYPE	refresh;
	STRING			oldName = theItem->getName();
	PTR_ITEM		oldParent = theItem->getOldParent();

	CopyMoveLinkForm->setItem( oldName, oldParent );
	CopyMoveLinkForm->Caption = "Restore";
	if( CopyMoveLinkForm->ShowModal() == mrOk )
	{
		theItem->restoreItem(
			CopyMoveLinkForm->getDestination(),
			CopyMoveLinkForm->EditName->Text.c_str()
		);
		theItem->updateDatabase();
		refresh = REFRESH_RELOAD;
	}
	else
		refresh = REFRESH_NONE;

	return refresh;
}

REFRESH_TYPE ACTION_PURGE::perform( PTR_ITEM theItem )
{
	theItem->getContent();
	ThreadPurging *theThread = new ThreadPurging( theItem );
	theThread->StartThread();

	return REFRESH_RELOAD;
}

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -p.
#	pragma option -a.
#endif

 