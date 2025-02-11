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

#include <io.h>
#include <dos.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <gak/io.h>
#include <gak/md5.h>
#include <gak/vcl_tools.h>
#include <gak/strFiles.h>
#include <gak/fcopy.h>

#include "File.h"
#include "ItemManager.h"
#include "FileCreateFrm.h"
#include "Folder.h"
#include "WebFolder.h"
#include "PermissionsFrm.h"
#include "StatusFrm.h"
#include "MimeTypesFrm.h"
#ifndef DOCMANBG
#include "LanguageDocCreateFrm.h"
#include "LanguageItemCreateFrm.h"
#endif
#include "ActionManager.h"
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

class FACTORY_FILE_REF : private FACTORY_BASE
{
	public:
	virtual int getItemType( void ) const;
	virtual bool acceptParent( const THE_ITEM *parent ) const;
	virtual PTR_ITEM createItem( int id ) const;
	virtual const char *getName( void ) const;
	virtual TItemCreateForm *getForm( void ) const;
	virtual PTR_ITEM createItemFromForm( const PTR_ITEM &parent ) const;
};

class FACTORY_LANGUAGE_DOC : private FACTORY_BASE
{
	public:
	virtual int getItemType( void ) const;
	virtual bool acceptParent( const THE_ITEM *parent ) const;
	virtual PTR_ITEM createItem( int id ) const;
	virtual const char *getName( void ) const;
	virtual TItemCreateForm *getForm( void ) const;
	virtual PTR_ITEM createItemFromForm( const PTR_ITEM &parent ) const;
};

class FACTORY_LANGUAGE_ITEM : private FACTORY_BASE
{
	public:
	virtual int getItemType( void ) const;
	virtual bool acceptParent( const THE_ITEM *parent ) const;
	virtual PTR_ITEM createItem( int id ) const;
	virtual const char *getName( void ) const;
	virtual TItemCreateForm *getForm( void ) const;
	virtual PTR_ITEM createItemFromForm( const PTR_ITEM &parent ) const;
};

class ACTION_DOWNLOAD : private ACTION_BASE_PROPERTIES
{
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
};

class ACTION_BRANCH_FILE : public ACTION_BASE_MOVE
{
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
};

