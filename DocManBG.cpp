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

#include <vcl/registry.hpp>

#include <gak/fmtNumber.h>
#include <gak/threadPool.h>
#include <gak/priorityQueue.h>
#include <gak/lockQueue.h>

#pragma hdrstop

#include "DocManBG.h"
#include "DocManMain.h"
#include "folder.h"
#include "config.h"

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

using namespace gak;

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

#define getIndexFileName()	\
	F_STRING(THE_FILE::getExternalStorageBase() + "index.dat")

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

struct IndexResult : public SharedObject
{
	StorageInfo		m_source;
	StringIndex		m_indexPositions;

	IndexResult( const StorageInfo &source, const StringIndex &stringIndex )
	: m_source( source )
	{
		m_indexPositions.moveFrom(const_cast<StringIndex &>(stringIndex));
	}

	int getPriority() const
	{
		return -m_source.storageID;
	}
};

typedef SharedPointer<StorageInfo>							IndexSourcePtr;
typedef SharedObjectPointer<IndexResult>					IndexResultPtr;
typedef PointerPriorityExtractor<IndexResult, int>			PrioExtractor;
typedef PriorityQueue<IndexResultPtr, int, PrioExtractor>	IndexPriorityQueue;
typedef LockQueue<IndexResultPtr, IndexPriorityQueue>		IndexResultQueue;

namespace gak
{
	template <>
	class FixedComparator<DOCMAN_DIRENTRY>
	{
		public:
		int operator() ( const DOCMAN_DIRENTRY &t1, const DOCMAN_DIRENTRY &t2 ) const
		{
			int compareResult = t1.modifiedDate.compare( t2.modifiedDate );

			if( !compareResult )
			{
				compareResult = strcmpi( t1.fileName, t2.fileName );
			}
			return compareResult;
		}
	};

	template <>
	class ProcessorType<IndexSourcePtr>
	{
		IndexResultQueue	*m_resultQueue;
		gak::Set<STRING>	*m_stopWords;
		F_STRING			m_storageBase;

		public:
		typedef IndexSourcePtr	object_type;

		ProcessorType(
			IndexResultQueue	*resultQueue = NULL,
			gak::Set<STRING>	*stopWords = NULL,
			const F_STRING		&storageBase=F_STRING() )
		: m_storageBase( storageBase )
		{
			m_stopWords = stopWords;
			m_resultQueue = resultQueue;
		}
		void process( const IndexSourcePtr &indexSource )
		{
			doEnterFunction("ProcessorType<IndexSourcePtr>::process");
			assert( m_resultQueue );
			assert( m_stopWords );

			F_STRING	filePath = indexSource->storageFile;
			F_STRING	storageFile = m_storageBase + filePath;
			STRING		fileContent;
			fileContent.readFromFile( storageFile );
			IndexResultPtr	result = IndexResultPtr(
				new IndexResult(
					*indexSource,
					indexString( fileContent, *m_stopWords )
				)
			);

			m_resultQueue->push( result );
		}
	};
}

typedef ProcessorType<IndexSourcePtr>	IndexProcessor;
typedef ThreadPool<IndexSourcePtr>		IndexPool;

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static const int IDLE_TIMEOUT = 60000;

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

