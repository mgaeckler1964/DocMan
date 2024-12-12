//---------------------------------------------------------------------------

#ifndef TaskFilesFrmH
#define TaskFilesFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include "ItemManager.h"
#include <Db.hpp>
#include <DBTables.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>

//---------------------------------------------------------------------------
class TTaskFilesForm : public TForm
{
__published:	// IDE-managed Components
	TQuery *OpenQuery;
	TIntegerField *OpenQueryID;
	TDataSource *OpenDataSource;
	TDBGrid *DBGridOpen;
	TDBGrid *DBGridSelected;
	TQuery *SelectedQuery;
	TIntegerField *SelectedQueryID;
	TDataSource *SelectedDataSource;
	TButton *ButtonAdd;
	TButton *ButtonRemove;
	TButton *ButtonClose;
	TQuery *AddQuery;
	TQuery *RemoveQuery;
	TButton *ButtonCheckIn;
	TLabel *Label1;
	TLabel *Label2;
	TCheckBox *CheckBoxBranch;
	TStringField *OpenQueryPATH;
	TStringField *SelectedQueryPATH;
	TIntegerField *SelectedQueryPARENTID;
	TStringField *SelectedQueryNAME;
	TIntegerField *OpenQueryPARENTID;
	TStringField *OpenQueryNAME;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall ButtonAddClick(TObject *Sender);
	void __fastcall ButtonRemoveClick(TObject *Sender);
	void __fastcall ButtonCheckInClick(TObject *Sender);
	void __fastcall SelectedQueryCalcFields(TDataSet *DataSet);
	void __fastcall OpenQueryCalcFields(TDataSet *DataSet);
private:	// User declarations
	PTR_ITEM	theItem;
	void refresh( void );
	static void gotoID( int ID, TQuery *query, TIntegerField *idField );
	static void gotoRecord( int recNO, TQuery *query );
public:		// User declarations
	void setItem( const PTR_ITEM &theItem )
	{
		this->theItem = theItem;
	}
	__fastcall TTaskFilesForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TTaskFilesForm *TaskFilesForm;
//---------------------------------------------------------------------------
#endif
