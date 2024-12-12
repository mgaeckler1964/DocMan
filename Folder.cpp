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

#include <sys/stat.h>

#include <gak/strFiles.h>
#include <gak/exif.h>
#include <gak/numericString.h>

#include <graphix/magic.h>

#include "Folder.h"
#include "WebFolder.h"
#include "File.h"
#include "ItemCreateFrm.h"
#include "ReleaseFrm.h"
#include "SyncFolderCreateFrm.h"
#include "SourceFolderCreateFrm.h"
#include "DocManMain.h"
#include "ActionManager.h"
#include "PermissionsFrm.h"
#include "DocManDM.h"
#include "Delete.h"
#include "Config.h"

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

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

class FACTORY_PUBLIC_TEMPLATE_FOLDER : public FACTORY_ROOT_FOLDER
{
	virtual int getItemType( void ) const;
	virtual PTR_ITEM createItem( int id ) const;
	virtual const char *getName( void ) const;
};

class FACTORY_PERSONAL_TEMPLATE_FOLDER : public FACTORY_ROOT_FOLDER
{
	virtual int getItemType( void ) const;
	virtual PTR_ITEM createItem( int id ) const;
	virtual const char *getName( void ) const;
};

class FACTORY_BOOKMARK_FOLDER : public FACTORY_ROOT_FOLDER
{
	virtual int getItemType( void ) const;
	virtual PTR_ITEM createItem( int id ) const;
	virtual const char *getName( void ) const;
};

class FACTORY_COMPANY_FOLDER : public FACTORY_ROOT_FOLDER
{
	virtual int getItemType( void ) const;
	virtual PTR_ITEM createItem( int id ) const;
	virtual const char *getName( void ) const;
};

class FACTORY_XSLT_FOLDER : public FACTORY_BASE
{
	virtual int getItemType( void ) const;
	virtual PTR_ITEM createItem( int id ) const;
	virtual const char *getName( void ) const;
	virtual bool acceptParent( const THE_ITEM *parent ) const;
};

class FACTORY_SYNC_FOLDER : public FACTORY_LOCAL_FOLDER
{
	virtual int getItemType( void ) const;
	virtual PTR_ITEM createItem( int id ) const;
	virtual const char *getName( void ) const;
	virtual TItemCreateForm *getForm( void ) const;
	virtual PTR_ITEM createItemFromForm( const PTR_ITEM &parent ) const;
};

class FACTORY_FOLDER_REF : public FACTORY_BASE
{
	virtual int getItemType( void ) const;
	virtual bool acceptParent( const THE_ITEM *parent ) const;
	virtual PTR_ITEM createItem( int id ) const;
	virtual const char *getName( void ) const;
};

class FACTORY_SOURCE_FOLDER : public FACTORY_SYNC_FOLDER
{
	virtual int getItemType( void ) const;
	virtual PTR_ITEM createItem( int id ) const;
	virtual const char *getName( void ) const;
	virtual TItemCreateForm *getForm( void ) const;
	virtual PTR_ITEM createItemFromForm( const PTR_ITEM &parent ) const;
};

class FACTORY_RELEASE_FOLDER : public FACTORY_BASE
{
	public:
	virtual int getItemType( void ) const;
	virtual bool acceptParent( const THE_ITEM *parent ) const;
	virtual PTR_ITEM createItem( int id ) const;
	virtual const char *getName( void ) const;
	virtual TItemCreateForm *getForm( void ) const;
	virtual PTR_ITEM createItemFromForm( const PTR_ITEM &parent ) const;
	virtual PTR_ITEM createItemFromTemplate(
		const PTR_ITEM &parent,
		const THE_ITEM *src,
		const STRING &newName=""
	) const;
};

class ACTION_REFRESH : public ACTION_BASE_MOVE
{
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
};

class ACTION_IMPORT : public ACTION_BASE_MOVE
{
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
};

class ACTION_EXPLORER : public ACTION_BASE_PROPERTIES
{
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
};

class ACTION_LOCK_TREE : public ACTION_BASE_VERSIONS
{
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
};

class ACTION_UNLOCK_TREE : public ACTION_BASE_VERSIONS
{
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
};

class THREAD_IMPORTING : public ThreadDocMan
{
	virtual const char *getTitle( void ) const;
	virtual void perform( void );

	public:
	THREAD_IMPORTING( const PTR_ITEM &theItemToHandle )
	: ThreadDocMan(theItemToHandle) {}
};

class THREAD_REFRESHING : public ThreadDocMan
{
	virtual const char *getTitle( void ) const;
	virtual void perform( void );

	public:
	THREAD_REFRESHING( const PTR_ITEM &theItemToHandle )
	: ThreadDocMan(theItemToHandle) {}
};

class THREAD_LOCK_TREE : public ThreadDocMan
{
	virtual const char *getTitle( void ) const;
	virtual void perform( void );

	public:
	THREAD_LOCK_TREE( const PTR_ITEM &theItemToHandle ) : ThreadDocMan(theItemToHandle) {};
};

class THREAD_UNLOCK_TREE : public ThreadDocMan
{
	virtual const char *getTitle( void ) const;
	virtual void perform( void );

