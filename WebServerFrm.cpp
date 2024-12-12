//---------------------------------------------------------------------------

#include <vcl.h>

#include <gak/httpBaseServer.h>
#include <gak/http.h>
#include <gak/numericString.h>

#pragma hdrstop

#include "ActionManager.h"
#include "File.h"
#include "Folder.h"
#include "WebServerFrm.h"

//---------------------------------------------------------------------------
using namespace gak;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TWebServerForm *WebServerForm;
//---------------------------------------------------------------------------
class DocmanServer : public net::HTTPserverBase
{
	virtual int handleGetRequest( const STRING &url );
	public:
	DocmanServer() : net::HTTPserverBase()
	{
	}
};
//---------------------------------------------------------------------------
class ACTION_BROWSER : private ACTION_BASE_PROPERTIES
{
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
};

//---------------------------------------------------------------------------
static gak::net::SocketServer<DocmanServer> theDocmanServer;
static ACTION_BROWSER theActionBrowser;
//---------------------------------------------------------------------------
PTR_ITEM getItemByUrl( int parentID, const STRING &name, bool *idLink )
{
	doEnterFunction("getItemByUrl( int parentID - STRING name )");

	PTR_ITEM	newItem = getItemByName( parentID, name );

	if( !newItem && name[0U] == '{' )
	{
		int itemID = atoi( name + 1U );
		if( itemID )
		{
			newItem = getItem( itemID );
			if( newItem )
			{
				*idLink = true;
			}
		}
	}
	if( !newItem )
	{
		CI_STRING	extension;
		STRING		baseName;

		size_t	dotPos = name.searchChar( '.' );
		if( dotPos != name.no_index )
		{
			extension = name.subString( dotPos +1 );
			baseName = name.leftString( dotPos );
		}
		else
		{
			baseName = name;
		}

		if( !extension.isEmpty() )
		{
			newItem = getItemByName( parentID, baseName );
		}

		if( !newItem && extension != "html" )
		{
			newItem = getItemByName( parentID, baseName + ".html" );
		}
		if( !newItem && extension != "xml" )
		{
			newItem = getItemByName( parentID, baseName + ".xml" );
		}
	}

	return newItem;
}

//---------------------------------------------------------------------------
static PTR_ITEM getItemByUrl( T_STRING path, bool *idLink )
{
	doEnterFunction("getItemByUrl( T_STRING path )");

	int			parentId = -1;
	PTR_ITEM	newItem;

	path += 1U;
	STRING name = path.getFirstToken( "/" );
	while( 1 )
	{
		if( !name.isEmpty() )
		{
			newItem = getItemByUrl( parentId, name, idLink );
			if( newItem )
				parentId = (*newItem).getID();
			else
/*v*/			break;
		}

		if( !path.hasNextToken() )
/*v*/		break;

		name = path.getNextToken();
	}

	return newItem;
}

