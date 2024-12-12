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

#undef STRICT
#define STRICT 1
//---------------------------------------------------------------------------

#include <vcl.h>

#include <gak/html>
#include <gak/xslt.h>
#include <gak/numericString.h>

#pragma hdrstop

#include "EditXmlFileFrm.h"
#include "File.h"
#include "WebFolder.h"
#include "TargetBrowseFrm.h"
#include "MimeTypesFrm.h"
#include "XmlViewerFrm.h"

//---------------------------------------------------------------------------
using namespace gak;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "EditFrm"
#pragma link "xmlEditFram"
#pragma resource "*.dfm"
TEditXmlFileForm *EditXmlFileForm;
//---------------------------------------------------------------------------
__fastcall TEditXmlFileForm::TEditXmlFileForm(TComponent* Owner)
	: TEditFileForm(Owner)
{
	XmlViewerForm = NULL;
}
//---------------------------------------------------------------------------
void TEditXmlFileForm::styleChangedCB( const STRING &styleSheetFile, const STRING &stylesheetType )
{
	ButtonTransform->Enabled = !styleSheetFile.isEmpty() && stylesheetType == "text/xsl";
}
//---------------------------------------------------------------------------
void TEditXmlFileForm::setText( const PTR_FILE &theFile, const STRING &data )
{
	STRING		fileName = theFile->getPath();
	CI_STRING	mimeType = theFile->getMimeType();
	STRING		extension = STRING('.');

	extension += MimeTypesForm->GetExtension( mimeType );
	if( extension != "." && !fileName.endsWithI( extension ) )
	{
		fileName += extension;
	}

	this->theFile = theFile;
	this->theDocument = NULL;

	xmlEditorFrame->refreshSchema();
	xmlEditorFrame->setText( data, fileName );
}
//---------------------------------------------------------------------------
void TEditXmlFileForm::setDocument( xml::Document *theDocument )
{
	this->theFile = NULL;
	this->theDocument = theDocument;

	xmlEditorFrame->refreshSchema();
	xmlEditorFrame->setDocument( theDocument, false );
}
//---------------------------------------------------------------------------
STRING TEditXmlFileForm::getText( void )
{
	return xmlEditorFrame->getText();
}
//---------------------------------------------------------------------------
void __fastcall TEditXmlFileForm::FormShow(TObject *)
{
	doEnterFunction("TEditXmlFileForm::FormShow");
	xmlEditorFrame->initShow();
}
//---------------------------------------------------------------------------

void __fastcall TEditXmlFileForm::FormCreate(TObject *Sender)
{
	doEnterFunction("TEditXmlFileForm::FormCreate");
	TEditFileForm::FormCreate(Sender);
	STRING	cssFilename = makeFullPath( Application->ExeName.c_str(), "editor.css" );
	if( exists( cssFilename ) )
	{
		std::ifstream	istream( cssFilename );
		editorRules.readCssFile( &istream, false );
	}
	xmlEditorFrame->initFrame( &theManager, &editorRules );
	xmlEditorFrame->setStyleChangedCB( styleChangedCB );
}
//---------------------------------------------------------------------------

void __fastcall TEditXmlFileForm::ButtonCheckClick(TObject *)
{
	STRING		errorText = xmlEditorFrame->testDocument();
	if( !errorText.isEmpty() )
	{
		Application->MessageBox( errorText, "Error", MB_OK|MB_ICONERROR );
	}
}
//---------------------------------------------------------------------------

