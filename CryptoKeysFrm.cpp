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

#include <gak/iostream.h>
#include <gak/directory.h>
#include <gak/vcl_tools.h>

#pragma hdrstop

#include "CryptoKeysFrm.h"
#include "ActionManager.h"
#include "CryptoFile.h"

//---------------------------------------------------------------------------
using namespace gak;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TCryptoKeysForm *CryptoKeysForm;
//---------------------------------------------------------------------------
class ACTION_CRYPTO_KEYS : public ACTION_BASE_PERMISSIONS
{
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
};
//---------------------------------------------------------------------------
bool ACTION_CRYPTO_KEYS::acceptItem( THE_ITEM *theItem )
{
	bool					accepted;
	const THE_CRYPTO_FILE	*theFile = dynamic_cast<const THE_CRYPTO_FILE *>(theItem);
	if( theFile )
	{
		accepted = theFile->canCreateVersion();
	}
	else
	{
		const THE_REMOTE_CRYPTO_FILE	*theFile = dynamic_cast<const THE_REMOTE_CRYPTO_FILE *>(theItem);
		if( theFile )
		{
			accepted = theFile->canCreateVersion();
		}
		else
		{
			accepted = false;
		}
	}
	if( accepted )
	{
		accepted = ACTION_BASE::acceptItem( theItem );
	}

	return accepted;
}
//---------------------------------------------------------------------------
const char *ACTION_CRYPTO_KEYS::getLabel( void ) const
{
	return "Change Keys...";
}
//---------------------------------------------------------------------------
REFRESH_TYPE ACTION_CRYPTO_KEYS::perform( PTR_ITEM theItem )
{
	CryptoKeysForm->setItem( theItem );
	CryptoKeysForm->ShowModal();

	return REFRESH_NONE;
}

//---------------------------------------------------------------------------
static ACTION_CRYPTO_KEYS theAction;
//---------------------------------------------------------------------------
__fastcall TCryptoKeysForm::TCryptoKeysForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TCryptoKeysForm::FormShow(TObject *)
{
	ListBoxAvailable->Items->Clear();
	ListBoxSelected->Items->Clear();

	PTR_CRYPTO_FILE	theFile = theItem;
	if( theFile )
	{
		fileName = theFile->downloadCrypted( 0, false, "");
	}
	else
	{
		PTR_REMOTE_CRYPTO_FILE	theFile = theItem;
		if( theFile )
		{
			fileName = theFile->downloadCrypted( 0, false, "" );
		}
		else
		{
			fileName = NULL_STRING;
		}
	}

	if( fileName.isEmpty() )
	{
/*@*/	throw Exception( "Unable to download file" );
	}
	readFromBinaryFile( fileName, &crypto, cryptoMagic, cryptoVersion, false );

	strRemove( fileName );

	crypto.decryptAesKey(
		DocManDataModule->getActUser()->userName,
		DocManDataModule->getPrivateKey()
	);

	Array<STRING>	keyList = crypto.getKeyList();
	for(
		Array<STRING>::iterator it = keyList.begin(), endIT = keyList.end();
		it != endIT;
		++it
	)
	{
		ListBoxSelected->Items->Add( static_cast<const char *>(*it) );
	}

	STRING	entry;
	STRING	path = THE_FILE::getExternalStorageBase() +
		DIRECTORY_DELIMITER_STRING "keys" DIRECTORY_DELIMITER_STRING
		"*.key_pub"
	;
	DirectoryList	thePubKeys;
	thePubKeys.findFiles( path );
	for(
		DirectoryList::iterator	it = thePubKeys.begin(), endIT = thePubKeys.end();
		it != endIT;
		++it
	)
	{
		entry = it->fileName;
		entry.cut( entry.strlen() - 8 );
		if( !keyList.hasElement( entry ) )
		{
			ListBoxAvailable->Items->Add( static_cast<const char *>(entry) );
		}
	}

	vcl::bringWindowToFront( this );
}
//---------------------------------------------------------------------------
void __fastcall TCryptoKeysForm::ButtonAddClick(TObject *)
{
	if( ListBoxAvailable->ItemIndex >= 0 )
	{
		STRING identifier = ListBoxAvailable->Items->Strings[ListBoxAvailable->ItemIndex].c_str();
		STRING	path = THE_FILE::getExternalStorageBase() +
			DIRECTORY_DELIMITER_STRING "keys" DIRECTORY_DELIMITER_STRING +
			identifier
		;

		crypto.addPublicKeyFromFile( identifier, path );
		ListBoxSelected->Items->Add( static_cast<const char *>(identifier) );
		ListBoxAvailable->Items->Delete( ListBoxAvailable->ItemIndex );
	}
}
//---------------------------------------------------------------------------

void __fastcall TCryptoKeysForm::ButtonOKClick(TObject *)
{
	writeToBinaryFile( fileName, crypto, cryptoMagic, cryptoVersion, owmOverwrite );

	PTR_CRYPTO_FILE	theFile = theItem;
	if( theFile )
	{
		theFile->createCryptedVersion( fileName, "" );
	}
	else
	{
		PTR_REMOTE_CRYPTO_FILE	theFile = theItem;
		if( theFile )
		{
			theFile->createCryptedVersion( fileName, "" );
		}
	}

	strRemove( fileName );
}
//---------------------------------------------------------------------------

void __fastcall TCryptoKeysForm::ButtonRemoveClick(TObject *)
{
	if( ListBoxSelected->ItemIndex >= 0 && ListBoxSelected->Items->Count >= 2 )
	{
		STRING identifier = ListBoxSelected->Items->Strings[ListBoxSelected->ItemIndex].c_str();

		crypto.removeKey( identifier );
		ListBoxAvailable->Items->Add( static_cast<const char *>(identifier) );
		ListBoxSelected->Items->Delete( ListBoxSelected->ItemIndex );
	}
}
//---------------------------------------------------------------------------

