//---------------------------------------------------------------------------

#ifndef OpenFilesFrmH
#define OpenFilesFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DBCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Db.hpp>
#include <DBGrids.hpp>
#include <DBTables.hpp>
#include <Grids.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TOpenFilesForm : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TDBNavigator *DBNavigator1;
	TDBGrid *DBGrid;
	TDataSource *DataSource;
	TQuery *QueryOpenFiles;
	TIntegerField *QueryOpenFilesID;
	TIntegerField *QueryOpenFilesParentID;
	TStringField *QueryOpenFilesName;
	TIntegerField *QueryOpenFilesReservedBy;
	TStringField *QueryOpenFilesReservedOn;
	TStringField *QueryOpenFilesUsername;
	TStringField *QueryOpenFilesPATH;
	TSpeedButton *SpeedButton;
	TCheckBox *CheckBoxAllFiles;
	TIntegerField *QueryOpenFilesReservedFor;
	TStringField *QueryOpenFilesTASK_PATH;
	void __fastcall ReloadQuery(TObject *Sender);
	void __fastcall FormHide(TObject *Sender);
	void __fastcall QueryOpenFilesCalcFields(TDataSet *DataSet);
	void __fastcall DBGridDblClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
	void reloadQuery( void );
public:		// User declarations
	__fastcall TOpenFilesForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TOpenFilesForm *OpenFilesForm;
//---------------------------------------------------------------------------
#endif
