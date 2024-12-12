//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MakeLangFrm.h"
#include "ActionManager.h"
#include "File.h"

//---------------------------------------------------------------------------
using namespace gak;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMakeLanguageForm *MakeLanguageForm;
//---------------------------------------------------------------------------
class ACTION_MAKE_LANGUAGE : public ACTION_BASE_MOVE
{
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
};
//---------------------------------------------------------------------------
bool ACTION_MAKE_LANGUAGE::acceptItem( THE_ITEM *theItem )
{
	if( !dynamic_cast<THE_FILE *>( theItem )
	&&  !dynamic_cast<THE_LANGUAGE_ITEM *>( theItem ) )
		return false;

	return ACTION_BASE::acceptItem( theItem );
}
//---------------------------------------------------------------------------
const char *ACTION_MAKE_LANGUAGE::getLabel( void ) const
{
	return "Make Language...";
}
//---------------------------------------------------------------------------
REFRESH_TYPE ACTION_MAKE_LANGUAGE::perform( PTR_ITEM theItem )
{
	if( MakeLanguageForm->ShowModal() == mrOk )
	{
		PTR_LANGUAGE_ITEM theLangItem = theItem;

		if( theLangItem )
		{
			THE_ITEM::ItemsCopiedMap itemsCopied;
			theItem->copy(
				itemsCopied,
				theItem->getParent(),
				false,
				MakeLanguageForm->EditLanguage->Text.c_str()
			);
		}
		else
		{
			STRING		oldName = (*theItem).getName();
			STRING		tmpName = "TMP";

			tmpName += formatNumber( theItem->getID() );
			theItem->setName( tmpName );
			theItem->updateDatabase();

			PTR_ITEM newItem = createItem( TYPE_LANGUAGE_DOC );
			newItem->setData(
				theItem->getParent(), oldName, theItem->getDescription()
			);
			newItem->updateDatabase();

			TQuery *theQuery = new TQuery( NULL );
			theQuery->DatabaseName = "docManDb";
			theQuery->SQL->Add(
				"update item_tree "
				"set item_type=:newType, name=:newName, parentId = :newParent "
				"where id = :oldId"
			);
			theQuery->Params->Items[0]->AsInteger = TYPE_LANGUAGE_ITEM;
			theQuery->Params->Items[1]->AsString  = MakeLanguageForm->EditLanguage->Text;
			theQuery->Params->Items[2]->AsInteger = (*newItem).getID();
			theQuery->Params->Items[3]->AsInteger = (*theItem).getID();

			theQuery->ExecSQL();

		}

		return REFRESH_RELOAD;
	}

	return REFRESH_NONE;
}
//---------------------------------------------------------------------------
static ACTION_MAKE_LANGUAGE theAction;
//---------------------------------------------------------------------------
__fastcall TMakeLanguageForm::TMakeLanguageForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
