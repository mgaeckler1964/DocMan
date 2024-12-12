//---------------------------------------------------------------------------

#ifndef VersionsFrmH
#define VersionsFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBTables.hpp>

#include <DBGrids.hpp>
#include <Grids.hpp>
#include <DBCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <ImgList.hpp>
#include <Mask.hpp>

#include "file.h"
#include "RemoteItems.h"
#include <DBClient.hpp>

//---------------------------------------------------------------------------

// class THE_FILE;
class TVersionsForm : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
	TTable *TableVersions;
	TDataSource *DataSourceVersions;
	TDBGrid *DBGridVersions;
	TPanel *Panel1;
	TDBNavigator *DBNavigator;
	TIntegerField *TableVersionsID;
	TIntegerField *TableVersionsFILE_ID;
	TIntegerField *TableVersionsVERSION;
	TStringField *TableVersionsMIME_TYPE;
	TStringField *TableVersionsFILENAME;
	TIntegerField *TableVersionsSTORAGE_ID;
	TIntegerField *TableVersionsCREATEDBY;
	TDateTimeField *TableVersionsCREATEDDATE;
	TDateTimeField *TableVersionsMODIFIEDDATE;
	TIntegerField *TableVersionsLOCKED;
	TSpeedButton *SpeedButtonLock;
	TSpeedButton *SpeedButtonUnlock;
	TQuery *QueryUser;
	TStringField *TableVersionsCREATORNAME;
	TSpeedButton *SpeedButtonMark;
	TIntegerField *TableVersionsFILESIZE;
	TDateTimeField *TableVersionsFILECREATEDDATE;
	TDateTimeField *TableVersionsFILEMODIFIEDDATE;
	TMemoField *TableVersionsDESCRIPTION;
	TDBMemo *DBMemoDescription;
	TSplitter *Splitter1;
	TSpeedButton *SpeedButtonCompare;
	TSpeedButton *SpeedButtonMakeCurrent;
	TQuery *QueryMimeTypes;
	TStringField *QueryMimeTypesMIMETYPE;
	TButton *ButtonPurgeVersions;
	TEdit *EditNumVersions;
	TButton *ButtonExif;
	TButton *ButtonAttributes;
	TClientDataSet *ClientDataSetVersions;
	TIntegerField *ClientDataSetVersionsID;
	TIntegerField *ClientDataSetVersionsitemID;
	TIntegerField *ClientDataSetVersionscreateBy;
	TStringField *ClientDataSetVersionsmimeType;
	TDateTimeField *ClientDataSetVersionscreateDate;
	TDateTimeField *ClientDataSetVersionsmodifiedDate;
	TMemoField *ClientDataSetVersionsDESCRIPTION;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall DeleteFileVersion(TDataSet *DataSet);
	void __fastcall CreateFileVersion(TDataSet *DataSet);
	void __fastcall TableVersionsAfterScroll(TDataSet *DataSet);
	void __fastcall SpeedButtonLockClick(TObject *Sender);
	void __fastcall DBGridVersionsDrawColumnCell(TObject *Sender,
          const TRect &Rect, int DataCol, TColumn *Column,
          TGridDrawState State);
	void __fastcall TableVersionsBeforePost(TDataSet *DataSet);
	void __fastcall SpeedButtonMarkClick(TObject *Sender);
	void __fastcall DBGridVersionsDblClick(TObject *Sender);
	void __fastcall SpeedButtonCompareClick(TObject *Sender);
	void __fastcall SpeedButtonMakeCurrentClick(TObject *Sender);
	void __fastcall EditNumVersionsKeyPress(TObject *Sender, char &Key);
	void __fastcall EditNumVersionsChange(TObject *Sender);
	void __fastcall ButtonPurgeVersionsClick(TObject *Sender);
	void __fastcall ButtonAttributesClick(TObject *Sender);
	void __fastcall ButtonExifClick(TObject *Sender);
	void __fastcall ClientDataSetVersionsAfterOpen(TDataSet *DataSet);
	void __fastcall ClientDataSetVersionsBeforePost(TDataSet *DataSet);
private:	// Anwender-Deklarationen
	PTR_FILE			theDbFile;
	PTR_REMOTE_FILE		theRemoteFile;
	TBookmark			bm;
	int					bookmarkedVersion;

	void loadRemoteVersions( void );
public:		// Anwender-Deklarationen
	void setItem( const PTR_FILE_BASE &theFile )
	{
		theDbFile = theFile;
		theRemoteFile = theFile;
	}
	__fastcall TVersionsForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TVersionsForm *VersionsForm;
//---------------------------------------------------------------------------
#endif
