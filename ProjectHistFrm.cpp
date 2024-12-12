//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ProjectHistFrm.h"
#include "ActionManager.h"
#include "File.h"
#include "Folder.h"
#include "DocManDM.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TProjectHistoryForm *ProjectHistoryForm;
//---------------------------------------------------------------------------
class TLocalDataSet : public TDataSet
{
	public:
	virtual _fastcall TLocalDataSet( TComponent* Owner ) : TDataSet( Owner ) {};
	virtual char * __fastcall AllocRecordBuffer(void);

};

//---------------------------------------------------------------------------
static int versionsCompare( const VERSIONS &e1, const VERSIONS &e2 )
{
	if( e1.STORAGE_ID > e2.STORAGE_ID )
		return -1;
	else if( e1.STORAGE_ID < e2.STORAGE_ID )
		return 1;
	else if( e1.CREATEDDATE > e2.CREATEDDATE )
		return -1;
	else if( e1.CREATEDDATE < e2.CREATEDDATE )
		return 1;
	else
		return strcmp(e2.NAME,e1.NAME);
}

//---------------------------------------------------------------------------
class ACTION_PROJECT_HISTORY : public ACTION_BASE_VERSIONS
{
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
};
//---------------------------------------------------------------------------
bool ACTION_PROJECT_HISTORY::acceptItem( THE_ITEM *theItem )
{
	if( !theItem->acceptChildType( TYPE_DOCUMENT ) )
		return false;

	return ACTION_BASE_VERSIONS::acceptItem( theItem );
}
//---------------------------------------------------------------------------
const char *ACTION_PROJECT_HISTORY::getLabel( void ) const
{
	return "Project History...";
}
//---------------------------------------------------------------------------
REFRESH_TYPE ACTION_PROJECT_HISTORY::perform( PTR_ITEM theItem )
{
	ProjectHistoryForm->setItem( theItem );
	ProjectHistoryForm->ShowModal();

	return REFRESH_NONE;
}
//---------------------------------------------------------------------------
static ACTION_PROJECT_HISTORY theAction;
//---------------------------------------------------------------------------
__fastcall TProjectHistoryForm::TProjectHistoryForm(TComponent* Owner)
	: TForm(Owner)
{
	// dataSet = new TLocalDataSet( Owner );
}
//---------------------------------------------------------------------------
void TProjectHistoryForm::appendVersions(
	Array<VERSIONS> *dataArray, const PTR_ITEM &theItem
)
{
	doEnterFunction("TProjectHistoryForm::appendVersions");

	STRING		path = theItem->getPath();

	QueryVersions->ParamByName( "theFolderID" )->AsInteger = theItem->getID();
	for(
		QueryVersions->Open();
		!QueryVersions->Eof;
		QueryVersions->Next()
	)
	{
		VERSIONS	&data = dataArray->createElement();

		data.NAME = path + '\\' + QueryVersionsNAME->AsString.c_str();

		data.versionID = QueryVersionsVERSION->AsInteger;
		data.STORAGE_ID = QueryVersionsSTORAGE_ID->AsInteger;
		data.MIME_TYPE = QueryVersionsMIME_TYPE->AsString.c_str();
		data.fileName = QueryVersionsFILENAME->AsString.c_str();
		data.FILESIZE = QueryVersionsFILESIZE->AsInteger;
		data.CREATEDDATE = QueryVersionsCREATEDDATE->AsDateTime;
		data.FILEMODIFIEDDATE = QueryVersionsFILEMODIFIEDDATE->AsDateTime;
		data.LOCKED = QueryVersionsLOCKED->AsInteger;
		data.DESCRIPTION = QueryVersionsDESCRIPTION->AsString.c_str();
		data.USERNAME = QueryVersionsUSERNAME->AsString.c_str();
	}

	QueryVersions->Close();

	ITEM_CONTENT	*theContent = theItem->getContent();
	for(
		ITEM_CONTENT::const_iterator it = theContent->cbegin(),
			endIT = theContent->cend();
		it != endIT;
		++it
	)
	{
		PTR_FOLDER	theSubItem = *it;
		if( theSubItem )
		{
			appendVersions( dataArray, theSubItem );
		}
	}
}

//---------------------------------------------------------------------------
void __fastcall TProjectHistoryForm::FormShow(TObject *)
{
	int				storageId, lastStorageId = -1;
	Array<VERSIONS>	dataArray;
	STRING			path = theItem->getPath();
	size_t			pathLen = path.strlen() + 1;
	STRING			newCaption = "Versions of ";

	newCaption += (*theItem).getName();
	Caption = (const char *)newCaption;

	::SetCursor( LoadCursor( NULL, IDC_WAIT ) );

	appendVersions( &dataArray, theItem );
	dataArray.sort( versionsCompare );

	for( std::size_t i = dataArray.size() -1; i != std::size_t(-1); i-- )
	{
		const VERSIONS	&element = dataArray[i];
		storageId = element.STORAGE_ID;
		if( storageId == lastStorageId )
		{
			dataArray.removeElementAt( i );
		}
		else
		{
			lastStorageId = storageId;
		}
	}

	ClientDataSetVersions->ReadOnly = false;
	ClientDataSetVersions->CreateDataSet();
	for(
		Array<VERSIONS>::const_iterator it = dataArray.cbegin(),
			endIT = dataArray.cend();
		it != endIT;
		++it
	)
	{
		const VERSIONS	&element = *it;
		ClientDataSetVersions->Append();
		ClientDataSetVersionsNAME->AsString = static_cast<const char *>(element.NAME) + pathLen;
		ClientDataSetVersionsVERSION->AsInteger = element.versionID;
		ClientDataSetVersionsMIME_TYPE->AsString = static_cast<const char *>(element.MIME_TYPE);
		ClientDataSetVersionsFILENAME->AsString = static_cast<const char *>(element.fileName);
		ClientDataSetVersionsFILESIZE->AsInteger = element.FILESIZE;
		ClientDataSetVersionsFILEMODIFIEDDATE->AsDateTime = element.FILEMODIFIEDDATE;
		ClientDataSetVersionsLOCKED->AsInteger = element.LOCKED;
		ClientDataSetVersionsDESCRIPTION->AsString = static_cast<const char *>(element.DESCRIPTION);
		ClientDataSetVersionsUSERNAME->AsString = static_cast<const char *>(element.USERNAME);
		ClientDataSetVersions->Post();
	}
	ClientDataSetVersions->ReadOnly = true;
	ClientDataSetVersions->First();

	::SetCursor( LoadCursor( NULL, IDC_ARROW ) );

	SetFocus();
	BringToFront();
}
//---------------------------------------------------------------------------
void __fastcall TProjectHistoryForm::FormClose(TObject *,
	  TCloseAction &)
{
	ClientDataSetVersions->Close();
}
//---------------------------------------------------------------------------
void __fastcall TProjectHistoryForm::DBGridVersionsDrawColumnCell(
	  TObject *, const TRect &Rect, int DataCol, TColumn *Column,
	  TGridDrawState State)
{
	if( State.Contains( gdFixed ) )
		DBGridVersions->Canvas->Brush->Color = (TColor)0xFFAAAA;

	if( ClientDataSetVersionsLOCKED->AsInteger )
		DBGridVersions->Canvas->Brush->Color = (TColor)0xAAAAFF;

	DBGridVersions->DefaultDrawColumnCell( Rect, DataCol, Column, State);
}
//---------------------------------------------------------------------------

