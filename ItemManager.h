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

#ifndef ITEM_MANAGER_H
#define ITEM_MANAGER_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <DBTables.hpp>
#include <Forms.hpp>

#include <gak/DateTime.h>
#include <gak/array.h>
#include <gak/thread.h>
#include <gak/shared.h>
#include <gak/t_string.h>
#include <gak/Map.h>
#include <gak/types.h>

#include "gaklib.h"

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT+
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

// Item Flags
const int ITEM_IS_CONTAINER		= 0x01;
const int ITEM_IS_BROWSEABLE	= 0x02;
const int ITEM_IS_MOVEABLE		= 0x04;
const int ITEM_IS_COPYABLE		= 0x08;
const int ITEM_IS_VERSIONED		= 0x10;
const int ITEM_IS_COMPLETABLE	= 0x20;

const int SORT_TYPE			= 1;
const int SORT_ORDER		= 2;
const int SORT_NAME			= 3;
const int SORT_SIZE			= 4;
const int SORT_DATE			= 5;
const int SORT_STATUS		= 6;
const int SORT_TASK_TYPE	= 7;
const int SORT_PRIORITY		= 8;
const int SORT_MODULE		= 9;
const int SORT_ASSIGNED		= 10;
const int SORT_NUM_LINKS	= 11;
const int SORT_EXTENSION	= 12;

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

class THE_ITEM;

typedef gak::SharedObjectPointer<THE_ITEM> PTR_ITEM;

template <typename ItemClass>
class PTR_TEMPLATE : public gak::SharedObjectPointer<ItemClass>
{
	public:
	PTR_TEMPLATE( const ItemClass *fp=NULL ) : gak::SharedObjectPointer<ItemClass>( fp )
	{
	}
	PTR_TEMPLATE( const PTR_ITEM &src ) : gak::SharedObjectPointer<ItemClass>(
		dynamic_cast<ItemClass*>(
			static_cast<THE_ITEM*>( src )
		)
	)
	{
	}

	PTR_TEMPLATE &operator = ( const ItemClass *src )
	{
		changePointer( const_cast<ItemClass *>(src) );

		return *this;
	}

	PTR_TEMPLATE &operator = ( const PTR_ITEM &src )
	{
		changePointer(
			dynamic_cast<ItemClass*>(
				static_cast<THE_ITEM*>( src )
			)
		);

		return *this;
	}

	operator PTR_ITEM () const
	{
		return PTR_ITEM(gak::SharedObjectPointer<ItemClass>::get());
	}
};

typedef Array<PTR_ITEM>	ITEM_CONTENT;
enum itemTypes
{
	TYPE_ROOT = -1, TYPE_FOLDER, TYPE_DOCUMENT,
	TYPE_SYNC_FOLDER, TYPE_SOURCE_FOLDER, TYPE_RELEASE_FOLDER,
	TYPE_PERSONAL_FOLDER, TYPE_PERSONAL_TRASH,
	TYPE_TASKLIST, TYPE_TASK,
	TYPE_COMPANY_FOLDER,
	TYPE_FILE_REF, TYPE_FOLDER_REF,
	TYPE_ALIAS, TYPE_BOOKMARK_FOLDER,
	TYPE_WEB_FOLDER, TYPE_WEB_MENU_FOLDER,
	TYPE_LANGUAGE_DOC, TYPE_LANGUAGE_ITEM, TYPE_XSLT_FOLDER,
	TYPE_PERSONAL_TEMPLATE_FOLDER, TYPE_PUBLIC_TEMPLATE_FOLDER,
	TYPE_ARCHIVE_FOLDER,
	TYPE_TEST_FOLDER, TYPE_TEST_STEP, TYPE_TEST_EXEC_FOLDER, TYPE_TEST_EXEC_STEP,
	TYPE_REMOTE_FOLDER, TYPE_REMOTE_FILE, TYPE_CRYPTO_FILE, TYPE_REMOTE_CRYPTO_FILE
};

