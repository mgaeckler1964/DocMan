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


// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/xslt.h>
#include <gak/numericString.h>

#include "WebFolder.h"
#include "DocManDM.h"
#include "SyncFolderCreateFrm.h"
#include "AboutOpenSSLfrm.h"

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

class FACTORY_WEB_FOLDER : public FACTORY_LOCAL_FOLDER
{
	virtual int getItemType( void ) const;
	virtual bool acceptParent( const THE_ITEM *parent ) const;
	virtual PTR_ITEM createItem( int id ) const;
	virtual const char *getName( void ) const;
	virtual TItemCreateForm *getForm( void ) const;
	virtual PTR_ITEM createItemFromForm( const PTR_ITEM &parentId ) const;
};

class FACTORY_WEB_MENU_FOLDER : public FACTORY_FOLDER
{
	virtual int getItemType( void ) const;
	virtual bool acceptParent( const THE_ITEM *parent ) const;
	virtual PTR_ITEM createItem( int id ) const;
	virtual const char *getName( void ) const;
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static FACTORY_WEB_FOLDER		theWebFolderFactory;
static FACTORY_WEB_MENU_FOLDER	theWebMenuFolderFactory;
static STRING					copyrightNote;

// --------------------------------------------------------------------- //
// ----- class static data --------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- prototypes ---------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module functions ---------------------------------------------- //
// --------------------------------------------------------------------- //

inline void setupCopyrightNote( void )
{
	if( copyrightNote.isEmpty() )
	{
		copyrightNote =
			(STRING)"\nCreated with DocMan " + AboutOpenSSLForm->getVersionString() + ".\n" +
			AboutOpenSSLForm->getLegalCopyRight() + ".\n"
			"http://www.cresd.de/\n"
			"====================\n"
			"DO NOT EDIT\n"
		;
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

xml::Element *THE_WEB_FOLDER::createXMLmenu( THE_ITEM *root, int level )
{
	doEnterFunction( "THE_WEB_FOLDER::createXMLmenu" );

	size_t	spacePos = 0;
	STRING	tag = root->getItemTypeString();
	STRING	mimeType;

	// remove all blanks
	while( 1 )
	{
		spacePos = tag.searchChar( ' ', spacePos );
		if( spacePos != (size_t)-1 )
		{
			tag = tag.leftString( spacePos ) + tag.subString( spacePos+1 );
		}
		else
		{
			break;
		}
	}

	xml::Any	*theElement = new xml::Any( tag );

	theElement->setIntegerAttribute( "ID", root->getID() );
	theElement->setStringAttribute( "name", root->getName() );
	theElement->setIntegerAttribute( "menuLevel", level );
	theElement->setBoolAttribute( "selected", false );
	theElement->setBoolAttribute( "visible", !level );

	int	oldSortOrder = root->sort( SORT_ORDER );

	ITEM_CONTENT *content = root->getContent();

	for(
		ITEM_CONTENT::const_iterator it = content->cbegin(),
			endIT = content->cend();
		it != endIT;
		++it
	)
	{
		mimeType = "";
		THE_ITEM *theChild = *it;
		THE_FILE *theFile = dynamic_cast<THE_FILE*>(theChild);
		if( theFile )
		{
			mimeType = theFile->getMimeType();
		}
		else if( dynamic_cast<THE_WEB_MENU_FOLDER*>(theChild) )
		{
			mimeType = "text/html";
		}

		if( mimeType == "text/html"	||mimeType == "text/xml" )
		{
			STRING	name = theChild->getName();
			if( !name.beginsWith( "index." ) )
			{
				theElement->addObject( createXMLmenu(theChild, level+1) );
			}
		}
	}

	root->sort( oldSortOrder );

	return theElement;
}

xml::Element *THE_WEB_FOLDER::findEntryInMenu(
	xml::Element *theMenuXML, const STRING &fileID
)
{
	doEnterFunction(
		"THE_WEB_FOLDER::findEntryInMenu(xml::Element * - const STRING &)"
	);
	xml::Element *theMenuEntry = NULL;

	for( size_t i=0; !theMenuEntry && i<theMenuXML->getNumObjects(); i++ )
	{
		xml::Element *subMenu = theMenuXML->getElement( i );
		if( subMenu->getAttribute( "ID" ) == fileID )
			theMenuEntry = subMenu;
		else
			theMenuEntry = findEntryInMenu( subMenu, fileID );
	}

	return theMenuEntry;
}

void THE_WEB_FOLDER::selectMenuPath( xml::Element *theMenuEntry, bool selected )
{
	while( theMenuEntry )
	{
		theMenuEntry->setBoolAttribute( "selected", selected );
		/*
			make all children visible (or not)
		*/
		for( size_t i=0; i<theMenuEntry->getNumObjects(); i++ )
		{
			theMenuEntry->getElement(i)->setBoolAttribute( "visible", selected );
		}

		theMenuEntry = theMenuEntry->getParent();
		/*
			make all siblings visible (or not)
		*/
		if( theMenuEntry && theMenuEntry->getParent() )
		{
			for( size_t i=0; i<theMenuEntry->getNumObjects(); i++ )
			{
				theMenuEntry->getElement(i)->setBoolAttribute( "visible", selected );
			}
		}
	}
}

// --------------------------------------------------------------------- //
// ----- class privates ------------------------------------------------ //
// --------------------------------------------------------------------- //

xml::Element *THE_WEB_FOLDER::findEntryInMenu( PTR_ITEM file )
{
	doEnterFunction("THE_WEB_FOLDER::findEntryInMenu( PTR_ITEM file )");
	xml::Element*theMenuEntry;
	STRING		fileID;

	while( 1 )
	{
		fileID = formatNumber( file->getID() );

		theMenuEntry = findEntryInMenu(
			theMenuDoc, fileID
		);
		if( theMenuEntry )
/*v*/		break;

		file = file->getParent();
		if( !file )
/*v*/		break;
	}

	return theMenuEntry;
}

void THE_WEB_FOLDER::clearXSLcache( void )
{
	for(
		XML_CACHE_ENTRYS::const_iterator it = xslCache.cbegin(),
			endIT = xslCache.cend();
		it != endIT;
		++it
	)
	{
		const XML_CACHE_ENTRY &theEntry = *it;
		delete theEntry.theDocument;
	}

	xslCache.clear();
}

xml::Element *THE_WEB_FOLDER::loadXSLfile( const STRING &fileName )
{
	doEnterFunction("THE_WEB_FOLDER::loadXSLfile");

	for(
		XML_CACHE_ENTRYS::const_iterator it = xslCache.cbegin(),
			endIT = xslCache.cend();
		it != endIT;
		++it
	)
	{
		const XML_CACHE_ENTRY &theEntry = *it;
		if( theEntry.fileName == fileName )
		{
/***/		return theEntry.theRoot;
		}
	}

	XML_CACHE_ENTRY	newValue;

	newValue.theDocument = loadXmlDoc( fileName );
	if( newValue.theDocument )
	{
		testDocument( newValue.theDocument );

		newValue.theRoot = newValue.theDocument->getElement(
			"stylesheet", xml::XMLNS_XSLT
		);
		if( newValue.theRoot )
		{
			newValue.fileName = fileName;
			xslCache.addElement( newValue );
/***/		return newValue.theRoot;
		}

		delete newValue.theDocument;
	}

	return NULL;
}

void THE_WEB_FOLDER::convertReferences(
	int sourceID, xml::Document *theResultDoc, const STRING &actFile,
	const STRING &language, int flags
)
{
	doEnterFunction("THE_WEB_FOLDER::convertReferences");
	STRING	dummy;

	if( (flags & FLAGS_SINGLE_FILE) || (flags & FLAGS_WEB_SERVER) )
		flags |= FLAGS_DONT_SAVE;
	flags |= FLAGS_PRE_EXPORT;

	doLogValue( theResultDoc->generate( xml::XML_MODE ) );

	if( StatusForm->pushStatus( "Resolve", actFile ) )
	{
/*@*/	return;
	}

	const char		*htmlTag, *attrName;
	STRING			ref;
	int				id;
	xml::Element	*theLink;
	xml::XmlArray	theElements;
	std::size_t		tagsIdx, attrIdx;
	static const char *tags[][3] =
	{
		{ "a", "href", NULL },
		{ "link", "href", NULL },
		{ "img", "src", NULL },
		{ "script", "src", NULL },
		{ "audio", "src", NULL },
		{ NULL }
	};

	for(
		tagsIdx=0;
		!StatusForm->isTerminated() && (htmlTag = tags[tagsIdx][0]) != NULL;
		tagsIdx++
	)
	{
		theResultDoc->getAllElements( &theElements, htmlTag, XMLNS_DOCMAN );

		for(
			xml::XmlArray::const_iterator it = theElements.cbegin(),
				endIT = theElements.cend();
			it != endIT && !StatusForm->isTerminated();
			++it
		)
		{
			theLink = *it;
			for(
				attrIdx=1;
				(attrName = tags[tagsIdx][attrIdx]) != NULL;
				attrIdx++
			)
			{
				ref = theLink->getAttribute( attrName );
				if( ref.beginsWith('{') && ref.endsWith( '}' ) )
				{
					if( !(flags & FLAGS_WEB_SERVER) )
					{
						ref = ref.subString( 1, ref.strlen()-2 );

						id = ref.getValueE<unsigned>();
						if( id == sourceID )
							ref = '#';
						else
						{
							ref = exportedFiles[id];
							if( ref.isEmpty() )
							{
								PTR_ITEM	theItem = getItem( id );
								if( theItem )
								{
									PTR_FILE	theFile = theItem;
									if( (THE_FILE*)theFile )
									{
										STRING localPath = theFile->getDownloadPath();
										makePath( localPath );
										ref = exportFile( theFile, localPath, flags, dummy );
									}
									else
									{
										THE_FOLDER *theFolder
											= dynamic_cast<THE_FOLDER*>(
												static_cast<THE_ITEM*>(theItem)
											)
										;
										if( theFolder )
										{
											ref = theFolder->getDownloadPath();
										}
										else
										{
											PTR_LANGUAGE_DOC theLangDoc = theItem;
											if( theLangDoc )
											{
												theFile = theLangDoc->getContentItem( language );
												if( !theFile )
												{
													int tmp=0;
													theFile = theLangDoc->getContentItem( tmp );
												}
												if( theFile && theFile->getID() != sourceID )
												{
													STRING localPath = theFile->getDownloadPath();
													makePath( localPath );
													ref = exportFile( theFile, localPath, flags, dummy );
												}
												else
												{
													ref = '#';
												}
											}
											else
											{
												logFile <<
													"Reference to " <<
													id <<
													" in " <<
													actFile <<
													" solved to bad type.\n"
												;
												ref = '#';
											}
										}
									}
								}
								else
								{
									logFile << "Reference to " << id << " in " <<
										actFile << " could not be resolved.\n";
								}
							}
							if( ref[0] != '#' )
							{
								ref = makeRelPath( actFile, ref );
							}
							if( ref.isEmpty() )
							{
								ref = "./";
							}
							else
							{
								ref.replaceChar( DIRECTORY_DELIMITER, '/' );
								size_t	spacePos = 0;
								while( 1 )
								{
									spacePos = ref.searchChar( ' ', spacePos );
									if( spacePos != (size_t)-1 )
										ref.replaceText( spacePos, 1, "%20" );
									else
										break;
								}
							}
						}

						theLink->setStringAttribute( attrName, ref );
					}
					else
					{
						ref.insStr( 0, "/" );
						theLink->setStringAttribute( attrName, ref );
					}
				}
			}

			theLink->setTag( htmlTag );
		}
	}
	doLogValue( theResultDoc->generate( xml::XML_MODE ) );

	StatusForm->restore();
}

void THE_WEB_FOLDER::createLanguageChunk( PTR_FILE theItem, xml::Document *theDoc )
{
	xml::Element	*theAnchor, *theRoot, *menuElement, *parentHtml, *xslStylesheet;
	xml::XmlArray	menuElements;
	PTR_FILE		child;
	STRING			href, stylesheet;
	PTR_ITEM		theParent = (*theItem).getParent();
	ITEM_CONTENT	*theContent = (*theParent).getContent();

	theDoc->getAllElements(
		&menuElements, "language", XMLNS_DOCMAN
	);
	for(
		xml::XmlArray::const_iterator it = menuElements.cbegin(),
			endIT = menuElements.cend();
		it != endIT;
		++it
	)
	{
		theRoot = new xml::Any( "div" );
		theRoot->setStringAttribute( "class", "languageLink" );

		for( size_t i=0; i<theContent->size(); i++ )
		{
			child = theParent->getContentItem( i );

			theAnchor = theRoot->addObject( new xml::Any( "a" ) );
			theAnchor->setStringAttribute( "class", "languageLink" );

			if( child->getID() != theItem->getID() )
			{
				href = (*child).getDownloadFile( "." );
				href += (size_t)2;
				theAnchor->setStringAttribute( "href", href );
			}

			theAnchor->addObject( new xml::PCData( child->getName() ) );
		}

		menuElement = *it;
		stylesheet = menuElement->getAttribute( "stylesheet" );
		if( !stylesheet.isEmpty() )
		{
			stylesheet = makeFullPath( (*theItem).getPath(), stylesheet );
			xslStylesheet = loadXSLfile( stylesheet );
			if( xslStylesheet )
			{
				xml::Document	theSourceDoc("");
				theSourceDoc.addObject( theRoot );
				xml::Transformator	theXslProcessor(
					&theSourceDoc, xslStylesheet
				);
				std::auto_ptr<xml::Document> theResultDoc(
					theXslProcessor.transform()
				);
				theRoot = theResultDoc->getRoot();
				theResultDoc->removeObject( theRoot );
			}
		}

		parentHtml = menuElement->getParent();
		parentHtml->replaceObject( menuElement, theRoot );

		delete menuElement;
	}
}

void THE_WEB_FOLDER::createHTMLmenuChunk(
	xml::XmlArray &menuElements, const STRING &styleSheetPath,
	XML_MENU_MAPS *menuMap
)
{
	doEnterFunction("THE_WEB_FOLDER::createHTMLmenuChunk");

	for(
		xml::XmlArray::const_iterator it = menuElements.cbegin(),
			endIT = menuElements.cend();
		!StatusForm->isTerminated() && it != endIT;
		++it
	)
	{
		xml::Element *theMenuStyle = *it;

		STRING	menuStyleSheetPath
			= theMenuStyle->getAttribute( "stylesheet" )
		;

		if( !menuStyleSheetPath.isEmpty() )
		{
			menuStyleSheetPath = makeFullPath(
				styleSheetPath,
				menuStyleSheetPath
			);
			xml::Element *menuStyleSheet = loadXSLfile(
				menuStyleSheetPath
			);
			if( menuStyleSheet )
			{
				/*
					transform the menu XML to an HTML chunk
				*/
				xml::Transformator	theXslProcessor(
					theMenuDoc, menuStyleSheet
				);
				std::auto_ptr<xml::Document> theMenuResultDoc(
					theXslProcessor.transform()
				);
				if( theMenuResultDoc.get() )
				{
					xml::Element *theMenuResult = theMenuResultDoc->getRoot();
					if( theMenuResult )
					{
						/*
							replace the menu entry in the
							document stylesheet by the created
							menu HTML chunh
						*/
						theMenuResult->remove();

						theMenuStyle->getParent()->replaceObject(
							theMenuStyle, theMenuResult
						);
						XML_MENU_MAP	newMap;
						newMap.menuElement = theMenuStyle;
						newMap.menuXslTree = theMenuResult;
						menuMap->addElement( newMap );

#ifdef _DEBUG
						logFile << theMenuResult->generate( xml::XML_MODE );
#endif
					}
					else
					{
						logFile << "Menu transformation returned an empty doc";
						logFile << theMenuDoc->generate( xml::XML_MODE ) << '\n';
						logFile << menuStyleSheet->generate( xml::XML_MODE ) << '\n';
					}
				}
				else
				{
					logFile << "Menu transformation returned no doc";
					logFile << theMenuDoc->generate( xml::XML_MODE ) << '\n';
					logFile << menuStyleSheet->generate( xml::XML_MODE ) << '\n';
				}
			}
			else
			{
				logFile << "No stylesheet in " << menuStyleSheetPath << " found.\n";
			}
		}
	}
}

STRING THE_WEB_FOLDER::transformXmlFile(
	xml::Document	*theSourceDoc,
	PTR_ITEM		file,
	STRING			fileName,
	const STRING	&localPath,
	int				flags
)
{
	doEnterFunction( "THE_WEB_FOLDER::transformXmlFile" );

	STRING	downloadPath;

	testDocument( theSourceDoc );

	STRING styleSheetType;
	STRING styleSheetPath = theSourceDoc->getStyleSheet( &styleSheetType );
	if( !styleSheetPath.isEmpty() && styleSheetType == "text/xsl" )
	{
		#if DIRECTORY_DELIMITER != '/'
			styleSheetPath.replaceChar( '/', DIRECTORY_DELIMITER );
		#endif
		styleSheetPath = makeFullPath(
			theSourceDoc->getFilename(), styleSheetPath
		);
		xml::Element *stylesheet = loadXSLfile( styleSheetPath );
		if( stylesheet )
		{
			/*
				search for menu elements in our XSL and replace
				them with the XSL transformed generated menu (HTML Chunk)
			*/

			xml::XmlArray	menuElements;
			XML_MENU_MAPS	menuMap;

			stylesheet->getAllElements(
				&menuElements, "menu", XMLNS_DOCMAN
			);

			if( menuElements.size() )
			{
				/*
					select the breadcrumb in our menu xml
				*/
				xml::Element *myMenuEntry = findEntryInMenu( file );
				if( myMenuEntry )
					selectMenuPath( myMenuEntry, true );

				createHTMLmenuChunk(
					menuElements, styleSheetPath,
					&menuMap
				);

				/*
					deselect the breadcrumb in our menu xml
				*/
				if( myMenuEntry )
				{
					selectMenuPath( myMenuEntry, false );
				}
			}

			/*
				now convert the XML source to the final result
			*/
			xml::Transformator	theXslProcessor(
				theSourceDoc, stylesheet
			);
			std::auto_ptr<xml::Document> theResultDoc(
				theXslProcessor.transform()
			);

#ifdef _DEBUG
			logFile << "\n---theSourceDoc---\n===================================================\n";
			logFile << theSourceDoc->generate( xml::XML_MODE );
			logFile << "\n---stylesheet---\n===================================================\n";
			logFile << stylesheet->generate( xml::XML_MODE );
			logFile << "\n---theResultDoc---\n===================================================\n";
			logFile << theResultDoc->generate( xml::XML_MODE );
			logFile << "\n==================================================================================================================\n";
#endif
			/*
				restore the original style sheet
			*/
			for(
				XML_MENU_MAPS::const_iterator it = menuMap.cbegin(),
					endIT = menuMap.cend();
				it != endIT;
				++it
			)
			{
				const XML_MENU_MAP	&map = *it;

				map.menuXslTree->getParent()->replaceObject(
					map.menuXslTree, map.menuElement
				);
				/*
					delete the menu HTML chunk
				*/
				delete map.menuXslTree;
			}

			if( theResultDoc.get() )
			{
				STRING styleSheetType;
				styleSheetPath = theResultDoc->getStyleSheet( &styleSheetType );
				if( !styleSheetPath.isEmpty()  && styleSheetType == "text/xsl" )
					downloadPath = transformXmlFile(
						theResultDoc.get(), file, fileName, localPath, flags
					);
				else
				{
					downloadPath = localPath;
					size_t	dotPos = fileName.searchRChar('.');
					if( dotPos != (size_t)-1 )
						fileName.cut( dotPos );
					if(
						dynamic_cast<html::Document*>(
							theResultDoc.get()
						)
					)
						fileName += ".html";
					else
						fileName += ".xml";

					if( !downloadPath.endsWith( DIRECTORY_DELIMITER ) )
						downloadPath += DIRECTORY_DELIMITER;

					downloadPath += fileName;

					if( !(flags & FLAGS_DONT_SAVE) )
					{
						if( !(flags & FLAGS_PRE_EXPORT) )
						{
							convertReferences(
								file->getID(), theResultDoc.get(), downloadPath,
								file->getName(),
								flags
							);
						}

						setupCopyrightNote();
						xml::Element *theCopyright = theResultDoc->addObject(
							new xml::Comment( copyrightNote )
						);
						theCopyright->moveTo( 0 );
						/*
							save the resulting XML/HTML to the destination path
						*/
						STRING	result = theResultDoc->generateDoc();
						if( result.isEmpty() )
						{
							result = "<html><body><p>Empty File</p></body></html>\n";
							logFile << "Empty result " << downloadPath << ".\n";
							logFile << theSourceDoc->generate( xml::XML_MODE ) << '\n';
							logFile << stylesheet->generate( xml::XML_MODE ) << '\n';
						}
						result.writeToFile( downloadPath );
					}
				}
			}
			else
			{
				logFile << "Document transformation returned no doc";
				logFile << theSourceDoc->generate( xml::XML_MODE ) << '\n';
				logFile << stylesheet->generate( xml::XML_MODE ) << '\n';
			}
		}
		else
		{
			logFile << "No stylesheet in " << styleSheetPath << " found.\n";
		}
	}

	return downloadPath;
}


STRING THE_WEB_FOLDER::exportFile(
	PTR_FILE file, const STRING &localPath, int flags,
	STRING &mimeType
)
{
	doEnterFunction("THE_WEB_FOLDER::exportFile");
	logFile << ((flags & FLAGS_PRE_EXPORT) ? "Prepare:  " : "Export: " )
			<< (*file).getName()
			<< " to "
			<< localPath << '\n';

	if( StatusForm->pushStatus(
		(flags & FLAGS_PRE_EXPORT) ? "Preparing" : "Exporting", file->getName() )
	)
	{
/*@*/	return "";
	}

	STRING downloadPath = exportedFiles[(*file).getID()];
	if( !downloadPath.isEmpty() )
	{
		if( !(flags & FLAGS_PRE_EXPORT) && !(flags & FLAGS_DONT_SAVE) )
		{
			if( downloadPath.endsWith( ".xml" )
			||  downloadPath.endsWith( ".html" )
			||  downloadPath.endsWith( ".htm" ) )
			{
				// do post processing not yet done
				doLogValue( downloadPath );
				html::Parser	theParser( downloadPath );
				std::auto_ptr<html::Document> theResultDoc(
					theParser.readFile( false )
				);
				theParser.closeStream();
				if( theResultDoc.get() )
				{
					convertReferences(
						file->getID(), theResultDoc.get(), downloadPath,
						file->getName(),
						flags
					);

					STRING	result = theResultDoc->generateDoc();
					if( result.isEmpty() )
					{
						result = "<html><body><p>Empty File</p></body></html>\n";
						logFile << "Empty result " << downloadPath << ".\n";
					}

					result.writeToFile( downloadPath );
				}
			}
		}
	}
	else
	{
		std::auto_ptr<html::Document>	theHtmlDoc( file->getHtmlDocument() );
		if( theHtmlDoc.get() )
		{
			downloadPath = file->getDownloadFile( localPath );

			if( !(flags & FLAGS_DONT_SAVE) )
			{
				testDocument( theHtmlDoc.get() );
				createLanguageChunk( file, theHtmlDoc.get() );
				if( !(flags & FLAGS_PRE_EXPORT) )
					convertReferences(
						file->getID(), theHtmlDoc.get(), downloadPath,
						file->getName(),
						flags
					);

				setupCopyrightNote();
				xml::Element *theCopyright = theHtmlDoc->addObject(
					new xml::Comment( copyrightNote )
				);
				theCopyright->moveTo( 0 );

				STRING	result = theHtmlDoc->generateDoc();
				if( result.isEmpty() )
				{
					result = "<html><body><p>Empty File</p></body></html>\n";
					logFile << "Empty result " << downloadPath << ".\n";
				}

				result.writeToFile( downloadPath );
			}
		}
		else
		{
			std::auto_ptr<xml::Document>	theXmlDoc( file->getXmlDocument() );
			if( theXmlDoc.get() )
			{
				downloadPath = transformXmlFile(
					theXmlDoc.get(), file, file->getName(), localPath, flags
				);
			}


			if( downloadPath.isEmpty() )
			{
				downloadPath = localPath;
				downloadPath += file->getName();
				if( !(flags & FLAGS_DONT_SAVE) )
					file->download( 0, false, downloadPath );
			}
		}

		exportedFiles[file->getID()] = downloadPath;
	}

	if( downloadPath.endsWith( ".xml" ) )
		mimeType = "text/xml";
	else if( downloadPath.endsWith( ".html" ) ||  downloadPath.endsWith( ".htm" ) )
		mimeType = "text/html";

	StatusForm->restore();
	return downloadPath;
}

void THE_WEB_FOLDER::exportLanguageDoc( THE_LANGUAGE_DOC *theDoc, const STRING &localPath )
{
	if( StatusForm->pushStatus( "Refreshing", (*theDoc).getName() ) )
	{
/*@*/	return;
	}
    STRING	dummy;

	ITEM_CONTENT *theContent = theDoc->getContent();
	for(
		ITEM_CONTENT::const_iterator it = theContent->cbegin(),
			endIT = theContent->cend();
		!StatusForm->isTerminated() && it != endIT;
		++it
	)
	{
		const PTR_ITEM	&child = *it;
		PTR_FILE		file = child;
		if( file )
		{
			exportFile( file, localPath, 0, dummy );
		}
	}

	StatusForm->restore();
}

STRING THE_WEB_FOLDER::indexFolder(
	PTR_FOLDER childFolder, const STRING &localPath,
	bool forWebServer
)
{
	doEnterFunction("THE_WEB_FOLDER::indexFolder");

	STRING	indexFile;

	PTR_ITEM indexXsl = (*childFolder).getContentItem( "index.xsl" );
	if( indexXsl )
	{
		PTR_ITEM indexXml = (*childFolder).getContentItem( "index.xml" );
		if( !indexXml )
		{
			STRING	indexXMLfile = (*childFolder).getPath() +
					DIRECTORY_DELIMITER_STRING "index.xml"
			;

			xml::Document	xmlDoc( indexXMLfile );
			xmlDoc.addObject( new xml::Declaration );
			xmlDoc.addObject( new xml::StyleSheet( "index.xsl" ) );
			xmlDoc.addObject( (*childFolder).createXML( 1 ) );
			indexXMLfile = localPath +
				DIRECTORY_DELIMITER_STRING "index.xml"
			;
			STRING	xmlIndex = xmlDoc.generate( xml::XML_MODE );
			xmlIndex.writeToFile( indexXMLfile );

			indexFile = transformXmlFile(
				&xmlDoc, childFolder, "index", localPath,
				forWebServer ? FLAGS_WEB_SERVER : 0
			);
		}
	}

	return indexFile;
}

void THE_WEB_FOLDER::exportFolder( THE_FOLDER *folder, const STRING &localPath )
{
	doEnterFunction("THE_WEB_FOLDER::exportFolder");

	if( StatusForm->pushStatus( "Refreshing", (*folder).getName() ) )
	{
/*@*/	return;
	}
	STRING	dummy;

	ITEM_CONTENT *theContent = folder->getContent();
	for(
		ITEM_CONTENT::const_iterator it = theContent->cbegin(),
			endIT = theContent->cend();
		!StatusForm->isTerminated() && it != endIT;
		++it
	)
	{
		const PTR_ITEM	&child = *it;
		PTR_FILE		file = child;
		if( file )
		{
			exportFile( file, localPath, 0, dummy );
		}
		else
		{
			PTR_FOLDER	childFolder = child;
			if( childFolder )
			{
				STRING	subPath = localPath;
				subPath += (*childFolder).getName();
				mkdir( subPath );
				subPath += DIRECTORY_DELIMITER;

				if( StatusForm->pushStatus( "Indexing", (*childFolder).getName() ) )
				{
/*@*/				return;
				}
				indexFolder( childFolder, subPath, false );
				if( StatusForm->restore() )
				{
/*@*/				return;
				}
				exportFolder( childFolder, subPath );
			}
			else
			{
				THE_LANGUAGE_DOC	*theDoc = dynamic_cast<THE_LANGUAGE_DOC*>(
					static_cast<THE_ITEM*>(child)
				);
				if( theDoc )
					exportLanguageDoc( theDoc, localPath );
			}
		}
	}

	StatusForm->restore();
}

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //
   
int FACTORY_WEB_FOLDER::getItemType( void ) const
{
	return (int)TYPE_WEB_FOLDER;
}

int FACTORY_WEB_MENU_FOLDER::getItemType( void ) const
{
	return (int)TYPE_WEB_MENU_FOLDER;
}

bool FACTORY_WEB_FOLDER::acceptParent( const THE_ITEM *_parent ) const
{
	PTR_ITEM	parent((THE_ITEM *)_parent);

	if( (*parent).getID() <= 0 )
		return false;	// Don't accept the root

	const THE_FOLDER *theFolder = dynamic_cast<const THE_FOLDER *>(
		static_cast<THE_ITEM*>(parent)
	);
	bool acceptable = theFolder ? true : false;

	while( acceptable && parent )
	{
		if(
			dynamic_cast<const THE_LOCAL_FOLDER *>(
				static_cast<THE_ITEM*>(parent)
			)
		)
			acceptable = false;
		else
			parent = (*parent).getParent();
	}

	return acceptable;
}

bool FACTORY_WEB_MENU_FOLDER::acceptParent( const THE_ITEM *parent ) const
{
	const THE_WEB_FOLDER *theWebFolder
		= dynamic_cast<const THE_WEB_FOLDER *>(parent)
	;
	if( !theWebFolder )
	{
		const THE_WEB_MENU_FOLDER *theMenuFolder
			=  dynamic_cast<const THE_WEB_MENU_FOLDER *>(parent)
		;
		if( !theMenuFolder )
/*@*/		return false;
	}

	return true;
}

PTR_ITEM FACTORY_WEB_FOLDER::createItem( int id ) const
{
	return new THE_WEB_FOLDER( id, this );
}

PTR_ITEM FACTORY_WEB_MENU_FOLDER::createItem( int id ) const
{
	return new THE_WEB_MENU_FOLDER( id, this );
}

const char *FACTORY_WEB_FOLDER::getName( void ) const
{
	return "Web Folder";
}

const char *FACTORY_WEB_MENU_FOLDER::getName( void ) const
{
	return "Menu Folder";
}

TItemCreateForm *FACTORY_WEB_FOLDER::getForm( void ) const
{
	return SyncFolderCreateForm;
}

PTR_ITEM FACTORY_WEB_FOLDER::createItemFromForm( const PTR_ITEM &parent ) const
{
	PTR_ITEM newFolder = createItem( 0 );
	THE_WEB_FOLDER *fp
		= (THE_WEB_FOLDER *)static_cast<THE_ITEM *>(newFolder)
	;

	fp->setData(
		parent,
		SyncFolderCreateForm->EditName->Text.c_str(),
		SyncFolderCreateForm->MemoDescription->Text.c_str(),
		SyncFolderCreateForm->EditLocalPath->Text.c_str()
	);
	fp->updateDatabase();

	return newFolder;
}

TGraphic *THE_WEB_FOLDER::getItemPicture( void ) const
{
	static Graphics::TBitmap *thePic = NULL;

	if( !thePic )
	{
		thePic = new Graphics::TBitmap();
		thePic->LoadFromResourceName((unsigned int)HInstance, "WEB_FOLDER_BITMAP");
	}
	return thePic;
}

TGraphic *THE_WEB_MENU_FOLDER::getItemPicture( void ) const
{
	static Graphics::TBitmap *thePic = NULL;

	if( !thePic )
	{
		thePic = new Graphics::TBitmap();
		thePic->LoadFromResourceName((unsigned int)HInstance, "WEB_MENU_FOLDER_BITMAP");
	}
	return thePic;
}


bool THE_WEB_FOLDER::refresh( bool , ostream * )
{
	doEnterFunction("THE_WEB_FOLDER::refresh");

	STRING localPath = getDownloadPath();

	if( localPath.isEmpty() )
	{
/*@*/	throw Exception( "Don't know local path" );
	}


	STRING	logFilePath = localPath;
	logFilePath += getName();
	logFilePath += ".log";

	logFile.open( logFilePath );
	logFile << "Exporting Web Folder " << getName() << '\n';

	clearXSLcache();
	exportedFiles.clear();

	theMenuDoc = new xml::Document( getPath() );
	theMenuDoc->addObject( new xml::Declaration );
	theMenuDoc->addObject( createXMLmenu( this ) );

	STRING	menuXMLfile = localPath;
	menuXMLfile += DIRECTORY_DELIMITER;
	menuXMLfile += "menu.xml";
	STRING	xmlMenu = theMenuDoc->generate( xml::XML_MODE );
	xmlMenu.writeToFile( menuXMLfile );

	exportFolder( this, localPath );

	delete theMenuDoc;

	exportedFiles.clear();
	clearXSLcache();

	logFile.close();
	ShellExecute( NULL, NULL, logFilePath, NULL, NULL, SW_SHOWDEFAULT );

	return false;
}

void THE_WEB_FOLDER::getItemFactories( Array<const FACTORY_BASE*> *factory ) const
{
	THE_FOLDER::getItemFactories( factory );
}

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

STRING THE_WEB_FOLDER::generateWebFile(
	PTR_FILE theFile, bool forWebServer, STRING &mimeType
)
{
	doEnterFunction("THE_WEB_FOLDER::generateWebFile");

	STRING	exportedFile;
	STRING localPath = forWebServer
		? STRING( getenv( "TEMP" ) )
		: getDownloadPath()
	;

	if( localPath.isEmpty()  )
	{
/*@*/	throw Exception( "Don't know local path" );
	}

	if( !localPath.endsWith( DIRECTORY_DELIMITER ) )
		localPath += DIRECTORY_DELIMITER;

	if( mimeType.isEmpty() )
		mimeType = theFile->getMimeType();

	STRING	logFilePath = localPath;
	logFilePath += getName();
	logFilePath += ".log";

	logFile.open( logFilePath );
	logFile << "Exporting Web Folder " << getName() << '\n';

	clearXSLcache();
	exportedFiles.clear();

	if( mimeType == "text/xml" )
	{
		theMenuDoc = new xml::Document( getPath() );
		theMenuDoc->addObject( new xml::Declaration );
		theMenuDoc->addObject( createXMLmenu( this ) );

		STRING	menuXMLfile = localPath;
		menuXMLfile += DIRECTORY_DELIMITER;
		menuXMLfile += "menu.xml";
		STRING	xmlMenu = theMenuDoc->generate( xml::XML_MODE );
		xmlMenu.writeToFile( menuXMLfile );
	}

	exportedFile = exportFile(
		theFile, localPath,
		forWebServer ? FLAGS_WEB_SERVER : FLAGS_SINGLE_FILE, mimeType
	);

	delete theMenuDoc;

	exportedFiles.clear();
	clearXSLcache();

	logFile.close();

	if( !forWebServer )
		ShellExecute( NULL, NULL, logFilePath, NULL, NULL, SW_SHOWDEFAULT );

	return exportedFile;
}

STRING THE_WEB_FOLDER::generateWebFolder(
	PTR_FOLDER theFolder, bool forWebServer, STRING &mimeType
)
{
	doEnterFunction("THE_WEB_FOLDER::generateWebFolder");

	assert( forWebServer );

	STRING localPath = forWebServer
		? STRING( getenv( "TEMP" ) )
		: getDownloadPath()
	;

	if( localPath.isEmpty()  )
	{
/*@*/	throw Exception( "Don't know local path" );
	}

	STRING	logFilePath = localPath;

	if( !logFilePath.endsWith( DIRECTORY_DELIMITER ) )
		logFilePath += DIRECTORY_DELIMITER;
                   
	logFilePath += getName();
	logFilePath += ".log";

	logFile.open( logFilePath );
	logFile << "Exporting Web Folder " << getName() << '\n';

	clearXSLcache();
	exportedFiles.clear();

	theMenuDoc = new xml::Document( getPath() );
	theMenuDoc->addObject( new xml::Declaration );
	theMenuDoc->addObject( createXMLmenu( this ) );

	STRING	menuXMLfile = localPath;
	menuXMLfile += DIRECTORY_DELIMITER;
	menuXMLfile += "menu.xml";
	STRING	xmlMenu = theMenuDoc->generate( xml::XML_MODE );
	xmlMenu.writeToFile( menuXMLfile );

	STRING exportedFile = indexFolder( theFolder, localPath, forWebServer );
	if( exportedFile.isEmpty() )
	{
		PTR_FILE indexXml = theFolder->getContentItem( "index.xml" );
		if( !indexXml )
			indexXml = theFolder->getContentItem( "index.html" );
		if( indexXml )
		{
			exportedFile = exportFile(
				indexXml, localPath,
				forWebServer ? FLAGS_WEB_SERVER : FLAGS_SINGLE_FILE, mimeType
			);
		}
	}

	if( exportedFile.endsWithI( ".xml" ) )
		mimeType = "text/xml";
	else if( exportedFile.endsWithI( ".html" ) )
 		mimeType = "text/html";

	delete theMenuDoc;

	exportedFiles.clear();
	clearXSLcache();

	logFile.close();

	if( !forWebServer )
	{
		ShellExecute( NULL, NULL, logFilePath, NULL, NULL, SW_SHOWDEFAULT );
	}

	return exportedFile;
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

