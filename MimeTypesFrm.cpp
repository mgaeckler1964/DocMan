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