struct ITEM_PERM
{
	int user_id, permissions;
};

typedef Array<ITEM_PERM>	ITEM_PERMLIST;
enum PERM_APPLY_MODE
{
	PERM_APPLY_OVERWRITE, PERM_APPLY_MERGE, PERM_APPLY_DELETE
};

struct COL_TITLE
{
	const char	*title;
	int			sortType;
};

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

class FACTORY_BASE;
class TBrowserFrame;

class THE_ITEM : public gak::SharedObject
{
	friend gak::SharedObjectPointer<THE_ITEM>;

	int					flags;

	const FACTORY_BASE	*theFactory;
	int					id, parentID, oldParentID, volumeID, oldVolumeID,
						permissionID, copyID, createdBy, ordering, numLinks,
						assignedTo;
	STRING				name, oldName, previousName, extension, description;
	TDateTime			m_createdDate, m_modifiedDate;
	gak::uint32			m_reminderDate;

	bool				ancestorsLoaded, contentLoaded, aclLoaded, aclChanged;
	ITEM_CONTENT		content;
	gak::ArrayOfInts	ancestors;
	ITEM_PERMLIST		acl;

	/*
		do not add pointer to parents becaus they will create
		circle links that will cause objects never be freed
	PTR_ITEM			parent, volume;
	*/

	bool				pathLoaded;
	STRING				downloadPath, previousDownloadPath;

	int					userPermissions;
	int					childCount;

	protected:
	int					sortType;

	static int itemCompare( const PTR_ITEM &e1, const PTR_ITEM &e2, int crit );

	virtual THE_ITEM::~THE_ITEM();

	THE_ITEM( int flags, int id, const FACTORY_BASE *theFactory )
	: sortType(SORT_NAME)
	{
		this->flags = flags;
		this->id = id;
		this->theFactory = theFactory;
		oldParentID = 0;
		userPermissions = 0;
		childCount = -1;
		pathLoaded = ancestorsLoaded = contentLoaded = aclLoaded = aclChanged =
			false
		;
		copyID = 0;
		ordering = 0;
		numLinks = 0;
		parentID = -1;
		oldVolumeID = volumeID = permissionID = 0;
		assignedTo = 0;
		m_createdDate = m_modifiedDate = TDateTime::CurrentDateTime();
		m_reminderDate = 0;

	}

	public:
/*
--------------------------------------------------------------------------------
	checklng flags
--------------------------------------------------------------------------------
*/
	bool isContainer( void ) const
	{
		return flags & ITEM_IS_CONTAINER;
	}
	bool isBrowseable( void ) const
	{
		return flags & ITEM_IS_BROWSEABLE;
	}
	bool isMoveable( void ) const
	{
		return flags & ITEM_IS_MOVEABLE;
	}
	bool isCopyable( void ) const
	{
		return flags & ITEM_IS_COPYABLE;
	}
	bool isVersioned( void ) const
	{
		return flags & ITEM_IS_VERSIONED;
	}
	bool isCompletable( void ) const
	{
		return flags & ITEM_IS_COMPLETABLE;
	}

/*
--------------------------------------------------------------------------------
	getting/changing data
--------------------------------------------------------------------------------
*/
	protected:
	void createPersonalName( void );

	public:
	void setName( const STRING &name )
	{
        pathLoaded = false;
		this->name = name;
		size_t	dotPos = name.searchRChar( '.' );
		if( dotPos != (size_t)-1 )
			extension = name.subString( dotPos );
		else
			extension = "";
	}
	void setData( const PTR_ITEM &parent, const STRING &name, const STRING &description )
	{
		setName( name );
		this->description = description;
		setParent( parent );
	}
	void setOrder( int order )
	{
		this->ordering = order;
	}