	public:
	THREAD_UNLOCK_TREE( const PTR_ITEM &theItemToHandle ) : ThreadDocMan(theItemToHandle) {};
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //



static FACTORY_PUBLIC_TEMPLATE_FOLDER	thePublicTemplateFolderFactory;
static FACTORY_PERSONAL_TEMPLATE_FOLDER	thePersonalTemplateFolderFactory;
static FACTORY_FOLDER					theFolderFactory;
static FACTORY_PERSONAL_FOLDER			thePersonalFolderFactory;
static FACTORY_COMPANY_FOLDER			theCompanyFolderFactory;
static FACTORY_XSLT_FOLDER				theXsltFolderFactory;

static FACTORY_BOOKMARK_FOLDER			theBookmarkFolderFactrory;

static FACTORY_SYNC_FOLDER				theSyncFolderFactory;
static FACTORY_FOLDER_REF				theFolderRefFactory;

static FACTORY_SOURCE_FOLDER			theSourceFolderFactory;
static FACTORY_RELEASE_FOLDER			theReleaseFolderFactory;

static ACTION_REFRESH					theRefreshAction;
static ACTION_IMPORT					theImportAction;
static ACTION_EXPLORER					theExplorerAction;
static ACTION_LOCK_TREE					theLockTreeAction;
static ACTION_UNLOCK_TREE				theUnlockTreeAction;

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

int THE_LOCAL_FOLDER::itemCompare( const PTR_ITEM &e1, const PTR_ITEM &e2, int theSort )
{
	PTR_FILE_BASE	fp1(e1);
	PTR_FILE_BASE	fp2(e2);

	int	compareResult = 0;

	if( abs( theSort ) == SORT_STATUS )
	{
		STRING status1 = fp1 ? fp1->calcStatus( false ) : NULL_STRING;
		STRING status2 = fp2 ? fp2->calcStatus( false ) : NULL_STRING;

		if( status1 == "OK" && status2 != "OK" )
			compareResult = 1;
		else if( status1 != "OK" && status2 == "OK" )
			compareResult = -1;
		else
			compareResult = strcmpi( status1, status2 );
	}

	if( !compareResult )
		return THE_FOLDER_REF::itemCompare( e1, e2, theSort );

	return theSort < 0 ? -compareResult : compareResult;
}

// --------------------------------------------------------------------- //
// ----- class privates ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //


int FACTORY_PUBLIC_TEMPLATE_FOLDER::getItemType( void ) const
{
	return (int)TYPE_PUBLIC_TEMPLATE_FOLDER;
}

int FACTORY_PERSONAL_TEMPLATE_FOLDER::getItemType( void ) const
{
	return (int)TYPE_PERSONAL_TEMPLATE_FOLDER;
}

int FACTORY_FOLDER::getItemType( void ) const
{
	return (int)TYPE_FOLDER;
}

int FACTORY_PERSONAL_FOLDER::getItemType( void ) const
{
	return (int)TYPE_PERSONAL_FOLDER;
}


int FACTORY_COMPANY_FOLDER::getItemType( void ) const
{
	return (int)TYPE_COMPANY_FOLDER;
}

int FACTORY_XSLT_FOLDER::getItemType( void ) const
{
	return (int)TYPE_XSLT_FOLDER;
}

int FACTORY_SYNC_FOLDER::getItemType( void ) const
{
	return (int)TYPE_SYNC_FOLDER;
}

int FACTORY_FOLDER_REF::getItemType( void ) const
{
	return (int)TYPE_FOLDER_REF;
}

int FACTORY_SOURCE_FOLDER::getItemType( void ) const
{
	return (int)TYPE_SOURCE_FOLDER;
}

int FACTORY_RELEASE_FOLDER::getItemType( void ) const
{
	return (int)TYPE_RELEASE_FOLDER;
}

int FACTORY_BOOKMARK_FOLDER::getItemType( void ) const
{
	return TYPE_BOOKMARK_FOLDER;
}

bool FACTORY_FOLDER::acceptParent( const THE_ITEM *parent ) const
{
	if( parent->getID() <= 0 )
		return true;	// accept the root

	const THE_FOLDER *theFolder = dynamic_cast<const THE_FOLDER *>(parent);
	return theFolder ? true : false;
}

bool FACTORY_LOCAL_FOLDER::acceptParent( const THE_ITEM *iParent ) const
{
	PTR_ITEM parent( const_cast<THE_ITEM *>(iParent) );

	if( parent->getID() <= 0 )
		return false;	// Don't accept the root

	PTR_FOLDER		theFolder = parent;
	PTR_WEB_FOLDER	webFolder;

	bool acceptable = theFolder ? true : false;

	while( acceptable && parent )
	{
		webFolder = parent;
		if( webFolder )
			acceptable = false;
		else
			parent = parent->getParent();
	}

	return acceptable;
}

bool FACTORY_FOLDER_REF::acceptParent( const THE_ITEM *parent ) const
{
	int theType = parent->getItemType();
	return theType == TYPE_SYNC_FOLDER || theType == TYPE_FOLDER_REF
		? true : false;
}

bool FACTORY_XSLT_FOLDER::acceptParent( const THE_ITEM *parent ) const
{
	int theType = parent->getItemType();
	return (theType == TYPE_XSLT_FOLDER) ? true : false;
}

bool FACTORY_RELEASE_FOLDER::acceptParent( const THE_ITEM *parent ) const
{
	const THE_SOURCE_FOLDER *theSourceFolder = dynamic_cast<const THE_SOURCE_FOLDER *>(parent);
	return theSourceFolder ? true : false;
}




PTR_ITEM FACTORY_PUBLIC_TEMPLATE_FOLDER::createItem( int id ) const
{
	return new THE_PUBLIC_TEMPLATE_FOLDER( id, this );
}

PTR_ITEM FACTORY_PERSONAL_TEMPLATE_FOLDER::createItem( int id ) const
{
	return new THE_PERSONAL_TEMPLATE_FOLDER( id, this );
}

PTR_ITEM FACTORY_FOLDER::createItem( int id ) const
{
	return new THE_FOLDER( id, this );
}

PTR_ITEM FACTORY_PERSONAL_FOLDER::createItem( int id ) const
{
	return new THE_PERSONAL_FOLDER( id, this );
}

PTR_ITEM FACTORY_COMPANY_FOLDER::createItem( int id ) const
{
	return new THE_COMPANY_FOLDER( id, this );
}

PTR_ITEM FACTORY_XSLT_FOLDER::createItem( int id ) const
{
	return new THE_XSLT_FOLDER( id, this );
}

PTR_ITEM FACTORY_SYNC_FOLDER::createItem( int id ) const
{
	return new THE_SYNC_FOLDER( id, this );
}

PTR_ITEM FACTORY_FOLDER_REF::createItem( int id ) const
{
	return new THE_FOLDER_REF( id, this );
}

PTR_ITEM FACTORY_SOURCE_FOLDER::createItem( int id ) const
{
	return new THE_SOURCE_FOLDER( id, this );
}

PTR_ITEM FACTORY_RELEASE_FOLDER::createItem( int id ) const
{
	return new THE_RELEASE_FOLDER( id, this );
}

PTR_ITEM FACTORY_BOOKMARK_FOLDER::createItem( int id ) const
{
	return new THE_BOOKMARK_FOLDER( id, this );
}



const char *FACTORY_PUBLIC_TEMPLATE_FOLDER::getName( void ) const
{
	return "Public Template Folder";
}

const char *FACTORY_PERSONAL_TEMPLATE_FOLDER::getName( void ) const
{
	return "Personal Template Folder";
}

const char *FACTORY_FOLDER::getName( void ) const
{
	return "Folder";
}

const char *FACTORY_PERSONAL_FOLDER::getName( void ) const
{
	return "Personal Folder";
}

const char *FACTORY_COMPANY_FOLDER::getName( void ) const
{
	return "Company Folder";
}

const char *FACTORY_XSLT_FOLDER::getName( void ) const
{
	return "XSLT Folder";
}

const char *FACTORY_SYNC_FOLDER::getName( void ) const
{
	return "Sync Folder";
}

const char *FACTORY_FOLDER_REF::getName( void ) const
{
	return "Folder Reference";
}

const char *FACTORY_SOURCE_FOLDER::getName( void ) const
{
	return "Source Folder";
}

const char *FACTORY_RELEASE_FOLDER::getName( void ) const
{
	return "Release";
}

const char *FACTORY_BOOKMARK_FOLDER::getName( void ) const
{
	return "Bookmark Folder";
}

TItemCreateForm *FACTORY_SYNC_FOLDER::getForm( void ) const
{
	return SyncFolderCreateForm;
}

TItemCreateForm *FACTORY_SOURCE_FOLDER::getForm( void ) const
{
	return SourceFolderCreateForm;
}

TItemCreateForm *FACTORY_RELEASE_FOLDER::getForm( void ) const
{
	return ReleaseForm;
}

PTR_ITEM FACTORY_SYNC_FOLDER::createItemFromForm( const PTR_ITEM &parent ) const
{
	PTR_ITEM newFolder = createItem( 0 );
	THE_SYNC_FOLDER *fp
		= (THE_SYNC_FOLDER *)static_cast<THE_ITEM *>(newFolder)
	;

	fp->setData(
		parent,
		SyncFolderCreateForm->EditName->Text.c_str(),
		SyncFolderCreateForm->MemoDescription->Text.c_str(),
		SyncFolderCreateForm->EditLocalPath->Text.c_str()
	);
	fp->updateDatabase();

	return newFolder;
}

PTR_ITEM FACTORY_SOURCE_FOLDER::createItemFromForm( const PTR_ITEM &parent ) const
{
	PTR_ITEM newFolder = createItem( 0 );
	THE_SOURCE_FOLDER *fp
		= (THE_SOURCE_FOLDER *)static_cast<THE_ITEM *>(newFolder)
	;

	fp->setData(
		parent,
		SourceFolderCreateForm->EditName->Text.c_str(),
		SourceFolderCreateForm->MemoDescription->Text.c_str(),
		SourceFolderCreateForm->EditLocalPath->Text.c_str()
	);
	fp->updateDatabase();

	unsigned 	major, minor, patch;
	major = SourceFolderCreateForm->UpDownMajor->Position;
	minor = SourceFolderCreateForm->UpDownMinor->Position;
	patch = SourceFolderCreateForm->UpDownPatch->Position;
	if( major || minor || patch )
	{
		PTR_ITEM	newItem = theReleaseFolderFactory.createItem( 0 );
		THE_RELEASE_FOLDER *newRelease
			= (THE_RELEASE_FOLDER *)static_cast<THE_ITEM *>(newItem)
		;

		STRING	name = formatNumber( major );
		name += '.';
		name += formatNumber( minor );
		name += '.';
		name += formatNumber( patch );

		newRelease->setData( newFolder, name, "", major, minor, patch );
		newRelease->updateDatabase();
	}

	return newFolder;
}

PTR_ITEM FACTORY_RELEASE_FOLDER::createItemFromForm( const PTR_ITEM &parent ) const
{
	PTR_ITEM newFolder = createItem( 0 );
	THE_RELEASE_FOLDER *fp
		= (THE_RELEASE_FOLDER *)static_cast<THE_ITEM *>(newFolder)
	;

	fp->setData(
		parent,
		ReleaseForm->EditName->Text.c_str(),
		ReleaseForm->MemoDescription->Text.c_str(),
		ReleaseForm->UpDownMajor->Position,
		ReleaseForm->UpDownMinor->Position,
		ReleaseForm->UpDownPatch->Position
	);
	fp->updateDatabase();

	return newFolder;
}

PTR_ITEM FACTORY_RELEASE_FOLDER::createItemFromTemplate(
		const PTR_ITEM &parent,
		const THE_ITEM *src,
		const STRING &newName
	) const
{
	PTR_ITEM newFolder;
	const THE_RELEASE_FOLDER *srcFolder = dynamic_cast<const THE_RELEASE_FOLDER *>(src);
	if( srcFolder )
	{
		newFolder = createItem( 0 );
		if( newFolder )
		{
			THE_RELEASE_FOLDER *fp
				= (THE_RELEASE_FOLDER *)static_cast<THE_ITEM *>(newFolder)
			;
			if( newName.isEmpty() )
			{
				fp->setData(
					parent,
					srcFolder->getName(),
					srcFolder->getDescription(),
					srcFolder->getMajor(),
					srcFolder->getMinor(),
					srcFolder->getPatch()
				);
			}
			else
			{
				T_STRING tmp = newName;
				STRING majorStr = tmp.getFirstToken( "." );
				STRING minorStr =  tmp.getNextToken();
				STRING patchStr =  tmp.getNextToken();

				int major, minor, patch;

				major = majorStr.getValueN<unsigned>();
				minor = minorStr.getValueN<unsigned>();
				patch = patchStr.getValueN<unsigned>();

				fp->setData(
					parent,
					newName,
					"",
					major,
					minor,
					patch
				);
			}
			fp->setCopyID( src->getID() );
			fp->updateDatabase();
		}
	}

	return newFolder;
}



TGraphic *THE_PUBLIC_TEMPLATE_FOLDER::getItemPicture( void ) const
{
	static Graphics::TBitmap *thePic = NULL;

	if( !thePic )
	{
		thePic = new Graphics::TBitmap();
		thePic->LoadFromResourceName((unsigned int)HInstance, "TEMPLATE_FOLDER_BITMAP");
	}
	return thePic;
}

TGraphic *THE_PERSONAL_TEMPLATE_FOLDER::getItemPicture( void ) const
{
	static Graphics::TBitmap *thePic = NULL;

	if( !thePic )
	{
		thePic = new Graphics::TBitmap();
		thePic->LoadFromResourceName((unsigned int)HInstance, "TEMPLATE_FOLDER_BITMAP");
	}
	return thePic;
}

TGraphic *THE_FOLDER::getItemPicture( void ) const
{
	static Graphics::TBitmap *thePic = NULL;

	if( !thePic )
	{
		thePic = new Graphics::TBitmap();
		thePic->LoadFromResourceName((unsigned int)HInstance, "FOLDER_BITMAP");
	}
	return thePic;
}

TGraphic *THE_PERSONAL_FOLDER::getItemPicture( void ) const
{
	static Graphics::TBitmap *thePic = NULL;

	if( !thePic )
	{
		thePic = new Graphics::TBitmap();
		thePic->LoadFromResourceName((unsigned int)HInstance, "PERSONAL_FOLDER_BITMAP");
	}
	return thePic;
}

TGraphic *THE_BOOKMARK_FOLDER::getItemPicture( void ) const
{
	static Graphics::TBitmap *thePic = NULL;

	if( !thePic )
	{
		thePic = new Graphics::TBitmap();
		thePic->LoadFromResourceName((unsigned int)HInstance, "BOOKMARK_FOLDER_BITMAP");
	}
	return thePic;
}

TGraphic *THE_SYNC_FOLDER::getItemPicture( void ) const
{
	static Graphics::TBitmap *thePic = NULL;

	if( !thePic )
	{
		thePic = new Graphics::TBitmap();
		thePic->LoadFromResourceName((unsigned int)HInstance, "SYNC_FOLDER_BITMAP");
	}
	return thePic;
}

TGraphic *THE_SOURCE_FOLDER::getItemPicture( void ) const
{
	static Graphics::TBitmap *thePic = NULL;

	if( !thePic )
	{
		thePic = new Graphics::TBitmap();
		thePic->LoadFromResourceName((unsigned int)HInstance, "SOURCE_FOLDER_BITMAP");
	}
	return thePic;
}

TGraphic *THE_RELEASE_FOLDER::getItemPicture( void ) const
{
	static Graphics::TBitmap *thePic = NULL;

	if( !thePic )
	{
		thePic = new Graphics::TBitmap();
		thePic->LoadFromResourceName((unsigned int)HInstance, "RELEASE_FOLDER_BITMAP");
	}
	return thePic;
}

TGraphic *THE_XSLT_FOLDER::getItemPicture( void ) const
{
	static Graphics::TBitmap *thePic = NULL;

	if( !thePic )
	{
		thePic = new Graphics::TBitmap();
		thePic->LoadFromResourceName((unsigned int)HInstance, "XSLT_FOLDER_BITMAP");
	}
	return thePic;
}

TGraphic *THE_COMPANY_FOLDER::getItemPicture( void ) const
{
	static Graphics::TBitmap *thePic = NULL;

	if( !thePic )
	{
		thePic = new Graphics::TBitmap();
		thePic->LoadFromResourceName((unsigned int)HInstance, "COMPANY_FOLDER_BITMAP");
	}
	return thePic;
}



void THE_LOCAL_FOLDER::loadFields( TQuery *query )
{
	THE_FOLDER::loadFields( query );

	STRING machine = TDocManMainForm::getMachine();

	std::auto_ptr<TQuery>	theQuery( new TQuery( NULL ) );
	theQuery->DatabaseName = "docManDB";

	theQuery->SQL->Add(
		"select id, local_path "
		"from i_local_folder "
		"where folder_id = :folderId "
		"and user_id = :actUser "
		"and lower( machine ) = lower( :theMachine )"
	);
	theQuery->Params->Items[0]->AsInteger = getID();
	theQuery->Params->Items[1]->AsInteger = vcl::getActUserID();
	theQuery->Params->Items[2]->AsString = static_cast<const char *>(machine);
	theQuery->Open();
	if( !theQuery->Eof )
	{
		id = theQuery->Fields->Fields[0]->AsInteger;
		setLocalPath( theQuery->Fields->Fields[1]->AsString.c_str() );
	}

	theQuery->Close();
}

void THE_RELEASE_FOLDER::loadFields( TQuery *query )
{
	THE_LOCAL_FOLDER::loadFields( query );

	release.major = query->FieldByName( "major_release" )->AsInteger;
	release.minor = query->FieldByName( "minor_release" )->AsInteger;
	release.patch = query->FieldByName( "patch_release" )->AsInteger;
}

void THE_FOLDER::updateDatabase( void )
{
	STRING	localPath = getDownloadPath();
	int		id = getID();

	THE_ITEM::updateDatabase();

	if( !localPath.isEmpty() )
	{
		localPath.cut(localPath.strlen()-1);	// remove trailling back slash
		if( !exists( localPath ) )
		{
			if( !id )
			{
				makeDirectory( localPath );
			}
			else
			{
				STRING oldName = getPreviousName();
				STRING oldPath = makeFullPath( localPath, oldName );
				if( exists( oldPath ) )
					strRename( oldPath, localPath );
			}
		}
	}
}

void THE_LOCAL_FOLDER::updateDatabase( void )
{
	THE_FOLDER::updateDatabase();

	if( !localPath.isEmpty() )
	{
		STRING machine = TDocManMainForm::getMachine();
		std::auto_ptr<TQuery>	theQuery( new TQuery( NULL ) );
		theQuery->DatabaseName = "docManDB";

		if( !id )
		{
			id = ConfigDataModule->getNewMaxValue(
				"i_local_folder", "ID"
			);
			theQuery->SQL->Add(
				"insert into i_local_folder "
				"( id, folder_id, user_id, machine, local_path ) "
				"values"
				"( :id, :folder_id, :user_id, :machine, :local_path )"
			);

			theQuery->Params->Items[0]->AsInteger = id;
			theQuery->Params->Items[1]->AsInteger = getID();
			theQuery->Params->Items[2]->AsInteger = vcl::getActUserID();
			theQuery->Params->Items[3]->AsString = static_cast<const char *>(machine);
			theQuery->Params->Items[4]->AsMemo = static_cast<const char *>(localPath);
		}
		else if( id )
		{
			theQuery->SQL->Add(
				"update i_local_folder set local_path = :newPath where id=:oldId"
			);
			theQuery->Params->Items[0]->AsMemo = (const char *)localPath;
			theQuery->Params->Items[1]->AsInteger = id;
		}

		theQuery->ExecSQL();
	}
}

void THE_LOCAL_FOLDER::purgeItem( void )
{
	if( !isDeleted() && isMoveable() )
		throw Exception( "Cannot purge items not deleted"  );

	if( !canDelete( true, false ) )
		throw Exception( "Cannot purge item"  );

	std::auto_ptr<TQuery>	theQuery( new TQuery( NULL ) );
	theQuery->DatabaseName = "docManDB";

	theQuery->SQL->Add(
		"delete from i_local_folder where folder_id = :id"
	);
	theQuery->Params->Items[0]->AsInteger = getID();
	theQuery->ExecSQL();

	THE_ITEM::purgeItem();
}

void THE_RELEASE_FOLDER::updateDatabase( void )
{
	THE_LOCAL_FOLDER::updateDatabase();

	std::auto_ptr<TQuery>	theQuery( new TQuery( NULL ) );
	theQuery->DatabaseName = "docManDB";

	theQuery->SQL->Add(
		"update item_tree "
		"set major_release = :major, "
			"minor_release = :minor, "
			"patch_release = :patch "
		"where id=:Id"
	);
	theQuery->Params->Items[0]->AsInteger = release.major;
	theQuery->Params->Items[1]->AsInteger = release.minor;
	theQuery->Params->Items[2]->AsInteger = release.patch;
	theQuery->Params->Items[3]->AsInteger = getID();

	theQuery->ExecSQL();
}

STRING THE_LOCAL_FOLDER::getDownloadPath( PTR_ITEM parent )
{
	doEnterFunction("THE_LOCAL_FOLDER::getDownloadPath");

	STRING	downloadPath = localPath;
	if( downloadPath.isEmpty() )
		downloadPath = THE_FOLDER::getDownloadPath( parent );

	return downloadPath;
}

STRING THE_RELEASE_FOLDER::getDownloadPath( PTR_ITEM parent )
{
	doEnterFunction("THE_RELEASE_FOLDER::getDownloadPath");

	STRING	downloadPath = getLocalPath();
	if( downloadPath.isEmpty() )
	{
		PTR_ITEM theParent = parent ? parent : getParent();
		if( theParent )
		{
			downloadPath = theParent->getDownloadPath();
		}
	}

	return downloadPath;
}

void THE_BOOKMARK_FOLDER::getItemFactories( Array<const FACTORY_BASE*> *factory ) const
{
	factory->clear();
}

bool THE_SOURCE_FOLDER::refresh( bool recursive, ostream * )
{
	doEnterFunction("THE_SOURCE_FOLDER::refresh");
	if( StatusForm->pushStatus( "Refreshing", getName() ) )
	{
/*@*/	return false;
	}

	bool				refreshFiles = true;
	PTR_RELEASE_FOLDER	release;
	STRING				localPath = getDownloadPath();

	if( localPath.isEmpty() )
		refreshFiles = false;

	ITEM_CONTENT	*theContent	= getContent();

	for( size_t i=0; i<theContent->size(); i++ )
	{
		PTR_ITEM	child = getContentItem( i );
		PTR_FILE	file = child;
		if( file )
		{
			if( refreshFiles )
			{
				STRING	name = file->getName();
				if( StatusForm->pushStatus( "Refreshing", name ) )
				{
/*v*/				break;
				}
				file->download( 0, true, localPath+name );
				if( StatusForm->restore() )
				{
/*v*/				break;
				}
			}
		}
		else
		{
			PTR_RELEASE_FOLDER tmpRelease = child;
			if( tmpRelease )
			{
				if( release )
				{
					const THE_RELEASE &r1 = release->getRelease();
					const THE_RELEASE &r2 = tmpRelease->getRelease();
					if( releaseCompare( r1, r2 ) < 0 )
						release = tmpRelease;
				}
				else
				{
					release = tmpRelease;
				}
			}
			else if( recursive )
			{
				PTR_SOURCE_FOLDER folder = child;
				if( folder )
				{
					folder->refresh();
				}
			}
		}
	}

	if( recursive && release )
	{
		release->refresh();
	}

	StatusForm->restore();

	return false;
}

bool THE_FOLDER_REF::refresh( bool recursive, ostream *stream )
{
	doEnterFunction( "THE_FOLDER_REF::refresh" );
	bool	hasChanged = false;

	F_STRING localPath = getDownloadPath();
	if( localPath.isEmpty() )
	{
/*@*/	throw Exception( "Don't know local path" );
	}

	DirectoryList	dirContent;
	ITEM_CONTENT	*theContent = getContent();

	dirContent.dirlist( localPath );

	for(
		DirectoryList::iterator it=dirContent.begin(), endIT = dirContent.end();
		it != endIT && !StatusForm->isTerminated();
		++it
	)
	{
		const DirectoryEntry	&dirEntry = *it;
		F_STRING				fileFound = dirEntry.fileName;
		F_STRING				filePath = localPath + fileFound;
		PTR_ITEM	child = getContentItem( fileFound );

		StatusForm->pushStatus( "Refreshing", fileFound );
#ifndef _DEBUG
		if( !recursive && StatusForm->waitForUserSleep( 60000 ) )
			break;
#endif
		if( child )
		{
			if( !dirEntry.directory )
			{
				gak::DateTime	localTime = dirEntry.modifiedDate;
				gak::DateTime	childDate = child->getModifiedDate();

				if( localTime != childDate )
				{
					ImageMetaData	metaData;
					PTR_FILE_REF 	theFileRef = child;
					const STRING &oldMD5CheckSum = theFileRef->getMD5CheckSum();
					STRING newMD5CheckSum = TDocManDataModule::md5file( filePath );
					if( oldMD5CheckSum != newMD5CheckSum )
					{
						theFileRef->updateImageMetaData( &metaData );
						theFileRef->setMd5CheckSum( newMD5CheckSum );
						if( stream )
							*stream << "File MD5 Changed: " << theFileRef->getPath() << '\n';
					}
					theFileRef->setCreatedDate( dirEntry.creationDate.calcOriginalTime() );
					theFileRef->setModifiedDate( dirEntry.modifiedDate.calcOriginalTime() );
					theFileRef->updateDatabase();

					if( stream )
						*stream << "File Updated: " << theFileRef->getPath() << '\n';

					hasChanged = true;
				}
			}
		}
		else
		{
			if( !dirEntry.directory )
			{
				ImageMetaData	metaData;
				PTR_FILE_REF	newFile = createItem( TYPE_FILE_REF );
				newFile->setData(
					this,
					fileFound,
					"",
					TDocManDataModule::md5file( filePath ),
					dirEntry.creationDate.calcOriginalTime(),
					dirEntry.modifiedDate.calcOriginalTime()
				);
				doLogValue( dirEntry.modifiedDate.getOriginalTime() );
				newFile->updateDatabase();
				newFile->updateImageMetaData( &metaData );

				if( stream )
					*stream << "File Created: " << newFile->getPath() << '\n';
				hasChanged = true;
			}
			else if( dirEntry.directory
			&& fileFound != "."
			&& fileFound != ".." )
			{
				PTR_FOLDER_REF newFolder = createItem( TYPE_FOLDER_REF );
				newFolder->setData(
					this,
					fileFound,
					"",
					dirEntry.creationDate.calcOriginalTime(),
					dirEntry.modifiedDate.calcOriginalTime()
				);
				newFolder->updateDatabase();

				newFolder->refresh();
				if( stream )
					*stream << "Folder Created: " << newFolder->getPath() << '\n';
				hasChanged = true;
			}
		}

		StatusForm->restore();
	}

	for(
		size_t i=0;
		i<theContent->size() && !StatusForm->isTerminated();
		i++
	)
	{
		PTR_ITEM child = getContentItem( i );
		StatusForm->pushStatus( "Refreshing", child->getName() );
#ifndef _DEBUG
		if( !recursive && StatusForm->waitForUserSleep( 60000 ) )
			break;
#endif

		PTR_FOLDER_REF theFolder = child;
		if( child )
		{
			STRING 					itemFound = child->getName();
			const DirectoryEntry	*dirEntry = dirContent.findElement(itemFound);
			if( !dirEntry )
			{
				child->purgeItem();

				if( stream )
					*stream << "Deleted: " << child->getPath() << '\n';
				hasChanged = true;
			}
			else if( recursive && theFolder )
			{
				gak::DateTime	localTime = dirEntry->modifiedDate;
				gak::DateTime	childDate = child->getModifiedDate();

				hasChanged = theFolder->refresh() || hasChanged;
				if( localTime != childDate )
				{
					theFolder->setCreatedDate( dirEntry->creationDate.calcOriginalTime() );
					theFolder->setModifiedDate( dirEntry->modifiedDate.calcOriginalTime() );
					theFolder->updateDatabase();

					if( stream )
						*stream << "Folder Date Changed: " << theFolder->getPath() << '\n';
					hasChanged = true;
				}
			}
		}
		StatusForm->restore();
	}

	if( hasChanged )
		clearContent();

	return hasChanged;
}

void THE_FOLDER_REF::updateDatabase( void )
{
	TDateTime	createdDate = getCreatedDate();
	TDateTime	modifiedDate = getModifiedDate();

	THE_ITEM::updateDatabase();

	std::auto_ptr<TQuery>	theQuery( new TQuery( NULL ) );
	theQuery->DatabaseName = "docManDB";

	theQuery->SQL->Add(
		"update item_tree "
		"set createdDate = :createdDate, "
			"modifiedDate = :modifiedDate "
		"where id=:Id"
	);
	theQuery->Params->Items[0]->AsDateTime = createdDate;
	theQuery->Params->Items[1]->AsDateTime = modifiedDate;
	theQuery->Params->Items[2]->AsInteger = getID();

	theQuery->ExecSQL();
}

TGraphic *THE_FOLDER_REF::getStatusPicture( void ) const
{
	static Graphics::TBitmap *thePic = NULL;

	if( !thePic )
	{
		thePic = new Graphics::TBitmap();
		thePic->LoadFromResourceName((unsigned int)HInstance, "FOLDER_REF_BITMAP");
	}
	return thePic;
}

TGraphic *THE_LOCAL_FOLDER::getStatusPicture( void ) const
{
	return NULL;
}

void THE_SYNC_FOLDER::getItemFactories( Array<const FACTORY_BASE*> *factory ) const
{
	factory->clear();
}

void THE_SOURCE_FOLDER::getItemFactories( Array<const FACTORY_BASE*> *factory ) const
{
	THE_FOLDER::getItemFactories( factory );
}

void THE_FOLDER_REF::getItemFactories( Array<const FACTORY_BASE*> *factory ) const
{
	factory->clear();
}

void THE_XSLT_FOLDER::getItemFactories( Array<const FACTORY_BASE*> *factory ) const
{
	factory->clear();
	if( getParentID() > 0 )
	{
		factory->addElement( getItemFactory( TYPE_DOCUMENT ) );
	}
}

int THE_SOURCE_FOLDER::getColCount( void ) const
{
	return 8;
}

int *THE_SOURCE_FOLDER::getColWidth( void ) const
{
	static int colWidth[] =
	{
		50,
		50,
		0,
		80,
		120,
		80,
		80,
		50,
	};
	return colWidth;
}

COL_TITLE *THE_SOURCE_FOLDER::getColumnTitles( void ) const
{
	static COL_TITLE	colTitles[] =
	{
		{ "Order", SORT_ORDER },
		{ "Type", SORT_TYPE },
		{ "Name", SORT_NAME },
		{ "Ext", SORT_EXTENSION },
		{ "Modified", SORT_DATE },
		{ "Size", SORT_SIZE },
		{ "Status", SORT_STATUS },
		{ "Links", SORT_NUM_LINKS },
	};

	return colTitles;
}

void THE_SOURCE_FOLDER::sort( void )
{
	getContent()->sort( itemCompare, getSortType() );
}

STRING THE_SOURCE_FOLDER::drawCell( int col, int row, TCanvas *canvas, TRect &Rect )
{
	STRING text;

	if( col == 6 )
	{
		PTR_FILE_BASE	theItem = getContentItem( row );
		if( theItem )
			text = theItem->calcStatus( false );
	}
	else if( col == 7 )
		text = THE_LOCAL_FOLDER::drawCell( 6, row, canvas, Rect );
	else
		text = THE_LOCAL_FOLDER::drawCell( col, row, canvas, Rect );

	return text;
}

bool ACTION_REFRESH::acceptItem( THE_ITEM *theItem )
{
	THE_FOLDER_REF *theFolder = dynamic_cast<THE_FOLDER_REF *>(theItem);
	return theFolder ? ACTION_BASE::acceptItem( theItem ) : false;
}

const char *ACTION_REFRESH::getLabel( void ) const
{
	return "Refresh";
}

const char *THREAD_REFRESHING::getTitle( void ) const
{
	return "Refresh";
}

void THREAD_REFRESHING::perform( void )
{
	doEnterFunction("THREAD_REFRESHING::perform");

	PTR_FOLDER_REF theFolder = theItemToHandle;
	if( theFolder )
	{
		theFolder->refresh();
	}
}

REFRESH_TYPE ACTION_REFRESH::perform( PTR_ITEM theItem )
{
	PTR_FOLDER_REF theFolder = theItem;
	if( theFolder )
	{
		theFolder->getContent();
		THREAD_REFRESHING *theThread = new THREAD_REFRESHING( theItem );
		theThread->StartThread();
		return REFRESH_RELOAD;
	}

	return REFRESH_NONE;
}

bool ACTION_IMPORT::acceptItem( THE_ITEM *theItem )
{
	THE_SOURCE_FOLDER *theFolder = dynamic_cast<THE_SOURCE_FOLDER *>(theItem);
	if( !theFolder )
/*@*/	return false;

	int perms = theItem->getUserPermissions();
	return perms & ITEM_PERM_ADD_ITEMS
		 ? ACTION_BASE::acceptItem( theItem )
		 : false
	;
}

const char *ACTION_IMPORT::getLabel( void ) const
{
	return "Import";
}

const char *THREAD_IMPORTING::getTitle( void ) const
{
	return "Import";
}

void THREAD_IMPORTING::perform( void )
{
	THE_SOURCE_FOLDER *theFolder = dynamic_cast<THE_SOURCE_FOLDER *>(
		static_cast<THE_ITEM*>(theItemToHandle)
	);
	if( theFolder )
		theFolder->import();
}

REFRESH_TYPE ACTION_IMPORT::perform( PTR_ITEM theItem )
{
	THE_SOURCE_FOLDER *theFolder = dynamic_cast<THE_SOURCE_FOLDER *>(
		static_cast<THE_ITEM*>(theItem)
	);
	if( theFolder )
	{
		THREAD_IMPORTING *theThread = new THREAD_IMPORTING( theItem );
		theThread->StartThread();
		return REFRESH_RELOAD;
	}

	return REFRESH_NONE;
}

bool ACTION_EXPLORER::acceptItem( THE_ITEM *theItem )
{
	THE_FOLDER_REF *theFolder = dynamic_cast<THE_FOLDER_REF *>(theItem);
	return theFolder ? ACTION_BASE::acceptItem( theItem ) : false;
}

const char *ACTION_EXPLORER::getLabel( void ) const
{
	return "Explorer";
}

REFRESH_TYPE ACTION_EXPLORER::perform( PTR_ITEM theItem )
{
	STRING	downloadPath = theItem->getDownloadPath();
	if( !downloadPath.isEmpty() )
	{
		ShellExecute( NULL, NULL, downloadPath, NULL, NULL, SW_SHOWDEFAULT );
	}
	else
	{
		throw Exception( "Don't know local path" );
	}

	return REFRESH_NONE;
}

bool ACTION_LOCK_TREE::acceptItem( THE_ITEM *theItem )
{
	if( !theItem->acceptChildType( TYPE_DOCUMENT ) )
		return false;

	int perms = theItem->getUserPermissions();
	if( !perms & ITEM_PERM_MODIFY )
/*@*/	return false;

	return ACTION_BASE_VERSIONS::acceptItem( theItem );
}
const char *ACTION_LOCK_TREE::getLabel( void ) const
{
	return "Lock Versions...";
}
REFRESH_TYPE ACTION_LOCK_TREE::perform( PTR_ITEM theItem )
{
	if( Application->MessageBox(
		"This will lock the latest version of all files.\n"
		"This cannot be undone. Continue?",
		"Lock",
		MB_ICONQUESTION|MB_YESNO
	) == IDYES )
	{
		THREAD_LOCK_TREE *theThread = new THREAD_LOCK_TREE( theItem );
		theThread->StartThread();
	}
	return REFRESH_NONE;
}

bool ACTION_UNLOCK_TREE::acceptItem( THE_ITEM *theItem )
{
	if( !theItem->acceptChildType( TYPE_DOCUMENT ) )
		return false;

	int perms = theItem->getUserPermissions();
	if( !perms & ITEM_PERM_MODIFY )
/*@*/	return false;

	return ACTION_BASE_VERSIONS::acceptItem( theItem );
}

const char *ACTION_UNLOCK_TREE::getLabel( void ) const
{
	return "Unlock Versions...";
}

REFRESH_TYPE ACTION_UNLOCK_TREE::perform( PTR_ITEM theItem )
{
	if( Application->MessageBox(
		"This will unlock all versions of all files.\n"
		"This cannot be undone. Continue?",
		"Unlock",
		MB_ICONQUESTION|MB_YESNO
	) == IDYES )
	{
		THREAD_UNLOCK_TREE *theThread = new THREAD_UNLOCK_TREE( theItem );
		theThread->StartThread();
	}
	return REFRESH_NONE;
}

const char *THREAD_LOCK_TREE::getTitle( void ) const
{
	return "Lock Tree";
}

void THREAD_LOCK_TREE::perform( void )
{
	(*theItemToHandle).lock();
}

const char *THREAD_UNLOCK_TREE::getTitle( void ) const
{
	return "Unlock Tree";
}

void THREAD_UNLOCK_TREE::perform( void )
{
	(*theItemToHandle).unlock();
}

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

void THE_SOURCE_FOLDER::import( void )
{
	F_STRING localPath = getDownloadPath();

	if( localPath.isEmpty() )
	{
		throw Exception( "Don't know local path" );
	}

	DirectoryList dirContent;
	dirContent.dirlist( localPath );
	ITEM_CONTENT *theContent = getContent();

	for(
		DirectoryList::iterator	it = dirContent.begin(), endIT = dirContent.end();
		it != endIT && !StatusForm->isTerminated();
		++it
	)
	{
		const DirectoryEntry &dirEntry = *it;
		F_STRING	fileFound = dirEntry.fileName;
		F_STRING	filePath = localPath + fileFound;

		StatusForm->pushStatus( "Importing", filePath );

		bool		fileInDb = false;

		for(
			size_t j=0;
			!fileInDb && j<theContent->size() && !StatusForm->isTerminated();
			++j
		)
		{
			PTR_ITEM	child = getContentItem( j );
			CI_STRING	itemFound = (*child).getName();

			if( itemFound == fileFound )
				fileInDb = true;
		}

		if( !fileInDb )
		{
			if( !dirEntry.directory )
			{
				PTR_FILE	newFile = createItem( TYPE_DOCUMENT );
				newFile->setData(
					this,
					fileFound,
					"",
					"" // filePath
				);
				newFile->reserve();
				newFile->updateDatabase();
			}
			else if( dirEntry.directory
			&& fileFound != "."
			&& fileFound != ".." )
			{
				PTR_ITEM newItem = createItem( TYPE_SOURCE_FOLDER );
				THE_SOURCE_FOLDER *newFolder = static_cast<THE_SOURCE_FOLDER*>(
					static_cast<THE_ITEM*>( newItem )
				);
				newFolder->setData(
					this,
					fileFound,
					"",
					""
				);
				newFolder->updateDatabase();

				newFolder->import();
			}
		}
		StatusForm->restore();
	}
}

void THE_SOURCE_FOLDER::checkIn( bool doBranch, const STRING &description )
{
	if( StatusForm->pushStatus( "Test", getName() ) )
/*@*/	return;
	
	STRING localPath = getDownloadPath();

	if( localPath.isEmpty() )
		throw Exception( "Don't know local path" );

	ITEM_CONTENT *theContent = getContent();

	for(
		size_t i=0;
		!StatusForm->isTerminated() && i<theContent->size();
		i++
	)
	{
		PTR_ITEM child = getContentItem( i );

		PTR_FILE file = child;
		if( file )
		{
			if( file->canUnreserve( true ) )
			{
				StatusForm->pushStatus( "Check In", file->getName() );
				file->unreserve( doBranch, description );
				file->updateDatabase();
				StatusForm->restore();
			}
		}
		else
		{
			PTR_SOURCE_FOLDER folder = child;
			if( folder )
			{
				folder->checkIn( doBranch, description );
			}
		}
	}

	StatusForm->restore();
}

void THE_SOURCE_FOLDER::reserve( int taskID )
{
	if( StatusForm->pushStatus( "Test", getName() ) )
/*@*/	return;
	
	STRING localPath = getDownloadPath();

	if( localPath.isEmpty() )
		throw Exception( "Don't know local path" );

	ITEM_CONTENT *theContent = getContent();

	for(
		size_t i=0;
		!StatusForm->isTerminated() && i<theContent->size();
		i++
	)
	{
		PTR_ITEM child = getContentItem( i );

		PTR_FILE file = child;
		if( file )
		{
			if( file->canReserve() )
			{
				StatusForm->pushStatus( "Check Out", file->getName() );
				file->reserve( taskID );
				file->updateDatabase();
				StatusForm->restore();
			}
		}
		else
		{
			PTR_SOURCE_FOLDER folder = child;
			if( folder )
			{
				folder->reserve( taskID );
			}
		}
	}

	StatusForm->restore();
}

void THE_LOCAL_FOLDER::compare( FolderCompareList *iTheList )
{
	FolderCompareList	theList;

	ITEM_CONTENT		*theContent = getContent();
	bool				hasFile = false;
	bool				releaseCompared = false;
	bool				compareFiles = true;
	F_STRING			localPath = getDownloadPath();
	PTR_RELEASE_FOLDER	release;

	if( localPath.isEmpty() )
	{
		compareFiles = false;
	}

	for(
		ITEM_CONTENT::const_iterator it = theContent->cbegin(),
			endIT = theContent->cend();
		it != endIT;
		++it
	)
	{
		const PTR_ITEM	&theChild = *it;
		PTR_FILE_BASE	theFile = theChild;
		if( theFile )
		{
			if( compareFiles )
			{
				/*
					check repository files against local filesystem
				*/
				hasFile = true;
				F_STRING	fileFound = theFile->getName();
				F_STRING	filePath = localPath + fileFound;

				FolderCompareEntry	&theEntry = theList[fileFound];
				theEntry.theParent = this;
				theEntry.filePath = filePath;
				theEntry.dbSize = theFile->getSizeInBytes();
				theEntry.dbModTime = theFile->getFileModifiedDate();
				theEntry.theFile = theFile;
				if( theFile->getReservedBy() )
				{
					theEntry.statusSTR = "Reserved - ";
					theEntry.reserved = true;
				}
			}
		}
		else
		{
			/*
				compare sub folder
			*/
			PTR_RELEASE_FOLDER tmpRelease = theChild;
			if( tmpRelease )
			{
				if( release )
				{
					const THE_RELEASE &r1 = release->getRelease();
					const THE_RELEASE &r2 = tmpRelease->getRelease();
					if( releaseCompare( r1, r2 ) < 0 )
					{
						release = tmpRelease;
					}
				}
				else
				{
					release = tmpRelease;
				}
			}
			else
			{
				PTR_LOCAL_FOLDER theFolder = theChild;
				if( theFolder )
				{
					theFolder->compare( iTheList );
				}
			}
		}
	}

	if( release )
	{
		release->compare( iTheList );
		releaseCompared = true;
	}

	if( compareFiles && (hasFile || !releaseCompared) )
	{
		DirectoryList	dirContent;

		dirContent.dirlist( localPath );

		for(
			DirectoryList::iterator	it = dirContent.begin(), endIT = dirContent.end();
			it != endIT && !StatusForm->isTerminated();
			++it
		)
		{
			/*
				check local filesystem against repository
			*/
			F_STRING	fileFound = it->fileName;
			F_STRING	filePath = localPath + fileFound;
			if( isFile( filePath ) )
			{
				struct stat	statBuf;

				strStat( filePath, &statBuf );

				FolderCompareEntry	&theEntry = theList[fileFound];
				theEntry.theParent = this;
				theEntry.filePath = filePath;
				theEntry.inFS = true;
				theEntry.localSize = statBuf.st_size;
				theEntry.localModTime = vcl::EncodeDateTime( statBuf.st_mtime );
			}
		}

		/*
			calculate the status
		*/
		for( int i=theList.size()-1; i>=0; i-- )
		{
			FolderCompareEntry	&theEntry = theList.getElementAt( i );

			double difference = (double(theEntry.localModTime) - double(theEntry.dbModTime))*3600.0*24.0;
			if( abs( difference ) <= 3 )
			{
				difference = 0;
			}

			if( theEntry.folder )
			{
				theEntry.status = COMPARE_FOLDER;
				theEntry.statusSTR += "Folder";
			}
			else if( !theEntry.theFile )
			{
				theEntry.status = DB_MISSING;
				theEntry.statusSTR += "Not in repository";
			}
			else if( !theEntry.inFS )
			{
				theEntry.status = LOCAL_MISSING;
				theEntry.statusSTR += "Not in local file system";
			}
			else if( difference < 0  )
			{
				theEntry.status = LOCAL_OLDER;
				theEntry.statusSTR += "Older";
			}
			else if( difference > 0 )
			{
				theEntry.status = LOCAL_NEWER;
				theEntry.statusSTR += "Newer";
			}
			else
			{
				theList.removeElementAt( i );
			}
		}
		if( theList.size() )
		{
			STRING				dmPath = getPath();
			FolderCompareEntry	&theFolderEntry = (*iTheList)[dmPath];

			theFolderEntry.folder = true;
			theFolderEntry.status = COMPARE_FOLDER;
			theFolderEntry.statusSTR += "Folder";

			iTheList->addElements( theList );
		}
	}
}

STRING THE_FOLDER::generateWebFolder( bool forWebServer, STRING &mimeType )
{
	STRING			exportedFile;
	PTR_WEB_FOLDER	webParent( this );

	if( webParent )
		exportedFile = (*webParent).generateWebFolder(
			this, forWebServer, mimeType
		);
	else
	{
		for(
			PTR_ITEM parent = getParent();
			(THE_ITEM*)parent;
			parent = (*parent).getParent()
		 )
		{
			webParent = parent;
			if( webParent )
			{
				exportedFile = (*webParent).generateWebFolder(
					this, forWebServer, mimeType
				);
/*v*/			break;
			}
		}
	}

	return exportedFile;
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -a.
#	pragma option -p.
#endif

