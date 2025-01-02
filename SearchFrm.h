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

#ifndef SearchFrmH
#define SearchFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Db.hpp>
#include <DBGrids.hpp>
#include <DBTables.hpp>
#include <Grids.hpp>
#include <ComCtrls.hpp>

#include <gak/array.h>

#include "DocManDM.h"

//---------------------------------------------------------------------------
class TSearchForm : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TLabel *Label1;
	TEdit *EditKeyword;
	TButton *ButtonSearch;
	TQuery *QuerySearch;
	TDataSource *DataSourceSearch;
	TDBGrid *DBGrid;
	TIntegerField *QuerySearchID;
	TStringField *QuerySearchNAME;
	TIntegerField *QuerySearchParentID;
	TStringField *QuerySearchPATH;
	TButton *ButtonAdvanced;
	TPageControl *PageControlAdvanced;
	TTabSheet *TabSheetExif;
	TEdit *EditEditMaxFocalLength;
	TEdit *EditMinFocalLength;
	TLabel *Label5;
	TLabel *Label8;
	TEdit *EditMinIso;
	TEdit *EditMaxIso;
	TEdit *EditMaxTime;
	TEdit *EditMinTime;
	TLabel *Label7;
	TLabel *Label6;
	TEdit *EditMinFnumber;
	TEdit *EditMaxFnumber;
	TComboBox *ComboBoxLens;
	TLabel *Label2;
	TLabel *Label4;
	TComboBox *ComboBoxCamera;
	TComboBox *ComboBoxManufacturer;
	TLabel *Label3;
	TTable *TableCategories;
	TIntegerField *TableCategoriesID;
	TStringField *TableCategoriesNAME;
	TIntegerField *TableCategoriesMAX_ATTR_ID;
	TLabel *Label9;
	TRadioButton *RadioButtonAny;
	TRadioButton *RadioButtonCompany;
	TRadioButton *RadioButtonPrivate;
	TRadioButton *RadioButtonArchive;
	TLabel *Label10;
	TComboBox *ComboBoxExposureProgram;
	TComboBox *ComboBoxMeteringMode;
	TLabel *Label11;
	TLabel *Label12;
	TDateTimePicker *DateTimePickerMinDate;
	TDateTimePicker *DateTimePickerMinTime;
	TDateTimePicker *DateTimePickerMaxDate;
	TDateTimePicker *DateTimePickerMaxTime;
	TLabel *Label13;
	TComboBox *ComboBoxFlash;
	TButton *ButtonImages;
	void __fastcall ButtonSearchClick(TObject *Sender);
	void __fastcall QuerySearchCalcFields(TDataSet *DataSet);
	void __fastcall DBGridDblClick(TObject *Sender);
	void __fastcall ButtonAdvancedClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall ButtonImagesClick(TObject *Sender);
	void __fastcall QuerySearchAfterOpen(TDataSet *DataSet);
	void __fastcall QuerySearchBeforeClose(TDataSet *DataSet);
private:	// User declarations
	/* TODO 1 -ogak -cRefactory : Do we need this array? */
	TTabSheets		tabSheets;
	CategoryDatas	attrData;

	void createControl( TWinControl *tab, AttributeValue *val, int yPos );
	void buildCategoryUI( TWinControl *tab, CategoryData *data );
public:		// User declarations
	__fastcall TSearchForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSearchForm *SearchForm;
//---------------------------------------------------------------------------
#endif
