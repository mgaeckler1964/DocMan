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

#include "ReminderFilesFrm.h"
#include "ItemManager.h"
#include "DocManDM.h"
#include "DocManMain.h"

//---------------------------------------------------------------------------
using namespace gak;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TReminderFilesForm *ReminderFilesForm;
//---------------------------------------------------------------------------
__fastcall TReminderFilesForm::TReminderFilesForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void TReminderFilesForm::reloadQuery( bool allways )
{
	if( allways || !QueryOpenFiles->Active )
	{
		gak::DateTime now;

		QueryOpenFiles->Close();
		QueryOpenFiles->Params->Items[0]->AsInteger = now.getUtcUnixSeconds();

		QueryOpenFiles->Open();
	}
}
//---------------------------------------------------------------------------
void __fastcall TReminderFilesForm::FormHide(TObject *)
{
	QueryOpenFiles->Close();
}
//---------------------------------------------------------------------------
void __fastcall TReminderFilesForm::QueryOpenFilesCalcFields(TDataSet *)
{
	int		parentID = QueryOpenFilesParentID->AsInteger;
	STRING	path = DocManDataModule->getPathForID( parentID );
	QueryOpenFilesPATH->AsString = (const char *)path;


	gak::uint32 reminderDate1 = QueryOpenFilesReminderDate->AsInteger;
	gak::DateTime reminderDate2 = gak::DateTime(time_t(reminderDate1),0).calcLocalTime();
	TDateTime reminderDate3 = reminderDate2;

	QueryOpenFilesReminder->AsDateTime = reminderDate3;
}
//---------------------------------------------------------------------------

void __fastcall TReminderFilesForm::DBGridDblClick(TObject *)
{
	PTR_ITEM	theParentItem;
	int			theItemId;

	theParentItem = getItem( QueryOpenFilesParentID->AsInteger );
	theItemId = QueryOpenFilesID->AsInteger;
	if( theParentItem )
	{
		DocManMainForm->openItem( theParentItem, theItemId );
		DocManMainForm->BringToFront();
		DocManMainForm->SetFocus();
	}
}
//---------------------------------------------------------------------------

void __fastcall TReminderFilesForm::FormCreate(TObject *)
{
	::SetWindowLong( Handle, GWL_HWNDPARENT, NULL );
}
//---------------------------------------------------------------------------

void __fastcall TReminderFilesForm::FormShow(TObject *Sender)
{
	reloadQuery(false);
	gak::vcl::bringWindowToFront( this );
}
//---------------------------------------------------------------------------

void __fastcall TReminderFilesForm::SpeedButtonClick(TObject *Sender)
{
	reloadQuery(true);
}
//---------------------------------------------------------------------------

void TReminderFilesForm::openChecked()
{
	reloadQuery(false);
	if( QueryOpenFiles->Bof && QueryOpenFiles->Eof )
	{
		QueryOpenFiles->Close();
	}
	else
	{
		Show();
		gak::vcl::bringWindowToFront( this );
		BringToFront();
		SetFocus();
	}
}

//---------------------------------------------------------------------------

void showReminder()
{
	ReminderFilesForm->openChecked();
	if( ReminderFilesForm->Visible )
	{
		gak::vcl::bringWindowToFront( ReminderFilesForm );
		ReminderFilesForm->SetFocus();
	}
}

