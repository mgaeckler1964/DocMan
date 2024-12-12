//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "LanguageItemCreateFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "FileCreateFrm"
#pragma resource "*.dfm"
TLanguageItemCreateForm *LanguageItemCreateForm;
//---------------------------------------------------------------------------
__fastcall TLanguageItemCreateForm::TLanguageItemCreateForm(TComponent* Owner)
	: TFileCreateForm(Owner)
{
}
//---------------------------------------------------------------------------
