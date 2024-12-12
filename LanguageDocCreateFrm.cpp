//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "LanguageDocCreateFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "FileCreateFrm"
#pragma resource "*.dfm"
TLanguageDocCreateForm *LanguageDocCreateForm;
//---------------------------------------------------------------------------
__fastcall TLanguageDocCreateForm::TLanguageDocCreateForm(TComponent* Owner)
	: TFileCreateForm(Owner)
{
}
//---------------------------------------------------------------------------

