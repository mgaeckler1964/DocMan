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

#include "DocManMain.h"
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
