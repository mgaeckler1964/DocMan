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

#include "DocManMain.h"
#include "ActionManager.h"
#include "ItemManager.h"
#include "ItemCreateFrm.h"
#include "DocManDM.h"
#include "DocManBG.h"
#include "XsltReportFrm.h"

#include "PermissionsFrm.h"
#include "ConfigFrm.h"
#include "config.h"

#include "File.h"

#include "TaskTypesFrm.h"
#include "TaskStatusFrm.h"
#include "BrowseFram.h"
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
TDocManMainForm *DocManMainForm;
//---------------------------------------------------------------------------
char registryKey[] = "\\Software\\CRESD\\DocMan";
//---------------------------------------------------------------------------
static int factoryCompare(
	const FACTORY_BASE * e1,
	const FACTORY_BASE * e2
)
{
	return strcmp( e1->getName(), e2->getName() );
}

//---------------------------------------------------------------------------

__fastcall TDocManMainForm::TDocManMainForm(TComponent* Owner)
	: TForm(Owner)
{
	browserFrame = NULL;
	currentItem = NULL;
	m_actUser = NULL;
	backgroundJob = false;
}
//---------------------------------------------------------------------------
void TDocManMainForm::login( void )
{
	doEnterFunction("TDocManMainForm::login");
	bool	showLogin;

	if( !m_actUser )	// first call
	{
		print2StartWindow( "%s", "Autologin" );

		m_actUser = &::getActUser( theDatabase->DatabaseName );
		showLogin = false;
		if( m_actUser->ID )
		{
			STRING		publicKeyFile = THE_FILE::getExternalStorageBase();

			if( !publicKeyFile.isEmpty() )
			{
				publicKeyFile += "keys" DIRECTORY_DELIMITER_STRING;
				publicKeyFile += m_actUser->userName;

				CryptoRSA	publicKey( true );
				if( publicKey.hasKey() )
				{
					makePath( publicKeyFile );
					publicKey.saveCypher( publicKeyFile );
				}
			}
		}
	}
	else			// call from menu
	{
		showLogin = true;
	}

	do
	{
		if( showLogin )
		{
			if( LoginForm->ShowModal( "Doc Manager" ) == mrOk )
			{
				actUser = vcl::loginUser(
					theDatabase->DatabaseName,
					LoginForm->EditUserName->Text,
					LoginForm->EditPassword->Text
				);
			}
			else
			{
/*v*/			break;
			}
		}

		if( !m_actUser->ID )
		{
			Application->MessageBox( "Unknown User or Password", "Error", MB_ICONSTOP );
		}

		if( !m_actUser->permissions & vcl::USER_LOGIN_PERM )
		{
			Application->MessageBox( "Login Denied", "Error", MB_ICONSTOP );
			m_actUser = NULL;
		}

		showLogin = true;
	} while( !m_actUser || m_actUser->ID == 0 );

	if( m_actUser->ID )
	{
		Administration->Enabled = vcl::isSystemAdmin();

		print2StartWindow( "%s", "Init. XSLT Volume" );
		initXsltVolume();

		print2StartWindow( "%s", "Init. Template Volume" );
		getPublicVolume( TYPE_PUBLIC_TEMPLATE_FOLDER );

		print2StartWindow( "%s", "Init. Archive Volume" );
		getArchiveVolume();

		print2StartWindow( "%s", "Loading Task Status" );
		DocManDataModule->refreshTaskStatus();

		print2StartWindow( "%s", "Loading Task Types" );
		DocManDataModule->refreshTaskTypes();

		print2StartWindow( "%s", "Loading Company Volume" );
		PTR_ITEM	companyVolume = getCompanyVolume();

		if( !isBackgroundJob() )
			openItem( companyVolume );
	}
}
//---------------------------------------------------------------------------
void TDocManMainForm::fillCreateItems( void )
{
	int perms = currentItem->loadPermissions();

	if( perms & ITEM_PERM_ADD_ITEMS )
	{
		currentItem->getItemFactories( &m_addItemTypes );
		m_addItemTypes.sort( factoryCompare );

		ComboBoxAddItemTypes->Items->Clear();
		if( m_addItemTypes.size() )
		{
			for(
				AddItemTypes::const_iterator it = m_addItemTypes.cbegin(),
					endIT = m_addItemTypes.cend();
				it !=  endIT;
				++it
			)
			{
				ComboBoxAddItemTypes->Items->Add( (*it)->getName() );
			}
			ComboBoxAddItemTypes->Visible = true;
			LabelAddItem->Visible = true;
		}
		else
		{
			ComboBoxAddItemTypes->Visible = false;
			LabelAddItem->Visible = false;
		}
	}
	else
	{
		ComboBoxAddItemTypes->Visible = false;
		LabelAddItem->Visible = false;
	}
}

