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

#include "PermissionsFrm.h"
#include "ActionManager.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TPermissionsForm *PermissionsForm;
//---------------------------------------------------------------------------
class ACTION_PERMISSIONS : public ACTION_BASE_PERMISSIONS
{
	virtual const char *getLabel( void ) const;
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
};
//---------------------------------------------------------------------------
const char *ACTION_PERMISSIONS::getLabel( void ) const
{
	return "Permissions...";
}
//---------------------------------------------------------------------------
REFRESH_TYPE ACTION_PERMISSIONS::perform( PTR_ITEM theItem )
{
	PermissionsForm->setItem( theItem );
	PermissionsForm->ShowModal();

	return REFRESH_RELOAD;
}

//---------------------------------------------------------------------------
static ACTION_PERMISSIONS theAction;
//---------------------------------------------------------------------------
__fastcall TPermissionsForm::TPermissionsForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TPermissionsForm::FormShow(TObject *)
{
	char filter[128];

	int perms = theItem->loadPermissions();
	int	permissionID = theItem->getPermissionID();

	STRING	newCaption = "Permissions for ";
	newCaption += theItem->getName();
	Caption = (const char *)newCaption;

	if( permissionID && permissionID != theItem->getID() )
		perms &= ~ITEM_PERM_EDIT_PERMS;
	else
		permissionID = theItem->getID();

	QueryAll->Open();
	TableACLs->Filtered = true;
	sprintf( filter, "ITEM_ID=%d", permissionID );
	TableACLs->Filter = filter;


	if( theItem->isContainer() && (perms & ITEM_PERM_EDIT_PERMS) )
		CheckBoxAddItems->Enabled = true;
	else
		CheckBoxAddItems->Enabled = false;

	if( (theItem->isContainer() || theItem->isVersioned())
	&& (perms & ITEM_PERM_EDIT_PERMS) )
	{
		CheckBoxDeleteVersions->Enabled = true;
		CheckBoxAddVersions->Enabled = true;
	}
	else
	{
		CheckBoxDeleteVersions->Enabled = false;
		CheckBoxAddVersions->Enabled = false;
	}

	if( (theItem->isContainer() || theItem->isCompletable())
	&& (perms & ITEM_PERM_EDIT_PERMS) )
		CheckBoxComplete->Enabled = true;
	else
		CheckBoxComplete->Enabled = false;

	if( perms & ITEM_PERM_EDIT_PERMS )
	{
		TableACLs->ReadOnly = false;
		CheckBoxSeeContents->Enabled = true;
		CheckBoxModify->Enabled = true;
		CheckBoxEditPermissions->Enabled = true;
		CheckBoxMove->Enabled = true;
		CheckBoxDelete->Enabled = true;
	}
	else
	{
		TableACLs->ReadOnly = true;
		CheckBoxSeeContents->Enabled = false;
		CheckBoxModify->Enabled = false;
		CheckBoxEditPermissions->Enabled = false;
		CheckBoxMove->Enabled = false;
		CheckBoxDelete->Enabled = false;
	}

	TableACLs->Open();
}
//---------------------------------------------------------------------------
void __fastcall TPermissionsForm::TableACLsAfterInsert(TDataSet *)
{
	TableACLsITEM_ID->AsInteger = theItem->getID();
	TableACLsPERMISSIONS->AsInteger = 0;
}
//---------------------------------------------------------------------------
void __fastcall TPermissionsForm::PermClicked(TObject *Sender)
{
	if( !TableACLs->ReadOnly )
	{
		if( TableACLs->State == dsBrowse )
			TableACLs->Edit();

		if( Sender == CheckBoxSeeContents && !CheckBoxSeeContents->Checked )
		{
			CheckBoxModify->Checked = false;
			CheckBoxAddVersions->Checked = false;
			CheckBoxAddItems->Checked = false;
			CheckBoxEditPermissions->Checked = false;
			CheckBoxMove->Checked = false;
			CheckBoxDeleteVersions->Checked = false;
			CheckBoxSeeContents->Checked = false;
		}

		if( Sender == CheckBoxModify && !CheckBoxModify->Checked )
		{
			CheckBoxComplete->Checked = false;
			CheckBoxModify->Checked = false;
		}

		if( Sender == CheckBoxDeleteVersions && !CheckBoxDeleteVersions->Checked )
		{
			CheckBoxDelete->Checked = false;
			CheckBoxDeleteVersions->Checked = false;
		}

		if( CheckBoxComplete->Checked )
			CheckBoxModify->Checked = true;

		if( CheckBoxDelete->Checked )
			CheckBoxDeleteVersions->Checked = true;

		if( CheckBoxDeleteVersions->Checked
		|| CheckBoxMove->Checked
		|| CheckBoxEditPermissions->Checked
		|| CheckBoxAddItems->Checked
		|| CheckBoxAddVersions->Checked
		|| CheckBoxModify->Checked )
			CheckBoxSeeContents->Checked = true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TPermissionsForm::TableACLsAfterScroll(TDataSet *)
{
	int perms = TableACLsPERMISSIONS->AsInteger;

	CheckBoxSeeContents->Checked = perms & ITEM_PERM_SEE ? true : false;
	CheckBoxModify->Checked = perms & ITEM_PERM_MODIFY ? true : false;
	CheckBoxComplete->Checked = perms & ITEM_PERM_COMPLETE_TASK ? true : false;
	CheckBoxAddVersions->Checked = perms & ITEM_PERM_ADD_VERSION ? true : false;
	CheckBoxAddItems->Checked = perms & ITEM_PERM_ADD_ITEMS ? true : false;
	CheckBoxDeleteVersions->Checked = perms & ITEM_PERM_DELETE_VERSION ? true : false;
	CheckBoxEditPermissions->Checked = perms & ITEM_PERM_EDIT_PERMS ? true : false;
	CheckBoxMove->Checked = perms & ITEM_PERM_MOVE ? true : false;
	CheckBoxDelete->Checked = perms & ITEM_PERM_DELETE ? true : false;

	if( TableACLs->State == dsEdit )
		TableACLs->Cancel();
}
//---------------------------------------------------------------------------

void __fastcall TPermissionsForm::TableACLsBeforePost(TDataSet *)
{
	int perms = 0;

	if( CheckBoxSeeContents->Checked )
		perms |= ITEM_PERM_SEE;
	if( CheckBoxModify->Checked )
		perms |= ITEM_PERM_MODIFY;
	if( CheckBoxComplete->Checked )
		perms |= ITEM_PERM_COMPLETE_TASK;
	if( CheckBoxAddVersions->Checked )
		perms |= ITEM_PERM_ADD_VERSION;
	if( CheckBoxAddItems->Checked )
		perms |= ITEM_PERM_ADD_ITEMS;
	if( CheckBoxDeleteVersions->Checked )
		perms |= ITEM_PERM_DELETE_VERSION;
	if( CheckBoxEditPermissions->Checked )
		perms |= ITEM_PERM_EDIT_PERMS;
	if( CheckBoxMove->Checked )
		perms |= ITEM_PERM_MOVE;
	if( CheckBoxDelete->Checked )
		perms |= ITEM_PERM_DELETE;

	TableACLsPERMISSIONS->AsInteger = perms;
}
//---------------------------------------------------------------------------


void __fastcall TPermissionsForm::FormClose(TObject *,
	  TCloseAction &)
{
	TableACLs->Close();
	QueryAll->Close();
	theItem->loadPermissions();
}
//---------------------------------------------------------------------------


