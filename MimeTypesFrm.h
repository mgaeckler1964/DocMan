//---------------------------------------------------------------------------

#ifndef MimeTypesFrmH
#define MimeTypesFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBCtrls.hpp>
#include <DBGrids.hpp>
#include <DBTables.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>

#include <gak/map.h>

#include "gaklib.h"

//---------------------------------------------------------------------------
class TMimeTypesForm : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TDBNavigator *DBNavigatorMimeTypes;
	TDBGrid *DBGridMimeTypes;
	TDataSource *DataSourceMimeTypes;
	TTable *TableMimeTypes;
	TStringField *TableMimeTypesEXTENSION;
	TStringField *TableMimeTypesMIMETYPE;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall TableMimeTypesBeforePost(TDataSet *DataSet);
private:	// User declarations

	gak::StrStrMap	mimeTypes;

public:		// User declarations
	__fastcall TMimeTypesForm(TComponent* Owner);
	STRING	GetMimeType( const STRING &fileName );
	STRING	GetExtension( const STRING &mimeType );
};
//---------------------------------------------------------------------------
extern PACKAGE TMimeTypesForm *MimeTypesForm;
//---------------------------------------------------------------------------
#endif
