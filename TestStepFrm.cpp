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

#include "TestStepFrm.h"
//---------------------------------------------------------------------------
using namespace gak;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ItemCreateFrm"
#pragma resource "*.dfm"
TTestStepForm *TestStepForm;
//---------------------------------------------------------------------------

class FACTORY_TEST_STEP : public FACTORY_BASE
{
	virtual int getItemType( void ) const;
	virtual bool acceptParent( const THE_ITEM *parent ) const;
	virtual PTR_ITEM createItem( int id ) const;
	virtual const char *getName( void ) const;

	virtual TItemCreateForm *getForm( void ) const;
	virtual PTR_ITEM createItemFromForm( const PTR_ITEM &parent ) const;
	virtual PTR_ITEM createItemFromTemplate(
		const PTR_ITEM &parent,
		const THE_ITEM *src,
		const STRING &newName=""
	) const;
};
static FACTORY_TEST_STEP factoryTestStep;

//---------------------------------------------------------------------------
__fastcall TTestStepForm::TTestStepForm(TComponent* Owner)
	: TItemCreateForm(Owner)
{
}
//---------------------------------------------------------------------------
int FACTORY_TEST_STEP::getItemType( void ) const
{
	return TYPE_TEST_STEP;
}
//---------------------------------------------------------------------------
bool FACTORY_TEST_STEP::acceptParent( const THE_ITEM *parent ) const
{
	return parent->getItemType() == TYPE_TEST_FOLDER;
}
//---------------------------------------------------------------------------
PTR_ITEM FACTORY_TEST_STEP::createItem( int id ) const
{
	return new THE_TEST_STEP( id, this );
}
//---------------------------------------------------------------------------
const char *FACTORY_TEST_STEP::getName( void ) const
{
	return "Test Step";
}
//---------------------------------------------------------------------------
TItemCreateForm *FACTORY_TEST_STEP::getForm( void ) const
{
	return TestStepForm;
}
//---------------------------------------------------------------------------
PTR_ITEM FACTORY_TEST_STEP::createItemFromForm( const PTR_ITEM &parent ) const
{
	PTR_TEST_STEP	newStep = createItem( 0 );

	newStep->setData(
		parent,
		TestStepForm->EditName->Text.c_str(),
		TestStepForm->MemoDescription->Text.c_str(),
		TestStepForm->MemoExpected->Text.c_str(),
		TestStepForm->ComboBoxAssignedTo->Tag
	);
	newStep->updateDatabase();

	return newStep;
}
//---------------------------------------------------------------------------
PTR_ITEM FACTORY_TEST_STEP::createItemFromTemplate(
	const PTR_ITEM &parent,
	const THE_ITEM *src,
	const STRING &newName
) const
{
	PTR_TEST_STEP	newStep;
	const THE_TEST_STEP *stepSrc = dynamic_cast<const THE_TEST_STEP *>(src);
	if( stepSrc )
	{
		newStep = createItem( 0 );
		if( newStep )
		{
			newStep->setData(
				parent,
				newName.isEmpty() ? stepSrc->getName() : newName,
				stepSrc->getDescription(),
				stepSrc->getExpectedResult(),
				stepSrc->getAssignedTo()
			);
			newStep->setCopyID( src->getID() );
			newStep->updateDatabase();
		}
	}

	return newStep;
}
//---------------------------------------------------------------------------
void THE_TEST_STEP::loadFields( TQuery *query )
{
	THE_ITEM::loadFields( query );

	this->expectedResult = query->FieldByName( "expected_result" )->AsString.c_str();
}
//---------------------------------------------------------------------------
void THE_TEST_STEP::updateDatabase( void )
{
	THE_ITEM::updateDatabase();

	std::auto_ptr<TQuery>	theQuery( new TQuery( NULL ) );
	theQuery->DatabaseName = "docManDB";

	theQuery->SQL->Add(
		"update item_tree "
		"set expected_result = :expectedResult "
		"where id=:theId"
	);

	theQuery->Params->Items[0]->AsString = (const char *)expectedResult;
	theQuery->Params->Items[1]->AsInteger = getID();

	theQuery->ExecSQL();
}
//---------------------------------------------------------------------------
TGraphic *THE_TEST_STEP::getItemPicture( void ) const
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

void THE_TEST_STEP::open( void )
{
	TestStepForm->setItem( this );

	if( TestStepForm->ShowModal() == mrOk )
	{
		setName(TestStepForm->EditName->Text.c_str());
		setDescription(TestStepForm->MemoDescription->Text.c_str());
		setAssignedTo( TestStepForm->ComboBoxAssignedTo->Tag );

		expectedResult = TestStepForm->MemoExpected->Text.c_str();

		updateDatabase();
	}
}
//---------------------------------------------------------------------------


void __fastcall TTestStepForm::FormShow(TObject *Sender)
{
	TItemCreateForm::FormShow( Sender );

	PTR_TEST_STEP	theStep = theItem;

	if( !theStep && theParent )
	{
		MemoExpected->Text = "";
		ComboBoxAssignedTo->Tag = 0;
	}
	else if( theStep && !theParent )
	{
		Caption = "Update Test Step";
		MemoExpected->Text = (const char *)theStep->getExpectedResult();
		ComboBoxAssignedTo->Tag = theStep->getAssignedTo();
	}
	else
		throw Exception( "Internal Error" );

	vcl::fillUserList(
		ComboBoxAssignedTo, "docManDB",
		ComboBoxAssignedTo->Tag
			? ComboBoxAssignedTo->Tag
			: DocManMainForm->vcl::getActUser()->ID
	);
}
//---------------------------------------------------------------------------

void __fastcall TTestStepForm::FormClose(TObject *Sender,
	  TCloseAction &Action)
{
	if( ComboBoxAssignedTo->ItemIndex >= 0 )
		ComboBoxAssignedTo->Tag = vcl::getUserByIndex(ComboBoxAssignedTo->ItemIndex);
	else
		ComboBoxAssignedTo->Tag = 0;

	TItemCreateForm::FormClose( Sender, Action );
}
//---------------------------------------------------------------------------