void ThreadBackground::updateIndex( void )
{
	doEnableLog();
	doEnterFunction("ThreadBackground::updateIndex");
	doDisableLog();

	static const char STATUS_VERB[] = "Update Index";
	StatusForm->pushStatus( STATUS_VERB, STATUS_VERB );

	gak::Set<STRING>	stopWords;
	DocManIndex			globalIndex;
	F_STRING			storageBase = THE_FILE::getExternalStorageBase();
	F_STRING			stopWordsFile = storageBase + "stop.words.txt";
	int 				maxStorageID = ConfigDataModule->GetValue( "indexStart", 0 );
	bool				hasLogChanged = false;
	bool				hasStopChanged = false;

	IndexResultQueue	indexResult;
	IndexProcessor		indexProcessor( &indexResult, &stopWords, storageBase );
	IndexPool			indexPool( Thread::getNumberOfCores() );

	readDocManIndex( &globalIndex );
	if( !strAccess( stopWordsFile, 0 ) )
	{
		std::ifstream	in(stopWordsFile);
		std::size_t		numWords = 0;

		while( in )
		{
			STRING word;
			in >> word;
			word.stripBlanks();

			if( !word.isEmpty() )
			{
				stopWords.addElement( word );
				numWords++;
				if( !hasStopChanged  && stopWords[stopWords.size()-1] != word )
				{
					hasStopChanged = true;
				}
			}
		}
		if( numWords != stopWords.size() )
		{
			hasStopChanged = true;
		}
	}
	if( hasStopChanged )
	{
		std::ofstream 	fp( stopWordsFile );
		fp << stopWords;
	}

	std::size_t	docReads = 0;
	indexPool.setObjectProcessor( indexProcessor );
	indexPool.start();
	StorageInfos	infos;
	DocManDataModule->loadAllStorageInfos( maxStorageID, &infos );
	for(
		StorageInfos::const_iterator it = infos.cbegin(),
			endIT = infos.cend();
		it != endIT;
		++it
	)
	{
#ifndef _DEBUG
		if( StatusForm->waitForUserSleep( IDLE_TIMEOUT ) )
		{
/*v*/		break;
		}
#endif
		const StorageInfo	&info = *it;
		STRING				mimeType = info.mimeType;
		if( mimeType.beginsWithI( "text/" ) )
		{
			++docReads;
			StatusForm->setStatus( "Indexing#1", info.storageFile );
			indexPool.process( IndexSourcePtr::makeShared( info ) );
			hasLogChanged = true;
		}
	}
	StatusForm->setStatus( STATUS_VERB, "Stoping Pool" );
	indexPool.flush();
	indexPool.shutdown();
	StatusForm->restore();
	while( indexResult.size() && !StatusForm->isTerminated() )
	{
#ifndef _DEBUG
		if( StatusForm->waitForUserSleep( IDLE_TIMEOUT ) )
		{
/*v*/		break;
		}
#endif
		STRING			title = "Indexing#2 ("
			+ formatNumber(indexResult.size())
			+ '/'
			+ formatNumber(docReads)
			+ ')'
		;
		IndexResultPtr	result = indexResult.pop();
		maxStorageID = result->m_source.storageID;
		StatusForm->setStatus( title, result->m_source.storageFile );
		globalIndex.mergeIndexPositions( result->m_source, result->m_indexPositions );
	}

	if( hasLogChanged )
	{
		StatusForm->setStatus( STATUS_VERB, "Saving Index" );
		ConfigDataModule->SetValue( "indexStart", maxStorageID );
		writeDocManIndex( globalIndex );

		if( !StatusForm->isTerminated() )
		{
			StatusForm->setStatus( STATUS_VERB, "Creating Stat" );
			StatistikData	data = globalIndex.getStatistik();

			StatusForm->setStatus( STATUS_VERB, "Saving Stat" );
			STRING	logPath = Session->PrivateDir.c_str();
			STRING	logName = logPath + DIRECTORY_DELIMITER_STRING "index.log";;
			std::ofstream 	fp( logName );
			fp << "Index:\n";
			fp << data;
			fp.close();
			ShellExecute( NULL, NULL, logName, NULL, NULL, SW_SHOW );
		}
	}
	StatusForm->restore();
	doDisableLog();
}

