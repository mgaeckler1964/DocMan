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
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
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
REFRESH_TYPE ACTION_MULTIACTIONS::perform( PTR_ITEM theItem )
{
	MultiActionForm->setItem( theItem );
	if( MultiActionForm->ShowModal() == mrOk )
		return REFRESH_RELOAD;
	else
		return REFRESH_NONE;
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

