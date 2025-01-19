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

#include <gak/arrayFile.h>
#include <gak/io.h>

#include "RemoteItems.h"
#include "CryptoFile.h"

#include "Folder.h"
#include "File.h"
#include "RemoteFolderCreateFrm.h"
#include "FileCreateFrm.h"
#ifndef DOCMANBG
#include "RemoteServerFrm.h"
#endif
#include "DocManService.h"
#include "MimeTypesFrm.h"

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

using namespace gak;
using namespace xml;

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

class FACTORY_REMOTE_FOLDER : public FACTORY_LOCAL_FOLDER
{
	virtual int getItemType( void ) const;
	virtual PTR_ITEM createItem( int id ) const;
	virtual const char *getName( void ) const;
	virtual TItemCreateForm *getForm( void ) const;
	virtual PTR_ITEM createItemFromForm( const PTR_ITEM &parent ) const;
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static FACTORY_REMOTE_FOLDER			theRemoteFolderFactory;
static FACTORY_REMOTE_FILE				theRemoteFileFactory;

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

size_t THE_REMOTE_FOLDER::findEntryByRemoteID( int remoteID )
{
	PTR_ITEM			theItem;
	PTR_REMOTE_FOLDER	theFolder;
	PTR_REMOTE_FILE		theFile;
	for( size_t i=0; theItem = getContentItem( i ); i++ )
	{
		if( (theFolder = theItem) && theFolder->remoteData.id == remoteID )
		{
			return i;
		}
		if( (theFile = theItem) && theFile->getRemoteFileID() == remoteID )
		{
			return i;
		}
	}

	return -1;
}
const DocManService::VersionRecord_t *THE_REMOTE_FILE::getLatestVersion( void )
{
	doEnterFunction( "THE_REMOTE_FILE::getLatestVersion" );

	if( !latestVersion.id )
	{
		int	serverID = PTR_REMOTE_FOLDER( getParent() )->getServerID();

		DocManService	*theService = RemoteServerForm->GetServiceByID(
			serverID
		);

		try
		{
			Element *result = theService->sGetVersions( remoteData.id, 0 );
			if( result )
			{
				Array<DocManService::VersionRecord_t>	versions;
				theService->fillVersionRecord( versions, result );
				if( versions.size() == 1 )
				{
					latestVersion = versions[0];
					setFileModifiedDate(
						DateTime( latestVersion.modifiedDate ).calcLocalTime()
					);
				}
				else
				{
					latestVersion.id = 0;
				}
			}
			else
			{
				doLogValue( theService->getBody() );
/*@*/			throw Exception( "Illegal result" );
			}
		}
		catch( net::SoapRequest::SoapException &e )
		{
			STRING	errText = e.faultString;
			if( !e.faultDetail.isEmpty() )
			{
				errText += ": ";
				errText += e.faultDetail;
			}
			doLogValue( theService->getBody() );
/*@*/		throw EDatabaseError( (const char *)errText );
		}
		catch( std::exception &e )
		{
			doLogValue( theService->getBody() );
/*@*/		throw Exception( e.what() );
		}
		setMimeType( latestVersion.mimeType );
	}

	return &latestVersion;
}

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //

bool FACTORY_REMOTE_FILE::acceptParent( const THE_ITEM *parent ) const
{
	PTR_REMOTE_FOLDER remoteParent( const_cast<THE_ITEM *>(parent) );

	return remoteParent;
}

int FACTORY_REMOTE_FILE::getItemType( void ) const
{
	return TYPE_REMOTE_FILE;
}

PTR_ITEM FACTORY_REMOTE_FILE::createItem( int id ) const
{
	return new THE_REMOTE_FILE( id, this );
}
const char *FACTORY_REMOTE_FILE::getName( void ) const
{
	return "Remote File";
}
PTR_ITEM FACTORY_REMOTE_FILE::createItemFromForm( const PTR_ITEM &parent ) const
{
	PTR_REMOTE_FILE newFile =  createItem( 0 );
#ifndef DOCMANBG
	PTR_FILE		source = FileCreateForm->getTemplate();
	STRING src = source
		? source->getExternalFile()
		: STRING(FileCreateForm->EditFilePath->Text.c_str())
	;
	STRING mimeType = source
		? source->getMimeType()
		: NULL_STRING
	;

	newFile->setData(
		parent,
		FileCreateForm->EditName->Text.c_str(),
		FileCreateForm->MemoDescription->Text.c_str(),
		src, mimeType,
		0,
		time( NULL ), time( NULL )
	);

	newFile->updateDatabase();
	if( FileCreateForm->CheckBoxReserve->Checked )
		newFile->reserve();
	newFile->updateDatabase();
#endif
	return newFile;
}


int FACTORY_REMOTE_FOLDER::getItemType( void ) const
{
	return (int)TYPE_REMOTE_FOLDER;
}

PTR_ITEM FACTORY_REMOTE_FOLDER::createItem( int id ) const
{
	return new THE_REMOTE_FOLDER( id, this );
}

const char *FACTORY_REMOTE_FOLDER::getName( void ) const
{
	return "Remote Folder";
}

TItemCreateForm *FACTORY_REMOTE_FOLDER::getForm( void ) const
{
#ifdef DOCMANBG
	return NULL;
#else
	return RemoteFolderCreateForm;
#endif
}

PTR_ITEM FACTORY_REMOTE_FOLDER::createItemFromForm( const PTR_ITEM &parent ) const
{
	PTR_ITEM			newFolder = createItem( 0 );
#ifndef DOCMANBG
	PTR_REMOTE_FOLDER	fp = newFolder;
	PTR_REMOTE_FOLDER	remoteParent = parent;
	int					RemoteFolderID;


	if( remoteParent )
		RemoteFolderID = 0;
	else if( RemoteFolderCreateForm->EditRemoteFolderID->Text > "" )
		RemoteFolderID = atoi(
			RemoteFolderCreateForm->EditRemoteFolderID->Text.c_str()
		);
	else
		RemoteFolderID = -1;

	fp->setData(
		parent,
		RemoteFolderCreateForm->EditName->Text.c_str(),
		RemoteFolderCreateForm->MemoDescription->Text.c_str(),
		RemoteFolderCreateForm->EditLocalPath->Text.c_str(),
		RemoteServerForm->GetServerId(
			RemoteFolderCreateForm->ComboBoxRemoteServer->ItemIndex
		),
		RemoteFolderID
	);
	fp->updateDatabase();
#endif
	return newFolder;
}

ITEM_CONTENT *THE_REMOTE_FOLDER::loadContent( void )
{
	ITEM_CONTENT	*myContent = THE_LOCAL_FOLDER::loadContent();

	DocManService *theService = RemoteServerForm->GetServiceByID( RemoteServerID );
	if( theService )
	{
		xml::Element *result = NULL;
		try
		{
			result = theService->sGetItemContent( remoteData.id );
		}
		catch( net::SoapRequest::SoapException &e )
		{
			STRING	errText = e.faultString;
			if( !e.faultDetail.isEmpty() )
			{
				errText += ": ";
				errText += e.faultDetail;
			}
/*@*/		throw EDatabaseError( (const char *)errText );
		}
		catch( std::exception &e )
		{
/*@*/		throw Exception( e.what() );
		}
		if( result )
		{
			Array<DocManService::ItemRecord_t>	items;
			theService->fillItemRecord( items, result );
			PTR_ITEM			childItem;
			PTR_REMOTE_FOLDER	childFolder;
			PTR_REMOTE_FILE		childFile;
			bool				found;
			/*
				remove all DB children that no longer exist on the remote server
			*/
			for(
				int child = myContent->size()-1;
				child >= 0;
				child--
			)
			{
				childItem = getContentItem( child );
				if( childItem )
				{
					found = false;
					childFolder = childItem;
					childFile = childItem;
					for(
						Array<DocManService::ItemRecord_t>::const_iterator it = items.cbegin(),
							endIT = items.cend();
						it != endIT;
						++it
					)
					{
						const DocManService::ItemRecord_t	&theItem = *it;
						if( childFolder && childFolder->getRemoteFolderID() == theItem.id )
						{
							if( theItem.item_type == TYPE_FOLDER )
								found = true;
							break;
						}
						else if( childFile && childFile->getRemoteFileID() == theItem.id )
						{
							if( theItem.item_type == TYPE_DOCUMENT || theItem.item_type == TYPE_CRYPTO_FILE )
								found = true;
							break;
						}
					}
					if( !found )
					{
						childItem->purgeItem();
						myContent->removeElementAt( child );
					}
				}
			}

			/*
				add all items in our database not yet stored
			*/
			for(
				Array<DocManService::ItemRecord_t>::const_iterator it = items.cbegin(),
					endIT = items.cend();
				it != endIT;
				++it
			)
			{
				const DocManService::ItemRecord_t	&theItem = *it;
				size_t	entryIdx = findEntryByRemoteID( theItem.id );

				if( theItem.item_type == TYPE_FOLDER )
				{
					if( entryIdx == -1 )
					{
						PTR_REMOTE_FOLDER	theFolder =
							createItem( TYPE_REMOTE_FOLDER )
						;
						theFolder->setData(
							this,
							theItem.name, theItem.description,
							"",
							RemoteServerID,
							theItem.id
						);
						theFolder->setRemoteData( theItem );
						myContent->addElement( theFolder );
					}
					else
					{
						PTR_REMOTE_FOLDER	theFolder =
							getContentItem( entryIdx )
						;
						if( theFolder )
							theFolder->setRemoteData( theItem );
					}
				}
				else if( theItem.item_type == TYPE_DOCUMENT )
				{
					if( entryIdx == -1 )
					{
						PTR_REMOTE_FILE	theFile =
							createItem( TYPE_REMOTE_FILE )
						;
						theFile->setData(
							this,
							theItem.name,
							theItem.description,
							"", "",
							theItem.id,
							theItem.createDate,
							theItem.modifiedDate
						);
						theFile->setRemoteData( theItem );
						myContent->addElement( theFile );
					}
					else
					{
						PTR_REMOTE_FILE	theFile =
							getContentItem( entryIdx )
						;
						if( theFile )
							theFile->setRemoteData( theItem );
					}
				}
				else if( theItem.item_type == TYPE_CRYPTO_FILE )
				{
					if( entryIdx == -1 )
					{
						PTR_REMOTE_CRYPTO_FILE	theFile =
							createItem( TYPE_REMOTE_CRYPTO_FILE )
						;
						theFile->setData(
							this,
							theItem.name,
							theItem.description,
							"", "",
							theItem.id,
							theItem.createDate,
							theItem.modifiedDate
						);
						theFile->setRemoteData( theItem );
						myContent->addElement( theFile );
					}
					else
					{
						PTR_REMOTE_CRYPTO_FILE	theFile =
							getContentItem( entryIdx )
						;
						if( theFile )
						{
							theFile->setRemoteData( theItem );
						}
					}
				}
			}
		}
	}
	return myContent;
}

void THE_REMOTE_FOLDER::loadFields( TQuery *query )
{
	THE_LOCAL_FOLDER::loadFields( query );

	RemoteServerID = query->FieldByName( "remote_server_id" )->AsInteger;
	remoteData.id = query->FieldByName( "remote_id" )->AsInteger;
}

void THE_REMOTE_FOLDER::updateDatabase( void )
{
	try
	{
		if( remoteData.id >= 0 && PTR_REMOTE_FOLDER(getParent()) )
		{
			DocManService *theService = RemoteServerForm->GetServiceByID( RemoteServerID );
			remoteData.name = getName();
			remoteData.description = getDescription();
			int newID = theService->sUpdateItem( remoteData );
			if( newID )
				remoteData.id = newID;
		}
	}
	catch( net::SoapRequest::SoapException &e )
	{
		STRING	errText = e.faultString;
		if( !e.faultDetail.isEmpty() )
		{
			errText += ": ";
			errText += e.faultDetail;
		}
/*@*/	throw EDatabaseError( (const char *)errText );
	}
	catch( std::exception &e )
	{
/*@*/	throw Exception( e.what() );
	}

	updateLocalDatabase();
}

void THE_REMOTE_FOLDER::purgeItem( void )
{
	PTR_REMOTE_FOLDER parent( getParent() );
	if( !parent )
		rootPurging = true;
	else
		rootPurging = parent->isRootPurging();

	THE_LOCAL_FOLDER::purgeItem();

	if( !rootPurging )
	{
		try
		{
			DocManService *theService = RemoteServerForm->GetServiceByID(
				RemoteServerID
			);
			theService->sDeleteItem( remoteData.id );
		}
		catch( net::SoapRequest::SoapException &e )
		{
			STRING	errText = e.faultString;
			if( !e.faultDetail.isEmpty() )
			{
				errText += ": ";
				errText += e.faultDetail;
			}
/*@*/		throw EDatabaseError( (const char *)errText );
		}
		catch( std::exception &e )
		{
/*@*/		throw Exception( e.what() );
		}
	}
}

TGraphic *THE_REMOTE_FOLDER::getItemPicture( void ) const
{
	static Graphics::TBitmap *thePic = NULL;

	if( !thePic )
	{
		thePic = new Graphics::TBitmap();
		thePic->LoadFromResourceName((unsigned int)HInstance, "REMOTE_FOLDER_BITMAP");
	}
	return thePic;
}

bool THE_REMOTE_FOLDER::refresh( bool recursive, ostream * )
{
	doEnterFunction("THE_REMOTE_FOLDER::refresh");
	if( StatusForm->pushStatus( "Refreshing", getName() ) )
	{
/*@*/	return false;
	}

	if( !RemoteServerForm->GetServiceConnectedByID( RemoteServerID ) )
	{
/*@*/	throw Exception( "Not connected!" );
	}

	bool	refreshFiles = true;
	STRING	localPath = getDownloadPath();

	if( localPath.isEmpty() )
		refreshFiles = false;

	ITEM_CONTENT	*theContent	= getContent();

	for( size_t i=0; i<theContent->size(); i++ )
	{
		PTR_ITEM			child = getContentItem( i );
		PTR_REMOTE_FILE		file = child;
		if( file )
		{
			if( refreshFiles
			&&  !file->canUnreserve( true )
			&&  file->calcStatus( false ) != "OK" )
			{
				STRING	path = file->getPath();
				STRING	name = file->getName();
				if( StatusForm->pushStatus( "Refreshing", path ) )
/*v*/				break;
				file->download( 0, true, localPath+name );
				if( StatusForm->restore() )
/*v*/				break;
			}
		}
		else if( recursive )
		{
			PTR_REMOTE_FOLDER folder = child;
			if( folder )
				folder->refresh();
		}
	}

	StatusForm->restore();

	return false;
}

void THE_REMOTE_FILE::loadFields( TQuery *query )
{
	THE_FILE_BASE::loadFields( query );

	remoteData.id = query->FieldByName( "remote_id" )->AsInteger;

	getLatestVersion();
}

bool THE_REMOTE_FILE::canReserve( void ) const
{
	bool			result = false;
	DocManService	*theService = getService();

	try
	{
		result = theService->sCanReserve( remoteData.id );
		doLogValue(theService->getBody());
	}
	catch( net::SoapRequest::SoapException &e )
	{
		STRING	errText = e.faultString;
		if( !e.faultDetail.isEmpty() )
		{
			errText += ": ";
			errText += e.faultDetail;
		}
/*@*/	throw EDatabaseError( (const char *)errText );
	}
	catch( std::exception &e )
	{
/*@*/	throw Exception( e.what() );
	}

	return result;
}

bool THE_REMOTE_FILE::canUnreserve( bool ) const
{
	bool result;

	if( getReservedOn() != TDocManDataModule::getMachine() )
		result = false;
	else
	{
		DocManService	*theService = getService();

		try
		{
			result = theService->sCanUnreserve( remoteData.id );
			doLogValue( theService->getBody() );
		}
		catch( net::SoapRequest::SoapException &e )
		{
			STRING	errText = e.faultString;
			if( !e.faultDetail.isEmpty() )
			{
				errText += ": ";
				errText += e.faultDetail;
			}
/*@*/		throw EDatabaseError( (const char *)errText );
		}
		catch( std::exception &e )
		{
/*@*/		throw Exception( e.what() );
		}
	}

	return result;
}

bool THE_REMOTE_FILE::hasChanged(
	const STRING &downloadFile, DateTime *oLocalDate,  DateTime *oRemoteDate
)
{
	bool result = true;

	const DocManService::VersionRecord_t *theVersion = getLatestVersion();

	struct stat localStat;
	if( !strStat( downloadFile, &localStat ) && theVersion->id )
	{
		DateTime	remoteDate( theVersion->modifiedDate );
		DateTime	localDate( localStat.st_mtime );

		if( oRemoteDate )
			*oRemoteDate = remoteDate;
		if( oLocalDate )
			*oLocalDate = localDate;

		long difference = remoteDate.getUtcUnixSeconds() - localDate.getUtcUnixSeconds();
		if( abs( difference ) <= 3 )
			result = false;
	}

	return result;
}

void THE_REMOTE_FILE::reserve( int reserveFor )
{
	if( !canReserve() )
/*@*/	throw Exception( "Item cannot be reserved" );

	STRING		downloadPath;
	PTR_ITEM	theParent = getParent();

	if( theParent )
		downloadPath = theParent->getDownloadPath();

	if( !downloadPath.isEmpty() )
	{
		DateTime	remoteDate, localDate;
		downloadPath += getName();

		struct stat localStat;
		strStat( downloadPath, &localStat );

		chmod( downloadPath, S_IREAD|S_IWRITE );

		if( getLatestVersion()->id
		&&  hasChanged( downloadPath, &localDate, &remoteDate ) )
		{
			STRING	message;
			if( localStat.st_mode & S_IWRITE )
			{
				message = "File is writable and changed.\n";
				message += "Repository: ";
				message += remoteDate.getOriginalTime();
				message += '\n';
				message += "Local: ";
				message += localDate.getOriginalTime();
				message += '\n';
				message += "Overwirte?";
			}
			if( message.isEmpty()
			|| Application->MessageBox( (const char *)message, "DocMan", MB_YESNO|MB_ICONQUESTION ) == IDYES )
			{
				download( 0, false, downloadPath );
			}
		}
	}

	try
	{
		DocManService	*theService = getService();

		THE_FILE_BASE::reserve( reserveFor );

		remoteData.reservedBy = theService->sReserve( remoteData.id );
		doLogValue( theService->getBody() );
	}
	catch( net::SoapRequest::SoapException &e )
	{
		STRING	errText = e.faultString;
		if( !e.faultDetail.isEmpty() )
		{
			errText += ": ";
			errText += e.faultDetail;
		}
/*@*/	throw EDatabaseError( (const char *)errText );
	}
	catch( std::exception &e )
	{
/*@*/	throw Exception( e.what() );
	}
}

void THE_REMOTE_FILE::unreserve(
	bool doBranch, const STRING &description, const STRING &newVersion
)
{
	if( !canUnreserve( false ) )
/*@*/	throw Exception( "Item cannot be unreserved" );

	DocManService	*theService = getService();

	try
	{
		bool	protect = false;		// set to true if the file comes
										// from a local folder
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
			if( hasChanged( newFile ) )
				createVersion( newFile, description );

			if( protect )
				chmod( newFile, S_IREAD );
		}

		theService->sUnreserve( remoteData.id );
		THE_FILE_BASE::cancelReserve();
		remoteData.reservedBy = 0;
	}
	catch( net::SoapRequest::SoapException &e )
	{
		STRING	errText = e.faultString;
		if( !e.faultDetail.isEmpty() )
		{
			errText += ": ";
			errText += e.faultDetail;
		}
/*@*/	throw EDatabaseError( (const char *)errText );
	}
	catch( std::exception &e )
	{
/*@*/	throw Exception( e.what() );
	}
}