	int getID( void ) const
	{
		return id;
	}
	const STRING &getName( void ) const
	{
		if( !oldName.isEmpty() )
			return oldName;
		else
			return name;
	}
	const STRING &getPreviousName( void ) const
	{
		return previousName;
	}
	const STRING &getPreviousDownloadPath( void ) const
	{
		return previousDownloadPath;
	}
	const STRING &getExtension( void ) const
	{
		return extension;
	}
	const STRING &getDescription( void ) const
	{
		return description;
	}
	void setDescription( const STRING &newDescription )
	{
		description = newDescription;
	}
	int getCreator( void ) const
	{
		return createdBy;
	}
	TDateTime getCreatedDate( void ) const
	{
		return m_createdDate;
	}
	void setCreatedDate( TDateTime createdDate )
	{
		m_createdDate = createdDate;
	}
	void setCreatedDate( time_t unixTime )
	{
		m_createdDate = gak::DateTime( unixTime ).calcOriginalTime();
	}
	TDateTime getModifiedDate( void ) const
	{
		return m_modifiedDate;
	}
	void setModifiedDate( TDateTime modifiedDate )
	{
		m_modifiedDate = modifiedDate;
	}
	void setModifiedDate( time_t unixTime )
	{
		m_modifiedDate = gak::DateTime( unixTime ).calcOriginalTime();
	}
	gak::uint32 getReminderDate( void ) const
	{
		return m_reminderDate;
	}
	void setReminderDate( gak::uint32 reminderDate )
	{
		m_reminderDate = reminderDate;
	}
	virtual STRING getSize( void );
	int getOrder( void ) const
	{
		return ordering;
	}
	int getNumLinks( void ) const
	{
		return numLinks;
	}
/*
--------------------------------------------------------------------------------
	reading content
--------------------------------------------------------------------------------
*/
	private:
	int loadChildCount( void );

	protected:
	void clearContent( void );

	public:
	virtual ITEM_CONTENT *loadContent( void );
	ITEM_CONTENT *getContent( void )
	{
		if( !contentLoaded )
			loadContent();
		return &content;
	}
	ITEM_CONTENT *getContent( int sortType )
	{
		sort( sortType, true );
		if( !contentLoaded )
		{
			loadContent();
		}
		return &content;
	}
	PTR_ITEM getContentItem( int index ) const
	{
		if( index >= 0 && index < int(content.size()) )
		{
			return content[index];
		}
		else
		{
			return PTR_ITEM();
		}
	}
	int getContentIndex( int ID );
	PTR_ITEM getContentItem( const char *name );
	int getChildCount( void )
	{
		if( childCount < 0 )
		{
			loadChildCount();
		}
		return childCount;
	}
	void incChildCount( void )
	{
		if( childCount >= 0 )
			childCount++;
	}
	void decChildCount( void )
	{
		childCount--;
	}

/*
--------------------------------------------------------------------------------
	permissions
--------------------------------------------------------------------------------
*/
	private:
	void loadACL( void );

	public:
	const ITEM_PERMLIST &getACL( void )
	{
		if( !aclLoaded )
			loadACL();

		return acl;
	}
	void setACL( const ITEM_PERMLIST &acl )
	{
		aclChanged = true;
		this->acl = acl;
	}
	void setACL( const ITEM_PERMLIST &acl, enum PERM_APPLY_MODE applyMode );
	void setPermissionID( int permissionID, int newPerms )
	{
		this->permissionID = permissionID;
		userPermissions = newPerms;
	}
	int getPermissionID( void ) const
	{
		return permissionID;
	}
	virtual int loadPermissions( void );
	int getUserPermissions( void ) const
	{
		return userPermissions;
	}
	void setUserPermissions( int newPerms )
	{
		userPermissions &= newPerms;
	}


/*
--------------------------------------------------------------------------------
	path
--------------------------------------------------------------------------------
*/
	private:
	void clearAncestors( void );

	gak::ArrayOfInts *loadAncestors( void );