//---------------------------------------------------------------------------
void TDocManMainForm::fillParentItems( void )
{
	AnsiString	title;
	STRING		path;
	ArrayOfInts	*theAncestors = currentItem->getAncestors();
	std::size_t	ancestorCount = theAncestors->size();
	STRING		theName;
	PTR_ITEM	ancestor;

	if( ancestorCount )
	{
		ComboBoxParents->Items->Clear();

		for( size_t i=0; i<ancestorCount; i++ )
		{
			ancestor = currentItem->getAncestorItem( i );
			if( ancestor )
			{
				theName = ancestor->getName();
				ComboBoxParents->Items->Add( (const char *)theName );
			}
		}
		theName = currentItem->getName();
		ComboBoxParents->Items->Add( (const char *)theName );

		ComboBoxParents->Visible = true;
		ComboBoxParents->ItemIndex = ComboBoxParents->Items->Count-1; 
		SpeedButtonUp->Visible = true;
	}
	else
	{
		ComboBoxParents->Visible = false;
		SpeedButtonUp->Visible = false;
	}

	path = currentItem->getPath();
	if( path.isEmpty() || path==DIRECTORY_DELIMITER_STRING )
	{
		path = "<root>";
	}

	title = "Doc Manager ";
	title += (const char *)path;
	Caption = title;
}
//---------------------------------------------------------------------------
void TDocManMainForm::fillContents( int selectID )
{
	doEnterFunction("TDocManMainForm::fillContents");

	::SetCursor( LoadCursor( NULL, IDC_WAIT ) );
//	Cursor = crHourGlass;
	clearItemCache( currentItem );
	fillCreateItems();
	fillParentItems();
	browserFrame->fillContents( currentItem, selectID );
//	Cursor = crDefault;
	::SetCursor( LoadCursor( NULL, IDC_ARROW ) );
}
//---------------------------------------------------------------------------
void __fastcall TDocManMainForm::ReportClick(TObject *Sender)
{
	STRING theReport = ConfigDataModule->GetValue( "reportPath", "reports\\" );
	theReport += ((TMenuItem*)Sender)->Caption.c_str();

	theReport = makeFullPath( Application->ExeName.c_str(), theReport );

	STRING theReportCmd = "srxViewer.exe DOCMAN \"";
	theReportCmd += theReport;
	theReportCmd += '\"';

	WinExec( theReportCmd, SW_SHOW );
}
//---------------------------------------------------------------------------

void __fastcall TDocManMainForm::FormCreate(TObject *)
{
	WindowProc = AppWindowProc;
	Application->OnMinimize = AppMinimize;
}
//---------------------------------------------------------------------------
void __fastcall TDocManMainForm::AppMinimize(TObject *)
{
	if( isBackgroundJob() )
	{
		::ShowWindow( Application->Handle, SW_HIDE );
	}
}

