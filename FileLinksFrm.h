//---------------------------------------------------------------------------

#ifndef FileLinksFrmH
#define FileLinksFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include "File.h"
#include <Db.hpp>
#include <DBGrids.hpp>
#include <DBTables.hpp>
#include <Grids.hpp>

//---------------------------------------------------------------------------
class TFileLinksForm : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
	TQuery *QueryLinks;
	TDataSource *DataSourceLinks;
	TDBGrid *DBGridLinks;
	TIntegerField *QueryLinksID;
	TStringField *QueryLinksPATH;
	void __fastcall QueryLinksCalcFields(TDataSet *DataSet);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// Anwender-Deklarationen
	PTR_FILE	theFile;
public:		// Anwender-Deklarationen
	void setFile( const PTR_FILE &theFile )
	{
		this->theFile = theFile;
	}
	__fastcall TFileLinksForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFileLinksForm *FileLinksForm;
//---------------------------------------------------------------------------
#endif