void ThreadBackground::updateSyncFolders( void )
{
	doEnterFunction("ThreadBackground::updateSyncFolders");

	gak::DateTime	now;
	STRING			localPath;
	bool			hasChanged;
	std::ofstream 	fp;
	STRING			logPath = Session->PrivateDir.c_str();
	STRING			logName = logPath + DIRECTORY_DELIMITER_STRING "refreshDB.log";;

	SortedArray<DOCMAN_DIRENTRY>	localDirs;

/*
select it.ID, it.NAME
from item_tree it
where it.ITEM_TYPE = 2
or it.ITEM_TYPE = 11

select it.ID, it.parentID, it.NAME, itp.name as parentname
from item_tree it
left join item_tree itp on it.parentID = itp.ID
where it.ITEM_TYPE = 2
or it.ITEM_TYPE = 11
order by it.parentID, it.ID
*/
	std::auto_ptr<TQuery> theFolderQuery( new TQuery( NULL ) );
	theFolderQuery->DatabaseName = "docManDB";
	theFolderQuery->SQL->Add(
		"select it.ID, it.NAME "
		"from item_tree it "
		"where it.ITEM_TYPE = :TypeSyncFolder "
		"or it.ITEM_TYPE = :TypeFolderRef "
	);
	theFolderQuery->Params->Items[0]->AsInteger = TYPE_SYNC_FOLDER;
	theFolderQuery->Params->Items[1]->AsInteger = TYPE_FOLDER_REF;

	/*
		collect the known directoryies
	*/
	for(
		theFolderQuery->Open();
		!theFolderQuery->Eof && !StatusForm->isTerminated();
		theFolderQuery->Next()
	)
	{
		m_state = STRING("Checking ")+theFolderQuery->Fields->Fields[1]->AsString.c_str();
		StatusForm->pushStatus(
			"Checking",
			theFolderQuery->Fields->Fields[1]->AsString.c_str()
		);

		int				currentID = theFolderQuery->Fields->Fields[0]->AsInteger;
		PTR_FOLDER_REF	theFolder = getItem( currentID );
		if( theFolder )
		{
			m_state = localPath = theFolder->getDownloadPath();
			if( !localPath.isEmpty() )
			{
				localPath.cut( localPath.strlen()-1 );

				try
				{
					DOCMAN_DIRENTRY	localStat;

					localStat.findFile( localPath );

					localStat.fileName = localPath;
					localStat.dbCreatedDate = theFolder->getCreatedDate();
					localStat.dbModifiedDate = theFolder->getModifiedDate();
					localStat.itemID = currentID;
					localDirs.addElement( localStat );
				}
				catch( ... )
				{
					doLogValue( localPath );
				}
			}
		}
#ifndef _DEBUG
		if( StatusForm->waitForUserSleep( IDLE_TIMEOUT ) )
		{
/*v*/		break;
		}
#endif
		StatusForm->restore();
	}
	theFolderQuery->Close();

	if( StatusForm->isTerminated() )
	{
/*@*/	return;
	}

	localDirs.resort();

	fp.open( logName, std::ofstream::app );
	fp << "Refresh newer: " << now << '\n';
	hasChanged = false;

	/*
		refresh directories with newer date
	*/
	for(
		size_t i=localDirs.size()-1;
		i!=-1 && !StatusForm->isTerminated();
		i--
	)
	{
		DOCMAN_DIRENTRY	&curEntry = localDirs[i];
		if( curEntry.dbModifiedDate < curEntry.modifiedDate )
		{
			int				currentID = curEntry.itemID;
			PTR_FOLDER_REF	theFolder = getItem( currentID );
			if( theFolder )
			{
				m_state = localPath = curEntry.fileName;
				StatusForm->pushStatus( "Refreshing#1", localPath );
				hasChanged = theFolder->refresh( false, &fp ) || hasChanged;
				theFolder->setCreatedDate( curEntry.creationDate.calcOriginalTime() );
				theFolder->setModifiedDate( curEntry.modifiedDate.calcOriginalTime() );
				theFolder->updateDatabase();
#ifndef _DEBUG
				if( StatusForm->waitForUserSleep( IDLE_TIMEOUT ) )
				{
/*v*/				break;
				}
#endif
				StatusForm->restore();
			}

			curEntry.dbModifiedDate = curEntry.modifiedDate;
		}
	}
	fp.close();
	if( hasChanged )
	{
		ShellExecute( NULL, NULL, logName, NULL, NULL, SW_SHOW );
	}
	if( StatusForm->isTerminated() )
	{
/*@*/	return;
	}

	/*
		refresh directories changed date
	*/
	fp.open( logName, std::ofstream::app );
	fp << "Refresh changed: " << now << '\n';
	hasChanged = false;
	for(
		size_t i=localDirs.size()-1;
		i!=-1 && !StatusForm->isTerminated();
		i--
	)
	{
		DOCMAN_DIRENTRY	&curEntry = localDirs[i];
		if( curEntry.dbModifiedDate != curEntry.modifiedDate )
		{
			int				currentID = curEntry.itemID;
			PTR_FOLDER_REF	theFolder = getItem( currentID );
			if( theFolder )
			{
				m_state = localPath = curEntry.fileName;
				StatusForm->pushStatus( "Refreshing#2", localPath );
				hasChanged = theFolder->refresh( false, &fp ) || hasChanged;
				theFolder->setCreatedDate( curEntry.creationDate.calcOriginalTime() );
				theFolder->setModifiedDate( curEntry.modifiedDate.calcOriginalTime() );
				theFolder->updateDatabase();
#ifndef _DEBUG
				if( StatusForm->waitForUserSleep( IDLE_TIMEOUT ) )
				{
/*v*/				break;
				}
#endif
				StatusForm->restore();
			}
			curEntry.dbModifiedDate = curEntry.modifiedDate;
		}
	}
	fp.close();
	if( hasChanged )
	{
		ShellExecute( NULL, NULL, logName, NULL, NULL, SW_SHOW );
	}
	if( StatusForm->isTerminated() )
	{
/*@*/	return;
	}

	/*
		refresh ALL directories
	*/
	fp.open( logName, std::ofstream::app );
	fp << "Refresh all: " << now << '\n';
	hasChanged = false;
	for(
		size_t i=localDirs.size()-1;
		i!=-1 && !StatusForm->isTerminated();
		i--
	)
	{
		const DOCMAN_DIRENTRY	&curEntry = localDirs[i];
		int						currentID = curEntry.itemID;
		PTR_FOLDER_REF			theFolder = getItem( currentID );
		if( theFolder )
		{
			m_state = localPath = curEntry.fileName;
			StatusForm->pushStatus( "Refreshing#3", localPath );
			hasChanged = theFolder->refresh( false, &fp ) || hasChanged;
			theFolder->setCreatedDate( curEntry.creationDate.calcOriginalTime() );
			theFolder->setModifiedDate( curEntry.modifiedDate.calcOriginalTime() );
			theFolder->updateDatabase();
#ifndef _DEBUG
			if( StatusForm->waitForUserSleep( IDLE_TIMEOUT ) )
			{
/*v*/			break;
			}
#endif
			StatusForm->restore();
		}
	}
	fp.close();
	if( hasChanged )
	{
		ShellExecute( NULL, NULL, logName, NULL, NULL, SW_SHOW );
	}
}

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //

