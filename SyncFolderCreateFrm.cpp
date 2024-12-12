//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SyncFolderCreateFrm.h"
#include "DirSelFrm.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ItemCreateFrm"
#pragma resource "*.dfm"
TSyncFolderCreateForm *SyncFolderCreateForm;
//---------------------------------------------------------------------------
__fastcall TSyncFolderCreateForm::TSyncFolderCreateForm(TComponent* Owner)
	: TItemCreateForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TSyncFolderCreateForm::SpeedButtonClick(TObject *)
{
	if( DirectorySelector->ShowModal( EditLocalPath->Text ) == mrOk )
		EditLocalPath->Text = DirectorySelector->DirectoryListBox->Directory;
}
//---------------------------------------------------------------------------
