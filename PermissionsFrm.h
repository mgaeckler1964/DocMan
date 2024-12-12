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

#ifndef PermissionsFrmH
#define PermissionsFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBTables.hpp>

#include "ItemManager.h"
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <DBCtrls.hpp>
#include <ExtCtrls.hpp>

//---------------------------------------------------------------------------
const int ITEM_PERM_NONE			=   0;
const int ITEM_PERM_SEE				=   1;
const int ITEM_PERM_MODIFY			=   2;
const int ITEM_PERM_MOVE			=   4;
const int ITEM_PERM_ADD_VERSION		=   8;
const int ITEM_PERM_ADD_ITEMS		=  16;
const int ITEM_PERM_EDIT_PERMS		=  32;
const int ITEM_PERM_DELETE_VERSION	=  64;
const int ITEM_PERM_DELETE			= 128;
const int ITEM_PERM_COMPLETE_TASK	= 256; 

//---------------------------------------------------------------------------

class TPermissionsForm : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
	TTable *TableACLs;
	TDataSource *DataSourceACLs;
	TDBGrid *DBGrid1;
	TIntegerField *TableACLsITEM_ID;
	TIntegerField *TableACLsUSER_ID;
	TIntegerField *TableACLsPERMISSIONS;
	TQuery *QueryAll;
	TStringField *TableACLsUSERNAME;
	TDBNavigator *DBNavigator;
	TCheckBox *CheckBoxSeeContents;
	TCheckBox *CheckBoxModify;
	TCheckBox *CheckBoxAddVersions;
	TCheckBox *CheckBoxAddItems;
	TCheckBox *CheckBoxDeleteVersions;
	TCheckBox *CheckBoxEditPermissions;
	TCheckBox *CheckBoxMove;
	TCheckBox *CheckBoxDelete;
	TCheckBox *CheckBoxComplete;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall TableACLsAfterInsert(TDataSet *DataSet);
	void __fastcall PermClicked(TObject *Sender);
	void __fastcall TableACLsAfterScroll(TDataSet *DataSet);
	void __fastcall TableACLsBeforePost(TDataSet *DataSet);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// Anwender-Deklarationen
	THE_ITEM	*theItem;
public:		// Anwender-Deklarationen
	void setItem( THE_ITEM *theItem )
	{
		this->theItem = theItem;
	}
	__fastcall TPermissionsForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TPermissionsForm *PermissionsForm;
//---------------------------------------------------------------------------
#endif
