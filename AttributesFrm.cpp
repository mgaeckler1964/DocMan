/*
		Project:		DocMan
		Module:			
		Description:	
		Author:			Martin Gäckler
		Address:		Hofmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2024 Martin Gäckler

		This program is free software: you can redistribute it and/or modify  
		it under the terms of the GNU General Public License as published by  
		the Free Software Foundation, version 3.

		You should have received a copy of the GNU General Public License 
		along with this program. If not, see <http://www.gnu.org/licenses/>.

		THIS SOFTWARE IS PROVIDED BY Martin Gäckler, Austria, Linz ``AS IS''
		AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
		TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
		PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR
		CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
		SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
		LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
		USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
		ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
		OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
		OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
		SUCH DAMAGE.
*/

//---------------------------------------------------------------------------

#include <vcl.h>
#include <gak/vcl_tools.h>

#pragma hdrstop

#include "ActionManager.h"
#include "AttributesFrm.h"
#include "File.h"
#include "Folder.h"
#include "Config.h"
#include "DocManDM.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#define MARGIN			8
#define CONTROL_SIZE	20
#define LABEL_WIDTH		128
#define	EDITOR_LEFT		(2*MARGIN+LABEL_WIDTH)
#define DEL_LEFT		(CONTROL_SIZE+MARGIN)
#define ADD_LEFT		(DEL_LEFT+CONTROL_SIZE+MARGIN)

#define LINE_HEIGHT		24

TAttributesForm *AttributesForm;
//---------------------------------------------------------------------------
class ACTION_ATTRIBUTES : public ACTION_BASE_PROPERTIES
{
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
	virtual const char *getLabel( void ) const;
};
//---------------------------------------------------------------------------
#pragma option -RT-
class THREAD_CAT_REMOVE_APPLY_DOWN : public ThreadDocMan
{
	int	categoryID;

	void removeCategory( TQuery *testSQL, TQuery *rmSQL, PTR_ITEM &theItemToHandle );

	virtual const char *getTitle( void ) const;
	virtual void perform( void );

	public:
	THREAD_CAT_REMOVE_APPLY_DOWN(
		const PTR_ITEM	&theItemToHandle,
		int				categoryID
	)
	: ThreadDocMan( theItemToHandle )
	{
		this->categoryID = categoryID;
	}
};

class THREAD_CAT_ADD_APPLY_DOWN : public ThreadDocMan
{
	const CategoryData &category;

	void addCategory( TQuery *testSQL, PTR_ITEM &theItemToHandle );

	virtual const char *getTitle( void ) const;
	virtual void perform( void );

