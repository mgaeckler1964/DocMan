//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include "TasklistBrowserFram.h"
#include "DocManDM.h"
#include "DocManMain.h"
#include "TaskList.h"

//---------------------------------------------------------------------------
using namespace gak;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BrowseFram"
#pragma resource "*.dfm"
TTasklistBrowserFrame *TasklistBrowserFrame;

//---------------------------------------------------------------------------
__fastcall TTasklistBrowserFrame::TTasklistBrowserFrame(TComponent* Owner)
	: TBrowserFrame(Owner)
{
}
//---------------------------------------------------------------------------
void TTasklistBrowserFrame::refreshVisibleItems( void )
{
	int				assignedTo;
	int				actUser = vcl::getActUserID();
	STRING			curStatus, curType;
	int				numVisibleItems = 0;
	ITEM_CONTENT	*theContent = currentItem->getContent();

	STRING	selStatus =
		( ComboBoxStatusFilter->ItemIndex > 0 )
		? ComboBoxStatusFilter->Items->Strings[ComboBoxStatusFilter->ItemIndex].c_str()
		: ""
	;

	STRING	selType =
		( ComboBoxTypeFilter->ItemIndex > 0 )
		? ComboBoxTypeFilter->Items->Strings[ComboBoxTypeFilter->ItemIndex].c_str()
		: ""
	;

	visibleItems.clear();
	for( size_t i=0; i<theContent->size(); i++ )
	{
		PTR_TASK	theTask = currentItem->getContentItem(i);

		if( theTask )
		{
			assignedTo = theTask->getAssignedTo();
			curStatus = DocManDataModule->getStatus( theTask->getStatusID() );
			curType = DocManDataModule->getType( theTask->getTypeID() );
		}
		else
		{
			assignedTo = 0;
			curStatus = "";
			curType = "";
		}

		if( !CheckBoxOwner->Checked || assignedTo == actUser )
		{
			if( selStatus.isEmpty() || selStatus == curStatus )
			{
				if( selType.isEmpty() || selType == curType )
				{
					visibleItems.addElement( i );
					numVisibleItems++;
				}
			}
		}
	}

	if( !numVisibleItems )		// one empty row, if there is no content
		numVisibleItems = 1;

	DrawGridContent->RowCount = numVisibleItems + currentItem->getHeaderCount();
	DrawGridContent->Invalidate();
}
//---------------------------------------------------------------------------
int TTasklistBrowserFrame::getVisibleItemIndex( int ARow )
{
	int	itemIndex = TBrowserFrame::getVisibleItemIndex( ARow );
	if( itemIndex >= 0 )
	{
		if( itemIndex < visibleItems.size() )
		{
			itemIndex = visibleItems[itemIndex];
		}
		else
		{
			itemIndex = currentItem->getContent()->size();
		}
	}

	return itemIndex;
}
//---------------------------------------------------------------------------
void TTasklistBrowserFrame::fillContents( PTR_ITEM currentItem, int selectID )
{
	doEnterFunction( "TTasklistBrowserFrame::fillContents()" );


	::SetCursor( LoadCursor( NULL, IDC_WAIT ) );

	/*
		refresh the status list
	*/
	if( ComboBoxStatusFilter->ItemIndex < 0 )
	{
		ComboBoxStatusFilter->Items->Clear();
		ComboBoxStatusFilter->Items->Add( "All Status" );
	}
	DocManDataModule->refreshTaskStatus(
		ComboBoxStatusFilter->ItemIndex < 0
		? ComboBoxStatusFilter->Items
		: NULL
	);
	if( ComboBoxStatusFilter->ItemIndex < 0 )
		ComboBoxStatusFilter->ItemIndex = 0;


	/*
		refresh the type list
	*/
	if( ComboBoxTypeFilter->ItemIndex < 0 )
	{
		ComboBoxTypeFilter->Items->Clear();
		ComboBoxTypeFilter->Items->Add( "All Types" );
	}

	DocManDataModule->refreshTaskTypes(
		ComboBoxTypeFilter->ItemIndex < 0
		? ComboBoxTypeFilter->Items
		: NULL
	);

	if( ComboBoxTypeFilter->ItemIndex < 0 )
		ComboBoxTypeFilter->ItemIndex = 0;


	TBrowserFrame::fillContents( currentItem, selectID );

	refreshVisibleItems();

	::SetCursor( LoadCursor( NULL, IDC_ARROW ) );
}
//---------------------------------------------------------------------------
void __fastcall TTasklistBrowserFrame::FilterChange(TObject *)
{
	refreshVisibleItems();
}
//---------------------------------------------------------------------------

void __fastcall TTasklistBrowserFrame::DrawGridContentMouseUp(
	  TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
	  int Y)
{
	TBrowserFrame::DrawGridContentMouseUp( Sender, Button, Shift, X, Y );
	refreshVisibleItems();
}
//---------------------------------------------------------------------------

