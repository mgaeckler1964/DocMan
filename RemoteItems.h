/*
		Project:		DocMan
		Module:			RemoteItems.h
		Description:	Manage files and folders on the remnote server
		Author:			Martin Gäckler
		Address:		Hofmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2025 Martin Gäckler

		This program is free software: you can redistribute it and/or modify
		it under the terms of the GNU General Public License as published by
		the Free Software Foundation, version 3.

		You should have received a copy of the GNU General Public License
		along with this program. If not, see <http://www.gnu.org/licenses/>.

		THIS SOFTWARE IS PROVIDED BY Martin Gäckler, Linz, Austria ``AS IS''
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

#ifndef DOC_REMOTE_ITEMS_H
#define DOC_REMOTE_ITEMS_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include "Folder.h"
#include "File.h"
#include "RemoteServerFrm.h"
#include "DocManService.h"

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

class FACTORY_REMOTE_FILE : public FACTORY_FILE
{
	virtual bool acceptParent( const THE_ITEM *parent ) const;
	virtual int getItemType( void ) const;
	virtual PTR_ITEM createItem( int id ) const;
	virtual const char *getName( void ) const;
	virtual PTR_ITEM createItemFromForm( const PTR_ITEM &parent ) const;
};

class THE_REMOTE_FOLDER : public THE_LOCAL_FOLDER
{
	bool							rootPurging;
	int								RemoteServerID;
	DocManService::ItemRecord_t		remoteData;

	size_t	findEntryByRemoteID( int remoteID );

	public:
	THE_REMOTE_FOLDER( int id, const FACTORY_BASE *theFactory )
	: THE_LOCAL_FOLDER( id, theFactory, ITEM_IS_BROWSEABLE|ITEM_IS_CONTAINER )
	{
		rootPurging = false;
		remoteData.id = 0;
		remoteData.parentID = -1;
		remoteData.item_type = TYPE_FOLDER;
	}
	virtual TGraphic *getItemPicture( void ) const;

	void setRemoteData( const DocManService::ItemRecord_t &remoteData )
	{
		bool	changedFlag = false;

		this->remoteData = remoteData;
		if( remoteData.name != getName() )
		{
			setName( remoteData.name );
			changedFlag = true;
		}
		if( remoteData.description != getDescription() )
		{
			setDescription( remoteData.description );
			changedFlag = true;
		}

		this->setCreatedDate( remoteData.createDate );
		this->setModifiedDate( remoteData.modifiedDate );

		if( changedFlag || !getID() )
			updateLocalDatabase();
	}
	void setData(
		const PTR_ITEM &parent, const STRING &name, const STRING &description,
		const STRING &newLocalPath, int RemoteServerID, int RemoteFolderID
	);

	virtual ITEM_CONTENT *loadContent( void );
	virtual void loadFields( TQuery *query );
	virtual void updateDatabase( void );
	virtual void purgeItem( void );
	void updateLocalDatabase( void );

	virtual void getItemFactories( Array<const FACTORY_BASE*> *factory ) const;
	virtual bool acceptDropFiles( void ) const;
	virtual void dropFile( const STRING &path );
	virtual void dropFolder( const STRING &path );

	int getServerID( void ) const
	{
		return RemoteServerID;
	}
	DocManService *getService( void ) const
	{
		DocManService	*theService = RemoteServerForm->GetServiceByID(
			RemoteServerID
		);

		return theService;
	}
	int getRemoteFolderID( void ) const
	{
		return remoteData.id;
	}
	const DocManService::ItemRecord_t &getRemoteData( void ) const
	{
		return remoteData;
	}
	bool isRootPurging( void );
	virtual bool refresh( bool recursive = true, std::ostream *stream=NULL );

	virtual ColumnTitle *getColumnTitles( void ) const;
	virtual int getColCount( void ) const;
	virtual int *getColWidth( void ) const;
	virtual void sort( void );
	virtual STRING drawCell( int col, int row, TCanvas *canvas, TRect &Rect );
};
typedef PTR_TEMPLATE<THE_REMOTE_FOLDER> PTR_REMOTE_FOLDER;

class THE_REMOTE_FILE : public THE_FILE_BASE
{
	protected:
	DocManService::ItemRecord_t		remoteData;
	DocManService::VersionRecord_t	latestVersion;
	STRING							localFile;

	virtual void loadFields( TQuery *query );

	const DocManService::VersionRecord_t *getLatestVersion( void );
	void clearLatestVersion( void )
	{
		latestVersion.id = 0;
	}
	const DocManService::VersionRecord_t *refreshLatestVersion( void )
	{
		clearLatestVersion();
		return getLatestVersion();
	}

	public:
	THE_REMOTE_FILE( int id, const FACTORY_BASE *theFactory )
	: THE_FILE_BASE(
		ITEM_IS_BROWSEABLE|ITEM_IS_VERSIONED,
		id, theFactory
	)
	{
		remoteData.id = 0;
		remoteData.parentID = -1;
		remoteData.item_type = TYPE_DOCUMENT;
		latestVersion.id = 0;
	}
	void setRemoteData( const DocManService::ItemRecord_t &remoteData )
	{
		bool	changedFlag = false;

		this->remoteData = remoteData;
		if( remoteData.name != getName() )
		{
			setName( remoteData.name );
			changedFlag = true;
		}
		if( remoteData.description != getDescription() )
		{
			setDescription( remoteData.description );
			changedFlag = true;
		}
		if( !remoteData.reservedBy && getReservedBy() )
		{
			THE_FILE_BASE::cancelReserve();
			changedFlag = true;
		}

		this->setCreatedDate( remoteData.createDate );
		this->setModifiedDate( remoteData.modifiedDate );

		if( changedFlag || !getID() )
			updateLocalDatabase();
	}
	void setData(
		const PTR_ITEM &parent, const STRING &name, const STRING &description,
		const STRING &localFile, const STRING &mimeType,
		int RemoteFileID,
		time_t createdDate, time_t modifiedDate
	)
	{
		THE_ITEM::setData( parent, name, description );
		this->localFile = localFile;
		remoteData.id = RemoteFileID;
		this->setCreatedDate( createdDate );
		this->setModifiedDate( modifiedDate );
		this->setMimeType( mimeType );
		PTR_REMOTE_FOLDER	remoteParent = parent;
		if( remoteParent )
		{
			remoteData.parentID = remoteParent->getRemoteFolderID();
			remoteData.mode = remoteParent->getRemoteData().mode;
			remoteData.ownerGroup = remoteParent->getRemoteData().ownerGroup;
			setPermissionID(
				remoteParent->getPermissionID(),
				remoteParent->getUserPermissions()
			);
		}
	}
	virtual TGraphic *getStatusPicture( void ) const;

	virtual bool hasChanged( const STRING &dest, gak::DateTime *oLocalDate=NULL, gak::DateTime *oRemoteDate=NULL );
	bool needDownload( const STRING &dest )
	{
		return !canUnreserve( true ) && hasChanged( dest );
	}
	virtual const char *compare( int firstVersion=0, int secondVersion=-1  );
	virtual bool canReserve( void ) const;
	virtual bool canUnreserve( bool noAdminCheck ) const;
	virtual void reserve( int reserveFor=0, bool doNotOverwrite=false );
	virtual void unreserve( bool doBranch, const STRING &description, const STRING &newVersion="" );
	virtual void cancelReserve( void );

	virtual bool canCreateVersion( void ) const;
	virtual void createVersion( const STRING &filePath, const STRING &description );
	void deleteVersion( int versionId );
	void purgeVersions( int numVersions );

	virtual void updateDatabase( void );
	void updateLocalDatabase( void );
	virtual void purgeItem( void );
	int getRemoteFileID( void ) const
	{
		return remoteData.id;
	}

	virtual STRING download( int version = 0, int flags=0, const STRING &dest = STRING((const char*)NULL) );
	virtual STRING getVersionFileName( void );

	DocManService *getService( void ) const
	{
		DocManService *theService = PTR_REMOTE_FOLDER(getParent())->getService();

		return theService;
	}
};
typedef PTR_TEMPLATE<THE_REMOTE_FILE> PTR_REMOTE_FILE;

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

inline void THE_REMOTE_FOLDER::setData(
	const PTR_ITEM &parent, const STRING &name, const STRING &description,
	const STRING &newLocalPath, int RemoteServerID, int RemoteFolderID
)
{
	THE_LOCAL_FOLDER::setData( parent, name, description, newLocalPath );
	remoteData.id = RemoteFolderID;
	PTR_REMOTE_FOLDER	remoteParent = parent;
	if( remoteParent )
	{
		this->RemoteServerID = remoteParent->RemoteServerID;
		remoteData.parentID = remoteParent->getRemoteFolderID();
		remoteData.mode = remoteParent->getRemoteData().mode;
		remoteData.ownerGroup = remoteParent->getRemoteData().ownerGroup;
		int permissionID = remoteParent->getPermissionID();
		if( !permissionID )
			permissionID = remoteParent->getID();
		setPermissionID( permissionID, remoteParent->getUserPermissions() );
	}
	else
		this->RemoteServerID = RemoteServerID;
}
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

#endif