void THE_REMOTE_FILE::cancelReserve( void )
{
	if( !canUnreserve( false ) )
/*@*/	throw Exception( "Item cannot be canceled" );

	remoteData.reservedBy = 0;
	THE_FILE_BASE::cancelReserve();

	DocManService	*theService = getService();

	PTR_ITEM	theParent = getParent();
	STRING		downloadPath = theParent
		? theParent->getDownloadPath()
		: NULL_STRING;


	if( !downloadPath.isEmpty() )
	{
		downloadPath += getName();

		DateTime	remoteDate,  localDate;

		if( hasChanged( downloadPath, &localDate, &remoteDate) )
		{
			STRING message = "You have made changes. They will be lost.\n";
			message += "Repository: ";
			message += remoteDate.getOriginalTime();
			message += '\n';
			message += "Local: ";
			message += localDate.getOriginalTime();
			message += '\n';
			message += "Overwirte?";

			if( Application->MessageBox(
				message,
				"DocMan",
				MB_ICONQUESTION|MB_YESNO
			) == IDYES )
			{
				download( 0, true, downloadPath );
			}
			else
/*@*/			throw EAbort("Abort");
		}
		else
			chmod( downloadPath, S_IREAD );
	}

	try
	{
		theService->sUnreserve( remoteData.id );
	}
	catch( net::SoapRequest::SoapException &e )
	{
		STRING	errText = e.faultString;
		if( !e.faultDetail.isEmpty() )
		{
			errText += ": ";
			errText += e.faultDetail;
		}
/*@*/	throw EDatabaseError( (const char *)errText );
	}
	catch( std::exception &e )
	{
/*@*/	throw Exception( e.what() );
	}
}

