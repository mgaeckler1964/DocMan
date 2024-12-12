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

