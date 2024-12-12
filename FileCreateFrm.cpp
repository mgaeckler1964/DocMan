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

#include <gak/string.h>

#pragma hdrstop

#include "FileCreateFrm.h"
#include "ItemManager.h"
//---------------------------------------------------------------------------
using namespace gak;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ItemCreateFrm"
#pragma resource "*.dfm"
TFileCreateForm *FileCreateForm;
//---------------------------------------------------------------------------
__fastcall TFileCreateForm::TFileCreateForm(TComponent* Owner)
	: TItemCreateForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TFileCreateForm::ButtonBrowseClick(TObject *)
{
	if( OpenDialog->Execute() )
	{
		EditFilePath->Text = OpenDialog->FileName;
		if( EditName->Text == "" )
		{
			STRING	filePath = OpenDialog->FileName.c_str();
			size_t	dirPos = filePath.searchRChar( DIRECTORY_DELIMITER )+1;
			filePath += dirPos;
			EditName->Text = (const char *)filePath;
			ComboBoxTemplate->ItemIndex = 0;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TFileCreateForm::FormShow(TObject *Sender)
{
	TItemCreateForm::FormShow( Sender );
	EditName->Text = "";
	EditFilePath->Text = "";
	OpenDialog->FileName = "";

	if( theParent )
	{
		STRING	downloadPath = theParent->getDownloadPath();
		if( !downloadPath.isEmpty() )
			OpenDialog->InitialDir = (const char *)downloadPath;
	}

	STRING		templateName;
	size_t		i;
	PTR_ITEM	theTemplate;
	PTR_FILE	theFileTemplate;

	ComboBoxTemplate->Items->Clear();
	ComboBoxTemplate->Items->Add( "<NONE>" );
	templates.clear();
	templates.addElement( theFileTemplate );

	PTR_ITEM	theTemplateFolder = getPersonalItem(
		TYPE_PERSONAL_TEMPLATE_FOLDER
	);
	if( theTemplateFolder )
	{
		for(
			i=0, theTemplateFolder->getContent();
			theTemplate = theTemplateFolder->getContentItem( i );
			i++
		)
		{
			theFileTemplate = theTemplate;
			if( theFileTemplate )
			{
				templateName = "Private: ";
				templateName += theTemplate->getName();
				ComboBoxTemplate->Items->Add( (const char *)templateName );
				templates.addElement( theFileTemplate );
			}
		}
	}
	theTemplateFolder = getPublicVolume( TYPE_PUBLIC_TEMPLATE_FOLDER );
	if( theTemplateFolder )
	{
		for(
			i=0, theTemplateFolder->getContent();
			theTemplate = theTemplateFolder->getContentItem( i );
			i++
		)
		{
			theFileTemplate = theTemplate;
			if( theFileTemplate )
			{
				templateName = "Public: ";
				templateName += theTemplate->getName();
				ComboBoxTemplate->Items->Add( (const char *)templateName );
				templates.addElement( theFileTemplate );
			}
		}
	}
	ComboBoxTemplate->ItemIndex = 0;
	vcl::bringWindowToFront( this );
}
//---------------------------------------------------------------------------


