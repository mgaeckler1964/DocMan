//---------------------------------------------------------------------------

#include <vcl.h>

#include <gak/vcl_tools.h>

#pragma hdrstop

#include "CategoriesFrm.h"
#include "Config.h"
#include "DocManMain.h"
//---------------------------------------------------------------------------
using namespace gak;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TCategoriesForm *CategoriesForm;
//---------------------------------------------------------------------------
__fastcall TCategoriesForm::TCategoriesForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TCategoriesForm::FormShow(TObject *)
{
	clrChanged();
	TableCategories->Open();

	vcl::bringWindowToFront( this );
}
//---------------------------------------------------------------------------
void __fastcall TCategoriesForm::FormClose(TObject *,
	  TCloseAction &)
{
	checkChanged();
	TableCategories->Close();
}
//---------------------------------------------------------------------------

void __fastcall TCategoriesForm::TableCategoriesAfterInsert(
	  TDataSet *)
{
	vcl::setNewMaxValue( TableCategories, TableCategoriesID );
	TableCategoriesMAX_ATTR_ID->AsInteger = maxAttrId = 0;
}
//---------------------------------------------------------------------------

void __fastcall TCategoriesForm::TableCategoriesAfterScroll(
	  TDataSet *)
{
	maxVersion = 0;
	maxAttrId = TableCategoriesMAX_ATTR_ID->AsInteger;

	QueryCatVersions->Params->Items[0]->AsInteger = TableCategoriesID->AsInteger;
	ComboBoxCatVersion->Items->Clear();
	for(
		QueryCatVersions->Open();
		!QueryCatVersions->Eof;
		QueryCatVersions->Next()
	)
	{
		ComboBoxCatVersion->Items->Add(
			QueryCatVersionsCATEGORY_VERSION->AsString
		);

		maxVersion = max(
			maxVersion,
			QueryCatVersionsCATEGORY_VERSION->AsInteger
		);
	}
	QueryCatVersions->Close();

	if( maxVersion )
	{
		ComboBoxCatVersion->ItemIndex = 0;
		ComboBoxCatVersionChange( NULL );
	}
	else
	{
		attrDefinition.attrDefinition.clear();
		refreshGrid();
		clrChanged();
	}

}
//---------------------------------------------------------------------------

void TCategoriesForm::checkChanged( void )
{
	if( changed )
	{
		if( Application->MessageBox(
			"Definition changed. Do you want to save?",
			"DocMan",
			MB_YESNO|MB_ICONQUESTION
		) == IDYES )
		{
			ButtonSaveClick( NULL );
		}
	}
}
//---------------------------------------------------------------------------

