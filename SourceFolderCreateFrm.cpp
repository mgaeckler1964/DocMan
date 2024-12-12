//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SourceFolderCreateFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SyncFolderCreateFrm"
#pragma resource "*.dfm"
TSourceFolderCreateForm *SourceFolderCreateForm;
//---------------------------------------------------------------------------
__fastcall TSourceFolderCreateForm::TSourceFolderCreateForm(TComponent* Owner)
	: TSyncFolderCreateForm(Owner)
{
}
//---------------------------------------------------------------------------


void __fastcall TSourceFolderCreateForm::FormShow(TObject *Sender)
{
	TItemCreateForm::FormShow( Sender );

	UpDownMajor->Position = 0;
	UpDownMinor->Position = 0;
	UpDownPatch->Position = 0;
}
//---------------------------------------------------------------------------


