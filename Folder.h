/*
		Project:		DocMan
		Module:			Folder.h
		Description:	the container stored in this database
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

#ifndef THE_FOLDER_H
#define THE_FOLDER_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/map.h>
#include <gak/xml.h>

#include "File.h"
#include "EditXmlFileFrm.h"

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

enum CompareStatus
{
	csOK, csFOLDER,
	csLOCAL_MISSING, csDB_MISSING,
	csLOCAL_NEWER, csLOCAL_OLDER,
	csMD5_Changed
};

class FolderCompareEntry;
typedef gak::UnorderedMap<FolderCompareEntry>	FolderCompareList;

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

class THE_PUBLIC_TEMPLATE_FOLDER : public THE_ITEM
{
	public:
	THE_PUBLIC_TEMPLATE_FOLDER(
		int id, const FACTORY_BASE *theFactory,
		int flags=ITEM_IS_CONTAINER|ITEM_IS_BROWSEABLE
	)
	: THE_ITEM(flags, id, theFactory )
	{
		setName( theFactory->getName() );
	}
	virtual TGraphic *getItemPicture() const;
};

class THE_PERSONAL_TEMPLATE_FOLDER : public THE_ITEM
{
	public:
	THE_PERSONAL_TEMPLATE_FOLDER(
		int id, const FACTORY_BASE *theFactory,
		int flags=ITEM_IS_CONTAINER|ITEM_IS_BROWSEABLE
	)
	: THE_ITEM(flags, id, theFactory )
	{
		createPersonalName();
	}
	virtual TGraphic *getItemPicture() const;
};

class THE_FOLDER : public THE_ITEM
{
	public:
	THE_FOLDER(
		int id, const FACTORY_BASE *theFactory,
		int flags=ITEM_IS_CONTAINER|ITEM_IS_BROWSEABLE|ITEM_IS_MOVEABLE|ITEM_IS_COPYABLE
	)
	: THE_ITEM(flags, id, theFactory )
	{}
	virtual TGraphic *getItemPicture() const;
	virtual void updateDatabase();
	STRING generateWebFolder( bool forWebServer, STRING &mimeType );
};

typedef PTR_TEMPLATE<THE_FOLDER> PTR_FOLDER;

class THE_PERSONAL_FOLDER : public THE_FOLDER
{
	public:
	THE_PERSONAL_FOLDER(
		int id, const FACTORY_BASE *theFactory,
		int flags=ITEM_IS_CONTAINER|ITEM_IS_BROWSEABLE
	)
	: THE_FOLDER( id, theFactory, flags )
	{
		createPersonalName();
	}
	virtual TGraphic *getItemPicture() const;
};

class THE_BOOKMARK_FOLDER : public THE_PERSONAL_FOLDER
{
	public:
	THE_BOOKMARK_FOLDER(
		int id, const FACTORY_BASE *theFactory,
		int flags=ITEM_IS_CONTAINER
	)
	: THE_PERSONAL_FOLDER( id, theFactory, flags ) {}
	virtual TGraphic *getItemPicture() const;
	virtual void getItemFactories( Array<const FACTORY_BASE*> *factory ) const;
};

typedef PTR_TEMPLATE<THE_BOOKMARK_FOLDER> PTR_BOOKMARK_FOLDER;

class THE_COMPANY_FOLDER : public THE_FOLDER
{
	public:
	THE_COMPANY_FOLDER(
		int id, const FACTORY_BASE *theFactory,
		int flags=ITEM_IS_CONTAINER|ITEM_IS_BROWSEABLE
	)
	: THE_FOLDER( id, theFactory, flags )
	{
		setName( theFactory->getName() );
	}
	virtual TGraphic *getItemPicture() const;
};

class THE_XSLT_FOLDER : public THE_COMPANY_FOLDER
{
	public:
	THE_XSLT_FOLDER(
		int id, const FACTORY_BASE *theFactory,
		int flags=ITEM_IS_CONTAINER|ITEM_IS_BROWSEABLE
	)
	: THE_COMPANY_FOLDER( id, theFactory, flags ) {}
	virtual void getItemFactories( Array<const FACTORY_BASE*> *factory ) const;
	virtual TGraphic *getItemPicture() const;
};

class THE_FOLDER_REF : public THE_FOLDER
{
	public:
	THE_FOLDER_REF(
		int id, const FACTORY_BASE *theFactory,
		int flags=ITEM_IS_CONTAINER
	) : THE_FOLDER( id, theFactory, flags )
	{
	}
	virtual bool refresh( bool recursive=true, std::ostream *stream=NULL );
	void setData(
		const PTR_ITEM &parent,
		const STRING &name, const STRING &description,
		const gak::DateTime &createdDate,
		const gak::DateTime &modifiedDate
	)
	{
		THE_FOLDER::setData(
			parent,
			name, description
		);
		setCreatedDate( createdDate );
		setModifiedDate( createdDate );
	}
	virtual void updateDatabase();
	virtual void getItemFactories( Array<const FACTORY_BASE*> *factory ) const;
	virtual TGraphic *getStatusPicture() const;
};
typedef PTR_TEMPLATE<THE_FOLDER_REF> PTR_FOLDER_REF;


class THE_LOCAL_FOLDER : public THE_FOLDER_REF
{
	int		m_id;
	STRING	m_localPath;

	protected:
	static int itemCompare( const PTR_ITEM &e1, const PTR_ITEM &e2, int crit );

	public:
	THE_LOCAL_FOLDER(
		int id, const FACTORY_BASE *theFactory,
		int flags=ITEM_IS_CONTAINER|ITEM_IS_BROWSEABLE|ITEM_IS_MOVEABLE|ITEM_IS_COPYABLE
	) : THE_FOLDER_REF( id, theFactory, flags )
	{
		m_id = 0;
	}
	void setData(
		const PTR_ITEM &parent, const STRING &name, const STRING &description,
		const STRING &newLocalPath
	)
	{
		THE_FOLDER::setData( parent, name, description );
		setLocalPath( newLocalPath );
	}
	virtual void loadFields( TQuery *query );
	virtual void updateDatabase();

	STRING getLocalPath() const
	{
		return m_localPath;
	}
	void setLocalPath( const STRING &newPath )
	{
		m_localPath = newPath;
		size_t	len = m_localPath.strlen();
		if( len && m_localPath[len-1] != DIRECTORY_DELIMITER )
		{
			m_localPath += DIRECTORY_DELIMITER;
		}
	}
	virtual STRING getDownloadPath( PTR_ITEM parent=PTR_ITEM() );
	virtual void purgeItem();
	virtual TGraphic *getStatusPicture() const;
	void compare( FolderCompareList *theList );
};
typedef PTR_TEMPLATE<THE_LOCAL_FOLDER> PTR_LOCAL_FOLDER;

class THE_SYNC_FOLDER : public THE_LOCAL_FOLDER
{
	public:
	THE_SYNC_FOLDER(
		int id, const FACTORY_BASE *theFactory,
		int flags=ITEM_IS_CONTAINER|ITEM_IS_MOVEABLE|ITEM_IS_COPYABLE
	) : THE_LOCAL_FOLDER( id, theFactory, flags )
	{
	}
	virtual TGraphic *getItemPicture() const;
	virtual void getItemFactories( Array<const FACTORY_BASE*> *factory ) const;
};
typedef PTR_TEMPLATE<THE_SYNC_FOLDER> PTR_SYNC_FOLDER;

class THE_SOURCE_FOLDER : public THE_LOCAL_FOLDER
{
	virtual ColumnTitle	*getColumnTitles() const;
	virtual int getColCount() const;
	virtual int *getColWidth() const;
	virtual void sort();
	virtual STRING drawCell( int col, int row, TCanvas *canvas, TRect &Rect );

	public:
	THE_SOURCE_FOLDER( int id, const FACTORY_BASE *theFactory )
	: THE_LOCAL_FOLDER( id, theFactory )
	{
	}
	virtual TGraphic *getItemPicture() const;
	virtual bool refresh( bool recursive=true, std::ostream *stream=NULL );
	void import();
	void checkIn( bool doBranch, const STRING &description );
	void reserve( int taskID, bool changedOnly );

	virtual void getItemFactories( Array<const FACTORY_BASE*> *factory ) const;
};
typedef PTR_TEMPLATE<THE_SOURCE_FOLDER> PTR_SOURCE_FOLDER;

struct FolderCompareEntry
{
	typedef gak::F_STRING	key_type;

	key_type			name, filePath;
	TDateTime			dbModTime, localModTime;
	unsigned long		dbSize, localSize;
	STRING				dbMD5, localMD5;
	PTR_LOCAL_FOLDER	theParent;
	PTR_FILE			theFile;
	bool				inFS, reserved, folder;
	STRING				statusSTR;
	CompareStatus		status;

	FolderCompareEntry()
	{
		folder = reserved = inFS = false;
	}
	key_type getKey() const
	{
		return name;
	}
	void setKey( const key_type &newName )
	{
		name = newName;
	}
};

struct THE_RELEASE
{
	int major, minor, patch;
};

class THE_RELEASE_FOLDER : public THE_SOURCE_FOLDER
{
	THE_RELEASE	m_release;

	public:
	THE_RELEASE_FOLDER( int id, const FACTORY_BASE *theFactory ) : THE_SOURCE_FOLDER( id, theFactory )
	{
		m_release.major = m_release.minor = m_release.patch = 0;
	}
	void setData( const PTR_ITEM &parent, const STRING &name, const STRING &description, int major, int minor, int patch )
	{
		THE_FOLDER::setData( parent, name, description );
		setRelease( major, minor, patch );
	}
	void setRelease( int major, int minor, int patch )
	{
		m_release.major = major;
		m_release.minor = minor;
		m_release.patch = patch;
	}
	int getMajor() const
	{
		return m_release.major;
	}
	int getMinor() const
	{
		return m_release.minor;
	}
	int getPatch() const
	{
		return m_release.patch;
	}
	const THE_RELEASE &getRelease() const
	{
		return m_release;
	}
	virtual TGraphic *getItemPicture() const;
	virtual void loadFields( TQuery *query );
	virtual void updateDatabase();
	virtual STRING getDownloadPath( PTR_ITEM parent=PTR_ITEM() );
};

typedef PTR_TEMPLATE<THE_RELEASE_FOLDER> PTR_RELEASE_FOLDER;

class FACTORY_FOLDER : public FACTORY_SIMPLE_BASE
{
	virtual int getItemType() const;
	virtual bool acceptParent( const THE_ITEM *parent ) const;
	virtual PTR_ITEM createItem( int id ) const;
	virtual const char *getName() const;
};

class FACTORY_PERSONAL_FOLDER : public FACTORY_ROOT_FOLDER
{
	virtual int getItemType() const;
	virtual PTR_ITEM createItem( int id ) const;
	virtual const char *getName() const;
};

class FACTORY_LOCAL_FOLDER : public FACTORY_FOLDER
{
	virtual bool acceptParent( const THE_ITEM *parent ) const;
};

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

inline int releaseCompare( const THE_RELEASE &r1, const THE_RELEASE &r2 )
{
	if( r1.major < r2.major )
		return -1;
	else if( r1.major > r2.major )
		return 1;
	else if( r1.minor < r2.minor )
		return -1;
	else if( r1.minor > r2.minor )
		return 1;
	else if( r1.patch < r2.patch )
		return -1;
	else if( r1.patch > r2.patch )
		return 1;

	return 0;
}

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif


#endif

