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

struct FileVersion
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

	PTR_ITEM	m_theItem;
	void	appendVersions( Array<FileVersion> *dataArray, const PTR_ITEM &theItem );
public:		// Anwender-Deklarationen
	void setItem( const PTR_ITEM &theItem )
	{
		m_theItem = theItem;
	}
	__fastcall TProjectHistoryForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TProjectHistoryForm *ProjectHistoryForm;
//---------------------------------------------------------------------------
#endif
