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

//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DocManMain.h"
#include "BrowseFram.h"
#include "ItemManager.h"
#include "ActionManager.h"
#include "Delete.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TBrowserFrame *BrowserFrame;
//---------------------------------------------------------------------------
__fastcall TBrowserFrame::TBrowserFrame(TComponent* Owner )
	: TFrame(Owner)
{
}
//---------------------------------------------------------------------------
int TBrowserFrame::getVisibleItemIndex( int ARow )
{
	return ARow - (*currentItem).getHeaderCount();
}
//---------------------------------------------------------------------------
void __fastcall TBrowserFrame::DrawGridContentDrawCell(TObject *,
	  int ACol, int ARow, TRect &Rect, TGridDrawState )
{
	STRING text;

	TCanvas *canvas = DrawGridContent->Canvas;

	::SetCursor( LoadCursor( NULL, IDC_WAIT ) );

	int	itemIndex = getVisibleItemIndex( ARow );
	if( itemIndex < 0 )
		text = (*currentItem).drawHeaderCell( ACol, ARow, canvas, Rect, ImageListSort );
	else
		text = (*currentItem).drawCell( ACol, itemIndex, canvas, Rect );

	if( !text.isEmpty() )
		canvas->TextOut( Rect.Left+2, Rect.Top+3, (const char *)text );

	::SetCursor( LoadCursor( NULL, IDC_ARROW ) );
}
//---------------------------------------------------------------------------
void __fastcall TBrowserFrame::DrawGridContentMouseDown(TObject *,
	  TMouseButton Button, TShiftState , int X, int Y)
{
	if( Button == mbRight )
	{
		int	row, col, group=-1;

		DrawGridContent->MouseToCell( X, Y, col, row );
		int	itemIndex = getVisibleItemIndex( row );
		if( itemIndex >= 0 )
		{
			DrawGridContent->Row = row;
			selectedItem = (*currentItem).getContentItem( itemIndex );
			if( selectedItem )
			{
				getItemActions( *DocManMainForm->getActUser(), selectedItem, &popupActions );
				PopupMenuActions->Items->Clear();
				for( size_t i=0; i<popupActions.size(); i++ )
				{
					ACTION_BASE *theAction = popupActions[i];
					if( i>0 && theAction->getGroup() != group )
					{
						TMenuItem	*newItem = new TMenuItem( PopupMenuActions );
						newItem->Caption = "-";
						PopupMenuActions->Items->Add( newItem );
					}
					group = theAction->getGroup();

					TMenuItem	*newItem = new TMenuItem( PopupMenuActions );
					newItem->Caption = theAction->getLabel();
					newItem->Tag = i;
					newItem->OnClick = ActionClick;
					PopupMenuActions->Items->Add( newItem );
				}
				TPoint	thePoint( X, Y );
				thePoint = DrawGridContent->ClientToScreen( thePoint );

				PopupMenuActions->Popup( thePoint.x, thePoint.y );
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TBrowserFrame::DrawGridContentMouseUp(TObject *,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	try
	{
		if( Button == mbLeft )
		{
			int	row, col;

			DrawGridContent->MouseToCell( X, Y, col, row );
			int	itemIndex = getVisibleItemIndex( row );
			if( itemIndex >= 0 )
			{
				DrawGridContent->Row = row;

				if( !Shift.Contains( ssCtrl	) )
					currentItem->openItem( itemIndex );
			}
			else
			{
				int itemId = selectedItem ? (*selectedItem).getID() : 0;
				currentItem->sort( currentItem->getSortType( col ), false );
				if( itemId )
					selectItem( itemId );

				DrawGridContent->Invalidate();
			}
		}
	}
	catch( std::exception &e )
	{
		throw Exception( e.what() );
	}
}
//---------------------------------------------------------------------------
void TBrowserFrame::selectItem( int selectID )
{
	PTR_ITEM		child;
	ITEM_CONTENT	*theContent = currentItem->getContent();
	std::size_t 	contentCount = theContent->size();

	for( size_t i=0; i<contentCount; i++ )
	{
		child = currentItem->getContentItem( i );
		if( child->getID() == selectID )
		{
			DrawGridContent->Row = i+1;
/*v*/		break;
		}
	}
}

//---------------------------------------------------------------------------
void TBrowserFrame::fillContents( PTR_ITEM currentItem, int selectID )
{
	doEnterFunction( "TBrowserFrame::fillContents()" );

	::SetCursor( LoadCursor( NULL, IDC_WAIT ) );

	this->currentItem = currentItem;
	colCount = currentItem->getColCount();
	colWidths = currentItem->getColWidth();

	try
	{
		currentItem->loadContent();
	}
	catch( Exception &e )
	{
		Application->ShowException( &e );
	}
	catch( std::exception &e )
	{
		std::auto_ptr<Exception>	vclException( new Exception( e.what() ) );
		Application->ShowException( vclException.get() );
	}
	catch( ... )
	{
		Application->HandleException( this );
	}
	currentItem->sort();

	if( currentItem->isDeleted() || PTR_PERSONAL_TRASH(currentItem) )
		DrawGridContent->Color = clLtGray;
	else if( currentItem->isArchived() )
		DrawGridContent->Color = clAqua;
	else
		DrawGridContent->Color = clWindow;

	DrawGridContent->ColCount = colCount;
	int numItems = currentItem->getRowCount();
	if( !numItems )		// one empty row, if there is no content
		numItems = 1;

	DrawGridContent->RowCount = numItems + currentItem->getHeaderCount();
	DrawGridContent->FixedRows = currentItem->getHeaderCount();

	numStrechingCols = 0;
	fixedWidth = 0;
	for( size_t i=0; i<colCount; i++ )
	{
		if( colWidths[i] )
		{
			DrawGridContent->ColWidths[i] = colWidths[i];
			fixedWidth += colWidths[i];
		}
		else
			numStrechingCols++;
	}

	FrameResize( NULL );

	DrawGridContent->Row = 1;
	if( selectID >= 0 )
	{
		selectItem( selectID );
	}

	DrawGridContent->Invalidate();

	::SetCursor( LoadCursor( NULL, IDC_ARROW ) );
}
//---------------------------------------------------------------------------
void __fastcall TBrowserFrame::ActionClick(TObject *Sender)
{
	try
	{
		int i = static_cast<TMenuItem *>(Sender)->Tag;
		if( std::size_t(i) < popupActions.size() )
		{
			ACTION_BASE *theAction = popupActions[i];

			switch( theAction->perform( selectedItem ) )
			{
				case REFRESH_RELOAD:
					fillContents( currentItem, selectedItem->getID() );
					break;
				case REFRESH_REDRAW:
					DrawGridContent->Invalidate();
					break;
			}
		}
		popupActions.clear();
	}
	catch( std::exception &e )
	{
		popupActions.clear();
		fillContents( currentItem, selectedItem->getID() );
		throw Exception( e.what() );
	}
}
//---------------------------------------------------------------------------
void __fastcall TBrowserFrame::FrameResize(TObject *)
{
	if( numStrechingCols )
	{
		int variableWidth = (ClientWidth - fixedWidth-colCount-25)/numStrechingCols;
		if( variableWidth < 50 )
			variableWidth = 50;

		for( size_t i=0; i<colCount; i++ )
		{
			if( !colWidths[i] )
				DrawGridContent->ColWidths[i] = variableWidth;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TBrowserFrame::DrawGridContentSelectCell(TObject *,
	  int , int ARow, bool &CanSelect)
{
	int	itemIndex = getVisibleItemIndex( ARow );
	if( itemIndex >= 0 )
	{
		selectedItem = currentItem->getContentItem( itemIndex );
		CanSelect = true;
	}
	else
		CanSelect = false;
}
//---------------------------------------------------------------------------

