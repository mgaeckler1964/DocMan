//---------------------------------------------------------------------------

#include <vcl.h>

#include <gak/fmtNumber.h>
#include <gak/vcl_tools.h>

#pragma hdrstop

#include "ItemCreateFrm.h"
#include "ItemManager.h"
#include "PermissionsFrm.h"

//---------------------------------------------------------------------------
using namespace gak;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TItemCreateForm *ItemCreateForm;
//---------------------------------------------------------------------------
__fastcall TItemCreateForm::TItemCreateForm(TComponent* Owner)
	: TForm(Owner)
{
	theItem = theParent = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TItemCreateForm::FormShow(TObject *)
{
	bool	enableControls;

	if( theItem )
	{
		STRING			name = theItem->getName();
		AnsiString		newCaption = "Update ";
		newCaption += (const char *)name;

		Caption = newCaption;

		EditName->Text = (const char *)name;
		MemoDescription->Text = (const char *)theItem->getDescription();
	}
	else
	{
		FACTORY_BASE	*childFactory = (FACTORY_BASE *)Tag;
		if( childFactory )
		{
			AnsiString		newCaption = "Create ";
			newCaption += childFactory->getName();

			Caption = newCaption;

			STRING	newName = "New ";
			newName += childFactory->getName();

			if( theParent )
			{
				STRING	tmpName = newName;
				int		i=1;
				while( theParent->getContentItem( tmpName ) )
				{
					tmpName = newName;
					tmpName += formatNumber( ++i );
				}
				newName = tmpName;
			}

			EditName->Text = (const char *)newName;
		}
		else
		{
			EditName->Text = "";
			Caption = "Create Item";
		}

		MemoDescription->Text = "";
	}

	if( !theItem || (theItem->loadPermissions() & ITEM_PERM_MODIFY) )
	{
		ButtonCancel->Visible = true;
		ButtonOk->ModalResult = mrOk;

		enableControls = true;
	}
	else
	{
		ButtonCancel->Visible = false;
		ButtonOk->ModalResult = mrCancel;

		enableControls = false;
	}

	int 		numControls = ControlCount;
	for( size_t i=0; i<numControls; i++ )
	{
		TControl	*theControl = Controls[i];
		TLabel *theLabel = dynamic_cast<TLabel *>(theControl);
		if( !theLabel && theControl != ButtonOk )
			theControl->Enabled = enableControls;
	}
	vcl::bringWindowToFront( this );
	EditName->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TItemCreateForm::ButtonOkClick(TObject *)
{
	if( EditName->Text == "" )
	{
		ModalResult = 0;
		EditName->SetFocus();
		throw Exception( "Name cannot be empty" );
	}
}
//---------------------------------------------------------------------------

void __fastcall TItemCreateForm::FormClose(TObject *,
	  TCloseAction &)
{
	theItem = theParent = NULL;
}
//---------------------------------------------------------------------------

