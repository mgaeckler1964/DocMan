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

#include "MultiActionFrm.h"
#include "ActionManager.h"
#include "Folder.h"
#include "PermissionsFrm.h"
#include "TargetBrowseFrm.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMultiActionForm *MultiActionForm;
//---------------------------------------------------------------------------
class ACTION_MULTIACTIONS : public ACTION_BASE_MOVE
{
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual RefhreshType perform( PTR_ITEM theItem );
};
//---------------------------------------------------------------------------
static ACTION_MULTIACTIONS	theAction;

//---------------------------------------------------------------------------
bool ACTION_MULTIACTIONS::acceptItem( THE_ITEM *theItem )
{
	THE_FOLDER *theFolder = dynamic_cast<THE_FOLDER *>(theItem);
	if( !theFolder )
/*@*/	return false;

	return ACTION_BASE::acceptItem( theItem );
}
//---------------------------------------------------------------------------
const char *ACTION_MULTIACTIONS::getLabel( void ) const
{
	return "Actions...";
}
//---------------------------------------------------------------------------
RefhreshType ACTION_MULTIACTIONS::perform( PTR_ITEM theItem )
{
	MultiActionForm->setItem( theItem );
	if( MultiActionForm->ShowModal() == mrOk )
		return rtRELOAD;
	else
		return rtNONE;
}

//---------------------------------------------------------------------------
__fastcall TMultiActionForm::TMultiActionForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void TMultiActionForm::setItem( PTR_ITEM theItem )
{
	this->theItem = theItem;

	ListBoxItems->Items->Clear();

	ITEM_CONTENT	*theContent = theItem->getContent();
	for(
		ITEM_CONTENT::const_iterator it = theContent->cbegin(),
			endIT = theContent->cend();
		it != endIT;
		++it
	)
	{
		PTR_ITEM	child = *it;
		ListBoxItems->Items->Add( static_cast<const char *>(child->getName()) );
	}

	ButtonCopy->Enabled = false;
	ButtonMove->Enabled = false;
	ButtonDelete->Enabled = false;
}
//---------------------------------------------------------------------------


void __fastcall TMultiActionForm::DoAction(TObject *Sender)
{
	int				result = mrCancel;
	ThreadDocMan	*theThread = NULL;

	if( Sender == ButtonCopy )
	{
		TargetBrowseForm->setItem( theItem, false );
		result = TargetBrowseForm->ShowModal();

		if( result == mrOk )
		{
			TTreeNode *selected = TargetBrowseForm->TreeView->Selected;
			if( selected )
			{
				theThread = new ThreadCopying(
					theItem,
					getItem( int(selected->Data) ),
					false, false,
					""
				);
			}
			else
				result = mrCancel;
		}
	}
	if( Sender == ButtonMove )
	{
		TargetBrowseForm->setItem( theItem, false );
		result = TargetBrowseForm->ShowModal();

		if( result == mrOk )
		{
			TTreeNode *selected = TargetBrowseForm->TreeView->Selected;
			if( selected )
			{
				theThread = new ThreadMoving(
					theItem,
					getItem( (int)selected->Data ),
					""
				);
			}
			else
			{
				result = mrCancel;
			}
		}
	}
	else if( Sender == ButtonDelete )
	{
		theThread = new ThreadDeletion( theItem );
	}
	if( theThread )
	{
		std::size_t	numItems = 0;
		for( int i=0; i<ListBoxItems->Items->Count; i++ )
		{
			if( ListBoxItems->Selected[i] )
			{
				PTR_ITEM	theSelItem = theItem->getContentItem(
					ListBoxItems->Items->Strings[i].c_str()
				);
				if( theSelItem )
				{
					theThread->addItem( theSelItem );
					numItems++;
				}
			}
		}
		if( numItems )
		{
			result = mrOk;
			theThread->StartThread();
		}
		else
		{
			result = mrCancel;
		}
	}
	ModalResult = result;
}
//---------------------------------------------------------------------------

void __fastcall TMultiActionForm::ListBoxItemsClick(TObject *)
{
	if( ListBoxItems->SelCount )
	{
		ButtonCopy->Enabled = true;
		ButtonMove->Enabled = true;
		ButtonDelete->Enabled = true;
	}
	else
	{
		ButtonCopy->Enabled = false;
		ButtonMove->Enabled = false;
		ButtonDelete->Enabled = false;
	}
}
//---------------------------------------------------------------------------

