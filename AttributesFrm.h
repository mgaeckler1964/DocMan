//---------------------------------------------------------------------------

#ifndef AttributesFrmH
#define AttributesFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include <ExtCtrls.hpp>

#include <gak/array.h>

#include "DocManDM.h"
#include "DocManMain.h"

//---------------------------------------------------------------------------
class TAttributesForm : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
	TPanel *Panel1;
	TComboBox *ComboBoxCategories;
	TButton *ButtonAdd;
	TButton *ButtonRemove;
	TPanel *Panel2;
	TButton *ButtonSave;
	TButton *ButtonCancel;
	TButton *ButtonUpgrade;
	TPageControl *PageControl;
	TTable *TableCategories;
	TIntegerField *TableCategoriesID;
	TStringField *TableCategoriesNAME;
	TIntegerField *TableCategoriesMAX_ATTR_ID;
	TQuery *delQuery;
	TQuery *insertQuery;
	TQuery *QueryCategoryForItem;
	TIntegerField *QueryCategoryForItemID;
	TStringField *QueryCategoryForItemNAME;
	TIntegerField *QueryCategoryForItemCATEGORY_VERSION;
	TButton *ButtonApplyDown;
	TQuery *QueryCatVersion;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ButtonAddClick(TObject *Sender);
	void __fastcall ButtonRemoveClick(TObject *Sender);
	void __fastcall ComboBoxCategoriesChange(TObject *Sender);
	void __fastcall ButtonSaveClick(TObject *Sender);
	void __fastcall PageControlChange(TObject *Sender);
	void __fastcall ButtonUpgradeClick(TObject *Sender);
	void __fastcall ButtonApplyDownClick(TObject *Sender);
private:	// Anwender-Deklarationen
	gak::ArrayOfStrings	selectedCategories;
	TTabSheets			tabSheets;
	CategoryDatas		attrData;
	PTR_ITEM			theItem;
	int					theVersion;
	bool				editable;

	bool findAttributeControl(
		TObject *control, size_t *catIdx, size_t *attrIdx, size_t *valueIdx
	);
	void createControl(
		TWinControl *tab, const AttrDefinition &def, AttributeValue *val, int numValues, int yPos
	);
	void checkAddDelButtons( size_t catIdx, size_t attrIdx );
	void __fastcall addButtonClick( TObject *Sender );
	void __fastcall delButtonClick( TObject *Sender );

	void buildCategoryUI( TWinControl *tab, CategoryData *data );

public:		// User declarations
	void saveCategory( const CategoryData &data, int itemId, int itemVersion );

	void setItem( const PTR_ITEM &theItem, int theVersion = 0 )
	{
		this->theItem = theItem;
		this->theVersion = theVersion;
	}
	__fastcall TAttributesForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TAttributesForm *AttributesForm;
//---------------------------------------------------------------------------
#endif