bool THE_REMOTE_FILE::canCreateVersion( void ) const
{
	return canReserve() || canUnreserve( false );
}

void THE_REMOTE_FILE::createVersion( const STRING &filePath, const STRING &description )
{
	struct stat statBuff;

	if( !strStat( filePath, &statBuff ) )
	{
		ArrayOfData	content;
		readFromFile( &content, filePath );
		STRING encodedData = encodeBase64( content );
		STRING mimeType = MimeTypesForm->GetMimeType( filePath );

		try
		{
			DocManService	*theService = getService();
			theService->sAddVersion(
				remoteData.id,
				mimeType,
				description,
				encodedData,
				statBuff.st_mtime
			);
			clearLatestVersion();
			calcStatus( true );
		}
		catch( net::SoapRequest::SoapException &e )
		{
			STRING	errText = e.faultString;
			if( !e.faultDetail.isEmpty() )
			{
				errText += ": ";
				errText += e.faultDetail;
			}
/*@*/		throw EDatabaseError( (const char *)errText );
		}
		catch( std::exception &e )
		{
/*@*/		throw Exception( e.what() );
		}
	}
	else
	{
		throw Exception( "File not found" );
	}
}

void THE_REMOTE_FILE::deleteVersion( int versionID )
{
	try
	{
		DocManService	*theService = getService();
		theService->sDeleteVersion( remoteData.id, versionID );
		clearLatestVersion();
	}
	catch( net::SoapRequest::SoapException &e )
	{
		STRING	errText = e.faultString;
		if( !e.faultDetail.isEmpty() )
		{
			errText += ": ";
			errText += e.faultDetail;
		}
/*@*/	throw EDatabaseError( (const char *)errText );
	}
	catch( std::exception &e )
	{
/*@*/	throw Exception( e.what() );
	}
}

