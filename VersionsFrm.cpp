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

#include "VersionsFrm.h"
#include "CreateVersionFrm.h"
#include "ActionManager.h"
#include "File.h"
#include "PermissionsFrm.h"
#include "DiffFrm.h"
#include "AttributesFrm.h"
#include "ExifFrm.h"
#include "RemoteItems.h"
#include "DocManService.h"

//---------------------------------------------------------------------------
using namespace gak;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TVersionsForm *VersionsForm;
//---------------------------------------------------------------------------
class ACTION_VERSIONS : public ACTION_BASE_VERSIONS
{
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
};
//---------------------------------------------------------------------------
bool ACTION_VERSIONS::acceptItem( THE_ITEM *theItem )
{
	const THE_FILE_BASE *theFile = dynamic_cast<const THE_FILE_BASE *>(theItem);
	return theFile ? ACTION_BASE::acceptItem( theItem ) : false;
}
//---------------------------------------------------------------------------
const char *ACTION_VERSIONS::getLabel( void ) const
{
	return "Versions...";
}
//---------------------------------------------------------------------------
REFRESH_TYPE ACTION_VERSIONS::perform( PTR_ITEM theItem )
{
	PTR_FILE_BASE	theFile( theItem );
	VersionsForm->setItem( theFile );
	VersionsForm->ShowModal();

	return REFRESH_REDRAW;
}
//---------------------------------------------------------------------------
static ACTION_VERSIONS theAction;
//---------------------------------------------------------------------------
__fastcall TVersionsForm::TVersionsForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void TVersionsForm::loadRemoteVersions( void )
{
	doEnterFunction("TVersionsForm::loadRemoteVersions");
	DocManService *theService = theRemoteFile->getService();

	try
	{
		xml::Element *result = theService->sGetVersions(
			theRemoteFile->getRemoteFileID(), -1
		);
		if( result )
		{
			typedef Array<DocManService::VersionRecord_t> Versions;
			Versions	versions;
			theService->fillVersionRecord( versions, result );

			ClientDataSetVersions->Tag = 1;
			ClientDataSetVersions->EmptyDataSet();

			ClientDataSetVersionsID->ReadOnly = false;
			ClientDataSetVersionsitemID->ReadOnly = false;
			ClientDataSetVersionscreateBy->ReadOnly = false;
			ClientDataSetVersionscreateDate->ReadOnly = false;
			ClientDataSetVersionsmodifiedDate->ReadOnly = false;

			for(
				Versions::const_iterator it = versions.cbegin(),
					endIT = versions.cend();
				it != endIT;
				++it
			)
			{
				const DocManService::VersionRecord_t &theVersion = *it;

				ClientDataSetVersions->Append();
				ClientDataSetVersionsID->AsInteger = theVersion.id;
				ClientDataSetVersionsitemID->AsInteger = theVersion.itemID;
				ClientDataSetVersionscreateBy->AsInteger = theVersion.createBy;
				ClientDataSetVersionsmimeType->AsString = (const char *)theVersion.mimeType;
				ClientDataSetVersionscreateDate->AsDateTime = gak::DateTime( (time_t)theVersion.createDate );
				ClientDataSetVersionsmodifiedDate->AsDateTime = gak::DateTime( (time_t)theVersion.modifiedDate );
				ClientDataSetVersionsDESCRIPTION->AsString = (const char *)theVersion.description;

				ClientDataSetVersions->Post();
			}

			ClientDataSetVersionsID->ReadOnly = true;
			ClientDataSetVersionsitemID->ReadOnly = true;
			ClientDataSetVersionscreateBy->ReadOnly = true;
			ClientDataSetVersionscreateDate->ReadOnly = true;
			ClientDataSetVersionsmodifiedDate->ReadOnly = true;

			ClientDataSetVersions->Tag = 0;
			ClientDataSetVersions->First();
		}
		else
		{
			doLogValue( theService->getBody() );
/*@*/		throw Exception( "Illegal result" );
		}
	}
	catch( net::SoapRequest::SoapException &e )
	{
		STRING	errText = e.faultString;
		if( !e.faultDetail.isEmpty() )
		{
			errText += ": ";
			errText += e.faultDetail;
		}
		doLogValue( theService->getBody() );
/*@*/	throw EDatabaseError( (const char *)errText );
	}
	catch( std::exception &e )
	{
		doLogValue( theService->getBody() );
/*@*/	throw Exception( e.what() );
	}
}
//---------------------------------------------------------------------------
void __fastcall TVersionsForm::FormShow(TObject *)
{
	int	buttonCount = 10;

	STRING	newCaption = "Versions of ";
	Dbctrls::TButtonSet VisibleButtons = DBNavigator->VisibleButtons;

	if( theDbFile )
	{
		newCaption += theDbFile->getName();

		int perms = theDbFile->loadPermissions();

		if( theDbFile->canCreateVersion() )
		{
			VisibleButtons << nbInsert;
			SpeedButtonMakeCurrent->Enabled = true;
		}
		else
		{
			VisibleButtons >> nbInsert;
			buttonCount--;
			SpeedButtonMakeCurrent->Enabled = false;
		}

		if( perms & ITEM_PERM_DELETE_VERSION )
			VisibleButtons << nbDelete;
		else
		{
			VisibleButtons >> nbDelete;
			buttonCount--;
		}

		if( perms & ITEM_PERM_MODIFY )
		{
			VisibleButtons << nbEdit;
			VisibleButtons << nbPost;
			VisibleButtons << nbCancel;
			DataSourceVersions->AutoEdit = true;
			SpeedButtonLock->Enabled = true;
			SpeedButtonUnlock->Enabled = true;
		}
		else
		{
			VisibleButtons >> nbEdit;
			VisibleButtons >> nbPost;
			VisibleButtons >> nbCancel;
			DataSourceVersions->AutoEdit = false;
			SpeedButtonLock->Enabled = false;
			SpeedButtonUnlock->Enabled = false;
			buttonCount -= 3;
		}

		VisibleButtons << nbRefresh;
		ButtonExif->Enabled = true;
		ButtonAttributes->Enabled = true;

		char filter[128];
		TableVersions->Filtered = true;
		sprintf( filter, "FILE_ID=%d", theDbFile->getFileID() );
		TableVersions->Filter = filter;

		DataSourceVersions->DataSet = TableVersions;

		QueryUser->Open();
		TableVersions->Open();

	}
	else if( theRemoteFile )
	{
		newCaption += theRemoteFile->getName();

		if( theRemoteFile->canCreateVersion() )
		{
			VisibleButtons << nbInsert;
			VisibleButtons << nbDelete;
			VisibleButtons << nbEdit;
			VisibleButtons << nbPost;
			VisibleButtons << nbCancel;
			SpeedButtonMakeCurrent->Enabled = true;
			DataSourceVersions->AutoEdit = true;
		}
		else
		{
			VisibleButtons >> nbInsert;
			VisibleButtons >> nbDelete;
			VisibleButtons >> nbEdit;
			VisibleButtons >> nbPost;
			VisibleButtons >> nbCancel;
			buttonCount -= 5;
			SpeedButtonMakeCurrent->Enabled = false;
			DataSourceVersions->AutoEdit = false;
		}

		VisibleButtons >> nbRefresh;
		buttonCount--;
		SpeedButtonLock->Enabled = false;
		SpeedButtonUnlock->Enabled = false;
		SpeedButtonMakeCurrent->Enabled = false;
		ButtonExif->Enabled = false;
		ButtonAttributes->Enabled = false;

		DataSourceVersions->DataSet = ClientDataSetVersions;
		ClientDataSetVersions->CreateDataSet();
	}

	DBNavigator->VisibleButtons = VisibleButtons;
	DBNavigator->Width = buttonCount * 25;

	Caption = (const char *)newCaption;

	SpeedButtonCompare->Enabled = false;
	bookmarkedVersion = 0;
	bm = NULL;

	// second column contains the mime type
	DBGridVersions->Columns->Items[1]->PickList->Clear();
	for( QueryMimeTypes->Open();
		!QueryMimeTypes->Eof;
		QueryMimeTypes->Next()
	)
	{
		DBGridVersions->Columns->Items[1]->PickList->Add(
			QueryMimeTypesMIMETYPE->AsString
		);
	}
	QueryMimeTypes->Close();

	EditNumVersions->Text = "1";
}
//---------------------------------------------------------------------------
void __fastcall TVersionsForm::FormClose(TObject *,
	  TCloseAction &)
{
	if( bm )
		TableVersions->FreeBookmark( bm );

	TableVersions->Close();
	QueryUser->Close();

	ClientDataSetVersions->Close();
}
//---------------------------------------------------------------------------