//---------------------------------------------------------------------------
static STRING getWebPath( THE_ITEM *theItem )
{
	STRING itemPath = theItem->getPath();
	itemPath.replaceChar( '\\', '/' );

	PTR_FOLDER theFolder( theItem );
	if( theFolder )
	{
		itemPath += '/';
	}

	return net::webEscape( itemPath );
}
//---------------------------------------------------------------------------
int DocmanServer::handleGetRequest( const STRING &url )
{
	doEnterFunction("DocmanServer::handleGetRequest");
	doLogValue( url );

	static Locker	serverLock;

	LockGuard	lock( serverLock, 600000 );
	STRING		externalFile, mimeType;
	TDateTime	lastModified;

	if( getClient().sin_addr.S_un.S_un_b.s_b1 != 127 )
	{
		setStatusCode( 401 );
		setError( "DocMan: Permisison denied" );
	}
	else if( !lock )
	{
		setStatusCode( 501 );
		setError( "DocMan: internal Timeout Error" );
	}
	else
	{
		try
		{
			WebServerForm->LabelLastUrl->Caption = (const char *)url;
			WebServerForm->LabelStatus->Caption = "Searching";

			STRING		myPath = net::webUnEscape( url );

			bool	idLink = false;

			PTR_ITEM	theItem = getItemByUrl( myPath, &idLink );
			PTR_FILE	theFile = theItem;
			PTR_FOLDER	theFolder = theItem;
			WebServerForm->LabelStatus->Caption = "Reading";
			if( theFile )
			{
				mimeType = theFile->getMimeType();
				if( idLink
				&& (mimeType == "text/xml"
					|| mimeType == "text/html"
					|| mimeType == "text/css")
				)
				{
					setLocation( getWebPath( theFile ) );
				}
				else
				{
					if( mimeType == "text/xml" || mimeType == "text/html" )
					{
						externalFile = theFile->generateWebFile( true, mimeType );
					}
					doLogValue( externalFile );
					if( externalFile.isEmpty() )
					{
						externalFile = theFile->getExternalFile();
					}

					doLogValue( externalFile );

					lastModified = theFile->getVersionModifiedDate();

					if( externalFile.isEmpty() || !exists( externalFile ) )
					{
						setError( "DocMan: Empty or non existing file result" );
					}
				}
			}
			else if( theFolder )
			{
				if( idLink )
				{
					setLocation( getWebPath( theFolder ) );
				}
				else
				{
					externalFile = theFolder->generateWebFolder( true, mimeType );
					lastModified = theFolder->getModifiedDate();
					doLogValue( externalFile );

					if( externalFile.isEmpty() || !exists( externalFile ) )
					{
						setError( "DocMan: Empty or non existing directory result" );
					}
				}
			}
			else
			{
				setError( "DocMan: File not found" );
				setStatusCode( 404 );
			}
		}
		catch( ... )
		{
			setError( "DocMan: Internal Exception" );
		}
	}

	if( !externalFile.isEmpty() && exists( externalFile ) )
	{
		int		c;

		setStatusCode( 200 );
		setContentType( mimeType );
		setLastModified( lastModified );
		sendResponse();

		std::ostream out( static_cast<std::streambuf*>(this) );

		out << "\r\n";

		/*
			send the file as response body
		*/
		std::fstream in( externalFile, std::ios_base::in|std::ios_base::binary );
		while( !in.eof() )
		{
			c = in.get();
			if( in.eof() )
				break;
			out.put( (char)c );
		}

		out.flush();

	}
	else
	{
		sendResponse();
	}

	WebServerForm->LabelStatus->Caption = "Waiting";
	return 0;
}
//---------------------------------------------------------------------------
bool ACTION_BROWSER::acceptItem( THE_ITEM *theItem )
{
	return (dynamic_cast<THE_FILE*>(theItem) || dynamic_cast<THE_FOLDER*>(theItem))
		? ACTION_BASE::acceptItem( theItem )
		: false;
}

const char *ACTION_BROWSER::getLabel( void ) const
{
	return "Browser...";
}

REFRESH_TYPE ACTION_BROWSER::perform( PTR_ITEM theItem )
{
	WebServerForm->setItem( theItem );
	WebServerForm->ShowModal();
	return REFRESH_NONE;
}
//---------------------------------------------------------------------------
__fastcall TWebServerForm::TWebServerForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TWebServerForm::ButtonStartClick(TObject *)
{
	unsigned short	port = 80;

	STRING openUrl = "http://localhost";
	if( EditPort->Text > "" )
	{
		openUrl += ':';
		openUrl += EditPort->Text.c_str();
		port = getValueN<unsigned short>( EditPort->Text.c_str() );
	}
	openUrl += EditItemPath->Text.c_str();

	if( !theDocmanServer.isRunning() )
	{
		theDocmanServer.startServer( port, 1 );
	}

	ShellExecute( NULL, NULL, openUrl, NULL, NULL, SW_SHOWDEFAULT );
	ButtonStart->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TWebServerForm::ButtonStopClick(TObject *)
{
	if( theDocmanServer.isRunning() )
	{
		theDocmanServer.stopServer( false );
	}
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TWebServerForm::FormShow(TObject *)
{
	STRING itemPath = getWebPath( theItem );

	EditItemPath->Text = (const char *)itemPath;
	ButtonStart->Enabled = true;
	WebServerForm->LabelStatus->Caption = "Waiting";
}
//---------------------------------------------------------------------------

