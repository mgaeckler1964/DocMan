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

#include <gak/css.h>
#include <gak/xslt.h>
#include <gak/html.h>
#include <gak/numericString.h>

#pragma hdrstop

#include "EditXmlFileFrm.h"
#include "WebWizardFrm.h"
#include "ActionManager.h"
#include "WebFolder.h"

//---------------------------------------------------------------------------
using namespace gak;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TWebWizardForm *WebWizardForm;
//---------------------------------------------------------------------------

static char *content = "content\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
	"the quick brown fox jums over the layz dog.\n"
	"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.\n"
;

#define BODY_FRAME_SETTINGS	"padding: 0;\n" \
							"border-width: 0;\nborder-style: none;\n" \
							"outline-width: 0;\noutline-style: none;\n"
#define DIV_FRAME_SETTINGS	BODY_FRAME_SETTINGS "margin: 0;\n"

static Element *form2Xml( TForm *theForm )
{
	TComponent	*theComponent;
	TEdit		*editControl;
	TComboBox	*comboControl;
	TCheckBox	*checkBox;

	xml::Any		*controlElement;
	xml::Any		*formElement = new xml::Any( theForm->Name.c_str() );


	for( int i=0; i<theForm->ComponentCount; i++ )
	{
		theComponent = theForm->Components[i];
		editControl = dynamic_cast<TEdit*>(theComponent);
		if( editControl )
		{
			controlElement = new xml::Any( editControl->Name.c_str() );
			controlElement->addObject(
				new xml::PCData( editControl->Text.c_str() )
			);
			formElement->addObject( controlElement );
			continue;
		}
		comboControl = dynamic_cast<TComboBox*>(theComponent);
		if( comboControl )
		{
			controlElement = new xml::Any( comboControl->Name.c_str() );
			controlElement->addObject(
				new xml::PCData( comboControl->Text.c_str() )
			);
			controlElement->setIntegerAttribute(
				"ItemIndex", comboControl->ItemIndex
			);
			formElement->addObject( controlElement );
			continue;
		}
		checkBox = dynamic_cast<TCheckBox*>(theComponent);
		if( checkBox )
		{
			controlElement = new xml::Any( checkBox->Name.c_str() );
			controlElement->setIntegerAttribute( "State", checkBox->State );
			formElement->addObject( controlElement );
			continue;
		}
	}

	return formElement;
}

static xml::Element *xml2Form( TForm *theForm, xml::Element *theDoc )
{
	TComponent	*theComponent;
	TEdit		*editControl;
	TComboBox	*comboControl;
	TCheckBox	*checkBox;

	STRING		strValue;
	int			intValue;

	xml::Element*controlElement;
	xml::Element*formElement = theDoc->getElement( theForm->Name.c_str() );

	if( formElement )
	{
		for( int i=0; i<theForm->ComponentCount; i++ )
		{
			theComponent = theForm->Components[i];
			editControl = dynamic_cast<TEdit*>(theComponent);
			if( editControl )
			{
				controlElement = formElement->getElement( editControl->Name.c_str() );
				if( controlElement )
					editControl->Text = (const char *)controlElement->getValue( xml::PLAIN_MODE );
				continue;
			}
			comboControl = dynamic_cast<TComboBox*>(theComponent);
			if( comboControl )
			{
				controlElement = formElement->getElement( comboControl->Name.c_str() );
				if( controlElement )
				{
					comboControl->Text = (const char *)controlElement->getValue( xml::PLAIN_MODE );
					strValue = controlElement->getAttribute( "ItemIndex" );
					if( !strValue.isEmpty() )
					{
						intValue = strValue.getValueE<int>();
						comboControl->ItemIndex = intValue;
					}
				}
				continue;
			}
			checkBox = dynamic_cast<TCheckBox*>(theComponent);
			if( checkBox )
			{
				controlElement = formElement->getElement( checkBox->Name.c_str() );
				if( controlElement )
				{
					strValue = controlElement->getAttribute( "State" );
					if( !strValue.isEmpty() )
					{
						intValue = strValue.getValueE<int>();
						checkBox->State = TCheckBoxState(intValue);
					}
				}
				continue;
			}
		}
	}

	return formElement;
}



class ACTION_WEB_WIZARD : public ACTION_BASE_VERSIONS
{
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
};
static ACTION_WEB_WIZARD webWizard;

//---------------------------------------------------------------------------
__fastcall TWebWizardForm::TWebWizardForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
bool ACTION_WEB_WIZARD::acceptItem( THE_ITEM *theItem )
{
	doEnterFunction("ACTION_WEB_WIZARD::acceptItem");

	bool			acceptable;
	THE_WEB_FOLDER	*theFolder = dynamic_cast<THE_WEB_FOLDER *>(theItem);
	if( theFolder )
		acceptable = true;
	else
	{
		THE_WEB_MENU_FOLDER	*theFolder = dynamic_cast<THE_WEB_MENU_FOLDER *>(theItem);
		if( theFolder )
			acceptable = true;
		else
			acceptable = false;
	}

	return acceptable;
}

const char *ACTION_WEB_WIZARD::getLabel( void ) const
{
	return "Web Wizard...";
}

