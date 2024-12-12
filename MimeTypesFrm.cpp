//---------------------------------------------------------------------------

#include <vcl.h>
#include <vcl/registry.hpp>

#pragma hdrstop

#include "MimeTypesFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMimeTypesForm *MimeTypesForm;
//---------------------------------------------------------------------------
__fastcall TMimeTypesForm::TMimeTypesForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMimeTypesForm::FormShow(TObject *)
{
	TableMimeTypes->IndexName = "";
	TableMimeTypes->Open();
}
//---------------------------------------------------------------------------

void __fastcall TMimeTypesForm::FormClose(TObject *,
	  TCloseAction &)
{
	TableMimeTypes->Close();
}
//---------------------------------------------------------------------------

void __fastcall TMimeTypesForm::TableMimeTypesBeforePost(TDataSet *)
{
	TableMimeTypesEXTENSION->AsString = TableMimeTypesEXTENSION->AsString.LowerCase();
}
//---------------------------------------------------------------------------
STRING	TMimeTypesForm::GetMimeType( const STRING &filePath )
{
	STRING theExtension( filePath );

	theExtension += theExtension.searchRChar( DIRECTORY_DELIMITER ) +1;
	theExtension += theExtension.searchRChar( '.' ) +1;
	theExtension.lowerCase();

	STRING	mimeType = mimeTypes[theExtension];
	if( mimeType.isEmpty() )
	{
		TableMimeTypes->IndexName = "";
		TableMimeTypes->Open();
		TableMimeTypes->SetKey();
		TableMimeTypes->FieldByName( "EXTENSION" )->AsString
			= (const char *)theExtension
		;
		if( TableMimeTypes->GotoKey() )
			mimeType = TableMimeTypesMIMETYPE->AsString.c_str();

		if( mimeType.isEmpty() )
		{
			std::auto_ptr<TRegistry> reg( new TRegistry(KEY_READ) );

			reg->RootKey = HKEY_CLASSES_ROOT;

			STRING	registryKey( "\\." );
			registryKey += theExtension;

			if( reg->OpenKey( (const char *)registryKey, false ) )
			{
				if( reg->ValueExists( "Content Type" ) )
					mimeType = reg->ReadString( "Content Type" ).c_str();

				reg->CloseKey();
			}

			if( !mimeType.isEmpty() )
			{
				TableMimeTypes->Insert();
				TableMimeTypesEXTENSION->AsString = (const char *)theExtension;
				TableMimeTypesMIMETYPE->AsString = (const char *)mimeType;
				TableMimeTypes->Post();
			}
		}

		if( !mimeType.isEmpty() )
			mimeTypes[theExtension] = mimeType;
		else
			mimeType = "application/octet-stream";

		TableMimeTypes->Close();
	}

	return mimeType;

}
//---------------------------------------------------------------------------
STRING	TMimeTypesForm::GetExtension( const STRING &mimeType )
{
	STRING theExtension = mimeTypes.findValue( mimeType );

	if( theExtension.isEmpty() )
	{
		TableMimeTypes->IndexName = "MIMETYPE_IDX";
		TableMimeTypes->Open();
		TableMimeTypes->SetKey();
		TableMimeTypes->FieldByName( "MIMETYPE" )->AsString
			= (const char *)mimeType
		;
		if( TableMimeTypes->GotoKey() )
			theExtension = TableMimeTypesEXTENSION->AsString.c_str();

		if( !theExtension.isEmpty() )
			mimeTypes[theExtension] = mimeType;

		TableMimeTypes->Close();
	}

	return theExtension;

}
//---------------------------------------------------------------------------

