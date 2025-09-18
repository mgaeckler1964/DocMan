/*
		Project:		DocMan
		Module:			ItemManager.cpp
		Description:	The base for all content types
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

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <stdlib.h>
#include <stdarg.h>

#include <graphics.hpp>

#include <gak/vcl_tools.h>

#include "ItemManager.h"
#include "UserDlg.h"
#ifndef DOCMANBG
#include "DocManMain.h"
#endif

#include "PermissionsFrm.h"
#include "StatusFrm.h"
#include "BrowseFram.h"
#include "DocManDM.h"
#include "Delete.h"
#include "Archive.h"
#include "ItemCreateFrm.h"
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

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

/*
	this is dynamicaly generated, because otherwise, the contructor
	of FACTORY_BASE may use this array, before the array itself is
	contructed
*/
typedef Array<FACTORY_BASE*>	ItemFactory;
static ItemFactory				*theItemFactory = NULL;
static FACTORY_VIRTUAL_ROOT		theVirtualRootFactory;

static ITEM_CONTENT				itemCache;

// --------------------------------------------------------------------- //
// ----- class static data --------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- prototypes ---------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module functions ---------------------------------------------- //
// --------------------------------------------------------------------- //

static std::size_t findItemInCache( int theItemID )
{
	size_t numCacheEntries = itemCache.size();
	for( size_t i=0; i<numCacheEntries; i++ )
	{
		if( (*(itemCache[i])).getID() == theItemID )
		{
/***/		return i;
		}
	}

	return -1;
}

static inline std::size_t findItemInCache( const PTR_ITEM &theItem )
{
	return findItemInCache( theItem->getID() );
}

static inline void addItemToCache( const PTR_ITEM &theItem )
{
	size_t	itemPos = findItemInCache( theItem );
	if( itemPos == size_t(-1) )
	{
		itemCache.addElement( theItem );
	}
	else
	{
		itemCache[itemPos] = theItem;
	}
}

// --------------------------------------------------------------------- //
// ----- class inlines ------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class constructors/destructors -------------------------------- //
// --------------------------------------------------------------------- //

FACTORY_BASE::FACTORY_BASE()
{
	if( !theItemFactory )
	{
		theItemFactory = new ItemFactory();
	}

	theItemFactory->addElement( this );
}

THE_ITEM::~THE_ITEM()
{
	clearContent();
	clearAncestors();
}

// --------------------------------------------------------------------- //
// ----- class static functions ---------------------------------------- //
// --------------------------------------------------------------------- //

int THE_ITEM::itemCompare( const PTR_ITEM &item1, const PTR_ITEM &item2, int theSort )
{
	int	compareResult = 0;

	if( abs( theSort ) == SORT_TYPE
	|| abs( theSort ) == SORT_SIZE )
	{
		if( item1->isContainer() && !item2->isContainer() )
			compareResult = -1;
		else if( !item1->isContainer() && item2->isContainer() )
			compareResult = 1;
		else if( item1->getItemType() != item2->getItemType() )
			compareResult = strcmpi( item1->getItemTypeString(), item2->getItemTypeString() );
		if( !compareResult && abs( theSort ) == SORT_SIZE )
			compareResult = atoi( item1->getSize() ) - atoi( item2->getSize() );
	}
	else if( abs( theSort ) == SORT_NUM_LINKS )
		compareResult = item1->getNumLinks()-item2->getNumLinks();
	else if( abs( theSort ) == SORT_ORDER )
		compareResult = item1->getOrder()-item2->getOrder();
	else if( abs( theSort ) == SORT_EXTENSION )
		compareResult = strcmpi( item1->getExtension(), item2->getExtension() );
	else if( abs( theSort ) == SORT_DATE )
	{
		TDateTime date1 = item1->getModifiedDate();
		TDateTime date2 = item2->getModifiedDate();

		if( date1 < date2 )
			compareResult = -1;
		else if( date1 > date2 )
			compareResult = 1;
		else
			compareResult = 0;
	}
	else if( abs( theSort ) == SORT_ASSIGNED )
	{
		compareResult = strcmpi(
			item1->getAssignedToUserName(),
			item2->getAssignedToUserName()
		);
	}

	if( !compareResult )
		compareResult = strcmpi( item1->getName(), item2->getName() );

	return theSort < 0 ? -compareResult : compareResult;
}

// --------------------------------------------------------------------- //
// ----- class privates ------------------------------------------------ //
// --------------------------------------------------------------------- //

void THE_ITEM::clearAncestors( void )
{
	m_ancestors.clear();
	m_ancestorsLoaded = false;
}

int THE_ITEM::loadChildCount( void )
{
	std::auto_ptr<TQuery>	theQuery( new TQuery( NULL ) );
	theQuery->DatabaseName = "docManDB";
	theQuery->SQL->Add( "select count(*) from ITEM_TREE where parentId = :theParent" );
	theQuery->Params->Items[0]->AsInteger = id;
	theQuery->Open();
	if( !theQuery->Eof )
		childCount = theQuery->Fields->Fields[0]->AsInteger;

	theQuery->Close();

	return childCount;
}

ArrayOfInts *THE_ITEM::loadAncestors( void )
{
	clearAncestors();

	PTR_ITEM	parent = getParent();
	while( parent )
	{
		m_ancestors.insertElement( (*parent).getID(), 0 );
		parent = (*parent).getParent();
	}

	m_ancestorsLoaded = true;

	return &m_ancestors;
}

void THE_ITEM::loadACL( void )
{
	ItemPermissions	itemPerm;

	std::auto_ptr<TQuery>	theQuery( new TQuery( NULL ) );
	theQuery->DatabaseName = "docManDB";
	theQuery->SQL->Add( "select * from I_ACLS where item_id = :theId" );
	theQuery->Params->Items[0]->AsInteger = id;
	m_acl.clear();

	for( theQuery->Open(); !theQuery->Eof; theQuery->Next() )
	{
		itemPerm.user_id = theQuery->FieldByName( "user_id" )->AsInteger;
		itemPerm.permissions = theQuery->FieldByName( "permissions" )->AsInteger;
		m_acl.addElement( itemPerm );
	}

	theQuery->Close();

	m_aclLoaded = true;
	m_aclChanged = false;
}

