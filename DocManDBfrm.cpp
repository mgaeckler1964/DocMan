//---------------------------------------------------------------------------

#include <vcl.h>

#include <gak/array.h>
#pragma hdrstop

#include "DocManDBfrm.h"
//---------------------------------------------------------------------------
using namespace gak;
using namespace vcl;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "dbConvertFrm"
#pragma resource "*.dfm"
TDocManDbConvertForm *DocManDbConvertForm;
//---------------------------------------------------------------------------
#pragma option -RT-
class DocmanDatabaseSchema : public DatabaseSchema
{
	bool refreshVolumeIDs;

	virtual void checkFieldsBeforePost( TDataSet *destTable, const STRING &tableName );
	virtual void closeTable( TDataSet *theTable, const STRING &tableName );

	public:
	DocmanDatabaseSchema()
	{
		refreshVolumeIDs = false;
	}
};
#pragma option -RT.
//---------------------------------------------------------------------------
void DocmanDatabaseSchema::checkFieldsBeforePost( TDataSet *destTable, const STRING &tableName )
{
	if( tableName == "ITEM_TREE" )
	{
		TField *volumeID = destTable->FieldByName( "VOLUME_ID" );
		if( volumeID->IsNull || volumeID->AsInteger == 0 )
		{
			int parentID = destTable->FieldByName( "PARENTID" )->AsInteger;
			if( parentID < 0 )
				volumeID->AsInteger = destTable->FieldByName( "ID" )->AsInteger;
			else
			{
				volumeID->AsInteger = 0;
				refreshVolumeIDs = true;
			}
		}
	}
}
//---------------------------------------------------------------------------
void DocmanDatabaseSchema::closeTable( TDataSet *theTable, const STRING &tableName )
{
	if( tableName == "ITEM_TREE" && refreshVolumeIDs )
	{
		int 		parentID, volumeID;
		ArrayOfInts	badParents;

		TQuery *theQuery = new TQuery( NULL );
		theQuery->DatabaseName = getDatabase()->DatabaseName;
		theQuery->SQL->Add( "select VOLUME_ID from ITEM_TREE where ID = :parentID" );

		TQuery *updateQuery = new TQuery( NULL );
		updateQuery->DatabaseName = getDatabase()->DatabaseName;
		updateQuery->SQL->Add( "update ITEM_TREE set VOLUME_ID=:newVolume where PARENTID = :parentID" );

		theTable->Close();
		theTable->Filter = "VOLUME_ID=0";
		theTable->Filtered = true;
		theTable->Open();

		while( !theTable->Eof )
		{
			parentID = theTable->FieldByName( "PARENTID" )->AsInteger;
			if( !badParents.hasElement( parentID ) )
			{
				theQuery->Params->Items[0]->AsInteger = parentID;
				theQuery->Open();
				if( !theQuery->Eof )
					volumeID = theQuery->Fields->Fields[0]->AsInteger;
				else
					volumeID = 0;
				theQuery->Close();

				if( volumeID )
				{
					updateQuery->Params->Items[0]->AsInteger = volumeID;
					updateQuery->Params->Items[1]->AsInteger = parentID;

					updateQuery->ExecSQL();
					theTable->Close();
					badParents.clear();
					theTable->Open();
				}
				else
				{
					theTable->Next();
					badParents.addElement( parentID );
				}
			}
			else
				theTable->Next();
		}
	}

	DatabaseSchema::closeTable( theTable, tableName );
}
//---------------------------------------------------------------------------
__fastcall TDocManDbConvertForm::TDocManDbConvertForm(TComponent* Owner)
	: TDbConvertForm(Owner)
{
}
//---------------------------------------------------------------------------
const char *TDocManDbConvertForm::getAliasName( void ) const
{
	return "DOCMAN";
}
//---------------------------------------------------------------------------
DatabaseSchema *TDocManDbConvertForm::createSchema( void ) const
{
	return new DocmanDatabaseSchema;
}
//---------------------------------------------------------------------------








