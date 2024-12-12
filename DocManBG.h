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

#ifndef DOCMAN_BG_H
#define DOCMAN_BG_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/directoryEntry.h>
#include <gak/indexer.h>

#include "DocManThreads.h"
#include "DocManDM.h"

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT-
#	pragma option -b
#	pragma option -a4
#	pragma option -pc
#endif

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

typedef gak::Index<StorageKey>		DocManIndex;
typedef DocManIndex::RelevantHits	DocManSearchResult;

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

#pragma option -RT-
struct DOCMAN_DIRENTRY : public gak::DirectoryEntry
{
	gak::DateTime	dbCreatedDate, dbModifiedDate;
	int				itemID;
};

class ThreadBackground : public ThreadDocMan
{
	STRING		m_state;
	STRING		m_errorText;

	void updateSyncFolders( void );
	void updateIndex( void );

	virtual const char *getTitle( void ) const;
	virtual void perform( void );

	public:
	const STRING &getError( void ) const
	{
		return m_errorText;
	}
};
#pragma option -RT.

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

void readDocManIndex( DocManIndex *index );
void writeDocManIndex( const DocManIndex &index );
void deleteDocManIndex();

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

#endif	// DOCMAN_BG_H

