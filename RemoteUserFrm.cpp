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
#pragma hdrstop

#include "RemoteUserFrm.h"
#include "DocManService.h"
#include "RemoteServerFrm.h"

//---------------------------------------------------------------------------
using namespace gak;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TRemoteUserForm *RemoteUserForm;
//---------------------------------------------------------------------------
__fastcall TRemoteUserForm::TRemoteUserForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void TRemoteUserForm::loadTable( void )
{
	ClientDataSetUserTableID->ReadOnly = false;
	ClientDataSetUserTable->Tag = 1;
	ClientDataSetUserTable->EmptyDataSet();

	try
	{
		DocManService *myService = RemoteServerForm->GetServiceByID(
			remoteServerID
		);
		xml::Element *result = myService->sGetUserTable();
		if( result )
		{
			Array<DocManService::UserRecord_t> itemArray;
			myService->fillUserRecord( itemArray, result );

			for(
				Array<DocManService::UserRecord_t>::const_iterator it = itemArray.cbegin(),
					endIT = itemArray.cend();
				it != endIT;
				++it
			)
			{
				const DocManService::UserRecord_t &theUser = *it;

				ClientDataSetUserTable->Append();
				ClientDataSetUserTableID->AsInteger = theUser.id;
				ClientDataSetUserTableVORNAME->AsString = (const char *)theUser.vorname;
				ClientDataSetUserTableNACHNAME->AsString = (const char *)theUser.nachname;
				ClientDataSetUserTableSTRASSE->AsString = (const char *)theUser.strasse;
				ClientDataSetUserTablePOSTFACH->AsString = (const char *)theUser.postfach;
				ClientDataSetUserTableLAND->AsString = (const char *)theUser.land;
				ClientDataSetUserTablePLZ->AsString = (const char *)theUser.plz;
				ClientDataSetUserTableORT->AsString = (const char *)theUser.ort;
				ClientDataSetUserTableEMAIL->AsString = (const char *)theUser.email;
				ClientDataSetUserTableGUEST->AsString = theUser.guest.isEmpty() ? "-" : "X";
				ClientDataSetUserTableLOGINENABLED->AsString = theUser.loginEnabled.isEmpty() ? "-" : "X";
				ClientDataSetUserTableADMINISTRATOR->AsString = theUser.administrator.isEmpty() ? "-" : "X";
				ClientDataSetUserTable->Post();
			}
		}
	}
	catch( net::SoapRequest::SoapException &e )
	{
		STRING	errText = e.faultString;
		if( !e.faultDetail.isEmpty() )
		{
			errText += ": ";
			errText += e.faultDetail;
		}
		ClientDataSetUserTable->Tag = 0;
		ClientDataSetUserTableID->ReadOnly = true;
		doLogMessageEx( gakLogging::llError, errText );
/*@*/	throw EDatabaseError( (const char *)errText );
	}
	catch( std::exception &e )
	{
		ClientDataSetUserTable->Tag = 0;
		ClientDataSetUserTableID->ReadOnly = true;
		doLogMessageEx( gakLogging::llError, e.what() );
/*@*/	throw Exception( e.what() );
	}
	catch( ... )
	{
		ClientDataSetUserTable->Tag = 0;
		ClientDataSetUserTableID->ReadOnly = true;
		doLogMessageEx( gakLogging::llError, "Unknown error" );
/*@*/	throw;
	}
	ClientDataSetUserTable->Tag = 0;
	ClientDataSetUserTableID->ReadOnly = true;
	ClientDataSetUserTable->First();
}
//---------------------------------------------------------------------------

void __fastcall TRemoteUserForm::FormShow(TObject *Sender)
{
	RemoteServerForm->FillCombobox( ComboBoxRemoteServer );
	ComboBoxRemoteServerChange( Sender );
}
//---------------------------------------------------------------------------


void __fastcall TRemoteUserForm::ClientDataSetUserTableAfterOpen(
	  TDataSet *)
{
	loadTable();
}
//---------------------------------------------------------------------------


