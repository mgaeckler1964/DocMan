//---------------------------------------------------------------------------

#include <FileCtrl.hpp>
#include <vcl.h>
#pragma hdrstop

#include "ConfigFrm.h"
#include "DirSelFrm.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TConfigForm *ConfigForm;
//---------------------------------------------------------------------------
__fastcall TConfigForm::TConfigForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TConfigForm::SpeedButtonExternalStorageClick(TObject *)
{
	if( DirectorySelector->ShowModal( EditExternalStorage->Text ) == mrOk )
		EditExternalStorage->Text = DirectorySelector->DirectoryListBox->Directory;
}
//---------------------------------------------------------------------------

void __fastcall TConfigForm::SpeedButtonReportsClick(TObject *)
{
	if( DirectorySelector->ShowModal( EditReports->Text ) == mrOk )
		EditReports->Text = DirectorySelector->DirectoryListBox->Directory;
}
//---------------------------------------------------------------------------

