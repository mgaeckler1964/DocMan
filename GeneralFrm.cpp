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

#include "GeneralFrm.h"
#include "ActionManager.h"
#include "ItemManager.h"
#include "File.h"
#include "UserDlg.h"
#include "DocManMain.h"

//---------------------------------------------------------------------------
using namespace gak;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ItemCreateFrm"
#pragma resource "*.dfm"
TGeneralForm *GeneralForm;
//---------------------------------------------------------------------------
class ACTION_GENERAL : public ACTION_BASE_PROPERTIES
{
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
	virtual const char *getLabel( void ) const;
};
//---------------------------------------------------------------------------
REFRESH_TYPE ACTION_GENERAL::perform( PTR_ITEM theItem )
{
	GeneralForm->setItem( theItem );
	if( GeneralForm->ShowModal() == mrOk )
	{
		theItem->setName( GeneralForm->EditName->Text.c_str() );
		theItem->setDescription( GeneralForm->MemoDescription->Text.c_str() );
		theItem->updateDatabase();
		return REFRESH_RELOAD;
	}
	else
		return REFRESH_NONE;
}
//---------------------------------------------------------------------------
const char *ACTION_GENERAL::getLabel( void ) const
{
	return "General...";
}
//---------------------------------------------------------------------------
static ACTION_GENERAL theAction;
//---------------------------------------------------------------------------
__fastcall TGeneralForm::TGeneralForm(TComponent* Owner)
	: TItemCreateForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TGeneralForm::FormShow(TObject *Sender)
{
	int			creatorId;
	UserOrGroup	creator, reservedFor;

	TItemCreateForm::FormShow( Sender );

	LabelType->Caption = theItem->getItemTypeString();
	LabelLocalPath->Caption = (const char *)theItem->getDownloadPath();

	creatorId = theItem->getCreator();
	DocManMainForm->getUserById( creatorId, &creator );
	LabelCreator->Caption = (const char *)creator.userName;

	TDateTime createdDate = theItem->getCreatedDate();
	TDateTime modifiedDate = theItem->getModifiedDate();

	LabelCreatedDate->Caption = createdDate.DateTimeString();
	LabelModifiedDate->Caption = modifiedDate.DateTimeString();

	LabelReserved->Visible = false;
	LabelReservedFor->Visible = false;
	PTR_FILE	theFile = theItem;
	if( theFile )
	{
		int reservedBy = theFile->getReservedBy();
		if( reservedBy )
		{
			DocManMainForm->getUserById( reservedBy, &reservedFor );
			LabelReserved->Visible = true;
			LabelReservedFor->Visible = true;
			LabelReservedFor->Caption = (const char *)reservedFor.userName;
		}
	}
	vcl::bringWindowToFront( this );
}
//---------------------------------------------------------------------------
