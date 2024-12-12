/*
		Project:
		Module:
		Description:
		Author:			Martin G�ckler
		Address:		Freibadstr. 14, D-81543 M�nchen
		Phone:			+49 - 89 - 65 30 95 63
		Telefax:		+49 - 89 - 65 30 95 64
		Copyright:		CRESD GmbH
		Date:			xx. xx. 200x
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

typedef enum
{
	COMPARE_OK, COMPARE_FOLDER, LOCAL_MISSING, DB_MISSING, LOCAL_NEWER, LOCAL_OLDER
} COMPARE_STATUS;

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
	virtual TGraphic *getItemPicture( void ) const;
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
	virtual TGraphic *getItemPicture( void ) const;
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
	virtual TGraphic *getItemPicture( void ) const;
	virtual void updateDatabase( void );
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
	virtual TGraphic *getItemPicture( void ) const;
};

class THE_BOOKMARK_FOLDER : public THE_PERSONAL_FOLDER
{
	public:
	THE_BOOKMARK_FOLDER(
		int id, const FACTORY_BASE *theFactory,
		int flags=ITEM_IS_CONTAINER
	)
	: THE_PERSONAL_FOLDER( id, theFactory, flags ) {}
	virtual TGraphic *getItemPicture( void ) const;
	virtual void getItemFactories( Array<const FACTORY_BASE*> *factory ) const;
};

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
	virtual TGraphic *getItemPicture( void ) const;
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
	virtual TGraphic *getItemPicture( void ) const;
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
	virtual void updateDatabase( void );
	virtual void getItemFactories( Array<const FACTORY_BASE*> *factory ) const;
	virtual TGraphic *getStatusPicture( void ) const;
};
typedef PTR_TEMPLATE<THE_FOLDER_REF> PTR_FOLDER_REF;


class THE_LOCAL_FOLDER : public THE_FOLDER_REF
{
	int		id;
	STRING	localPath;

	protected:
	static int itemCompare( const PTR_ITEM &e1, const PTR_ITEM &e2, int crit );

	public:
	THE_LOCAL_FOLDER(
		int id, const FACTORY_BASE *theFactory,
		int flags=ITEM_IS_CONTAINER|ITEM_IS_BROWSEABLE|ITEM_IS_MOVEABLE|ITEM_IS_COPYABLE
	) : THE_FOLDER_REF( id, theFactory, flags )
	{
		this->id = 0;
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
	virtual void updateDatabase( void );

	STRING getLocalPath( void ) const
	{
		return localPath;
	}
	void setLocalPath( const STRING &newPath )
	{
		localPath = newPath;
		size_t	len = localPath.strlen();
		if( len && localPath[len-1] != DIRECTORY_DELIMITER )
			localPath += DIRECTORY_DELIMITER;
	}
	virtual STRING getDownloadPath( PTR_ITEM parent=PTR_ITEM() );
	virtual void purgeItem( void );
	virtual TGraphic *getStatusPicture( void ) const;
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
	virtual TGraphic *getItemPicture( void ) const;
	virtual void getItemFactories( Array<const FACTORY_BASE*> *factory ) const;
};
typedef PTR_TEMPLATE<THE_SYNC_FOLDER> PTR_SYNC_FOLDER;

class THE_SOURCE_FOLDER : public THE_LOCAL_FOLDER
{
	virtual COL_TITLE	*getColumnTitles( void ) const;
	virtual int getColCount( void ) const;
	virtual int *getColWidth( void ) const;
	virtual void sort( void );
	virtual STRING drawCell( int col, int row, TCanvas *canvas, TRect &Rect );

	public:
	THE_SOURCE_FOLDER( int id, const FACTORY_BASE *theFactory )
	: THE_LOCAL_FOLDER( id, theFactory )
	{
	}
	virtual TGraphic *getItemPicture( void ) const;
	virtual bool refresh( bool recursive=true, std::ostream *stream=NULL );
	void import( void );
	void checkIn( bool doBranch, const STRING &description );
	void reserve( int taskID );

	virtual void getItemFactories( Array<const FACTORY_BASE*> *factory ) const;
};
typedef PTR_TEMPLATE<THE_SOURCE_FOLDER> PTR_SOURCE_FOLDER;

class FolderCompareEntry
{
	public:
	typedef gak::F_STRING	key_type;

	key_type			name, filePath;
	TDateTime			dbModTime, localModTime;
	unsigned long		dbSize, localSize;
	PTR_LOCAL_FOLDER	theParent;
	PTR_FILE_BASE		theFile;
	bool				inFS, reserved, folder;
	STRING				statusSTR;
	COMPARE_STATUS		status;

	FolderCompareEntry()
	{
		folder = reserved = inFS = false;
	}
	key_type getKey( void ) const
	{
		return name;
	}
	void setKey( const key_type &newName )
	{
		name = newName;
	}
};

class THE_RELEASE
{
	public:
	int major, minor, patch;
};

class THE_RELEASE_FOLDER : public THE_SOURCE_FOLDER
{
	THE_RELEASE	release;

	public:
	THE_RELEASE_FOLDER( int id, const FACTORY_BASE *theFactory ) : THE_SOURCE_FOLDER( id, theFactory )
	{
		release.major = release.minor = release.patch = 0;
	}
	void setData( const PTR_ITEM &parent, const STRING &name, const STRING &description, int major, int minor, int patch )
	{
		THE_FOLDER::setData( parent, name, description );
		setRelease( major, minor, patch );
	}
	void setRelease( int major, int minor, int patch )
	{
		release.major = major;
		release.minor = minor;
		release.patch = patch;
	}
	int getMajor( void ) const
	{
		return release.major;
	}
	int getMinor( void ) const
	{
		return release.minor;
	}
	int getPatch( void ) const
	{
		return release.patch;
	}
	const THE_RELEASE &getRelease( void ) const
	{
		return release;
	}
	virtual TGraphic *getItemPicture( void ) const;
	virtual void loadFields( TQuery *query );
	virtual void updateDatabase( void );
	virtual STRING getDownloadPath( PTR_ITEM parent=PTR_ITEM() );
};

typedef PTR_TEMPLATE<THE_RELEASE_FOLDER> PTR_RELEASE_FOLDER;

class FACTORY_FOLDER : public FACTORY_SIMPLE_BASE
{
	virtual int getItemType( void ) const;
	virtual bool acceptParent( const THE_ITEM *parent ) const;
	virtual PTR_ITEM createItem( int id ) const;
	virtual const char *getName( void ) const;
};

class FACTORY_PERSONAL_FOLDER : public FACTORY_ROOT_FOLDER
{
	virtual int getItemType( void ) const;
	virtual PTR_ITEM createItem( int id ) const;
	virtual const char *getName( void ) const;
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
