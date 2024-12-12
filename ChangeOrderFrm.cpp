//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ChangeOrderFrm.h"
#include "ActionManager.h"
#include "Folder.h"
#include "PermissionsFrm.h"
#include "TestFolder.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TChangeOrderForm *ChangeOrderForm;
//---------------------------------------------------------------------------
class ACTION_ORDERING : public ACTION_BASE_PROPERTIES
{
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
};
//---------------------------------------------------------------------------
static ACTION_ORDERING	theAction;

//---------------------------------------------------------------------------
bool ACTION_ORDERING::acceptItem( THE_ITEM *theItem )
{
	if( !dynamic_cast<THE_FOLDER *>(theItem)
	&&  !dynamic_cast<THE_LANGUAGE_DOC*>(theItem)
	&&  !dynamic_cast<THE_TEST_FOLDER*>(theItem) )
/*@*/	return false;

	int perms = theItem->getUserPermissions();
	return (perms & ITEM_PERM_MODIFY)
		? ACTION_BASE::acceptItem( theItem )
		: false
	;
}
//---------------------------------------------------------------------------
const char *ACTION_ORDERING::getLabel( void ) const
{
	return "Change Order...";
}
//---------------------------------------------------------------------------
REFRESH_TYPE ACTION_ORDERING::perform( PTR_ITEM theItem )
{
	int perms = theItem->getUserPermissions();
	if( !perms & ITEM_PERM_MODIFY )
	{
/*@*/	throw Exception( "Permission denied" );
	}

	ChangeOrderForm->ListBoxItems->Items->Clear();
	theItem->sort( SORT_ORDER );

	PTR_ITEM		child;
	ITEM_CONTENT	*theContent = theItem->getContent();
	for(
		ITEM_CONTENT::const_iterator it = theContent->cbegin(),
			endIT = theContent->cend();
		it != endIT;
		++it
	)
	{
		child = *it;
		ChangeOrderForm->ListBoxItems->Items->Add(
			static_cast<const char *>(child->getName())
		);
	}

	switch( ChangeOrderForm->ShowModal() )
	{
	case mrOk:
		for(
			ITEM_CONTENT::const_iterator it = theContent->cbegin(),
				endIT = theContent->cend();
			it != endIT;
			++it
		)
		{
			child = *it;
			child->setOrder(
				ChangeOrderForm->ListBoxItems->Items->IndexOf(
					static_cast<const char *>(child->getName())
				)+1
			);
			child->updateDatabase();
		}

/***/	return REFRESH_REDRAW;
	case mrNoToAll:
		for(
			ITEM_CONTENT::const_iterator it = theContent->cbegin(),
				endIT = theContent->cend();
			it != endIT;
			++it
		)
		{
			child = *it;
			child->setOrder( 0 );
			child->updateDatabase();
		}

/***/	return REFRESH_REDRAW;
	}

	return REFRESH_NONE;
}

//---------------------------------------------------------------------------
__fastcall TChangeOrderForm::TChangeOrderForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TChangeOrderForm::ListBoxItemsEndDrag(TObject *,
	  TObject *, int, int Y )
{
	if( ListBoxItems->ItemIndex >= 0 )
	{
		int		dragOverIndex = Y/ListBoxItems->ItemHeight + ListBoxItems->TopIndex;

		if( dragOverIndex >= 0 && dragOverIndex < ListBoxItems->Items->Count )
			ListBoxItems->Items->Move( ListBoxItems->ItemIndex, dragOverIndex );
	}
}
//---------------------------------------------------------------------------
void __fastcall TChangeOrderForm::ListBoxItemsDragOver(TObject *,
	  TObject *, int , int Y, TDragState , bool &Accept)
{
	if( ListBoxItems->ItemIndex < 0 )
		Accept = false;
	else
	{
		int		dragOverIndex = Y/ListBoxItems->ItemHeight + ListBoxItems->TopIndex;
		if( dragOverIndex < 0 || dragOverIndex >= ListBoxItems->Items->Count )
			Accept = false;
		else
			Accept = true;
	}
}
//---------------------------------------------------------------------------

