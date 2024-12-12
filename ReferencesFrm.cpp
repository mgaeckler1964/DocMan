//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ReferencesFrm.h"
#include "ActionManager.h"
#include "File.h"
#include "PermissionsFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
class ACTION_SHOW_REFERENCES : public ACTION_BASE_MOVE
{
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
};
//---------------------------------------------------------------------------
TReferencesForm *ReferencesForm;
static ACTION_SHOW_REFERENCES theAction;

//---------------------------------------------------------------------------
bool ACTION_SHOW_REFERENCES::acceptItem( THE_ITEM *theItem )
{
	return ACTION_BASE::acceptItem( theItem );;
}
//---------------------------------------------------------------------------
const char *ACTION_SHOW_REFERENCES::getLabel( void ) const
{
	return "Show References...";
}
//---------------------------------------------------------------------------
REFRESH_TYPE ACTION_SHOW_REFERENCES::perform( PTR_ITEM theItem )
{
	ReferencesForm->setItem( theItem );
	ReferencesForm->ShowModal();

	return REFRESH_NONE;
}
//---------------------------------------------------------------------------
__fastcall TReferencesForm::TReferencesForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TReferencesForm::FormShow(TObject *)
{
	QueryReferences->ParamByName( "theID" )->AsInteger = (*theItem).getID();
	QueryReferences->Open();
}
//---------------------------------------------------------------------------
void __fastcall TReferencesForm::QueryReferencesCalcFields(TDataSet *)
{
	PTR_ITEM	theItem = getItem( QueryReferencesID->AsInteger );
	if( theItem )
		QueryReferencesPATH->AsString = (const char *)(*theItem).getPath();
	else
		QueryReferencesPATH->AsString = "???";
}
//---------------------------------------------------------------------------
void __fastcall TReferencesForm::FormClose(TObject *,
	  TCloseAction &)
{
	QueryReferences->Close();
}
//---------------------------------------------------------------------------