void THE_REMOTE_FILE::purgeVersions( int numVersions )
{
	doEnterFunction("THE_REMOTE_FILE::purgeVersions");

	if( StatusForm->setStatus( "Purging Versions", getName() ) )
	{
/***/	return;
	}

	DocManService *theService = getService();

	try
	{
		xml::Element *result = theService->sGetVersions(
			getRemoteFileID(), -1
		);
		if( result )
		{
			Array<DocManService::VersionRecord_t>	versions;
			theService->fillVersionRecord( versions, result );

			for(
				Array<DocManService::VersionRecord_t>::const_iterator it = versions.cbegin(),
					endIT = versions.cend();
				it != endIT;
				++it
			)
			{
				const DocManService::VersionRecord_t	&theVersion = *it;
				deleteVersion( theVersion.id );
			}
		}
	}
	catch( net::SoapRequest::SoapException &e )
	{
		STRING	errText = e.faultString;
		if( !e.faultDetail.isEmpty() )
		{
			errText += ": ";
			errText += e.faultDetail;
		}
		doLogValue( theService->getBody() );
/*@*/	throw EDatabaseError( (const char *)errText );
	}
	catch( std::exception &e )
	{
		doLogValue( theService->getBody() );
/*@*/	throw Exception( e.what() );
	}
}

