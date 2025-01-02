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
#include "DocManMain.h"
#include "DocManDM.h"

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
		if( theFile->getReservedOn() != TDocManDataModule::getMachine() )
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