void THE_ITEM::acceptParent( THE_ITEM *newParent, bool forDelete )
{
	int perms = newParent ? newParent->userPermissions : 0;
	if( !perms & ITEM_PERM_ADD_ITEMS )
		throw Exception(
			"Permission on target denied"
		);
	if( !forDelete && !theFactory->acceptParent( newParent ) )
		throw Exception(
			"Target not acceptable"
		);
}

void THE_ITEM::setParent( const PTR_ITEM &newParent, bool forDelete )
{
	acceptParent( newParent, forDelete );

	if( id )
	{
		ArrayOfInts	*ancestors = newParent->getAncestors();
		for( size_t i=0; i<ancestors->size(); i++ )
		{
			PTR_ITEM ancestor = newParent->getAncestorItem( i );
			if( ancestor->id == id )
				throw Exception(
					"New target cannot be a children of the item to move"
				);
		}

		DocManDataModule->initPathMapping();
	}

	clearAncestors();

	if( newParent )
	{
		parentID = newParent->getID();
		volumeID = newParent->getVolumeID();
		newParent->incChildCount();
	}
	else
		volumeID = parentID = -1;
}


// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

void THE_ITEM::clearContent( void )
{
	m_content.clear();
	m_contentLoaded = false;
}

void THE_ITEM::createPersonalName( void )
{
	STRING name = getItemTypeString();
	name += " of ";
	const UserOrGroup	*actUser = DocManDataModule->getActUser();
	name += actUser->userName;

	setName( name );
}

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //

int FACTORY_VIRTUAL_ROOT::getItemType( void ) const
{
	return (int)TYPE_ROOT;
}

bool FACTORY_ROOT_FOLDER::acceptParent( const THE_ITEM *  ) const
{
	return false;
}

bool FACTORY_VIRTUAL_ROOT::acceptParent( const THE_ITEM * ) const
{
	return false;
}

PTR_ITEM FACTORY_VIRTUAL_ROOT::createItem( int ) const
{
	return new THE_ROOT( this );
}

const char *FACTORY_VIRTUAL_ROOT::getName( void ) const
{
	return "Virtual Root";
}

TItemCreateForm *FACTORY_BASE::getForm( void ) const
{
	return NULL;
}

TItemCreateForm *FACTORY_SIMPLE_BASE::getForm( void ) const
{
	return ItemCreateForm;
}

PTR_ITEM FACTORY_BASE::createItemFromForm( const PTR_ITEM & ) const
{
	return PTR_ITEM();
}

PTR_ITEM FACTORY_SIMPLE_BASE::createItemFromForm( const PTR_ITEM &parent ) const
{
	PTR_ITEM	newItem( createItem( 0 ) );

	newItem->setData(
		parent,
		ItemCreateForm->EditName->Text.c_str(),
		ItemCreateForm->MemoDescription->Text.c_str()
	);
	newItem->updateDatabase();

	return newItem;
}

PTR_ITEM FACTORY_BASE::createItemFromTemplate(
		const PTR_ITEM &parent,
		const THE_ITEM *src,
		const STRING &newName
	) const
{
	PTR_ITEM theItem = createItem( 0 );
	if( theItem )
	{
		theItem->setData(
			parent,
			newName.isEmpty() ? src->getName() : newName,
			src->getDescription()
		);
		theItem->setCopyID( src->getID() );
		theItem->updateDatabase();
	}

	return theItem;
}

void THE_ITEM::loadFields( TQuery *query )
{
	doEnterFunctionEx( gakLogging::llDetail, "THE_ITEM::loadFields" );

	oldVolumeID = volumeID = query->FieldByName( "VOLUME_ID" )->AsInteger;
	permissionID = query->FieldByName( "PERMISSION_ID" )->AsInteger;
	parentID = query->FieldByName( "PARENTID" )->AsInteger;
	ordering = query->FieldByName( "ORDERING" )->AsInteger;
	numLinks = query->FieldByName( "NUM_LINKS" )->AsInteger;
	oldParentID = query->FieldByName( "OLD_PARENT" )->AsInteger;
	copyID = query->FieldByName( "COPYID" )->AsInteger;
	STRING name = query->FieldByName( "NAME" )->AsString.c_str();
	setName( name );
	m_oldName = query->FieldByName( "OLD_NAME" )->AsString.c_str();
	createdBy = query->FieldByName( "CREATEDBY" )->AsInteger;
	m_createdDate = query->FieldByName( "createdDate" )->AsDateTime;
	m_modifiedDate = query->FieldByName( "modifiedDate" )->AsDateTime;
	m_reminderDate = query->FieldByName( "reminderDate" )->AsInteger;
	m_description = query->FieldByName( "description" )->AsString.c_str();
	this->assignedTo = query->FieldByName( "assigned_to" )->AsInteger;

	loadPermissions();

	m_previousDownloadPath = getDownloadPath();
	m_previousName = m_name;
}

int THE_ITEM::loadPermissions( void )
{
	doEnterFunctionEx( gakLogging::llDetail, "THE_ITEM::loadPermissions()" );

	const UserOrGroup *actUser = DocManDataModule->getActUser();
	if( actUser->permissions & vcl::USER_SYSTEM_PERM )
		userPermissions = -1;
	else if( !id )
		userPermissions = 0;
	else
	{
		std::auto_ptr<TQuery>	theQuery( new TQuery( NULL ) );
		theQuery->DatabaseName = "docManDB";
		theQuery->SQL->Add(
			"select permissions "
			"from i_acls "
			"where item_id = :theItem "
			"and (user_id = :theUser1 "
//				"or user_id in ("
//					"select group_id "
//					"from u_rights "
//					"where user_id = :theUser2"
//				")"
//			")"
		);

		if( !actUser->groupList.isEmpty() )
		{
			theQuery->SQL->Add( "or user_id in (" );
			theQuery->SQL->Add( (const char *)actUser->groupList );
			theQuery->SQL->Add( ")" );
		}

		theQuery->SQL->Add( ")" );

		theQuery->Params->Items[0]->AsInteger = permissionID ? permissionID : id;
		theQuery->Params->Items[1]->AsInteger = actUser->ID;
//		theQuery->Params->Items[2]->AsInteger = actUser->ID;

		userPermissions = 0;
		for( theQuery->Open(); !theQuery->Eof; theQuery->Next() )
			userPermissions |= theQuery->Fields->Fields[0]->AsInteger;
		theQuery->Close();

	}

	m_aclLoaded = false;

	return userPermissions;
}

