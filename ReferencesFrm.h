//---------------------------------------------------------------------------

#ifndef ReferencesFrmH
#define ReferencesFrmH
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
class TReferencesForm : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
	TQuery *QueryReferences;
	TDataSource *DataSourceLinks;
	TDBGrid *DBGridLinks;
	TIntegerField *QueryReferencesID;
	TStringField *QueryReferencesPATH;
	void __fastcall QueryReferencesCalcFields(TDataSet *DataSet);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// Anwender-Deklarationen
	PTR_ITEM	theItem;
public:		// Anwender-Deklarationen
	void setItem( const PTR_ITEM &theItem )
	{
		this->theItem = theItem;
	}
	__fastcall TReferencesForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TReferencesForm *ReferencesForm;
//---------------------------------------------------------------------------
#endif
