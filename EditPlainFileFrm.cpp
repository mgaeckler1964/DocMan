//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "EditPlainFileFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "EditFrm"
#pragma resource "*.dfm"
TEditPlainFileForm *EditPlainFileForm;
//---------------------------------------------------------------------------
__fastcall TEditPlainFileForm::TEditPlainFileForm(TComponent* Owner)
	: TEditFileForm(Owner)
{
}
//---------------------------------------------------------------------------
void TEditPlainFileForm::setText( const PTR_FILE &, const STRING &data )
{
	MemoPlainFile->Text = (const char *)data;
}

//---------------------------------------------------------------------------
STRING TEditPlainFileForm::getText( void )
{
	return MemoPlainFile->Text.c_str();
}
//---------------------------------------------------------------------------





