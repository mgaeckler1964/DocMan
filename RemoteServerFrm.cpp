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

#include <vcl.h>
#include <vcl/registry.hpp>
#pragma hdrstop

#include "RemoteServerFrm.h"
#include "DocManService.h"
#include "Config.h"
#include "LoginDlg.h"
#include "DocManDM.h"

//---------------------------------------------------------------------------
using namespace gak;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TRemoteServerForm *RemoteServerForm;
//---------------------------------------------------------------------------
__fastcall TRemoteServerForm::TRemoteServerForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
__fastcall TRemoteServerForm::~TRemoteServerForm()
{
	clearServerList();
}
//---------------------------------------------------------------------------
void TRemoteServerForm::clearServerList( void )
{
	for(
		RemoteServers::iterator it = serverList.begin(),
			endIT = serverList.end();
		it != endIT;
		++it
	)
	{
		RemoteServer	&theServer = *it;
		if( theServer.theService )
		{
			delete theServer.theService;
			theServer.theService = NULL;
		}
	}
	serverList.clear();
}
//---------------------------------------------------------------------------
void TRemoteServerForm::fillServerList( void )
{
	clearServerList();

	for(
		TableRemoteServer->First();
		!TableRemoteServer->Eof;
		TableRemoteServer->Next()
	)
	{
		RemoteServer	&theServer = serverList.createElement();
		theServer.ID = TableRemoteServerID->AsInteger;
		theServer.name = TableRemoteServerREMOTE_SERVER_NAME->AsString.c_str();
		theServer.url = TableRemoteServerREMOTE_SERVER_URL->AsString.c_str();
		theServer.theService = NULL;
	}
}
//---------------------------------------------------------------------------
void __fastcall TRemoteServerForm::FormShow(TObject *)
{
	TableRemoteServer->Open();
}
//---------------------------------------------------------------------------
void __fastcall TRemoteServerForm::FormClose(TObject *,
	  TCloseAction &)
{
	fillServerList();
	TableRemoteServer->Close();
}
//---------------------------------------------------------------------------
void __fastcall TRemoteServerForm::FormCreate(TObject *)
{
	TableRemoteServer->Open();
	fillServerList();
	TableRemoteServer->Close();
}
//---------------------------------------------------------------------------


void __fastcall TRemoteServerForm::TableRemoteServerAfterInsert(
	  TDataSet *)
{
	TableRemoteServerID->AsInteger = ConfigDataModule->getNewMaxValue(
		"REMOTE_SERVER", "ID"
	);
}
//---------------------------------------------------------------------------
void TRemoteServerForm::FillCombobox( TComboBox *theBox, int serverID )
{
	int	i = 0,
		newSelection = 0;

	theBox->Items->Clear();

	for(
		RemoteServers::const_iterator it = serverList.cbegin(),
			endIT = serverList.cend();
		it != endIT;
		++it, ++i
	)
	{
		const RemoteServer	&theServer = *it;
		theBox->Items->Add( static_cast<const char *>(theServer.name) );
		if( theServer.ID == serverID )
		{
			newSelection = i;
		}
	}
	theBox->ItemIndex = newSelection;
}
//---------------------------------------------------------------------------
DocManService *TRemoteServerForm::GetServiceByID( int serverID )
{
	DocManService	*result = NULL;
	for(
		RemoteServers::iterator it = serverList.begin(),
			endIT = serverList.end();
		it != endIT;
		++it
	)
	{
		RemoteServer	&theServer = *it;
		if( theServer.ID == serverID )
		{
			if( !theServer.theService )
			{
				STRING	wsdURL = theServer.url + "service.php?wsdl";
				result = new DocManService( wsdURL );
				theServer.theService = result;
			}
			else
			{
				result = theServer.theService;
			}

			if( result->getNeedCredentials() )
			{
				std::auto_ptr<TRegistry>	registry( new TRegistry );

				AnsiString serverName = AnsiString((const char*)theServer.name);
				AnsiString userNameValue = serverName + "UserName";
				AnsiString passwordValue = serverName + "Password";
				AnsiString serverKey = registryKey;
				serverKey += "\\RemoteServers";

				if( registry->OpenKey( serverKey, true ) )
				{
					if( registry->ValueExists( userNameValue ) )
						LoginForm->EditUserName->Text = registry->ReadString(
							userNameValue
						);
					else
						LoginForm->EditUserName->Text = "";
					if( registry->ValueExists( passwordValue ) )
						LoginForm->EditPassword->Text = registry->ReadString(
							passwordValue
						);
					else
						LoginForm->EditPassword->Text = "";
				}


				do
				{
					if( LoginForm->ShowModal( serverName.c_str() ) == mrOk )
					{
						AnsiString	userName = LoginForm->EditUserName->Text;
						AnsiString	password = LoginForm->EditPassword->Text;

						registry->WriteString( userNameValue, userName );
						registry->WriteString( passwordValue, password );
						try
						{
							result->setCredentials(
								userName.c_str(),
								password.c_str()
							);
						}
						catch( net::NoAuthorisationError & )
						{
							// ignore
						}
					}
					else
/*v*/					break;
				}
				while( result->getNeedCredentials() );
			}
/*v*/		break;
		}
	}
	return result;
}
//---------------------------------------------------------------------------
bool TRemoteServerForm::GetServiceConnectedByID( int serverID )
{
	DocManService	*service = NULL;
	bool			connected = false;
	for(
		RemoteServers::const_iterator it = serverList.cbegin(),
			endIT = serverList.cend();
		it != endIT;
		++it
	)
	{
		const RemoteServer	&theServer = *it;
		if( theServer.ID == serverID )
		{
			service = theServer.theService;
			break;
		}
	}

	if( service && service->getHttpStatusCode() == 200 )
	{
		connected = true;
	}

	return connected;
}
//---------------------------------------------------------------------------
STRING TRemoteServerForm::GetServerURLbyID( int serverID )
{
	STRING	url;

	for(
		RemoteServers::const_iterator it = serverList.cbegin(),
			endIT = serverList.cend();
		it != endIT;
		++it
	)
	{
		const RemoteServer	&theServer = *it;
		if( theServer.ID == serverID )
		{
			url = theServer.url;
/*v*/		break;
		}
	}

	return url;
}
//---------------------------------------------------------------------------

