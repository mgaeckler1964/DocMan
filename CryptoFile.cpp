/*
		Project:		
		Module:			
		Description:	
		Author:			Martin G�ckler
		Address:		Grafrather Str 8, D-81245 M�nchen
		Phone:			+49 - 89 - 65 30 95 63
		Web:			http://www.cresd.de/

		Copyright:		(c) 1988-2014 CRESD GmbH

		Sie d�rfen die Software f�r private Zwecke oder, wenn die hiermit
		erstellte Software ebenfalls kostenlos bereitgestellt wird,
		kostenlos nutzen. Eine Nutzung mit Gewinnerzielungsabsicht, eine
		anderweitige gewerbliche Nutung oder eine Weitergabe der Quellen
		ist generell untersagt und wird zivilrechtlich verfolgt.

		You may use this software for private, non-comercial projects,
		only. Any distribution of the source is NOT allowed.

		Die Software wird ohne GEW�HRLEISTUNG vertrieben. Das Risiko bei
		der Anwendung der Software liegt voll und ganz bei Ihnen. Es gibt
		auch keinerlei Garantie, da� die Software einen bestimmten Zweck
		erf�llt, jemals erf�llt hat oder irgendwann einmal erf�llen wird.

		THIS SOFTWARE IS PROVIDED BY CRESD GmbH, Germany, Munich ``AS IS''
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

		F�r eine kommerzielle Lizenz wenden Sie sich bitte an

					support@cresd.de.

		For a commercial licence, please, contact

					support@cresd.de
*/


// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/strFiles.h>
#include <gak/cryptoShared.h>

#include "CryptoFile.h"
#include "DocManMain.h"

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT-
#	pragma option -b
#	ifdef __WIN32__
#		pragma option -a4
#		pragma option -pc
#	else
#		pragma option -po
#		pragma option -a2
#	endif
#endif

using namespace gak;

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

class FACTORY_CRYPTO_FILE : public FACTORY_FILE
{
	virtual int getItemType( void ) const;
	virtual PTR_ITEM createItem( int id ) const;
	virtual const char *getName( void ) const;
};