void THE_ITEM::updateDatabase( void )
{
	doEnterFunctionEx(gakLogging::llInfo, "THE_ITEM::updateDatabase");

	const char	*step = "retrieving parent";
	STRING		errMsg;

	try
	{
		int			parentID = getParentID();
		bool		isNew;

		std::auto_ptr<TQuery>	theQuery( new TQuery( NULL ) );
		theQuery->DatabaseName = "docManDB";

		if( id )
		{
			m_modifiedDate = TDateTime::CurrentDateTime();
			step = "updating";
			theQuery->SQL->Add(
				"update ITEM_TREE "
				"set name = :name, parentId = :parentID, "
					"old_name = :oldName, old_parent = :oldParentId, "
					"modifiedDate = :mDate, "
					"reminderDate = :rDate, "
					"description=:description, "
					"ordering = :theOrder, "
					"volume_id = :volumeID, permission_id = :permissionID, "
					"assigned_to = :assignedTo "
				"where id=:theId"
			);
			unsigned i=0;
			theQuery->Params->Items[i++]->AsString = (const char *)m_name.c_str();
			theQuery->Params->Items[i++]->AsInteger = parentID;
			theQuery->Params->Items[i++]->AsString = (const char *)m_oldName;
			theQuery->Params->Items[i++]->AsInteger = oldParentID;
			theQuery->Params->Items[i++]->AsDateTime = m_modifiedDate;
			theQuery->Params->Items[i++]->AsInteger = m_reminderDate;
			theQuery->Params->Items[i++]->AsMemo = (const char *)m_description;
			theQuery->Params->Items[i++]->AsInteger = ordering;
			theQuery->Params->Items[i++]->AsInteger = volumeID;
			theQuery->Params->Items[i++]->AsInteger = permissionID;
			theQuery->Params->Items[i++]->AsInteger = assignedTo;
			theQuery->Params->Items[i++]->AsInteger = id;

			isNew = false;
		}
		else
		{
			PTR_ITEM	parent = getParent();

			step = "creating";
			createdBy = vcl::getActUserID();
			id = ConfigDataModule->getNewMaxValue( "ITEM_TREE", "ID" );
			if( parentID > 0 )
				oldVolumeID = volumeID = parent->getVolumeID();
			else
				oldVolumeID = volumeID = id;

			theQuery->SQL->Add(
				"insert into ITEM_TREE "
				"( "
					"parentId, name, item_type, createdBy, id, "
					"createdDate, modifiedDate, reminderDate, "
					"description, copyID, volume_id, "
					"permission_id, ordering, assigned_to "
				") "
				"values"
				"( "
					":newParentID, :newName, :theType, :createdBy, :theId, "
					":createdDate, :modifiedDate, :reminderDate, "
					":description, :copyID, :volumeID, "
					":permissionID, :theOrder, :assignedTo "
				")"
			);
			unsigned i=0;
			theQuery->Params->Items[i++]->AsInteger = parentID;
			theQuery->Params->Items[i++]->AsString = m_name.c_str();
			theQuery->Params->Items[i++]->AsInteger = getItemType();
			theQuery->Params->Items[i++]->AsInteger = createdBy;
			theQuery->Params->Items[i++]->AsInteger = id;
			theQuery->Params->Items[i++]->AsDateTime = m_createdDate;;
			theQuery->Params->Items[i++]->AsDateTime = m_modifiedDate;
			theQuery->Params->Items[i++]->AsInteger = m_reminderDate;
			theQuery->Params->Items[i++]->AsMemo = m_description.c_str();
			theQuery->Params->Items[i++]->AsInteger = copyID;
			theQuery->Params->Items[i++]->AsInteger = volumeID;
			theQuery->Params->Items[i++]->AsInteger = permissionID;
			theQuery->Params->Items[i++]->AsInteger = ordering;
			theQuery->Params->Items[i++]->AsInteger = assignedTo;

			if( !m_aclChanged && !permissionID )
			{
				if( parentID > 0 )
					setACL( parent->getACL() );

				m_aclChanged = true;
			}

			isNew = true;
		}

		theQuery->ExecSQL();

		if( permissionID && permissionID != id && m_acl.size() )
		{
			m_acl.clear();
			m_aclChanged = true;
		}
		if( m_aclChanged )
		{
			step = "removing ACLs";

			theQuery->SQL->Clear();
			theQuery->SQL->Add( "delete from i_acls where item_id = :item_id" );
			theQuery->Params->Items[0]->AsInteger = id;
			theQuery->ExecSQL();

			step = "inserting ACLs";
			theQuery->SQL->Clear();
			theQuery->SQL->Add(
				"insert into i_acls "
				"( item_id, user_id, permissions )"
				"values"
				"( :item_id, :user_id, :permissions )"
			);

			size_t	numAcl = m_acl.size();
			if( numAcl )
			{
				for(
					ITEM_PERMLIST::const_iterator it=m_acl.cbegin(),
						endIT=m_acl.cend();
					it != endIT;
					++it
				)
				{
					const ItemPermissions &itemPerm = *it;
					theQuery->Params->Items[0]->AsInteger = id;
					theQuery->Params->Items[1]->AsInteger = itemPerm.user_id;
					theQuery->Params->Items[2]->AsInteger = itemPerm.permissions;
					theQuery->ExecSQL();
				}
			}
			else
			{
				theQuery->Params->Items[0]->AsInteger = id;
				theQuery->Params->Items[1]->AsInteger = vcl::getActUserID();
				theQuery->Params->Items[2]->AsInteger = -1;
				theQuery->ExecSQL();
			}

			step = "loading ACLs";
			loadPermissions();
		}

		if( !isNew && oldVolumeID != volumeID )
		{
			step = "loading content";
			getContent();

			step = "updating child volume";
			for(
				ITEM_CONTENT::const_iterator it = m_content.cbegin(),
					endIT = m_content.cend();
				it != endIT;
				++it
			)
			{
				const PTR_ITEM &theChild = *it;
				if( theChild->getVolumeID() != volumeID )
				{
					theChild->setVolumeID( volumeID );
					theChild->updateDatabase();
				}
			}
			oldVolumeID = volumeID;
		}

		if( isNew )
		{
			step = "copy attributes";
			DocManDataModule->copyAttributeValues(
				getParentID(), 0, getID(), 0
			);
		}
	}
	catch( Exception &e )
	{
		errMsg = e.Message.c_str();
	}
	catch( ... )
	{
		errMsg = "Unkown";
	}

	if( !errMsg.isEmpty() )
	{
		errMsg = STRING("Error " ) + step + ' ' + m_name + '\n' + errMsg;
/*@*/	throw Exception( (const char*)errMsg );
	}
}

