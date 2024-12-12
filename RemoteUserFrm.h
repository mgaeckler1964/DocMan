//---------------------------------------------------------------------------

#ifndef RemoteUserFrmH
#define RemoteUserFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBClient.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <DBCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
class DocManService;
//---------------------------------------------------------------------------
class TRemoteUserForm : public TForm
{
__published:	// IDE-managed Components
	TDBGrid *DBGridUserTable;
	TClientDataSet *ClientDataSetUserTable;
	TStringField *ClientDataSetUserTableVORNAME;
	TStringField *ClientDataSetUserTableNACHNAME;
	TStringField *ClientDataSetUserTableEMAIL;
	TDataSource *DataSourceUserTable;
	TPanel *Panel1;
	TDBNavigator *DBNavigator;
	TSpeedButton *SpeedButtonRefresh;
	TAutoIncField *ClientDataSetUserTableID;
	TStringField *ClientDataSetUserTableSTRASSE;
	TStringField *ClientDataSetUserTablePOSTFACH;
	TStringField *ClientDataSetUserTableLAND;
	TStringField *ClientDataSetUserTablePLZ;
	TStringField *ClientDataSetUserTableORT;
	TStringField *ClientDataSetUserTableGUEST;
	TStringField *ClientDataSetUserTableLOGINENABLED;
	TStringField *ClientDataSetUserTableADMINISTRATOR;
	TDBCheckBox *DBCheckBoxGuest;
	TDBCheckBox *DBCheckBoxLoginEnabled;
	TDBCheckBox *DBCheckBoxAdmin;
	TComboBox *ComboBoxRemoteServer;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ClientDataSetUserTableAfterOpen(TDataSet *DataSet);
	void __fastcall SpeedButtonRefreshClick(TObject *Sender);
	void __fastcall ClientDataSetUserTableBeforePost(TDataSet *DataSet);
	void __fastcall ClientDataSetUserTableBeforeDelete(TDataSet *DataSet);
	void __fastcall FormHide(TObject *Sender);
	void __fastcall ClientDataSetUserTableAfterInsert(TDataSet *DataSet);
	void __fastcall ComboBoxRemoteServerChange(TObject *Sender);
private:	// User declarations
	int	remoteServerID;
	void loadTable( void );
public:		// User declarations
	__fastcall TRemoteUserForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TRemoteUserForm *RemoteUserForm;
//---------------------------------------------------------------------------
#endif

