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

#include "SyncLocalFrm.h"
#include "ActionManager.h"
#include "Folder.h"
#include "PermissionsFrm.h"
#include "DirSelFrm.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSyncLocalForm *SyncLocalForm;
//---------------------------------------------------------------------------
class ACTION_SYNCED_LOCAL_PATH : public ACTION_BASE_PROPERTIES
{
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
};
//---------------------------------------------------------------------------
bool ACTION_SYNCED_LOCAL_PATH::acceptItem( THE_ITEM *theItem )
{
	THE_LOCAL_FOLDER	*theFolder = dynamic_cast<THE_LOCAL_FOLDER *>(theItem);

	if( theFolder )
	{
		int perms = theFolder->getUserPermissions();
		return perms & ITEM_PERM_MODIFY ? ACTION_BASE::acceptItem( theItem ) : false;
	}
	return false;
}
//---------------------------------------------------------------------------
REFRESH_TYPE ACTION_SYNCED_LOCAL_PATH::perform( PTR_ITEM theItem )
{
	THE_LOCAL_FOLDER	*theFolder = dynamic_cast<THE_LOCAL_FOLDER *>(
		static_cast<THE_ITEM*>( theItem )
	);

	if( theFolder )
	{
		STRING machine = TDocManDataModule::getMachine();
		STRING oldPath = theFolder->getLocalPath();
		SyncLocalForm->LabelMachine->Caption = (const char *)machine;
		SyncLocalForm->EditLocalPath->Text = (const char *)oldPath;

		if( SyncLocalForm->ShowModal() == mrOk )
		{
			STRING newPath = SyncLocalForm->EditLocalPath->Text.c_str();
			if( oldPath != newPath )
			{
				theFolder->setLocalPath( newPath );
				theFolder->updateDatabase();

				return REFRESH_RELOAD;
			}
		}
	}

	return REFRESH_NONE;
}
//---------------------------------------------------------------------------
const char *ACTION_SYNCED_LOCAL_PATH::getLabel( void ) const
{
	return "Local Path...";
}
//---------------------------------------------------------------------------
static ACTION_SYNCED_LOCAL_PATH theAction;
//---------------------------------------------------------------------------
__fastcall TSyncLocalForm::TSyncLocalForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TSyncLocalForm::SpeedButtonClick(TObject *)
{
	if( DirectorySelector->ShowModal( EditLocalPath->Text ) == mrOk )
		EditLocalPath->Text = DirectorySelector->DirectoryListBox->Directory;
}
//---------------------------------------------------------------------------

