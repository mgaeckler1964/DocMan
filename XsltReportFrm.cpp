//---------------------------------------------------------------------------

#include <vcl.h>

#include <gak/xslt.h>
#include <gak/html.h>

#include <winlib/winlib.h>

#pragma hdrstop

#include "XsltReportFrm.h"
#include "ItemManager.h"
#include "ActionManager.h"
#include "File.h"

//---------------------------------------------------------------------------
using namespace gak;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TXsltReportForm *XsltReportForm;
//---------------------------------------------------------------------------
static itemTypes theAllowedTypes[] =
{
	TYPE_FOLDER, TYPE_SYNC_FOLDER, TYPE_SOURCE_FOLDER, TYPE_RELEASE_FOLDER,
	TYPE_PERSONAL_FOLDER, TYPE_TASKLIST, TYPE_WEB_FOLDER,
	TYPE_WEB_MENU_FOLDER, TYPE_TEST_FOLDER, TYPE_TEST_EXEC_FOLDER
};
//---------------------------------------------------------------------------
static PTR_ITEM getXsltFolder( THE_ITEM	*sourceItem )
{
	int			sourceType = sourceItem->getItemType();
	PTR_ITEM	xsltFolder;

	for( size_t i=0; i<sizeof(theAllowedTypes)/sizeof(theAllowedTypes[0]); i++ )
	{
		if( sourceType == theAllowedTypes[i] )
		{
			PTR_ITEM	theXsltVolume = getPublicVolume( TYPE_XSLT_FOLDER );
			if( theXsltVolume )
			{
				STRING name = sourceItem->getItemTypeString();
				xsltFolder = (*theXsltVolume).getContentItem( name );
			}
/*@*/		break;
		}
	}

	return xsltFolder;
}

//---------------------------------------------------------------------------
class ACTION_REPORT : public ACTION_BASE_PROPERTIES
{
	public:
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
};
//---------------------------------------------------------------------------
static ACTION_REPORT theAction;
//---------------------------------------------------------------------------
bool ACTION_REPORT::acceptItem( THE_ITEM *theItem )
{
	doEnterFunction("ACTION_REPORT::acceptItem");

	bool		acceptable = false;
	int			sourceType = theItem->getItemType();

	for( size_t i=0; i<sizeof(theAllowedTypes)/sizeof(theAllowedTypes[0]); i++ )
	{
		if( sourceType == theAllowedTypes[i] )
		{
			acceptable = true;
/*@*/		break;
		}
	}

	if( acceptable )
		acceptable = ACTION_BASE::acceptItem( theItem );

	return acceptable;
}

//---------------------------------------------------------------------------
const char *ACTION_REPORT::getLabel( void ) const
{
	return "Report...";
}

//---------------------------------------------------------------------------
REFRESH_TYPE ACTION_REPORT::perform( PTR_ITEM theItem )
{
	PTR_FILE	theFile;
	PTR_ITEM	theXsltFolder = getXsltFolder( theItem );
	if( theXsltFolder )
	{
		XsltReportForm->ComboBoxStylesheet->Items->Clear();
		XsltReportForm->ComboBoxStylesheet->Items->Add("---NONE---");
		ITEM_CONTENT	*content = theXsltFolder->getContent();
		for( size_t	i=0; i<content->size(); i++ )
		{
			theFile = theXsltFolder->getContentItem( i );
			if( theFile && theFile->getMimeType() == "text/xsl" )
				XsltReportForm->ComboBoxStylesheet->Items->Add(
					static_cast<const char *>(theFile->getName())
				);
		}
		XsltReportForm->ComboBoxStylesheet->ItemIndex = 0;
		if( XsltReportForm->ShowModal() == mrOk )
		{
			bool			transformed = false;
			xml::Element	*xmlSource = theItem->createXML();
			xml::Document	theXmlDoc( theItem->getPath() );
			theXmlDoc.addObject( new xml::Declaration );
			theXmlDoc.addObject( xmlSource );
			xml::Document *theResultDoc = &theXmlDoc;

			if( XsltReportForm->ComboBoxStylesheet->ItemIndex > 0 )
			{
				theFile = theXsltFolder->getContentItem(
					XsltReportForm->ComboBoxStylesheet->Text.c_str()
				);
				std::auto_ptr<xml::Document> xmlStyleDoc(
					theFile->getXmlDocument()
				);
				if( xmlStyleDoc.get() )
				{
					xml::Element *theStylesheet = xmlStyleDoc->getElement(
						"stylesheet", xml::XMLNS_XSLT
					);
					if( theStylesheet )
					{
						xml::Transformator	theXslProcessor(
							&theXmlDoc, theStylesheet
						);
						theResultDoc = theXslProcessor.transform();
						transformed = true;
						if( !theResultDoc )
						{
							Application->MessageBox(
								"Transformation failed",
								"Error",
								MB_OK|MB_ICONERROR
							);
						}
					}
					else
						Application->MessageBox(
							"File is not an XSLT File",
							"Error",
							MB_OK|MB_ICONERROR
						);
				}
				else
					Application->MessageBox(
						"File is not an XML File",
						"Error",
						MB_OK|MB_ICONERROR
					);
			}

			if( theResultDoc )
			{
				STRING	resultFile = getenv( "TEMP" );
				if( !resultFile.endsWith( DIRECTORY_DELIMITER ) )
					resultFile += DIRECTORY_DELIMITER;
				resultFile += "report.";
				if( dynamic_cast<html::Document*>(theResultDoc) )
					resultFile += "html";
				else
					resultFile += "xml";

				STRING	result = theResultDoc->generateDoc();
				result.writeToFile( resultFile );

				ShellExecute(
					NULL, "open", resultFile, NULL, NULL, SW_SHOWNORMAL
				);

				if( transformed )
					delete theResultDoc;
			}
		}
	}

	return REFRESH_NONE;
}

//---------------------------------------------------------------------------
__fastcall TXsltReportForm::TXsltReportForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void initXsltVolume( void )
{
	doEnterFunction("initXsltVolume");

	STRING		name;
	PTR_ITEM	theXsltVolume = getPublicVolume( TYPE_XSLT_FOLDER );

	for( size_t i=0; i<sizeof(theAllowedTypes)/sizeof(theAllowedTypes[0]); i++ )
	{
		try
		{
			name = getItemTypeString( theAllowedTypes[i] );
			print2StartWindow( "XSL %s", (const char *)name );
			doLogValue( name );

			PTR_ITEM	child = theXsltVolume->getContentItem( name );
			if( !child )
			{
				child = createItem( TYPE_XSLT_FOLDER );
				child->setData( theXsltVolume, name, "" );
				child->updateDatabase();
			}
		}
		catch( ... )
		{
			// ignore any errors
		}
	}
}
//---------------------------------------------------------------------------