static void createContent(
	int contentType,			// 0: empty, 1: layout, 2: item, 3: menu
	const STRING &which,
	xml::Element *layoutElement,
	xml::Element *xslBodyElement,
	xml::Element *exampleRoot
)
{
	xml::Element *xslDivElement, *xslMenuElement, *templateElement, *exampleElement;

	if( contentType == 1 )
	{
		xslDivElement = NULL;
		layoutElement = layoutElement->getElement( which,  XMLNS_DOCMAN );
		if( layoutElement )
		{
			xslDivElement = layoutElement->copy();
			if( xslDivElement )
				xslDivElement->setTag( "div" );
		}
		if( !xslDivElement )
			xslDivElement = new xml::Any( "div" );
	}
	else if( contentType == 2 )
	{
		xslDivElement = NULL;

		if( exampleRoot )
		{
			exampleElement = exampleRoot->addObject( new xml::Any( which ) );
			if( which == "content" )
				exampleElement->addObject( new xml::PCData( content ) );
			else
				exampleElement->addObject( new xml::PCData( which ) );
		}

		layoutElement = layoutElement->getElement( which,  XMLNS_DOCMAN );
		if( layoutElement && layoutElement->getNumObjects() )
		{
			xslDivElement = layoutElement->copy();
			if( xslDivElement )
				xslDivElement->setTag( "div" );
		}
		if( !xslDivElement )
		{
			xslDivElement = new xml::Any( "div" );
			templateElement = xslDivElement->addObject( new xml::Any( "xsl:copy-of" ));
			templateElement->setStringAttribute( "select", which + "/*" );
		}
	}
	else if( contentType == 3 )
	{
		xslDivElement = new xml::Any( "div" );
		xslMenuElement = xslDivElement->addObject( new xml::Any( "doc:menu" ) );
		xslMenuElement->setStringAttribute( "stylesheet", "menu.xsl" );
	}
	else // if( contentType == 0 )
	{
		xslDivElement = new xml::Any( "div" );
	}

	xslBodyElement->addObject( xslDivElement );
	xslDivElement->setStringAttribute( "class", which );
}