TGraphic *THE_REMOTE_FILE::getStatusPicture( void ) const
{
	if( remoteData.reservedBy )
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

void THE_REMOTE_FILE::updateDatabase( void )
{
	DocManService	*theService = getService();

	try
	{
		remoteData.name = getName();
		remoteData.description = getDescription();
		int newID = theService->sUpdateItem( remoteData );
		if( newID )
			remoteData.id = newID;

		if( !localFile.isEmpty() )
		{
			createVersion( localFile, getDescription() );
			localFile = NULL_STRING;
		}
	}
	catch( net::SoapRequest::SoapException &e )
	{
		STRING	errText = e.faultString;
		if( !e.faultDetail.isEmpty() )
		{
			errText += ": ";
			errText += e.faultDetail;
		}
/*@*/	throw EDatabaseError( (const char *)errText );
	}
	catch( std::exception &e )
	{
		doLogValueEx( gakLogging::llError, theService->getBody() );
		doLogValueEx( gakLogging::llError, e.what() );
/*@*/	throw Exception( e.what() );
	}

	updateLocalDatabase();
}

void THE_REMOTE_FILE::purgeItem( void )
{
	THE_ITEM::purgeItem();

	PTR_REMOTE_FOLDER parent( getParent() );

	if( !parent->isRootPurging() )
	{
		try
		{
			DocManService	*theService = getService();
			theService->sDeleteItem( remoteData.id );
		}
		catch( net::SoapRequest::SoapException &e )
		{
			STRING	errText = e.faultString;
			if( !e.faultDetail.isEmpty() )
			{
				errText += ": ";
				errText += e.faultDetail;
			}
/*@*/		throw EDatabaseError( (const char *)errText );
		}
		catch( std::exception &e )
		{
/*@*/		throw Exception( e.what() );
		}
	}
}

void THE_REMOTE_FOLDER::getItemFactories( Array<const FACTORY_BASE*> *factory ) const
{
	factory->clear();
	factory->addElement( getItemFactory( TYPE_REMOTE_FOLDER ) );
	factory->addElement( getItemFactory( TYPE_REMOTE_FILE ) );
	factory->addElement( getItemFactory( TYPE_REMOTE_CRYPTO_FILE ) );
}

bool THE_REMOTE_FOLDER::acceptDropFiles( void ) const
{
	doEnterFunction("THE_REMOTE_FOLDER::acceptDropFiles");
	bool	acceptFiles;

	const FACTORY_BASE *theFactory = getItemFactory( TYPE_REMOTE_FILE );
	if( theFactory
	&&  theFactory->acceptParent( this ) )
		acceptFiles = true;
	else
		acceptFiles = false;

	return acceptFiles;
}


void THE_REMOTE_FOLDER::dropFile( const STRING &path )
{
	STRING	fileName;

	size_t slashPos = path.searchRChar( DIRECTORY_DELIMITER );
	if( slashPos != (size_t)-1 )
	{
		fileName = path;
		fileName += slashPos+1;

		PTR_ITEM oldItem = getContentItem( fileName );
		if( oldItem )
		{
/* TODO 1 -ogak -cDB : check for updated file */
			PTR_REMOTE_FILE	theFile = oldItem;
			if( theFile )
			{
				theFile->createVersion( path, "" );
				theFile->updateDatabase();
			}
		}
		else
		{
			PTR_REMOTE_FILE	theFile = createItem( TYPE_REMOTE_FILE );
			theFile->setData( this, fileName, "", path, "", 0, time( NULL ), time( NULL ) );
			theFile->updateDatabase();
		}
	}
}

void THE_REMOTE_FOLDER::dropFolder( const STRING &path )
{
	STRING	newPath, fileName;

	size_t slashPos = path.searchRChar( DIRECTORY_DELIMITER );
	if( slashPos != (size_t)-1 )
	{
		fileName = path;
		fileName += slashPos+1;

		PTR_REMOTE_FOLDER	theFolder;
		PTR_ITEM			theItem = getContentItem( fileName );
		if( !theItem )
		{
			theItem = createItem( TYPE_REMOTE_FOLDER );
			theFolder = theItem;
			theFolder->setData( this, fileName, "", "", 0, 0 );
			theFolder->updateDatabase();
		}

		DirectoryList	dirContent;
		dirContent.dirlist( path );
		for(
			DirectoryList::iterator	it = dirContent.begin(), endIT = dirContent.end();
			it != endIT;
			++it
		)
		{
			const DirectoryEntry	&fileFound = *it;
			const STRING			&fileName = fileFound.fileName;

			if( fileName != "." && fileName != ".." )
			{
				newPath = path;
				newPath += DIRECTORY_DELIMITER;
				newPath += fileName;

				if( fileFound.directory )
					theItem->dropFolder( newPath );
				else
					theItem->dropFile( newPath );
			}
		}
	}
}

STRING THE_REMOTE_FILE::getVersionFileName( void )
{
	return getName();
}

STRING THE_REMOTE_FILE::download( int version, bool protect, const STRING &i_dest )
{
	doEnterFunction("THE_REMOTE_FILE::download");

	DocManService	*theService = getService();

	int			serverID = PTR_REMOTE_FOLDER( getParent() )->getServerID();
	STRING		src = RemoteServerForm->GetServerURLbyID(
		serverID
	) + "viewDoc.php?ID=" + formatNumber( remoteData.id );
	if( version )
		src += "&versionID=" + formatNumber( version );

	STRING	dest = i_dest;
	if( dest.isEmpty() )
	{
		dest = version ? getTempDownloadPath() : getDownloadPath();
		dest = getDownloadFile( dest );
	}

	if( version || !i_dest.isEmpty() || getReservedOn() != TDocManDataModule::getMachine() )
	{
		if( (version || hasChanged( dest )) && theService->Get( src ) )
		{
			makePath( dest );

			struct stat localStat;

			if( !strStat( dest, &localStat ) && !(localStat.st_mode & S_IWRITE) )
				protect = true;

			chmod( dest, S_IREAD|S_IWRITE );
			FILE *fp = fopen( dest, "wb" );
			if( fp )
			{
				const net::HTTPclientResponse	&theResponse = theService->getHttpResponse();
				fwrite( theResponse.getBody(), 1, theResponse.getContentLength(), fp );

				fclose( fp );

				if( !version )
				{
					const DocManService::VersionRecord_t *theVersion = getLatestVersion();
					setModTime( dest, theVersion->modifiedDate );

					if( protect )
						chmod( dest, S_IREAD );
				}
			}
		}
	}

	return dest;
}


const char *THE_REMOTE_FILE::compare( int firstVersion, int secondVersion )
{
	STRING		firstFile, secondFile;
	const char *diffResult = NULL;

	PTR_ITEM	theParent = getParent();
	if( theParent )
	{
		if( firstVersion >= 0 )
		{
			firstFile = getenv( "TMP" );
			firstFile += DIRECTORY_DELIMITER_STRING "tmp1.txt";

			firstFile = download( firstVersion, false, firstFile );
		}
		else
		{
			firstFile = theParent->getDownloadPath();
			if( !firstFile.isEmpty() )
				firstFile += getName();
		}
		if( secondVersion >= 0 )
		{
			secondFile = getenv( "TMP" );
			secondFile += DIRECTORY_DELIMITER_STRING "tmp2.txt";

			secondFile = download( secondVersion, false, secondFile );
		}
		else
		{
			secondFile = theParent->getDownloadPath();
			if( !secondFile.isEmpty() )
				secondFile += getName();
		}
	}

	if( !firstFile.isEmpty() && !secondFile.isEmpty() )
	{
		diffResult = diff( firstFile, secondFile );
	}
	else
/*@*/	throw Exception( "No local Path" );

	return diffResult;
}

int THE_REMOTE_FOLDER::getColCount( void ) const
{
	return 7;
}

int *THE_REMOTE_FOLDER::getColWidth( void ) const
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
	};
	return colWidth;
}