PTR_ITEM THE_ITEM::copy(
	ItemsCopiedMap &itemsCopied,
	const PTR_ITEM &newTarget, bool createLink, bool moveReserve, const STRING &newName
)
{
	if( StatusForm->setStatus( "Copying", getName() ) )
	{
		return PTR_ITEM();
	}

	PTR_ITEM newItem = theFactory->createItemFromTemplate( newTarget, this, newName );
	if( newItem )
	{
		loadContent();

		for(
			ITEM_CONTENT::const_iterator it = m_content.cbegin(),
				endIT = m_content.cend();
			it != endIT && !StatusForm->isTerminated();
			++it
		)
		{
			const PTR_ITEM &child = *it;
			child->copy( itemsCopied, newItem, createLink, moveReserve );
		}
	}

	return newItem;
}

STRING THE_ITEM::getSize( void )
{
	if( isContainer() )
	{
		int		childCount = getChildCount();
		STRING	size = formatNumber( childCount );

		size += " Items";

		return size;
	}
	else
		return "";
}

TBrowserFrame *THE_ITEM::getFrame( void ) const
{
#ifndef DOCMANBG
	static TBrowserFrame *theFrame = NULL;

	if( !theFrame )
		theFrame = new TBrowserFrame( NULL );

	return theFrame;
#else
	return NULL;
#endif
}

int THE_ITEM::getColCount( void ) const
{
	return 7;
}

int *THE_ITEM::getColWidth( void ) const
{
	static int colWidth[] =
	{
		50,
		50,
		0,
		50,
		120,
		80,
		50,
	};
	return colWidth;
}

int THE_ITEM::getHeaderCount( void ) const
{
	return 1;
}

int THE_ITEM::getRowCount( void ) const
{
	return m_content.size();
}

ColumnTitle *THE_ITEM::getColumnTitles( void ) const
{
	static ColumnTitle colTitles[] =
	{
		{ "Order", SORT_ORDER },
		{ "Type", SORT_TYPE },
		{ "Name", SORT_NAME },
		{ "Ext", SORT_EXTENSION },
		{ "Modified", SORT_DATE },
		{ "Size", SORT_SIZE },
		{ "Links", SORT_NUM_LINKS },
	};

	return colTitles;
}

void THE_ITEM::sort( void )
{
	m_content.sort( itemCompare, sortType );
}

STRING THE_ITEM::drawHeaderCell( int col, int, TCanvas *canvas, TRect &Rect, TImageList *ImageListSort )
{
	STRING text;

	ColumnTitle *colTitles = getColumnTitles();
	text = colTitles[col].title;

	if( abs(sortType) == colTitles[col].sortType )
	{
		ImageListSort->Draw(
			canvas,
			Rect.Left + canvas->TextWidth( (const char *)text ) +4,
			Rect.Top + 5,
			sortType < 0 ? 1 : 0,
			true
		);
	}

	return text;
}

TGraphic *THE_ITEM::getStatusPicture( void ) const
{
	return NULL;
}

STRING THE_ITEM::drawCell( int col, int row, TCanvas *canvas, TRect &Rect )
{
	STRING text;

	if( size_t(row) < m_content.size() )
	{
		PTR_ITEM	theItem = m_content[row];
		if( col == 0 )
		{
			int order = theItem->getOrder();
			text = formatNumber( order );
		}
		else if( col == 1 )
		{
			TGraphic	*thePic = theItem->getStatusPicture();
			if( thePic )
			{
				canvas->Draw( Rect.Left, Rect.Top+2, thePic );
			}
			thePic = theItem->getItemPicture();
			if( thePic )
			{
				canvas->Draw( Rect.Left+16, Rect.Top+2, thePic );
			}
		}
		else if( col == 2 )
		{
			text = theItem->getName();
		}
		else if( col == 3 )
		{
			text = theItem->getExtension();
		}
		else if( col == 4 )
		{
			TDateTime	modDate = theItem->getModifiedDate();
			AnsiString	modDateSTR = modDate.DateTimeString();
			text = modDateSTR.c_str();
		}
		else if( col == 5 )
		{
			text = theItem->getSize();
		}
		else if( col == 6 )
		{
			size_t numLinks = theItem->getNumLinks();
			text = formatNumber( numLinks );
		}
	}

	return text;
}

void THE_ITEM::open( void )
{
#ifndef DOCMANBG
	DocManMainForm->openItem( this );
#endif
}

void THE_ITEM::openItem( int itemIdx )
{
	PTR_ITEM	theItem = getContentItem( itemIdx );
	if( theItem )
	{
		if( theItem->getChildCount() == 1 )
		{
			ITEM_CONTENT *content = theItem->getContent();
			PTR_FOLDER	theFolder = (*content)[0];
			if( theFolder )
			{
				theItem->openItem(0);
/***/			return;
			}
		}

/***/	theItem->open();
	}
}

STRING THE_ITEM::getDownloadPath( PTR_ITEM parent )
{
	doEnterFunctionEx(gakLogging::llDetail, "THE_ITEM::getDownloadPath");
	if( !m_pathLoaded )
	{
		if( !parent )
		{
			parent = getParent();
		}
		if( parent )
		{
			m_downloadPath = parent->getDownloadPath();
		}

		if( !m_downloadPath.isEmpty() )
		{
			m_downloadPath += m_name;
			if( isContainer() )
				m_downloadPath += DIRECTORY_DELIMITER;
		}

		m_pathLoaded = true;
	}

	return m_downloadPath;
}

void THE_ITEM::getItemFactories( Array<const FACTORY_BASE*> *factory ) const
{
	doEnterFunctionEx(gakLogging::llDetail, "FACTORY_BASE::getItemFactories");
	doLogValueEx( gakLogging::llDetail, theItemFactory->size() );

	factory->clear();
	for(
		ItemFactory::const_iterator it = theItemFactory->cbegin(),
			endIT = theItemFactory->cend();
		it != endIT;
		++it
	)
	{
		FACTORY_BASE *theFactory = *it;
		if( theFactory->acceptParent( this ) && theFactory->getForm() )
		{
			factory->addElement( theFactory );
		}
	}
}

bool THE_ITEM::acceptChildType( itemTypes theType ) const
{
	Array<const FACTORY_BASE*> factories;

	getItemFactories( &factories );

	return acceptChildType( factories, theType );
}

