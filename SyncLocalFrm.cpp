//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SyncLocalFrm.h"
#include "ActionManager.h"
#include "Folder.h"
#include "PermissionsFrm.h"
#include "DirSelFrm.h"
#include "DocManMain.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSyncLocalForm *SyncLocalForm;
//---------------------------------------------------------------------------
class ACTION_SYNCED_LOCAL_PATH : public ACTION_BASE_PROPERTIES
{
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
};
//---------------------------------------------------------------------------
bool ACTION_SYNCED_LOCAL_PATH::acceptItem( THE_ITEM *theItem )
{
	THE_LOCAL_FOLDER	*theFolder = dynamic_cast<THE_LOCAL_FOLDER *>(theItem);

	if( theFolder )
	{
		int perms = theFolder->getUserPermissions();
		return perms & ITEM_PERM_MODIFY ? ACTION_BASE::acceptItem( theItem ) : false;
	}
	return false;
}
//---------------------------------------------------------------------------
REFRESH_TYPE ACTION_SYNCED_LOCAL_PATH::perform( PTR_ITEM theItem )
{
	THE_LOCAL_FOLDER	*theFolder = dynamic_cast<THE_LOCAL_FOLDER *>(
		static_cast<THE_ITEM*>( theItem )
	);

	if( theFolder )
	{
		STRING machine = TDocManMainForm::getMachine();
		STRING oldPath = theFolder->getLocalPath();
		SyncLocalForm->LabelMachine->Caption = (const char *)machine;
		SyncLocalForm->EditLocalPath->Text = (const char *)oldPath;

		if( SyncLocalForm->ShowModal() == mrOk )
		{
			STRING newPath = SyncLocalForm->EditLocalPath->Text.c_str();
			if( oldPath != newPath )
			{
				theFolder->setLocalPath( newPath );
				theFolder->updateDatabase();

				return REFRESH_RELOAD;
			}
		}
	}

	return REFRESH_NONE;
}
//---------------------------------------------------------------------------
const char *ACTION_SYNCED_LOCAL_PATH::getLabel( void ) const
{
	return "Local Path...";
}
//---------------------------------------------------------------------------
static ACTION_SYNCED_LOCAL_PATH theAction;
//---------------------------------------------------------------------------
__fastcall TSyncLocalForm::TSyncLocalForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TSyncLocalForm::SpeedButtonClick(TObject *)
{
	if( DirectorySelector->ShowModal( EditLocalPath->Text ) == mrOk )
		EditLocalPath->Text = DirectorySelector->DirectoryListBox->Directory;
}
//---------------------------------------------------------------------------

