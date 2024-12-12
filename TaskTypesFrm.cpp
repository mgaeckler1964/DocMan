//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include "TaskTypesFrm.h"
#include "DocManDM.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TTaskTypesForm *TaskTypesForm;
//---------------------------------------------------------------------------
__fastcall TTaskTypesForm::TTaskTypesForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TTaskTypesForm::FormShow(TObject *)
{
	DocManDataModule->TableTaskTypes->Open();
}
//---------------------------------------------------------------------------
void __fastcall TTaskTypesForm::FormClose(TObject *,
	  TCloseAction &)
{
	DocManDataModule->TableTaskTypes->Close();
}
//---------------------------------------------------------------------------