class FACTORY_REMOTE_CRYPTO_FILE : public FACTORY_REMOTE_FILE
{
	virtual int getItemType( void ) const;
	virtual PTR_ITEM createItem( int id ) const;
	virtual const char *getName( void ) const;
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static FACTORY_CRYPTO_FILE theFactory;
static FACTORY_REMOTE_CRYPTO_FILE theRemoteFactory;

// --------------------------------------------------------------------- //
// ----- class static data --------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- prototypes ---------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module functions ---------------------------------------------- //
// --------------------------------------------------------------------- //

static template <typename ITEM>
STRING dowload( ITEM *theItem, int version, bool protect, const STRING &dest )
{
	doEnterFunction("template <typename ITEM>::dowload");
	CryptoShared	theCrypto;
	STRING			cryptedFile = theItem->downloadCrypted( version, false, dest );
	STRING			result = cryptedFile;

	try
	{
		readFromFile( cryptedFile, &theCrypto, cryptoMagic );

		if( result.endsWith( ".crypted" ) )
			result.cut( result.strlen()-8 );
		chmod( result, S_IREAD|S_IWRITE );
		theCrypto.decryptFile(
			DocManMainForm->getActUser()->userName,
			DocManMainForm->getPrivateKey(),
			result
		);

		if( protect )
			chmod( result, S_IREAD );

		if( result != cryptedFile )
			strRemove( cryptedFile );
	}
	catch( ... )
	{
		strRemove( cryptedFile );
		throw;
	}

	return result;
}


static template <typename ITEM>
void createVersion( ITEM *theItem, const STRING &filePath, const STRING &description )
{
	doEnterFunction("template <typename ITEM>::createVersion");

	struct stat statBuff;

	if( !strStat( filePath, &statBuff ) )
	{
		CryptoShared	theCrypto;
		STRING			actUser = DocManMainForm->getActUser()->userName;
		STRING			publicKeyFile = THE_FILE::getExternalStorageBase() + DIRECTORY_DELIMITER_STRING "keys" DIRECTORY_DELIMITER_STRING + actUser;

		try
		{
			STRING		cryptedFile = theItem->downloadCrypted( 0, false, "" );

			readFromFile( cryptedFile, &theCrypto, cryptoMagic );
			theCrypto.decryptAesKey( actUser, DocManMainForm->getPrivateKey() );
			strRemove( cryptedFile );
		}
		catch( ... )
		{
			// ignore errors
		}

		STRING cryptedFile = filePath + ".crypted";

		theCrypto.encryptFile( filePath, actUser, publicKeyFile );
		writeToFile( cryptedFile, theCrypto, cryptoMagic );

		setModTime( cryptedFile, statBuff.st_mtime );
		theItem->createCryptedVersion( cryptedFile, description );

		strRemove( cryptedFile );
	}
	else
	{
/*@*/	throw Exception( "File not found" );
	}
}
// --------------------------------------------------------------------- //
// ----- class inlines ------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class constructors/destructors -------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class static functions ---------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class privates ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //
   
int FACTORY_CRYPTO_FILE::getItemType( void ) const
{
	return (int)TYPE_CRYPTO_FILE;
}

PTR_ITEM FACTORY_CRYPTO_FILE::createItem( int id ) const
{
	return new THE_CRYPTO_FILE( id, this );
}

const char *FACTORY_CRYPTO_FILE::getName( void ) const
{
	return "Encrypted File";
}

int FACTORY_REMOTE_CRYPTO_FILE::getItemType( void ) const
{
	return (int)TYPE_REMOTE_CRYPTO_FILE;
}

PTR_ITEM FACTORY_REMOTE_CRYPTO_FILE::createItem( int id ) const
{
	return new THE_REMOTE_CRYPTO_FILE( id, this );
}

const char *FACTORY_REMOTE_CRYPTO_FILE::getName( void ) const
{
	return "Remote Encrypted File";
}

void THE_CRYPTO_FILE::createVersion( const STRING &filePath, const STRING &description )
{
	::createVersion( this, filePath, description );
}

STRING THE_CRYPTO_FILE::download( int version, bool protect, const STRING &dest )
{
	return ::dowload( this, version, protect, dest );
}

TGraphic *THE_CRYPTO_FILE::getItemPicture( void ) const
{
	static Graphics::TBitmap *thePic = NULL;

	if( !thePic )
	{
		thePic = new Graphics::TBitmap();
		thePic->LoadFromResourceName((unsigned int)HInstance, "CRYPTO_FILE_BITMAP");
	}
	return thePic;
}

STRING THE_REMOTE_CRYPTO_FILE::getVersionFileName( void )
{
	return getName() + ".crypted";
}

void THE_REMOTE_CRYPTO_FILE::createVersion( const STRING &filePath, const STRING &description )
{
	doEnterFunction("THE_REMOTE_CRYPTO_FILE::createVersion");
	doLogValue( filePath );
	::createVersion( this, filePath, description );
}

STRING THE_REMOTE_CRYPTO_FILE::download( int version, bool protect, const STRING &iDest )
{
	STRING dest = ::dowload( this, version, protect, iDest );
	if( !version )
	{
		chmod( dest, S_IREAD|S_IWRITE );

		const DocManService::VersionRecord_t *theVersion = getLatestVersion();
		setModTime( dest, theVersion->modifiedDate );

		if( protect )
			chmod( dest, S_IREAD );
	}

	return dest;
}

TGraphic *THE_REMOTE_CRYPTO_FILE::getItemPicture( void ) const
{
	static Graphics::TBitmap *thePic = NULL;

	if( !thePic )
	{
		thePic = new Graphics::TBitmap();
		thePic->LoadFromResourceName((unsigned int)HInstance, "CRYPTO_FILE_BITMAP");
	}
	return thePic;
}

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif
