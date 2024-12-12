//---------------------------------------------------------------------------

#ifndef TaskTypesFrmH
#define TaskTypesFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBCtrls.hpp>
#include <DBGrids.hpp>
#include <DBTables.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
class TTaskTypesForm : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TDBNavigator *DBNavigator;
	TDBGrid *DBGrid;
	TDataSource *DataSource;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
	__fastcall TTaskTypesForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TTaskTypesForm *TaskTypesForm;
//---------------------------------------------------------------------------
#endif
