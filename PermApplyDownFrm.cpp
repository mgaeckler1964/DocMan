//---------------------------------------------------------------------------

#include <vcl.h>
#include <gak/vcl_tools.h>

#pragma hdrstop

#include "DocManThreads.h"
#include "PermApplyDownFrm.h"
#include "ActionManager.h"
#include "ItemManager.h"
#include "PermissionsFrm.h"

//---------------------------------------------------------------------------
using namespace gak;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TPermApplyDownForm *PermApplyDownForm;
//---------------------------------------------------------------------------
#pragma option -RT-
class THREAD_PERM_APPLY_DOWN : public ThreadDocMan
{
	ITEM_PERMLIST	acl;
	enum  PERM_APPLY_MODE	permMode;

	virtual const char *getTitle( void ) const;
	virtual void perform( void );

	public:
	THREAD_PERM_APPLY_DOWN(
		THE_ITEM				*theItemToHandle,
		ITEM_PERMLIST			&acl,
		enum  PERM_APPLY_MODE	permMode
	)
	: ThreadDocMan(theItemToHandle)
	{
		this->acl = acl;
		this->permMode = permMode;
	}
};
#pragma option -RT.

class ACTION_PERM_APPLY_DOWN : public ACTION_BASE_PERMISSIONS
{
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
};
//---------------------------------------------------------------------------
bool ACTION_PERM_APPLY_DOWN::acceptItem( THE_ITEM *theItem )
{
	if( theItem->isContainer() && !theItem->getPermissionID() )
	{
		int perm = theItem->getUserPermissions();
		if( perm & ITEM_PERM_EDIT_PERMS )
/***/		return true;
	}
	return false;;
}
//---------------------------------------------------------------------------
const char *ACTION_PERM_APPLY_DOWN::getLabel( void ) const
{
	return "Permissions Apply Down...";
}
//---------------------------------------------------------------------------
const char *THREAD_PERM_APPLY_DOWN::getTitle( void ) const
{
	return "Apply Permissions";
}
//---------------------------------------------------------------------------
void THREAD_PERM_APPLY_DOWN::perform( void )
{
	theItemToHandle->setACL( acl, permMode );
}
//---------------------------------------------------------------------------
REFRESH_TYPE ACTION_PERM_APPLY_DOWN::perform( PTR_ITEM theItem )
{
	PermApplyDownForm->setItem( theItem );
	if( PermApplyDownForm->ShowModal() == mrOk )
	{
		for( int row = PermApplyDownForm->StringGrid->RowCount-1; row > 0; row-- )
		{
			if( PermApplyDownForm->StringGrid->Cells[2][row] == "" )
			{
				PermApplyDownForm->acl.removeElementAt( row-1 );
			}
		}
		enum  PERM_APPLY_MODE	permMode;
		if( PermApplyDownForm->RadioButtonMerge->Checked )
			permMode = PERM_APPLY_MERGE;
		else if( PermApplyDownForm->RadioButtonOverwrite->Checked )
			permMode = PERM_APPLY_OVERWRITE;
		else
			permMode = PERM_APPLY_DELETE;

		if( PermApplyDownForm->acl.size() )
		{
			THREAD_PERM_APPLY_DOWN *theThread = new THREAD_PERM_APPLY_DOWN(
				theItem,
				PermApplyDownForm->acl,
				permMode
			);
			theThread->StartThread();
		}
		else
		{
			Application->MessageBox(
				"Nothing to apply", "DocMan",
				MB_ICONSTOP|MB_OK
			);
		}
	}

	return REFRESH_NONE;
}
//---------------------------------------------------------------------------
static ACTION_PERM_APPLY_DOWN theAction;
//---------------------------------------------------------------------------
__fastcall TPermApplyDownForm::TPermApplyDownForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TPermApplyDownForm::FormShow(TObject *)
{
	UserOrGroup	theUser;
	STRING		permStr;
	int			perms;

	std::size_t	numAcls;

	StringGrid->Cells[0][0] = "User";
	StringGrid->Cells[1][0] = "Permissions";
	StringGrid->Cells[2][0] = "Select";

	acl = theItem->getACL();
	theUser.ID = -1;

	numAcls = acl.size();
	if( numAcls )
	{
		StringGrid->RowCount = numAcls+1;
		for( std::size_t i=0, row=1; i<numAcls; i++, row++ )
		{
			const ITEM_PERM &permEntry = acl[i];
			if( theUser.ID != permEntry.user_id )
				getUserById( "DocManDB", permEntry.user_id, &theUser );

			permStr = "";
			perms = permEntry.permissions;

			if( perms & ITEM_PERM_SEE )
				permStr += "See Contents - ";
			if( perms & ITEM_PERM_MODIFY )
				permStr += "Modify - ";
			if( perms & ITEM_PERM_MOVE )
				permStr += "Move - ";
			if( perms & ITEM_PERM_ADD_VERSION )
				permStr += "Add Version - ";
			if( perms & ITEM_PERM_ADD_ITEMS )
				permStr += "Add Items - ";
			if( perms & ITEM_PERM_EDIT_PERMS )
				permStr += "Edit Permissions - ";
			if( perms & ITEM_PERM_DELETE_VERSION )
				permStr += "Delete Version - ";
			if( perms & ITEM_PERM_DELETE )
				permStr += "Delete - ";
			if( perms & ITEM_PERM_COMPLETE_TASK )
				permStr += "Complete Task - ";

			StringGrid->Cells[0][row] = (const char *)theUser.userName;
			StringGrid->Cells[1][row] = (const char *)permStr;
			StringGrid->Cells[2][row] = "";
		}
	}
	else
	{
		StringGrid->RowCount = 2;
		Application->MessageBox( "Nothing to apply", "DocMan", MB_ICONSTOP|MB_OK );
		ModalResult = mrCancel;
		Close();
	}

	RadioButtonMerge->Checked = true;
}
//---------------------------------------------------------------------------
void __fastcall TPermApplyDownForm::StringGridDrawCell(TObject *,
	  int ACol, int ARow, TRect &Rect, TGridDrawState )
{
	if( ARow && ACol == 2 && StringGrid->Cells[ACol][ARow] != "" )
	{
		Graphics::TBitmap		*theBitmap = new Graphics::TBitmap;
		ImageList->GetBitmap( 0, theBitmap );
		StringGrid->Canvas->Draw( Rect.Left, Rect.Top, theBitmap );
	}
}
//---------------------------------------------------------------------------

void __fastcall TPermApplyDownForm::StringGridMouseUp(TObject *,
	  TMouseButton Button, TShiftState , int X, int Y)
{
	if( Button == mbLeft )
	{
		int	row, col;

		StringGrid->MouseToCell( X, Y, col, row );
		if( row && col == 2 )
		{
			if( StringGrid->Cells[col][row] == "." )
				StringGrid->Cells[col][row] = "";
			else
				StringGrid->Cells[col][row] = ".";
		}
	}
}
//---------------------------------------------------------------------------