	bool acceptChildType( const Array<const FACTORY_BASE*> &factories, itemTypes theType ) const;
	void acceptParent( THE_ITEM *newParent, bool forDelete );
	void setParent( const PTR_ITEM &newParent, bool forDelete=false );
	void setVolumeID( int volumeID )
	{
		this->volumeID = volumeID;
	}

	public:
	gak::ArrayOfInts *getAncestors( void )
	{
		if( !ancestorsLoaded )
			loadAncestors();

		return &ancestors;
	}
	STRING getPath( void ) const;

	PTR_ITEM getAncestorItem( int index );

	int getParentID( void ) const
	{
		return parentID;
	}
	PTR_ITEM getParent( void ) const;

	int getVolumeID( void ) const
	{
		return volumeID;
	}
	PTR_ITEM getVolume( void ) const;

	PTR_ITEM getOldParent( void ) const;
	void acceptParent( THE_ITEM *newParent )
	{
		return acceptParent( newParent, false );
	}

	bool acceptChildType( itemTypes theType ) const;
	bool acceptChildTypes( itemTypes theType, ... ) const;
/*
--------------------------------------------------------------------------------
	DB access
--------------------------------------------------------------------------------
*/
	virtual void updateDatabase( void );
	virtual void loadFields( TQuery *query );

/*
--------------------------------------------------------------------------------
	factory
--------------------------------------------------------------------------------
*/
	int getItemType( void ) const;
	const char *getItemTypeString( void ) const;
	const FACTORY_BASE *getFactory( void ) const
	{
		return theFactory;
	}
	virtual void getItemFactories( Array<const FACTORY_BASE*> *factory ) const;
	virtual bool acceptDropFiles( void ) const;
	virtual void dropFile( const STRING &path );
	virtual void dropFolder( const STRING &path );

/*
--------------------------------------------------------------------------------
	displaying content
--------------------------------------------------------------------------------
*/
	private:
	virtual COL_TITLE *getColumnTitles( void ) const;

	public:
	virtual TBrowserFrame *getFrame( void ) const;
	virtual int getColCount( void ) const;
	virtual int *getColWidth( void ) const;
	virtual int getHeaderCount( void ) const;
	virtual int getRowCount( void ) const;
	int getSortType( int col ) const
	{
		COL_TITLE	*colTitles = getColumnTitles();

		return colTitles[col].sortType;
	}
	int getSortType( void ) const
	{
		return sortType;
	}
	void setSortType( int sortType )
	{
		this->sortType = sortType;
	}
	int sort( int sortType, bool noReverse=true )
	{
		int oldSort = this->sortType;

		if( !noReverse || oldSort != sortType )
		{
			if( oldSort == sortType )
				sortType = -sortType;

			this->sortType = sortType;

			sort();
		}

		return oldSort;
	}
	virtual void sort( void );
	virtual STRING drawHeaderCell(
		int col, int row,
		TCanvas *canvas,
		TRect &Rect,
		TImageList *ImageListSort
	);
	virtual STRING drawCell( int col, int row, TCanvas *canvas, TRect &Rect );
	virtual TGraphic *getStatusPicture( void ) const;
	virtual TGraphic *getItemPicture( void ) const = 0;

/*
--------------------------------------------------------------------------------
	misc.
--------------------------------------------------------------------------------
*/
	typedef gak::TreeMap<int,int>	ItemsCopiedMap;

	virtual void open( void );
	virtual void openItem( int itemIdx );
	virtual STRING getDownloadPath( PTR_ITEM parent=PTR_ITEM() );

	void moveTo( const PTR_ITEM &newParent, const STRING &newName = "" );
	virtual PTR_ITEM copy(
		ItemsCopiedMap &itemsCopied,
		const PTR_ITEM &target, bool createLink, bool moveReserve,
		const STRING &newName=""
	);
	void setCopyID( int copyID )
	{
		this->copyID = copyID;
	}
	int getCopyID( void ) const
	{
		return copyID;
	}
	virtual bool hasReserved( const STRING &machine, int userId );

