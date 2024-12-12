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

#include "ReleaseFrm.h"
#include "ActionManager.h"
#include "Folder.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ItemCreateFrm"
#pragma resource "*.dfm"
TReleaseForm *ReleaseForm;
//---------------------------------------------------------------------------
class ACTION_BRANCH_RELEASE : public ACTION_BASE_VERSIONS
{
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
};
//---------------------------------------------------------------------------
class ACTION_RELEASE : public ACTION_BASE_PROPERTIES
{
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
};
//---------------------------------------------------------------------------
bool ACTION_BRANCH_RELEASE::acceptItem( THE_ITEM *theItem )
{
	THE_RELEASE_FOLDER *theFolder = dynamic_cast<THE_RELEASE_FOLDER *>(theItem);
	return theFolder ? ACTION_BASE::acceptItem( theItem ) : false;
}
//---------------------------------------------------------------------------
const char *ACTION_BRANCH_RELEASE::getLabel( void ) const
{
	return "Branch...";
}
//---------------------------------------------------------------------------
REFRESH_TYPE ACTION_BRANCH_RELEASE::perform( PTR_ITEM theItem )
{
	REFRESH_TYPE	refresh;
	PTR_ITEM 		parent = (*theItem).getParent();

	ReleaseForm->createBranch( theItem );
	if( ReleaseForm->ShowModal() == mrOk )
	{
		ThreadCopying *theThread = new ThreadCopying(
			theItem,
			parent,
			ReleaseForm->CheckBoxCreateLinks->Checked,
			true,
			ReleaseForm->EditName->Text.c_str(),
			ReleaseForm->MemoDescription->Text.c_str()
		);
		theThread->StartThread();

		refresh = REFRESH_RELOAD;
	}
	else
	{
		refresh = REFRESH_NONE;
	}
	return refresh;
}
//---------------------------------------------------------------------------
bool ACTION_RELEASE::acceptItem( THE_ITEM *theItem )
{
	THE_RELEASE_FOLDER *theFolder = dynamic_cast<THE_RELEASE_FOLDER *>(theItem);
	return theFolder ? ACTION_BASE::acceptItem( theItem ) : false;
}
//---------------------------------------------------------------------------
REFRESH_TYPE ACTION_RELEASE::perform( PTR_ITEM theItem )
{
	THE_RELEASE_FOLDER	*theFolder = dynamic_cast<THE_RELEASE_FOLDER *>(
		static_cast<THE_ITEM*>(theItem)
	);
	if( theFolder )
	{
		ReleaseForm->updateRelease( theFolder );
		if( ReleaseForm->ShowModal() == mrOk )
		{
			theFolder->setName( ReleaseForm->EditName->Text.c_str() );
			theFolder->setDescription( ReleaseForm->MemoDescription->Text.c_str() );
			theFolder->setRelease(
				ReleaseForm->UpDownMajor->Position,
				ReleaseForm->UpDownMinor->Position,
				ReleaseForm->UpDownPatch->Position
			);
			theFolder->updateDatabase();

			return REFRESH_RELOAD;
		}
	}

	return REFRESH_NONE;
}
//---------------------------------------------------------------------------
const char *ACTION_RELEASE::getLabel( void ) const
{
	return "Release...";
}
//---------------------------------------------------------------------------
static ACTION_BRANCH_RELEASE	theBranchAction;
static ACTION_RELEASE			theReleaseAction;
//---------------------------------------------------------------------------
__fastcall TReleaseForm::TReleaseForm(TComponent* Owner)
	: TItemCreateForm(Owner)
{
}
//---------------------------------------------------------------------------


void __fastcall TReleaseForm::FormShow(TObject *Sender)
{
	TItemCreateForm::FormShow( Sender );

	THE_RELEASE_FOLDER	*theFolder = dynamic_cast<THE_RELEASE_FOLDER *>(
		static_cast<THE_ITEM*>(theItem)
	);
	if( theFolder )
	{
		EditName->Text = static_cast<const char *>(theFolder->getName());
		UpDownMajor->Position = short(theFolder->getMajor());
		UpDownMinor->Position = short(theFolder->getMinor());
		if( createBranchFlag )
		{
			UpDownPatch->Position = short(theFolder->getPatch()+1);
			createName();
		}
		else
			UpDownPatch->Position = short(theFolder->getPatch());
	}
	else
	{
		EditName->Text = "1.0.0";
		UpDownMajor->Position = 1;
		UpDownMinor->Position = 0;
		UpDownPatch->Position = 0;
	}

	EditName->Enabled = false;
	EditName->ReadOnly = true;

	if( createBranchFlag )
	{
		Caption = "Create Branch";
		CheckBoxCreateLinks->Visible = true;
	}
	else
	{
		//Caption = "Create Branch";
		CheckBoxCreateLinks->Visible = false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TReleaseForm::UpDownClick(TObject *, TUDBtnType )
{
	createName();
}
//---------------------------------------------------------------------------


void __fastcall TReleaseForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
	TItemCreateForm::FormClose( Sender, Action );
	createBranchFlag = false;
}
//---------------------------------------------------------------------------