const char *ThreadBackground::getTitle( void ) const
{
	return "Background Process";
}

void ThreadBackground::perform( void )
{
	doEnterFunction("ThreadBackground::perform");

	STRING						dateTime, inputTime;
	TDateTime					lastCheck = 0;
	std::auto_ptr<TRegistry>	registry( new TRegistry );

	if( registry->OpenKey( registryKey, true ) )
	{
		if( registry->ValueExists( "bgLastCheck" ) )
		{
			lastCheck = registry->ReadDate( "bgLastCheck" );
		}
	}

	try
	{
		while( !StatusForm->isTerminated() )
		{
			updateIndex();
			if( StatusForm->isTerminated() )
			{
/*v*/			break;
			}

			updateSyncFolders();
			if( StatusForm->isTerminated() )
			{
/*v*/			break;
			}

			m_state = "checkDB";
			if( (int(TDateTime::CurrentDate()) - int(lastCheck)) >= 7 )
			{
				DocManDataModule->checkDB( true );
				lastCheck = TDateTime::CurrentDate();
#ifndef _DEBUG
				registry->WriteDate( "bgLastCheck", lastCheck );
#endif
			}
			if( StatusForm->isTerminated() )
			{
/*v*/			break;
			}
			clearItemCache();

			m_state = "Sleep";
			StatusForm->pushStatus( "Sleeping", "" );
			do
			{
				gak::DateTime	now;
				dateTime = now.getLocalTime();

				gak::Time		inpTime( GetLastInputTime() );
				inputTime = inpTime.toString();

				StatusForm->pushStatus( "Sleeping", dateTime + " Idle for: " + inputTime );
				Sleep( 600000 );
				StatusForm->restore();
			}
			while( GetLastInputTime() > 600000 && !StatusForm->isTerminated() );

			StatusForm->restore();
		}
	}
	catch( Exception &e )
	{
		m_errorText = m_state + '\n' + e.Message.c_str();
	}
	catch( std::exception &e )
	{
		m_errorText = m_state + '\n' + e.what();
	}
	catch( ... )
	{
		m_errorText = m_state + "\nAn unkown error had occured";
	}
}

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

void readDocManIndex( DocManIndex *index )
{
	F_STRING indexFile = getIndexFileName();
	try
	{
		if( !strAccess( indexFile, 0 ) )
		{
			readFromFile( indexFile, index, 0x8456 );
		}
	}
	catch( std::exception &e )
	{
		strRemove( indexFile );
		throw Exception( e.what() );
	}
}

void writeDocManIndex( const DocManIndex &index )
{
	F_STRING indexFile = getIndexFileName();
	writeToFile( indexFile, index, 0x8456, ovmShortDown );
}

void deleteDocManIndex()
{
	F_STRING indexFile = getIndexFileName();
	strRemove(indexFile);
	ConfigDataModule->SetValue( "indexStart", 0 );
}

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif
