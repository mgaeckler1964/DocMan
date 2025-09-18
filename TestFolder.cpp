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

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include "TestFolder.h"
#include "Folder.h"
#include "ItemCreateFrm.h"
#include "TestPrepareFrm.h"

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT-
#	pragma option -b
#	ifdef __WIN32__
#		pragma option -a4
#		pragma option -pc
#	else
#		pragma option -po
#		pragma option -a2
#	endif
#endif

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

class FACTORY_TEST_FOLDER : public FACTORY_BASE
{
	virtual bool acceptParent( const THE_ITEM *parent ) const;
	virtual int getItemType( void ) const;
	virtual PTR_ITEM createItem( int id ) const;
	virtual const char *getName( void ) const;

	virtual TItemCreateForm *FACTORY_TEST_FOLDER::getForm( void ) const;
	PTR_ITEM createItemFromForm( const PTR_ITEM &parent ) const;
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static FACTORY_TEST_FOLDER	factoryTestFolder;

// --------------------------------------------------------------------- //
// ----- class static data --------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- prototypes ---------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module functions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class inlines ------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class constructors/destructors -------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class static functions ---------------------------------------- //
// --------------------------------------------------------------------- //

int THE_TEST_FOLDER::itemCompare( const PTR_ITEM &e1, const PTR_ITEM &e2, int theSort )
{
	PTR_TEST_EXEC_FOLDER	step1(e1);
	PTR_TEST_EXEC_FOLDER	step2(e2);

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

// --------------------------------------------------------------------- //
// ----- class privates ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //
   
bool FACTORY_TEST_FOLDER::acceptParent( const THE_ITEM *parent ) const
{
	bool		ok;
	PTR_FOLDER	theFolder( const_cast<THE_ITEM *>(parent) );

	if( theFolder )
		ok = true;
	else
	{
		PTR_TEST_FOLDER theTestFolder( const_cast<THE_ITEM *>(parent) );
		if( theTestFolder )
			ok = true;
		else
			ok = false;
	}
	return ok;
}

TItemCreateForm *FACTORY_TEST_FOLDER::getForm( void ) const
{
	return ItemCreateForm;
}

int FACTORY_TEST_FOLDER::getItemType( void ) const
{
	return TYPE_TEST_FOLDER;
}
PTR_ITEM FACTORY_TEST_FOLDER::createItem( int id ) const
{
	return new THE_TEST_FOLDER( id, this );
}
const char *FACTORY_TEST_FOLDER::getName( void ) const
{
	return "Test Folder";
}

PTR_ITEM FACTORY_TEST_FOLDER::createItemFromForm( const PTR_ITEM &parent ) const
{
	PTR_ITEM	newFolder( createItem( 0 ) );

	(*newFolder).setData(
		parent,
		ItemCreateForm->EditName->Text.c_str(),
		ItemCreateForm->MemoDescription->Text.c_str()
	);
	(*newFolder).updateDatabase();

	return newFolder;
}

TGraphic *THE_TEST_FOLDER::getItemPicture( void ) const
{
	static Graphics::TBitmap *thePic = NULL;

	if( !thePic )
	{
		thePic = new Graphics::TBitmap();
		thePic->LoadFromResourceName((unsigned int)HInstance, "TEST_FOLDER_BITMAP");
	}
	return thePic;
}

int THE_TEST_FOLDER::getColCount( void ) const
{
	return 6;
}

int *THE_TEST_FOLDER::getColWidth( void ) const
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

ColumnTitle *THE_TEST_FOLDER::getColumnTitles( void ) const
{
	static ColumnTitle colTitles[] =
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

void THE_TEST_FOLDER::sort( void )
{
	getContent()->sort( itemCompare, getSortType() );
}

STRING THE_TEST_FOLDER::drawCell( int col, int row, TCanvas *canvas, TRect &Rect )
{
	STRING text;

	if( col == 4 )
	{
		PTR_ITEM	theItem = getContentItem( row );
		if( theItem )
			text = (*theItem).getAssignedToUserName();
	}
	else if( col == 5 )
	{
		PTR_TEST_EXEC_FOLDER	theItem = getContentItem( row );
		if( theItem )
			text = (*theItem).getStatus();
	}
	else if( col == 3 )
		text = THE_ITEM::drawCell( 4, row, canvas, Rect );
	else
		text = THE_ITEM::drawCell( col, row, canvas, Rect );

	return text;
}

void THE_TEST_FOLDER::loadFields( TQuery *query )
{
	THE_ITEM::loadFields( query );

	this->taskListID = query->FieldByName( "task_list" )->AsInteger;
}

void THE_TEST_FOLDER::updateDatabase( void )
{
	THE_ITEM::updateDatabase();

	std::auto_ptr<TQuery>	theQuery( new TQuery( NULL ) );
	theQuery->DatabaseName = "docManDB";

	theQuery->SQL->Add(
		"update item_tree "
		"set task_list = :taskListID "
		"where id=:theId"
	);

	theQuery->Params->Items[0]->AsInteger = taskListID;
	theQuery->Params->Items[1]->AsInteger = getID();

	theQuery->ExecSQL();
}
//---------------------------------------------------------------------------
// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

