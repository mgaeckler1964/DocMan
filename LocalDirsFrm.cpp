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