	virtual void lock( void );
	virtual void unlock( void );

	void setAssignedTo( int assignedTo )
	{
		this->assignedTo = assignedTo;
	}
	int getAssignedTo( void ) const
	{
		return assignedTo;
	}
	STRING getAssignedToUserName( void ) const;

	virtual void createXMLattributes( gak::xml::Element *target );
	gak::xml::Element *createXML( size_t maxLevel=-1, size_t curLevel=0 );
/*
--------------------------------------------------------------------------------
	trash & archive
--------------------------------------------------------------------------------
*/
	bool isDeleted( void );
	virtual bool canDelete( bool forPurge, bool recursive );
	virtual void deleteItem( void );
	virtual void purgeItem( void );
	virtual void purgeVersions( int numVersions );
	void restoreItem( const PTR_ITEM &parent, const STRING &name )
	{
		oldParentID = 0;
		oldName = "";
		setName( name );
		setParent( parent );
	}

	bool isArchived( void );
	virtual PTR_ITEM getArchive( bool create );
	virtual void archive( void );
};

class THE_ROOT : public THE_ITEM
{
	public:
	THE_ROOT( const FACTORY_BASE *theFactory )
	: THE_ITEM( ITEM_IS_CONTAINER, -1, theFactory )
	{
	}
	virtual TGraphic *getItemPicture( void ) const;
};

class TItemCreateForm;
class FACTORY_BASE
{
	public:
	FACTORY_BASE();

	virtual int getItemType( void ) const = 0;
	virtual bool acceptParent( const THE_ITEM *parent ) const = 0;
	virtual PTR_ITEM createItem( int id ) const = 0;
	virtual const char *getName( void ) const = 0;

	virtual TItemCreateForm *getForm( void ) const;
	virtual PTR_ITEM createItemFromForm( const PTR_ITEM &parent ) const;
	virtual PTR_ITEM createItemFromTemplate(
		const PTR_ITEM &parent,
		const THE_ITEM *src,
		const STRING &newName=""
	) const;
};

class FACTORY_SIMPLE_BASE : public FACTORY_BASE
{
	virtual TItemCreateForm *getForm( void ) const;
	virtual PTR_ITEM createItemFromForm( const PTR_ITEM &parent ) const;
};

class FACTORY_VIRTUAL_ROOT : public FACTORY_BASE
{
	virtual int getItemType( void ) const;
	virtual bool acceptParent( const THE_ITEM *parent ) const;
	virtual PTR_ITEM createItem( int id ) const;
	virtual const char *getName( void ) const;
};

class FACTORY_ROOT_FOLDER : public FACTORY_BASE
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

const FACTORY_BASE *getItemFactory( int itemType );
const char *getItemTypeString( int itemType );
PTR_ITEM createItem( int itemType, int id=0 );
PTR_ITEM getItem( int id );
PTR_ITEM getItemByName( int parentId, const char *name );
PTR_ITEM getItemByPath( gak::T_STRING path );
PTR_ITEM getPersonalItem( int itemType );
PTR_ITEM getPublicVolume( int itemType );
PTR_ITEM getCompanyVolume( void );
PTR_ITEM getArchiveVolume( void );

void clearItemCache( const PTR_ITEM &holdItem=PTR_ITEM() );

// --------------------------------------------------------------------- //
// ----- module functions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class inlines ------------------------------------------------- //
// --------------------------------------------------------------------- //

inline int THE_ITEM::getItemType( void ) const
{
	return theFactory->getItemType();
}

inline const char *THE_ITEM::getItemTypeString( void ) const
{
	return theFactory->getName();
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

inline PTR_ITEM THE_ITEM::getAncestorItem( int index )
{
	return getItem( ancestors[index] );
}
// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -a.
#	pragma option -p.
#endif

#endif