bool THE_ITEM::acceptChildTypes( itemTypes theType, ... ) const
{
	Array<const FACTORY_BASE*>	factories;
	bool						accepted = false;

	getItemFactories( &factories );

	if( factories.size() )
	{
		va_list	ap;
		va_start( ap, theType );
		while( theType >= 0 )
		{
			accepted = acceptChildType( factories, theType );
			if( accepted )
				break;

			theType = va_arg( ap, itemTypes );
		}
		va_end( ap );
	}

	return accepted;
}

bool THE_ITEM::acceptChildType(
	const Array<const FACTORY_BASE*> &factories, itemTypes theType
) const
{
	for(
		Array<const FACTORY_BASE*>::const_iterator it = factories.cbegin(),
			endIT = factories.cend();
		it != endIT;
		++it
	)
	{
		const FACTORY_BASE *theFactory = *it;
		if( theFactory->getItemType() == theType )
		{
/***/		return true;
		}
	}

	return false;
}

bool THE_ITEM::acceptDropFiles( void ) const
{
	doEnterFunctionEx(gakLogging::llDetail, "FACTORY_BASE::acceptDropFiles");
	bool	acceptFiles = false;

	doLogValueEx( gakLogging::llDetail, theItemFactory->size() );

	for(
		ItemFactory::const_iterator it = theItemFactory->cbegin(),
			endIT = theItemFactory->cend();
		it != endIT;
		++it
	)
	{
		FACTORY_BASE *theFactory = *it;
		if( theFactory->getItemType() == TYPE_DOCUMENT
		&&  theFactory->acceptParent( this ) )
		{
			acceptFiles = true;
			break;
		}
	}

	return acceptFiles;
}


void THE_ITEM::dropFile( const STRING &path )
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
			PTR_FILE	theFile = oldItem;
			if( theFile )
			{
				theFile->createVersion( path, "" );
				theFile->updateDatabase();
			}
		}
		else
		{
			PTR_FILE	theFile = createItem( TYPE_DOCUMENT );
			theFile->setData( this, fileName, "", path );
			theFile->updateDatabase();
		}
	}
}

