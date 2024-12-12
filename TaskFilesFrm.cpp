//---------------------------------------------------------------------------

#include <vcl.h>

#include <gak/vcl_tools.h>

#include <winlib/winlib.h>

#pragma hdrstop

#include "TaskFilesFrm.h"
#include "ActionManager.h"
#include "TaskList.h"
#include "DocManMain.h"
#include "PermissionsFrm.h"
#include "File.h"

//---------------------------------------------------------------------------
using namespace gak;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TTaskFilesForm *TaskFilesForm;
//---------------------------------------------------------------------------
class ACTION_TASK_FILES : public ACTION_BASE_CHECK
{
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
};
static ACTION_TASK_FILES theAction;

//---------------------------------------------------------------------------
bool ACTION_TASK_FILES::acceptItem( THE_ITEM *theItem )
{
	if( !dynamic_cast<THE_TASK*>( theItem ) )
/*@*/	return false;

	int perms = theItem->getUserPermissions();
	if( !perms & ITEM_PERM_MODIFY )
/*@*/	return false;

	return ACTION_BASE_CHECK::acceptItem( theItem );
}
//---------------------------------------------------------------------------
const char *ACTION_TASK_FILES::getLabel( void ) const
{
	return "Assigned Files...";
}
//---------------------------------------------------------------------------
REFRESH_TYPE ACTION_TASK_FILES::perform( PTR_ITEM theItem )
{
	TaskFilesForm->setItem( theItem );
	TaskFilesForm->ShowModal();

	return REFRESH_NONE;
}

//---------------------------------------------------------------------------
__fastcall TTaskFilesForm::TTaskFilesForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void TTaskFilesForm::gotoID( int ID, TQuery *query, TIntegerField *idField )
{
	for( query->First(); !query->Eof; query->Next() )
		if( idField->AsInteger == ID )
			break;
}
//---------------------------------------------------------------------------
void TTaskFilesForm::gotoRecord( int recNO, TQuery *query )
{
	for( query->First(); !query->Eof; query->Next() )
		if( query->RecNo >= recNO )
			break;
}
//---------------------------------------------------------------------------
void TTaskFilesForm::refresh()
{
	OpenQuery->Close();
	SelectedQuery->Close();
	OpenQuery->Open();
	SelectedQuery->Open();
}
//---------------------------------------------------------------------------
void __fastcall TTaskFilesForm::FormShow(TObject *)
{
	OpenQuery->ParamByName( "actUser" )->AsInteger = vcl::getActUserID();
	OpenQuery->ParamByName( "actMachine" )->AsString = static_cast<const char *>(TDocManMainForm::getMachine());
	OpenQuery->Open();

	SelectedQuery->ParamByName( "actTask" )->AsInteger = theItem->getID();
	SelectedQuery->Open();

	vcl::bringWindowToFront( this );
}
//---------------------------------------------------------------------------
void __fastcall TTaskFilesForm::FormClose(TObject *,
	  TCloseAction &)
{
	SelectedQuery->Close();
	OpenQuery->Close();
}
//---------------------------------------------------------------------------
void __fastcall TTaskFilesForm::ButtonAddClick(TObject *)
{
	if( !OpenQueryID->IsNull )
	{
		int recNO = OpenQuery->RecNo;
		int fileID = OpenQueryID->AsInteger;

		AddQuery->ParamByName( "theTask" )->AsInteger = theItem->getID();
		AddQuery->ParamByName( "theFile" )->AsInteger = fileID;
		AddQuery->ExecSQL();

		SelectedQuery->DisableControls();
		OpenQuery->DisableControls();
		refresh();
		gotoID( fileID, SelectedQuery, SelectedQueryID );
		gotoRecord( recNO, OpenQuery );
		SelectedQuery->EnableControls();
		OpenQuery->EnableControls();
	}
}
//---------------------------------------------------------------------------
void __fastcall TTaskFilesForm::ButtonRemoveClick(TObject *)
{
	if( !SelectedQueryID->IsNull )
	{
		int recNO = SelectedQuery->RecNo;
		int fileID = SelectedQueryID->AsInteger;

		RemoveQuery->ParamByName( "theFile" )->AsInteger = fileID;
		RemoveQuery->ExecSQL();

		SelectedQuery->DisableControls();
		OpenQuery->DisableControls();
		refresh();
		gotoID( fileID, OpenQuery, OpenQueryID );
		gotoRecord( recNO, SelectedQuery );
		SelectedQuery->EnableControls();
		OpenQuery->EnableControls();
	}
}
//---------------------------------------------------------------------------
void __fastcall TTaskFilesForm::ButtonCheckInClick(TObject *)
{
	STRING		description = "Task ";
	PTR_TASK	theTask = theItem;

	if( theTask )
	{
		description += formatNumber( theTask->getID() );
		description += ": ";
		description += theTask->getName();
		description += "\n\n";
		description += theTask->getDescription();
		description += "\n\n";
		description += theTask->getExternalRemarks();
		description += "\n\n";
		description += theTask->getInternalRemarks();
		description.stripBlanks();

		for(
			SelectedQuery->First();
			!SelectedQuery->Eof;
			SelectedQuery->Next()
		)
		{
			PTR_FILE_BASE theFile = ::getItem( SelectedQueryID->AsInteger );
			if( theFile && theFile->canUnreserve( true ) )
			{
				theFile->unreserve( CheckBoxBranch->Checked, description );
				theFile->updateDatabase();
			}
			idleLoop();
		}
		refresh();
	}
}
//---------------------------------------------------------------------------
void __fastcall TTaskFilesForm::SelectedQueryCalcFields(TDataSet *)
{
	SelectedQueryPATH->AsString = static_cast<const char *>(
		DocManDataModule->getPathForID(
			SelectedQueryPARENTID->AsInteger
		) + '\\' + SelectedQueryNAME->AsString.c_str()
	);
}
//---------------------------------------------------------------------------

void __fastcall TTaskFilesForm::OpenQueryCalcFields(TDataSet *)
{
	OpenQueryPATH->AsString = static_cast<const char *>(
		DocManDataModule->getPathForID(
			OpenQueryPARENTID->AsInteger
		) + '\\' + OpenQueryNAME->AsString.c_str()
	);
}
//---------------------------------------------------------------------------

