//---------------------------------------------------------------------------

#include <vcl.h>

#include <gak/vcl_tools.h>

#pragma hdrstop

#include "TestFolder.h"
#include "TestStepFrm.h"
#include "TestPrepareFrm.h"
#include "ActionManager.h"
#include "DocManMain.h"
#include "TargetBrowseFrm.h"
#include "ItemManager.h"
#include "Folder.h"
#include "TestStepExecFrm.h"

//---------------------------------------------------------------------------
using namespace gak;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TTestPrepareForm *TestPrepareForm;
//---------------------------------------------------------------------------
class FACTORY_TEST_EXEC_FOLDER : public FACTORY_ROOT_FOLDER
{
	virtual int getItemType( void ) const;
	virtual PTR_ITEM createItem( int id ) const;
	virtual const char *getName( void ) const;
	bool acceptParent( const THE_ITEM *parent ) const;
};
static FACTORY_TEST_EXEC_FOLDER	factoryTestExecFolder;

//---------------------------------------------------------------------------
class ACTION_TEST_PREPARE : ACTION_BASE_CHECK
{
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
};

static ACTION_TEST_PREPARE	actionTestPrepare;

//---------------------------------------------------------------------------
class ACTION_OPEN : ACTION_BASE_PROPERTIES
{
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
};

static ACTION_OPEN	actionOpen;

//---------------------------------------------------------------------------

int THE_TEST_EXEC_FOLDER::itemCompare( const PTR_ITEM &e1, const PTR_ITEM &e2, int theSort )
{
	PTR_TEST_EXEC_STEP	step1(e1);
	PTR_TEST_EXEC_STEP	step2(e2);

	int	compareResult = 0;

	if( abs( theSort ) == SORT_STATUS )
	{
		int status1 = step1 ? (*step1).getTestStatus() : 0;
		int status2 = step2 ? (*step2).getTestStatus() : 0;

		compareResult = status1 - status2;
	}

	if( !compareResult )
		return THE_ITEM::itemCompare( e1, e2, theSort );

	return theSort < 0 ? -compareResult : compareResult;
}

//---------------------------------------------------------------------------
void THE_TEST_EXEC_FOLDER::loadFields( TQuery *query )
{
	THE_ITEM::loadFields( query );

	this->testStatus = query->FieldByName( "test_status" )->AsInteger;
	this->taskListID = query->FieldByName( "task_list" )->AsInteger;
}
//---------------------------------------------------------------------------
void THE_TEST_EXEC_FOLDER::updateDatabase( void )
{
	THE_ITEM::updateDatabase();

	std::auto_ptr<TQuery>	theQuery( new TQuery( NULL ) );
	theQuery->DatabaseName = "docManDB";

	theQuery->SQL->Add(
		"update item_tree "
		"set test_status = :testStatus, "
			"task_list = :taskListID "
		"where id=:theId"
	);

	theQuery->Params->Items[0]->AsInteger = testStatus;
	theQuery->Params->Items[1]->AsInteger = taskListID;
	theQuery->Params->Items[2]->AsInteger = getID();

	theQuery->ExecSQL();
}
//---------------------------------------------------------------------------
void THE_TEST_EXEC_FOLDER::open( void )
{
	bool					found = false;
	PTR_TEST_EXEC_STEP		actualItem;
	ITEM_CONTENT			*theContent = getContent( SORT_ORDER );

	for( std::size_t i=0; i<theContent->size(); i++ )
	{
		actualItem = getContentItem( i );
		if( !actualItem->getTestStatus() )
		{
			found = true;
			actualItem->open();
/*v*/		break;
		}
	}

	if( !found )
	{
		THE_ITEM::open();
	}
}
//---------------------------------------------------------------------------
TGraphic *THE_TEST_EXEC_FOLDER::getItemPicture( void ) const
{
	static Graphics::TBitmap *thePic = NULL;

	if( !thePic )
	{
		thePic = new Graphics::TBitmap();
		thePic->LoadFromResourceName((unsigned int)HInstance, "TEST_EXEC_FOLDER_BITMAP");
	}
	return thePic;
}

