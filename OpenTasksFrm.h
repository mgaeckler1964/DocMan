//---------------------------------------------------------------------------

#ifndef OpenTasksFrmH
#define OpenTasksFrmH
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
class TOpenTasksForm : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TDBNavigator *DBNavigator1;
	TDBGrid *DBGrid;
	TDataSource *DataSource;
	TQuery *QueryOpenTasks;
	TIntegerField *QueryOpenTasksID;
	TIntegerField *QueryOpenTasksParentID;
	TStringField *QueryOpenTasksName;
	TIntegerField *QueryOpenTasksAssigned_To;
	TStringField *QueryOpenTasksUsername;
	TStringField *QueryOpenTasksPATH;
	TSpeedButton *SpeedButton;
	TCheckBox *CheckBoxAllTasks;
	TStringField *QueryOpenTasksTask_Status;
	TIntegerField *QueryOpenTasksstatus;
	void __fastcall ReloadQuery(TObject *Sender);
	void __fastcall FormHide(TObject *Sender);
	void __fastcall QueryOpenTasksCalcFields(TDataSet *DataSet);
	void __fastcall DBGridDblClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TOpenTasksForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TOpenTasksForm *OpenTasksForm;
//---------------------------------------------------------------------------
#endif