void __fastcall TVersionsForm::DeleteFileVersion(TDataSet *DataSet )
{
	if( DataSet->Tag )
/***/	return;

	if( DataSet->RecordCount <= 1)
	{
/*@*/	throw EAbort( "Dummy" );
	}

	if( theDbFile )
	{
		if( !TableVersionsLOCKED->AsInteger )
		{
			theDbFile->deleteVersion(
				TableVersionsID->AsInteger,
				TableVersionsSTORAGE_ID->AsInteger,
				true
			);
			TableVersions->Refresh();
		}
	}
	else if( theRemoteFile )
	{
		theRemoteFile->deleteVersion(
			ClientDataSetVersionsID->AsInteger
		);
		ClientDataSetVersions->Close();
		ClientDataSetVersions->CreateDataSet();
	}

	throw EAbort( "Dummy" );
}
//---------------------------------------------------------------------------

void __fastcall TVersionsForm::CreateFileVersion(TDataSet *DataSet )
{
	if( DataSet->Tag )
/***/	return;

	if( VersionCreateForm->ShowModal() == mrOk )
	{
		if( DataSet == TableVersions && theDbFile )
		{
			theDbFile->createVersion(
				VersionCreateForm->EditFilePath->Text.c_str(),
				VersionCreateForm->MemoDescription->Text.c_str()
			);
			TableVersions->Refresh();
		}
		else if( DataSet == ClientDataSetVersions && theRemoteFile )
		{
			theRemoteFile->createVersion(
				VersionCreateForm->EditFilePath->Text.c_str(),
				VersionCreateForm->MemoDescription->Text.c_str()
			);
			ClientDataSetVersions->Close();
			ClientDataSetVersions->CreateDataSet();
		}
	}

	throw EAbort( "Dummy" );
}
//---------------------------------------------------------------------------

