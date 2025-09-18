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

#include "ReferencesFrm.h"
#include "ActionManager.h"
#include "File.h"
#include "PermissionsFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
class ACTION_SHOW_REFERENCES : public ACTION_BASE_MOVE
{
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual RefhreshType perform( PTR_ITEM theItem );
};
//---------------------------------------------------------------------------
TReferencesForm *ReferencesForm;
static ACTION_SHOW_REFERENCES theAction;

//---------------------------------------------------------------------------
bool ACTION_SHOW_REFERENCES::acceptItem( THE_ITEM *theItem )
{
	return ACTION_BASE::acceptItem( theItem );;
}
//---------------------------------------------------------------------------
const char *ACTION_SHOW_REFERENCES::getLabel( void ) const
{
	return "Show References...";
}
//---------------------------------------------------------------------------
RefhreshType ACTION_SHOW_REFERENCES::perform( PTR_ITEM theItem )
{
	ReferencesForm->setItem( theItem );
	ReferencesForm->ShowModal();

	return rtNONE;
}
//---------------------------------------------------------------------------
__fastcall TReferencesForm::TReferencesForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TReferencesForm::FormShow(TObject *)
{
	QueryReferences->ParamByName( "theID" )->AsInteger = theItem->getID();
	QueryReferences->Open();
}
//---------------------------------------------------------------------------
void __fastcall TReferencesForm::QueryReferencesCalcFields(TDataSet *)
{
	PTR_ITEM	theItem = getItem( QueryReferencesID->AsInteger );
	if( theItem )
		QueryReferencesPATH->AsString = (const char *)theItem->getPath();
	else
		QueryReferencesPATH->AsString = "???";
}
//---------------------------------------------------------------------------
void __fastcall TReferencesForm::FormClose(TObject *,
	  TCloseAction &)
{
	QueryReferences->Close();
}
//---------------------------------------------------------------------------
