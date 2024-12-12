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

//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TestStepExecFrm.h"
#include "TestPrepareFrm.h"
#include "TestFolder.h"
#include "DocManDM.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TestStepFrm"
#pragma resource "*.dfm"
TTestStepExecForm *TestStepExecForm;
//---------------------------------------------------------------------------
class FACTORY_TEST_EXEC_STEP : public FACTORY_ROOT_FOLDER
{
	virtual int getItemType( void ) const;
	virtual PTR_ITEM createItem( int id ) const;
	virtual const char *getName( void ) const;
	virtual bool acceptParent( const THE_ITEM *parent ) const;
};
static FACTORY_TEST_EXEC_STEP	factoryTestExecStep;
//---------------------------------------------------------------------------
__fastcall TTestStepExecForm::TTestStepExecForm(TComponent* Owner)
	: TTestStepForm(Owner)
{
}
//---------------------------------------------------------------------------
TGraphic *THE_TEST_EXEC_STEP::getItemPicture( void ) const
{
	static Graphics::TBitmap *thePic = NULL;

	if( !thePic )
	{
		thePic = new Graphics::TBitmap();
		thePic->LoadFromResourceName((unsigned int)HInstance, "TASK_BITMAP");
	}
	return thePic;
}

//---------------------------------------------------------------------------
void THE_TEST_EXEC_STEP::loadFields( TQuery *query )
{
	THE_TEST_STEP::loadFields( query );

	this->actualResult = query->FieldByName( "actual_result" )->AsString.c_str();
	this->testStatus = query->FieldByName( "test_status" )->AsInteger;
}
//---------------------------------------------------------------------------
void THE_TEST_EXEC_STEP::updateDatabase( void )
{
	THE_TEST_STEP::updateDatabase();

	std::auto_ptr<TQuery>	theQuery( new TQuery( NULL ) );
	theQuery->DatabaseName = "docManDB";

	theQuery->SQL->Add(
		"update item_tree "
		"set actual_result = :actualResult, "
			 "test_status = :testStatus "
		"where id=:theId"
	);

	theQuery->Params->Items[0]->AsString = (const char *)actualResult;
	theQuery->Params->Items[1]->AsInteger = testStatus;
	theQuery->Params->Items[2]->AsInteger = getID();

	theQuery->ExecSQL();
}
//---------------------------------------------------------------------------
void THE_TEST_EXEC_STEP::open( void )
{
	int						modalResult;
	PTR_TEST_EXEC_FOLDER	parentItem = getParent();
	ITEM_CONTENT			*theContent = parentItem->getContent( SORT_ORDER );
	int						myIndex = parentItem->getContentIndex( getID() );
	int						numItems = theContent->size();
	PTR_TEST_EXEC_STEP		actualItem = this;
	int 					oldStatus = parentItem->getTestStatus();
	int						newStatus = oldStatus;
	PTR_TEST_FOLDER			testFolder;
	PTR_TEST_STEP			testStep;
	PTR_TASKLIST			taskList;
	PTR_TASK				newTask;
	int						assignedTo;
	STRING					newName, actualResult;

	testFolder = getItem( parentItem->getCopyID() );
	if( testFolder )
		taskList = getItem( testFolder->getTaskListID() );

	while( myIndex < numItems )
	{
		if( actualItem )
		{
			TestStepExecForm->setItem( actualItem );
			modalResult = TestStepExecForm->ShowModal();

			if( modalResult == mrNo || modalResult == mrOk )
			{
				actualResult = TestStepExecForm->MemoActualResult->Text.c_str();
				actualItem->actualResult = actualResult;
				if( modalResult == mrNo )
				{
					if( taskList )
					{
						testStep = getItem( actualItem->getCopyID() );
						if( testStep )
							assignedTo = testStep->getAssignedTo();
						else
							assignedTo = 0;
						newTask = createItem( TYPE_TASK );

						newName = actualItem->getName();
						newName += ' ';
						newName += TDateTime::CurrentDateTime().DateTimeString().c_str();

						(*newTask).setData(
							taskList, newName, actualItem->getDescription(),
							// const STRING &customerRef, const STRING &module,
							"", "",
							//const STRING &badFunction, const STRING &release,
							"", "",
							STRING("Actual Result:\n" ) + actualResult,
							STRING("Expected Result:\n" ) + actualItem->getExpectedResult(),
							// int priority, double estEffort, double actEffort,
							0, 0, 0,
							DocManDataModule->getDefaultTaskType(),
							DocManDataModule->getDefaultTaskStatus(),
							assignedTo,
							0				// completed date
						);
						(*newTask).updateDatabase();
					}
					newStatus = -1;
					actualItem->testStatus = -1;
				}
				else
				{
					actualItem->testStatus = 1;
				}

				actualItem->updateDatabase();

				actualItem = parentItem->getContentItem( ++myIndex );
			}
			else
			{
				break;
			}
		}
	}
	if( myIndex >= numItems && !newStatus )
		newStatus = 1;

	if( newStatus != oldStatus )
	{
		parentItem->setTestStatus( newStatus );
		parentItem->updateDatabase();
	}
}
//---------------------------------------------------------------------------
int FACTORY_TEST_EXEC_STEP::getItemType( void ) const
{
	return TYPE_TEST_EXEC_STEP;
}
//---------------------------------------------------------------------------
PTR_ITEM FACTORY_TEST_EXEC_STEP::createItem( int id ) const
{
	return new THE_TEST_EXEC_STEP( id, this );
}
//---------------------------------------------------------------------------
const char *FACTORY_TEST_EXEC_STEP::getName( void ) const
{
	return "Test Protocol Step";
}
//---------------------------------------------------------------------------
bool FACTORY_TEST_EXEC_STEP::acceptParent( const THE_ITEM *parent ) const
{
	return parent->getItemType() == TYPE_TEST_EXEC_FOLDER;
}
//---------------------------------------------------------------------------

void __fastcall TTestStepExecForm::FormShow(TObject *Sender)
{
	TTestStepForm::FormShow( Sender );

	Caption = "Execute Test Step";

	PTR_TEST_EXEC_STEP	theStep = theItem;

	if( theStep && !theParent )
		MemoActualResult->Text = (const char *)(*theStep).getActualResult();
	else
		throw Exception( "Internal Error" );
}
//---------------------------------------------------------------------------


