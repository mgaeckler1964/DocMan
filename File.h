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

#ifndef ITEM_FILE_HEADER_H
#define ITEM_FILE_HEADER_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/htmlParser.h>
#include <gak/stringStream.h>

#include <gak/exif.h>
#include <graphix/magic.h>

#include "ItemManager.h"
#include "DocManDM.h"

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

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

extern const char STATUS_OK[];
extern const char STATUS_OLDER[];
extern const char STATUS_NEWER[];
extern const char STATUS_MISSING[];
extern const char STATUS_RESERVED[];

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //
class FACTORY_FILE : public FACTORY_BASE
{
	public:

	virtual int getItemType( void ) const;
	virtual bool acceptParent( const THE_ITEM *parent ) const;
	virtual PTR_ITEM createItem( int id ) const;
	virtual const char *getName( void ) const;
	virtual TItemCreateForm *getForm( void ) const;
	virtual PTR_ITEM createItemFromForm( const PTR_ITEM &parent ) const;
};

class THE_FILE_VERSION
{
	private:
	int				versionID, version, storageID, storageUsageCount, locked;
	unsigned long	size;
	STRING			mimeType, fileName, storagePath;
	TDateTime		versionCreatedDate, versionModifiedDate;
	TDateTime		fileCreatedDate, fileModifiedDate;

	public:
	THE_FILE_VERSION( int fileId, int version=0 );

	STRING getMimeType( void ) const
	{
		return mimeType;
	}
	STRING getStoragePath( void ) const
	{
		return storagePath;
	}
	STRING getFileName( void ) const
	{
		return fileName;
	}

	int getStorageID( void ) const
	{
		return storageID;
	}
	int getVersionID( void ) const
	{
		return versionID;
	}
	int getUsageCount( void ) const
	{
		return storageUsageCount;
	}
	int incrUsageCount( void )
	{
		return ++storageUsageCount;
	}
	unsigned long getSize( void ) const
	{
		return size;
	}
	TDateTime getVersionModifiedDate( void ) const
	{
		return versionModifiedDate;
	}
	TDateTime getVersionCreationDate( void ) const
	{
		return versionCreatedDate;
	}
	TDateTime getFileModifiedDate( void ) const
	{
		return fileModifiedDate;
	}
	TDateTime getFileCreationDate( void ) const
	{
		return fileCreatedDate;
	}
	int getVersionNum( void ) const
	{
		return version;
	}
	bool getLocked( void ) const
	{
		return locked ? true : false;
	}
};

class THE_FILE_BASE : public THE_ITEM
{
	STRING				mimeType, reservedOn, status;
	int					reservedBy, reservedFor;
	TDateTime			fileModifiedDate;
	unsigned long		fileSize;

	public:
	THE_FILE_BASE( int flags, int id, const FACTORY_BASE *theFactory )
	: THE_ITEM( flags, id, theFactory )
	{
		reservedFor = reservedBy = 0;
	}

	virtual STRING getDownloadPath( PTR_ITEM parent=PTR_ITEM() );
	virtual TGraphic *getItemPicture( void ) const;

	static STRING getTempDownloadPath( void );

	virtual void loadFields( TQuery *query );
	virtual void updateDatabase( void );

	void openVersion( int version )
	{
		STRING dest = download( version, false, "" );
		ShellExecute( NULL, NULL, dest, NULL, NULL, SW_SHOWDEFAULT );
	}
	virtual void open( void );
	virtual STRING getVersionFileName( void ) = 0;
	virtual STRING download( int version, bool protect, const STRING &dest ) = 0;
	virtual STRING getDownloadFile( const STRING &downloadPath );
	STRING getDownloadFile( const PTR_ITEM &parent=PTR_ITEM() )
	{
#pragma warn -inl
		return getDownloadFile( getDownloadPath( parent ) );
#pragma warn +inl
	}

	protected:
	void setMimeType( const STRING &theMimeType )
	{
		mimeType = theMimeType;
	}
	public:
	const STRING &getMimeType( void ) const
	{
		return mimeType;
	}
	protected:
	void setFileSize( unsigned long fileSize )
	{
		this->fileSize = fileSize;
	}
	public:
	unsigned long getSizeInBytes( void )
	{
		return fileSize;
	}

	protected:
	void setFileModifiedDate( TDateTime fileModifiedDate )
	{
		this->fileModifiedDate = fileModifiedDate;
	}

	public:
	TDateTime getFileModifiedDate( void ) const
	{
		return fileModifiedDate;
	}

