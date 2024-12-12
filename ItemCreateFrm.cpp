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

#include <gak/fmtNumber.h>
#include <gak/vcl_tools.h>

#pragma hdrstop

#include "ItemCreateFrm.h"
#include "ItemManager.h"
#include "PermissionsFrm.h"

//---------------------------------------------------------------------------
using namespace gak;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TItemCreateForm *ItemCreateForm;
//---------------------------------------------------------------------------
__fastcall TItemCreateForm::TItemCreateForm(TComponent* Owner)
	: TForm(Owner)
{
	theItem = theParent = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TItemCreateForm::FormShow(TObject *)
{
	bool	enableControls;

	if( theItem )
	{
		STRING			name = theItem->getName();
		AnsiString		newCaption = "Update ";
		newCaption += (const char *)name;

		Caption = newCaption;

		EditName->Text = (const char *)name;
		MemoDescription->Text = (const char *)theItem->getDescription();
	}
	else
	{
		FACTORY_BASE	*childFactory = (FACTORY_BASE *)Tag;
		if( childFactory )
		{
			AnsiString		newCaption = "Create ";
			newCaption += childFactory->getName();

			Caption = newCaption;

			STRING	newName = "New ";
			newName += childFactory->getName();

			if( theParent )
			{
				STRING	tmpName = newName;
				int		i=1;
				while( theParent->getContentItem( tmpName ) )
				{
					tmpName = newName;
					tmpName += formatNumber( ++i );
				}
				newName = tmpName;
			}

			EditName->Text = (const char *)newName;
		}
		else
		{
			EditName->Text = "";
			Caption = "Create Item";
		}

		MemoDescription->Text = "";
	}

	if( !theItem || (theItem->loadPermissions() & ITEM_PERM_MODIFY) )
	{
		ButtonCancel->Visible = true;
		ButtonOk->ModalResult = mrOk;

		enableControls = true;
	}
	else
	{
		ButtonCancel->Visible = false;
		ButtonOk->ModalResult = mrCancel;

		enableControls = false;
	}

	int 		numControls = ControlCount;
	for( size_t i=0; i<numControls; i++ )
	{
		TControl	*theControl = Controls[i];
		TLabel *theLabel = dynamic_cast<TLabel *>(theControl);
		if( !theLabel && theControl != ButtonOk )
			theControl->Enabled = enableControls;
	}
	vcl::bringWindowToFront( this );
	EditName->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TItemCreateForm::ButtonOkClick(TObject *)
{
	if( EditName->Text == "" )
	{
		ModalResult = 0;
		EditName->SetFocus();
		throw Exception( "Name cannot be empty" );
	}
}
//---------------------------------------------------------------------------

void __fastcall TItemCreateForm::FormClose(TObject *,
	  TCloseAction &)
{
	theItem = theParent = NULL;
}
//---------------------------------------------------------------------------