void __fastcall TVersionsForm::TableVersionsAfterScroll(TDataSet *)
{
	if( TableVersionsLOCKED->AsInteger )
	{
		SpeedButtonUnlock->Visible = true;
		SpeedButtonLock->Visible = false;
	}
	else
	{
		SpeedButtonUnlock->Visible = false;
		SpeedButtonLock->Visible = true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TVersionsForm::SpeedButtonLockClick(TObject *)
{
	if( TableVersions->State == dsBrowse )
		TableVersions->Edit();

	if( TableVersionsLOCKED->AsInteger )
		TableVersionsLOCKED->AsInteger = 0;
	else
		TableVersionsLOCKED->AsInteger = 1;

	TableVersionsAfterScroll( TableVersions );
}
//---------------------------------------------------------------------------



void __fastcall TVersionsForm::DBGridVersionsDrawColumnCell(
	  TObject *, const TRect &Rect, int DataCol, TColumn *Column,
	  TGridDrawState State)
{
	if( State.Contains( gdFixed ) )
		DBGridVersions->Canvas->Brush->Color = (TColor)0xFFAAAA;

	if( theDbFile && TableVersionsLOCKED->AsInteger )
		DBGridVersions->Canvas->Brush->Color = (TColor)0xAAAAFF;

	if( bm && DataCol == 0 )
	{
		TDataSet	*theDataset = DataSourceVersions->DataSet;
		TBookmark	current = theDataset->GetBookmark();
		if( !theDataset->CompareBookmarks( current, bm ) )
			DBGridVersions->Canvas->Brush->Color = (TColor)0xAAFFAA;
	}
	DBGridVersions->DefaultDrawColumnCell( Rect, DataCol, Column, State);
}
//---------------------------------------------------------------------------

void __fastcall TVersionsForm::TableVersionsBeforePost(TDataSet *)
{
	TableVersionsMODIFIEDDATE->ReadOnly = false;
	TableVersionsMODIFIEDDATE->AsDateTime = TDateTime::CurrentDateTime();
	TableVersionsMODIFIEDDATE->ReadOnly = true;
}
//---------------------------------------------------------------------------

void __fastcall TVersionsForm::SpeedButtonMarkClick(TObject *)
{
	TDataSet	*theDataset = DataSourceVersions->DataSet;

	if( bm )
		theDataset->FreeBookmark( bm );

	bm = theDataset->GetBookmark();
	DBGridVersions->Invalidate();
	SpeedButtonCompare->Enabled = true;
	if( theDbFile )
		bookmarkedVersion = TableVersionsVERSION->AsInteger;
	else if( theRemoteFile )
		bookmarkedVersion = ClientDataSetVersionsID->AsInteger;
}
//---------------------------------------------------------------------------


void __fastcall TVersionsForm::DBGridVersionsDblClick(TObject *)
{
	if( theDbFile )
		theDbFile->openVersion( TableVersionsVERSION->AsInteger );
}
//---------------------------------------------------------------------------

void __fastcall TVersionsForm::SpeedButtonCompareClick(TObject *)
{

	if( bm )
	{
		if( theDbFile )
		{
			int currentVersion = TableVersionsVERSION->AsInteger;

			if( currentVersion > bookmarkedVersion )
				DiffForm->ShowDiff( theDbFile, bookmarkedVersion, currentVersion );
			else
				DiffForm->ShowDiff( theDbFile, currentVersion, bookmarkedVersion );
		}
		else if( theRemoteFile )
		{
			int currentVersion = ClientDataSetVersionsID->AsInteger;

			if( currentVersion > bookmarkedVersion )
				DiffForm->ShowDiff( theRemoteFile, bookmarkedVersion, currentVersion );
			else
				DiffForm->ShowDiff( theRemoteFile, currentVersion, bookmarkedVersion );
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TVersionsForm::SpeedButtonMakeCurrentClick(TObject *)
{
	if( theDbFile )
	{
		int srcVersion = TableVersionsVERSION->AsInteger;
		int latestVersion = theDbFile->getVersionNum();
		if( srcVersion != latestVersion )
		{
			theDbFile->createVersion( srcVersion );
			TableVersions->Refresh();
		}
	}
}
//---------------------------------------------------------------------------


void __fastcall TVersionsForm::EditNumVersionsKeyPress(TObject *, char &Key)
{
	if( (Key < 0 || Key >= ' ') && (Key < '0' || Key > '9') )
		Key = 0;
}
//---------------------------------------------------------------------------

void __fastcall TVersionsForm::EditNumVersionsChange(TObject *)
{
	int	numVersion = atoi( EditNumVersions->Text.c_str() );

	if( numVersion >= 1 )
		ButtonPurgeVersions->Enabled = true;
	else
		ButtonPurgeVersions->Enabled = false;

}
//---------------------------------------------------------------------------

void __fastcall TVersionsForm::ButtonPurgeVersionsClick(TObject *)
{
	if( Application->MessageBox(
		"Are You sure to delete old versions?", "Warning",
		MB_OKCANCEL|MB_ICONWARNING
	) == IDOK )
	{
		int	numVersion = atoi( EditNumVersions->Text.c_str() );
		if( numVersion >= 1 )
		{
			TDataSet	*DataSet;
			if( theDbFile )
			{
				DataSet = TableVersions;

				DataSet->First();
				while( !DataSet->Eof && numVersion-- )
					DataSet->Next();

				while( !DataSet->Eof )
				{
					if( !TableVersionsLOCKED->AsInteger )
					{
						theDbFile->deleteVersion(
							TableVersionsID->AsInteger,
							TableVersionsSTORAGE_ID->AsInteger,
							true
						);
					}

					DataSet->Next();
				}

				TableVersions->Refresh();
			}
			else if( theRemoteFile )
			{
				DataSet = ClientDataSetVersions;

				DataSet->First();
				while( !DataSet->Eof && numVersion-- )
					DataSet->Next();

				while( !DataSet->Eof )
				{
					theRemoteFile->deleteVersion(
						ClientDataSetVersionsID->AsInteger
					);

					DataSet->Next();
				}

				ClientDataSetVersions->Close();
				ClientDataSetVersions->CreateDataSet();
			}
		}
	}
}

//---------------------------------------------------------------------------

void __fastcall TVersionsForm::ButtonAttributesClick(TObject *)
{
	if( theDbFile )
	{
		AttributesForm->setItem( theDbFile, TableVersionsVERSION->AsInteger );
		AttributesForm->ShowModal();
	}
}
//---------------------------------------------------------------------------

void __fastcall TVersionsForm::ButtonExifClick(TObject *)
{
	if( theDbFile )
	{
		ExifForm->setItem( theDbFile, TableVersionsVERSION->AsInteger );
		ExifForm->ShowModal();
	}
}
//---------------------------------------------------------------------------


void __fastcall TVersionsForm::ClientDataSetVersionsAfterOpen(
	  TDataSet *)
{
	loadRemoteVersions();
}
//---------------------------------------------------------------------------

void __fastcall TVersionsForm::ClientDataSetVersionsBeforePost(
      TDataSet *)
{
	DocManService *theService = theRemoteFile->getService();

	try
	{
		theService->sUpdateVersion(
			ClientDataSetVersionsitemID->AsInteger,
			ClientDataSetVersionsID->AsInteger,
			ClientDataSetVersionsmimeType->AsString.c_str(),
			ClientDataSetVersionsDESCRIPTION->AsString.c_str()
		);
	}
	catch( net::SoapRequest::SoapException &e )
	{
		STRING	errText = e.faultString;
		if( !e.faultDetail.isEmpty() )
		{
			errText += ": ";
			errText += e.faultDetail;
		}
		doLogValue( theService->getBody() );
/*@*/	throw EDatabaseError( (const char *)errText );
	}
	catch( std::exception &e )
	{
		doLogValue( theService->getBody() );
/*@*/	throw Exception( e.what() );
	}
}
//---------------------------------------------------------------------------

