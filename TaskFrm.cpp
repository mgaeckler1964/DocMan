//---------------------------------------------------------------------------

#include <vcl.h>

#include <gak/sortedArray.h>
#pragma hdrstop

#include "TaskList.h"
#include "TaskFrm.h"
#include "DocManDM.h"
#include "PermissionsFrm.h"

//---------------------------------------------------------------------------
using namespace gak;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ItemCreateFrm"
#pragma resource "*.dfm"
TTaskForm *TaskForm;
//---------------------------------------------------------------------------
struct TASK_USER
{
	int		id;
	STRING	name;
};
//---------------------------------------------------------------------------
static int TaskUserCompare( const TASK_USER &u1, const TASK_USER &u2, int sortType )
{
	if( !sortType )
		return u2.id - u1.id;
	else
		return strcmpi( u1.name, u2.name );
}
//---------------------------------------------------------------------------
static SortedArray<TASK_USER, CompareFunction<TASK_USER,int> > theTaskUser( TaskUserCompare, 0 );
static SortedArray<TASK_USER, CompareFunction<TASK_USER,int> > theTaskGroups( TaskUserCompare, 0 );
//---------------------------------------------------------------------------
static void fillTaskUserByQuery( TQuery *theQuery, int groupPermissions=0 )
{
	size_t		pos;
	TASK_USER	userFound;

	for( theQuery->Open(); !theQuery->Eof; theQuery->Next() )
	{
		int	permissions = groupPermissions ? groupPermissions : theQuery->Fields->Fields[3]->AsInteger;
		if( permissions & ITEM_PERM_MODIFY )
		{
			userFound.id = theQuery->Fields->Fields[0]->AsInteger;
			userFound.name = theQuery->Fields->Fields[1]->AsString.c_str();
			int type = theQuery->Fields->Fields[2]->AsInteger;
			if( type )
			{
				pos = theTaskGroups.findElement( userFound );
				if( pos == (size_t)-1 )
				{
					theTaskGroups.addElement( userFound );

					std::auto_ptr<TQuery>	theQuery( new TQuery( NULL ) );
					theQuery->DatabaseName = "docManDb";

					theQuery->SQL->Add(
						"select u.id, u.username, u.type "
						"from user_tab u, u_member m "
						"where m.member_id = u.id "
						"and m.group_id = :groupId"
					);
					theQuery->Params->Items[0]->AsInteger = userFound.id;
					fillTaskUserByQuery( theQuery.get(), permissions );
				}
			}
			else
			{
				pos = theTaskUser.findElement( userFound );
				if( pos == (size_t)-1 )
					theTaskUser.addElement( userFound );
			}
		}
	}
}

