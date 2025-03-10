/*
		Project:		DocMan
		Module:			
		Description:	
		Author:			Martin G�ckler
		Address:		Hofmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2024 Martin G�ckler

		This program is free software: you can redistribute it and/or modify  
		it under the terms of the GNU General Public License as published by  
		the Free Software Foundation, version 3.

		You should have received a copy of the GNU General Public License 
		along with this program. If not, see <http://www.gnu.org/licenses/>.

		THIS SOFTWARE IS PROVIDED BY Martin G�ckler, Austria, Linz ``AS IS''
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

#ifndef DocManMainH
#define DocManMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <Grids.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>

#include <gak/array.h>
#include <gak/rsa.h>
#include <gak/vcl_tools.h>

#include <Grids.hpp>
#include <Buttons.hpp>
#include <Menus.hpp>

#include "ItemManager.h"

#include "gaklib.h"

//---------------------------------------------------------------------------
typedef Array<TTabSheet*>	TTabSheets;

class FACTORY_BASE;
class ACTION_BASE;

class TBrowserFrame;
class TDocManMainForm : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TComboBox *ComboBoxAddItemTypes;
	TSpeedButton *SpeedButtonUp;
	TComboBox *ComboBoxParents;
	TMainMenu *MainMenu;
	TMenuItem *File;
	TMenuItem *FileUser;
	TPopupMenu *PopupMenuActions;
	TMenuItem *test1;
	TLabel *LabelAddItem;
	TSpeedButton *SpeedButtonPopup;
	TMenuItem *AdminConfigure;
	TMenuItem *Personal;
	TMenuItem *PersonalFolder;
	TMenuItem *Trash;
	TMenuItem *Administration;
	TMenuItem *AdminTaskTypes;
	TMenuItem *N1;
	TMenuItem *FileExit;
	TMenuItem *AdminTaskStatus;
	TMenuItem *AdminCheckDatabase;
	TMenuItem *OpenFiles;
	TMenuItem *OpenTasks;
	TMenuItem *Company1;
	TMenuItem *Workspace;
	TMenuItem *FileLogin;
	TMenuItem *N2;
	TMenuItem *ChangePassword;
	TMenuItem *N3;
	TMenuItem *Bookmarks;
	TMenuItem *LocalDirectories1;
	TMenuItem *AdminMimeTypes;
	TSpeedButton *SpeedButtonRefresh;
	TSpeedButton *SpeedButtonBack;
	TSpeedButton *SpeedButtonForward;
	TMenuItem *XSLTFolder;
	TStatusBar *StatusBar;
	TMenuItem *N5;
	TMenuItem *N6;
	TMenuItem *Categories1;
	TMenuItem *Help1;
	TMenuItem *About1;
	TMenuItem *PublicTemplateFolder;
	TMenuItem *PersonalTemplateFolder;
	TMenuItem *MenuReports;
	TMenuItem *Archive1;
	TMenuItem *RefreshExifs1;
	TMenuItem *N7;
	TMenuItem *N8;
	TMenuItem *N9;
	TMenuItem *RefreshMimeTypes1;
	TMenuItem *MenuOpenTests;
	TMenuItem *MenuSearch;
	TMenuItem *RemoteUsers;
	TMenuItem *RemoteServer;
	TMenuItem *ChangeDecryptionPassword;
	TMenuItem *IdexSearch1;
	TMenuItem *AdminReindexDatabase;
	TMenuItem *Reminder1;
	TMenuItem *N4;
	TTimer *ReminderTimer;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall ComboBoxAddItemTypesChange(TObject *Sender);
	void __fastcall SpeedButtonUpClick(TObject *Sender);
	void __fastcall ComboBoxParentsChange(TObject *Sender);
	void __fastcall FileUserClick(TObject *Sender);
	void __fastcall ActionClick(TObject *Sender);
	void __fastcall SpeedButtonPopupClick(TObject *Sender);
	void __fastcall AdminConfigureClick(TObject *Sender);
	void __fastcall PersonalFolderClick(TObject *Sender);
	void __fastcall TrashClick(TObject *Sender);
	void __fastcall AdminTaskTypesClick(TObject *Sender);
	void __fastcall AdminTaskStatusClick(TObject *Sender);
	void __fastcall AdminCheckDatabaseClick(TObject *Sender);
	void __fastcall FileExitClick(TObject *Sender);
	void __fastcall OpenFilesClick(TObject *Sender);
	void __fastcall OpenTasksClick(TObject *Sender);
	void __fastcall WorkspaceClick(TObject *Sender);
	void __fastcall FileLoginClick(TObject *Sender);
	void __fastcall ChangePasswordClick(TObject *Sender);
	void __fastcall BookmarksClick(TObject *Sender);
	void __fastcall LocalDirectories1Click(TObject *Sender);
	void __fastcall AdminMimeTypesClick(TObject *Sender);
	void __fastcall SpeedButtonRefreshClick(TObject *Sender);
	void __fastcall SpeedButtonBackClick(TObject *Sender);
	void __fastcall SpeedButtonForwardClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall XSLTFolderClick(TObject *Sender);
	void __fastcall Categories1Click(TObject *Sender);
	void __fastcall About1Click(TObject *Sender);
	void __fastcall PublicTemplateFolderClick(TObject *Sender);
	void __fastcall PersonalTemplateFolderClick(TObject *Sender);
	void __fastcall Archive1Click(TObject *Sender);
	void __fastcall RefreshExifs1Click(TObject *Sender);
	void __fastcall RefreshMimeTypes1Click(TObject *Sender);
	void __fastcall MenuOpenTestsClick(TObject *Sender);
	void __fastcall MenuSearchClick(TObject *Sender);
	void __fastcall RemoteUsersClick(TObject *Sender);
	void __fastcall RemoteServerClick(TObject *Sender);
	void __fastcall ChangeDecryptionPasswordClick(TObject *Sender);
	void __fastcall IdexSearch1Click(TObject *Sender);
	void __fastcall AdminReindexDatabaseClick(TObject *Sender);
	void __fastcall Reminder1Click(TObject *Sender);
	void __fastcall ReminderTimerTimer(TObject *Sender);
private:	// User declarations
	typedef Array<const FACTORY_BASE *>	AddItemTypes;
	typedef Array<ACTION_BASE *>		PopupActions;

	TBrowserFrame			*browserFrame;
	PTR_ITEM				currentItem,
							selectedItem;
	AddItemTypes			m_addItemTypes;
	PopupActions			m_popupActions;
	gak::Stack<PTR_ITEM>	history, forwardHistory;

	void fillCreateItems( void );
	void fillParentItems( void );
	void fillContents( int selectID );
	const UserOrGroup *login( void );
	void __fastcall ReportClick(TObject *Sender);
	void __fastcall WmDropFiles(TWMDropFiles& Message);
	void __fastcall AppWindowProc(TMessage &msg);
	void __fastcall AppMinimize(TObject *Sender);

	void initMenu( void );
public:		// User declarations
	void openItem( PTR_ITEM newItem, int selectID=-1, bool handleHistory=true );
	__fastcall TDocManMainForm(TComponent* Owner);

	virtual void __fastcall Dispatch(void *Message);
	void ShowWindow( TForm *window )
	{
		window->Show();

		SetWindowPos( Handle, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE	);
		gak::vcl::bringWindowToFront( window );
	}
};

//---------------------------------------------------------------------------
extern PACKAGE TDocManMainForm *DocManMainForm;
//---------------------------------------------------------------------------
#endif
