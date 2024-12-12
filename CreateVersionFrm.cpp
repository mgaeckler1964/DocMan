//---------------------------------------------------------------------------

#include <vcl.h>

#include <gak/directory.h>

#pragma hdrstop

#include "CreateVersionFrm.h"
#include "PermissionsFrm.h"
#include "DocManMain.h"
#include "File.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TVersionCreateForm *VersionCreateForm;
ACTION_ADD_VERSION theCreateVersionAction;
//---------------------------------------------------------------------------

REFRESH_TYPE ACTION_ADD_VERSION::perform( PTR_ITEM theItem )
{
	PTR_FILE_BASE theFile = theItem;
	if( theFile )
	{
		STRING	downloadPath = theFile->getDownloadPath();
		VersionCreateForm->OpenDialog->InitialDir = (const char *)downloadPath;
		downloadPath += theFile->getName();

		if( !isFile( downloadPath ) )
			downloadPath = "";

		VersionCreateForm->EditFilePath->Text = (const char *)downloadPath;
		VersionCreateForm->OpenDialog->FileName = (const char *)downloadPath;

		if( VersionCreateForm->ShowModal() == mrOk )
		{
			theFile->createVersion(
				VersionCreateForm->EditFilePath->Text.c_str(),
				VersionCreateForm->MemoDescription->Text.c_str()
			);
			theFile->updateDatabase();

			return REFRESH_REDRAW;
		}
	}

	return REFRESH_NONE;
}

//---------------------------------------------------------------------------
const char *ACTION_ADD_VERSION::getLabel( void ) const
{
	return "Add Version...";
}
//---------------------------------------------------------------------------
bool ACTION_ADD_VERSION::acceptItem( THE_ITEM *theItem )
{
	doEnterFunction("ACTION_ADD_VERSION::acceptItem");
	bool	accepted = false;

	const THE_FILE_BASE *theFile = dynamic_cast<const THE_FILE_BASE *>(theItem);
	if( theFile )
	{
		accepted = theFile->canCreateVersion() && ACTION_BASE::acceptItem( theItem );
	}

	return accepted;
}
//---------------------------------------------------------------------------
__fastcall TVersionCreateForm::TVersionCreateForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TVersionCreateForm::ButtonBrowseFileClick(TObject *)
{
	if( OpenDialog->Execute() )
		EditFilePath->Text = OpenDialog->FileName;
}
//---------------------------------------------------------------------------