//---------------------------------------------------------------------------
static void fillTaskUserForItem( int itemId )
{
	theTaskUser.clear();
	theTaskUser.resort( 0 );
	theTaskGroups.clear();
	std::auto_ptr<TQuery>	theQuery( new TQuery( NULL ) );
	theQuery->DatabaseName = "docManDb";

	theQuery->SQL->Add(
		"select u.id, u.username, u.type, i.permissions "
		"from user_tab u, i_acls i "
		"where i.user_id = u.id "
		"and i.item_id = :itemId"
	);
	theQuery->Params->Items[0]->AsInteger = itemId;
	fillTaskUserByQuery( theQuery.get() );
}
//---------------------------------------------------------------------------
__fastcall TTaskForm::TTaskForm(TComponent* Owner)
	: TItemCreateForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TTaskForm::FormShow(TObject *Sender)
{
	TItemCreateForm::FormShow( Sender );

	int			parentID;
	THE_TASK	*theTask = dynamic_cast<THE_TASK *>(
		static_cast<THE_ITEM*>(theItem)
	);

	if( !theTask && theParent )
	{
		EditCustomerRef->Text = "";
		ComboBoxModule->Text = "";
		ComboBoxFunction->Text = "";
		EditRelease->Text = "";
		MemoExternalRemarks->Text = "";
		MemoInternalRemarks->Text = "";
		UpDownPriority->Position = 1;
		EditEstEffort->Text = "0.0";
		EditActEffort->Text = "0.0";
		ComboBoxTaskType->Tag = 0;
		ComboBoxTaskStatus->Tag = 0;
		ComboBoxAssignedTo->Tag = 0;
		completed = 0;
		LabelCompleted->Caption = "";
	}
	else if( theTask && !theParent )
	{
		char	buffer[64];

		Caption = "Update Task";
		EditName->Text = (const char *)theTask->getName();
		MemoDescription->Text = (const char *)theTask->getDescription();
		EditCustomerRef->Text = (const char *)theTask->getCustomerRef();
		ComboBoxModule->Text = (const char *)theTask->getModule();
		ComboBoxFunction->Text = (const char *)theTask->getBadFunction();
		EditRelease->Text = (const char *)theTask->getRelease();
		MemoExternalRemarks->Text = (const char *)theTask->getExternalRemarks();
		MemoInternalRemarks->Text = (const char *)theTask->getInternalRemarks();
		UpDownPriority->Position = (short)theTask->getPriority();
		sprintf( buffer, "%.2f", theTask->getEstEffort() );
		EditEstEffort->Text = buffer;
		sprintf( buffer, "%.2f", theTask->getActEffort() );
		EditActEffort->Text = buffer;
		ComboBoxTaskType->Tag = theTask->getTypeID();
		ComboBoxTaskStatus->Tag = theTask->getStatusID();
		ComboBoxAssignedTo->Tag = theTask->getAssignedTo();
		completed = theTask->getCompleted();
		if( (int)completed )
			LabelCompleted->Caption = completed.DateTimeString();
		else
			LabelCompleted->Caption = "";
	}
	else
		throw Exception( "Internal Error" );

	parentID = theParent ? (*theParent).getID() : (*theItem).getParentID();
	std::auto_ptr<TQuery>	theQuery( new TQuery( NULL ) );
	theQuery->DatabaseName = "docManDB";

	ComboBoxModule->Items->Clear();
	theQuery->SQL->Add(
		"select distinct module from item_tree where parentId = :theParent"
	);
	theQuery->Params->Items[0]->AsInteger = parentID;
	for( theQuery->Open(); !theQuery->Eof; theQuery->Next() )
		ComboBoxModule->Items->Add( theQuery->Fields->Fields[0]->AsString );
	theQuery->Close();

	ComboBoxFunction->Items->Clear();
	theQuery->SQL->Clear();
	theQuery->SQL->Add(
		"select distinct bad_function from item_tree where parentId = :theParent"
	);
	theQuery->Params->Items[0]->AsInteger = parentID;
	for( theQuery->Open(); !theQuery->Eof; theQuery->Next() )
		ComboBoxFunction->Items->Add( theQuery->Fields->Fields[0]->AsString );
	theQuery->Close();

	int	i, itemIndex = 0;
	ComboBoxTaskType->Items->Clear();
	for(
		i=0, DocManDataModule->TableTaskTypes->Open();
		!DocManDataModule->TableTaskTypes->Eof;
		i++, DocManDataModule->TableTaskTypes->Next()
	)
	{
		ComboBoxTaskType->Items->Add(
			DocManDataModule->TableTaskTypesTASK_TYPE->AsString
		);
		if( DocManDataModule->TableTaskTypesID->AsInteger == ComboBoxTaskType->Tag )
		{
			itemIndex = i;
		}
	}
	ComboBoxTaskType->ItemIndex = itemIndex;

	itemIndex = 0;
	ComboBoxTaskStatus->Items->Clear();
	for(
		i=0, DocManDataModule->TableTaskStatus->Open();
		!DocManDataModule->TableTaskStatus->Eof;
		i++, DocManDataModule->TableTaskStatus->Next()
	)
	{
		ComboBoxTaskStatus->Items->Add(
			DocManDataModule->TableTaskStatusTASK_STATUS->AsString
		);
		if( DocManDataModule->TableTaskStatusID->AsInteger == ComboBoxTaskStatus->Tag )
		{
			itemIndex = i;
		}
	}
	ComboBoxTaskStatus->ItemIndex = itemIndex;

	ComboBoxAssignedTo->Items->Clear();
	fillTaskUserForItem( theItem ? theItem->getID() : theParent->getID() );
	theTaskUser.resort( 1 );
	for( i=0, itemIndex = -1; i<theTaskUser.size(); i++ )
	{
		const TASK_USER	&user = theTaskUser[i];
		ComboBoxAssignedTo->Items->Add( static_cast<const char *>(user.name) );
		if( ComboBoxAssignedTo->Tag )
		{
			if( ComboBoxAssignedTo->Tag == user.id )
			{
				itemIndex = i;
			}
		}
		else if( user.id == DocManMainForm->getActUser()->ID )
		{
			itemIndex = i;
		}
	}
	ComboBoxAssignedTo->ItemIndex = itemIndex;

	vcl::bringWindowToFront( this );
}
//---------------------------------------------------------------------------

void __fastcall TTaskForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	int	taskTypeID = 0;
	if( ComboBoxTaskType->ItemIndex >= 0 )
	{
		AnsiString taskType =
			ComboBoxTaskType->Items->Strings[
				ComboBoxTaskType->ItemIndex
			]
		;
		for(
			DocManDataModule->TableTaskTypes->First();
			!DocManDataModule->TableTaskTypes->Eof;
			DocManDataModule->TableTaskTypes->Next()
		)
		{
			if( taskType == DocManDataModule->TableTaskTypesTASK_TYPE->AsString )
			{
				taskTypeID = DocManDataModule->TableTaskTypesID->AsInteger;
				break;
			}
		}
	}
	ComboBoxTaskType->Tag = taskTypeID;

	int	taskStatusID = 0;
	if( ComboBoxTaskStatus->ItemIndex >= 0 )
	{
		AnsiString taskStatus =
			ComboBoxTaskStatus->Items->Strings[
				ComboBoxTaskStatus->ItemIndex
			]
		;
		for(
			DocManDataModule->TableTaskStatus->First();
			!DocManDataModule->TableTaskStatus->Eof;
			DocManDataModule->TableTaskStatus->Next()
		)
		{
			if( taskStatus == DocManDataModule->TableTaskStatusTASK_STATUS->AsString )
			{
				taskStatusID = DocManDataModule->TableTaskStatusID->AsInteger;
				if( !DocManDataModule->TableTaskStatusCOMPLETED->AsInteger )
					completed = 0;
				else if( !(int)completed )
				{
					if( theItem
					&& !(theItem->getUserPermissions() & ITEM_PERM_COMPLETE_TASK) )
					{
						Action = caNone;
						Application->MessageBox(
							"Permission Denied to complete task", "Error",
							MB_ICONERROR
						);

/*@*/					return;
					}

					completed = TDateTime::CurrentDateTime();
				}
				break;
			}
		}
	}
	ComboBoxTaskStatus->Tag = taskStatusID;
	if( ComboBoxAssignedTo->ItemIndex >= 0 )
		ComboBoxAssignedTo->Tag = theTaskUser[ComboBoxAssignedTo->ItemIndex].id;
	else
		ComboBoxAssignedTo->Tag = 0;

	DocManDataModule->TableTaskTypes->Close();
	DocManDataModule->TableTaskStatus->Close();

	TItemCreateForm::FormClose( Sender, Action );
}
//---------------------------------------------------------------------------