	public:
	THREAD_CAT_ADD_APPLY_DOWN(
		const PTR_ITEM	&theItemToHandle,
		const CategoryData 	&nCategory
	)
	: ThreadDocMan( theItemToHandle ), category( nCategory )
	{
	}
};
#pragma option -RT.
//---------------------------------------------------------------------------
const char *THREAD_CAT_REMOVE_APPLY_DOWN::getTitle( void ) const
{
	return "Remove Category";
}
//---------------------------------------------------------------------------
const char *THREAD_CAT_ADD_APPLY_DOWN::getTitle( void ) const
{
	return "Add Category";
}
//---------------------------------------------------------------------------
void THREAD_CAT_REMOVE_APPLY_DOWN::removeCategory( TQuery *testSQL, TQuery *rmSQL, PTR_ITEM &theItemToHandle )
{
	if( StatusForm->setStatus( "Removing Category from", (*theItemToHandle).getName() ) )
		return;

	int			itemId = (*theItemToHandle).getID();
	int			version = 0;
	PTR_FILE	theFile = theItemToHandle;
	if( theFile )
		version = theFile->getVersionNum();

	testSQL->Params->Items[0]->AsInteger = itemId;
	testSQL->Params->Items[1]->AsInteger = version;
	testSQL->Params->Items[2]->AsInteger = categoryID;
	testSQL->Open();
	if( testSQL->Fields->Fields[0]->AsInteger )
	{
		if( theFile )
			version = theFile->createVersion( version );

		rmSQL->Params->Items[0]->AsInteger = itemId;
		rmSQL->Params->Items[1]->AsInteger = version;
		rmSQL->Params->Items[2]->AsInteger = categoryID;

		rmSQL->ExecSQL();
	}
	testSQL->Close();

	theItemToHandle->loadContent();

	for(
		int i=0;
		i<theItemToHandle->getChildCount() && !StatusForm->isTerminated();
		++i
	)
	{
		PTR_ITEM	child = theItemToHandle->getContentItem( i );
		removeCategory( testSQL, rmSQL, child );
	}
}
//---------------------------------------------------------------------------
void THREAD_CAT_ADD_APPLY_DOWN::addCategory( TQuery *testSQL, PTR_ITEM &theItemToHandle )
{
	doEnterFunction("THREAD_CAT_ADD_APPLY_DOWN::addCategory");

	if( StatusForm->setStatus( "Adding Category to", (*theItemToHandle).getName() ) )
	{
/*@*/	return;
	}

	int			itemId = (*theItemToHandle).getID();
	int			version = 0;
	PTR_FILE	theFile = theItemToHandle;
	if( theFile )
		version = theFile->getVersionNum();

	testSQL->Params->Items[0]->AsInteger = itemId;
	testSQL->Params->Items[1]->AsInteger = version;
	testSQL->Params->Items[2]->AsInteger = category.catID;
	testSQL->Open();
	if( !testSQL->Fields->Fields[0]->AsInteger )
	{
#if 0
		/* TODO 1 -ogak -cDB : Add create version (should copy EXIF data not reread) */
		if( theFile )
			version = theFile->createVersion( version );
#endif
		AttributesForm->saveCategory( category, itemId, version );
	}
	testSQL->Close();

	theItemToHandle->loadContent();

	for(
		int i=0;
		i<theItemToHandle->getChildCount() && !StatusForm->isTerminated();
		i++
	)
	{
		PTR_ITEM	child = theItemToHandle->getContentItem( i );
		addCategory( testSQL, child );
	}
}
//---------------------------------------------------------------------------
void THREAD_CAT_REMOVE_APPLY_DOWN::perform( void )
{
	std::auto_ptr<TQuery>	testSQL( new TQuery( NULL ) );
	std::auto_ptr<TQuery>	rmSQL( new TQuery( NULL ) );

	testSQL->DatabaseName = "docManDb";
	rmSQL->DatabaseName = "docManDb";

	testSQL->SQL->Add(
		"select count(*) from c_attr_values "
		"where item_id = :theID "
		"and item_version = :theVersion "
		"and attr_id in "
		"( "
			"select ID from c_attr_defs "
			"where category_id = :theCategory "
		")"
	);
	rmSQL->SQL->Add(
		"delete from c_attr_values "
		"where item_id = :theID "
		"and item_version = :theVersion "
		"and attr_id in "
		"( "
			"select ID from c_attr_defs "
			"where category_id = :theCategory "
		")"
	);

	removeCategory( testSQL.get(), rmSQL.get(), theItemToHandle );
}
//---------------------------------------------------------------------------
void THREAD_CAT_ADD_APPLY_DOWN::perform( void )
{
	std::auto_ptr<TQuery>	testSQL( new TQuery( NULL ) );

	testSQL->DatabaseName = "docManDb";

	testSQL->SQL->Add(
		"select count(*) "
		"from c_attr_values v, c_attr_defs d "
		"where v.item_id = :theID "
		"and v.item_version = :theVersion "
		"and v.attr_id = d.ID "
		"and d.category_id = :theCategory "
	);

	addCategory( testSQL.get(), theItemToHandle );
}
//---------------------------------------------------------------------------
REFRESH_TYPE ACTION_ATTRIBUTES::perform( PTR_ITEM theItem )
{
	AttributesForm->setItem( theItem );
	if( AttributesForm->ShowModal() == mrOk )
		return REFRESH_RELOAD;
	else
		return REFRESH_NONE;
}
//---------------------------------------------------------------------------
const char *ACTION_ATTRIBUTES::getLabel( void ) const
{
	return "Attributes...";
}
//---------------------------------------------------------------------------
static ACTION_ATTRIBUTES theAction;
//---------------------------------------------------------------------------
__fastcall TAttributesForm::TAttributesForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TAttributesForm::FormShow(TObject *)
{
	PTR_FOLDER	theFolder = theItem;
	ButtonApplyDown->Visible = theFolder ? true : false;

	attrData.clear();

	for(
		TTabSheets::const_iterator it = tabSheets.cbegin(),
			endIT = tabSheets.cend();
			it != endIT;
		++it
	)
	{
		delete *it;
	}
	tabSheets.clear();

	selectedCategories.clear();
	ComboBoxCategories->Items->Clear();
	for(
		TableCategories->Open();
		!TableCategories->Eof;
		TableCategories->Next()
	)
	{
		ComboBoxCategories->Items->Add( TableCategoriesNAME->AsString );
	}
	TableCategories->Close();

	if( !theVersion )
	{
		PTR_FILE	theFile = theItem;

		if( (THE_FILE*)theFile )
			theVersion = (*theFile).getVersionNum();

		editable = true;
	}
	else
		editable = false;

	ButtonSave->Enabled = editable;
	ButtonAdd->Enabled = editable;
	ButtonRemove->Enabled = editable;
	ButtonUpgrade->Enabled = editable;

	QueryCategoryForItem->Params->Items[0]->AsInteger = (*theItem).getID();
	QueryCategoryForItem->Params->Items[1]->AsInteger = theVersion;

	for(
		QueryCategoryForItem->Open();
		!QueryCategoryForItem->Eof;
		QueryCategoryForItem->Next()
	)
	{
		TTabSheet	*newSheet = new TTabSheet( this );
		newSheet->Parent = PageControl;
		newSheet->PageControl = PageControl;
		newSheet->Caption = QueryCategoryForItemNAME->AsString;
		selectedCategories.addElement( QueryCategoryForItemNAME->AsString.c_str() );
		tabSheets.addElement( newSheet );

		TScrollBox	*scrollBox = new TScrollBox( newSheet );
		scrollBox->Parent = newSheet;
		scrollBox->Align = alClient;

		CategoryData	data;
		DocManDataModule->loadCategoryDefinition(
			QueryCategoryForItemID->AsInteger, QueryCategoryForItemCATEGORY_VERSION->AsInteger,
			&data,
			theItem->getID(), theVersion
		);

		buildCategoryUI( scrollBox, &data );
		attrData.addElement( data );
	}
	QueryCategoryForItem->Close();


	ComboBoxCategories->ItemIndex = 0;
	ComboBoxCategoriesChange( ComboBoxCategories );

	gak::vcl::bringWindowToFront( this );
}
//---------------------------------------------------------------------------
bool TAttributesForm::findAttributeControl(
	TObject *control, size_t *catIdx, size_t *attrIdx, size_t *valueIdx
)
{
	doEnterFunction("TAttributesForm::findAttributeControl");

	for( size_t cIdx = 0; cIdx < attrData.size(); cIdx++ )
	{
		const CategoryData &data = attrData[cIdx];

		for( size_t aIdx=0; aIdx<data.attrDefinition.size(); aIdx++ )
		{
			const AttrDefinition &def = data.attrDefinition[aIdx];
			for( size_t vIdx = 0; vIdx < def.values.size(); vIdx++ )
			{
				const AttributeValue &val = def.values[vIdx];
				if( val.editor == control
				||  val.addButton == control
				||  val.delButton == control )
				{
					*catIdx = cIdx;
					*attrIdx = aIdx;
					*valueIdx = vIdx;

/***/				return true;
				}
			}
		}
	}

	return false;
}
//---------------------------------------------------------------------------
void TAttributesForm::createControl(
	TWinControl *tab, const AttrDefinition &def, AttributeValue *val, int numValues, int yPos
)
{
	doEnterFunction("TAttributesForm::createControl");

	TEdit			*edit;
	TDateTimePicker	*dateTime;
	TButton			*button;
	int				minCount, maxCount;

	minCount = def.minCount;
	maxCount = def.maxCount;

	if( def.type == ATTR_String
	|| def.type == ATTR_Integer
	|| def.type == ATTR_Number )
	{
		edit = new TEdit( tab );
		if( val->value.isDefined() )
		{
			edit->Text = static_cast<const char *>(STRING(val->value));
		}

		val->editor = edit;
	}
	else if( def.type == ATTR_Date )
	{
		dateTime = new TDateTimePicker( tab );

		dateTime->Kind = dtkDate;
		dateTime->DateMode = dmComboBox;

		if( val->value.isDefined() )
		{
			gak::Date	tmp1 = val->value;
			TDateTime	tmp2 = tmp1;
			dateTime->Date = tmp2;
		}

		val->editor = dateTime;
	}
	else if( def.type == ATTR_Time )
	{
		dateTime = new TDateTimePicker( tab );

		dateTime->Kind = dtkTime;

		if( val->value.isDefined() )
		{
			gak::Time	tmp1 = val->value;
			TDateTime	tmp2 = tmp1;
			dateTime->Time = tmp2;
		}

		val->editor = dateTime;
	}

	TAnchors		editAnchors, buttonAnchors;

	editAnchors << akTop << akLeft << akRight;
	buttonAnchors << akTop << akRight;

	val->editor->Parent = tab;
	val->editor->Left = EDITOR_LEFT;
	val->editor->Top = yPos;
	val->editor->Height = CONTROL_SIZE;
	val->editor->Width = tab->ClientWidth - ADD_LEFT - MARGIN - EDITOR_LEFT;
	val->editor->Anchors = editAnchors;
	val->editor->Enabled = editable;


	button = new TButton( tab );
	button->Parent = tab;
	button->Left = tab->ClientWidth-ADD_LEFT;
	button->Top = yPos;
	button->Width = CONTROL_SIZE;
	button->Height = CONTROL_SIZE;
	button->Caption = "+";
	button->Visible = ( numValues <  maxCount );
	button->OnClick = addButtonClick;
	button->Anchors = buttonAnchors;
	button->Enabled = editable;
	val->addButton = button;

	button = new TButton( tab );
	button->Parent = tab;
	button->Left = tab->ClientWidth-DEL_LEFT;
	button->Top = yPos;
	button->Width = CONTROL_SIZE;
	button->Height = CONTROL_SIZE;
	button->Caption = "-";
	button->Visible = ( numValues >  minCount );
	button->OnClick = delButtonClick;
	button->Anchors = buttonAnchors;
	button->Enabled = editable;
	val->delButton = button;
}
//---------------------------------------------------------------------------
void TAttributesForm::checkAddDelButtons( size_t catIdx, size_t attrIdx )
{
	const CategoryData		&data = attrData[catIdx];
	const AttrDefinition	&def = data.attrDefinition[attrIdx];
	std::size_t				numValues = def.values.size();

	size_t minCount = def.minCount;
	size_t maxCount = def.maxCount;
	for( std::size_t vIdx = 0; vIdx < numValues; vIdx++ )
	{
		const AttributeValue	&val = def.values[vIdx];

		val.addButton->Visible = numValues < maxCount;
		val.delButton->Visible = numValues > minCount;
	}
}
//---------------------------------------------------------------------------
void __fastcall TAttributesForm::addButtonClick( TObject *Sender )
{
	doEnterFunction("TAttributesForm::addButtonClick");

	size_t	catIdx, attrIdx, valueIdx;

	if( findAttributeControl( Sender, &catIdx, &attrIdx, &valueIdx ) )
	{
		int			yPos;
		size_t 		numValues;

		CategoryData			&data = attrData[catIdx];
		AttrDefinition			&def = data.attrDefinition[attrIdx];
		const AttributeValue	&val = def.values[valueIdx];
		int						newPos = val.editor->Top;

		numValues = def.values.size();
		for( size_t vIdx = valueIdx; vIdx < numValues; vIdx++ )
		{
			const AttributeValue	&val = def.values[vIdx];
			yPos = val.editor->Top + LINE_HEIGHT;
			val.editor->Top = yPos;
			val.addButton->Top = yPos;
			val.delButton->Top = yPos;
		}

		for( size_t aIdx = attrIdx+1; aIdx < data.attrDefinition.size(); aIdx++ )
		{
			const AttrDefinition	&def = data.attrDefinition[aIdx];
			def.label->Top += LINE_HEIGHT;

			numValues = def.values.size();
			for( size_t vIdx = 0; vIdx < numValues; vIdx++ )
			{
				const AttributeValue	&val = def.values[vIdx];
				yPos = val.editor->Top + LINE_HEIGHT;
				val.editor->Top = yPos;
				val.addButton->Top = yPos;
				val.delButton->Top = yPos;
			}
		}

		AttributeValue	newValue;
		createControl(
			((TWinControl*)Sender)->Parent,
			def, &newValue, def.values.size()+1, newPos
		);
		def.values.insertElement( newValue, valueIdx );

		checkAddDelButtons( catIdx, attrIdx );
	}
}
//---------------------------------------------------------------------------
void __fastcall TAttributesForm::delButtonClick( TObject *Sender )
{
	doEnterFunction("TAttributesForm::delButtonClick");

	size_t	catIdx, attrIdx, valueIdx;

	if( findAttributeControl( Sender, &catIdx, &attrIdx, &valueIdx ) )
	{
		int			yPos;
		size_t 		numValues;

		CategoryData			&data = attrData[catIdx];
		AttrDefinition			&def = data.attrDefinition[attrIdx];
		const AttributeValue	&val = def.values[valueIdx];

		delete val.editor;
		delete val.addButton;
		val.delButton->Visible = false;

		numValues = def.values.size();
		for( size_t vIdx = valueIdx+1; vIdx < numValues; vIdx++ )
		{
			const AttributeValue	&val = def.values[vIdx];
			yPos = val.editor->Top - LINE_HEIGHT;
			val.editor->Top = yPos;
			val.addButton->Top = yPos;
			val.delButton->Top = yPos;
		}

		for( size_t aIdx = attrIdx+1; aIdx < data.attrDefinition.size(); aIdx++ )
		{
			const AttrDefinition	&def = data.attrDefinition[aIdx];
			def.label->Top -= LINE_HEIGHT;

			numValues = def.values.size();
			for( size_t vIdx = 0; vIdx < numValues; vIdx++ )
			{
				const AttributeValue	&val = def.values[vIdx];
				yPos = val.editor->Top - LINE_HEIGHT;
				val.editor->Top = yPos;
				val.addButton->Top = yPos;
				val.delButton->Top = yPos;
			}
		}

		def.values.removeElementAt( valueIdx );

		checkAddDelButtons( catIdx, attrIdx );
	}
}
//---------------------------------------------------------------------------
void TAttributesForm::buildCategoryUI( TWinControl *tab, CategoryData *data )
{
	doEnterFunction("TAttributesForm::buildCategoryUI");

	TLabel			*label;
	size_t			numValues, valueIdx;
	int				yPos = MARGIN;

	for( AttrDefinitions::iterator it = data->attrDefinition.begin(),
		endIT = data->attrDefinition.end();
		it != endIT;
		++it
	)
	{
		AttrDefinition &def = *it;
		label = new TLabel( tab );
		label->Parent = tab;
		label->Left = MARGIN;
		label->Top = yPos;
		label->Width = LABEL_WIDTH;
		label->Height = 20;
		label->Caption = (const char *)(def.name + (def.required ? " *:" : ":"));
		def.label = label;

		numValues = def.values.size();
		if( def.minCount > numValues )
		{
			numValues = def.minCount;
		}

		for( valueIdx = 0; valueIdx < numValues; valueIdx++ )
		{
			AttributeValue	&val = def.values[valueIdx];
			createControl( tab, def, &val, numValues, yPos );
			yPos += LINE_HEIGHT;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TAttributesForm::ButtonAddClick(TObject *)
{
	doEnterFunction("TAttributesForm::ButtonAddClick");

	STRING	selectedCategory = ComboBoxCategories->Items->Strings[
		ComboBoxCategories->ItemIndex
	].c_str();

	if( !selectedCategories.hasElement( selectedCategory ) )
	{
		TTabSheet	*newSheet = new TTabSheet( this );
		newSheet->Parent = PageControl;
		newSheet->PageControl = PageControl;
		newSheet->Caption = (const char *)selectedCategory;
		selectedCategories.addElement( selectedCategory );
		tabSheets.addElement( newSheet );

		TScrollBox	*scrollBox = new TScrollBox( newSheet );
		scrollBox->Parent = newSheet;
		scrollBox->Align = alClient;

		CategoryData	data;
		DocManDataModule->loadCategoryDefinition(
			selectedCategory, &data
		);

		buildCategoryUI( scrollBox, &data );
		attrData.addElement( data );

		PageControl->ActivePage = newSheet;

		ButtonAdd->Enabled = false;
		ButtonRemove->Enabled = editable;
	}
}
//---------------------------------------------------------------------------

void __fastcall TAttributesForm::ButtonRemoveClick(TObject *)
{
	STRING	selectedCategory = ComboBoxCategories->Items->Strings[
		ComboBoxCategories->ItemIndex
	].c_str();
	size_t	catIdx = selectedCategories.findElement( selectedCategory );
	if( catIdx != (size_t)-1 )
	{
		delete tabSheets[catIdx];
		tabSheets.removeElementAt( catIdx );
		attrData.removeElementAt( catIdx );
		selectedCategories.removeElementAt( catIdx );

		ButtonAdd->Enabled = editable;
		ButtonRemove->Enabled = false;
	}
}
//---------------------------------------------------------------------------

void __fastcall TAttributesForm::ComboBoxCategoriesChange(TObject *)
{
	STRING	selectedCategory = ComboBoxCategories->Items->Strings[
		ComboBoxCategories->ItemIndex
	].c_str();

	size_t	cIdx = selectedCategories.findElement( selectedCategory );
	if( cIdx == (size_t)-1 )
	{
		ButtonAdd->Enabled = editable;
		ButtonRemove->Enabled = false;
		ButtonUpgrade->Enabled = false;
	}
	else
	{
		ButtonAdd->Enabled = false;
		ButtonRemove->Enabled = editable;

		CategoryData &data = attrData[cIdx];

		QueryCatVersion->Params->Items[0]->AsInteger = data.catID;
		QueryCatVersion->Open();
		if( !QueryCatVersion->Eof
		&& QueryCatVersion->Fields->Fields[0]->AsInteger > data.version )
			ButtonUpgrade->Enabled = editable;
		else
			ButtonUpgrade->Enabled = false;
		QueryCatVersion->Close();
	}
}
//---------------------------------------------------------------------------
void TAttributesForm::saveCategory(
	const CategoryData &data, int itemId, int itemVersion
)
{
	doEnterFunction("TAttributesForm::saveCategory");

	TParam	*itemIdParam = insertQuery->ParamByName( "ITEM_ID" );
	TParam	*itemVersionParam = insertQuery->ParamByName( "ITEM_VERSION" );

	itemIdParam->AsInteger = itemId;
	itemVersionParam->AsInteger = itemVersion;

	TParam	*attrIdParam = insertQuery->ParamByName( "ATTR_ID" );
	TParam	*valueIdParam = insertQuery->ParamByName( "ID" );
	TParam	*orderParam = insertQuery->ParamByName( "ORDER_NR" );
	TParam	*stringParam = insertQuery->ParamByName( "STRING_VALUE" );

	TParam	*dateParam = insertQuery->ParamByName( "DATE_VALUE" );
//	dateParam->ParamType = ptInput;
//	dateParam->DataType = ftDateTime;

	TParam	*floatParam = insertQuery->ParamByName( "FLOAT_VALUE" );
//	floatParam->ParamType = ptInput;
//	floatParam->DataType = ftFloat;

	TParam	*intParam = insertQuery->ParamByName( "INTEGER_VALUE" );
//	intParam->ParamType = ptInput;
//	intParam->DataType = ftInteger;


	for(
		AttrDefinitions::const_iterator it = data.attrDefinition.cbegin(),
			endIT = data.attrDefinition.cend();
		it != endIT;
		++it
	)
	{
		const AttrDefinition &def = *it;

		attrIdParam->AsInteger = def.ID;

		int	vIdx = 0;
		for(
			AttributeValues::const_iterator it = def.values.cbegin(),
				endIT = def.values.cend();
			it != endIT;
			++it, ++vIdx
		)
		{
			const AttributeValue &val = *it;

			valueIdParam->AsInteger = ConfigDataModule->getNewMaxValue(
				"C_ATTR_VALUES", "ID"
			);
			orderParam->AsInteger = vIdx;

			if( def.type == ATTR_String )
			{
				stringParam->AsString = ((TEdit*)val.editor)->Text;
				dateParam->Clear();
				dateParam->Bound = true;
				intParam->Clear();
				intParam->Bound = true;
				floatParam->Clear();
				floatParam->Bound = true;
			}
			else if( def.type == ATTR_Date )
			{
				stringParam->AsString = "";
				dateParam->AsDateTime = floor( ((TDateTimePicker*)val.editor)->Date );
				intParam->Clear();
				intParam->Bound = true;
				floatParam->Clear();
				floatParam->Bound = true;
			}
			else if( def.type == ATTR_Time )
			{
				stringParam->AsString = "";
				dateParam->AsDateTime = ((TDateTimePicker*)val.editor)->Time - floor( ((TDateTimePicker*)val.editor)->Time );
				intParam->Clear();
				intParam->Bound = true;
				floatParam->Clear();
				floatParam->Bound = true;
			}
			else if( def.type == ATTR_Number )
			{
				AnsiString	strValue = ((TEdit*)val.editor)->Text;
				stringParam->AsString = "";
				dateParam->Clear();
				dateParam->Bound = true;
				intParam->Clear();
				intParam->Bound = true;
				if( strValue > "" )
					floatParam->AsFloat = strValue.ToDouble();
				else
				{
					floatParam->Clear();
					floatParam->Bound = true;
				}
			}
			else if( def.type == ATTR_Integer )
			{
				AnsiString	strValue = ((TEdit*)val.editor)->Text;

				stringParam->AsString = "";
				dateParam->Clear();
				dateParam->Bound = true;

				if( strValue > "" )
					intParam->AsInteger = strValue.ToInt();
				else
				{
					intParam->Clear();
					intParam->Bound = true;
				}

				floatParam->Clear();
				floatParam->Bound = true;
			}

			doEnterFunction("insertQuery->ExecSQL");
			insertQuery->ExecSQL();
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TAttributesForm::ButtonSaveClick(TObject *)
{
	for( size_t cIdx = 0; cIdx < attrData.size(); cIdx++ )
	{
		const CategoryData &data = attrData[cIdx];

		for(
			AttrDefinitions::const_iterator it = data.attrDefinition.cbegin(),
				endIT = data.attrDefinition.cend();
			it != endIT;
			++it
		)
		{
			const AttrDefinition &def = *it;
			if( (def.required && def.type == ATTR_String)
			|| def.type == ATTR_Number
			|| def.type == ATTR_Integer )
			{
				for(
					AttributeValues::const_iterator it = def.values.cbegin(),
						endIT = def.values.cend();
					it != endIT;
					++it
				)
				{
					const AttributeValue	&val = *it;
					AnsiString				strValue = ((TEdit*)val.editor)->Text;

					if( def.required && strValue == "" )
					{
						PageControl->ActivePage = tabSheets[cIdx];
						val.editor->SetFocus();
/*@*/					throw Exception( "Value Required" );
					}
					else if( strValue > "" )
					{
						try
						{
							if( def.type == ATTR_Number )
								strValue.ToDouble();
							else if( def.type == ATTR_Integer )
								strValue.ToInt();
						}
						catch( ... )
						{
							PageControl->ActivePage = tabSheets[cIdx];
							val.editor->SetFocus();
/*@*/						throw;
						}
					}
				}
			}
		}
	}

	delQuery->Params->Items[0]->AsInteger = theItem->getID();
	delQuery->Params->Items[1]->AsInteger = theVersion;

	delQuery->ExecSQL();

	for(
		CategoryDatas::const_iterator it = attrData.cbegin(),
			endIT = attrData.cend();
		it != endIT;
		++it
	)
	{
		const CategoryData &data = *it;

		saveCategory( data, theItem->getID(), theVersion );
	}

	ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TAttributesForm::PageControlChange(TObject *)
{
	TTabSheet	*activePage = PageControl->ActivePage;
	AnsiString	selectedCategory = activePage->Caption;
	int			categoryIndex = ComboBoxCategories->Items->IndexOf(
		selectedCategory
	);

	ComboBoxCategories->ItemIndex = categoryIndex;
	ComboBoxCategoriesChange( ComboBoxCategories );
}
//---------------------------------------------------------------------------

void __fastcall TAttributesForm::ButtonUpgradeClick(TObject *)
{
	STRING	selectedCategory = ComboBoxCategories->Items->Strings[
		ComboBoxCategories->ItemIndex
	].c_str();
	size_t	cIdx = selectedCategories.findElement( selectedCategory );
	if( cIdx != std::size_t(-1) )
	{
		CategoryData	&oldData = attrData[cIdx];
		CategoryData	newData;

		DocManDataModule->loadCategoryDefinition(
			selectedCategory, &newData
		);

		for( size_t aIdx = 0; aIdx<oldData.attrDefinition.size(); aIdx++ )
		{
			const AttrDefinition	&oldAttr = oldData.attrDefinition[aIdx];
			for( size_t anIdx = 0; anIdx<newData.attrDefinition.size(); anIdx++ )
			{
				AttrDefinition	&newAttr = newData.attrDefinition[anIdx];
				if( oldAttr.attrID == newAttr.attrID )
				{
					if( oldAttr.type == newAttr.type )
					{
						for( size_t vIdx=0; vIdx<oldAttr.values.size(); vIdx++ )
						{
							const AttributeValue &oldValue = oldAttr.values[vIdx];
							AttributeValue &newValue = newAttr.values[vIdx];
							if( oldAttr.type == ATTR_String )
							{
								newValue.value = ((TEdit*)oldValue.editor)->Text.c_str();
							}
							else if( oldAttr.type == ATTR_Integer )
							{
								newValue.value = (long)(((TEdit*)oldValue.editor)->Text.ToInt());
							}
							else if( oldAttr.type == ATTR_Number )
							{
								newValue.value = ((TEdit*)oldValue.editor)->Text.ToDouble();
							}
							else if( oldAttr.type == ATTR_Date )
							{
								TDateTime	tmp1 = ((TDateTimePicker*)oldValue.editor)->Date;
								gak::Date	tmp2 = tmp1;
								newValue.value = tmp2;
							}
							else if( oldAttr.type == ATTR_Time )
							{
								TDateTime	tmp1 = ((TDateTimePicker*)oldValue.editor)->Time;
								gak::Time	tmp2 = tmp1;
								newValue.value = tmp2;
							}
						}
					}
					break;
				}
			}
		}

		for( size_t aIdx = 0; aIdx<oldData.attrDefinition.size(); aIdx++ )
		{
			const AttrDefinition	&oldAttr = oldData.attrDefinition[aIdx];

			delete oldAttr.label;

			for( size_t vIdx=0; vIdx<oldAttr.values.size(); vIdx++ )
			{
				const AttributeValue &oldValue = oldAttr.values[vIdx];

				delete oldValue.editor;
				delete oldValue.addButton;
				delete oldValue.delButton;
			}
		}

		oldData = newData;
		TTabSheet	*tabSheet = tabSheets[cIdx];
		TWinControl	*tab = (TWinControl*)tabSheet->Controls[0];
		buildCategoryUI( tab, &oldData );
	}

	ButtonUpgrade->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TAttributesForm::ButtonApplyDownClick(TObject *)
{
	STRING	selectedCategory = ComboBoxCategories->Items->Strings[
		ComboBoxCategories->ItemIndex
	].c_str();
	size_t	cIdx = selectedCategories.findElement( selectedCategory );
	if( cIdx == (size_t)-1 )
	{
		int categoryID, categoryVersion;
		DocManDataModule->getCategoryID( selectedCategory, &categoryID, &categoryVersion );

		THREAD_CAT_REMOVE_APPLY_DOWN	theThread( theItem, categoryID );
		theThread.StartThread();
	}
	else
	{
		CategoryData				&category = attrData[cIdx];
		THREAD_CAT_ADD_APPLY_DOWN	theThread( theItem, category );

		theThread.StartThread();
	}
}
//---------------------------------------------------------------------------