class ACTION_GENERATE : public ACTION_BASE_MOVE
{
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

const char STATUS_OK[] = "OK";
const char STATUS_NEWER[] = "Newer";
const char STATUS_OLDER[] = "Older";
const char STATUS_MISSING[] = "Missing";
const char STATUS_RESERVED[] = "Reserved";

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static FACTORY_FILE				theFileFactory;
static FACTORY_FILE_REF			theFileRefFactory;
static FACTORY_LANGUAGE_DOC		theLanguageDocFactory;
static FACTORY_LANGUAGE_ITEM	theLanguageItemFactory;
static ACTION_DOWNLOAD			theDownloadAction;
static ACTION_BRANCH_FILE		theBranchAction;
static ACTION_GENERATE			theGenerateAction;

// --------------------------------------------------------------------- //
// ----- class static data --------------------------------------------- //
// --------------------------------------------------------------------- //

STRING THE_FILE::s_externalStorage;

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

THE_FILE_VERSION::THE_FILE_VERSION( int fileID, int version )
{
	doEnterFunctionEx( gakLogging::llInfo, "THE_FILE_VERSION::THE_FILE_VERSION(int fileID, int version)" );

	std::auto_ptr<TQuery>	theQuery( new TQuery( NULL ) );
	theQuery->DatabaseName = "docManDB";

	if( !version )
	{
		theQuery->SQL->Add( "select version from i_files where id=:id" );
		theQuery->Params->Items[0]->AsInteger = fileID;
		theQuery->Open();
		if( !theQuery->Eof )
			version = theQuery->Fields->Fields[0]->AsInteger;
		theQuery->Close();

		theQuery->SQL->Clear();
	}
	this->version = version;

	theQuery->SQL->Add(
		"select fv.id, fv.mime_type, fv.filename, fv.filesize, fv.storage_id, "
			"fv.createdDate, fv.modifiedDate, "
			"fv.fileCreatedDate, fv.fileModifiedDate, "
			"s.file_path, s.usage_count, fv.locked "
		"from i_file_vers fv, i_storage s "
		"where fv.file_id = :id and fv.version = :fileVersion "
		"and s.id = fv.storage_id "
	);
	theQuery->Params->Items[0]->AsInteger = fileID;
	theQuery->Params->Items[1]->AsInteger = version;
	theQuery->Open();
	if( !theQuery->Eof )
	{
		versionID = theQuery->Fields->Fields[0]->AsInteger;
		mimeType = theQuery->Fields->Fields[1]->AsString.c_str();
		fileName = theQuery->Fields->Fields[2]->AsString.c_str();
		size = theQuery->Fields->Fields[3]->AsInteger;
		storageID = theQuery->Fields->Fields[4]->AsInteger;
		versionCreatedDate = theQuery->Fields->Fields[5]->AsDateTime;
		versionModifiedDate = theQuery->Fields->Fields[6]->AsDateTime;
		fileCreatedDate = theQuery->Fields->Fields[7]->AsDateTime;
		fileModifiedDate = theQuery->Fields->Fields[8]->AsDateTime;
		storagePath = theQuery->Fields->Fields[9]->AsString.c_str();
		storageUsageCount = theQuery->Fields->Fields[10]->AsInteger;
		locked = theQuery->Fields->Fields[11]->AsInteger;
	}
	else
	{
		versionID = 0;
		size = 0;
		storageID = 0;
		versionCreatedDate = 0;
		versionModifiedDate = 0;
		fileCreatedDate = 0;
		fileModifiedDate = 0;
		storageUsageCount = 0;
		locked = 0;
	}
	theQuery->Close();
}

THE_FILE::~THE_FILE()
{
	clearLatestVersion();
}

// --------------------------------------------------------------------- //
// ----- class static functions ---------------------------------------- //
// --------------------------------------------------------------------- //

STRING THE_FILE_BASE::getTempDownloadPath( void )
{
	STRING downloadPath = getenv( "TEMP" );
	size_t	len = downloadPath.strlen();
	if( !len || downloadPath[len-1] != DIRECTORY_DELIMITER )
		downloadPath += DIRECTORY_DELIMITER;

	return downloadPath;
}

// --------------------------------------------------------------------- //
// ----- class privates ------------------------------------------------ //
// --------------------------------------------------------------------- //

STRING THE_FILE::getStoragePath( int theVersionNum )
{
	if( theVersionNum )
	{
		std::auto_ptr<THE_FILE_VERSION> theVersion( getVersion( theVersionNum ) );
		STRING storagePath = theVersion->getStoragePath();

		return storagePath;
	}
	else
		return getLatestVersion()->getStoragePath();
}

int THE_FILE::createStorage( const STRING &filePath )
{
	if( exists( filePath ) )
	{
		STRING	destinationPath = getExternalStorageBase();
		char	file[16];
		int storageId = ConfigDataModule->getNewMaxValue(
			"I_STORAGE", "ID"
		);
		sprintf( file, "%08d.dat", storageId );

		for( size_t i=0;i<2;i++ )
			destinationPath += file[i];
		mkdir( destinationPath );
		destinationPath += DIRECTORY_DELIMITER;
		for( size_t i=2;i<4;i++ )
			destinationPath += file[i];
		mkdir( destinationPath );
		destinationPath += DIRECTORY_DELIMITER;
		for( size_t i=4;i<6;i++ )
			destinationPath += file[i];
		mkdir( destinationPath );
		destinationPath += DIRECTORY_DELIMITER;
		destinationPath += file;

		fcopy( filePath, destinationPath );

		destinationPath += getExternalStorageBase().strlen();

		STRING		md5base64 = TDocManDataModule::md5file( filePath );
 
		std::auto_ptr<TQuery>	theQuery( new TQuery( NULL ) );
		theQuery->DatabaseName = "docManDB";

		theQuery->SQL->Add(
			"insert into I_STORAGE "
			"( id, usage_count, file_path, md5_checksum ) "
			"values"
			"( :id, 1, :destinationPath, :md5base64 )"
		);

		theQuery->Params->Items[0]->AsInteger = storageId;
		theQuery->Params->Items[1]->AsString = (const char *)destinationPath;
		theQuery->Params->Items[2]->AsString = (const char *)md5base64;

		theQuery->ExecSQL();

		return storageId;
	}
	else
	{
		STRING errorMsg = "File ";
		errorMsg += filePath;
		errorMsg += " does not exists";

		throw Exception( (const char *)errorMsg );
	}
}

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //

int FACTORY_FILE::getItemType( void ) const
{
	return (int)TYPE_DOCUMENT;
}

int FACTORY_LANGUAGE_DOC::getItemType( void ) const
{
	return (int)TYPE_LANGUAGE_DOC;
}

int FACTORY_LANGUAGE_ITEM::getItemType( void ) const
{
	return (int)TYPE_LANGUAGE_ITEM;
}

int FACTORY_FILE_REF::getItemType( void ) const
{
	return (int)TYPE_FILE_REF;
}

bool FACTORY_FILE::acceptParent( const THE_ITEM *parent ) const
{
	const THE_FOLDER *theFolder = dynamic_cast<const THE_FOLDER *>(parent);
	if( theFolder )
		return true;
	const THE_PUBLIC_TEMPLATE_FOLDER *thePublicTemplateFolder =
		dynamic_cast<const THE_PUBLIC_TEMPLATE_FOLDER *>(parent)
	;
	if( thePublicTemplateFolder )
		return true;
	const THE_PERSONAL_TEMPLATE_FOLDER *thePersonalTemplateFolder =
		dynamic_cast<const THE_PERSONAL_TEMPLATE_FOLDER *>(parent)
	;
	if( thePersonalTemplateFolder )
		return true;

	return false;
}

bool FACTORY_FILE_REF::acceptParent( const THE_ITEM *parent ) const
{
	const THE_SYNC_FOLDER *theSyncFolder = dynamic_cast<const THE_SYNC_FOLDER *>(parent);
	if( theSyncFolder )
		return true;

	const THE_LOCAL_FOLDER *theLocalFolder = dynamic_cast<const THE_LOCAL_FOLDER *>(parent);
	if( theLocalFolder )
		return false;

	const THE_FOLDER_REF *theFolderRef = dynamic_cast<const THE_FOLDER_REF *>(parent);
	if( theFolderRef )
		return true;

	return false;
}

bool FACTORY_LANGUAGE_DOC::acceptParent( const THE_ITEM *parent ) const
{
	const THE_FOLDER *theFolder = dynamic_cast<const THE_FOLDER *>(parent);
	return theFolder ? true : false;
}

bool FACTORY_LANGUAGE_ITEM::acceptParent( const THE_ITEM *parent ) const
{
	const THE_LANGUAGE_DOC *theDoc = dynamic_cast<const THE_LANGUAGE_DOC *>(parent);
	return theDoc ? true : false;
}

PTR_ITEM FACTORY_FILE::createItem( int id ) const
{
	return new THE_FILE( id, this );
}

PTR_ITEM FACTORY_FILE_REF::createItem( int id ) const
{
	return new THE_FILE_REF( id, this );
}

PTR_ITEM FACTORY_LANGUAGE_DOC::createItem( int id ) const
{
	return new THE_LANGUAGE_DOC( id, this );
}

PTR_ITEM FACTORY_LANGUAGE_ITEM::createItem( int id ) const
{
	return new THE_LANGUAGE_ITEM( id, this );
}

const char *FACTORY_FILE::getName( void ) const
{
	return "File";
}

const char *FACTORY_FILE_REF::getName( void ) const
{
	return "File Reference";
}

const char *FACTORY_LANGUAGE_DOC::getName( void ) const
{
	return "Multi Language Doc";
}

const char *FACTORY_LANGUAGE_ITEM::getName( void ) const
{
	return "Translation";
}

TItemCreateForm *FACTORY_FILE::getForm( void ) const
{
#ifdef DOCMANBG
	return NULL;
#else
	return FileCreateForm;
#endif
}

TItemCreateForm *FACTORY_FILE_REF::getForm( void ) const
{
	return ItemCreateForm;
}

TItemCreateForm *FACTORY_LANGUAGE_DOC::getForm( void ) const
{
#ifdef DOCMANBG
	return NULL;
#else
	return LanguageDocCreateForm;
#endif
}

TItemCreateForm *FACTORY_LANGUAGE_ITEM::getForm( void ) const
{
#ifdef DOCMANBG
	return NULL;
#else
	return LanguageItemCreateForm;
#endif
}

PTR_ITEM FACTORY_FILE::createItemFromForm( const PTR_ITEM &parent ) const
{
	PTR_FILE					newFile;
#ifndef DOCMANBG
	THE_ITEM::ItemsCopiedMap	itemsCopied;
	PTR_FILE					source = FileCreateForm->getTemplate();

	if( !source )
	{
		newFile = createItem( 0 );
		newFile->setData(
			parent,
			FileCreateForm->EditName->Text.c_str(),
			FileCreateForm->MemoDescription->Text.c_str(),
			FileCreateForm->EditFilePath->Text.c_str()
		);
	}
	else
	{
		newFile = source->copy(
			itemsCopied, parent, false, FileCreateForm->EditName->Text.c_str()
		);
		newFile->setData(
			parent,
			FileCreateForm->EditName->Text.c_str(),
			FileCreateForm->MemoDescription->Text.c_str(),
			""
		);
	}
	if( FileCreateForm->CheckBoxReserve->Checked )
		newFile->reserve();

	newFile->updateDatabase();
#endif
	return newFile;
}

PTR_ITEM FACTORY_FILE_REF::createItemFromForm( const PTR_ITEM &parent ) const
{
	THE_FILE_REF	*newFile = new THE_FILE_REF( 0, this );
	newFile->setData(
		parent,
		ItemCreateForm->EditName->Text.c_str(),
		ItemCreateForm->MemoDescription->Text.c_str(),
		"",
		DateTime(),
		DateTime()
	);

	newFile->updateDatabase();

	return newFile;
}

PTR_ITEM FACTORY_LANGUAGE_DOC::createItemFromForm( const PTR_ITEM &parent ) const
{
	THE_LANGUAGE_DOC *newDoc = new THE_LANGUAGE_DOC( 0, this );
#ifndef DOCMANBG
	newDoc->setData(
		parent,
		LanguageDocCreateForm->EditName->Text.c_str(),
		LanguageDocCreateForm->MemoDescription->Text.c_str()
	);
#endif
	newDoc->updateDatabase();
#ifndef DOCMANBG
	PTR_ITEM newItem = theLanguageItemFactory.createItem( 0 );
	THE_LANGUAGE_ITEM *newLI = static_cast<THE_LANGUAGE_ITEM *>(
		static_cast<THE_ITEM*>(newItem)
	);
	newLI->setData(
		newDoc,
		LanguageDocCreateForm->EditDefaultLanguage->Text.c_str(),
		LanguageDocCreateForm->MemoDescription->Text.c_str(),
		LanguageDocCreateForm->EditFilePath->Text.c_str()
	);
	if( LanguageDocCreateForm->CheckBoxReserve->Checked )
		newLI->reserve();

	newLI->updateDatabase();
#endif
	return newDoc;
}

PTR_ITEM FACTORY_LANGUAGE_ITEM::createItemFromForm( const PTR_ITEM &parent ) const
{
	THE_FILE	*newFile = new THE_FILE( 0, this );
#ifndef DOCMANBG
	newFile->setData(
		parent,
		LanguageItemCreateForm->EditName->Text.c_str(),
		LanguageItemCreateForm->MemoDescription->Text.c_str(),
		LanguageItemCreateForm->EditFilePath->Text.c_str()
	);
	if( LanguageItemCreateForm->CheckBoxReserve->Checked )
		newFile->reserve();
#endif
	newFile->updateDatabase();

	return newFile;
}

void THE_FILE_BASE::loadFields( TQuery *query )
{
	THE_ITEM::loadFields( query );

	reservedOn = query->FieldByName( "RESERVEDON" )->AsString.c_str();
	reservedBy = query->FieldByName( "RESERVEDBY" )->AsInteger;
	reservedFor = query->FieldByName( "RESERVEDFOR" )->AsInteger;
	mimeType = query->FieldByName( "mime_type" )->AsString.c_str();
}

void THE_FILE::loadFields( TQuery *query )
{
	doEnterFunctionEx(gakLogging::llDetail, "THE_FILE::loadFields" );

	THE_FILE_BASE::loadFields( query );

	m_fileID = query->FieldByName( "FILE_ID" )->AsInteger;


	setFileModifiedDate( query->FieldByName( "fileModifiedDate" )->AsDateTime );
	setFileSize( query->FieldByName( "fileSize" )->AsInteger );

	m_previousFilePath = getDownloadFile( getParent() );
}

void THE_FILE_REF::loadFields( TQuery *query )
{
	doEnterFunctionEx(gakLogging::llDetail, "THE_FILE_REF::loadFields" );
	THE_ITEM::loadFields( query );

	m_md5CheckSum = query->FieldByName( "md5_checksum" )->AsString.c_str();

	STRING	localFile = getDownloadPath( getParent() );

	if( !localFile.isEmpty() )
	{
		struct stat localStat;
		strStat( localFile, &localStat );

		m_size = localStat.st_size;
	}
	m_previousFilePath = localFile;
	doLogValue(m_previousFilePath);
}

void THE_FILE_REF::updateDatabase( void )
{
	doEnterFunctionEx( gakLogging::llInfo, "THE_FILE_REF::updateDatabase" );
	TDateTime	createdDate = getCreatedDate();
	TDateTime	modifiedDate = getModifiedDate();

	THE_ITEM::updateDatabase();

	std::auto_ptr<TQuery>	theQuery( new TQuery( NULL ) );
	theQuery->DatabaseName = "docManDB";

	theQuery->SQL->Add(
		"update item_tree "
		"set md5_checksum = :md5CheckSum, "
			"createdDate = :createdDate, "
			"modifiedDate = :modifiedDate "
		"where id=:Id"
	);
	theQuery->Params->Items[0]->AsString = (const char *)m_md5CheckSum;
	theQuery->Params->Items[1]->AsDateTime = createdDate;
	theQuery->Params->Items[2]->AsDateTime = modifiedDate;
	theQuery->Params->Items[3]->AsInteger = getID();

	theQuery->ExecSQL();

	if( !m_previousFilePath.isEmpty() /* && fileName( &m_previousFilePath ) */ )
	{
		STRING localFile = getDownloadPath();
		if( localFile != m_previousFilePath )
		{
			doLogValue(m_previousFilePath);
			strRename( m_previousFilePath, localFile );
			m_previousFilePath = localFile;
			doLogValue(localFile);
			doLogValue(m_previousFilePath);
		}
	}
}

int THE_FILE::loadPermissions( void )
{
	int reservedBy;

	THE_ITEM::loadPermissions();
	if( !vcl::isSystemAdmin()
	&&  (reservedBy = getReservedBy()) != 0
	&&  reservedBy != vcl::getActUserID() )
	{
		setUserPermissions( ITEM_PERM_SEE );
	}

	return getUserPermissions();
}

void THE_FILE_BASE::updateDatabase( void )
{
	THE_ITEM::updateDatabase();

	std::auto_ptr<TQuery>	theQuery( new TQuery( NULL ) );
	theQuery->DatabaseName = "docManDB";

	theQuery->SQL->Add(
		"update item_tree "
		"set reservedBy = :userId, "
			"reservedFor = :reservedFor, "
			"reservedOn = :reservedOn "
		"where id=:theId"
	);

	theQuery->Params->Items[0]->AsInteger = reservedBy;
	theQuery->Params->Items[1]->AsInteger = reservedFor;
	theQuery->Params->Items[2]->AsString = (const char *)reservedOn;
	theQuery->Params->Items[3]->AsInteger = getID();

	theQuery->ExecSQL();
}

void THE_FILE::updateDatabase( void )
{
	THE_FILE_BASE::updateDatabase();

	std::auto_ptr<TQuery>	theQuery( new TQuery( NULL ) );
	theQuery->DatabaseName = "docManDB";

	if( !m_fileID )
	{
		m_fileID = ConfigDataModule->getNewMaxValue(
			"I_FILES", "ID"
		);
		theQuery->SQL->Add(
			"insert into I_FILES "
			"( id, usagecount, version ) "
			"values"
			"( :id, 1, 0 )"
		);

		theQuery->Params->Items[0]->AsInteger = m_fileID;

		theQuery->ExecSQL();
	}

	theQuery->SQL->Clear();
	theQuery->SQL->Add(
		"update item_tree "
		"set file_id = :theFileId "
		"where id=:theId"
	);

	theQuery->Params->Items[0]->AsInteger = m_fileID;
	theQuery->Params->Items[1]->AsInteger = getID();

	theQuery->ExecSQL();

	if( !m_newFilePath.isEmpty() && exists( m_newFilePath ) )
	{
		createVersion( m_newFilePath, "" );
	}
	else if( !m_previousFilePath.isEmpty() && fileName( &m_previousFilePath ) )
	{
		STRING localPath = getParent()->getDownloadPath();
		if( !localPath.isEmpty() )
		{
			STRING localFile = getDownloadFile( localPath );

			if( localFile != m_previousFilePath || !exists( localFile ) )
			{
				strRename( m_previousFilePath, localFile );
				m_previousFilePath = localFile;
			}
		}
	}

	m_newFilePath = (const char *)NULL;
}

TGraphic *THE_FILE_BASE::getStatusPicture( void ) const
{
	if( reservedBy )
	{
		static Graphics::TBitmap *thePic = NULL;

		if( !thePic )
		{
			thePic = new Graphics::TBitmap();
			thePic->LoadFromResourceName((unsigned int)HInstance, "RESERVED_BITMAP");
		}
		return thePic;
	}

	return NULL;
}

TGraphic *THE_FILE_BASE::getItemPicture( void ) const
{
	static Graphics::TBitmap *thePic = NULL;

	if( !thePic )
	{
		thePic = new Graphics::TBitmap();
		thePic->LoadFromResourceName((unsigned int)HInstance, "FILE_BITMAP");
	}
	return thePic;
}

TGraphic *THE_LANGUAGE_DOC::getItemPicture( void ) const
{
	static Graphics::TBitmap *thePic = NULL;

	if( !thePic )
	{
		thePic = new Graphics::TBitmap();
		thePic->LoadFromResourceName((unsigned int)HInstance, "LANGUAGE_DOC_BITMAP");
	}
	return thePic;
}

TGraphic *THE_FILE_REF::getItemPicture( void ) const
{
	static Graphics::TBitmap *thePic = NULL;

	if( !thePic )
	{
		thePic = new Graphics::TBitmap();
		thePic->LoadFromResourceName((unsigned int)HInstance, "FILE_BITMAP");
	}
	return thePic;
}

TGraphic *THE_FILE_REF::getStatusPicture( void ) const
{
	static Graphics::TBitmap *thePic = NULL;

	if( !thePic )
	{
		thePic = new Graphics::TBitmap();
		thePic->LoadFromResourceName((unsigned int)HInstance, "FOLDER_REF_BITMAP");
	}
	return thePic;
}

STRING THE_FILE::getSize( void )
{
	return formatNumber( getSizeInBytes() ) + " Bytes";
}

STRING THE_FILE_REF::getSize( void )
{
	return formatNumber( getSizeInBytes() ) + " Bytes";
}

void THE_FILE_BASE::open( void )
{
	openVersion( 0 );
}

void THE_FILE_REF::open( void )
{
	STRING	localFile = getDownloadPath();

	ShellExecute( NULL, NULL, localFile, NULL, NULL, SW_SHOWDEFAULT );
}

STRING THE_FILE::getVersionFileName( void )
{
	THE_FILE_VERSION *latestVersion = getLatestVersion();

	return latestVersion ? latestVersion->getFileName() : NULL_STRING;
}

STRING THE_FILE_BASE::getDownloadFile( const STRING &downloadPath )
{
	STRING	destFile = downloadPath;

	if( destFile.isEmpty() || !destFile.endsWith( DIRECTORY_DELIMITER ) )
		destFile += DIRECTORY_DELIMITER;
	destFile += getName();

	STRING myMimeType = getMimeType();
	STRING globalMimeType = MimeTypesForm->GetMimeType( destFile );

	if( myMimeType != globalMimeType )
	{
		STRING theExtension( getVersionFileName() );

		theExtension += theExtension.searchRChar( DIRECTORY_DELIMITER ) +1;
		theExtension += theExtension.searchRChar( '.' )+1;

		if( !destFile.endsWithI( theExtension ) )
		{
			destFile += '.';
			destFile += theExtension;
			globalMimeType = MimeTypesForm->GetMimeType( destFile );
		}

		if( myMimeType != globalMimeType )
		{
			theExtension = '.';
			theExtension += MimeTypesForm->GetExtension( myMimeType );
			if( theExtension != "." && !destFile.endsWithI( theExtension ) )
				destFile += theExtension;
		}
	}

	// Replace illegal charachters by underscores
	destFile.replaceChar( '*', '_' );
	destFile.replaceChar( '?', '_' );

	return destFile;
}

STRING THE_FILE_BASE::getDownloadPath( PTR_ITEM parent )
{
	doEnterFunctionEx(gakLogging::llDetail,"THE_FILE_BASE::getDownloadPath");

	STRING		downloadPath;

	if( !parent )
	{
		parent = getParent();
	}

	if( parent )
	{
		downloadPath = parent->getDownloadPath();
	}

	if( downloadPath.isEmpty() )
		downloadPath = getTempDownloadPath();

	return downloadPath;
}

STRING THE_LANGUAGE_DOC::getDownloadPath( PTR_ITEM parent )
{
	STRING		downloadPath;
	PTR_ITEM 	theParent = parent ? parent : getParent();

	if( theParent )
	{
		downloadPath = theParent->getDownloadPath();
	}
	if( downloadPath.isEmpty() )
	{
		downloadPath = THE_FILE_BASE::getTempDownloadPath();
	}
	return downloadPath;
}

STRING THE_LANGUAGE_ITEM::getDownloadFile( const STRING &downloadPath )
{
	STRING		destFile = downloadPath;

	if( destFile.isEmpty() || !destFile.endsWith( DIRECTORY_DELIMITER ) )
	{
		destFile += DIRECTORY_DELIMITER;
	}

	STRING	parentName = (*getParent()).getName();
	size_t	dotPos = parentName.searchRChar( '.' );
	if( dotPos != parentName.no_index )
	{
		parentName.insStr( dotPos, getName() );
	}
	else
	{
		parentName += getName();
	}

	destFile += parentName;

	STRING myMimeType = getMimeType();
	STRING globalMimeType = MimeTypesForm->GetMimeType( destFile );

	if( myMimeType != globalMimeType )
	{
		STRING extension = STRING('.');
		extension += MimeTypesForm->GetExtension( myMimeType );
		if( extension != "." && !destFile.endsWithI( extension ) )
		{
			destFile += extension;
		}
	}
	return destFile;
}

bool THE_FILE::canReserve( void ) const
{
	if( !getReservedBy() )
	{
		int perms = getUserPermissions();
		return (perms & ITEM_PERM_ADD_VERSION) ? true : false;
	}

	return false;
}

bool THE_FILE::canUnreserve( bool noAdminCheck ) const
{
	int	reservedBy = getReservedBy();

	if( !reservedBy )
/***/	return false;

	if( !noAdminCheck && vcl::isSystemAdmin() )
/***/	return true;

	if( reservedBy != vcl::getActUserID() )
/***/	return false;

	if( !getReservedOn().isEmpty() && getReservedOn() != TDocManDataModule::getMachine() )
/***/	return false;

	int perms = getUserPermissions();
	return (perms & ITEM_PERM_ADD_VERSION) ? true : false;
}

void THE_FILE::reserve( int reserveFor, bool doNotOverwrite )
{
	if( getID() && !canReserve() )
/*@*/	throw Exception( "Item cannot be reserved" );

	THE_FILE_BASE::reserve( reserveFor );

	STRING		downloadPath;
	PTR_ITEM	theParent = getParent();

	if( theParent )
		downloadPath = theParent->getDownloadPath();

	if( !downloadPath.isEmpty() )
	{
		THE_FILE_VERSION *latestVersion = getLatestVersion();

		if( latestVersion->getVersionID() )
		{
			downloadPath += getName();

			struct stat localStat;
			strStat( downloadPath, &localStat );

			chmod( downloadPath, S_IREAD|S_IWRITE );

			TDateTime localTime = vcl::EncodeDateTime( localStat.st_mtime );

			if( !doNotOverwrite )
			{
				long difference = ((double)localTime - (double)latestVersion->getFileModifiedDate())*3600.0*24.0;
				if( abs( difference ) <= 3 )
					difference = 0;

				if( difference
				|| (unsigned long)localStat.st_size != latestVersion->getSize() )
				{
					STRING	message;
					if( localStat.st_mode & S_IWRITE )
					{
						message = "File is writable and changed.\n";
						message += "Repository: ";
						message += latestVersion->getFileModifiedDate().DateTimeString().c_str();
						message += '\n';
						message += "Local: ";
						message += localTime.DateTimeString().c_str();
						message += '\n';
						message += "Overwirte?";
					}
					if( message.isEmpty()
					|| Application->MessageBox( (const char *)message, "DocMan", MB_YESNO|MB_ICONQUESTION ) == IDYES )
					{
						STRING	src = getExternalFile();
						fcopy( src, downloadPath );
					}
				}
			}
		}
	}

#if 0
	// this is a new imported file or an existing file in a local folder
	// in this case we save the current machine so that unreserve can be
	// performed on the same machine only
	if( !getID() || !downloadPath.isEmpty() )
		setLocalMachine();
#endif
}

void THE_FILE::unreserve(
	bool doBranch, const STRING &description, const STRING &newVersion
)
{
	if( !canUnreserve( false ) )
/*@*/	throw Exception( "Item cannot be unreserved" );

	bool	protect = false;		// set to true if the file comes
									// from a local folder
	bool	restoreTime = false;	// set top true if file date changed
									// but not the content

	if( doBranch && getNumLinks() <= 1 )
		doBranch = false;

	STRING		newFile = newVersion;
	PTR_ITEM	theParent = getParent();
	STRING		downloadPath = theParent
		? theParent->getDownloadPath()
		: NULL_STRING;

	if( !downloadPath.isEmpty() )
		downloadPath += getName();

	if( newFile.isEmpty() )
	{
		newFile = downloadPath;
		protect = true;
	}
	else if( !strcmpi( newFile, downloadPath ) )
		protect = true;

	if( !newFile.isEmpty() )
	{
		THE_FILE_VERSION *latestVersion = getLatestVersion();

		STRING		src = getExternalFile();

		struct stat localStat;
		strStat( newFile, &localStat );

		if( (unsigned long)localStat.st_size != latestVersion->getSize() )
		{
			if( doBranch )
				branch();

			createVersion( newFile, description );
		}
		else
		{
			TDateTime localTime = vcl::EncodeDateTime( localStat.st_mtime );
			long difference = ((double)localTime - (double)latestVersion->getFileModifiedDate())*3600.0*24.0;
			if( abs( difference ) <= 3 )
				difference = 0;
			if( difference )
			{
				unsigned char srcMD5[16], localMD5[16];

				md5_file( src, srcMD5 );
				md5_file( newFile, localMD5 );

				if( memcmp( srcMD5, localMD5, 16 ) )
				{
					if( doBranch )
						branch();
					createVersion( newFile, description );
				}
				else
					restoreTime = true;
			}
		}

		if( protect )
		{
			if( restoreTime )
			{
				// restore original date-time
				int				handle_source, handle_dest;
				struct ftime	time;

				if( _dos_open( src, O_RDONLY, &handle_source ) == 0 )
				{
					getftime( handle_source, &time );
					if( _dos_open( newFile, O_RDWR, &handle_dest ) == 0 )
					{
						setftime( handle_dest, &time );
						_dos_close( handle_dest );
					}

					_dos_close(handle_source);
				}
			}

			chmod( newFile, S_IREAD );
		}
	}

	setStatus( STATUS_OK );

	THE_FILE_BASE::cancelReserve();
}

void THE_FILE::cancelReserve( void )
{
	if( !canUnreserve( false ) )
/*@*/	throw Exception( "Item cannot be canceled" );

	PTR_ITEM	theParent = getParent();
	STRING		downloadPath = theParent
		? theParent->getDownloadPath()
		: NULL_STRING;

	if( !downloadPath.isEmpty() )
	{
		downloadPath += getName();
		THE_FILE_VERSION *latestVersion = getLatestVersion();

		STRING		src = getExternalFile();

		struct stat localStat;
		strStat( downloadPath, &localStat );

		TDateTime localTime = vcl::EncodeDateTime( localStat.st_mtime );

		long difference = ((double)localTime - (double)latestVersion->getFileModifiedDate())*3600.0*24.0;
		if( abs( difference ) <= 3 )
			difference = 0;

		if( difference
		|| (unsigned long)localStat.st_size != latestVersion->getSize() )
		{
			STRING message = "You have made changes. They will be lost.\n";
			message += "Repository: ";
			message += latestVersion->getFileModifiedDate().DateTimeString().c_str();
			message += '\n';
			message += "Local: ";
			message += localTime.DateTimeString().c_str();
			message += '\n';
			message += "Overwirte?";

			if( Application->MessageBox(
				message,
				"DocMan",
				MB_ICONQUESTION|MB_YESNO
			) == IDYES )
			{
				chmod( downloadPath, S_IREAD|S_IWRITE );
				fcopy( src, downloadPath );
			}
			else
/*@*/			throw EAbort("Abort");
		}

		chmod( downloadPath, S_IREAD );
	}

	THE_FILE_BASE::cancelReserve();
	setStatus( STATUS_OK );
}

bool THE_FILE::canDelete(  bool forPurge, bool recursive  )
{
	if( getReservedBy() && !canUnreserve( false ) )
/***/	return false;

	int		lockCount = 0;
	std::auto_ptr<TQuery>	theQuery( new TQuery( NULL ) );
	theQuery->DatabaseName = "docManDB";

	theQuery->SQL->Add(
		"select count(*) from i_file_vers where file_id = :id and locked=1"
	);
	theQuery->Params->Items[0]->AsInteger = m_fileID;
	theQuery->Open();
	if( !theQuery->Eof )
		lockCount = theQuery->Fields->Fields[0]->AsInteger;
	theQuery->Close();

	if( lockCount )
		return false;

	return THE_ITEM::canDelete( forPurge, recursive );
}

void THE_FILE::purgeItem( void )
{
	if( !isDeleted() && isMoveable() )
/*@*/	throw Exception( "Cannot purge items not deleted"  );

	if( !canDelete( true, false ) )
/*@*/	throw Exception( "Cannot purge item"  );

	int		usageCount=0;

	std::auto_ptr<TQuery>	theQuery( new TQuery( NULL ) );
	theQuery->DatabaseName = "docManDB";

	theQuery->SQL->Add(
		"select usageCount from i_files where id = :id"
	);
	theQuery->Params->Items[0]->AsInteger = m_fileID;
	theQuery->Open();
	if( !theQuery->Eof )
		usageCount = theQuery->Fields->Fields[0]->AsInteger;
	theQuery->Close();
	theQuery->SQL->Clear();
	if( usageCount > 1 )
	{
		usageCount--;
		theQuery->SQL->Add(
			"update i_files set usageCount = :theUC where id = :id"
		);
		theQuery->Params->Items[0]->AsInteger = usageCount;
		theQuery->Params->Items[1]->AsInteger = m_fileID;

		theQuery->ExecSQL();
	}
	else
	{
		theQuery->SQL->Add(
			"select id, storage_id from i_file_vers where file_id = :theFile"
		);
		theQuery->Params->Items[0]->AsInteger = m_fileID;
		for( theQuery->Open(); !theQuery->Eof; theQuery->Next() )
		{
			deleteVersion(
				theQuery->Fields->Fields[0]->AsInteger,
				theQuery->Fields->Fields[1]->AsInteger,
				false
			);
		}
		theQuery->Close();

		theQuery->SQL->Clear();

		theQuery->SQL->Add(
			"delete from i_files where id = :id"
		);
		theQuery->Params->Items[0]->AsInteger = m_fileID;

		theQuery->ExecSQL();
	}

	THE_ITEM::purgeItem();
}

void THE_FILE::purgeVersions( int numVersions )
{
	THE_ITEM::purgeVersions( numVersions );

	int perms = getUserPermissions();
	if( perms & ITEM_PERM_DELETE_VERSION )
	{
		std::auto_ptr<TQuery>	theQuery( new TQuery( NULL ) );
		theQuery->DatabaseName = "docManDB";

		theQuery->SQL->Add(
			"select id, storage_id, locked, version "
			"from i_file_vers "
			"where file_id = :theFile "
			"order by version desc"
		);
		theQuery->Params->Items[0]->AsInteger = m_fileID;
		for( theQuery->Open(); !theQuery->Eof; theQuery->Next(), numVersions-- )
		{
			if( numVersions <= 0 && !theQuery->Fields->Fields[2]->AsInteger )
			{
				deleteVersion(
					theQuery->Fields->Fields[0]->AsInteger,
					theQuery->Fields->Fields[1]->AsInteger,
					false
				);
			}
		}
		theQuery->Close();
	}
}

PTR_ITEM THE_FILE::copy(
	ItemsCopiedMap &itemsCopied,
	const PTR_ITEM &target, bool createLink, bool moveReserve,
	const STRING &newName
)
{
	if( StatusForm->setStatus( "Copying", getName() ) )
	{
/*@*/	return PTR_ITEM();
	}

	if( createLink )
	{
/***/	return link( target, newName );
	}

	if( itemsCopied.hasElement( m_fileID ) )
	{
		PTR_FILE theSource = getItem( itemsCopied[m_fileID] );
		if( theSource )
		{
/***/		return theSource->link( target, newName );
		}
	}

	PTR_FILE newItem = getFactory()->createItemFromTemplate(
		target, this, newName
	);
	if( newItem )
	{
		THE_FILE_VERSION *latestVersion = getLatestVersion();

		int fileVerId = ConfigDataModule->getNewMaxValue(
			"I_FILE_VERS", "ID"
		);
		int col;

		std::auto_ptr<TQuery>	theQuery( new TQuery( NULL ) );
		theQuery->DatabaseName = "docManDB";

		theQuery->SQL->Add(
			"insert into i_file_vers "
			"( "
				"id, file_id, version, mime_type, filename, "
				"createdby, createddate, modifieddate, "
				"locked, storage_id, filesize, "
				"filecreateddate, filemodifieddate "
			") "
			"values "
			"( "
				":id, :file_id, 1, :mime_type, :filename, "
				":createdby, :createddate, :modifieddate, "
				"0, :storage_id, :file_size, "
				":filecreateddate, :filemodifieddate "
			") "
		);
		col = 0;
		theQuery->Params->Items[col++]->AsInteger = fileVerId;
		theQuery->Params->Items[col++]->AsInteger = (*newItem).m_fileID;
		theQuery->Params->Items[col++]->AsString = (const char *)latestVersion->getMimeType();
		theQuery->Params->Items[col++]->AsString = (const char *)latestVersion->getFileName();
		theQuery->Params->Items[col++]->AsInteger = vcl::getActUserID();
		theQuery->Params->Items[col++]->AsDateTime = TDateTime::CurrentDateTime();
		theQuery->Params->Items[col++]->AsDateTime = TDateTime::CurrentDateTime();
		theQuery->Params->Items[col++]->AsInteger = latestVersion->getStorageID();
		theQuery->Params->Items[col++]->AsInteger = latestVersion->getSize();
		theQuery->Params->Items[col++]->AsDateTime = latestVersion->getFileCreationDate();
		theQuery->Params->Items[col++]->AsDateTime = latestVersion->getFileModifiedDate();

		theQuery->ExecSQL();

		theQuery->SQL->Clear();
		theQuery->SQL->Add(
			"update i_storage set usage_count = :newUD where id=:storageId"
		);
		theQuery->Params->Items[0]->AsInteger = latestVersion->incrUsageCount();
		theQuery->Params->Items[1]->AsInteger = latestVersion->getStorageID();

		theQuery->ExecSQL();

		theQuery->SQL->Clear();
		theQuery->SQL->Add(
			"update i_files set version = 1 where id = :fileId"
		);
		theQuery->Params->Items[0]->AsInteger = newItem->m_fileID;

		theQuery->ExecSQL();
		itemsCopied[m_fileID] =newItem->getID();

		if( moveReserve && canUnreserve(true) )
		{
			THE_FILE_BASE::cancelReserve();
			updateDatabase();
			newItem->THE_FILE_BASE::reserve();
			newItem->updateDatabase();
		}
	}

	return newItem;
}

bool THE_FILE::hasReserved( const STRING &machine, int userId )
{
	int reservedBy = getReservedBy();

	if( reservedBy )
	{
		if( !machine.isEmpty() && machine != getReservedOn() )
			return false;

		if( userId && userId != reservedBy )
			return false;

		return true;
	}
	else
		return false;
}
//---------------------------------------------------------------------------
bool ACTION_DOWNLOAD::acceptItem( THE_ITEM *theItem )
{
	return dynamic_cast<THE_FILE*>(theItem) ? true : false;
}

const char *ACTION_DOWNLOAD::getLabel( void ) const
{
	return "Download...";
}

REFRESH_TYPE ACTION_DOWNLOAD::perform( PTR_ITEM theItem )
{
	PTR_FILE	theFile = theItem;
	if( theFile )
	{
		std::auto_ptr<TSaveDialog>	saveDialog( new TSaveDialog( NULL ) );

		saveDialog->FileName = (const char *)theItem->getName();
		if( saveDialog->Execute() )
			theFile->download( 0, false, saveDialog->FileName.c_str() );
	}

	return REFRESH_NONE;
}

//---------------------------------------------------------------------------
bool ACTION_BRANCH_FILE::acceptItem( THE_ITEM *theItem )
{
	doEnterFunctionEx(gakLogging::llInfo, "ACTION_BRANCH_FILE::acceptItem");
	bool	accepted = false;

	const THE_FILE *theFile = dynamic_cast<const THE_FILE *>(theItem);
	if( theFile )
	{

		int numLinks = theItem->getNumLinks();
		if( numLinks > 1 )
		{
			int perms = theItem->getUserPermissions();
			if( perms & ITEM_PERM_MODIFY )
				accepted = ACTION_BASE::acceptItem( theItem );;
		}
	}

	return accepted;
}

const char *ACTION_BRANCH_FILE::getLabel( void ) const
{
	return "Branch...";
}

REFRESH_TYPE ACTION_BRANCH_FILE::perform( PTR_ITEM theItem )
{
	PTR_FILE	theFile( theItem );

	if( Application->MessageBox(
		"This will disconnect the file from its links.\n"
		"This cannot be undone. Continue?",
		"Branch",
		MB_ICONQUESTION|MB_YESNO
	) == IDYES )
	{
		(*theFile).branch();
		return REFRESH_RELOAD;
	}

	return REFRESH_NONE;
}

//---------------------------------------------------------------------------
bool ACTION_GENERATE::acceptItem( THE_ITEM *theItem )
{
	doEnterFunctionEx(gakLogging::llInfo, "ACTION_GENERATE::acceptItem");

	bool		acceptable = false;
	THE_FILE	*theFile = dynamic_cast<THE_FILE *>(theItem);

	if( theFile )
	{
		PTR_ITEM	parent = theFile->getParent();
		while( (THE_ITEM*)parent )
		{
			PTR_WEB_FOLDER webParent( parent );
			if( (THE_WEB_FOLDER*)webParent )
			{
				acceptable = true;
/*v*/			break;
			}

			parent = (*parent).getParent();
		}

		if( acceptable )
			acceptable = ACTION_BASE::acceptItem( theItem );
	}

	return acceptable;
}

const char *ACTION_GENERATE::getLabel( void ) const
{
	return "Generate Web File";
}

REFRESH_TYPE ACTION_GENERATE::perform( PTR_ITEM theItem )
{
	PTR_FILE	theFile( theItem );
	STRING		dummy;

	theFile->generateWebFile( false, dummy );

	return REFRESH_NONE;
}
//---------------------------------------------------------------------------
void THE_FILE::createXMLattributes( xml::Element *theElement )
{
	THE_ITEM::createXMLattributes( theElement );

	theElement->setStringAttribute( "mimeType", getMimeType() );
	theElement->setIntegerAttribute( "fileSize", getSizeInBytes() );
}

void THE_FILE::lock( void )
{
	int perms = getUserPermissions();
	if( perms & ITEM_PERM_MODIFY )
	{
		THE_FILE_VERSION *theVersion = getLatestVersion();

		if( !theVersion->getLocked() )
		{
			if( StatusForm->pushStatus( "Locking", getName() ) )
/*@*/			return;

			std::auto_ptr<TQuery>	theQuery( new TQuery( NULL ) );
			theQuery->DatabaseName = "docManDB";
			theQuery->SQL->Add(
				"update i_file_vers "
				"set locked = 1 "
				"where id = :versionID"
			);
			theQuery->Params->Items[0]->AsInteger = theVersion->getVersionID();
			theQuery->ExecSQL();

			StatusForm->restore();
		}
	}
}

void THE_FILE::unlock( void )
{
	int perms = getUserPermissions();
	if( perms & ITEM_PERM_MODIFY )
	{
		THE_ITEM::unlock();
		std::auto_ptr<TQuery>	theQuery( new TQuery( NULL ) );
		theQuery->DatabaseName = "docManDB";
		theQuery->SQL->Add(
			"update i_file_vers "
			"set locked = 0 "
			"where file_id = :fileID"
		);
		theQuery->Params->Items[0]->AsInteger = m_fileID;
		theQuery->ExecSQL();
	}
}

bool THE_FILE::hasChanged(
	const STRING &dest, DateTime *oLocalDate, DateTime *oDbDate
)
{
	bool	result = true;
	struct stat localStat;

	if( !strStat( dest, &localStat ) )
	{
//		THE_FILE_VERSION *latestVersion = getLatestVersion();

		DateTime	localDate( localStat.st_mtime );
		DateTime	dbDate( getFileModifiedDate() );

		if( oDbDate )
			*oDbDate = dbDate;
		if( oLocalDate )
			*oLocalDate = localDate;

		long difference = dbDate.getUtcUnixSeconds() - localDate.getUtcUnixSeconds();
		if( abs( difference ) <= 3 )
			result = false;
	}

	return result;
}

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

const STRING &THE_FILE_BASE::calcStatus( bool force )
{
	if( force || status.isEmpty() )
	{
		PTR_ITEM parent = getParent();
		if( parent )
		{
			STRING		downloadPath = parent->getDownloadPath();

			if( !downloadPath.isEmpty() )
			{
				downloadPath += getName();

				DateTime	localDate( false );
				DateTime	dbDate( false );

				if( strAccess( downloadPath, 04 ) )
					status = STATUS_MISSING;
				else if( hasChanged( downloadPath, &localDate, &dbDate ) )
				{
					if( localDate > dbDate  )
						status = STATUS_NEWER;
					else
						status = STATUS_OLDER;
				}
				else if( getReservedBy() )
					status = STATUS_RESERVED;
				else
					status = STATUS_OK;
			}
		}
		else
			status = "-";
	}

	return status;
}

PTR_ITEM THE_FILE::link( const PTR_ITEM &target, const STRING &newName )
{
	THE_FILE	*newLink = new THE_FILE( 0, getFactory() );
	PTR_ITEM	newItem( newLink );

	newLink->m_fileID = m_fileID;
	newLink->setData( target, newName.isEmpty() ? getName() : newName, "", "" );
	newLink->setACL( getACL() );
	newLink->setCopyID( getID() );
	newLink->updateDatabase();
	std::auto_ptr<TQuery>	theQuery( new TQuery( NULL ) );
	theQuery->DatabaseName = "docManDB";
	theQuery->SQL->Add(
		"update i_files "
		"set usageCount = usageCount+1 "
		"where id = :fileID"
	);
	theQuery->Params->Items[0]->AsInteger = m_fileID;
	theQuery->ExecSQL();

	return newItem;
}

void THE_FILE::branch( void )
{
	int numLinks = getNumLinks();
	if( numLinks <= 1 )
	{
/*@*/	throw Exception( "There are no other links" );
	}

	TQuery	*theQuery = new TQuery( NULL );
	theQuery->DatabaseName = "docManDB";

	THE_FILE_VERSION	*src = getLatestVersion();
	int	oldFileID = m_fileID;

	/*
		create a new file id
	*/
	m_fileID = ConfigDataModule->getNewMaxValue(
		"I_FILES", "ID"
	);
	theQuery->SQL->Add(
		"insert into I_FILES "
		"( id, usagecount, version ) "
		"values"
		"( :id, 1, 1 )"
	);

	theQuery->Params->Items[0]->AsInteger = m_fileID;
	theQuery->ExecSQL();

	/*
		decrement usage counter
	*/
	theQuery->SQL->Clear();
	theQuery->SQL->Add(
		"update i_files set usagecount = usagecount-1 "
		"where id = :oldFileId"
	);
	theQuery->Params->Items[0]->AsInteger = oldFileID;
	theQuery->ExecSQL();


	/*
		create version entry
	*/

	int fileVerId = ConfigDataModule->getNewMaxValue(
		"I_FILE_VERS", "ID"
	);

	theQuery->SQL->Clear();
	theQuery->SQL->Add(
		"insert into I_FILE_VERS "
		"( "
			"id, file_id, version, mime_type, filename, storage_id, "
			"createdBy, createdDate, modifiedDate, fileCreatedDate, "
			"fileModifiedDate, filesize "
		") "
		"values"
		"( "
			":id, :file_id, 1, :mime_type, :filename, :storage_id, "
			":createdBy, :createdDate, :modifiedDate, :fileCreatedDate, "
			":fileModifiedDate, :filesize "
		")"
	);

	int i=0;
	theQuery->Params->Items[i++]->AsInteger = fileVerId;
	theQuery->Params->Items[i++]->AsInteger = m_fileID;
	theQuery->Params->Items[i++]->AsString = static_cast<const char *>(src->getMimeType());
	theQuery->Params->Items[i++]->AsString = static_cast<const char *>(src->getFileName());
	theQuery->Params->Items[i++]->AsInteger = src->getStorageID();
	theQuery->Params->Items[i++]->AsInteger = vcl::getActUserID();
	theQuery->Params->Items[i++]->AsDateTime = TDateTime::CurrentDateTime();
	theQuery->Params->Items[i++]->AsDateTime = TDateTime::CurrentDateTime();
	theQuery->Params->Items[i++]->AsDateTime = src->getFileCreationDate();
	theQuery->Params->Items[i++]->AsDateTime = src->getFileModifiedDate();
	theQuery->Params->Items[i]->AsInteger = src->getSize();

	theQuery->ExecSQL();

	/*
		update usage counter of storage
	*/
	theQuery->SQL->Clear();
	theQuery->SQL->Add(
		"update i_storage set usage_count = :newUC where id = :storageID"
	);
	theQuery->Params->Items[0]->AsInteger = src->incrUsageCount();
	theQuery->Params->Items[1]->AsInteger = src->getStorageID();

	theQuery->ExecSQL();

	clearLatestVersion();
	updateDatabase();
}

const char *THE_FILE::compare( int firstVersion, int secondVersion )
{
	const char *diffResult = NULL;

	STRING		firstFile = getExternalStorageBase();
	STRING		secondFile, downloadPath;

	THE_FILE_VERSION theFirstVersion( m_fileID, firstVersion );

	firstFile += theFirstVersion.getStoragePath();

	if( secondVersion > 0 )
	{
		secondFile = getExternalStorageBase();
		THE_FILE_VERSION theSecondVersion( m_fileID, secondVersion );
		secondFile += theSecondVersion.getStoragePath();
	}
	else
	{
		PTR_ITEM	theParent = getParent();
		if( theParent )
		{
			secondFile = theParent->getDownloadPath();
			if( !secondFile.isEmpty() )
				secondFile += getName();
		}
	}

	if( !secondFile.isEmpty() )
	{
		unsigned char firstMD5[16], secondMD5[16];

		md5_file( firstFile, firstMD5 );
		md5_file( secondFile, secondMD5 );

		if( memcmp( firstMD5, secondMD5, 16 ) )
			diffResult = diff( firstFile, secondFile );
	}
	else
/*@*/	throw Exception( "No local Path" );

	return diffResult;
}

bool THE_FILE::canCreateVersion( void ) const
{
	if( getReservedBy() )
/***/	return canUnreserve( false );

	int perms = getUserPermissions();
	return (perms & ITEM_PERM_ADD_VERSION) ? true : false;
}

int THE_FILE::createVersion( int srcVersion )
{
	if( !canCreateVersion() )
/*@*/	throw Exception( "Permission denied" );

	int 				fileVerId = ConfigDataModule->getNewMaxValue(
		"I_FILE_VERS", "ID"
	);
	char				filter[32];
	sprintf( filter, "file_id=%d", m_fileID );
	int					newVersion = vcl::getNewMaxValue(
		"docManDB", "I_FILE_VERS", "VERSION", filter
	);
	THE_FILE_VERSION	src( m_fileID, srcVersion );

	std::auto_ptr<TQuery>	theQuery( new TQuery( NULL ) );
	theQuery->DatabaseName = "docManDB";

	theQuery->SQL->Add(
		"insert into I_FILE_VERS "
		"( "
			"id, file_id, version, mime_type, filename, storage_id, "
			"createdBy, createdDate, modifiedDate, fileCreatedDate, "
			"fileModifiedDate, filesize "
		") "
		"values"
		"( "
			":id, :file_id, :version, :mime_type, :filename, :storage_id, "
			":createdBy, :createdDate, :modifiedDate, :fileCreatedDate, "
			":fileModifiedDate, :filesize "
		")"
	);

	theQuery->Params->Items[0]->AsInteger = fileVerId;
	theQuery->Params->Items[1]->AsInteger = m_fileID;
	theQuery->Params->Items[2]->AsInteger = newVersion;
	theQuery->Params->Items[3]->AsString = static_cast<const char *>(src.getMimeType());
	theQuery->Params->Items[4]->AsString = static_cast<const char *>(src.getFileName());
	theQuery->Params->Items[5]->AsInteger = src.getStorageID();
	theQuery->Params->Items[6]->AsInteger = vcl::getActUserID();
	theQuery->Params->Items[7]->AsDateTime = TDateTime::CurrentDateTime();
	theQuery->Params->Items[8]->AsDateTime = TDateTime::CurrentDateTime();
	theQuery->Params->Items[9]->AsDateTime = src.getFileCreationDate();
	theQuery->Params->Items[10]->AsDateTime = src.getFileModifiedDate();
	theQuery->Params->Items[11]->AsInteger = src.getSize();

	theQuery->ExecSQL();

	theQuery->SQL->Clear();
	theQuery->SQL->Add(
		"update i_files set version = :theVersion where id = :fileId"
	);
	theQuery->Params->Items[0]->AsInteger = newVersion;
	theQuery->Params->Items[1]->AsInteger = m_fileID;

	theQuery->ExecSQL();

	theQuery->SQL->Clear();
	theQuery->SQL->Add(
		"update i_storage set usage_count = :newUC where id = :storageID"
	);
	theQuery->Params->Items[0]->AsInteger = src.incrUsageCount();
	theQuery->Params->Items[1]->AsInteger = src.getStorageID();

	theQuery->ExecSQL();

	setFileSize( src.getSize() );
	setFileModifiedDate( src.getFileModifiedDate() );
	setMimeType( src.getMimeType() );
	clearLatestVersion();

	DocManDataModule->copyAttributeValues(
		getID(), srcVersion, getID(), newVersion
	);
	ImageMetaData	metaData;
	updateImageMetaData( &metaData, newVersion );

	return newVersion;
}

void THE_FILE::createVersion( const STRING &filePath, const STRING &description )
{
	if( !canCreateVersion() )
/*@*/	throw Exception( "Permission denied" );

	int fileVerId = ConfigDataModule->getNewMaxValue(
		"I_FILE_VERS", "ID"
	);
	char	filter[32];
	sprintf( filter, "file_id=%d", m_fileID );
	int	version = vcl::getNewMaxValue(
		"docManDB", "I_FILE_VERS", "VERSION", filter
	);
	int storageID = createStorage( filePath );
	int srcVersion = getVersionNum();

	struct stat statBuf;
	stat( filePath, &statBuf );

	setFileSize( statBuf.st_size );
	setFileModifiedDate( vcl::EncodeDateTime(statBuf.st_mtime) );
	setStatus( STATUS_OK );

	STRING	fileName = filePath;
	size_t	slashPos = fileName.searchRChar( DIRECTORY_DELIMITER );
	if( slashPos != (size_t)-1 )
		fileName += slashPos+1;

	setMimeType( MimeTypesForm->GetMimeType( fileName ) );

	std::auto_ptr<TQuery>	theQuery( new TQuery( NULL ) );
	theQuery->DatabaseName = "docManDB";

	theQuery->SQL->Add(
		"insert into I_FILE_VERS "
		"( "
			"id, file_id, version, mime_type, filename, storage_id, "
			"createdBy, createdDate, modifiedDate, fileCreatedDate, "
			"fileModifiedDate, filesize, description "
		") "
		"values"
		"( "
			":id, :file_id, :version, :mime_type, :filename, :storage_id, "
			":createdBy, :createdDate, :modifiedDate, :fileCreatedDate, "
			":fileModifiedDate, :filesize, :description "
		")"
	);

	theQuery->Params->Items[0]->AsInteger = fileVerId;
	theQuery->Params->Items[1]->AsInteger = m_fileID;
	theQuery->Params->Items[2]->AsInteger = version;
	theQuery->Params->Items[3]->AsString = static_cast<const char *>(getMimeType());
	theQuery->Params->Items[4]->AsString = static_cast<const char *>(fileName);
	theQuery->Params->Items[5]->AsInteger = storageID;
	theQuery->Params->Items[6]->AsInteger = vcl::getActUserID();
	theQuery->Params->Items[7]->AsDateTime = TDateTime::CurrentDateTime();
	theQuery->Params->Items[8]->AsDateTime = TDateTime::CurrentDateTime();
	theQuery->Params->Items[9]->AsDateTime = vcl::EncodeDateTime(statBuf.st_ctime);
	theQuery->Params->Items[10]->AsDateTime = getFileModifiedDate();
	theQuery->Params->Items[11]->AsInteger = statBuf.st_size;
	theQuery->Params->Items[12]->AsMemo = static_cast<const char *>(description);

	theQuery->ExecSQL();

	theQuery->SQL->Clear();
	theQuery->SQL->Add(
		"update i_files set version = :theVersion where id = :fileId"
	);
	theQuery->Params->Items[0]->AsInteger = version;
	theQuery->Params->Items[1]->AsInteger = m_fileID;

	theQuery->ExecSQL();

	DocManDataModule->copyAttributeValues(
		getID(), srcVersion, getID(), version
	);
	clearLatestVersion();

	ImageMetaData	metaData;
	updateImageMetaData( &metaData, version );
}

void THE_FILE::deleteVersion( int versionId, int storageId, bool updateMaxVersion )
{
	STRING		filePath;
	int			usageCount = 0;
	int			itemID = getID();
	int			version = 0;

	std::auto_ptr<TQuery>	theQuery( new TQuery( NULL ) );
	theQuery->DatabaseName = "docManDB";

	theQuery->SQL->Add( "select * from i_storage where id = :storageId" );
	theQuery->Params->Items[0]->AsInteger = storageId;
	theQuery->Open();
	if( !theQuery->Eof )
	{
		filePath = theQuery->FieldByName("file_path")->AsString.c_str();
		usageCount = theQuery->FieldByName( "usage_count" )->AsInteger;
	}
	theQuery->Close();

	theQuery->SQL->Clear();
	theQuery->SQL->Add( "select version from i_file_vers where id = :versionId" );
	theQuery->Params->Items[0]->AsInteger = versionId;
	theQuery->Open();
	if( !theQuery->Eof )
	{
		version = theQuery->FieldByName( "version" )->AsInteger;
	}
	theQuery->Close();

	theQuery->SQL->Clear();
	theQuery->SQL->Add(
		"delete from i_file_vers where id = :versionId"
	);
	theQuery->Params->Items[0]->AsInteger = versionId;

	theQuery->ExecSQL();

	if( version )
	{
		theQuery->SQL->Clear();
		theQuery->SQL->Add(
			"delete from i_exif_data where item_id = :itemId and version=:theVersion"
		);
		theQuery->Params->Items[0]->AsInteger = itemID;
		theQuery->Params->Items[1]->AsInteger = version;

		theQuery->ExecSQL();

		theQuery->SQL->Clear();
		theQuery->SQL->Add(
			"delete from c_attr_values where item_id = :itemId and item_version=:theVersion"
		);
		theQuery->Params->Items[0]->AsInteger = itemID;
		theQuery->Params->Items[1]->AsInteger = version;

		theQuery->ExecSQL();
	}

	theQuery->SQL->Clear();
	if( usageCount > 1 )
	{
		theQuery->SQL->Add(
			"update i_storage set usage_count = :newUC where id = :storageId"
		);
		theQuery->Params->Items[0]->AsInteger = usageCount-1;
		theQuery->Params->Items[1]->AsInteger = storageId;

		theQuery->ExecSQL();
	}
	else
	{
		STRING		src = getExternalStorageBase();

		theQuery->SQL->Add(
			"delete from i_storage where id = :storageId"
		);
		theQuery->Params->Items[0]->AsInteger = storageId;

		theQuery->ExecSQL();

		src += filePath;

		unlink( src );
	}

	if( updateMaxVersion )
	{
		theQuery->SQL->Clear();
		theQuery->SQL->Add(
			"update i_files "
			"set version = ("
				"select max(version) from i_file_vers where file_id = :fileId1"
			") "
			"where id = :fileId2"
		);
		theQuery->Params->Items[0]->AsInteger = m_fileID;
		theQuery->Params->Items[1]->AsInteger = m_fileID;

		theQuery->ExecSQL();

		refreshLatestVersion();
	}
}

STRING THE_FILE::download( int version, bool protect, const STRING &i_dest )
{
	STRING		dest = i_dest;

	doEnterFunctionEx(gakLogging::llInfo, "THE_FILE::download");

	STRING		src = getExternalStorageBase();

	THE_FILE_VERSION *theFileVersion = version
		? new THE_FILE_VERSION(m_fileID, version)
		: getLatestVersion()
	;

	if( theFileVersion->getStorageID() )
	{
		try
		{
			src += theFileVersion->getStoragePath();

			if( dest.isEmpty() )
			{
				dest = version ? getTempDownloadPath() : getDownloadPath();
				dest = getDownloadFile( dest );
			}

			if( version || getReservedOn() != TDocManDataModule::getMachine() )
			{
				struct stat localStat;
				strStat( dest, &localStat );

				TDateTime localTime = vcl::EncodeDateTime( localStat.st_mtime );

				long difference = (
					(double)localTime - (double)theFileVersion->getFileModifiedDate()
				)*3600.0*24.0;
				if( abs( difference ) <= 2 )
					difference = 0;

				if( difference
				|| (unsigned long)localStat.st_size != theFileVersion->getSize() )
				{
					chmod( dest, S_IREAD|S_IWRITE );
					makePath( dest );
					fcopy( src, dest );
				}

				if( protect )
					chmod( dest, S_IREAD );
			}
			if( version )
				delete theFileVersion;
		}
		catch( ... )
		{
			doLogPosition();
			if( version )
				delete theFileVersion;
/*@*/		throw;
		}
	}
	else
	{
/*@*/	throw Exception( "No version available" );
	}

	return dest;
}

STRING THE_FILE::generateWebFile( bool forWebServer, STRING &mimeType )
{
	STRING			exportedFile;
	PTR_WEB_FOLDER	webParent;

	for(
		PTR_ITEM parent = getParent();
		(THE_ITEM*)parent;
		parent = (*parent).getParent()
	 )
	{
		webParent = parent;
		if( webParent )
		{
			exportedFile = (*webParent).generateWebFile(
				this, forWebServer, mimeType
			);
/*v*/		break;
		}
	}

	return exportedFile;
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

xml::Document *loadXmlDoc( const STRING &xmlFile )
{
	doEnterFunctionEx(gakLogging::llInfo, "loadXmlDoc( const STRING &xmlFile )");

	xml::Document	*theDoc = NULL;
	PTR_FILE		theFile = getItemByPath( xmlFile );

	if( (THE_FILE*)theFile )
	{
		theDoc = (*theFile).getXmlDocument();
	}

	return theDoc;
}

STRING loadCssDoc( const STRING &cssFile )
{
	doEnterFunctionEx(gakLogging::llInfo, "loadCssDoc( const STRING &xmlFile )");

	STRING		css;
	PTR_FILE	theFile = getItemByPath( cssFile );

	if( theFile )
	{
		css = (*theFile).getCSScontent();
	}

	return css;
}

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -a.
#	pragma option -p.
#endif

