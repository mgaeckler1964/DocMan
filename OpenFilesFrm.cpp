//---------------------------------------------------------------------------

#include <vcl.h>
#include <gak/vcl_tools.h>

#pragma hdrstop

#include "OpenFilesFrm.h"
#include "ItemManager.h"
#include "DocManDM.h"
#include "DocManMain.h"

//---------------------------------------------------------------------------
using namespace gak;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TOpenFilesForm *OpenFilesForm;
//---------------------------------------------------------------------------
__fastcall TOpenFilesForm::TOpenFilesForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void TOpenFilesForm::reloadQuery( void )
{
}
//---------------------------------------------------------------------------
void __fastcall TOpenFilesForm::ReloadQuery(TObject *)
{
	if( !vcl::isSystemAdmin() )
	{
		CheckBoxAllFiles->Checked = false;
		CheckBoxAllFiles->Enabled = false;
		CheckBoxAllFiles->Visible = false;
	}
	else
	{
		CheckBoxAllFiles->Enabled = true;
		CheckBoxAllFiles->Visible = true;
	}
	QueryOpenFiles->Close();
	QueryOpenFiles->SQL->Clear();
	QueryOpenFiles->SQL->Add(
		"select it.ID, it.ParentID, it.Name, it.ReservedBy, it.ReservedOn, it.ReservedFor, ut.Username "
		"from item_tree it "
		"left join user_tab ut on ut.id = it.ReservedBy "
		"where it.ReservedBy <> 0 "
	);

	if( !CheckBoxAllFiles->Checked )
		QueryOpenFiles->SQL->Add( "and it.ReservedBy = :actUser" );

	QueryOpenFiles->SQL->Add( "order by it.ParentID, it.name" );

	if( !CheckBoxAllFiles->Checked )
	{
		QueryOpenFiles->Params->Items[0]->AsInteger = vcl::getActUserID();
	}

	QueryOpenFiles->Open();
}
//---------------------------------------------------------------------------
void __fastcall TOpenFilesForm::FormHide(TObject *)
{
	QueryOpenFiles->Close();
}
//---------------------------------------------------------------------------
void __fastcall TOpenFilesForm::QueryOpenFilesCalcFields(TDataSet *)
{
	int		parentID = QueryOpenFilesParentID->AsInteger;
	STRING	path = DocManDataModule->getPathForID( parentID );
	QueryOpenFilesPATH->AsString = (const char *)path;

	int		taskId = QueryOpenFilesReservedFor->AsInteger;
	if( taskId )
		path = DocManDataModule->getPathForID( taskId );
	else
		path = "";
	QueryOpenFilesTASK_PATH->AsString = (const char *)path;
}
//---------------------------------------------------------------------------

void __fastcall TOpenFilesForm::DBGridDblClick(TObject *)
{
	PTR_ITEM	theParentItem;
	int			theItemId;

	if( DBGrid->SelectedField == QueryOpenFilesTASK_PATH )
	{
		PTR_ITEM theTask = getItem( QueryOpenFilesReservedFor->AsInteger );
		if( theTask )
		{
			theParentItem = (*theTask).getParent();
			theItemId = QueryOpenFilesReservedFor->AsInteger;
		}
	}
	else
	{
		theParentItem = getItem( QueryOpenFilesParentID->AsInteger );
		theItemId = QueryOpenFilesID->AsInteger;
	}
	if( theParentItem )
	{
		DocManMainForm->openItem( theParentItem, theItemId );
		DocManMainForm->BringToFront();
		DocManMainForm->SetFocus();
	}
}
//---------------------------------------------------------------------------

void __fastcall TOpenFilesForm::FormCreate(TObject *)
{
	::SetWindowLong( Handle, GWL_HWNDPARENT, NULL );
}
//---------------------------------------------------------------------------