//---------------------------------------------------------------------------
void __fastcall TDocManMainForm::AppWindowProc(TMessage &msg)
{
	doEnterFunction( "TDocManMainForm::AppWindowProc" );

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
void TDocManMainForm::initMenu( void )
{
	DirectoryList	reports;
	STRING 			reportPath = ConfigDataModule->GetValue( "reportPath", "reports\\" );

	reportPath += "*.srx";
	reportPath = makeFullPath( Application->ExeName.c_str(), reportPath );

	reports.findFiles( reportPath );

	MenuReports->AutoHotkeys = maManual;
	for(
		DirectoryList::iterator it = reports.begin(), endIT = reports.end();
		it != endIT;
		++it
	)
	{
		TMenuItem *newMenu = new TMenuItem( this );
		newMenu->AutoHotkeys = maManual;
		newMenu->Caption = static_cast<const char *>(it->fileName);
		newMenu->OnClick = ReportClick;

		MenuReports->Add( newMenu );
	}
}

//---------------------------------------------------------------------------
void TDocManMainForm::performBackgroundTasks( void )
{
	doEnterFunction( "TDocManMainForm::performBackgroundTasks(" );
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
void __fastcall TDocManMainForm::FormShow(TObject *Sender)
{
	STRING		privateDir;
	const char *tempDir = getenv( "TEMP" );

	print2StartWindow( "%s", "Connecting To Database" );

	if( tempDir )
	{
		privateDir = tempDir;
		privateDir += "\\DocMan";
	}
	else
	{
		privateDir += "C:\\DocMan";
	}

	if( Application->Tag )
	{
		privateDir += "BG";
		backgroundJob = true;
	}

	mkdir( privateDir );

	Session->PrivateDir = (const char*)privateDir;

	STRING errorText = ConfigDataModule->OpenDatabase( theDatabase );
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
		AdminConfigureClick( Sender );
	else
		THE_FILE::setExternalStorage( externalStorage );

	STRING proxyServer = ConfigDataModule->GetValue( "proxyServer", "" );
	int proxyPort = ConfigDataModule->GetValue( "proxyPort", 0 );
	STRING noProxy = ConfigDataModule->GetValue( "noProxy", "" );
	net::HTTPrequest::setProxy( proxyServer, proxyPort, noProxy );

	login();
	if( !m_actUser->ID )					// sorry
	{
		Application->Terminate();
	}

	if( !isBackgroundJob() )
	{
		initMenu();
	}

	closeStartup();

	if( isBackgroundJob() )
	{
		performBackgroundTasks();
		Application->Terminate();
	}
}
//---------------------------------------------------------------------------
void __fastcall TDocManMainForm::FormClose(TObject *,
	  TCloseAction &)
{
	doEnterFunction("TDocManMainForm::FormClose");
	theDatabase->Close();
}
//---------------------------------------------------------------------------

void __fastcall TDocManMainForm::ComboBoxAddItemTypesChange(TObject *)
{
	if( ComboBoxAddItemTypes->ItemIndex >= 0 )
	{
		try
		{
			const FACTORY_BASE *childFactory = m_addItemTypes[ComboBoxAddItemTypes->ItemIndex];

			TItemCreateForm *childForm = childFactory->getForm();

			if( childForm )
			{
				childForm->Tag = (int)childFactory;
				childForm->setParent( currentItem );
			}

			if( !childForm || childForm->ShowModal() == mrOk )
			{
				PTR_ITEM newItem = childFactory->createItemFromForm( currentItem );
				if( newItem )
				{
					fillContents( newItem->getID() );
				}
			}
		}
		catch( ... )
		{
			ComboBoxAddItemTypes->ItemIndex = -1;
/*@*/		throw;
		}

		ComboBoxAddItemTypes->ItemIndex = -1;
	}
}
//---------------------------------------------------------------------------
void __fastcall TDocManMainForm::WmDropFiles(TWMDropFiles& Message)
{
	char	buff[MAX_PATH*10];
	HDROP	hDrop = (HDROP)Message.Drop;
	int		numFiles = DragQueryFile(hDrop,-1, NULL, NULL);

	::SetCursor( LoadCursor( NULL, IDC_WAIT ) );

	for (int i=0;i<numFiles;i++)
	{
		DragQueryFile( hDrop, i, buff, sizeof(buff) );

		if( isFile( buff ) )
			currentItem->dropFile( buff );
		else
			currentItem->dropFolder( buff );
	}
	DragFinish(hDrop);

	browserFrame->fillContents( currentItem, -1 );

	::SetCursor( LoadCursor( NULL, IDC_ARROW ) );
}
//---------------------------------------------------------------------------

void TDocManMainForm::openItem( PTR_ITEM newItem, int selectID, bool handleHistory )
{
	doEnterFunction("TDocManMainForm::openItem");

	if( newItem != currentItem
	&& (!currentItem || newItem->getID() != currentItem->getID()) )
	{
		if( currentItem )
		{
			if( selectID < 0 )
				selectID = currentItem->getID();
			newItem->setSortType( currentItem->getSortType() );

			if( handleHistory )
			{
				history.push( currentItem );
				forwardHistory.clear();
				SpeedButtonBack->Enabled = true;
				SpeedButtonForward->Enabled = false;
			}
		}
		currentItem = newItem;
		TBrowserFrame *newFrame = currentItem->getFrame();
		if( browserFrame != newFrame )
		{
			if( browserFrame )
			{
				RemoveControl( browserFrame );
				browserFrame->Parent = NULL;
			}
			newFrame->Parent = this;
			browserFrame = newFrame;
		}
		fillContents( selectID );

		bool	acceptFiles = currentItem->acceptDropFiles();
		DragAcceptFiles( Handle, acceptFiles );

		STRING	localPath = newItem->getDownloadPath( newItem->getParent() );
		StatusBar->SimpleText = (const char *)localPath;
	}
	else if( selectID > 0 )
	{
		browserFrame->selectItem( selectID );
	}
}
//---------------------------------------------------------------------------

void __fastcall TDocManMainForm::SpeedButtonUpClick(TObject *)
{
	PTR_ITEM	parentItem = currentItem->getParent();
	if( parentItem )
		openItem( parentItem );
}
//---------------------------------------------------------------------------

void __fastcall TDocManMainForm::ComboBoxParentsChange(TObject *)
{
	int newParentIdx = ComboBoxParents->ItemIndex;
	if( newParentIdx >= 0 )
	{
		ArrayOfInts	*theAncestors = currentItem->getAncestors();
		std::size_t	numAncestors = theAncestors->size();

		if( newParentIdx < int(numAncestors) )
		{
			int		selectID;

			PTR_ITEM	newItem = currentItem->getAncestorItem( newParentIdx );
			if( newParentIdx < (int)numAncestors-1 )
			{
				PTR_ITEM	selectItem =currentItem->getAncestorItem(
					newParentIdx+1
				);
				selectID = selectItem->getID();
			}
			else
			{
				selectID = -1;
			}

			openItem( newItem, selectID );
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TDocManMainForm::FileUserClick(TObject *)
{
	UserDialog->ShowModalWithDB( theDatabase->DatabaseName );
}
//---------------------------------------------------------------------------

void __fastcall TDocManMainForm::SpeedButtonPopupClick(TObject *)
{
	selectedItem = currentItem;
	getItemActions( *m_actUser, selectedItem, &m_popupActions );
	PopupMenuActions->Items->Clear();
	int group=-1;
	for( size_t i=0; i<m_popupActions.size(); i++ )
	{
		const ACTION_BASE *theAction = m_popupActions[i];
		if( i>0 && theAction->getGroup() != group )
		{
			TMenuItem	*newItem = new TMenuItem( PopupMenuActions );
			newItem->Caption = "-";
			PopupMenuActions->Items->Add( newItem );
		}
		group = theAction->getGroup();
		TMenuItem	*newItem = new TMenuItem( PopupMenuActions );
		newItem->Caption = theAction->getLabel();
		newItem->Tag = i;
		newItem->OnClick = ActionClick;
		PopupMenuActions->Items->Add( newItem );
	}
	TPoint	thePoint( 0, 0 );
	thePoint = SpeedButtonPopup->ClientToScreen( thePoint );

	PopupMenuActions->Popup( thePoint.x, thePoint.y );
}
//---------------------------------------------------------------------------

void __fastcall TDocManMainForm::ActionClick( TObject *Sender )
{
	try
	{
		int i = static_cast<TMenuItem *>(Sender)->Tag;
		if( i < int(m_popupActions.size()) )
		{
			ACTION_BASE *theAction = m_popupActions[i];
			switch( theAction->perform( selectedItem ) )
			{
				case REFRESH_RELOAD:
					fillContents( selectedItem->getID() );
					break;
				case REFRESH_REDRAW:
					browserFrame->DrawGridContent->Invalidate();
					break;
			}
		}
		m_popupActions.clear();
	}
	catch( std::exception &e )
	{
		m_popupActions.clear();
		fillContents( selectedItem->getID() );
		throw Exception( e.what() );
	}
}
//---------------------------------------------------------------------------

void __fastcall TDocManMainForm::AdminConfigureClick(TObject *)
{
	STRING externalStorage = ConfigDataModule->GetValue( "externalStorage", "" );
	ConfigForm->EditExternalStorage->Text = (const char *)externalStorage;
	STRING reportPath = ConfigDataModule->GetValue( "reportPath", "reports\\" );
	ConfigForm->EditReports->Text = (const char *)reportPath;
	STRING proxyServer = ConfigDataModule->GetValue( "proxyServer", "" );
	ConfigForm->EditProxyServer->Text = (const char *)proxyServer;
	int proxyPort = ConfigDataModule->GetValue( "proxyPort", 0 );
	ConfigForm->UpDownProxyPort->Position = short(proxyPort);
	STRING noProxy = ConfigDataModule->GetValue( "noProxy", "" );
	ConfigForm->MemoNoProxy->Text = (const char *)noProxy;

	if( ConfigForm->ShowModal() == mrOk )
	{
		externalStorage = ConfigForm->EditExternalStorage->Text.c_str();
		ConfigDataModule->SetValue( "externalStorage", externalStorage );
		THE_FILE::setExternalStorage( externalStorage );

		reportPath = ConfigForm->EditReports->Text.c_str();
		if( !reportPath.isEmpty() && !reportPath.endsWith( DIRECTORY_DELIMITER ) )
			reportPath += DIRECTORY_DELIMITER;

		ConfigDataModule->SetValue( "reportPath", reportPath );

		proxyServer = ConfigForm->EditProxyServer->Text.c_str();
		ConfigDataModule->SetValue( "proxyServer", proxyServer );

		proxyPort = ConfigForm->UpDownProxyPort->Position;
		ConfigDataModule->SetValue( "proxyPort", proxyPort );

		noProxy = ConfigForm->MemoNoProxy->Text.c_str();
		ConfigDataModule->SetValue( "noProxy", noProxy );

		net::HTTPrequest::setProxy( proxyServer, proxyPort, noProxy );
	}
}
//---------------------------------------------------------------------------
const STRING &TDocManMainForm::getMachine( void )
{
	static STRING machine;
	if( machine.isEmpty() )
	{
		char			computerName[MAX_COMPUTERNAME_LENGTH+2];
		unsigned long	size = MAX_COMPUTERNAME_LENGTH+1;

		GetComputerName( computerName, &size );
		machine = computerName;
	}

	return machine;
}

//---------------------------------------------------------------------------
void __fastcall TDocManMainForm::PersonalFolderClick(TObject *)
{
	openItem( getPersonalItem( TYPE_PERSONAL_FOLDER ) );
}
//---------------------------------------------------------------------------

void __fastcall TDocManMainForm::TrashClick(TObject *)
{
	openItem( getPersonalItem( TYPE_PERSONAL_TRASH ) );
}
//---------------------------------------------------------------------------

void __fastcall TDocManMainForm::AdminTaskTypesClick(TObject *)
{
	TaskTypesForm->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TDocManMainForm::AdminTaskStatusClick(TObject *)
{
	TaskStatusForm->ShowModal();
}
//---------------------------------------------------------------------------



void __fastcall TDocManMainForm::AdminCheckDatabaseClick(TObject *)
{
	THREAD_CHECK_DB	*theThread = new THREAD_CHECK_DB;

	theThread->StartThread();
}
//---------------------------------------------------------------------------

void __fastcall TDocManMainForm::FileExitClick(TObject *)
{
	Close();
	Application->Terminate();	
}
//---------------------------------------------------------------------------

void __fastcall TDocManMainForm::OpenFilesClick(TObject *)
{
	ShowWindow( OpenFilesForm );
}
//---------------------------------------------------------------------------

void __fastcall TDocManMainForm::OpenTasksClick(TObject *)
{
	ShowWindow( OpenTasksForm );
}
//---------------------------------------------------------------------------

void __fastcall TDocManMainForm::WorkspaceClick(TObject *)
{
	openItem( getCompanyVolume() );
}
//---------------------------------------------------------------------------

void __fastcall TDocManMainForm::FileLoginClick(TObject *)
{
	login();
	if( !m_actUser->ID )					// sorry
	{
		Application->Terminate();
	}
}
//---------------------------------------------------------------------------

void __fastcall TDocManMainForm::ChangePasswordClick(TObject *)
{
	if( ChangePasswordForm->ShowModal() == mrOk )
	{
		vcl::changePassword(
			theDatabase->DatabaseName,
			ChangePasswordForm->EditOldPassword->Text,
			ChangePasswordForm->EditNewPassword->Text
		);
	}
}
//---------------------------------------------------------------------------

void __fastcall TDocManMainForm::BookmarksClick(TObject *)
{
	openItem( getPersonalItem( TYPE_BOOKMARK_FOLDER ) );
}
//---------------------------------------------------------------------------

void __fastcall TDocManMainForm::LocalDirectories1Click(TObject *)
{
	ShowWindow( LocalDirsForm );
}
//---------------------------------------------------------------------------

void __fastcall TDocManMainForm::AdminMimeTypesClick(TObject *)
{
	MimeTypesForm->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TDocManMainForm::SpeedButtonRefreshClick(TObject *)
{
	browserFrame->fillContents( currentItem, -1 );
}
//---------------------------------------------------------------------------

void __fastcall TDocManMainForm::SpeedButtonBackClick(TObject *)
{
	PTR_ITEM	newItem = history.pop();
	if( newItem )
	{
		forwardHistory.push( currentItem );
		SpeedButtonForward->Enabled = true;
		openItem( newItem, -1, false );
	}
	if( !history.size() )
	{
		SpeedButtonBack->Enabled = false;
	}
}
//---------------------------------------------------------------------------

void __fastcall TDocManMainForm::SpeedButtonForwardClick(TObject *)
{
	PTR_ITEM	newItem = forwardHistory.pop();
	if( newItem )
	{
		history.push( currentItem );
		SpeedButtonBack->Enabled = true;
		openItem( newItem, -1, false );
	}
	if( !forwardHistory.size() )
	{
		SpeedButtonForward->Enabled = false;
	}
}
//---------------------------------------------------------------------------


void __fastcall TDocManMainForm::XSLTFolderClick(TObject *)
{
	openItem( getPublicVolume( TYPE_XSLT_FOLDER ) );
}
//---------------------------------------------------------------------------


void __fastcall TDocManMainForm::Categories1Click(TObject *)
{
	CategoriesForm->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TDocManMainForm::About1Click(TObject *)
{
	AboutOpenSSLForm->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TDocManMainForm::PublicTemplateFolderClick(TObject *)
{
	openItem( getPublicVolume( TYPE_PUBLIC_TEMPLATE_FOLDER ) );
}
//---------------------------------------------------------------------------

void __fastcall TDocManMainForm::PersonalTemplateFolderClick(TObject *)
{
	openItem( getPersonalItem( TYPE_PERSONAL_TEMPLATE_FOLDER ) );
}
//---------------------------------------------------------------------------

void __fastcall TDocManMainForm::Archive1Click(TObject *)
{
	openItem( getArchiveVolume() );
}
//---------------------------------------------------------------------------

void __fastcall TDocManMainForm::RefreshExifs1Click(TObject *)
{
	THREAD_REFRESH_EXIFS	*theThread = new THREAD_REFRESH_EXIFS;

	theThread->StartThread();
}
//---------------------------------------------------------------------------

void __fastcall TDocManMainForm::RefreshMimeTypes1Click(TObject *)
{
	THREAD_REFRESH_MIME_TYPES	*theThread = new THREAD_REFRESH_MIME_TYPES;

	theThread->StartThread();
}
//---------------------------------------------------------------------------
void __fastcall TDocManMainForm::Dispatch(void *Message)
{
	try
	{
		switch( ((PMessage)Message)->Msg )
		{
			case WM_DROPFILES:
				WmDropFiles(*((TWMDropFiles *)Message));
				break;
			default:
				TForm::Dispatch(Message);
				break;
		}
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

void __fastcall TDocManMainForm::MenuOpenTestsClick(TObject *)
{
	ShowWindow( OpenTestsForm );
}
//---------------------------------------------------------------------------

void __fastcall TDocManMainForm::MenuSearchClick(TObject *)
{
	ShowWindow( SearchForm );
}
//---------------------------------------------------------------------------

void __fastcall TDocManMainForm::RemoteUsersClick(TObject *)
{
	RemoteUserForm->ShowModal();	
}
//---------------------------------------------------------------------------


void __fastcall TDocManMainForm::RemoteServerClick(TObject *)
{
	RemoteServerForm->ShowModal();
}
//---------------------------------------------------------------------------

CryptoRSA &TDocManMainForm::getPrivateKey( void )
{
	if( !privateKey.hasKey() )
	{
		if( PasswordForm->ShowModal() == mrOk )
		{
			STRING	txtCypher = PasswordForm->EditPassword->Text.c_str();
			privateKey.loadCryptedPersonalCypher( txtCypher );
		}
		else
		{
			throw EAbort( "Canceled" );
		}
	}

	return privateKey;
}
//---------------------------------------------------------------------------

void __fastcall TDocManMainForm::ChangeDecryptionPasswordClick(
	  TObject *)
{
	if( ChangePasswordForm->ShowModal() == mrOk )
	{
		STRING	oldPassword = ChangePasswordForm->EditOldPassword->Text.c_str();
		STRING	newPassword = ChangePasswordForm->EditNewPassword->Text.c_str();
		if( oldPassword != newPassword )
		{
			CryptoRSA	myPrivateKey;
			if( !oldPassword.isEmpty() )
			{
				myPrivateKey.loadCryptedPersonalCypher( oldPassword );
			}

			if( newPassword.isEmpty() )
			{
				myPrivateKey.savePersonalCypher();
			}
			else
			{
				myPrivateKey.saveCryptedPersonalCypher( newPassword );
			}
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TDocManMainForm::IdexSearch1Click(TObject *)
{
	ShowWindow( IndexForm );
}
//---------------------------------------------------------------------------


void __fastcall TDocManMainForm::AdminReindexDatabaseClick(TObject *)
{
	deleteDocManIndex();
}
//---------------------------------------------------------------------------
