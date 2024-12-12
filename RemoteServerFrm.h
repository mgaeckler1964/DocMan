//---------------------------------------------------------------------------

#ifndef RemoteServerFrmH
#define RemoteServerFrmH
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

#include <gak/array.h>
#include <gak/ci_string.h>

#include "gaklib.h"

//---------------------------------------------------------------------------
class DocManService;
struct RemoteServer
{
	int				ID;
	gak::CI_STRING	name;
	STRING			url;
	DocManService	*theService;
/*
	const CI_STRING &getKey( void ) const
	{
		return name;
	}
	void setKey( const CI_STRING &key )
	{
		name = key;
	}
*/
};
typedef gak::Array<RemoteServer>	RemoteServers;

//---------------------------------------------------------------------------
class TRemoteServerForm : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TDBNavigator *DBNavigatorRemoteServer;
	TDBGrid *DBGridRemoteServer;
	TDataSource *DataSourceRemoteServer;
	TTable *TableRemoteServer;
	TStringField *TableRemoteServerREMOTE_SERVER_NAME;
	TStringField *TableRemoteServerREMOTE_SERVER_URL;
	TIntegerField *TableRemoteServerID;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall TableRemoteServerAfterInsert(TDataSet *DataSet);
private:	// User declarations
	RemoteServers	serverList;

	void clearServerList( void );
	void fillServerList( void );
public:		// User declarations
	void FillCombobox( TComboBox *theBox, int selectID=0 );
	int GetServerId( int idx )
	{
		return serverList[idx].ID;
	}
	DocManService *GetServiceByID( int serverID );
	bool GetServiceConnectedByID( int serverID );
	STRING GetServerURLbyID( int serverID );
	__fastcall TRemoteServerForm(TComponent* Owner);
	virtual __fastcall ~TRemoteServerForm();
};
//---------------------------------------------------------------------------
extern PACKAGE TRemoteServerForm *RemoteServerForm;
//---------------------------------------------------------------------------
#endif