	virtual bool canCreateVersion( void ) const = 0;
	virtual void createVersion( const STRING &filePath, const STRING &description ) = 0;
	virtual bool hasChanged( const STRING &dest, gak::DateTime *oLocalDate=NULL, gak::DateTime *oRemoteDate=NULL ) = 0;
	virtual const char *compare( int firstVersion=0, int secondVersion=-1  ) = 0;
	virtual bool canReserve( void ) const = 0;
	virtual bool canUnreserve( bool noAdminCheck ) const = 0;
	virtual void reserve( int taskID=0, bool doNotOverwrite=false )
	{
		setTask( taskID );
		reservedBy = gak::vcl::getActUserID();
		reservedOn = TDocManDataModule::getMachine();
		setStatus( STATUS_RESERVED );
	}
	void setTask( int taskID=0 )
	{
		this->reservedFor = taskID;
	}
	virtual void unreserve( bool doBranch, const STRING &description, const STRING &newVersion="" ) = 0;

	virtual void cancelReserve( void )
	{
		reservedBy = 0;
		reservedFor = 0;
		reservedOn = "";
		setStatus( STATUS_OK );
	}
	const STRING &getReservedOn( void ) const
	{
		return reservedOn;
	}
	int getTask( void ) const
	{
		return reservedFor;
	}
	int getReservedBy( void ) const
	{
		return reservedBy;
	}
	void setStatus( const STRING &newStatus )
	{
		status = newStatus;
	}
	const STRING &calcStatus( bool force );
	const STRING &getStatus( void ) const
	{
		return status;
	}
	virtual TGraphic *getStatusPicture( void ) const;
};
typedef PTR_TEMPLATE<THE_FILE_BASE> PTR_FILE_BASE;

class THE_FILE : public THE_FILE_BASE
{
	int					m_fileID;
	gak::F_STRING		m_newFilePath, m_previousFilePath;

	THE_FILE_VERSION	*m_latestVersion;

	static STRING		s_externalStorage;

	int createStorage( const STRING &filePath );

	THE_FILE_VERSION *getVersion( int version )
	{
		return new THE_FILE_VERSION( m_fileID, version );
	}
	THE_FILE_VERSION *getLatestVersion( void )
	{
		if( !m_latestVersion )
		{
			m_latestVersion = getVersion( 0 );
			setFileSize( m_latestVersion->getSize() );
			setFileModifiedDate( m_latestVersion->getFileModifiedDate() );
			setMimeType( m_latestVersion->getMimeType() );
		}

		return m_latestVersion;
	}
	void clearLatestVersion( void )
	{
		if( m_latestVersion )
		{
			delete m_latestVersion;
			m_latestVersion = NULL;
		}
	}
	THE_FILE_VERSION *refreshLatestVersion( void )
	{
		clearLatestVersion();
		return getLatestVersion();
	}
	STRING getStoragePath( int theVersionNum );

	protected:
	THE_FILE( int flags, int id, const FACTORY_BASE *theFactory )
	: THE_FILE_BASE( flags, id, theFactory )
	{
		m_fileID = 0;
		m_latestVersion = NULL;
	}
	public:
	THE_FILE( int id, const FACTORY_BASE *theFactory )
	: THE_FILE_BASE(
		ITEM_IS_BROWSEABLE|ITEM_IS_MOVEABLE|ITEM_IS_COPYABLE|ITEM_IS_VERSIONED,
		id, theFactory
	)
	{
		m_fileID = 0;
		m_latestVersion = NULL;
	}
	~THE_FILE();
	void setData( const PTR_ITEM &parent, const STRING &name, const STRING &description, const STRING &newFilePath )
	{
		THE_ITEM::setData( parent, name, description );
		m_newFilePath = newFilePath;
	}
	int getFileID( void ) const
	{
		return m_fileID;
	}

	virtual void loadFields( TQuery *query );
	virtual int loadPermissions( void );
	virtual void updateDatabase( void );

	virtual bool canCreateVersion( void ) const;
	int createVersion( int srcVersion );
	virtual void createVersion( const STRING &filePath, const STRING &description );
	void deleteVersion( int versionId, int storageId, bool updateMaxVersion );
	virtual STRING getSize( void );
	int getVersionNum( void )
	{
		THE_FILE_VERSION *latestVersion = getLatestVersion();

		return latestVersion ? latestVersion->getVersionNum() : 0;
	}
	int getStorageID( void )
	{
		return getLatestVersion()->getStorageID();
	}
	TDateTime getFileCreationDate( void )
	{
		return getLatestVersion()->getFileCreationDate();
	}


	TDateTime getVersionModifiedDate( void )
	{
		return getLatestVersion()->getVersionModifiedDate();
	}

