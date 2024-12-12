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

#include "OpenTasksFrm.h"
#include "ItemManager.h"
#include "DocManDM.h"
#include "DocManMain.h"

//---------------------------------------------------------------------------
using namespace gak;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TOpenTasksForm *OpenTasksForm;
//---------------------------------------------------------------------------
__fastcall TOpenTasksForm::TOpenTasksForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TOpenTasksForm::ReloadQuery(TObject *)
{
	if( !vcl::isSystemAdmin() )
	{
		CheckBoxAllTasks->Checked = false;
		CheckBoxAllTasks->Enabled = false;
		CheckBoxAllTasks->Visible = false;
	}
	QueryOpenTasks->Close();
	QueryOpenTasks->SQL->Clear();
	QueryOpenTasks->SQL->Add(
		"select it.ID, it.ParentID, it.Name, it.Assigned_To, ut.Username, it.status, s.Task_Status "
		"from item_tree it, user_tab ut, i_task_status s "
		"where it.Assigned_To = ut.id "
		"and it.status = s.id "
		"and s.completed = 0 "
		"and it.item_type = :TypeTask"
	);

	if( !CheckBoxAllTasks->Checked )
		QueryOpenTasks->SQL->Add( "and it.Assigned_To = :actUser" );

	QueryOpenTasks->SQL->Add( "order by it.status, it.ParentID, it.name" );

	QueryOpenTasks->Params->Items[0]->AsInteger = TYPE_TASK;

	if( !CheckBoxAllTasks->Checked )
		QueryOpenTasks->Params->Items[1]->AsInteger = vcl::getActUserID();

	QueryOpenTasks->Open();
}
//---------------------------------------------------------------------------
void __fastcall TOpenTasksForm::FormHide(TObject *)
{
	QueryOpenTasks->Close();
}
//---------------------------------------------------------------------------
void __fastcall TOpenTasksForm::QueryOpenTasksCalcFields(TDataSet *)
{
	int		parentID = QueryOpenTasksParentID->AsInteger;
	STRING	path = DocManDataModule->getPathForID( parentID );

	QueryOpenTasksPATH->AsString = (const char *)path;
}
//---------------------------------------------------------------------------

void __fastcall TOpenTasksForm::DBGridDblClick(TObject *)
{
	PTR_ITEM	theItem = getItem( QueryOpenTasksParentID->AsInteger );
	if( theItem )
	{
		DocManMainForm->openItem( theItem, QueryOpenTasksID->AsInteger );
		DocManMainForm->BringToFront();
		DocManMainForm->SetFocus();
	}
}
//---------------------------------------------------------------------------

void __fastcall TOpenTasksForm::FormCreate(TObject *)
{
	::SetWindowLong( Handle, GWL_HWNDPARENT, NULL );
}
//---------------------------------------------------------------------------