void __fastcall TRemoteUserForm::SpeedButtonRefreshClick(TObject *)
{
	ClientDataSetUserTable->Close();
	ClientDataSetUserTable->CreateDataSet();
}
//---------------------------------------------------------------------------

void __fastcall TRemoteUserForm::ClientDataSetUserTableBeforePost(
	  TDataSet *)
{
	if( !ClientDataSetUserTable->Tag )
	{
		ClientDataSetUserTableID->ReadOnly = false;
		DocManService::UserRecord_t theUser;

		theUser.id = ClientDataSetUserTableID->AsInteger;
		theUser.vorname = ClientDataSetUserTableVORNAME->AsString.c_str();
		theUser.nachname = ClientDataSetUserTableNACHNAME->AsString.c_str();
		theUser.strasse = ClientDataSetUserTableSTRASSE->AsString .c_str();
		theUser.postfach = ClientDataSetUserTablePOSTFACH->AsString.c_str();
		theUser.land = ClientDataSetUserTableLAND->AsString.c_str();
		theUser.plz = ClientDataSetUserTablePLZ->AsString.c_str();
		theUser.ort = ClientDataSetUserTableORT->AsString.c_str();
		theUser.email = ClientDataSetUserTableEMAIL->AsString.c_str();

		theUser.guest = ClientDataSetUserTableGUEST->AsString[1] == 'X' ? "X" : "";
		theUser.loginEnabled = ClientDataSetUserTableLOGINENABLED->AsString[1] == 'X' ? "X" : "";
		theUser.administrator = ClientDataSetUserTableADMINISTRATOR->AsString[1] == 'X' ? "X" : "";

		try
		{
			DocManService *myService = RemoteServerForm->GetServiceByID(
				remoteServerID
			);
			ClientDataSetUserTableID->AsInteger = myService->sUpdateUser( theUser );
		}
		catch( net::SoapRequest::SoapException &e )
		{
			STRING	errText = e.faultString;
			if( !e.faultDetail.isEmpty() )
			{
				errText += ": ";
				errText += e.faultDetail;
			}
			ClientDataSetUserTableID->ReadOnly = true;
/*@*/		throw EDatabaseError( (const char *)errText );
		}
		catch( std::exception &e )
		{
			ClientDataSetUserTableID->ReadOnly = true;
/*@*/		throw Exception( e.what() );
		}
		catch( ... )
		{
			ClientDataSetUserTableID->ReadOnly = true;
/*@*/		throw;
		}
		ClientDataSetUserTableID->ReadOnly = true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TRemoteUserForm::ClientDataSetUserTableBeforeDelete(
	  TDataSet *)
{
	try
	{
		DocManService *myService = RemoteServerForm->GetServiceByID(
			remoteServerID
		);
		myService->sDeleteUser( ClientDataSetUserTableID->AsInteger );
	}
	catch( net::SoapRequest::SoapException &e )
	{
		STRING	errText = e.faultString;
		if( !e.faultDetail.isEmpty() )
		{
			errText += ": ";
			errText += e.faultDetail;
		}
/*@*/	throw EDatabaseError( (const char *)errText );
	}
	catch( std::exception &e )
	{
/*@*/	throw Exception( e.what() );
	}
}
//---------------------------------------------------------------------------

void __fastcall TRemoteUserForm::FormHide(TObject *)
{
	ClientDataSetUserTable->Close();
}
//---------------------------------------------------------------------------

void __fastcall TRemoteUserForm::ClientDataSetUserTableAfterInsert(
	  TDataSet *)
{
	ClientDataSetUserTableGUEST->AsString = "-";
	ClientDataSetUserTableLOGINENABLED->AsString = "X";
	ClientDataSetUserTableADMINISTRATOR->AsString = "-";
}
//---------------------------------------------------------------------------

void __fastcall TRemoteUserForm::ComboBoxRemoteServerChange(
	  TObject *)
{
	ClientDataSetUserTable->Close();
	remoteServerID = RemoteServerForm->GetServerId(
		ComboBoxRemoteServer->ItemIndex
	);
	ClientDataSetUserTable->CreateDataSet();
}
//---------------------------------------------------------------------------

