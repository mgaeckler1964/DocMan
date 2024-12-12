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

#include <winlib/winlib.h>

#pragma hdrstop

#include "SearchFrm.h"
#include "ActionManager.h"
#include "DocManDM.h"
#include "DocManMain.h"
#include "ImageFrm.h"

//---------------------------------------------------------------------------
using namespace gak;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSearchForm *SearchForm;
//---------------------------------------------------------------------------
#define VAL_UNKNOWN "<Unknown>"

#define MARGIN			8
#define CONTROL_SIZE	20
#define LABEL_WIDTH		128
#define	EDITOR_LEFT		(2*MARGIN+LABEL_WIDTH)

#define LINE_HEIGHT		24

#if 0
//---------------------------------------------------------------------------
class ACTION_SEARCH : public ACTION_BASE_PROPERTIES
{
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
	virtual const char *getLabel( void ) const;
};
//---------------------------------------------------------------------------
REFRESH_TYPE ACTION_SEARCH::perform( PTR_ITEM theItem )
{
//	GeneralForm->setItem( theItem );
	SearchForm->Show();
	SearchForm->BringToFront();
	SearchForm->SetFocus();
	return REFRESH_NONE;
}
//---------------------------------------------------------------------------
const char *ACTION_SEARCH::getLabel( void ) const
{
	return "Search...";
}
//---------------------------------------------------------------------------
static ACTION_SEARCH theAction;
#endif
//---------------------------------------------------------------------------
__fastcall TSearchForm::TSearchForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TSearchForm::ButtonSearchClick(TObject *)
{
	double	minFnumber = 0, maxFnumber = 0;
	long	minIso = -1, maxIso = -1;
	long	minFocalLength = 0, maxFocalLength = 0;
	double	minTime = 0, maxTime = 0;

	DBGrid->Visible = false;
	idleLoop();
	::SetCursor( LoadCursor( NULL, IDC_WAIT ) );

	QuerySearch->Close();
	QuerySearch->SQL->Clear();
	QuerySearch->SQL->Add(
		"select distinct it.ID, it.ParentID, it.NAME from item_tree it "
		"left outer join i_file_vers fv "
		"on it.FILE_ID = fv.FILE_ID "
		"left outer join i_exif_data ed "
		"on it.ID = ed.ITEM_ID "
		"left outer join c_attr_values cv "
		"on it.ID = cv.ITEM_ID "
		"left outer join c_attr_defs cd "
		"on cv.ATTR_ID = cd.ID "
		"where (it.name like '%' || :key || '%' "
		"or it.description like '%' || :key || '%' "
		"or it.module like '%' || :key || '%' "
		"or it.bad_function like '%' || :key || '%' "
		"or it.external_remarks like '%' || :key || '%' "
		"or it.internal_remarks like '%' || :key || '%' "
		"or fv.description like '%' || :key || '%' "
		"or ed.image_description like '%' || :key || '%' "
		"or ed.usercomment like '%' || :key || '%') "
	);
	QuerySearch->ParamByName("key")->AsString = EditKeyword->Text;

	PTR_ITEM volume;
	if( RadioButtonCompany->Checked )
		volume = getCompanyVolume();
	else if( RadioButtonArchive->Checked )
		volume = getArchiveVolume();
	else if( RadioButtonPrivate->Checked )
		volume = getPersonalItem( TYPE_PERSONAL_FOLDER );

	if( volume )
	{
		QuerySearch->SQL->Add( "and it.volume_id = :theVolumeID" );
		QuerySearch->ParamByName("theVolumeID")->AsInteger = (*volume).getID();
	}

	if( ComboBoxManufacturer->Text > "" )
	{
		if( ComboBoxManufacturer->Text == VAL_UNKNOWN )
		{
			QuerySearch->SQL->Add( "and ed.manufacturer is null " );
		}
		else
		{
			QuerySearch->SQL->Add( "and ed.manufacturer = :manufacturer " );
			QuerySearch->ParamByName("manufacturer")->AsString = ComboBoxManufacturer->Text;
		}
	}

	if( ComboBoxCamera->Text > "" )
	{
		if( ComboBoxCamera->Text == VAL_UNKNOWN )
		{
			QuerySearch->SQL->Add( "and ed.camera is null " );
		}
		else
		{
			QuerySearch->SQL->Add( "and ed.camera = :camera " );
			QuerySearch->ParamByName("camera")->AsString = ComboBoxCamera->Text;
		}
	}

	if( ComboBoxLens->Text > "" )
	{
		if( ComboBoxLens->Text == VAL_UNKNOWN )
		{
			QuerySearch->SQL->Add( "and ed.lens is null " );
		}
		else
		{
			QuerySearch->SQL->Add( "and ed.lens = :lens " );
			QuerySearch->ParamByName("lens")->AsString = ComboBoxLens->Text;
		}
	}

	if( ComboBoxExposureProgram->ItemIndex >= 0 )
	{
		if( !ComboBoxExposureProgram->ItemIndex )
		{
			QuerySearch->SQL->Add( "and ed.exposure_program is null " );
		}
		else
		{
			QuerySearch->SQL->Add( "and ed.exposure_program = :expProg " );
			QuerySearch->ParamByName("expProg")->AsInteger = ComboBoxExposureProgram->ItemIndex-1;
		}
	}

	if( ComboBoxMeteringMode->ItemIndex >= 0 )
	{
		if( !ComboBoxMeteringMode->ItemIndex )
		{
			QuerySearch->SQL->Add( "and ed.metering_mode is null " );
		}
		else
		{
			QuerySearch->SQL->Add( "and ed.metering_mode = :metMode " );
			QuerySearch->ParamByName("metMode")->AsInteger = ComboBoxMeteringMode->ItemIndex-1;
		}
	}

	if( !ComboBoxFlash->ItemIndex )
	{
		QuerySearch->SQL->Add( "and (ed.flash is null or ed.red_eye is null)" );
	}
	else if( ComboBoxFlash->ItemIndex == 1 )
	{
		QuerySearch->SQL->Add( "and ed.flash = 'F'" );
	}
	else if( ComboBoxFlash->ItemIndex == 2 )
	{
		QuerySearch->SQL->Add( "and ed.flash = 'T'" );
	}
	else if( ComboBoxFlash->ItemIndex == 3 )
	{
		QuerySearch->SQL->Add( "and ed.flash = 'T' and ed.red_eye = 'F'" );
	}
	else if( ComboBoxFlash->ItemIndex == 4 )
	{
		QuerySearch->SQL->Add( "and ed.flash = 'T' and ed.red_eye = 'T'" );
	}

	if( EditMinFnumber->Text > "" )
	{
		minFnumber = atof( EditMinFnumber->Text.c_str() );
	}
	if( EditMaxFnumber->Text > "" )
	{
		maxFnumber = atof( EditMaxFnumber->Text.c_str() );
	}
	if( minFnumber > 0 || maxFnumber > 0 )
	{
		if( minFnumber > 0 && maxFnumber > 0 )
		{
			QuerySearch->SQL->Add(
				"and ed.fnumber >= :minFnumber and ed.fnumber <= :maxFnumber "
			);
			QuerySearch->ParamByName("minFnumber")->AsFloat = minFnumber;
			QuerySearch->ParamByName("maxFnumber")->AsFloat = maxFnumber;
		}
		else if( minFnumber > 0 )
		{
			QuerySearch->SQL->Add(
				"and ed.fnumber >= :minFnumber "
			);
			QuerySearch->ParamByName("minFnumber")->AsFloat = minFnumber;
		}
		else
		{
			QuerySearch->SQL->Add(
				"and ed.fnumber <= :maxFnumber "
			);
			QuerySearch->ParamByName("maxFnumber")->AsFloat = maxFnumber;
		}
	}

	if( EditMinIso->Text > "" )
	{
		minIso = atol( EditMinIso->Text.c_str() );
	}
	if( EditMaxIso->Text > "" )
	{
		maxIso = atol( EditMaxIso->Text.c_str() );
	}
	if( minIso >= 0 || maxIso >= 0 )
	{
		if( minIso > 0 && maxIso > 0 )
		{
			QuerySearch->SQL->Add(
				"and ed.iso_speed >= :minIso and ed.iso_speed <= :maxIso "
			);
			QuerySearch->ParamByName("minIso")->AsInteger = minIso;
			QuerySearch->ParamByName("maxIso")->AsInteger = maxIso;
		}
		else if( minIso > 0 )
		{
			QuerySearch->SQL->Add( "and ed.iso_speed >= :minIso " );
			QuerySearch->ParamByName("minIso")->AsInteger = minIso;
		}
		else if( maxIso > 0 )
		{
			QuerySearch->SQL->Add( "and ed.iso_speed <= :maxIso " );
			QuerySearch->ParamByName("maxIso")->AsInteger = maxIso;
		}
		else
		{
			QuerySearch->SQL->Add( "and ed.iso_speed is null " );
		}
	}

	if( EditMinFocalLength->Text > "" )
	{
		minFocalLength = atol( EditMinFocalLength->Text.c_str() );
	}
	if( EditEditMaxFocalLength->Text > "" )
	{
		maxFocalLength = atol( EditEditMaxFocalLength->Text.c_str() );
	}
	if( minFocalLength > 0 || maxFocalLength > 0 )
	{
		if( minFocalLength > 0 && maxFocalLength > 0 )
		{
			QuerySearch->SQL->Add(
				"and ed.focal_length >= :minFocalLength and ed.focal_length <= :maxFocalLength "
			);
			QuerySearch->ParamByName("minFocalLength")->AsInteger = minFocalLength;
			QuerySearch->ParamByName("maxFocalLength")->AsInteger = maxFocalLength;
		}
		else if( minFocalLength > 0 )
		{
			QuerySearch->SQL->Add( "and ed.focal_length >= :minFocalLength " );
			QuerySearch->ParamByName("minFocalLength")->AsInteger = minFocalLength;
		}
		else
		{
			QuerySearch->SQL->Add( "and ed.focal_length <= :maxFocalLength " );
			QuerySearch->ParamByName("maxFocalLength")->AsInteger = maxFocalLength;
		}
	}

	if( EditMinTime->Text > "" )
	{
		minTime = atof( EditMinTime->Text.c_str() );
		if( !EditMinTime->Text.Pos( "." ) )
			minTime = 1/minTime;
	}
	if( EditMaxTime->Text > "" )
	{
		maxTime = atof( EditMaxTime->Text.c_str() );
		if( !EditMaxTime->Text.Pos( "." ) )
			maxTime = 1/maxTime;
	}
	if( minTime > 0 || maxTime > 0 )
	{
		if( minTime > 0 && maxTime > 0 )
		{
			QuerySearch->SQL->Add(
				"and ed.exposure_time >= :minTime and ed.exposure_time <= :maxTime "
			);
			QuerySearch->ParamByName("minTime")->AsFloat = minTime;
			QuerySearch->ParamByName("maxTime")->AsFloat = maxTime;
		}
		else if( minTime > 0 )
		{
			QuerySearch->SQL->Add( "and ed.exposure_time >= :minTime " );
			QuerySearch->ParamByName("minTime")->AsFloat = minTime;
		}
		else
		{
			QuerySearch->SQL->Add( "and ed.exposure_time <= :maxTime " );
			QuerySearch->ParamByName("maxTime")->AsFloat = maxTime;
		}
	}

	TDateTime	minDate = DateTimePickerMinDate->Date + DateTimePickerMinTime->Time;
	TDateTime	maxDate = DateTimePickerMaxDate->Date + DateTimePickerMaxTime->Time;
	if( (double)minDate > 0 || (double)maxDate > 0 )
	{
		if( (double)minDate > 0 && (double)maxDate > 0 )
		{
			QuerySearch->SQL->Add(
				"and ed.original_date_time >= :minDate and ed.original_date_time  <= :maxDate "
			);
			QuerySearch->ParamByName("minDate")->AsDateTime = minDate;
			QuerySearch->ParamByName("maxDate")->AsDateTime = maxDate;
		}
		else if( (double)minDate > 0 )
		{
			QuerySearch->SQL->Add( "and ed.original_date_time  >= :minDate " );
			QuerySearch->ParamByName("minDate")->AsDateTime = minDate;
		}
		else
		{
			QuerySearch->SQL->Add( "and ed.original_date_time  <= :maxDate " );
			QuerySearch->ParamByName("maxDate")->AsDateTime = maxDate;
		}
	}

	size_t	pIdx = 0;
	STRING	pName, whereClause;
	for(
		CategoryDatas::const_iterator it = attrData.cbegin(),
			endIT = attrData.cend();
		it != endIT;
		++it
	)
	{
		const CategoryData	&category = *it;
		for(
			AttrDefinitions::const_iterator it = category.attrDefinition.cbegin(),
				endIT = category.attrDefinition.cend();
			it != endIT;
			++it
		)
		{
			const AttrDefinition	&attrDef = *it;
			const AttributeValue	&attrValue = attrDef.values[0];
			AnsiString				value = ((TEdit*)(attrValue.editor))->Text;

			if( value > "" )
			{
				pIdx++;
				pName = "attrId";
				pName += formatNumber( pIdx );

				whereClause = "and cd.ATTR_ID = :";
				whereClause += pName;

				QuerySearch->SQL->Add( (const char *)whereClause );
				QuerySearch->ParamByName( (const char *)pName )->AsInteger = attrDef.attrID;

				pName = "attrValue";
				pName += formatNumber( pIdx );

				if( attrDef.type == ATTR_String )
					whereClause = "and cv.STRING_VALUE like :";
				else if( attrDef.type == ATTR_Number )
					whereClause = "and cv.FLOAT_VALUE = :";
				else if( attrDef.type == ATTR_Integer )
					whereClause = "and cv.INTEGER_VALUE = :";
				else if( attrDef.type == ATTR_Date || attrDef.type == ATTR_Time )
					whereClause = "and cv.DATE_VALUE = :";

				whereClause += pName;
				QuerySearch->SQL->Add( (const char *)whereClause );

				if( attrDef.type == ATTR_String )
					QuerySearch->ParamByName( (const char *)pName )->AsString = value;
				else if( attrDef.type == ATTR_Number )
					QuerySearch->ParamByName( (const char *)pName )->AsFloat = value.ToDouble();
				else if( attrDef.type == ATTR_Integer )
					QuerySearch->ParamByName( (const char *)pName )->AsInteger = value.ToInt();
				else if( attrDef.type == ATTR_Date )
				{
					TDateTime	dateVal( value, TDateTime::Date );
					QuerySearch->ParamByName( (const char *)pName )->AsDateTime = dateVal;
				}
				else if( attrDef.type == ATTR_Time )
				{
					TDateTime	dateVal( value, TDateTime::Time );
					QuerySearch->ParamByName( (const char *)pName )->AsDateTime = dateVal;
				}
			}
		}
	}
	QuerySearch->Open();

	DBGrid->Visible = true;
	::SetCursor( LoadCursor( NULL, IDC_ARROW ) );
	idleLoop();
}
//---------------------------------------------------------------------------
void __fastcall TSearchForm::QuerySearchCalcFields(TDataSet *)
{
	int		parentID = QuerySearchParentID->AsInteger;
	if( parentID > 0 )
	{
		STRING	path = DocManDataModule->getPathForID( parentID );
		QuerySearchPATH->AsString = (const char *)path;
	}
	else
		QuerySearchPATH->AsString = "/";

}
//---------------------------------------------------------------------------
void __fastcall TSearchForm::DBGridDblClick(TObject *)
{
	PTR_ITEM	theItem = getItem( QuerySearchParentID->AsInteger );
	if( theItem )
	{
		DocManMainForm->openItem( theItem, QuerySearchID->AsInteger );
		DocManMainForm->BringToFront();
		DocManMainForm->SetFocus();
	}
}
//---------------------------------------------------------------------------
void TSearchForm::createControl( TWinControl *tab, AttributeValue *val, int yPos )
{
	doEnterFunction("TSearchForm::createControl");

	TAnchors 		editAnchors;
	TEdit			*edit = new TEdit( tab );

	editAnchors << akTop << akLeft << akRight;

	edit->Parent = tab;
	edit->Left = EDITOR_LEFT;
	edit->Top = yPos;
	edit->Height = CONTROL_SIZE;
	edit->Width = tab->ClientWidth - MARGIN - EDITOR_LEFT;
	edit->Anchors = editAnchors;

	val->editor = edit;
}
//---------------------------------------------------------------------------
void TSearchForm::buildCategoryUI( TWinControl *tab, CategoryData *data )
{
	doEnterFunction("TSearchForm::buildCategoryUI");

	TLabel			*label;
	int				yPos = MARGIN;

	for(
		AttrDefinitions::iterator it = data->attrDefinition.begin(),
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
		label->Caption = (const char *)(def.name + ":");
		def.label = label;

		AttributeValue	&val = def.values.createElement();
		createControl( tab, &val, yPos );
		yPos += LINE_HEIGHT;
	}
}
//---------------------------------------------------------------------------
void __fastcall TSearchForm::ButtonAdvancedClick(TObject *)
{
	if( PageControlAdvanced->Visible )
		PageControlAdvanced->Visible = false;
	else
	{
		::SetCursor( LoadCursor( NULL, IDC_WAIT ) );

		std::auto_ptr<TQuery>	fillSql( new TQuery( NULL ) );
		fillSql->DatabaseName = "docManDB";

		if( !ComboBoxManufacturer->Items->Count )
		{
			fillSql->SQL->Clear();
			fillSql->SQL->Add(
				"select distinct manufacturer from i_exif_data where manufacturer is not null order by 1"
			);
			ComboBoxManufacturer->Items->Add( VAL_UNKNOWN );
			for( fillSql->Open(); !fillSql->Eof; fillSql->Next() )
				ComboBoxManufacturer->Items->Add( fillSql->Fields->Fields[0]->AsString );
			fillSql->Close();
		}
		if( !ComboBoxCamera->Items->Count )
		{
			fillSql->SQL->Clear();
			fillSql->SQL->Add(
				"select distinct camera from i_exif_data where camera is not null order by 1"
			);
			ComboBoxCamera->Items->Add( VAL_UNKNOWN );
			for( fillSql->Open(); !fillSql->Eof; fillSql->Next() )
				ComboBoxCamera->Items->Add( fillSql->Fields->Fields[0]->AsString );
			fillSql->Close();
		}
		if( !ComboBoxLens->Items->Count )
		{
			fillSql->SQL->Clear();
			fillSql->SQL->Add(
				"select distinct lens from i_exif_data where lens is not null order by 1"
			);
			ComboBoxLens->Items->Add( VAL_UNKNOWN );
			for( fillSql->Open(); !fillSql->Eof; fillSql->Next() )
				ComboBoxLens->Items->Add( fillSql->Fields->Fields[0]->AsString );
			fillSql->Close();
		}

		if( PageControlAdvanced->PageCount <= 1 )
		{
			//for( size_t i=0; i<tabSheets.getNumElements(); i++ )
				//delete tabSheets[i];
			//tabSheets.clear();

			for(
				TableCategories->Open();
				!TableCategories->Eof;
				TableCategories->Next()
			)
			{
				TTabSheet	*newSheet = new TTabSheet( this );
				newSheet->Parent = PageControlAdvanced;
				newSheet->PageControl = PageControlAdvanced;
				newSheet->Caption = TableCategoriesNAME->AsString;
				tabSheets.addElement( newSheet );

				TScrollBox	*scrollBox = new TScrollBox( newSheet );
				scrollBox->Parent = newSheet;
				scrollBox->Align = alClient;

				CategoryData	data;
				DocManDataModule->loadCategoryDefinition(
					TableCategoriesNAME->AsString.c_str(),
					&data
				);

				buildCategoryUI( scrollBox, &data );
				attrData.addElement( data );
			}
			TableCategories->Close();
		}

		PageControlAdvanced->Visible = true;
		::SetCursor( LoadCursor( NULL, IDC_ARROW ) );
	}
}
//---------------------------------------------------------------------------