	static void setExternalStorage( const STRING &newPath )
	{
		s_externalStorage = newPath;
		if( !s_externalStorage.isEmpty() )
		{
			if( !s_externalStorage.endsWith( DIRECTORY_DELIMITER ) )
			{
				s_externalStorage += DIRECTORY_DELIMITER;
			}
			if( strAccess( s_externalStorage + '.', 0 ) )
			{
				s_externalStorage = "";
			}
		}
	}
	static const STRING &getExternalStorageBase( void )
	{
		return s_externalStorage;
	}
	STRING getExternalFile( int theVersionNum=0 )
	{
#pragma warn -inl
		return getExternalStorageBase()+ getStoragePath( theVersionNum );
#pragma warn +inl
	}

	virtual PTR_ITEM copy(
		ItemsCopiedMap &itemsCopied,
		const PTR_ITEM &target, bool createLink, bool moveReserve,
		const STRING &newName=""
	);
	PTR_ITEM link( const PTR_ITEM &target, const STRING &newName="" );
	void branch( void );

	virtual STRING download( int version, bool protect, const STRING &dest );
	virtual STRING getVersionFileName( void );

	virtual bool canDelete( bool forPurge, bool recursive  );
	virtual void purgeItem( void );
	virtual void purgeVersions( int numVersions );

	virtual bool canReserve( void ) const;
	virtual bool canUnreserve( bool noAdminCheck ) const;
	virtual void reserve( int reserveFor=0, bool doNotOverwrite=false );
	virtual void unreserve( bool doBranch, const STRING &description, const STRING &newVersion="" );
	virtual void cancelReserve( void );
	virtual const char *compare( int firstVersion=0, int secondVersion=-1  );
	virtual bool hasReserved( const STRING &machine, int userId );
	virtual bool hasChanged( const STRING &dest, gak::DateTime *oLocalDate=NULL, gak::DateTime *oRemoteDate=NULL );
	STRING getContent( void )
	{
		STRING	src = getExternalFile();
		STRING	content;

		content.readFromFile( src );

#pragma warn -inl
		return content;
#pragma warn +inl
	}

	STRING getCSScontent( void )
	{
		doEnterFunctionEx(gakLogging::llDetail, "THE_FILE::getCSScontent");

		STRING	mimeType = getMimeType();
		STRING	content = mimeType == "text/css" ? getContent() : (STRING)"";

#pragma warn -inl
		return content;
#pragma warn +inl
	}
	gak::xml::Document *getXmlDocument( void )
	{
		doEnterFunctionEx(gakLogging::llDetail, "THE_FILE::getXmlDocument");
		gak::xml::Document *result = NULL;
		STRING mimeType = getMimeType();

		if( mimeType == "text/xml"
		|| mimeType == "text/xsl"
		|| mimeType == "text/xsd" )
		{
			STRING				content = getContent();
			gak::iSTRINGstream	theInput( content );
			gak::xml::Parser	xmlParser( &theInput, getPath() );

			result = xmlParser.readFile( false );
		}

		return result;
	}
	gak::html::Document *getHtmlDocument( void )
	{
		doEnterFunctionEx(gakLogging::llDetail, "THE_FILE::getHtmlDocument");

		gak::html::Document	*result = NULL;
		STRING 				mimeType = getMimeType();

		if( mimeType == "text/html" )
		{
			STRING				content = getContent();
			gak::iSTRINGstream	theInput( content );
			gak::html::Parser	htmlParser( &theInput, getPath() );

			result = htmlParser.readFile( false );
		}

		return result;
	}
	virtual void createXMLattributes( gak::xml::Element *target );

	virtual void lock( void );
	virtual void unlock( void );

	STRING generateWebFile( bool forWebServer, STRING &mimeType );

	bool loadImageMetaData( gak::ImageMetaData *metaData, int theVersionNum )
	{
		doEnterFunctionEx(gakLogging::llDetail, "THE_FILE::loadImageMetaData");

		bool	success = false;
		STRING	fileName = getExternalFile( theVersionNum );

		if( !fileName.isEmpty() )
		{
			doLogValue( fileName );

			cBool			dummy;
			Magic			fType = MAGICgetFormat( fileName, &dummy );

			if( (fType == MAGIC_JPEG_IMAGE || fType == MAGIC_MOTOROLA_TIFF || fType == MAGIC_INTEL_TIFF)
			&& readImageMetaData( fileName, metaData ) )
			{
				success = true;
			}
		}

		return success;
	}
	bool updateImageMetaData( gak::ImageMetaData *metaData, int theVersionNum )
	{
		doEnterFunctionEx(gakLogging::llDetail, "THE_FILE::updateImageMetaData");

		bool success = loadImageMetaData( metaData, theVersionNum );
		if( success )
		{
			DocManDataModule->updateExifData( this, theVersionNum, metaData );
		}

		return success;
	}
};
typedef PTR_TEMPLATE<THE_FILE> PTR_FILE;

