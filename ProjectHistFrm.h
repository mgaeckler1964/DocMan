//---------------------------------------------------------------------------

#ifndef ProjectHistFrmH
#define ProjectHistFrmH
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
#include <DBClient.hpp>

#include <gak/string.h>

#include "ItemManager.h"

//---------------------------------------------------------------------------

struct VERSIONS
{
	STRING		NAME;
	int			versionID, STORAGE_ID;
	STRING		MIME_TYPE, fileName;
	int			FILESIZE;
	TDateTime	CREATEDDATE, FILEMODIFIEDDATE;
	bool		LOCKED;
	STRING		DESCRIPTION, USERNAME;
};

// class TLocalDataSet;
//---------------------------------------------------------------------------

class TProjectHistoryForm : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
	TDataSource *DataSourceVersions;
	TDBGrid *DBGridVersions;
	TPanel *Panel1;
	TDBNavigator *DBNavigator;
	TDBMemo *DBMemoDescription;
	TSplitter *Splitter1;
	TQuery *QueryVersions;
	TStringField *QueryVersionsNAME;
	TIntegerField *QueryVersionsVERSION;
	TStringField *QueryVersionsMIME_TYPE;
	TStringField *QueryVersionsFILENAME;
	TIntegerField *QueryVersionsFILESIZE;
	TDateTimeField *QueryVersionsFILEMODIFIEDDATE;
	TIntegerField *QueryVersionsLOCKED;
	TStringField *QueryVersionsUSERNAME;
	TMemoField *QueryVersionsDESCRIPTION;
	TClientDataSet *ClientDataSetVersions;
	TIntegerField *ClientDataSetVersionsVERSION;
	TStringField *ClientDataSetVersionsMIME_TYPE;
	TStringField *ClientDataSetVersionsFILENAME;
	TIntegerField *ClientDataSetVersionsFILESIZE;
	TDateTimeField *ClientDataSetVersionsFILEMODIFIEDDATE;
	TStringField *ClientDataSetVersionsUSERNAME;
	TIntegerField *ClientDataSetVersionsLOCKED;
	TMemoField *ClientDataSetVersionsDESCRIPTION;
	TStringField *ClientDataSetVersionsNAME;
	TIntegerField *QueryVersionsID;
	TIntegerField *QueryVersionsSTORAGE_ID;
	TDateTimeField *QueryVersionsCREATEDDATE;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall DBGridVersionsDrawColumnCell(TObject *Sender,
		  const TRect &Rect, int DataCol, TColumn *Column,
		  TGridDrawState State);
private:	// Anwender-Deklarationen
//	TLocalDataSet	*dataSet;

	PTR_ITEM	theItem;
	void	appendVersions( Array<VERSIONS>	*dataArray, const PTR_ITEM &theItem );
public:		// Anwender-Deklarationen
	void setItem( const PTR_ITEM &theItem )
	{
		this->theItem = theItem;
	}
	__fastcall TProjectHistoryForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TProjectHistoryForm *ProjectHistoryForm;
//---------------------------------------------------------------------------
#endif
