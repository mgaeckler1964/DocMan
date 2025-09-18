/*
		Project:		DocMan
		Module:			
		Description:	
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

#undef STRICT
#define STRICT 1

//---------------------------------------------------------------------------

#include <vcl.h>

#include <gak/gaklib.h>

#include <winlib/winlib.h>
#include <winlib/winapp.h>

#pragma hdrstop
USERES("DocMan.res");
USEFORM("DocManMain.cpp", DocManMainForm);
USEUNIT("ItemManager.cpp");
USELIB("..\..\Object\gaklib\gaklib_bcb.lib");
USE("DocMan.todo", ToDo);
USEFORM("..\GAKLIB\Repository\UserDlg.cpp", UserDialog);
USEUNIT("ActionManager.cpp");
USEFORM("ProjectHistFrm.cpp", ProjectHistoryForm);
USEUNIT("Folder.cpp");
USEFORM("..\GAKLIB\Repository\Config.cpp", ConfigDataModule); /* TDataModule: File Type */
USEFORM("ConfigFrm.cpp", ConfigForm);
USEFORM("PermissionsFrm.cpp", PermissionsForm);
USEFORM("TargetBrowseFrm.cpp", TargetBrowseForm);
USEFORM("CopyMoveLinkFrm.cpp", CopyMoveLinkForm);
USEFORM("SyncLocalFrm.cpp", SyncLocalForm);
USEFORM("..\GAKLIB\Repository\DirSelFrm.cpp", DirectorySelector);
USEFORM("ItemCreateFrm.cpp", ItemCreateForm);
USEUNIT("Delete.cpp");
USEFORM("CreateVersionFrm.cpp", VersionCreateForm);
USEFORM("DiffFrm.cpp", DiffForm);
USEFORM("DirectoryDiffFrm.cpp", DirectoryDiffForm);
USEUNIT("TaskList.cpp");
USEFORM("TaskFrm.cpp", TaskForm);
USEFORM("SyncFolderCreateFrm.cpp", SyncFolderCreateForm);
USEFORM("RemoteFolderCreateFrm.cpp", RemoteFolderCreateForm);
USEFORM("GeneralFrm.cpp", GeneralForm);
USEFORM("FileCreateFrm.cpp", FileCreateForm);
USEFORM("ReleaseFrm.cpp", ReleaseForm);
USEFORM("TaskStatusFrm.cpp", TaskStatusForm);
USEFORM("DocManDM.cpp", DocManDataModule); /* TDataModule: File Type */
USEFORM("TaskTypesFrm.cpp", TaskTypesForm);
USEFORM("..\GAKLIB\Repository\StatusFrm.cpp", StatusForm);
USEFORM("BrowseFram.cpp", BrowserFrame); /* TFrame: File Type */
USEFORM("TasklistBrowserFram.cpp", TasklistBrowserFrame); /* TFrame: File Type */
USEFORM("CheckInFrm.cpp", CheckInForm);
USEFORM("OpenTestsFrm.cpp", OpenTestsForm);
USEFORM("LocalDirsFrm.cpp", LocalDirsForm);
USEFORM("PermApplyDownFrm.cpp", PermApplyDownForm);
USEFORM("..\GAKLIB\Repository\PasswordDlg.cpp", PasswordForm);
USEFORM("..\GAKLIB\Repository\ChangePasswordDlg.cpp", ChangePasswordForm);
USEUNIT("Alias.cpp");
USEFORM("ReminderFilesFrm.cpp", ReminderFilesForm);
USEFORM("MimeTypesFrm.cpp", MimeTypesForm);
USEFORM("EditFrm.cpp", EditFileForm);
USEFORM("EditPlainFileFrm.cpp", EditPlainFileForm);
USEFORM("EditXmlFileFrm.cpp", EditXmlFileForm);
USEUNIT("File.cpp");
USEFORM("LanguageDocCreateFrm.cpp", LanguageDocCreateForm);
USEFORM("LanguageItemCreateFrm.cpp", LanguageItemCreateForm);
USEFORM("MultiActionFrm.cpp", MultiActionForm);
USEFORM("XsltReportFrm.cpp", XsltReportForm);
USEFORM("TaskFilesFrm.cpp", TaskFilesForm);
USEFORM("CheckOut.cpp", CheckOutForm);
USEFORM("ReferencesFrm.cpp", ReferencesForm);
USEFORM("FileLinksFrm.cpp", FileLinksForm);
USEFORM("VersionsFrm.cpp", VersionsForm);
USEFORM("ChangeOrderFrm.cpp", ChangeOrderForm);
USEFORM("PurgeVersionsFrm.cpp", PurgeVersionsForm);
USEFORM("MakeLangFrm.cpp", MakeLanguageForm);
USEUNIT("WebFolder.cpp");
USEFORM("SearchFrm.cpp", SearchForm);
USEFORM("ExifFrm.cpp", ExifForm);
USEUNIT("..\GRAPHIX\MAGIC.C");
USEFORM("CategoriesFrm.cpp", CategoriesForm);
USEFORM("AttributesFrm.cpp", AttributesForm);
USERC("bitmaps.rc");
USEUNIT("Archive.cpp");
USEFORM("ImageFrm.cpp", ImageBrowserForm);
USELIB("..\..\Object\winlib\winlibBCB.lib");
USEFORM("WebServerFrm.cpp", WebServerForm);
USEUNIT("TestFolder.cpp");
USEFORM("TestStepFrm.cpp", TestStepForm);
USEFORM("TestPrepareFrm.cpp", TestPrepareForm);
USEFORM("TestStepExecFrm.cpp", TestStepExecForm);
USEFORM("OpenTasksFrm.cpp", OpenTasksForm);
USEFORM("WebWizardFrm.cpp", WebWizardForm);
USEUNIT("DocManService.cpp");
USEFORM("RemoteUserFrm.cpp", RemoteUserForm);
USEFORM("RemoteServerFrm.cpp", RemoteServerForm);
USEFORM("SourceFolderCreateFrm.cpp", SourceFolderCreateForm);
USEUNIT("RemoteItems.cpp");
USEUNIT("CryptoFile.cpp");
USELIB("..\..\Object\openssl.lib");
USEFORM("CryptoKeysFrm.cpp", CryptoKeysForm);
USEFORM("..\GAKLIB\Repository\LoginDlg.cpp", LoginForm);
USEFORM("..\GAKLIB\Repository\AboutFrm.cpp", AboutProgramForm);
USEFORM("..\GAKLIB\Repository\AboutOpenSSLfrm.cpp", AboutOpenSSLForm);
USEUNIT("DocManThreads.cpp");
USEFORM("IndexFrm.cpp", IndexForm);
USEFORM("..\XML_Edit\xmlEditFram.cpp", xmlEditorFrame); /* TFrame: File Type */
USEFORM("..\XML_Edit\XmlViewerFrm.cpp", XmlViewerForm);
USEFORM("..\XML_Edit\cssStyleFram.cpp", cssStyleFrame); /* TFrame: File Type */
USEFORM("..\XML_Edit\cssStyleFrm.cpp", cssStyleForm);
USEFORM("OpenFilesFrm.cpp", OpenFilesForm);
USEUNIT("DocManBGThread.cpp");
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

	XmlMySchemaManager::setXmlDocLoadFunction( loadXmlDoc );
	XmlMySchemaManager::setSchemaFile4NamespaceCB( getSchemaFile4Namespace );
	TxmlEditorFrame::setPlainTextLoader( loadCssDoc );
	try
	{
		Application->Initialize();
		Application->Title = "Doc Manager";
		Application->CreateForm(__classid(TDocManMainForm), &DocManMainForm);
		Application->CreateForm(__classid(TUserDialog), &UserDialog);
		Application->CreateForm(__classid(TProjectHistoryForm), &ProjectHistoryForm);
		Application->CreateForm(__classid(TConfigDataModule), &ConfigDataModule);
		Application->CreateForm(__classid(TConfigForm), &ConfigForm);
		Application->CreateForm(__classid(TPermissionsForm), &PermissionsForm);
		Application->CreateForm(__classid(TTargetBrowseForm), &TargetBrowseForm);
		Application->CreateForm(__classid(TCopyMoveLinkForm), &CopyMoveLinkForm);
		Application->CreateForm(__classid(TSyncLocalForm), &SyncLocalForm);
		Application->CreateForm(__classid(TDirectorySelector), &DirectorySelector);
		Application->CreateForm(__classid(TItemCreateForm), &ItemCreateForm);
		Application->CreateForm(__classid(TVersionCreateForm), &VersionCreateForm);
		Application->CreateForm(__classid(TDiffForm), &DiffForm);
		Application->CreateForm(__classid(TDirectoryDiffForm), &DirectoryDiffForm);
		Application->CreateForm(__classid(TTaskForm), &TaskForm);
		Application->CreateForm(__classid(TSyncFolderCreateForm), &SyncFolderCreateForm);
		Application->CreateForm(__classid(TRemoteFolderCreateForm), &RemoteFolderCreateForm);
		Application->CreateForm(__classid(TGeneralForm), &GeneralForm);
		Application->CreateForm(__classid(TFileCreateForm), &FileCreateForm);
		Application->CreateForm(__classid(TReleaseForm), &ReleaseForm);
		Application->CreateForm(__classid(TTaskStatusForm), &TaskStatusForm);
		Application->CreateForm(__classid(TDocManDataModule), &DocManDataModule);
		Application->CreateForm(__classid(TTaskTypesForm), &TaskTypesForm);
		Application->CreateForm(__classid(TStatusForm), &StatusForm);
		Application->CreateForm(__classid(TCheckInForm), &CheckInForm);
		Application->CreateForm(__classid(TOpenTestsForm), &OpenTestsForm);
		Application->CreateForm(__classid(TLocalDirsForm), &LocalDirsForm);
		Application->CreateForm(__classid(TPermApplyDownForm), &PermApplyDownForm);
		Application->CreateForm(__classid(TPasswordForm), &PasswordForm);
		Application->CreateForm(__classid(TChangePasswordForm), &ChangePasswordForm);
		Application->CreateForm(__classid(TReminderFilesForm), &ReminderFilesForm);
		Application->CreateForm(__classid(TMimeTypesForm), &MimeTypesForm);
		Application->CreateForm(__classid(TLanguageDocCreateForm), &LanguageDocCreateForm);
		Application->CreateForm(__classid(TLanguageItemCreateForm), &LanguageItemCreateForm);
		Application->CreateForm(__classid(TMultiActionForm), &MultiActionForm);
		Application->CreateForm(__classid(TXsltReportForm), &XsltReportForm);
		Application->CreateForm(__classid(TTaskFilesForm), &TaskFilesForm);
		Application->CreateForm(__classid(TCheckOutForm), &CheckOutForm);
		Application->CreateForm(__classid(TReferencesForm), &ReferencesForm);
		Application->CreateForm(__classid(TVersionsForm), &VersionsForm);
		Application->CreateForm(__classid(TChangeOrderForm), &ChangeOrderForm);
		Application->CreateForm(__classid(TFileLinksForm), &FileLinksForm);
		Application->CreateForm(__classid(TPurgeVersionsForm), &PurgeVersionsForm);
		Application->CreateForm(__classid(TMakeLanguageForm), &MakeLanguageForm);
		Application->CreateForm(__classid(TSearchForm), &SearchForm);
		Application->CreateForm(__classid(TExifForm), &ExifForm);
		Application->CreateForm(__classid(TCategoriesForm), &CategoriesForm);
		Application->CreateForm(__classid(TAttributesForm), &AttributesForm);
		Application->CreateForm(__classid(TWebServerForm), &WebServerForm);
		Application->CreateForm(__classid(TTestStepForm), &TestStepForm);
		Application->CreateForm(__classid(TTestPrepareForm), &TestPrepareForm);
		Application->CreateForm(__classid(TTestStepExecForm), &TestStepExecForm);
		Application->CreateForm(__classid(TOpenTasksForm), &OpenTasksForm);
		Application->CreateForm(__classid(TWebWizardForm), &WebWizardForm);
		Application->CreateForm(__classid(TRemoteUserForm), &RemoteUserForm);
		Application->CreateForm(__classid(TRemoteServerForm), &RemoteServerForm);
		Application->CreateForm(__classid(TSourceFolderCreateForm), &SourceFolderCreateForm);
		Application->CreateForm(__classid(TCryptoKeysForm), &CryptoKeysForm);
		Application->CreateForm(__classid(TLoginForm), &LoginForm);
		Application->CreateForm(__classid(TAboutOpenSSLForm), &AboutOpenSSLForm);
		Application->CreateForm(__classid(TIndexForm), &IndexForm);
		Application->CreateForm(__classid(TXmlViewerForm), &XmlViewerForm);
		Application->CreateForm(__classid(TcssStyleForm), &cssStyleForm);
		Application->CreateForm(__classid(TOpenFilesForm), &OpenFilesForm);
		print2StartWindow( "%s", "Starting Application" );

		Application->Run();
	}
	catch (Exception &exception)
	{
		doLogPosition();
		closeStartup();
		Application->ShowException(&exception);
		doLogPosition();
	}

	return 0;
}
//---------------------------------------------------------------------------
