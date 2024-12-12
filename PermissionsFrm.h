//---------------------------------------------------------------------------

#ifndef PermissionsFrmH
#define PermissionsFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBTables.hpp>

#include "ItemManager.h"
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <DBCtrls.hpp>
#include <ExtCtrls.hpp>

//---------------------------------------------------------------------------
const int ITEM_PERM_NONE			=   0;
const int ITEM_PERM_SEE				=   1;
const int ITEM_PERM_MODIFY			=   2;
const int ITEM_PERM_MOVE			=   4;
const int ITEM_PERM_ADD_VERSION		=   8;
const int ITEM_PERM_ADD_ITEMS		=  16;
const int ITEM_PERM_EDIT_PERMS		=  32;
const int ITEM_PERM_DELETE_VERSION	=  64;
const int ITEM_PERM_DELETE			= 128;
const int ITEM_PERM_COMPLETE_TASK	= 256; 

//---------------------------------------------------------------------------

class TPermissionsForm : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
	TTable *TableACLs;
	TDataSource *DataSourceACLs;
	TDBGrid *DBGrid1;
	TIntegerField *TableACLsITEM_ID;
	TIntegerField *TableACLsUSER_ID;
	TIntegerField *TableACLsPERMISSIONS;
	TQuery *QueryAll;
	TStringField *TableACLsUSERNAME;
	TDBNavigator *DBNavigator;
	TCheckBox *CheckBoxSeeContents;
	TCheckBox *CheckBoxModify;
	TCheckBox *CheckBoxAddVersions;
	TCheckBox *CheckBoxAddItems;
	TCheckBox *CheckBoxDeleteVersions;
	TCheckBox *CheckBoxEditPermissions;
	TCheckBox *CheckBoxMove;
	TCheckBox *CheckBoxDelete;
	TCheckBox *CheckBoxComplete;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall TableACLsAfterInsert(TDataSet *DataSet);
	void __fastcall PermClicked(TObject *Sender);
	void __fastcall TableACLsAfterScroll(TDataSet *DataSet);
	void __fastcall TableACLsBeforePost(TDataSet *DataSet);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// Anwender-Deklarationen
	THE_ITEM	*theItem;
public:		// Anwender-Deklarationen
	void setItem( THE_ITEM *theItem )
	{
		this->theItem = theItem;
	}
	__fastcall TPermissionsForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TPermissionsForm *PermissionsForm;
//---------------------------------------------------------------------------
#endif