void TCategoriesForm::refreshGrid( void )
{
	int 	i=0;
	size_t	numAttributes = attrDefinition.attrDefinition.size();
	StringGridAttributes->RowCount = numAttributes;
	for(
		AttrDefinitions::const_iterator it = attrDefinition.attrDefinition.cbegin(),
			endIT = attrDefinition.attrDefinition.cend();
		it != endIT;
		++it, ++i
	)
	{
		const AttrDefinition	&attrDef = *it;
		StringGridAttributes->Cells[0][i] = static_cast<const char *>(
			attrDef.name
		);
	}
	if( !numAttributes )
	{
		StringGridAttributes->Cells[0][0] = "<EMPTY>";
		ComboBoxType->Enabled = false;
		EditMinCount->Enabled = false;
		EditMaxCount->Enabled = false;
		CheckBoxRequired->Enabled = false;
		ButtonUp->Enabled = false;
		ButtonDown->Enabled = false;
		ButtonRemove->Enabled = false;
	}
	else
	{
		ButtonRemove->Enabled = true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TCategoriesForm::ComboBoxCatVersionChange(TObject *)
{
	checkChanged();

	DocManDataModule->loadCategoryDefinition(
		TableCategoriesID->AsInteger, atoi( ComboBoxCatVersion->Text.c_str() ),
		&attrDefinition
	);
	refreshGrid();

	if( StringGridAttributes->RowCount )
	{
		StringGridAttributes->Row = 0;
		bool dummy;
		StringGridAttributesSelectCell( NULL, 0, StringGridAttributes->Row, dummy );
	}
	clrChanged();
}

//---------------------------------------------------------------------------

void __fastcall TCategoriesForm::StringGridAttributesSelectCell(
	  TObject *, int , int ARow, bool &CanSelect)
{
	if( std::size_t(ARow) < attrDefinition.attrDefinition.size() )
	{
		STRING					tmpVal;
		const AttrDefinition	&attrDef = attrDefinition.attrDefinition[ARow];

		ComboBoxType->ItemIndex = attrDef.type;
		tmpVal = formatNumber( attrDef.minCount );
		EditMinCount->OnChange = NULL;
		EditMinCount->Text = (const char *)tmpVal;
		EditMinCount->OnChange = EditMinCountChange;

		tmpVal = formatNumber( attrDef.maxCount );
		EditMaxCount->OnChange = NULL;
		EditMaxCount->Text = (const char *)tmpVal;
		EditMaxCount->OnChange = EditMaxCountChange;

		CheckBoxRequired->OnClick = NULL;
		CheckBoxRequired->Checked = attrDef.required;
		CheckBoxRequired->OnClick = CheckBoxRequiredClick;

		ComboBoxType->Enabled = true;
		EditMinCount->Enabled = true;
		EditMaxCount->Enabled = true;
		CheckBoxRequired->Enabled = true;

		if( ARow > 0 )
			ButtonUp->Enabled = true;
		else
			ButtonUp->Enabled = false;

		if( std::size_t(ARow) < attrDefinition.attrDefinition.size()-1 )
			ButtonDown->Enabled = true;
		else
			ButtonDown->Enabled = false;

		CanSelect = true;
	}
	else
	{
		CanSelect = false;
	}
}

//---------------------------------------------------------------------------


void __fastcall TCategoriesForm::StringGridAttributesSetEditText(
	  TObject *, int , int ARow, const AnsiString Value)
{
	if( std::size_t(ARow) >= attrDefinition.attrDefinition.size() )
	{
		ButtonNewClick( NULL );
	}

	AttrDefinition	&attrDef = attrDefinition.attrDefinition[ARow];

	if( attrDef.name != Value.c_str() )
	{
		attrDef.name = Value.c_str();
		setChanged();
		StringGridAttributes->Row = ARow;
	}
}
//---------------------------------------------------------------------------

void __fastcall TCategoriesForm::ComboBoxTypeChange(TObject *)
{
	AttrDefinition	&attrDef = attrDefinition.attrDefinition[StringGridAttributes->Row];

	if( attrDef.type != ComboBoxType->ItemIndex )
	{
		attrDef.type = AttributeType(ComboBoxType->ItemIndex);
		setChanged();
	}
}
//---------------------------------------------------------------------------

void __fastcall TCategoriesForm::EditMinCountChange(TObject *)
{
	AttrDefinition	&attrDef = attrDefinition.attrDefinition[StringGridAttributes->Row];
	std::size_t		minCount = EditMinCount->Text.ToInt();

	if( minCount > 0 && attrDef.minCount != minCount )
	{
		attrDef.minCount = minCount;
		setChanged();
	}
}
//---------------------------------------------------------------------------

void __fastcall TCategoriesForm::EditMaxCountChange(TObject *)
{
	AttrDefinition	&attrDef = attrDefinition.attrDefinition[StringGridAttributes->Row];
	std::size_t		maxCount = EditMaxCount->Text.ToInt();

	if( maxCount > 0 && attrDef.maxCount != maxCount )
	{
		attrDef.maxCount = maxCount;
		setChanged();
	}
}
//---------------------------------------------------------------------------

void __fastcall TCategoriesForm::CheckBoxRequiredClick(TObject *)
{
	AttrDefinition	&attrDef = attrDefinition.attrDefinition[StringGridAttributes->Row];

	if( attrDef.required != CheckBoxRequired->Checked )
	{
		attrDef.required = CheckBoxRequired->Checked;
		setChanged();
	}
}
//---------------------------------------------------------------------------

void __fastcall TCategoriesForm::ButtonUpClick(TObject *)
{
	int	selected = StringGridAttributes->Row;
	if( selected > 0 )
	{
		AttrDefinition	tmp = attrDefinition.attrDefinition[selected];
		attrDefinition.attrDefinition[selected] = attrDefinition.attrDefinition[selected-1];
		attrDefinition.attrDefinition[selected-1] = tmp;
		refreshGrid();
		StringGridAttributes->Row = selected -1;

		setChanged();
	}
}
//---------------------------------------------------------------------------

void __fastcall TCategoriesForm::ButtonDownClick(TObject *)
{
	int	selected = StringGridAttributes->Row;
	if( selected < StringGridAttributes->RowCount-1 )
	{
		AttrDefinition	tmp = attrDefinition.attrDefinition[selected];
		attrDefinition.attrDefinition[selected] = attrDefinition.attrDefinition[selected+1];
		attrDefinition.attrDefinition[selected+1] = tmp;
		refreshGrid();
		StringGridAttributes->Row = selected +1;

		setChanged();
	}
}
//---------------------------------------------------------------------------

void __fastcall TCategoriesForm::ButtonNewClick(TObject *Sender )
{
	AttrDefinition attrDef;

	int newIndex = int(attrDefinition.attrDefinition.size());

	StringGridAttributes->RowCount = newIndex+1;

	if( Sender == ButtonNew )
	{
		attrDef.name = "Attr ";
		attrDef.name += formatNumber( newIndex+1 );
		StringGridAttributes->Cells[0][newIndex] = (const char *)attrDef.name;
	}

	attrDef.type = ATTR_String;
	attrDef.attrID = ++maxAttrId;
	attrDef.minCount = 1;
	attrDef.maxCount = 1;
	attrDef.required = false;

	attrDefinition.attrDefinition.addElement( attrDef );

	StringGridAttributes->Row = newIndex;

	setChanged();

	bool dummy;
	StringGridAttributesSelectCell( NULL, 0, StringGridAttributes->Row, dummy );
}

//---------------------------------------------------------------------------
void __fastcall TCategoriesForm::ButtonRemoveClick(TObject *)
{
	int	selected = StringGridAttributes->Row;
	if( std::size_t(selected) < attrDefinition.attrDefinition.size() )
	{
		attrDefinition.attrDefinition.removeElementAt(selected);

		refreshGrid();

		if( selected >= StringGridAttributes->RowCount )
		{
			selected = StringGridAttributes->RowCount-1;
		}
		if( selected >= 0 )
		{
			StringGridAttributes->Row = selected;
			bool dummy;
			StringGridAttributesSelectCell( NULL, 0, StringGridAttributes->Row, dummy );
		}

		setChanged();
	}
}
//---------------------------------------------------------------------------

void __fastcall TCategoriesForm::ButtonSaveClick(TObject *)
{
	if( changed )
	{
		int	newID = ConfigDataModule->getNewMaxValue(
			"C_ATTR_DEFS", "ID"
		);

		maxVersion++;

		int i=0;
		for(
			AttrDefinitions::const_iterator it = attrDefinition.attrDefinition.cbegin(),
				endIT = attrDefinition.attrDefinition.cend();
			it != endIT;
			++it, ++i
		)
		{
			const AttrDefinition	&attrDef = *it;

			QuerySaveAttributes->ParamByName( "ID" )->AsInteger = newID++;
			QuerySaveAttributes->ParamByName( "CATEGORY_ID" )->AsInteger = TableCategoriesID->AsInteger;
			QuerySaveAttributes->ParamByName( "CATEGORY_VERSION" )->AsInteger = maxVersion;
			QuerySaveAttributes->ParamByName( "ATTR_ID" )->AsInteger = attrDef.attrID;
			QuerySaveAttributes->ParamByName( "NAME" )->AsString = (const char *)attrDef.name;
			QuerySaveAttributes->ParamByName( "TYPE" )->AsInteger = attrDef.type;
			QuerySaveAttributes->ParamByName( "ORDER_NR" )->AsInteger = i;
			QuerySaveAttributes->ParamByName( "REQUIRED" )->AsInteger = attrDef.required;
			QuerySaveAttributes->ParamByName( "MIN_COUNT" )->AsInteger = attrDef.minCount;
			QuerySaveAttributes->ParamByName( "MAX_COUNT" )->AsInteger = attrDef.maxCount;

			QuerySaveAttributes->ExecSQL();
		}

		TableCategories->Edit();
		TableCategoriesMAX_ATTR_ID->AsInteger = maxAttrId;
		TableCategories->Post();
		
		STRING tmpVal = formatNumber( maxVersion );
		ComboBoxCatVersion->Items->Insert( 0, static_cast<const char *>(tmpVal) );
		ComboBoxCatVersion->ItemIndex = 0;

		clrChanged();
	}
}
//---------------------------------------------------------------------------

void __fastcall TCategoriesForm::TableCategoriesBeforeScroll(
	  TDataSet *)
{
	checkChanged();
}
//---------------------------------------------------------------------------

void __fastcall TCategoriesForm::TableCategoriesBeforeDelete(
	  TDataSet *)
{
	QueryDeleteAttributes->Params->Items[0]->AsInteger = TableCategoriesID->AsInteger;
	QueryDeleteAttributes->ExecSQL();
}
//---------------------------------------------------------------------------

void __fastcall TCategoriesForm::ButtonPurgeVersionsClick(TObject *)
{
	QueryPurgeAttributes->Params->Items[0]->AsInteger = TableCategoriesID->AsInteger;
	QueryPurgeAttributes->Params->Items[1]->AsInteger = atoi(
		ComboBoxCatVersion->Text.c_str()
	);
	QueryPurgeAttributes->ExecSQL();
	TableCategoriesAfterScroll( TableCategories );
}
//---------------------------------------------------------------------------
#if 0
 /* TODO 5 -ogak -cDB : Sometime we need an upgrade of categories */
	QueryCountUpgradeItems->Params->Items[0]->AsInteger = maxVersion;
	QueryCountUpgradeItems->Params->Items[1]->AsInteger = TableCategoriesID->AsInteger;
	QueryCountUpgradeItems->Params->Items[2]->AsInteger = maxVersion;
	QueryCountUpgradeItems->Params->Items[3]->AsInteger = TableCategoriesID->AsInteger;

	QueryCountUpgradeItems->Open();
	int counter = QueryCountUpgradeItemsCOUNTER->AsInteger;
	if( !counter )
		Application->MessageBox( "Nothing to upgrade", "Doc Manager", MB_ICONSTOP );
	else
	{
		STRING	message = "Do you want to upgrade ";
		counter >> message;
		message += " items?";
		Application->MessageBox( message, "Doc Manager", MB_ICONQUESTION|MB_YESNO );
	}
#endif
//---------------------------------------------------------------------------

