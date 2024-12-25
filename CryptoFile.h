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
const gak::uint16 cryptoVersion = ('D'<<24) | ('m'<<16) | ('C'<<8) | 'r';

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