class THE_FILE_REF : public THE_ITEM
{
	gak::F_STRING	m_previousFilePath;
	STRING			m_md5CheckSum;
	unsigned long	m_size;

	public:
	THE_FILE_REF( int id, const FACTORY_BASE *theFactory )
	: THE_ITEM(
		ITEM_IS_BROWSEABLE, id, theFactory
	)
	{
		m_size = 0;
	}
	virtual void open( void );
	void setData(
		const PTR_ITEM		&parent,
		const STRING		&name, const STRING &description,
		const STRING		&md5CheckSum,
		const gak::DateTime	&createdDate,
		const gak::DateTime	&modifiedDate
	)
	{
		doEnterFunctionEx(gakLogging::llDetail, "THE_FILE_REF::setData");
		doLogValue(m_previousFilePath);
		THE_ITEM::setData(
			parent,
			name, description
		);
		doLogValue(name);
		doLogValue(m_previousFilePath);
		m_md5CheckSum = md5CheckSum;
		setCreatedDate( createdDate );
		setModifiedDate( modifiedDate );
	}
	void setMd5CheckSum( const STRING &md5CheckSum )
	{
		m_md5CheckSum = md5CheckSum;
	}
	const STRING &getMD5CheckSum( void ) const
	{
		return m_md5CheckSum;
	}
	virtual void loadFields( TQuery *query );
	virtual void updateDatabase( void );

	virtual TGraphic *getItemPicture( void ) const;
	virtual TGraphic *getStatusPicture( void ) const;
	virtual STRING getSize( void );
	unsigned long getSizeInBytes( void )
	{
		return m_size;
	}
	bool loadImageMetaData( gak::ImageMetaData *metaData )
	{
		doEnterFunctionEx(gakLogging::llDetail, "THE_FILE_REF::loadImageMetaData");

		bool	success = false;
		STRING	fileName = getDownloadPath();

		if( !fileName.isEmpty() )
		{
			doLogValue( fileName );
			cBool			dummy;
			Magic			fType = MAGICgetFormat( fileName, &dummy );

			if( (fType == MAGIC_JPEG_IMAGE || fType == MAGIC_MOTOROLA_TIFF || fType == MAGIC_INTEL_TIFF)
			&& readImageMetaData( fileName, metaData ) )
			{
				success = true;
			}
		}

		return success;
	}
	bool updateImageMetaData( gak::ImageMetaData *metaData )
	{
		doEnterFunctionEx(gakLogging::llDetail, "THE_FILE_REF::updateImageMetaData");
		bool success = loadImageMetaData( metaData );
		if( success )
		{
			success = DocManDataModule->updateExifData( this, 0, metaData );
		}

		return success;
	}
};
typedef PTR_TEMPLATE<THE_FILE_REF> PTR_FILE_REF;

class THE_LANGUAGE_DOC : public THE_ITEM
{
	public:
	THE_LANGUAGE_DOC( int id, const FACTORY_BASE *theFactory )
	: THE_ITEM(
		ITEM_IS_CONTAINER|ITEM_IS_BROWSEABLE|ITEM_IS_MOVEABLE|ITEM_IS_COPYABLE,
		id, theFactory
	)
	{
	}
	virtual TGraphic *getItemPicture( void ) const;
	virtual STRING getDownloadPath( PTR_ITEM parent=PTR_ITEM() );
};
typedef PTR_TEMPLATE<THE_LANGUAGE_DOC> PTR_LANGUAGE_DOC;

class THE_LANGUAGE_ITEM : public THE_FILE
{
	public:
	THE_LANGUAGE_ITEM( int id, const FACTORY_BASE *theFactory )
	: THE_FILE( ITEM_IS_BROWSEABLE, id, theFactory )
	{
	}
	virtual STRING getDownloadFile( const STRING &downloadPath );
};
typedef PTR_TEMPLATE<THE_LANGUAGE_ITEM> PTR_LANGUAGE_ITEM;

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

gak::xml::Document *loadXmlDoc( const STRING &xmlFile );
STRING loadCssDoc( const STRING &cssFile );

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
#	pragma option -a.
#	pragma option -p.
#endif


#endif
