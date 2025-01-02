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

#define XMLNS_DOCMAN	"http://www.gaeckler.at/XSL/DocMan"
#define XMLNS_DMCONTENT	"http://www.gaeckler.at/XML/DocMan"

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
#ifndef DOCMANBG
		std::auto_ptr<XML_MY_SCHEMA_MANAGER>	theManager( new XML_MY_SCHEMA_MANAGER );
		STRING errText = theManager->testDocument( theDocument );
		if( !errText.isEmpty() )
			logFile <<
				">>> Schema check failed: " <<
				theDocument->getFilename() <<
				' ' <<
				errText << '\n'
			;
#endif
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