COL_TITLE *THE_REMOTE_FOLDER::getColumnTitles( void ) const
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
	};

	return colTitles;
}

void THE_REMOTE_FOLDER::sort( void )
{
	getContent()->sort( itemCompare, getSortType() );
}

STRING THE_REMOTE_FOLDER::drawCell( int col, int row, TCanvas *canvas, TRect &Rect )
{
	STRING text;

	if( col == 6 )
	{
		PTR_FILE_BASE	theItem = getContentItem( row );
		if( theItem )
			text = theItem->calcStatus( false );
	}
	else
		text = THE_LOCAL_FOLDER::drawCell( col, row, canvas, Rect );

	return text;
}
// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

bool THE_REMOTE_FOLDER::isRootPurging( void )
{
	if( !rootPurging )
	{
		PTR_REMOTE_FOLDER	parent( getParent() );
		if( parent )
			rootPurging = parent->isRootPurging();
	}

	return rootPurging;
}

void THE_REMOTE_FOLDER::updateLocalDatabase( void )
{
	TDateTime createdDate = getCreatedDate();
	TDateTime modifiedDate = getModifiedDate();

	THE_LOCAL_FOLDER::updateDatabase();

	if( !getPermissionID() )
		setPermissionID( getID(), getUserPermissions() );

	std::auto_ptr<TQuery>	theQuery( new TQuery( NULL ) );
	theQuery->DatabaseName = "docManDB";

	theQuery->SQL->Add(
		"update item_tree "
		"set remote_server_id = :RemoteServerID, "
			"remote_id = :RemoteFolderID, "
			"modifiedDate = :mDate, "
			"createdDate = :cDate, "
			"permission_id = :permissionID "
		"where id=:Id"
	);
	theQuery->Params->Items[0]->AsInteger = RemoteServerID;
	theQuery->Params->Items[1]->AsInteger = remoteData.id;
	theQuery->Params->Items[2]->AsDateTime = modifiedDate;
	theQuery->Params->Items[3]->AsDateTime = createdDate;
	theQuery->Params->Items[4]->AsInteger = getPermissionID();
	theQuery->Params->Items[5]->AsInteger = getID();

	theQuery->ExecSQL();
}

void THE_REMOTE_FILE::updateLocalDatabase( void )
{
	TDateTime createdDate = getCreatedDate();
	TDateTime modifiedDate = getCreatedDate();

	THE_FILE_BASE::updateDatabase();

	std::auto_ptr<TQuery>	theQuery( new TQuery( NULL ) );
	theQuery->DatabaseName = "docManDB";

	theQuery->SQL->Add(
		"update item_tree "
		"set remote_id = :RemoteFolderID, "
			"modifiedDate = :mDate, "
			"createdDate = :cDate "
		"where id=:Id"
	);
	theQuery->Params->Items[0]->AsInteger = remoteData.id;
	theQuery->Params->Items[1]->AsDateTime = modifiedDate;
	theQuery->Params->Items[2]->AsDateTime = createdDate;
	theQuery->Params->Items[3]->AsInteger = getID();

	theQuery->ExecSQL();
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

