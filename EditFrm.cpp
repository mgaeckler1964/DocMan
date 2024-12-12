//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "EditFrm.h"
#include "EditPlainFileFrm.h"
#include "EditXmlFileFrm.h"
#include "ActionManager.h"
#include "file.h"
#include "PermissionsFrm.h"
#include "MimeTypesFrm.h"

//---------------------------------------------------------------------------
using namespace gak;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEditFileForm *EditFileForm;
//---------------------------------------------------------------------------
class ACTION_EDIT : public ACTION_BASE_VERSIONS
{
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
};
//---------------------------------------------------------------------------
static ACTION_EDIT theAction;
//---------------------------------------------------------------------------
bool ACTION_EDIT::acceptItem( THE_ITEM *theItem )
{
	THE_FILE	*theFile = dynamic_cast<THE_FILE*>(theItem);
	if( theFile && theFile->canCreateVersion() )
	{
		CI_STRING	mimeType = theFile->getMimeType();
		if( mimeType.beginsWith( "text/" ) )
		{
			return true;
		}
	}

	return false;
}

//---------------------------------------------------------------------------
const char *ACTION_EDIT::getLabel( void ) const
{
	return "Edit...";
}
//---------------------------------------------------------------------------
REFRESH_TYPE ACTION_EDIT::perform( PTR_ITEM theItem )
{
	PTR_FILE	theFile = theItem;
	if( (THE_FILE*)theFile && theFile->canCreateVersion() )
	{
		bool	createVersion;
		STRING	sourceFile, destFile, content;
		if( theFile->getReservedOn() != DocManMainForm->getMachine() )
		{
			sourceFile = theFile->getExternalFile();

			destFile = theFile->getDownloadFile();
			makePath( destFile );

			createVersion = true;
		}
		else
		{
			sourceFile = theFile->getDownloadFile();
			destFile = sourceFile;

			createVersion = false;
		}
		content.readFromFile( sourceFile );

		TEditFileForm	*theEditor;

		CI_STRING	mimeType = theFile->getMimeType();
		if( mimeType == "text/xml"
		||  mimeType == "text/html"
		||  mimeType == "text/xsd"
		||  mimeType == "text/xsl" )
		{
//			Application->CreateForm(__classid(TEditXmlFileForm), &theEditor);
			theEditor = new TEditXmlFileForm( (TComponent*)NULL );
		}
		else
		{
//			Application->CreateForm(__classid(TEditPlainFileForm), &theEditor);
			theEditor = new TEditPlainFileForm( (TComponent*)NULL );
		}

		theEditor->setText( theFile, content );
		theEditor->setDestination( theFile, destFile, createVersion );
		DocManMainForm->ShowWindow( theEditor );
	}

	return REFRESH_NONE;
}
//---------------------------------------------------------------------------
__fastcall TEditFileForm::TEditFileForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------


void __fastcall TEditFileForm::Button1Click(TObject *)
{
	STRING content = getText();

	content.writeToFile( destFile );
	if( createVersion )
	{
		(*theFile).createVersion( destFile, "" );
		(*theFile).updateDatabase();
	}

	Close();
}
//---------------------------------------------------------------------------

void __fastcall TEditFileForm::FormClose(TObject *,
	  TCloseAction &Action)
{
	Action = caFree;
}
//---------------------------------------------------------------------------

void __fastcall TEditFileForm::Button2Click(TObject *)
{
	Close();	
}
//---------------------------------------------------------------------------

void __fastcall TEditFileForm::FormCreate(TObject *)
{
	::SetWindowLong( Handle, GWL_HWNDPARENT, NULL );
}
//---------------------------------------------------------------------------

