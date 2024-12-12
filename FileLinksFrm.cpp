//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FileLinksFrm.h"
#include "ActionManager.h"
#include "File.h"
#include "PermissionsFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
class ACTION_SHOW_LINKS : public ACTION_BASE_MOVE
{
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
};
//---------------------------------------------------------------------------
TFileLinksForm *FileLinksForm;
static ACTION_SHOW_LINKS theLinksAction;

//---------------------------------------------------------------------------
bool ACTION_SHOW_LINKS::acceptItem( THE_ITEM *theItem )
{
	const THE_FILE *theFile = dynamic_cast<const THE_FILE *>(theItem);
	if( !theFile )
		return false;

	int numLinks = theItem->getNumLinks();
	if( numLinks <= 1 )
		return false;

	return ACTION_BASE::acceptItem( theItem );;
}
//---------------------------------------------------------------------------
const char *ACTION_SHOW_LINKS::getLabel( void ) const
{
	return "Show Links...";
}
//---------------------------------------------------------------------------
REFRESH_TYPE ACTION_SHOW_LINKS::perform( PTR_ITEM theItem )
{
	PTR_FILE	theFile( theItem );

	FileLinksForm->setFile( theFile );
	FileLinksForm->ShowModal();

	return REFRESH_NONE;
}
//---------------------------------------------------------------------------
__fastcall TFileLinksForm::TFileLinksForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFileLinksForm::FormShow(TObject *)
{
	QueryLinks->ParamByName( "theFileId" )->AsInteger = (*theFile).getFileID();
	QueryLinks->Open();
}
//---------------------------------------------------------------------------
void __fastcall TFileLinksForm::QueryLinksCalcFields(TDataSet *)
{
	PTR_ITEM	theItem = getItem( QueryLinksID->AsInteger );
	if( theItem )
		QueryLinksPATH->AsString = (const char *)(*theItem).getPath();
	else
		QueryLinksPATH->AsString = "???";
}
//---------------------------------------------------------------------------
void __fastcall TFileLinksForm::FormClose(TObject *,
	  TCloseAction &)
{
	QueryLinks->Close();
}
//---------------------------------------------------------------------------