void __fastcall TSearchForm::FormCreate(TObject *)
{
	::SetWindowLong( Handle, GWL_HWNDPARENT, NULL );

	DateTimePickerMinDate->ShowCheckbox = true;
	DateTimePickerMinTime->ShowCheckbox = true;
	DateTimePickerMaxDate->ShowCheckbox = true;
	DateTimePickerMaxTime->ShowCheckbox = true;

	DateTimePickerMinDate->Date = 0;
	DateTimePickerMinTime->Date = 0;
	DateTimePickerMaxDate->Date = 0;
	DateTimePickerMaxTime->Date = 0;

	DateTimePickerMinDate->Time = 0;
	DateTimePickerMinTime->Time = 0;
	DateTimePickerMaxDate->Time = 0;
	DateTimePickerMaxTime->Time = 0;

	DateTimePickerMinDate->ShowCheckbox = false;
	DateTimePickerMinTime->ShowCheckbox = false;
	DateTimePickerMaxDate->ShowCheckbox = false;
	DateTimePickerMaxTime->ShowCheckbox = false;
}
//---------------------------------------------------------------------------


void __fastcall TSearchForm::ButtonImagesClick(TObject *)
{
	if( QuerySearch->Active )
	{
		size_t numImages = 0;
		Application->CreateForm(__classid(TImageBrowserForm), &ImageBrowserForm);

		for(
			QuerySearch->First();
			!QuerySearch->Eof;
			QuerySearch->Next()
		)
		{
			PTR_ITEM theItem = getItem( QuerySearchID->AsInteger );
			PTR_FILE theFile = theItem;
			if( theFile )
			{
				if( ImageBrowserForm->addImage( theFile ) )
					numImages++;
			}
			else
			{
				PTR_FILE_REF theFileRef = theItem;
				if( theFileRef )
				{
					if( ImageBrowserForm->addImage(
						theFileRef->getID(), theFileRef->getDownloadPath()
					))
					{
						numImages++;
					}
				}
			}
		}

		if( numImages )
		{
			ImageBrowserForm->Caption = "Search Result";
			ImageBrowserForm->Show();
			ImageBrowserForm->SetFocus();
			ImageBrowserForm->BringToFront();
		}
		else
		{
			Application->MessageBox(
				"No Images found",
				"Image Browser",
				MB_OK|MB_ICONSTOP
			);
			delete ImageBrowserForm;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TSearchForm::QuerySearchAfterOpen(TDataSet *)
{
	ButtonImages->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TSearchForm::QuerySearchBeforeClose(TDataSet *)
{
	ButtonImages->Enabled = false;
}
//---------------------------------------------------------------------------

