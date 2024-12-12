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