void THE_ITEM::dropFolder( const STRING &path )
{
	STRING	newPath, fileName;

	size_t slashPos = path.searchRChar( DIRECTORY_DELIMITER );
	if( slashPos != (size_t)-1 )
	{
		fileName = path;
		fileName += slashPos+1;

		PTR_ITEM theItem = getContentItem( fileName );
		if( !theItem )
		{
			theItem = createItem( TYPE_FOLDER );
			theItem->setData( this, fileName, "" );
			theItem->updateDatabase();
		}

		DirectoryList	dirContent;
		dirContent.dirlist( path );
		for(
			DirectoryList::iterator it = dirContent.begin(), endIT = dirContent.end();
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

TGraphic *THE_ROOT::getItemPicture( void ) const
{
	static Graphics::TBitmap *thePic = NULL;

	if( !thePic )
	{
		thePic = new Graphics::TBitmap();
		thePic->LoadFromResourceName((unsigned int)HInstance, "ROOT_BITMAP");
	}
	return thePic;
}

ITEM_CONTENT *THE_ITEM::loadContent( void )
{
	doEnterFunctionEx( gakLogging::llInfo, "THE_ITEM::loadContent()" );

	if( isContainer() )
	{
		clearContent();

		std::auto_ptr<TQuery>	theQuery( new TQuery( NULL ) );
		theQuery->DatabaseName = "docManDB";
		theQuery->SQL->Add(
			// "select * from ITEM_TREE i where i.parentId = :theParent"
			"SELECT i.*, "
				"v.fileSize, v.fileModifiedDate, v.mime_type, f.usageCount as num_links "
			"FROM ITEM_TREE i "
			"LEFT OUTER JOIN I_FILES f "
			"ON i.FILE_ID = f.ID "
			"LEFT OUTER JOIN I_FILE_VERS v "
			"ON f.ID = v.file_id and f.version = v.version "
			"LEFT OUTER JOIN I_STORAGE s "
			"ON s.ID = v.storage_id "
			"where i.parentId = :theParent"
		);

		theQuery->Params->Items[0]->AsInteger = id;
		for( theQuery->Open(); !theQuery->Eof; theQuery->Next() )
		{
			PTR_ITEM	newItem = createItem(
				theQuery->FieldByName( "ITEM_TYPE" )->AsInteger,
				theQuery->FieldByName( "ID" )->AsInteger
			);
			if( newItem )
			{
				newItem->loadFields( theQuery.get() );
				if( newItem->getUserPermissions() )
				{
					m_content.addElement( newItem );
					addItemToCache( newItem );
				}
			}
		}
		theQuery->Close();

		m_contentLoaded = true;

		sort();
	}

	return &m_content;
}

bool THE_ITEM::hasReserved( const STRING &machine, int userId )
{
	if( !m_contentLoaded )
	{
		loadContent();
	}

	for(
		ITEM_CONTENT::iterator it = m_content.begin(), endIT = m_content.end();
		it != endIT;
		++it
	)
	{
		THE_ITEM	*child = *it;
		if( child->hasReserved( machine, userId ) )
		{
			return true;
		}
	}

	return false;
}

void THE_ITEM::lock( void )
{
	if( StatusForm->pushStatus( "Locking", getName() ) )
	{
		return;
	}

	loadContent();

	for(
		ITEM_CONTENT::iterator it = m_content.begin(),
			endIT = m_content.end();
		it != endIT && !StatusForm->isTerminated();
		++it
	)
	{
		PTR_ITEM	child = *it;
		child->lock();
	}

	StatusForm->restore();
}

void THE_ITEM::unlock( void )
{
	if( StatusForm->setStatus( "Unlocking", getName() ) )
	{
		return;
	}

	loadContent();
	for(
		ITEM_CONTENT::iterator it = m_content.begin(),
			endIT = m_content.end();
		it != endIT && !StatusForm->isTerminated();
		++it
	)
	{
		PTR_ITEM	child = *it;
		child->unlock();
	}
}

PTR_ITEM THE_ITEM::getArchive( bool create )
{
	PTR_ITEM	archiveParent, theArchive;
	int			archiveID = -1;

	PTR_ITEM archiveVolume = getArchiveVolume();

	TQuery *theQuery = new TQuery( NULL );
	theQuery->DatabaseName = "docManDB";
	theQuery->SQL->Add(
		"select ID "
		"from ITEM_TREE "
		"where VOLUME_ID=:theArchive "
		"and PERMISSION_ID=:mySelf"
	);
	theQuery->Params->Items[0]->AsInteger = (*archiveVolume).getID();
	theQuery->Params->Items[1]->AsInteger = getID();
	theQuery->Open();
	if( !theQuery->Eof )
		archiveID = theQuery->Fields->Fields[0]->AsInteger;
	theQuery->Close();

	if( archiveID <= 0 )
	{
		PTR_ITEM parent = getParent();
		if( parent )
			archiveParent = parent->getArchive( create );
		else
			archiveParent = archiveVolume;

		if( create )
		{
			theArchive = createItem( TYPE_FOLDER );
			theArchive->setData( archiveParent, getName(), "" );
			theArchive->setPermissionID( getID(), getUserPermissions() );
			theArchive->updateDatabase();
		}
		else
			theArchive = archiveParent;
	}
	else
		theArchive = getItem( archiveID );

	return theArchive;
}

void THE_ITEM::archive( void )
{
	PTR_ITEM	parent = getParent();
	if( parent )
	{
		PTR_ITEM	archive = parent->getArchive( true );
		oldParentID = getParentID();
		setParent( archive );
		updateDatabase();
	}
}

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

STRING THE_ITEM::getPath( void ) const
{
	doEnterFunctionEx(gakLogging::llDetail, "THE_ITEM::getPath");
	STRING	path;

	if( getParentID() > 0 )
		path = DocManDataModule->getPathForID( getParentID() );

	path += DIRECTORY_DELIMITER;
	path += getName();

	return path;
}

PTR_ITEM THE_ITEM::getParent( void ) const
{
	return (parentID > 0) ? getItem( parentID ) : PTR_ITEM();
}

PTR_ITEM THE_ITEM::getVolume( void ) const
{
	return (volumeID > 0) ? getItem( volumeID ) : PTR_ITEM();
}
PTR_ITEM THE_ITEM::getOldParent( void ) const
{
	if( id > 0 && oldParentID > 0 )
		return getItem( oldParentID );
	else
		return PTR_ITEM();
}

void THE_ITEM::moveTo( const PTR_ITEM &newParent, const STRING &newName )
{
	if( !(userPermissions & ITEM_PERM_MOVE) )
/*@*/	throw Exception(
			"Permission on source denied"
		);

	setParent( newParent );
	if( !newName.isEmpty() )
		setName( newName );
}

bool THE_ITEM::isDeleted( void )
{
	if( getID() == getVolumeID() )
		return false;

	PTR_PERSONAL_TRASH	theVolume = getVolume();

	return ! (!theVolume);
}

bool THE_ITEM::isArchived( void )
{
	PTR_ARCHIVE_FOLDER	theVolume = getVolume();

	return ! (!theVolume);
}

bool THE_ITEM::canDelete( bool forPurge, bool recursive )
{
	if( StatusForm->setStatus( "Checking", getPath() ) )
	{
		return false;
	}
	if( !forPurge && isDeleted() )
	{
		return false;
	}
	if( !(userPermissions & ITEM_PERM_DELETE) )
	{
		return false;
	}
	if( !m_contentLoaded )
	{
		loadContent();
	}
	if( m_content.size() < size_t(getChildCount()) )
	{
		return false;
	}
	if( recursive )
	{
		for(
			ITEM_CONTENT::iterator it = m_content.begin(),
				endIT = m_content.end();
			it != endIT && !StatusForm->isTerminated();
			++it
		)
		{
			THE_ITEM	*child = *it;
			if( !child->canDelete( forPurge, recursive ) )
			{
				return false;
			}
		}
	}
	return !StatusForm->isTerminated();
}

void THE_ITEM::purgeItem( void )
{
	STRING	path = getPath();

	if( StatusForm->setStatus( "Purging", path ) )
	{
		return;
	}

	if( !isDeleted() && isMoveable() )
	{
		STRING	error = "Cannot purge items not deleted\n";
		error += path;
/*@*/	throw Exception( (const char *)error  );
	}

	if( !canDelete( true, false ) )
	{
		STRING	error = "Cannot purge item\n";
		error += path;
/*@*/	throw Exception( (const char *)error  );
	}

	if( !m_contentLoaded )
	{
		loadContent();
	}

	for(
		ITEM_CONTENT::iterator it = m_content.begin(),
			endIT = m_content.end();
		it != endIT && !StatusForm->isTerminated();
		++it
	)
	{
		THE_ITEM	*child = *it;
		child->purgeItem();
	}

	if( !StatusForm->isTerminated() )
	{
		std::auto_ptr<TQuery>	theQuery( new TQuery( NULL ) );
		theQuery->DatabaseName = "docManDB";
		theQuery->SQL->Add( "delete from I_ACLS where item_id = :theID" );
		theQuery->Params->Items[0]->AsInteger = id;
		theQuery->ExecSQL();

		theQuery->SQL->Clear();
		theQuery->SQL->Add( "delete from I_EXIF_DATA where item_id = :theID" );
		theQuery->Params->Items[0]->AsInteger = id;
		theQuery->ExecSQL();

		theQuery->SQL->Clear();
		theQuery->SQL->Add( "delete from C_ATTR_VALUES where item_id = :theID" );
		theQuery->Params->Items[0]->AsInteger = id;
		theQuery->ExecSQL();

		theQuery->SQL->Clear();
		theQuery->SQL->Add( "delete from ITEM_TREE where id = :theID" );
		theQuery->Params->Items[0]->AsInteger = id;
		theQuery->ExecSQL();
	}
}

void THE_ITEM::purgeVersions( int numVersions )
{
	if( StatusForm->setStatus( "Purging Versions", getPath() ) )
	{
		return;
	}

	if( !m_contentLoaded )
	{
		loadContent();
	}

	for(
		ITEM_CONTENT::iterator it = m_content.begin(),
			endIT = m_content.end();
		it != endIT && !StatusForm->isTerminated();
		++it
	)
	{
		THE_ITEM	*child = *it;
		child->purgeVersions( numVersions );
	}
}

void THE_ITEM::deleteItem( void )
{
	if( !isMoveable() )
	{
		purgeItem();
/***/	return;
	}

	STRING	path = getPath();

	if( !canDelete( false, true ) && !StatusForm->isTerminated())
	{
		STRING	error = "Cannot delete item\n";
		error += path;
/*@*/	throw Exception( (const char *)error  );
	}

	if( StatusForm->setStatus( "Deleting", path ) )
/*@*/	return;

	if( !StatusForm->isTerminated() )
	{
		PTR_ITEM	theTrash = getPersonalItem( TYPE_PERSONAL_TRASH );
		if( theTrash )
		{
			if( !oldParentID )
				oldParentID = getParentID();
			m_oldName = m_name;
			STRING newName = formatNumber( id );
			setParent( theTrash, true );
			setName( newName );
			updateDatabase();
		}
	}
}

int THE_ITEM::getContentIndex( int ID )
{
	int index;
	getContent();
	for( index = int(m_content.size())-1; index >= 0; index-- )
	{
		if( ID == m_content[index]->getID() )
		{
			break;
		}
	}
	return index;
}

PTR_ITEM THE_ITEM::getContentItem( const char *name )
{
	getContent();

	for(
		ITEM_CONTENT::iterator it = m_content.begin(),
			endIT = m_content.end();
		it != endIT && !StatusForm->isTerminated();
		++it
	)
	{
		PTR_ITEM theChild = *it;
		if( theChild->getName() == name )
		{
/***/		return theChild;
		}
	}

	return PTR_ITEM();
}

void THE_ITEM::setACL(
	const ITEM_PERMLIST &newAcl, enum PERM_APPLY_MODE applyMode
)
{
	if( StatusForm->setStatus( "Perm Apply Down", getPath() ) )
	{
		return;
	}

	int perms = userPermissions;
	int	permissionID = getPermissionID();

	if( (perms & ITEM_PERM_EDIT_PERMS)
	&& (!permissionID || permissionID == getID()) )
	{
		if( applyMode == PERM_APPLY_OVERWRITE )
		{
			setACL( newAcl );
		}
		else
		{
			if( !m_aclLoaded )
			{
				loadACL();
			}
			for(
				ITEM_PERMLIST::const_iterator it = newAcl.cbegin(),
					endIT = newAcl.cend();
				it != endIT;
				++it
			)
			{
				bool					processed = false;
				const ItemPermissions	&permToProcess = *it;
				for( size_t j=0; j<m_acl.size(); j++ )
				{
					ItemPermissions &curPerm = m_acl[j];
					if( curPerm.user_id == permToProcess.user_id )
					{
						if( applyMode == PERM_APPLY_DELETE )
							m_acl.removeElementAt( j );
						else if( applyMode == PERM_APPLY_MERGE )
							curPerm.permissions |= permToProcess.permissions;
						processed = true;
						m_aclChanged = true;
/*v*/					break;
					}
				}
				if( applyMode == PERM_APPLY_MERGE && !processed )
				{
					m_acl.addElement( permToProcess );
					m_aclChanged = true;
				}
			}
		}

		if( m_aclChanged )
		{
			updateDatabase();
		}
	}

	if( !permissionID )
	{
		if( !m_contentLoaded )
		{
			loadContent();
		}

		for(
			ITEM_CONTENT::iterator it = m_content.begin(),
				endIT = m_content.end();
			it != endIT && !StatusForm->isTerminated();
			++it
		)
		{
			THE_ITEM	*child = *it;
			child->setACL( newAcl, applyMode );
		}
	}
}

void THE_ITEM::createXMLattributes( xml::Element *theElement )
{
	theElement->setIntegerAttribute( "ID", getID() );
	theElement->setStringAttribute( "name", getName() );
	theElement->setStringAttribute( "created", m_createdDate.DateTimeString().c_str() );
	theElement->setStringAttribute( "modified", m_modifiedDate.DateTimeString().c_str() );
	theElement->setIntegerAttribute( "reminder", m_reminderDate );

	theElement->addObject(
		new xml::Any( "description", m_description )
	);
}

xml::Element *THE_ITEM::createXML( size_t maxLevel, size_t curLevel )
{
	size_t	spacePos = 0;
	STRING	tag = getItemTypeString();

	// remove all blanks
	while( 1 )
	{
		spacePos = tag.searchChar( ' ', spacePos );
		if( spacePos != (size_t)-1 )
		{
			tag = tag.leftString( spacePos ) + tag.subString( spacePos+1 );
		}
		else
			break;
	}

	xml::Any	*theElement = new xml::Any( tag );
	createXMLattributes( theElement );

	if( maxLevel > curLevel )
	{
		getContent();
		int sortType = sort( SORT_ORDER );

		curLevel++;
		for(
			ITEM_CONTENT::iterator it = m_content.begin(),
				endIT = m_content.end();
			it != endIT;
			++it
		)
		{
			THE_ITEM *theChild = *it;

			theElement->addObject( theChild->createXML( maxLevel, curLevel ) );
		}

		sort( sortType );
	}

	return theElement;
}

STRING THE_ITEM::getAssignedToUserName( void ) const
{
	UserOrGroup	theUser;

	DocManDataModule->getUserById( assignedTo, &theUser );
	return theUser.userName;
}
// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

const FACTORY_BASE *getItemFactory( int itemType )
{
	for(
		ItemFactory::const_iterator it = theItemFactory->cbegin(),
			endIT = theItemFactory->cend();
		it != endIT;
		++it
	)
	{
		const FACTORY_BASE *theFactory = *it;
		if( theFactory->getItemType() == itemType )
		{
/*@*/		return theFactory;
		}
	}

	return NULL;
}

const char *getItemTypeString( int itemType )
{
	for(
		ItemFactory::const_iterator it = theItemFactory->cbegin(),
			endIT = theItemFactory->cend();
		it != endIT;
		++it
	)
	{
		FACTORY_BASE *theFactory = *it;
		if( theFactory->getItemType() == itemType )
		{
			return theFactory->getName();
		}
	}

	return NULL;
}

PTR_ITEM createItem( int itemType, int id )
{
	for(
		ItemFactory::const_iterator it = theItemFactory->cbegin(),
			endIT = theItemFactory->cend();
		it != endIT;
		++it
	)
	{
		FACTORY_BASE *theFactory = *it;
		if( theFactory->getItemType() == itemType )
		{
			return theFactory->createItem( id );
		}
	}

	return PTR_ITEM();
}

PTR_ITEM getItem( int id )
{
	doEnterFunctionEx(gakLogging::llDetail, "PTR_ITEM getItem( int id )");
	PTR_ITEM	newItem;
/* TODO 1 -cDB : perm check??? */
	if( id>0 )
	{
		size_t	cachePos = findItemInCache( id );
		if( cachePos != (size_t)-1 )
			newItem = itemCache[cachePos];
		else
		{
			std::auto_ptr<TQuery>	theQuery( new TQuery( NULL ) );
			theQuery->DatabaseName = "docManDB";
			theQuery->SQL->Add(
				"SELECT i.*, "
					"v.fileSize, v.fileModifiedDate, v.mime_type, f.usageCount as num_links "
				"FROM ITEM_TREE i "
				"LEFT OUTER JOIN I_FILES f "
				"ON i.FILE_ID = f.ID "
				"LEFT OUTER JOIN I_FILE_VERS v "
				"ON f.ID = v.file_id and f.version = v.version "
				"LEFT OUTER JOIN I_STORAGE s "
				"ON s.ID = v.storage_id "
				"where i.id = :theID"
			);
			theQuery->Params->Items[0]->AsInteger = id;
			theQuery->Open();
			if( !theQuery->Eof )
			{
				newItem = createItem(
					theQuery->FieldByName( "ITEM_TYPE" )->AsInteger,
					theQuery->FieldByName( "ID" )->AsInteger
				);
				if( newItem )
				{
					newItem->loadFields( theQuery.get() );
					itemCache.addElement( newItem );
				}

			}
			theQuery->Close();
		}
	}
	else
	{
		newItem = createItem( TYPE_ROOT );
		newItem->loadPermissions();
	}

	return newItem;
}

PTR_ITEM getItemByName( int id, const char *name )
{
	doEnterFunctionEx(gakLogging::llDetail, "getItemByName( int id - const char *name )");

	PTR_ITEM	newItem;
/* TODO 1 -cDB : perm check??? */
	std::auto_ptr<TQuery>	theQuery( new TQuery( NULL ) );
	theQuery->DatabaseName = "docManDB";
	theQuery->SQL->Add(
		"SELECT i.*, "
			"v.fileSize, v.fileModifiedDate, v.mime_type, f.usageCount as num_links "
		"FROM ITEM_TREE i "
		"LEFT OUTER JOIN I_FILES f "
		"ON i.FILE_ID = f.ID "
		"LEFT OUTER JOIN I_FILE_VERS v "
		"ON f.ID = v.file_id and f.version = v.version "
		"LEFT OUTER JOIN I_STORAGE s "
		"ON s.ID = v.storage_id "
		"where i.parentId = :theID and i.name=:theName"
	);
	theQuery->Params->Items[0]->AsInteger = id;
	theQuery->Params->Items[1]->AsString = name;
	theQuery->Open();
	if( !theQuery->Eof )
	{
		newItem = createItem(
			theQuery->FieldByName( "ITEM_TYPE" )->AsInteger,
			theQuery->FieldByName( "ID" )->AsInteger
		);
		if( newItem )
		{
			newItem->loadFields( theQuery.get() );
			addItemToCache( newItem );
		}

	}
	theQuery->Close();

	return newItem;
}

PTR_ITEM getItemByPath( T_STRING path )
{
	doEnterFunctionEx(gakLogging::llDetail, "getItemByPath( T_STRING path )");

	int			parentId = -1;
	PTR_ITEM	newItem;

	path += 1U;
	STRING name = path.getFirstToken( DIRECTORY_DELIMITER_STRING );
	while( 1 )
	{
		newItem = getItemByName( parentId, name );
		if( newItem )
		{
			parentId = (*newItem).getID();

			if( !path.hasNextToken() )
/*v*/			break;

			name = path.getNextToken();
		}
		else
/*v*/		break;
	}


	return newItem;
}

PTR_ITEM getPersonalItem( int itemType )
{
	doEnterFunctionEx(gakLogging::llDetail, "getPersonalItem");

	PTR_ITEM	newItem;

	std::auto_ptr<TQuery>	theQuery( new TQuery( NULL ) );
	theQuery->DatabaseName = "docManDB";
	theQuery->SQL->Add(
		"select ITEM_TREE.*, 1 as num_links "
		"from ITEM_TREE "
		"where item_type = :theType and createdBy = :theUser"
	);
	theQuery->Params->Items[0]->AsInteger = itemType;
	theQuery->Params->Items[1]->AsInteger = vcl::getActUserID();
	theQuery->Open();
	if( !theQuery->Eof )
	{
		newItem = createItem(
			itemType,
			theQuery->FieldByName( "ID" )->AsInteger
		);
		if( newItem )
			newItem->loadFields( theQuery.get() );

	}
	theQuery->Close();

	if( !newItem )
	{
		newItem = createItem( itemType );
		newItem->updateDatabase();
	}

	return newItem;
}

PTR_ITEM getPublicVolume( int itemType )
{
	doEnterFunctionEx(gakLogging::llDetail, "getPublicVolume");
	PTR_ITEM	newItem;

	std::auto_ptr<TQuery>	theQuery( new TQuery( NULL ) );
	theQuery->DatabaseName = "docManDB";
	theQuery->SQL->Add(
		"select ITEM_TREE.*, 1 as num_links "
		"from ITEM_TREE "
		"where item_type = :theType and parentID = -1"
	);
	theQuery->Params->Items[0]->AsInteger = itemType;
	theQuery->Open();
	if( !theQuery->Eof )
	{
		newItem = createItem(
			itemType,
			theQuery->FieldByName( "ID" )->AsInteger
		);

		if( newItem )
			newItem->loadFields( theQuery.get() );
	}
	theQuery->Close();

	if( !newItem )
	{
		newItem = createItem( itemType );
		newItem->updateDatabase();
	}

	return newItem;
}

PTR_ITEM getArchiveVolume( void )
{
	static PTR_ITEM archiveVolume;

	if( !archiveVolume )
		archiveVolume = getPublicVolume( TYPE_ARCHIVE_FOLDER );

	return archiveVolume;
}

PTR_ITEM getCompanyVolume( void )
{
	static PTR_ITEM companyVolume;

	if( !companyVolume )
		companyVolume = getPublicVolume( TYPE_COMPANY_FOLDER );

	return companyVolume;
}

void clearItemCache( const PTR_ITEM &holdItem )
{
	itemCache.clear();
	if( holdItem )
		itemCache.addElement( holdItem );
}

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -a.
#	pragma option -p.
#endif

