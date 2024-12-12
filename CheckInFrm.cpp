//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "CheckInFrm.h"
#include "PermissionsFrm.h"
#include "DocManMain.h"

#include "File.h"
#include "Folder.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CreateVersionFrm"
#pragma resource "*.dfm"
TCheckInForm 		*CheckInForm;
ACTION_CHECK_IN 	theCheckInAction;
//---------------------------------------------------------------------------
class ACTION_CHECK_IN_TREE : public ACTION_BASE_CHECK
{
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
};
//---------------------------------------------------------------------------

#pragma option -RT-
class THREAD_CHECK_IN_TREE : public ThreadDocMan
{
	bool doBranch;
	STRING	description;

	virtual const char *getTitle( void ) const;
	virtual void perform( void );

	public:
	THREAD_CHECK_IN_TREE( const PTR_ITEM &theItemToHandle, bool doBranch, const STRING &newDescription )
	: description( newDescription ), ThreadDocMan(theItemToHandle)
	{
		this->doBranch = doBranch;
	}
};
#pragma option -RT.

//---------------------------------------------------------------------------
bool ACTION_CHECK_IN::acceptItem( THE_ITEM *theItem )
{
	const THE_FILE_BASE *theFile = dynamic_cast<const THE_FILE_BASE *>(theItem);
	if( theFile )
		return theFile->canUnreserve( false ) && ACTION_BASE::acceptItem( theItem );

	return false;
}
//---------------------------------------------------------------------------
const char *ACTION_CHECK_IN::getLabel( void ) const
{
	return "Check In...";
}
//---------------------------------------------------------------------------
REFRESH_TYPE ACTION_CHECK_IN::perform( PTR_ITEM theItem )
{
	REFRESH_TYPE	result = REFRESH_NONE;
	PTR_FILE_BASE	theFile = theItem;

	if( theFile && theFile->canUnreserve( false ) )
	{
		STRING	downloadPath = theFile->getDownloadPath();

		CheckInForm->OpenDialog->InitialDir = (const char *)downloadPath;
		CheckInForm->OpenDialog->FileName = "";
		downloadPath += theFile->getName();

		CheckInForm->CheckBoxAddVersion->Checked = isFile( downloadPath );
		CheckInForm->CheckBoxAddVersion->Enabled = true;
		CheckInForm->EditFilePath->Text = (const char *)downloadPath;
		CheckInForm->EditFilePath->Enabled = true;
		CheckInForm->ButtonBrowseFile->Enabled = true;

		CheckInForm->CheckBoxBranch->Checked = false;
		if( theFile->getNumLinks() <= 1 )
			CheckInForm->CheckBoxBranch->Visible = false;
		else
			CheckInForm->CheckBoxBranch->Visible = true;

		if( CheckInForm->ShowModal() == mrOk )
		{
			result = REFRESH_REDRAW;

			STRING	versionFile = CheckInForm->CheckBoxAddVersion->Checked
				? CheckInForm->EditFilePath->Text.c_str()
				: "";

			if( CheckInForm->CheckBoxBranch->Checked )
				result = REFRESH_RELOAD;

			theFile->unreserve(
				CheckInForm->CheckBoxBranch->Checked,
				CheckInForm->MemoDescription->Text.c_str(),
				versionFile
			);
			theFile->updateDatabase();

		}
	}
	else
	{
		STRING message = "File cannot be unreserved";

/*@*/	throw Exception( (const char*)message );
	}

	return result;
}

//---------------------------------------------------------------------------

bool ACTION_CHECK_IN_TREE::acceptItem( THE_ITEM *theItem )
{
	THE_SOURCE_FOLDER *theFolder = dynamic_cast<THE_SOURCE_FOLDER *>(theItem);
	return theFolder ? ACTION_BASE::acceptItem( theItem ) : false;
}

const char *ACTION_CHECK_IN_TREE::getLabel( void ) const
{
	return "Check In...";
}

const char *THREAD_CHECK_IN_TREE::getTitle( void ) const
{
	return "Check In";
}

void THREAD_CHECK_IN_TREE::perform( void )
{
	PTR_SOURCE_FOLDER theFolder = theItemToHandle;
	if( theFolder )
	{
		theFolder->checkIn( doBranch, description );
	}
}

REFRESH_TYPE ACTION_CHECK_IN_TREE::perform( PTR_ITEM theItem )
{
	PTR_SOURCE_FOLDER theFolder = theItem;
	if( (THE_SOURCE_FOLDER *)theFolder )
	{
		STRING	localPath = theFolder->getDownloadPath();
		if( localPath.isEmpty() )
/*@*/		throw Exception( "Don't know local path" );

		CheckInForm->CheckBoxAddVersion->Checked = true;
		CheckInForm->CheckBoxAddVersion->Enabled = false;
		CheckInForm->EditFilePath->Text = (const char *)localPath;
		CheckInForm->EditFilePath->Enabled = false;
		CheckInForm->ButtonBrowseFile->Enabled = false;

		CheckInForm->CheckBoxBranch->Checked = false;
		CheckInForm->CheckBoxBranch->Visible = true;

		if( CheckInForm->ShowModal() == mrOk )
		{
			THREAD_CHECK_IN_TREE *theThread = new THREAD_CHECK_IN_TREE(
				theFolder,
				CheckInForm->CheckBoxBranch->Checked,
				CheckInForm->MemoDescription->Text.c_str()
			);
			theThread->StartThread();
			return REFRESH_RELOAD;
		}
	}

	return REFRESH_NONE;
}
//---------------------------------------------------------------------------
static ACTION_CHECK_IN_TREE		theTreeAction;

//---------------------------------------------------------------------------
__fastcall TCheckInForm::TCheckInForm(TComponent* Owner)
	: TVersionCreateForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TCheckInForm::CheckBoxAddVersionClick(TObject *)
{
	if( CheckBoxAddVersion->Checked )
	{
		LabelFile->Visible = true;
		EditFilePath->Visible = true;
		ButtonBrowseFile->Visible = true;
		LabelDescription->Visible = true;
		MemoDescription->Visible = true;
	}
	else
	{
		LabelFile->Visible = false;
		EditFilePath->Visible = false;
		ButtonBrowseFile->Visible = false;
		LabelDescription->Visible = false;
		MemoDescription->Visible = false;
	}
}
//---------------------------------------------------------------------------

