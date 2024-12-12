//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include "TaskStatusFrm.h"
#include "DocManDM.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TTaskStatusForm *TaskStatusForm;
//---------------------------------------------------------------------------
__fastcall TTaskStatusForm::TTaskStatusForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TTaskStatusForm::FormShow(TObject *)
{
	DocManDataModule->TableTaskStatus->Open();
}
//---------------------------------------------------------------------------
void __fastcall TTaskStatusForm::FormClose(TObject *,
	  TCloseAction &)
{
	DocManDataModule->TableTaskStatus->Close();
}
//---------------------------------------------------------------------------
