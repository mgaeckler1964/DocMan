//---------------------------------------------------------------------------

#ifndef LocalDirsFrmH
#define LocalDirsFrmH
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
class TLocalDirsForm : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TDBNavigator *DBNavigator1;
	TDBGrid *DBGrid1;
	TDataSource *DataSource;
	TQuery *QueryLocalDirs;
	TIntegerField *QueryLocalDirsFOLDER_ID;
	TStringField *QueryLocalDirsNAME;
	TStringField *QueryLocalDirsUSERNAME;
	TStringField *QueryLocalDirsPATH;
	TSpeedButton *SpeedButton;
	TCheckBox *CheckBoxAllDirectories;
	TStringField *QueryLocalDirsMACHINE;
	TMemoField *QueryLocalDirsLOCAL_PATH;
	TSpeedButton *SpeedButtonDelete;
	TIntegerField *QueryLocalDirsUSER_ID;
	TIntegerField *QueryLocalDirsID;
	TStringField *QueryLocalDirsSHORT_PATH;
	void __fastcall ReloadQuery(TObject *Sender);
	void __fastcall FormHide(TObject *Sender);
	void __fastcall QueryLocalDirsCalcFields(TDataSet *DataSet);
	void __fastcall SpeedButtonDeleteClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
	void reloadQuery( void );
public:		// User declarations
	__fastcall TLocalDirsForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TLocalDirsForm *LocalDirsForm;
//---------------------------------------------------------------------------
#endif
