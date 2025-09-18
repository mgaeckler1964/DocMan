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

#include "MakeLangFrm.h"
#include "ActionManager.h"
#include "File.h"

//---------------------------------------------------------------------------
using namespace gak;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMakeLanguageForm *MakeLanguageForm;
//---------------------------------------------------------------------------
class ACTION_MAKE_LANGUAGE : public ACTION_BASE_MOVE
{
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual RefhreshType perform( PTR_ITEM theItem );
};
//---------------------------------------------------------------------------
bool ACTION_MAKE_LANGUAGE::acceptItem( THE_ITEM *theItem )
{
	if( !dynamic_cast<THE_FILE *>( theItem )
	&&  !dynamic_cast<THE_LANGUAGE_ITEM *>( theItem ) )
		return false;

	return ACTION_BASE::acceptItem( theItem );
}
//---------------------------------------------------------------------------
const char *ACTION_MAKE_LANGUAGE::getLabel( void ) const
{
	return "Make Language...";
}
//---------------------------------------------------------------------------
RefhreshType ACTION_MAKE_LANGUAGE::perform( PTR_ITEM theItem )
{
	if( MakeLanguageForm->ShowModal() == mrOk )
	{
		PTR_LANGUAGE_ITEM theLangItem = theItem;

		if( theLangItem )
		{
			THE_ITEM::ItemsCopiedMap itemsCopied;
			theItem->copy(
				itemsCopied,
				theItem->getParent(),
				false,
				MakeLanguageForm->EditLanguage->Text.c_str()
			);
		}
		else
		{
			STRING		oldName = theItem->getName();
			STRING		tmpName = "TMP";

			tmpName += formatNumber( theItem->getID() );
			theItem->setName( tmpName );
			theItem->updateDatabase();

			PTR_ITEM newItem = createItem( TYPE_LANGUAGE_DOC );
			newItem->setData(
				theItem->getParent(), oldName, theItem->getDescription()
			);
			newItem->updateDatabase();

			TQuery *theQuery = new TQuery( NULL );
			theQuery->DatabaseName = "docManDb";
			theQuery->SQL->Add(
				"update item_tree "
				"set item_type=:newType, name=:newName, parentId = :newParent "
				"where id = :oldId"
			);
			theQuery->Params->Items[0]->AsInteger = TYPE_LANGUAGE_ITEM;
			theQuery->Params->Items[1]->AsString  = MakeLanguageForm->EditLanguage->Text;
			theQuery->Params->Items[2]->AsInteger = newItem->getID();
			theQuery->Params->Items[3]->AsInteger = theItem->getID();

			theQuery->ExecSQL();

		}

		return rtRELOAD;
	}

	return rtNONE;
}
//---------------------------------------------------------------------------
static ACTION_MAKE_LANGUAGE theAction;
//---------------------------------------------------------------------------
__fastcall TMakeLanguageForm::TMakeLanguageForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
