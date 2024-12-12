//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "GeneralFrm.h"
#include "ActionManager.h"
#include "ItemManager.h"
#include "File.h"
#include "UserDlg.h"
#include "DocManMain.h"

//---------------------------------------------------------------------------
using namespace gak;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ItemCreateFrm"
#pragma resource "*.dfm"
TGeneralForm *GeneralForm;
//---------------------------------------------------------------------------
class ACTION_GENERAL : public ACTION_BASE_PROPERTIES
{
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
	virtual const char *getLabel( void ) const;
};
//---------------------------------------------------------------------------
REFRESH_TYPE ACTION_GENERAL::perform( PTR_ITEM theItem )
{
	GeneralForm->setItem( theItem );
	if( GeneralForm->ShowModal() == mrOk )
	{
		theItem->setName( GeneralForm->EditName->Text.c_str() );
		theItem->setDescription( GeneralForm->MemoDescription->Text.c_str() );
		theItem->updateDatabase();
		return REFRESH_RELOAD;
	}
	else
		return REFRESH_NONE;
}
//---------------------------------------------------------------------------
const char *ACTION_GENERAL::getLabel( void ) const
{
	return "General...";
}
//---------------------------------------------------------------------------
static ACTION_GENERAL theAction;
//---------------------------------------------------------------------------
__fastcall TGeneralForm::TGeneralForm(TComponent* Owner)
	: TItemCreateForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TGeneralForm::FormShow(TObject *Sender)
{
	int			creatorId;
	UserOrGroup	creator, reservedFor;

	TItemCreateForm::FormShow( Sender );

	LabelType->Caption = theItem->getItemTypeString();
	LabelLocalPath->Caption = (const char *)theItem->getDownloadPath();

	creatorId = theItem->getCreator();
	DocManMainForm->getUserById( creatorId, &creator );
	LabelCreator->Caption = (const char *)creator.userName;

	TDateTime createdDate = theItem->getCreatedDate();
	TDateTime modifiedDate = theItem->getModifiedDate();

	LabelCreatedDate->Caption = createdDate.DateTimeString();
	LabelModifiedDate->Caption = modifiedDate.DateTimeString();

	LabelReserved->Visible = false;
	LabelReservedFor->Visible = false;
	PTR_FILE	theFile = theItem;
	if( theFile )
	{
		int reservedBy = theFile->getReservedBy();
		if( reservedBy )
		{
			DocManMainForm->getUserById( reservedBy, &reservedFor );
			LabelReserved->Visible = true;
			LabelReservedFor->Visible = true;
			LabelReservedFor->Caption = (const char *)reservedFor.userName;
		}
	}
	vcl::bringWindowToFront( this );
}
//---------------------------------------------------------------------------