REFRESH_TYPE ACTION_WEB_WIZARD::perform( PTR_ITEM theItem )
{
	int				cssID = 0;

	xml::Document	*theConfig = NULL;
	xml::Element	*layoutElement = NULL;
	xml::Element	*formElement;

	xml::Document	*xmlExample = NULL;
	xml::Element	*xmlRoot = NULL;
	xml::Element	*titleElement;


	PTR_FOLDER	theFolder = (*theItem).getContentItem( "_layout" );
	if( !theFolder )
	{
		theFolder = createItem( TYPE_FOLDER );
		(*theFolder).setData( theItem, "_layout", "created by web wizard" );
		(*theFolder).updateDatabase();
	}

	WebWizardForm->dataLoaded = false;
	PTR_FILE theFile = (*theFolder).getContentItem( "layout.xml" );
	if( theFile )
	{
		theConfig = (*theFile).getXmlDocument();
		if( theConfig )
		{
			layoutElement = theConfig->getRoot();
			if( layoutElement )
			{
				formElement = xml2Form( WebWizardForm, layoutElement );
				if( formElement )
				{
					layoutElement->removeObject( formElement );
					WebWizardForm->dataLoaded = true;
				}
			}
			else
			{
				delete theConfig;
				theConfig = NULL;
			}
		}
	}
	if( !theConfig )
	{
		theConfig = new xml::Document("layout.xml");
		layoutElement = theConfig->addObject( new xml::Any( "doc:docman" ) );
		layoutElement->setNamespace( "doc", XMLNS_DOCMAN );
		layoutElement->setNamespace( "xsl", xml::XMLNS_XSLT );
		layoutElement->addObject( new xml::Any( "doc:head" ) );
		layoutElement->addObject( new xml::Any( "doc:topLeft" ) );
		layoutElement->addObject( new xml::Any( "doc:top" ) );
		layoutElement->addObject( new xml::Any( "doc:topRight" ) );
		layoutElement->addObject( new xml::Any( "doc:left" ) );
		layoutElement->addObject( new xml::Any( "doc:content" ) );
		layoutElement->addObject( new xml::Any( "doc:right" ) );
		layoutElement->addObject( new xml::Any( "doc:bottomLeft" ) );
		layoutElement->addObject( new xml::Any( "doc:bottom" ) );
		layoutElement->addObject( new xml::Any( "doc:bottomRight" ) );
	}
	WebWizardForm->configDocument = theConfig;

	STRING newCaption = STRING( "Web Wizard " ) + (*theItem).getName();
	WebWizardForm->Caption = (const char *)newCaption;

	if( WebWizardForm->ShowModal() == mrOk )
	{
		STRING	unit = "px";
		bool	noExample = WebWizardForm->CheckBoxNoExample->Checked;

		int contentWidth = atoi( WebWizardForm->EditContent->Text.c_str() );
		int leftWidth = WebWizardForm->EditLeft->Enabled ? atoi( WebWizardForm->EditLeft->Text.c_str() ) : 0;
		int rightWidth = WebWizardForm->EditRight->Enabled ? atoi( WebWizardForm->EditRight->Text.c_str() ) : 0;

		int topHeight = WebWizardForm->EditTop->Enabled ? atoi( WebWizardForm->EditTop->Text.c_str() ) : 0;
		int bottomHeight = WebWizardForm->EditBottom->Enabled ? atoi( WebWizardForm->EditBottom->Text.c_str() ) : 0;

		STRING	cssPath = getenv( "TEMP" );
		if( cssPath.isEmpty() )
			cssPath = "C:";
		if( !cssPath.endsWith( DIRECTORY_DELIMITER ) )
			cssPath += DIRECTORY_DELIMITER;
		STRING	xmlPath = cssPath + "example.xml";
		STRING	confPath = cssPath + "layout.xml";
		STRING	xslPath = cssPath + "layout.xsl";
		cssPath += "layout.css";

		xml::Element *xslRoot = new xml::Any( "xsl:stylesheet" );
		xslRoot->setNamespace( "xsl", xml::XMLNS_XSLT );
		xslRoot->setNamespace( "doc", XMLNS_DOCMAN );
		xslRoot->setStringAttribute( "version", "1.0" );
		xml::Element *outputElement = xslRoot->addObject(
			new xml::Any( STRING("xsl:") +  xml::XSL_OUTPUT )
		);
		outputElement->setStringAttribute( xml::XSL_METHOD, xml::XSL_HTML );
		outputElement->setStringAttribute( "encoding", "iso-8859-1" );
		outputElement->setStringAttribute( xml::XSL_SYSTEM, html::HTML_STRICT_SYSTEM );
		outputElement->setStringAttribute( xml::XSL_PUBLIC, html::HTML_STRICT_PUBLIC );

		xml::Element *templateElement = xslRoot->addObject( new xml::Any("xsl:template") );
		templateElement->setStringAttribute( "match", "/layout" );
		xml::Element *xslHtmlElement = templateElement->addObject( new xml::Any( "html" ) );
		xslHtmlElement->setNamespace( "doc", XMLNS_DOCMAN );
		xml::Element *xslHeadElement = xslHtmlElement->addObject( new xml::Any( "head" ) );
		xml::Element *xslBodyElement = xslHtmlElement->addObject( new xml::Any( "body" ) );

		xml::Element *xslTitleElement = xslHeadElement->addObject( new xml::Any( "title" ) );
		templateElement = xslTitleElement->addObject( new xml::Any( "xsl:value-of" ) );
		templateElement->setStringAttribute( "select", "title" );

		if( !noExample )
		{
			xmlExample = new xml::Document( xmlPath );
			xmlExample->addObject( new xml::StyleSheet( "_layout/layout.xsl" ) );
			xmlRoot = xmlExample->addObject( new xml::Any( "layout" ) );
			xmlRoot->setStringAttribute( xml::XMLNS_ATTRIBUTE, XMLNS_DMCONTENT );
			titleElement = xmlRoot->addObject( new xml::Any( "title" ) );
			titleElement->addObject( new xml::PCData( "Example File" ) );
		}

		std::ofstream 	cssFile( cssPath );
		if( cssFile.is_open() )
		{
			cssFile <<
				"/*\n"
					"\tlayout.css\n"
					"\t==========\n"
					"\tcreated with DocMan\n"
					"\t(c)2013-2024 by Martin Gäckler\n"
					"\tDO NOT EDIT\n"
				"*/\n";

			/*
				=============================================================
				body
				=============================================================
			*/
			cssFile <<
				"body {\n"
				BODY_FRAME_SETTINGS
				"background-color:silver;\n"
				"overflow:hidden;\n";
			if( contentWidth )
			{
				cssFile << "width: " << (leftWidth+contentWidth+rightWidth)<<unit<<";\n";
				if( WebWizardForm->ComboBoxContent->ItemIndex == 1 )
					cssFile << "margin: auto;\n";
				else
					cssFile << "margin: 0;\n";
			}
			else
				cssFile << "margin: 0;\n";
			cssFile << "}\n";

			/*
				=============================================================
				TOP LEFT CORNER
				=============================================================
			*/
			if( WebWizardForm->ComboBoxTopLeft->Enabled
			&& WebWizardForm->ComboBoxTopLeft->ItemIndex == 2 )
			{
				cssFile <<
					"div.topLeft {\n"
					"width: " << leftWidth << unit  << ";\n"
					"height:" << topHeight << unit  << ";\n"
					DIV_FRAME_SETTINGS
					"position: fixed;\n"
					"top: 0;\n";

				if( !contentWidth || WebWizardForm->ComboBoxContent->ItemIndex!=2 )
					cssFile << "left: 0;\n";
				else
				{
					cssFile <<
						"right: " << (contentWidth + rightWidth) << unit << ";\n"
					;
				}

				cssFile << "background-color: " << WebWizardForm->ComboBoxColorTopLeft->Text.c_str() << ";\n";
				cssFile << "}\n";

				createContent(
					WebWizardForm->ComboBoxContentTopLeft->ItemIndex,
					"topLeft",
					layoutElement,
					xslBodyElement,
					xmlRoot
				);
			}

			/*
				=============================================================
				TOP BORDER
				=============================================================
			*/
			if( WebWizardForm->ComboBoxTop->ItemIndex == 0 )
			{
				cssFile <<
					"div.top {\n"
					"height:" << topHeight << unit  << ";\n"
					DIV_FRAME_SETTINGS
					"position: fixed;\n"
					"top: 0;\n";

				if( !contentWidth || WebWizardForm->ComboBoxContent->ItemIndex == 0 )
				{
					if( !WebWizardForm->ComboBoxTopLeft->Enabled
					|| WebWizardForm->ComboBoxTopLeft->ItemIndex == 0 )
						cssFile << "left: 0;\n";
					else
						cssFile << "left: " << leftWidth << unit  << ";\n";
				}

				if( contentWidth )
				{
					int topWidth = contentWidth;

					if( !WebWizardForm->ComboBoxTopLeft->Enabled
					|| WebWizardForm->ComboBoxTopLeft->ItemIndex == 0 )
						topWidth += leftWidth;

					if( !WebWizardForm->ComboBoxTopRight->Enabled
					|| WebWizardForm->ComboBoxTopRight->ItemIndex == 0 )
						topWidth += rightWidth;

					cssFile << "width: " << topWidth << unit << ";\n";
				}

				if( !contentWidth || WebWizardForm->ComboBoxContent->ItemIndex == 2 )
				{
					if( !WebWizardForm->ComboBoxTopRight->Enabled
					|| WebWizardForm->ComboBoxTopRight->ItemIndex == 0 )
						cssFile << "right: 0" << unit << ";\n";
					else
						cssFile << "right: " << rightWidth << unit  << ";\n";
				}


				cssFile << "background-color: " << WebWizardForm->ComboBoxColorTop->Text.c_str() << ";\n";
				cssFile << "}\n";

				createContent(
					WebWizardForm->ComboBoxContentTop->ItemIndex,
					"top",
					layoutElement,
					xslBodyElement,
					xmlRoot
				);
			}

			/*
				=============================================================
				TOP RIGHT CORNER
				=============================================================
			*/
			if( WebWizardForm->ComboBoxTopRight->Enabled
			&& WebWizardForm->ComboBoxTopRight->ItemIndex == 2 )
			{
				cssFile <<
					"div.topRight {\n"
					"width: " << rightWidth << unit  << ";\n"
					"height:" << WebWizardForm->EditTop->Text.c_str() << unit  << ";\n"
					DIV_FRAME_SETTINGS
					"position: fixed;\n"
					"top: 0;\n";
				if( contentWidth && WebWizardForm->ComboBoxContent->ItemIndex==0 )
					cssFile << "left: " << (contentWidth + leftWidth) << unit << ";\n";
				else
					cssFile << "right: 0;\n";

				cssFile << "background-color: " << WebWizardForm->ComboBoxColorTopRight->Text.c_str() << ";\n";
				cssFile << "}\n";

				createContent(
					WebWizardForm->ComboBoxContentTopRight->ItemIndex,
					"topRight",
					layoutElement,
					xslBodyElement,
					xmlRoot
				);
			}

			/*
				=============================================================
				LEFT BORDER
				=============================================================
			*/
			if( WebWizardForm->ComboBoxLeft->ItemIndex == 0 )
			{
				cssFile <<
					"div.left {\n"
					"width:" << WebWizardForm->EditLeft->Text.c_str() << unit  << ";\n"
					DIV_FRAME_SETTINGS
					"position: fixed;\n";

				if( !contentWidth || WebWizardForm->ComboBoxContent->ItemIndex != 2 )
					cssFile << "left: 0;\n";
				else
					cssFile << "right: " << (contentWidth + rightWidth) << unit << ";\n";

				if( !WebWizardForm->ComboBoxTopLeft->Enabled
				|| WebWizardForm->ComboBoxTopLeft->ItemIndex == 1 )
					cssFile << "top: 0;\n";
				else
					cssFile << "top: " << topHeight << unit  << ";\n";

				if( !WebWizardForm->ComboBoxBottomLeft->Enabled
				|| WebWizardForm->ComboBoxBottomLeft->ItemIndex == 0 )
					cssFile << "bottom: 0;\n";
				else
					cssFile << "bottom: " << bottomHeight << unit  << ";\n";

				cssFile << "background-color: " << WebWizardForm->ComboBoxColorLeft->Text.c_str() << ";\n";
				cssFile << "}\n";

				createContent(
					WebWizardForm->ComboBoxContentLeft->ItemIndex,
					"left",
					layoutElement,
					xslBodyElement,
					xmlRoot
				);
			}
			/*
				=============================================================
				CONTENT
				=============================================================
			*/
			cssFile <<
				"div.content {\n"
				DIV_FRAME_SETTINGS
				"overflow:auto;\n"
				"position: fixed;\n";
			if( WebWizardForm->ComboBoxTop->ItemIndex == 0 )
				cssFile <<
					"top:" << topHeight << unit  << ";\n";
			else
				cssFile << "top: 0;\n";

			if( !contentWidth || WebWizardForm->ComboBoxContent->ItemIndex==0 )
			{
				if( WebWizardForm->ComboBoxLeft->ItemIndex == 0 )
					cssFile <<
						"left:" << leftWidth << unit  << ";\n";
				else
					cssFile << "left: 0;\n";
			}

			if( contentWidth )
				cssFile << "width: " << contentWidth << unit << ";\n";

			if( !contentWidth || WebWizardForm->ComboBoxContent->ItemIndex==2 )
			{
				if( WebWizardForm->ComboBoxRight->ItemIndex == 0 )
					cssFile <<
						"right:" << rightWidth << unit  << ";\n";
				else
					cssFile << "right: 0;\n";
			}
			if( WebWizardForm->ComboBoxBottom->ItemIndex == 0 )
				cssFile <<
					"bottom:" << bottomHeight << unit  << ";\n";
			else
				cssFile << "bottom: 0;\n";
			cssFile << "background-color: " << WebWizardForm->ComboBoxColorContent->Text.c_str() << ";\n";
			cssFile << "}\n";

			createContent(
				2,
				"content",
				layoutElement,
				xslBodyElement,
				xmlRoot
			);

			/*
				=============================================================
				RIGHT BORDER
				=============================================================
			*/
			if( WebWizardForm->ComboBoxRight->ItemIndex == 0 )
			{
				cssFile <<
					"div.right {\n"
					"width:" << WebWizardForm->EditRight->Text.c_str() << unit << ";\n"
					DIV_FRAME_SETTINGS
					"position: fixed;\n";
				if( contentWidth && WebWizardForm->ComboBoxContent->ItemIndex==0 )
					cssFile << "left: " << (contentWidth+leftWidth) << unit << ";\n";
				else
					cssFile << "right: 0;\n";

				if( !WebWizardForm->ComboBoxTopRight->Enabled
				|| WebWizardForm->ComboBoxTopRight->ItemIndex == 1 )
					cssFile << "top: 0;\n";
				else
					cssFile << "top: " << topHeight << unit  << ";\n";

				if( !WebWizardForm->ComboBoxBottomRight->Enabled
				|| WebWizardForm->ComboBoxBottomRight->ItemIndex == 0 )
					cssFile << "bottom: 0;\n";
				else
					cssFile << "bottom: " << bottomHeight << unit  << ";\n";

				cssFile << "background-color: " << WebWizardForm->ComboBoxColorRight->Text.c_str() << ";\n";
				cssFile << "}\n";

				createContent(
					WebWizardForm->ComboBoxContentRight->ItemIndex,
					"right",
					layoutElement,
					xslBodyElement,
					xmlRoot
				);
			}

			/*
				=============================================================
				BOTTOM LEFT CORNER
				=============================================================
			*/
			if( WebWizardForm->ComboBoxBottomLeft->Enabled
			&& WebWizardForm->ComboBoxBottomLeft->ItemIndex == 2 )
			{
				cssFile <<
					"div.bottomLeft {\n"
					"width: " << leftWidth << unit << ";\n"
					"height:" << bottomHeight << unit  << ";\n"
					DIV_FRAME_SETTINGS
					"position: fixed;\n"
					"bottom: 0;\n";

				if( !contentWidth || WebWizardForm->ComboBoxContent->ItemIndex!=2 )
					cssFile << "left: 0;\n";
				else
				{
					cssFile <<
						"right: " << (contentWidth + rightWidth) << unit << ";\n"
					;
				}

				cssFile << "background-color: " << WebWizardForm->ComboBoxColorBottomLeft->Text.c_str() << ";\n";
				cssFile << "}\n";

				createContent(
					WebWizardForm->ComboBoxContentBottomLeft->ItemIndex,
					"bottomLeft",
					layoutElement,
					xslBodyElement,
					xmlRoot
				);
			}

			/*
				=============================================================
				BOTTOM BORDER
				=============================================================
			*/
			if( WebWizardForm->ComboBoxBottom->ItemIndex == 0 )
			{
				cssFile <<
					"div.bottom {\n"
					"height:" << bottomHeight << unit  << ";\n"
					DIV_FRAME_SETTINGS
					"position: fixed;\n"
					"bottom: 0;\n";


				if( !contentWidth || WebWizardForm->ComboBoxContent->ItemIndex == 0 )
				{
					if( !WebWizardForm->ComboBoxBottomLeft->Enabled
					|| WebWizardForm->ComboBoxBottomLeft->ItemIndex == 1 )
						cssFile << "left: 0;\n";
					else
						cssFile << "left: " << leftWidth << unit  << ";\n";
				}

				if( contentWidth  )
				{
					int bottomWidth = contentWidth;
					if( !WebWizardForm->ComboBoxBottomLeft->Enabled
					|| WebWizardForm->ComboBoxBottomLeft->ItemIndex == 1 )
						bottomWidth += leftWidth;

					if( !WebWizardForm->ComboBoxBottomRight->Enabled
					|| WebWizardForm->ComboBoxBottomRight->ItemIndex == 1 )
						bottomWidth += rightWidth;

					cssFile << "width: " << bottomWidth << unit << ";\n";
				}

				if( !contentWidth || WebWizardForm->ComboBoxContent->ItemIndex == 2 )
				{
					if( !WebWizardForm->ComboBoxBottomRight->Enabled
					|| WebWizardForm->ComboBoxBottomRight->ItemIndex == 1 )
						cssFile << "right: 0;\n";
					else
						cssFile << "right: " << rightWidth << unit  << ";\n";
				}

				cssFile << "background-color: " << WebWizardForm->ComboBoxColorBottom->Text.c_str() << ";\n";
				cssFile << "}\n";

				createContent(
					WebWizardForm->ComboBoxContentBottom->ItemIndex,
					"bottom",
					layoutElement,
					xslBodyElement,
					xmlRoot
				);
			}
			/*
				=============================================================
				BOTTOM RIGHT CORNER
				=============================================================
			*/
			if( WebWizardForm->ComboBoxBottomRight->Enabled
			&& WebWizardForm->ComboBoxBottomRight->ItemIndex == 2 )
			{
				cssFile <<
					"div.bottomRight {\n"
					"width: " << rightWidth << unit  << ";\n"
					"height:" << bottomHeight << unit  << ";\n"
					DIV_FRAME_SETTINGS
					"position: fixed;\n"
					"bottom: 0;\n";
				if( contentWidth  && WebWizardForm->ComboBoxContent->ItemIndex==0 )
					cssFile << "left: " << (contentWidth+leftWidth) << unit << ";\n";
				else
					cssFile << "right: 0;\n";

				cssFile << "background-color: " << WebWizardForm->ComboBoxColorBottomRight->Text.c_str() << ";\n";
				cssFile << "}\n";

				createContent(
					WebWizardForm->ComboBoxContentBottomRight->ItemIndex,
					"bottomRight",
					layoutElement,
					xslBodyElement,
					xmlRoot
				);
			}

			cssFile.close();

			theFile = theFolder->getContentItem( "layout.css" );
			if( !theFile )
			{
				theFile = createItem( TYPE_DOCUMENT );
				theFile->setData( theFolder, "layout.css", "created by web wizard", cssPath );
				theFile->updateDatabase();
			}
			else
			{
				theFile->createVersion( cssPath, "created by web wizard" );
				theFile->updateDatabase();
			}

			cssID = (*theFile).getID();
		}

		formElement = form2Xml( WebWizardForm );
		layoutElement->addObject( formElement );
		STRING configXml = theConfig->generateDoc();
		configXml.writeToFile( confPath );

		theFile = theFolder->getContentItem( "layout.xml" );
		if( !theFile )
		{
			theFile = createItem( TYPE_DOCUMENT );
			theFile->setData( theFolder, "layout.xml", "created by web wizard", confPath );
			theFile->updateDatabase();
		}
		else
		{
			theFile->createVersion( confPath, "created by web wizard" );
			theFile->updateDatabase();
		}

		if( cssID )
		{
			xml::Element *docLinkElement = xslHeadElement->addObject(
				new xml::Any( "doc:link" )
			);
			docLinkElement->setStringAttribute( "rel", "stylesheet" );
			docLinkElement->setStringAttribute( "type", "text/css" );
			STRING	cssIDstr = STRING('{');
			cssIDstr += formatNumber( cssID );
			cssIDstr += '}';
			docLinkElement->setStringAttribute( "href", cssIDstr );
		}

		xml::Element *layoutHeadElement = layoutElement->getElement( "head", XMLNS_DOCMAN );
		if( layoutHeadElement )
		{
			for( size_t i=0; i<layoutHeadElement->getNumObjects(); i++ )
			{
				xml::Element *theElement = layoutHeadElement->getElement( i );
				xslHeadElement->addObject( theElement->copy() );
			}
		}

		STRING xslStr = xslRoot->generateDoc();
		xslStr.writeToFile( xslPath );

		theFile = theFolder->getContentItem( "layout.xsl" );
		if( !theFile )
		{
			theFile = createItem( TYPE_DOCUMENT );
			theFile->setData( theFolder, "layout.xsl", "created by web wizard", xslPath );
			theFile->updateDatabase();
		}
		else
		{
			theFile->createVersion( xslPath, "created by web wizard" );
			theFile->updateDatabase();
		}

		if( !noExample )
		{
			STRING xmlStr = xmlExample->generateDoc();
			xmlStr.writeToFile( xmlPath );

			theFile = theItem->getContentItem( "example.xml" );
			if( !theFile )
			{
				theFile = createItem( TYPE_DOCUMENT );
				theFile->setData( theItem, "example.xml", "created by web wizard", xmlPath );
				theFile->updateDatabase();
			}
			else
			{
				theFile->createVersion( xmlPath, "created by web wizard" );
				theFile->updateDatabase();
			}

			delete xmlExample;
		}

		delete xslRoot;

		return REFRESH_RELOAD;
	}

	delete theConfig;

	return REFRESH_NONE;
}
//---------------------------------------------------------------------------
void __fastcall TWebWizardForm::EnableDisableLayout(TObject *)
{
	if( ComboBoxTop->ItemIndex == 1
	|| ComboBoxLeft->ItemIndex == 1 )
		ComboBoxTopLeft->Enabled = false;
	else
		ComboBoxTopLeft->Enabled = true;

	if( ComboBoxTopLeft->Enabled && ComboBoxTopLeft->ItemIndex == 2 )
	{
		ComboBoxColorTopLeft->Enabled = true;
		ComboBoxContentTopLeft->Enabled = true;
	}
	else
	{
		ComboBoxColorTopLeft->Enabled = false;
		ComboBoxContentTopLeft->Enabled = false;
	}

	if( ComboBoxTop->ItemIndex == 1
	|| ComboBoxRight->ItemIndex == 1 )
		ComboBoxTopRight->Enabled = false;
	else
		ComboBoxTopRight->Enabled = true;

	if( ComboBoxTopRight->Enabled && ComboBoxTopRight->ItemIndex == 2 )
	{
		ComboBoxColorTopRight->Enabled = true;
		ComboBoxContentTopRight->Enabled = true;
	}
	else
	{
		ComboBoxColorTopRight->Enabled = false;
		ComboBoxContentTopRight->Enabled = false;
	}

	if( ComboBoxBottom->ItemIndex == 1
	|| ComboBoxRight->ItemIndex == 1 )
		ComboBoxBottomRight->Enabled = false;
	else
		ComboBoxBottomRight->Enabled = true;

	if( ComboBoxBottomRight->Enabled && ComboBoxBottomRight->ItemIndex == 2 )
	{
		ComboBoxColorBottomRight->Enabled = true;
		ComboBoxContentBottomRight->Enabled = true;
	}
	else
	{
		ComboBoxColorBottomRight->Enabled = false;
		ComboBoxContentBottomRight->Enabled = false;
	}

	if( ComboBoxBottom->ItemIndex == 1
	|| ComboBoxLeft->ItemIndex == 1 )
		ComboBoxBottomLeft->Enabled = false;
	else
		ComboBoxBottomLeft->Enabled = true;

	if( ComboBoxBottomLeft->Enabled && ComboBoxBottomLeft->ItemIndex == 2 )
	{
		ComboBoxColorBottomLeft->Enabled = true;
		ComboBoxContentBottomLeft->Enabled = true;
	}
	else
	{
		ComboBoxColorBottomLeft->Enabled = false;
		ComboBoxContentBottomLeft->Enabled = false;
	}

	if( ComboBoxTop->ItemIndex == 1 )
	{
		EditTop->Enabled = false;
		ComboBoxColorTop->Enabled = false;
		ComboBoxContentTop->Enabled = false;
	}
	else
	{
		EditTop->Enabled = true;
		ComboBoxColorTop->Enabled = true;
		ComboBoxContentTop->Enabled = true;
	}

	if( ComboBoxRight->ItemIndex == 1 )
	{
		EditRight->Enabled = false;
		ComboBoxColorRight->Enabled = false;
		ComboBoxContentRight->Enabled = false;
	}
	else
	{
		EditRight->Enabled = true;
		ComboBoxColorRight->Enabled = true;
		ComboBoxContentRight->Enabled = true;
	}

	if( ComboBoxBottom->ItemIndex == 1 )
	{
		EditBottom->Enabled = false;
		ComboBoxColorBottom->Enabled = false;
		ComboBoxContentBottom->Enabled = false;
	}
	else
	{
		EditBottom->Enabled = true;
		ComboBoxColorBottom->Enabled = true;
		ComboBoxContentBottom->Enabled = true;
	}

	if( ComboBoxLeft->ItemIndex == 1 )
	{
		EditLeft->Enabled = false;
		ComboBoxColorLeft->Enabled = false;
		ComboBoxContentLeft->Enabled = false;
	}
	else
	{
		EditLeft->Enabled = true;
		ComboBoxColorLeft->Enabled = true;
		ComboBoxContentLeft->Enabled = true;
	}

	PaintBoxPreview->Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TWebWizardForm::FormShow(TObject *Sender)
{
	if( !dataLoaded )
	{
		ComboBoxContent->ItemIndex = 0;

		ComboBoxTopLeft->ItemIndex = 0;
		ComboBoxTopRight->ItemIndex = 0;
		ComboBoxBottomLeft->ItemIndex = 1;
		ComboBoxBottomRight->ItemIndex = 1;

		ComboBoxTop->ItemIndex = 1;
		ComboBoxLeft->ItemIndex = 1;
		ComboBoxRight->ItemIndex = 1;
		ComboBoxBottom->ItemIndex = 1;

		ComboBoxContentTopLeft->ItemIndex = 0;
		ComboBoxContentTop->ItemIndex = 0;
		ComboBoxContentLeft->ItemIndex = 0;
		ComboBoxContentBottomLeft->ItemIndex = 0;
		ComboBoxContentBottom->ItemIndex = 0;
		ComboBoxContentBottomRight->ItemIndex = 0;
		ComboBoxContentTopRight->ItemIndex = 0;
		ComboBoxContentRight->ItemIndex = 0;
	}

	EnableDisableLayout( Sender );
}
//---------------------------------------------------------------------------

void __fastcall TWebWizardForm::PaintBoxPreviewPaint(TObject *)
{
	css::Color	cssColor;

	TRect	left, topLeft, top, topRight, right, bottomRight, bottom, bottomLeft;
	TRect	content;

	int topHeight = atoi( EditTop->Text.c_str() );
	int bottomHeight = atoi( EditBottom->Text.c_str() );
	int leftWidth = atoi( EditLeft->Text.c_str() );
	int rightWidth = atoi( EditRight->Text.c_str() );
	int	contentWidth = atoi( EditContent->Text.c_str() );
	int	contentDelta, contentAutoWidth;

	int windowWidth = PaintBoxPreview->Width;
	int windowHeight = PaintBoxPreview->Height;

	TCanvas	*myCanvas = PaintBoxPreview->Canvas;
	if( ComboBoxLeft->ItemIndex == 0 )
	{
		// there is a left frame
		left.left = left.top = 0;
		left.right = leftWidth;
		left.bottom = windowHeight;
	}
	else
		left.left = left.top = left.right = left.bottom = 0;

	if( ComboBoxTop->ItemIndex == 0 )
	{
		// there is a left frame
		top.left = top.top = 0;
		top.right = windowWidth;
		top.bottom = topHeight;
	}
	else
		top.left = top.top = top.right = top.bottom = 0;

	if( ComboBoxRight->ItemIndex == 0 )
	{
		// there is a left frame
		right.top = 0;
		right.right = windowWidth;
		right.left = right.right-rightWidth;
		right.bottom = windowHeight;
	}
	else
		right.left = right.top = right.right = right.bottom = windowWidth;

	if( ComboBoxBottom->ItemIndex == 0 )
	{
		// there is a left frame
		bottom.right = windowWidth;
		bottom.left = 0;
		bottom.bottom = windowHeight;
		bottom.top = bottom.bottom-bottomHeight;
	}
	else
		bottom.left = bottom.top = bottom.right = bottom.bottom = windowHeight;

	if( ComboBoxTopLeft->Enabled )
	{
		if( ComboBoxTopLeft->ItemIndex == 0 )
		{
			topLeft.left = topLeft.top = topLeft.right = topLeft.bottom = 0;
			left.top = top.bottom;
		}
		else if( ComboBoxTopLeft->ItemIndex == 1 )
		{
			topLeft.left = topLeft.top = topLeft.right = topLeft.bottom = 0;
			top.left = left.right;
		}
		else
		{
			left.top = top.bottom;
			top.left = left.right;
			topLeft.left = topLeft.top = 0;
			topLeft.right = left.right;
			topLeft.bottom = top.bottom;
		}
	}
	else
		topLeft.left = topLeft.top = topLeft.right = topLeft.bottom = 0;

	if( ComboBoxTopRight->Enabled )
	{
		if( ComboBoxTopRight->ItemIndex == 0 )
		{
			topRight.left = topRight.top = topRight.right = topRight.bottom = 0;
			right.top = top.bottom;
		}
		else if( ComboBoxTopRight->ItemIndex == 1 )
		{
			topRight.left = topRight.top = topRight.right = topRight.bottom = 0;
			top.right = right.left;
		}
		else
		{
			right.top = top.bottom;
			top.right = right.left;

			topRight.top = 0;
			topRight.left = right.left;
			topRight.right = right.right;
			topRight.bottom = top.bottom;
		}
	}
	else
		topRight.left = topRight.top = topRight.right = topRight.bottom = 0;

	if( ComboBoxBottomRight->Enabled )
	{
		if( ComboBoxBottomRight->ItemIndex == 1 )
		{
			bottomRight.left = bottomRight.top = bottomRight.right = bottomRight.bottom = 0;
			right.bottom = bottom.top;
		}
		else if( ComboBoxBottomRight->ItemIndex == 0 )
		{
			bottomRight.left = bottomRight.top = bottomRight.right = bottomRight.bottom = 0;
			bottom.right = right.left;
		}
		else
		{
			right.bottom = bottom.top;
			bottom.right = right.left;

			bottomRight.top = bottom.top;
			bottomRight.bottom = bottom.bottom;

			bottomRight.right = right.right;
			bottomRight.left = right.left;
		}
	}
	else
		bottomRight.left = bottomRight.top = bottomRight.right = bottomRight.bottom = 0;

	if( ComboBoxBottomLeft->Enabled )
	{

		if( ComboBoxBottomLeft->ItemIndex == 1 )
		{
			bottomLeft.left = bottomLeft.top = bottomLeft.right = bottomLeft.bottom = 0;
			left.bottom = bottom.top;
		}
		else if( ComboBoxBottomLeft->ItemIndex == 0 )
		{
			bottomLeft.left = bottomLeft.top = bottomLeft.right = bottomLeft.bottom = 0;
			bottom.left = left.right;
		}
		else
		{
			left.bottom = bottom.top;
			bottom.left = left.right;
			bottomLeft.left = 0;
			bottomLeft.right = left.right;
			bottomLeft.top = bottom.top;
			bottomLeft.bottom = bottom.bottom;
		}
	}
	else
		bottomLeft.left = bottomLeft.top = bottomLeft.right = bottomLeft.bottom = 0;

	content.left = left.right;
	content.top = top.bottom;
	content.right = right.left;
	content.bottom = bottom.top;

	if( contentWidth > 0 )
	{
		contentAutoWidth = content.Width();
		contentDelta = contentAutoWidth-contentWidth;

		if( ComboBoxContent->ItemIndex == 0 )
		{
			content.right -= contentDelta;

			top.right -= contentDelta;

			topRight.left -= contentDelta;
			topRight.right -= contentDelta;

			right.left -= contentDelta;
			right.right -= contentDelta;

			bottomRight.left -= contentDelta;
			bottomRight.right -= contentDelta;

			bottom.right -= contentDelta;
		}
		else if( ComboBoxContent->ItemIndex == 1 )
		{
			contentDelta /= 2;

			content.right -= contentDelta;

			top.right -= contentDelta;

			topRight.left -= contentDelta;
			topRight.right -= contentDelta;

			right.left -= contentDelta;
			right.right -= contentDelta;

			bottomRight.left -= contentDelta;
			bottomRight.right -= contentDelta;

			bottom.right -= contentDelta;

			content.left += contentDelta;

			top.left += contentDelta;

			topLeft.left += contentDelta;
			topLeft.right += contentDelta;

			left.left += contentDelta;
			left.right += contentDelta;

			bottomLeft.left += contentDelta;
			bottomLeft.right += contentDelta;

			bottom.left += contentDelta;
		}
		else
		{
			content.left += contentDelta;

			top.left += contentDelta;

			topLeft.left += contentDelta;
			topLeft.right += contentDelta;

			left.left += contentDelta;
			left.right += contentDelta;

			bottomLeft.left += contentDelta;
			bottomLeft.right += contentDelta;

			bottom.left += contentDelta;
		}
	}

	myCanvas->Pen->Style = psClear;
	myCanvas->Brush->Color = clSilver;
	myCanvas->Rectangle( 0, 0, windowWidth, windowHeight );

	myCanvas->Pen->Width = 1;
	myCanvas->Pen->Style = psSolid;
	myCanvas->Pen->Color = clBlack;
	myCanvas->Brush->Style = bsSolid;

	if( content.Width() && content.Height() )
	{
		if( css::Styles::parseColor(
			css::Value(ComboBoxColorContent->Text.LowerCase().c_str()),
			&cssColor
		) )
		{
			myCanvas->Brush->Color = (TColor)((cssColor.blue << 16)
				| (cssColor.green << 8)
				| cssColor.red)
			;
		}
		else
			myCanvas->Brush->Color = clWhite;
		myCanvas->Rectangle( content );
	}

	if( left.Width() && left.Height() )
	{
		if( css::Styles::parseColor(
			css::Value(ComboBoxColorLeft->Text.LowerCase().c_str()),
			&cssColor
		) )
		{
			myCanvas->Brush->Color = (TColor)((cssColor.blue << 16)
				| (cssColor.green << 8)
				| cssColor.red)
			;
		}
		else
			myCanvas->Brush->Color = clAqua;
		myCanvas->Rectangle( left );
	}
	if( topLeft.Width() && topLeft.Height() )
	{
		if( css::Styles::parseColor(
			css::Value(ComboBoxColorTopLeft->Text.LowerCase().c_str()),
			&cssColor
		) )
		{
			myCanvas->Brush->Color = (TColor)((cssColor.blue << 16)
				| (cssColor.green << 8)
				| cssColor.red)
			;
		}
		else
			myCanvas->Brush->Color = clBlue;
		myCanvas->Rectangle( topLeft );
	}
	if( top.Width() && top.Height() )
	{
		if( css::Styles::parseColor(
			css::Value(ComboBoxColorTop->Text.LowerCase().c_str()),
			&cssColor
		) )
		{
			myCanvas->Brush->Color = (TColor)((cssColor.blue << 16)
				| (cssColor.green << 8)
				| cssColor.red)
			;
		}
		else
			myCanvas->Brush->Color = clGreen;
		myCanvas->Rectangle( top );
	}
	if( topRight.Width() && topRight.Height() )
	{
		if( css::Styles::parseColor(
			css::Value(ComboBoxColorTopRight->Text.LowerCase().c_str()),
			&cssColor
		) )
		{
			myCanvas->Brush->Color = (TColor)((cssColor.blue << 16)
				| (cssColor.green << 8)
				| cssColor.red)
			;
		}
		else
			myCanvas->Brush->Color = clMaroon;
		myCanvas->Rectangle( topRight );
	}
	if( right.Width() && right.Height() )
	{
		if( css::Styles::parseColor(
			css::Value(ComboBoxColorRight->Text.LowerCase().c_str()),
			&cssColor
		) )
		{
			myCanvas->Brush->Color = (TColor)((cssColor.blue << 16)
				| (cssColor.green << 8)
				| cssColor.red)
			;
		}
		else
			myCanvas->Brush->Color = clYellow;
		myCanvas->Rectangle( right );
	}
	if( bottomRight.Width() && bottomRight.Height() )
	{
		if( css::Styles::parseColor(
			css::Value(ComboBoxColorBottomRight->Text.LowerCase().c_str()),
			&cssColor
		) )
		{
			myCanvas->Brush->Color = (TColor)((cssColor.blue << 16)
				| (cssColor.green << 8)
				| cssColor.red)
			;
		}
		else
			myCanvas->Brush->Color = clPurple;
		myCanvas->Rectangle( bottomRight );
	}
	if( bottom.Width() && bottom.Height() )
	{
		if( css::Styles::parseColor(
			css::Value(ComboBoxColorBottom->Text.LowerCase().c_str()),
			&cssColor
		) )
		{
			myCanvas->Brush->Color = (TColor)((cssColor.blue << 16)
				| (cssColor.green << 8)
				| cssColor.red)
			;
		}
		else
			myCanvas->Brush->Color = clRed;
		myCanvas->Rectangle( bottom );
	}
	if( bottomLeft.Width() && bottomLeft.Height() )
	{
		if( css::Styles::parseColor(
			css::Value(ComboBoxColorBottomLeft->Text.LowerCase().c_str()),
			&cssColor
		) )
		{
			myCanvas->Brush->Color = (TColor)((cssColor.blue << 16)
				| (cssColor.green << 8)
				| cssColor.red)
			;
		}
		else
			myCanvas->Brush->Color = clTeal;
		myCanvas->Rectangle( bottomLeft );
	}
}
//---------------------------------------------------------------------------
void __fastcall TWebWizardForm::ButtonEditClick(TObject *)
{
	TEditXmlFileForm	*theEditor = new TEditXmlFileForm( (TComponent*)NULL );
	theEditor->setDocument( configDocument );
	theEditor->ShowModal();
	configDocument->clearTypes();
}
//---------------------------------------------------------------------------

