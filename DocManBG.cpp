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

#undef STRICT
#define STRICT 1

//---------------------------------------------------------------------------

#include <vcl.h>

#include <gak/gaklib.h>

#include <winlib/winlib.h>
#include <winlib/winapp.h>

#pragma hdrstop
USERES("DocManBG.res");
USEFORM("DocManBgMain.cpp", DocManBgMainForm);
USEUNIT("ItemManager.cpp");
USELIB("..\..\Object\gaklib\gaklib_bcb.lib");
USE("DocManBG.todo", ToDo);
USEUNIT("ActionManager.cpp");
USEUNIT("Folder.cpp");
USEFORM("..\GAKLIB\Repository\Config.cpp", ConfigDataModule); /* TDataModule: File Type */
USEFORM("ItemCreateFrm.cpp", ItemCreateForm);
USEUNIT("TaskList.cpp");
USEFORM("GeneralFrm.cpp", GeneralForm);
USEFORM("DocManDM.cpp", DocManDataModule); /* TDataModule: File Type */
USEFORM("..\GAKLIB\Repository\StatusFrm.cpp", StatusForm);
USEUNIT("Alias.cpp");
USEFORM("ReminderFilesFrm.cpp", ReminderFilesForm);
USEFORM("MimeTypesFrm.cpp", MimeTypesForm);
USEUNIT("File.cpp");
USEUNIT("WebFolder.cpp");
USEFORM("SearchFrm.cpp", SearchForm);
USEUNIT("..\GRAPHIX\MAGIC.C");
USERC("bitmaps.rc");
USEUNIT("Archive.cpp");
USELIB("..\..\Object\winlib\winlibBCB.lib");
USEUNIT("TestFolder.cpp");
USEUNIT("DocManService.cpp");
USEUNIT("RemoteItems.cpp");
USEUNIT("CryptoFile.cpp");
USELIB("..\..\Object\openssl.lib");
USEFORM("..\GAKLIB\Repository\LoginDlg.cpp", LoginForm);
USEUNIT("DocManThreads.cpp");
USEUNIT("DocManBGThread.cpp");
USEFORM("..\GAKLIB\Repository\AboutFrm.cpp", AboutProgramForm);
USEFORM("..\GAKLIB\Repository\PasswordDlg.cpp", PasswordForm);
USEFORM("RemoteServerFrm.cpp", RemoteServerForm);
//---------------------------------------------------------------------------
#include "xmlEditFram.h"
#include "file.h"
#include "WebFolder.h"

//---------------------------------------------------------------------------
static STRING getSchemaFile4Namespace( const STRING &nameSpace )
{
	if( nameSpace == XMLNS_DOCMAN )
		return gak::makeFullPath(
			Application->ExeName.c_str(), "docman.xsd"
		);

	if( nameSpace == XMLNS_DMCONTENT )
		return gak::makeFullPath(
			Application->ExeName.c_str(), "content.xsd"
		);

	return "";
}

//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE hInstance, HINSTANCE , LPSTR , int)
{
	doEnableLogEx(gakLogging::llInfo);
	doEnterFunctionEx(gakLogging::llInfo, "WinMain");

	openStartup( NULL, "SPLASH_BITMAP" );

	winlib::Application theVclApp;
	theVclApp.setInstance( hInstance );

//	XML_MY_SCHEMA_MANAGER::setXmlDocLoadFunction( loadXmlDoc );
//	XML_MY_SCHEMA_MANAGER::setSchemaFile4NamespaceCB( getSchemaFile4Namespace );
//	TxmlEditorFrame::setPlainTextLoader( loadCssDoc );

	try
	{
		Application->Initialize();
		Application->Title = "Doc Manager";
		Application->CreateForm(__classid(TDocManBgMainForm), &DocManBgMainForm);
		Application->CreateForm(__classid(TConfigDataModule), &ConfigDataModule);
		Application->CreateForm(__classid(TItemCreateForm), &ItemCreateForm);
		Application->CreateForm(__classid(TGeneralForm), &GeneralForm);
		Application->CreateForm(__classid(TDocManDataModule), &DocManDataModule);
		Application->CreateForm(__classid(TStatusForm), &StatusForm);
		Application->CreateForm(__classid(TReminderFilesForm), &ReminderFilesForm);
		Application->CreateForm(__classid(TMimeTypesForm), &MimeTypesForm);
		Application->CreateForm(__classid(TSearchForm), &SearchForm);
		Application->CreateForm(__classid(TLoginForm), &LoginForm);
		Application->CreateForm(__classid(TPasswordForm), &PasswordForm);
		Application->CreateForm(__classid(TRemoteServerForm), &RemoteServerForm);
		Application->CreateForm(__classid(TAboutProgramForm), &AboutProgramForm);
		print2StartWindow( "%s", "Starting Application" );

		Application->Run();
	}
	catch (Exception &exception)
	{
		doLogPositionEx(gakLogging::llFatal);
		closeStartup();
		Application->ShowException(&exception);
		doLogPositionEx(gakLogging::llFatal);
	}

	return 0;
}
//---------------------------------------------------------------------------
