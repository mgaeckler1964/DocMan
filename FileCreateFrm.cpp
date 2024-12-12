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


