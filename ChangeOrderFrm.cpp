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

#include "ChangeOrderFrm.h"
#include "ActionManager.h"
#include "Folder.h"
#include "PermissionsFrm.h"
#include "TestFolder.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TChangeOrderForm *ChangeOrderForm;
//---------------------------------------------------------------------------
class ACTION_ORDERING : public ACTION_BASE_PROPERTIES
{
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual RefhreshType perform( PTR_ITEM theItem );
};
//---------------------------------------------------------------------------
static ACTION_ORDERING	theAction;

//---------------------------------------------------------------------------
bool ACTION_ORDERING::acceptItem( THE_ITEM *theItem )
{
	if( !dynamic_cast<THE_FOLDER *>(theItem)
	&&  !dynamic_cast<THE_LANGUAGE_DOC*>(theItem)
	&&  !dynamic_cast<THE_TEST_FOLDER*>(theItem) )
/*@*/	return false;

	int perms = theItem->getUserPermissions();
	return (perms & ITEM_PERM_MODIFY)
		? ACTION_BASE::acceptItem( theItem )
		: false
	;
}
//---------------------------------------------------------------------------
const char *ACTION_ORDERING::getLabel( void ) const
{
	return "Change Order...";
}
//---------------------------------------------------------------------------
RefhreshType ACTION_ORDERING::perform( PTR_ITEM theItem )
{
	int perms = theItem->getUserPermissions();
	if( !perms & ITEM_PERM_MODIFY )
	{
/*@*/	throw Exception( "Permission denied" );
	}

	ChangeOrderForm->ListBoxItems->Items->Clear();
	theItem->sort( SORT_ORDER );

	PTR_ITEM		child;
	ITEM_CONTENT	*theContent = theItem->getContent();
	for(
		ITEM_CONTENT::const_iterator it = theContent->cbegin(),
			endIT = theContent->cend();
		it != endIT;
		++it
	)
	{
		child = *it;
		ChangeOrderForm->ListBoxItems->Items->Add(
			static_cast<const char *>(child->getName())
		);
	}

	switch( ChangeOrderForm->ShowModal() )
	{
	case mrOk:
		for(
			ITEM_CONTENT::const_iterator it = theContent->cbegin(),
				endIT = theContent->cend();
			it != endIT;
			++it
		)
		{
			child = *it;
			child->setOrder(
				ChangeOrderForm->ListBoxItems->Items->IndexOf(
					static_cast<const char *>(child->getName())
				)+1
			);
			child->updateDatabase();
		}

/***/	return rtREDRAW;
	case mrNoToAll:
		for(
			ITEM_CONTENT::const_iterator it = theContent->cbegin(),
				endIT = theContent->cend();
			it != endIT;
			++it
		)
		{
			child = *it;
			child->setOrder( 0 );
			child->updateDatabase();
		}

/***/	return rtREDRAW;
	}

	return rtNONE;
}

//---------------------------------------------------------------------------
__fastcall TChangeOrderForm::TChangeOrderForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TChangeOrderForm::ListBoxItemsEndDrag(TObject *,
	  TObject *, int, int Y )
{
	if( ListBoxItems->ItemIndex >= 0 )
	{
		int		dragOverIndex = Y/ListBoxItems->ItemHeight + ListBoxItems->TopIndex;

		if( dragOverIndex >= 0 && dragOverIndex < ListBoxItems->Items->Count )
			ListBoxItems->Items->Move( ListBoxItems->ItemIndex, dragOverIndex );
	}
}
//---------------------------------------------------------------------------
void __fastcall TChangeOrderForm::ListBoxItemsDragOver(TObject *,
	  TObject *, int , int Y, TDragState , bool &Accept)
{
	if( ListBoxItems->ItemIndex < 0 )
		Accept = false;
	else
	{
		int		dragOverIndex = Y/ListBoxItems->ItemHeight + ListBoxItems->TopIndex;
		if( dragOverIndex < 0 || dragOverIndex >= ListBoxItems->Items->Count )
			Accept = false;
		else
			Accept = true;
	}
}
//---------------------------------------------------------------------------