void __fastcall TEditXmlFileForm::ButtonTransformClick(TObject *)
{
	STRING stylesheetType;
	STRING	stylesheetFile = xmlEditorFrame->getStylesheetFile( &stylesheetType );
	if( !stylesheetFile.isEmpty() && stylesheetType == "text/xsl" )
	{
		xml::Document *styleSheetDoc = loadXmlDoc( stylesheetFile );
		if( styleSheetDoc )
		{
			xml::Element	*stylesheet = styleSheetDoc->getElement(
				"stylesheet", xml::XMLNS_XSLT
			);
			if( stylesheet )
			{
				xml::Transformator	theXslProcessor(
					xmlEditorFrame->getDocument(), stylesheet
				);
				xml::Document	*theResultDoc = theXslProcessor.transform();
				if( theResultDoc )
				{
					html::Document	*htmlDoc = dynamic_cast<html::Document*>(
						theResultDoc
					);
					STRING	result = theResultDoc->generateDoc();

					if( !result.isEmpty() )
					{
						STRING	resultFile = getenv( "TEMP" );

						resultFile += htmlDoc ? "\\result.html" : "\\result.xml";
						result.writeToFile(resultFile);

						if( htmlDoc )
							ShellExecute(
								NULL,
								"open", resultFile,
								NULL, NULL, SW_SHOWNORMAL
							);
						else
						{
							STRING	cmd = "XmlEdit.exe ";
							cmd += " \"";
							cmd += resultFile;
							cmd += '\"';
							WinExec( cmd, SW_SHOW );
						}
					}
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TEditXmlFileForm::SelectPopupClick(TObject *Sender)
{
	TMenuItem *menuItem = dynamic_cast<TMenuItem*>(Sender);
	if( menuItem )
	{
		TTreeNode *node = xmlEditorFrame->XmlTreeView->Selected;
		if( node )
		{
			xml::Element *theElement = static_cast<xml::Element*>(node->Data);
			if( theElement )
			{
				PTR_ITEM	startItem = theFile;
				int			row = menuItem->Tag-1;

				int destinationItemID;
				STRING	ref = theElement->getAttribute( row );
				if( ref.beginsWith( '{' ) && ref.endsWith( '}' ) )
				{
					ref = ref.subString( 1, ref.strlen()-2 );
					destinationItemID = ref.getValueE<unsigned>();
					startItem = getItem( destinationItemID );
				}

				TargetBrowseForm->setItem( startItem, true );
				if( TargetBrowseForm->ShowModal() == mrOk )
				{
					TTreeNode *selected = TargetBrowseForm->TreeView->Selected;
					if( selected )
					{
						destinationItemID = (int)selected->Data;
						ref  = '{';
						ref += formatNumber( destinationItemID );
						ref += '}';

						theElement->setStringAttribute( row, ref );
						xmlEditorFrame->AttributeGrid->Cells[row+1][1] = (const char *)ref;
						xmlEditorFrame->refreshValue();
						theManager.setChanged( NULL, theElement );
					}
				}
			}
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TEditXmlFileForm::xmlEditorFrameAttributeGridContextPopup(
	  TObject *Sender, TPoint &MousePos, bool &Handled)
{
	xmlEditorFrame->AttributeGridContextPopup(Sender, MousePos, Handled);

	int	row, col;
	xmlEditorFrame->AttributeGrid->MouseToCell(
		MousePos.x, MousePos.y, col, row
	);
	TTreeNode *node = xmlEditorFrame->XmlTreeView->Selected;
	if( node && row )
	{
		xml::Element *theElement = static_cast<xml::Element*>(node->Data);
		if( theElement )
		{
			ArrayOfStrings	attributes;

			if( col == 1 )
			{
				STRING attrName = theElement->getAttributeName( row-1 );
				if( attrName == "xmlns" || attrName.beginsWith( "xmlns:" ) )
				{
					attributes.addElement( XMLNS_DOCMAN );
					attributes.addElement( XMLNS_DMCONTENT );
				}
				else
				{
					xml::Validator *theValidator = theElement->getValidator();
					if( theValidator
					&& theValidator->getAttributeType( theElement, attrName ) == "ref" )
					{
						TMenuItem *item = new TMenuItem( this );
						item->Caption = "Select...";
						item->OnClick = SelectPopupClick;
						item->Tag = row;
						xmlEditorFrame->AttributePopupMenu->Items->Add( item );
					}

				}
			}

			for(
				ArrayOfStrings::const_iterator it = attributes.cbegin(),
					endIT = attributes.cend();
				it != endIT;
				++it
			)
			{
				TMenuItem *item = new TMenuItem( this );
				item->Hint = static_cast<const char *>(*it);
				item->Caption = static_cast<const char *>(*it);
				item->OnClick = xmlEditorFrame->AttributePopupClick;
				item->Tag = col;
				xmlEditorFrame->AttributePopupMenu->Items->Add( item );
			}
		}
	}

}
//---------------------------------------------------------------------------


void __fastcall TEditXmlFileForm::ButtonRendererClick(TObject *)
{
	doEnterFunction("TEditXmlFileForm::ButtonRendererClick");
	STRING css;

	xml::Document	*theDocument = xmlEditorFrame->getDocument();
	if( theDocument )
	{
		if( !XmlViewerForm )
			XmlViewerForm = new TXmlViewerForm( this );

		XmlViewerForm->setDocument( theDocument, &theManager );
		XmlViewerForm->Show();
	}
}
//---------------------------------------------------------------------------

void __fastcall TEditXmlFileForm::FormClose( TObject *Sender, TCloseAction &Action )
{
	if( XmlViewerForm )
	{
		XmlViewerForm->Close();
		delete XmlViewerForm;
		XmlViewerForm = NULL;
	}
	TEditFileForm::FormClose( Sender, Action );
}
//---------------------------------------------------------------------------
void __fastcall TEditXmlFileForm::Button1Click(TObject *Sender)
{
	if( theFile )
		TEditFileForm::Button1Click( Sender );
}
//---------------------------------------------------------------------------

