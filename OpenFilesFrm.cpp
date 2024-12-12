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
#include <gak/vcl_tools.h>

#pragma hdrstop

#include "OpenFilesFrm.h"
#include "ItemManager.h"
#include "DocManDM.h"
#include "DocManMain.h"

//---------------------------------------------------------------------------
using namespace gak;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TOpenFilesForm *OpenFilesForm;
//---------------------------------------------------------------------------
__fastcall TOpenFilesForm::TOpenFilesForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void TOpenFilesForm::reloadQuery( void )
{
}
//---------------------------------------------------------------------------
void __fastcall TOpenFilesForm::ReloadQuery(TObject *)
{
	if( !vcl::isSystemAdmin() )
	{
		CheckBoxAllFiles->Checked = false;
		CheckBoxAllFiles->Enabled = false;
		CheckBoxAllFiles->Visible = false;
	}
	else
	{
		CheckBoxAllFiles->Enabled = true;
		CheckBoxAllFiles->Visible = true;
	}
	QueryOpenFiles->Close();
	QueryOpenFiles->SQL->Clear();
	QueryOpenFiles->SQL->Add(
		"select it.ID, it.ParentID, it.Name, it.ReservedBy, it.ReservedOn, it.ReservedFor, ut.Username "
		"from item_tree it "
		"left join user_tab ut on ut.id = it.ReservedBy "
		"where it.ReservedBy <> 0 "
	);

	if( !CheckBoxAllFiles->Checked )
		QueryOpenFiles->SQL->Add( "and it.ReservedBy = :actUser" );

	QueryOpenFiles->SQL->Add( "order by it.ParentID, it.name" );

	if( !CheckBoxAllFiles->Checked )
	{
		QueryOpenFiles->Params->Items[0]->AsInteger = vcl::getActUserID();
	}

	QueryOpenFiles->Open();
}
//---------------------------------------------------------------------------
void __fastcall TOpenFilesForm::FormHide(TObject *)
{
	QueryOpenFiles->Close();
}
//---------------------------------------------------------------------------
void __fastcall TOpenFilesForm::QueryOpenFilesCalcFields(TDataSet *)
{
	int		parentID = QueryOpenFilesParentID->AsInteger;
	STRING	path = DocManDataModule->getPathForID( parentID );
	QueryOpenFilesPATH->AsString = (const char *)path;

	int		taskId = QueryOpenFilesReservedFor->AsInteger;
	if( taskId )
		path = DocManDataModule->getPathForID( taskId );
	else
		path = "";
	QueryOpenFilesTASK_PATH->AsString = (const char *)path;
}
//---------------------------------------------------------------------------

void __fastcall TOpenFilesForm::DBGridDblClick(TObject *)
{
	PTR_ITEM	theParentItem;
	int			theItemId;

	if( DBGrid->SelectedField == QueryOpenFilesTASK_PATH )
	{
		PTR_ITEM theTask = getItem( QueryOpenFilesReservedFor->AsInteger );
		if( theTask )
		{
			theParentItem = (*theTask).getParent();
			theItemId = QueryOpenFilesReservedFor->AsInteger;
		}
	}
	else
	{
		theParentItem = getItem( QueryOpenFilesParentID->AsInteger );
		theItemId = QueryOpenFilesID->AsInteger;
	}
	if( theParentItem )
	{
		DocManMainForm->openItem( theParentItem, theItemId );
		DocManMainForm->BringToFront();
		DocManMainForm->SetFocus();
	}
}
//---------------------------------------------------------------------------

void __fastcall TOpenFilesForm::FormCreate(TObject *)
{
	::SetWindowLong( Handle, GWL_HWNDPARENT, NULL );
}
//---------------------------------------------------------------------------

