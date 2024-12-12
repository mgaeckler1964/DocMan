/*
		Project:		
		Module:			
		Description:	
		Author:			Martin Gäckler
		Address:		Grafrather Str 8, D-81245 München
		Phone:			+49 - 89 - 65 30 95 63
		Web:			http://www.cresd.de/

		Copyright:		(c) 1988-2014 CRESD GmbH

		Sie dürfen die Software für private Zwecke oder, wenn die hiermit
		erstellte Software ebenfalls kostenlos bereitgestellt wird,
		kostenlos nutzen. Eine Nutzung mit Gewinnerzielungsabsicht, eine
		anderweitige gewerbliche Nutung oder eine Weitergabe der Quellen
		ist generell untersagt und wird zivilrechtlich verfolgt.

		You may use this software for private, non-comercial projects,
		only. Any distribution of the source is NOT allowed.

		Die Software wird ohne GEWÄHRLEISTUNG vertrieben. Das Risiko bei
		der Anwendung der Software liegt voll und ganz bei Ihnen. Es gibt
		auch keinerlei Garantie, daß die Software einen bestimmten Zweck
		erfüllt, jemals erfüllt hat oder irgendwann einmal erfüllen wird.

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

		Für eine kommerzielle Lizenz wenden Sie sich bitte an

					support@cresd.de.

		For a commercial licence, please, contact

					support@cresd.de
*/

#ifndef DOC_CRYPTO_FILE_H
#define DOC_CRYPTO_FILE_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include "File.h"
#include "RemoteItems.h"

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

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

const gak::uint32 cryptoMagic = ('D'<<24) | ('m'<<16) | ('C'<<8) | 'r';

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

class THE_CRYPTO_FILE : public THE_FILE
{
	public:
	THE_CRYPTO_FILE( int id, const FACTORY_BASE *theFactory )
	: THE_FILE(
		id, theFactory
	)
	{
	}

	virtual TGraphic *getItemPicture( void ) const;
	virtual void createVersion( const STRING &filePath, const STRING &description );
	void createCryptedVersion( const STRING &filePath, const STRING &description )
	{
		THE_FILE::createVersion( filePath, description );
	}
	virtual STRING download( int version, bool protect, const STRING &dest );
	STRING downloadCrypted( int version, bool protect, const STRING &dest )
	{
		return THE_FILE::download( version, protect, dest );
	}
};
typedef PTR_TEMPLATE<THE_CRYPTO_FILE> PTR_CRYPTO_FILE;

class THE_REMOTE_CRYPTO_FILE : public THE_REMOTE_FILE
{
	public:
	THE_REMOTE_CRYPTO_FILE( int id, const FACTORY_BASE *theFactory )
	: THE_REMOTE_FILE(
		id, theFactory
	)
	{
		remoteData.item_type = TYPE_CRYPTO_FILE;
	}

	virtual TGraphic *getItemPicture( void ) const;
	virtual void createVersion( const STRING &filePath, const STRING &description );
	void createCryptedVersion( const STRING &filePath, const STRING &description )
	{
		THE_REMOTE_FILE::createVersion( filePath, description );
	}
	virtual STRING download( int version, bool protect, const STRING &dest );
	STRING downloadCrypted( int version, bool protect, const STRING &dest )
	{
		return THE_REMOTE_FILE::download( version, protect, dest );
	}
	virtual STRING getVersionFileName( void );
};
typedef PTR_TEMPLATE<THE_REMOTE_CRYPTO_FILE> PTR_REMOTE_CRYPTO_FILE;


// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class static data --------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- prototypes ---------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module functions ---------------------------------------------- //
// --------------------------------------------------------------------- //

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

#endif