//---------------------------------------------------------------------------
int FACTORY_TEST_EXEC_FOLDER::getItemType( void ) const
{
	return TYPE_TEST_EXEC_FOLDER;
}
//---------------------------------------------------------------------------
PTR_ITEM FACTORY_TEST_EXEC_FOLDER::createItem( int id ) const
{
	return new THE_TEST_EXEC_FOLDER( id, this );
}
//---------------------------------------------------------------------------
const char *FACTORY_TEST_EXEC_FOLDER::getName( void ) const
{
	return "Test Protocol";
}
//---------------------------------------------------------------------------
bool FACTORY_TEST_EXEC_FOLDER::acceptParent( const THE_ITEM *parent ) const
{
	const THE_FOLDER *theFolder = dynamic_cast<const THE_FOLDER *>(parent);
	if( theFolder )
/***/	return true;

	const THE_TEST_FOLDER *theTestFolder = dynamic_cast<const THE_TEST_FOLDER *>(parent);

	return theTestFolder ? true : false;
}
//---------------------------------------------------------------------------
bool ACTION_TEST_PREPARE::acceptItem( THE_ITEM *theItem )
{
	if( theItem->getItemType() == TYPE_TEST_FOLDER )
		return ACTION_BASE_CHECK::acceptItem( theItem );
	else
		return false;
}
//---------------------------------------------------------------------------
const char *ACTION_TEST_PREPARE::getLabel( void ) const
{
	return "Prepare Test...";
}
//---------------------------------------------------------------------------
REFRESH_TYPE ACTION_TEST_PREPARE::perform( PTR_ITEM theItem )
{
	PTR_TEST_FOLDER	theTestFolder = theItem;
	if( theTestFolder )
	{
		int				oldAssignedTo = theTestFolder->getAssignedTo();
		int				oldTaskListID = theTestFolder->getTaskListID();
		PTR_TASKLIST	oldTaskList;

		TestPrepareForm->setAssignedTo( oldAssignedTo );
		if( oldTaskListID )
			oldTaskList = getItem( oldTaskListID );

		TestPrepareForm->setTaskList( oldTaskList );

		if( TestPrepareForm->ShowModal() == mrOk )
		{
			PTR_TEST_EXEC_FOLDER testExecFolder = createItem(
				TYPE_TEST_EXEC_FOLDER
			);
			if( testExecFolder )
			{
				int			assignedTo = TestPrepareForm->getAssignedTo();
				int			taskListID = TestPrepareForm->getTaskListID();

				theTestFolder->setAssignedTo( assignedTo );
				theTestFolder->setTaskList( taskListID );
				theTestFolder->updateDatabase();

				STRING		newName = theTestFolder->getName();
				newName += ' ';
				newName += TDateTime::CurrentDateTime().DateTimeString().c_str();
				testExecFolder->setData(
					theTestFolder->getParent(),
					newName, "",
					assignedTo,
					taskListID
				);
				testExecFolder->setCopyID( theTestFolder->getID() );
				testExecFolder->updateDatabase();

				ITEM_CONTENT *theContent = theTestFolder->getContent();
				for( size_t i=0; i<theContent->size(); i++ )
				{
					PTR_TEST_STEP theStep = theTestFolder->getContentItem( i );
					if( theStep )
					{
						PTR_TEST_EXEC_STEP theExecStep = createItem(
							TYPE_TEST_EXEC_STEP
						);

						if( theExecStep )
						{
							theExecStep->setData(
								testExecFolder,
								theStep->getName(),
								theStep->getDescription(),
								theStep->getExpectedResult(),
								assignedTo,
								theStep->getOrder(),
								theStep->getID()
							);
							(*theExecStep).updateDatabase();
						}
					}

				}
			}
		}
		return REFRESH_RELOAD;
	}

	return REFRESH_NONE;
}
//---------------------------------------------------------------------------
bool ACTION_OPEN::acceptItem( THE_ITEM *theItem )
{
	if( theItem->getItemType() == TYPE_TEST_EXEC_FOLDER )
		return ACTION_BASE_PROPERTIES::acceptItem( theItem );
	else
		return false;
}
//---------------------------------------------------------------------------
const char *ACTION_OPEN::getLabel( void ) const
{
	return "Browse";
}
//---------------------------------------------------------------------------
REFRESH_TYPE ACTION_OPEN::perform( PTR_ITEM theItem )
{
	theItem->THE_ITEM::open();
	return REFRESH_NONE;
}
//---------------------------------------------------------------------------
__fastcall TTestPrepareForm::TTestPrepareForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TTestPrepareForm::FormShow(TObject *)
{
	vcl::fillUserList(
		ComboBoxAssignedTo, "docManDB",
		ComboBoxAssignedTo->Tag
			? ComboBoxAssignedTo->Tag
			: DocManMainForm->getActUser()->ID
	);
}
//---------------------------------------------------------------------------

void __fastcall TTestPrepareForm::ButtonBrowseClick(TObject *)
{
	TargetBrowseForm->setItem( PTR_ITEM(), TYPE_TASKLIST );
	if( TargetBrowseForm->ShowModal() == mrOk )
	{
		TTreeNode *selected = TargetBrowseForm->TreeView->Selected;
		if( selected )
		{
			theTaskList = getItem( (int)selected->Data );
			STRING		Text;
			while( selected )
			{
				if( selected->Parent )	// ignore the root
					Text = (STRING)"\\" + selected->Text.c_str() + Text;
				selected = selected->Parent;
			}
			if( Text.isEmpty() )
				Text = "<root>";
			EditTaskList->Text = (const char*)Text;
		}
	}
}
//---------------------------------------------------------------------------

int THE_TEST_EXEC_FOLDER::getColCount( void ) const
{
	return 6;
}

int *THE_TEST_EXEC_FOLDER::getColWidth( void ) const
{
	static int colWidth[] =
	{
		50,
		50,
		0,
		120,
		120,
		80,
	};
	return colWidth;
}

COL_TITLE *THE_TEST_EXEC_FOLDER::getColumnTitles( void ) const
{
	static COL_TITLE	colTitles[] =
	{
		{ "Order", SORT_ORDER },
		{ "Type", SORT_TYPE },
		{ "Name", SORT_NAME },
		{ "Modified", SORT_DATE },
		{ "Assigned To", SORT_ASSIGNED },
		{ "Status", SORT_STATUS },
	};

	return colTitles;
}

void THE_TEST_EXEC_FOLDER::sort( void )
{
	getContent()->sort( itemCompare, getSortType() );
}

STRING THE_TEST_EXEC_FOLDER::drawCell( int col, int row, TCanvas *canvas, TRect &Rect )
{
	STRING text;

	if( col == 4 )
	{
		PTR_ITEM	theItem = getContentItem( row );
		if( theItem )
			text = theItem->getAssignedToUserName();
	}
	else if( col == 5 )
	{
		PTR_TEST_EXEC_STEP	theItem = getContentItem( row );
		if( theItem )
			text = theItem->getStatus();
	}
	else if( col == 3 )
		text = THE_ITEM::drawCell( 4, row, canvas, Rect );
	else
		text = THE_ITEM::drawCell( col, row, canvas, Rect );

	return text;
}


