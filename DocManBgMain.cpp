/*
		Project:		DocMan
		Module:			DocManBgMain.cpp
		Description:	The main form for the background processor
		Author:			Martin Gäckler
		Address:		Hofmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2025 Martin Gäckler

		This program is free software: you can redistribute it and/or modify  
		it under the terms of the GNU General Public License as published by  
		the Free Software Foundation, version 3.

		You should have received a copy of the GNU General Public License 
		along with this program. If not, see <http://www.gnu.org/licenses/>.

		THIS SOFTWARE IS PROVIDED BY Martin Gäckler, Linz, Austria ``AS IS''
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

#include <windows.h>
#include <shellapi.h>

#include <vcl.h>
#include <vcl/registry.hpp>

#include <winlib/winlib.h>

#include <gak/strFiles.h>
#include <gak/http.h>
#include <gak/rsa.h>

#pragma hdrstop

#include "LoginDlg.h"
#include "UserDlg.h"
#include "RemoteUserFrm.h"
#include "ChangePasswordDlg.h"
#include "SearchFrm.h"
#include "IndexFrm.h"

#include "DocManBgMain.h"
#include "ActionManager.h"
#include "ItemManager.h"
#include "ItemCreateFrm.h"
#include "DocManDM.h"
#include "DocManBGThread.h"
#include "XsltReportFrm.h"

#include "PermissionsFrm.h"
#include "ConfigFrm.h"
#include "config.h"

#include "File.h"

#include "TaskTypesFrm.h"
#include "TaskStatusFrm.h"
#include "BrowseFram.h"
#include "ReminderFilesFrm.h"
#include "OpenFilesFrm.h"
#include "OpenTasksFrm.h"
#include "OpenTestsFrm.h"
#include "LocalDirsFrm.h"
#include "MimeTypesFrm.h"
#include "CategoriesFrm.h"
#include "AboutOpenSSLfrm.h"
#include "Folder.h"
#include "RemoteServerFrm.h"
#include "PasswordDlg.h"

//---------------------------------------------------------------------------
using namespace gak;
using namespace vcl;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDocManBgMainForm *DocManBgMainForm;
//---------------------------------------------------------------------------
static const int DB_VERSION = 8;
//---------------------------------------------------------------------------
static int factoryCompare(
	const FACTORY_BASE * e1,
	const FACTORY_BASE * e2
)
{
	return strcmp( e1->getName(), e2->getName() );
}

//---------------------------------------------------------------------------

__fastcall TDocManBgMainForm::TDocManBgMainForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
inline const UserOrGroup *TDocManBgMainForm::login()
{
	return DocManDataModule->login();
}
//---------------------------------------------------------------------------
void __fastcall TDocManBgMainForm::FormCreate(TObject *)
{
	WindowProc = AppWindowProc;
	Application->OnMinimize = AppMinimize;
}
//---------------------------------------------------------------------------
void __fastcall TDocManBgMainForm::AppMinimize(TObject *)
{
	::ShowWindow( Application->Handle, SW_HIDE );
}

//---------------------------------------------------------------------------
void __fastcall TDocManBgMainForm::AppWindowProc(TMessage &msg)
{
	doEnterFunction( "TDocManBgMainForm::AppWindowProc" );

	try
	{
		doLogValue( msg.Msg );

		if( msg.Msg == WM_USER + 1 )
		{
			if( msg.LParam == WM_LBUTTONUP
			||  msg.LParam == WM_RBUTTONUP )
			{
	//			::ShowWindow( StatusForm->Handle, SW_SHOW );
				::ShowWindow( Application->Handle, SW_SHOW );
				Application->Restore();

				StatusForm->WindowState = wsNormal;
				StatusForm->Show();
				StatusForm->SetFocus();
			}
		}
		else if( msg.Msg == WM_ACTIVATEAPP )
		{
	#if 0
			if( isBackgroundJob() )
			{
				StatusForm->stopThread();

				StatusForm->WindowState = wsNormal;
				StatusForm->Show();
				StatusForm->SetFocus();
			}
	#endif
		}
		else if( msg.Msg == WM_QUERYENDSESSION  || msg.Msg == WM_ENDSESSION )
		{
			StatusForm->stopThread();
		}

		WndProc( msg );
	}
	catch( std::exception &e )
	{
		throw Exception( e.what() );
	}
	catch( Exception &e )
	{
		throw;
	}
	catch( ... )
	{
		throw Exception( "Unkown Error" );
	}
}

//---------------------------------------------------------------------------
void TDocManBgMainForm::performBackgroundTasks()
{
	doEnterFunction( "TDocManBgMainForm::performBackgroundTasks(" );
	NOTIFYICONDATA	IconData;

	IconData.cbSize = sizeof(IconData);
	IconData.hWnd = Handle;
	IconData.uID = 100;
	IconData.uFlags = NIF_MESSAGE + NIF_ICON + NIF_TIP;
	IconData.uCallbackMessage = WM_USER + 1;
	IconData.hIcon = Application->Icon->Handle;
	strcpy( IconData.szTip, "DocMan Background Process" );

	Shell_NotifyIcon(NIM_ADD, &IconData);

	::ShowWindow( Application->Handle, SW_HIDE );
	Application->Minimize();

	SharedObjectPointer<ThreadBackground> theThread = new ThreadBackground;

	theThread->StartThread( true, true );

#ifndef NDEBUG
	StatusForm->WindowState = wsNormal;
	StatusForm->Show();
	StatusForm->SetFocus();
#endif

	while( theThread->isRunning && !Thread::waitForMsgThreads() )
	{
		doLogPosition();
		idleLoop();
	}

	Shell_NotifyIcon(NIM_DELETE, &IconData);

	const STRING &errorText = theThread->getError();
	if( !errorText.isEmpty() )
	{
		::ShowWindow( Application->Handle, SW_SHOW );
		Application->Restore();

		StatusForm->WindowState = wsNormal;
		StatusForm->Show();
		StatusForm->SetFocus();

		Application->MessageBox( errorText, "Doc Man", MB_OK|MB_ICONERROR );
	}
}

//---------------------------------------------------------------------------
void __fastcall TDocManBgMainForm::FormShow(TObject *)
{
	STRING		privateDir;
	const char *tempDir = getenv( "TEMP" );

	print2StartWindow( "%s", "Connecting To Database" );

	if( tempDir )
	{
		privateDir = tempDir;
		privateDir += "\\DocManBG";
	}
	else
	{
		privateDir += "C:\\DocManBG";
	}

	mkdir( privateDir );

	Session->PrivateDir = (const char*)privateDir;

	STRING errorText = ConfigDataModule->OpenDatabase( DocManDataModule->theDatabase, DB_VERSION );
	if( !errorText.isEmpty() )
	{
		errorText = STRING("Unable to connect to database.\n") + errorText;
		Application->MessageBox( errorText, "Error", MB_ICONSTOP );
		Application->Terminate();
/*@*/	return;
	}

	print2StartWindow( "%s", "Creating Session" );

	STRING externalStorage = ConfigDataModule->GetValue( "externalStorage", "" );
	if( externalStorage.isEmpty() )
	{
		errorText = STRING("External Storage unknown.\n");
		Application->MessageBox( errorText, "Error", MB_ICONSTOP );
		Application->Terminate();
/*@*/	return;
	}
	else
		THE_FILE::setExternalStorage( externalStorage );

	STRING proxyServer = ConfigDataModule->GetValue( "proxyServer", "" );
	int proxyPort = ConfigDataModule->GetValue( "proxyPort", 0 );
	STRING noProxy = ConfigDataModule->GetValue( "noProxy", "" );
	net::HTTPrequest::setProxy( proxyServer, proxyPort, noProxy );

	const UserOrGroup *actUser = login();
	if( !actUser->ID )					// sorry
	{
		Application->Terminate();
	}

	closeStartup();

	performBackgroundTasks();
	Application->Terminate();
}

//---------------------------------------------------------------------------
void __fastcall TDocManBgMainForm::FormClose(TObject *,
	  TCloseAction &)
{
	doEnterFunction("TDocManBgMainForm::FormClose");
	DocManDataModule->theDatabase->Close();
}
//---------------------------------------------------------------------------

void __fastcall TDocManBgMainForm::ReminderTimerTimer(TObject *)
{
	if( ReminderFilesForm->Active )
	{
		ReminderTimer->Interval = 600000;
	}
	else if( !ReminderFilesForm->Visible )
	{
		ReminderTimer->Interval = 10000;
	}
	showReminder();
}
//--------------------------------------------------------------------------
