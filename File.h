/*
		Project:		DocMan
		Module:			File.h
		Description:	the files stored in our database
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
extern const char STATUS_CHECKSUM[];

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

	virtual int getItemType() const;
	virtual bool acceptParent( const THE_ITEM *parent ) const;
	virtual PTR_ITEM createItem( int id ) const;
	virtual const char *getName() const;
	virtual TItemCreateForm *getForm() const;
	virtual PTR_ITEM createItemFromForm( const PTR_ITEM &parent ) const;
};

class THE_FILE_VERSION
{
	private:
	int				m_versionID, m_version, m_storageID, m_storageUsageCount,
					m_locked;
	unsigned long	m_size;
	STRING			m_mimeType, m_fileName, m_storagePath;
	TDateTime		m_versionCreatedDate, m_versionModifiedDate;
	TDateTime		m_fileCreatedDate, m_fileModifiedDate;
	STRING			m_md5CheckSum;

	public:
	THE_FILE_VERSION( int fileId, int version=0 );

	const STRING &getMimeType() const
	{
		return m_mimeType;
	}
	const STRING &getStoragePath() const
	{
		return m_storagePath;
	}
	const STRING &getFileName() const
	{
		return m_fileName;
	}
	const STRING &getMD5checksum() const
	{
		return m_md5CheckSum;
	}
	int getStorageID() const
	{
		return m_storageID;
	}
	int getVersionID() const
	{
		return m_versionID;
	}
	int getUsageCount() const
	{
		return m_storageUsageCount;
	}
	int incrUsageCount()
	{
		return ++m_storageUsageCount;
	}
	unsigned long getSize() const
	{
		return m_size;
	}
	TDateTime getVersionModifiedDate() const
	{
		return m_versionModifiedDate;
	}
	TDateTime getVersionCreationDate() const
	{
		return m_versionCreatedDate;
	}
	TDateTime getFileModifiedDate() const
	{
		return m_fileModifiedDate;
	}
	TDateTime getFileCreationDate() const
	{
		return m_fileCreatedDate;
	}
	int getVersionNum() const
	{
		return m_version;
	}
	bool getLocked() const
	{
		return m_locked ? true : false;
	}
};

class THE_FILE_BASE : public THE_ITEM
{
	STRING				m_mimeType, m_reservedOn, m_status;
	int					m_reservedBy, m_reservedFor;
	TDateTime			m_fileModifiedDate;
	unsigned long		m_fileSize;

	public:
	THE_FILE_BASE( int flags, int id, const FACTORY_BASE *theFactory )
	: THE_ITEM( flags, id, theFactory )
	{
		m_reservedFor = m_reservedBy = 0;
	}

	virtual STRING getDownloadPath( PTR_ITEM parent=PTR_ITEM() );
	virtual TGraphic *getItemPicture() const;

	static STRING getTempDownloadPath();

	virtual void loadFields( TQuery *query );
	virtual void updateDatabase();

	void openVersion( int version )
	{
		STRING dest = download( version, false, "" );
		ShellExecute( NULL, NULL, dest, NULL, NULL, SW_SHOWDEFAULT );
	}
	virtual void open();
	virtual STRING getVersionFileName() = 0;
	virtual STRING download( int version, bool protect, const STRING &dest ) = 0;
	virtual STRING getDownloadFile( const STRING &downloadPath );
	STRING getDownloadFile( const PTR_ITEM &parent=PTR_ITEM() )
	{
#pragma warn -inl
		return getDownloadFile( getDownloadPath( parent ) );
#pragma warn +inl
	}

	protected:
	void setMimeType( const STRING &mimeType )
	{
		m_mimeType = mimeType;
	}
	public:
	const STRING &getMimeType() const
	{
		return m_mimeType;
	}
	protected:
	void setFileSize( unsigned long fileSize )
	{
		m_fileSize = fileSize;
	}
	public:
	unsigned long getSizeInBytes() const
	{
		return m_fileSize;
	}

	protected:
	void setFileModifiedDate( TDateTime fileModifiedDate )
	{
		m_fileModifiedDate = fileModifiedDate;
	}

	public:
	TDateTime getFileModifiedDate() const
	{
		return m_fileModifiedDate;
	}

	virtual bool canCreateVersion() const = 0;
	virtual void createVersion( const STRING &filePath, const STRING &description ) = 0;
	virtual bool hasChanged( const STRING &dest, gak::DateTime *oLocalDate=NULL, gak::DateTime *oRemoteDate=NULL ) = 0;
	virtual const char *compare( int firstVersion=0, int secondVersion=-1  ) = 0;
	virtual bool canReserve() const = 0;
	virtual bool canUnreserve( bool noAdminCheck ) const = 0;
	virtual void reserve( int taskID=0, bool doNotOverwrite=false )
	{
		setTask( taskID );
		m_reservedBy = gak::vcl::getActUserID();
		m_reservedOn = TDocManDataModule::getMachine();
		setStatus( STATUS_RESERVED );
	}
	void setTask( int taskID=0 )
	{
		m_reservedFor = taskID;
	}
	virtual void unreserve( bool doBranch, const STRING &description, const STRING &newVersion="" ) = 0;

	virtual void cancelReserve()
	{
		m_reservedBy = 0;
		m_reservedFor = 0;
		m_reservedOn = "";
		setStatus( STATUS_OK );
	}
	const STRING &getReservedOn() const
	{
		return m_reservedOn;
	}
	int getTask() const
	{
		return m_reservedFor;
	}
	int getReservedBy() const
	{
		return m_reservedBy;
	}
	void setStatus( const STRING &status )
	{
		m_status = status;
	}
	const STRING &calcStatus( bool force );
	const STRING &getStatus() const
	{
		return m_status;
	}
	virtual TGraphic *getStatusPicture() const;
};
typedef PTR_TEMPLATE<THE_FILE_BASE> PTR_FILE_BASE;

class THE_FILE : public THE_FILE_BASE
{
	int					m_fileID;
	gak::F_STRING		m_newFilePath, m_previousFilePath;

	THE_FILE_VERSION	*m_latestVersion;
	STRING				m_md5hash;

	static STRING		s_externalStorage;

	int createStorage( const STRING &filePath, STRING *md5Hash );

	THE_FILE_VERSION *getVersion( int version )
	{
		return new THE_FILE_VERSION( m_fileID, version );
	}
	THE_FILE_VERSION *getLatestVersion()
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
	void clearLatestVersion()
	{
		if( m_latestVersion )
		{
			delete m_latestVersion;
			m_latestVersion = NULL;
		}
	}
	THE_FILE_VERSION *refreshLatestVersion()
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
	int getFileID() const
	{
		return m_fileID;
	}

	virtual void loadFields( TQuery *query );
	virtual int loadPermissions();
	virtual void updateDatabase();

	virtual bool canCreateVersion() const;
	int createVersion( int srcVersion );
	virtual void createVersion( const STRING &filePath, const STRING &description );
	void deleteVersion( int versionId, int storageId, bool updateMaxVersion );
	virtual STRING getSize();
	int getVersionNum()
	{
		THE_FILE_VERSION *latestVersion = getLatestVersion();

		return latestVersion ? latestVersion->getVersionNum() : 0;
	}
	int getStorageID()
	{
		return getLatestVersion()->getStorageID();
	}
	TDateTime getFileCreationDate()
	{
		return getLatestVersion()->getFileCreationDate();
	}

	TDateTime getVersionModifiedDate()
	{
		return getLatestVersion()->getVersionModifiedDate();
	}
	const STRING &getMD5checksum()
	{
		return m_md5hash;
		//return getLatestVersion()->getMD5checksum();
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
	static const STRING &getExternalStorageBase()
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
	void branch();

	virtual STRING download( int version, bool protect, const STRING &dest );
	virtual STRING getVersionFileName();

	virtual bool canDelete( bool forPurge, bool recursive  );
	virtual void purgeItem();
	virtual void purgeVersions( int numVersions );

	virtual bool canReserve() const;
	virtual bool canUnreserve( bool noAdminCheck ) const;
	virtual void reserve( int reserveFor=0, bool doNotOverwrite=false );
	virtual void unreserve( bool doBranch, const STRING &description, const STRING &newVersion="" );
	virtual void cancelReserve();
	virtual const char *compare( int firstVersion=0, int secondVersion=-1  );
	virtual bool hasReserved( const STRING &machine, int userId );
	virtual bool hasChanged( const STRING &dest, gak::DateTime *oLocalDate=NULL, gak::DateTime *oRemoteDate=NULL );
	STRING getContent()
	{
		STRING	src = getExternalFile();
		STRING	content;

		content.readFromFile( src );

#pragma warn -inl
		return content;
#pragma warn +inl
	}

	STRING getCSScontent()
	{
		doEnterFunctionEx(gakLogging::llDetail, "THE_FILE::getCSScontent");

		STRING	mimeType = getMimeType();
		STRING	content = mimeType == "text/css" ? getContent() : (STRING)"";

#pragma warn -inl
		return content;
#pragma warn +inl
	}
	gak::xml::Document *getXmlDocument()
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
	gak::html::Document *getHtmlDocument()
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

	virtual void lock();
	virtual void unlock();

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
	virtual void open();
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
	const STRING &getMD5CheckSum() const
	{
		return m_md5CheckSum;
	}
	virtual void loadFields( TQuery *query );
	virtual void updateDatabase();

	virtual TGraphic *getItemPicture() const;
	virtual TGraphic *getStatusPicture() const;
	virtual STRING getSize();
	unsigned long getSizeInBytes()
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
	virtual TGraphic *getItemPicture() const;
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
