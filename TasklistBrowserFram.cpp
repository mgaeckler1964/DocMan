/*
		Project:		DocMan
		Module:			
		Description:	
		Author:			Martin G�ckler
		Address:		Hofmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2024 Martin G�ckler

		This program is free software: you can redistribute it and/or modify  
		it under the terms of the GNU General Public License as published by  
		the Free Software Foundation, version 3.

		You should have received a copy of the GNU General Public License 
		along with this program. If not, see <http://www.gnu.org/licenses/>.

		THIS SOFTWARE IS PROVIDED BY Martin G�ckler, Austria, Linz ``AS IS''
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

#include "TasklistBrowserFram.h"
#include "DocManDM.h"
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

