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

#include "LocalDirsFrm.h"
#include "ItemManager.h"
#include "DocManDM.h"

//---------------------------------------------------------------------------
using namespace gak;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TLocalDirsForm *LocalDirsForm;
//---------------------------------------------------------------------------
__fastcall TLocalDirsForm::TLocalDirsForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void TLocalDirsForm::reloadQuery( void )
{
	int recNO = QueryLocalDirs->RecNo;

	QueryLocalDirs->Close();
	for(
		QueryLocalDirs->Open();
		!QueryLocalDirs->Eof;
		QueryLocalDirs->Next()
	)
	{
		if( QueryLocalDirs->RecNo == recNO )
			break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TLocalDirsForm::ReloadQuery(TObject *)
{
	if( !vcl::isSystemAdmin() )
	{
		CheckBoxAllDirectories->Checked = false;
		CheckBoxAllDirectories->Enabled = false;
		CheckBoxAllDirectories->Visible = false;
	}
	else
	{
		CheckBoxAllDirectories->Enabled = true;
		CheckBoxAllDirectories->Visible = true;
	}

	QueryLocalDirs->Close();
	QueryLocalDirs->SQL->Clear();
	QueryLocalDirs->SQL->Add(
		"select lp.ID, lp.FOLDER_ID, it.ParentID, lp.USER_ID, it.NAME, lp.MACHINE, lp.LOCAL_PATH, ut.Username "
		"from item_tree it, user_tab ut, i_local_folder lp "
		"where it.ID = lp.FOLDER_ID "
		"and lp.user_id = ut.id"
	);

	if( !CheckBoxAllDirectories->Checked )
		QueryLocalDirs->SQL->Add( "and lp.user_id = :actUser" );

	QueryLocalDirs->SQL->Add( "order by it.ParentID, it.NAME" );

	if( !CheckBoxAllDirectories->Checked )
	{
		QueryLocalDirs->Params->Items[0]->AsInteger = vcl::getActUserID();
	}

	QueryLocalDirs->Open();
}
//---------------------------------------------------------------------------
void __fastcall TLocalDirsForm::FormHide(TObject *)
{
	QueryLocalDirs->Close();
}
//---------------------------------------------------------------------------
void __fastcall TLocalDirsForm::QueryLocalDirsCalcFields(TDataSet *)
{
	int		itemID = QueryLocalDirsFOLDER_ID->AsInteger;
	STRING	path = DocManDataModule->getPathForID( itemID );

	QueryLocalDirsPATH->AsString = (const char *)path;
	/*
		local path is a memo field that cannot be display in the DB Grid
	*/
	QueryLocalDirsSHORT_PATH->AsString = QueryLocalDirsLOCAL_PATH->AsString;
}
//---------------------------------------------------------------------------

void __fastcall TLocalDirsForm::SpeedButtonDeleteClick(TObject *)
{
	::SetCursor( LoadCursor( NULL, IDC_WAIT ) );

	try
	{
		int itemID =  QueryLocalDirsFOLDER_ID->AsInteger;

		PTR_ITEM	theitem = getItem( itemID );
		if( theitem )
		{
			STRING	machine = QueryLocalDirsMACHINE->AsString.c_str();
			int		reserveCount;

			std::auto_ptr<TQuery>	theQuery( new TQuery( NULL ) );
			theQuery->DatabaseName = "docmandb";
			theQuery->SQL->Add(
				"select count(*) "
				"from item_tree "
				"where reservedby = :theuser and reservedon = :themachine"
			);
			theQuery->Params->Items[0]->AsInteger = QueryLocalDirsUSER_ID->AsInteger;
			theQuery->Params->Items[1]->AsString = QueryLocalDirsMACHINE->AsString;

			theQuery->Open();
			reserveCount = theQuery->Fields->Fields[0]->AsInteger;
			theQuery->Close();

			if( reserveCount
			&&  theitem->hasReserved( machine, QueryLocalDirsUSER_ID->AsInteger ) )
			{
/*@*/			throw Exception( "there are reserved items" );
			}

			theQuery->SQL->Clear();
			theQuery->SQL->Add( "delete from i_local_folder where id=:thelfid" );
			theQuery->Params->Items[0]->AsInteger = QueryLocalDirsID->AsInteger;
			theQuery->ExecSQL();

			reloadQuery();
		}
		else
/*@*/		throw Exception( "item not found" );
	}
	catch( ... )
	{
		::SetCursor( LoadCursor( NULL, IDC_ARROW ) );
		throw;
	}
	::SetCursor( LoadCursor( NULL, IDC_ARROW ) );
}
//---------------------------------------------------------------------------

void __fastcall TLocalDirsForm::FormCreate(TObject *)
{
	::SetWindowLong( Handle, GWL_HWNDPARENT, NULL );
}
//---------------------------------------------------------------------------

