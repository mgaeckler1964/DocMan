/*
		Project:
		Module:
		Description:
		Author:			Martin Gäckler
		Address:		Freibadstr. 14, D-81543 München
		Phone:			+49 - 89 - 65 30 95 63
		Telefax:		+49 - 89 - 65 30 95 64
		Copyright:		CRESD GmbH
		Date:			xx. xx. 200x
*/

#ifndef THE_WEB_FOLDER_H
#define THE_WEB_FOLDER_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/string.h>
#include "Folder.h"

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

#define XMLNS_DOCMAN	"http://www.cresd.com/XSL/DocMan"
#define XMLNS_DMCONTENT	"http://www.cresd.com/XML/DocMan"

#define FLAGS_PRE_EXPORT	1
#define FLAGS_SINGLE_FILE	2
#define FLAGS_DONT_SAVE		4
#define FLAGS_WEB_SERVER	8

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

class THE_FILE;


struct XML_CACHE_ENTRY
{
	STRING		fileName;
	Document	*theDocument;
	Element		*theRoot;
};
typedef Array<XML_CACHE_ENTRY>	XML_CACHE_ENTRYS;

struct XML_MENU_MAP
{
	Element *menuElement, *menuXslTree;
};
typedef Array<XML_MENU_MAP>		XML_MENU_MAPS;

class THE_WEB_FOLDER : public THE_LOCAL_FOLDER
{
	private:
	std::ofstream		logFile;
	gak::IntStrMap		exportedFiles;
	XML_CACHE_ENTRYS	xslCache;
	Document			*theMenuDoc;

	static Element *createXMLmenu( THE_ITEM *root, int level=0 );
	static void selectMenuPath( Element *theMenuEntry, bool selected );
	static Element *findEntryInMenu(
		Element *theMenuXML, const STRING &fileID
	);

	Element *findEntryInMenu( PTR_ITEM file );
	void clearXSLcache( void );
	Element *loadXSLfile( const STRING &fileName );
	void convertReferences(
		int sourceId, Document *theResultDoc, const STRING &actFile,
		const STRING &language, int flags
	);
	void createLanguageChunk( PTR_FILE theItem, Document *theDoc );
	void createHTMLmenuChunk(
		gak::xml::XmlArray &menuElements, const STRING &styleSheetPath,
		XML_MENU_MAPS *menuMap
	);

	void testDocument( Document *theDocument )
	{
		std::auto_ptr<XML_MY_SCHEMA_MANAGER>	theManager( new XML_MY_SCHEMA_MANAGER );
		STRING errText = theManager->testDocument( theDocument );
		if( !errText.isEmpty() )
			logFile <<
				">>> Schema check failed: " <<
				theDocument->getFilename() <<
				' ' <<
				errText << '\n'
			;
	}
	STRING transformXmlFile(
		Document		*theSourceDoc,
		PTR_ITEM		file,
		STRING			fileName,
		const STRING	&localPath,
		int				flags
	);
	STRING exportFile(
		PTR_FILE theFile, const STRING &localPath, int flags,
		STRING &mimeType
	);
	STRING indexFolder(
		PTR_FOLDER childFolder, const STRING &localPath, bool forWebServer
	);
	void exportFolder( THE_FOLDER *theFolder, const STRING &localPath );
	void exportLanguageDoc( THE_LANGUAGE_DOC *theDoc, const STRING &localPath );

	public:
	THE_WEB_FOLDER(
		int id, const FACTORY_BASE *theFactory
	) : THE_LOCAL_FOLDER( id, theFactory )
	{
	}
	virtual TGraphic *getItemPicture( void ) const;
	virtual void getItemFactories( Array<const FACTORY_BASE*> *factory ) const;
	virtual bool refresh( bool recursive=true, std::ostream *stream=NULL );
	STRING generateWebFile( PTR_FILE theFile, bool forWebServer, STRING &mimeType );
	STRING generateWebFolder( PTR_FOLDER theFolder, bool forWebServer, STRING &mimeType );
};

typedef PTR_TEMPLATE<THE_WEB_FOLDER> PTR_WEB_FOLDER;

class THE_WEB_MENU_FOLDER : public THE_FOLDER
{
	public:
	THE_WEB_MENU_FOLDER(
		int id, const FACTORY_BASE *theFactory
	) : THE_FOLDER( id, theFactory )
	{
	}
	virtual TGraphic *getItemPicture( void ) const;
};

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
