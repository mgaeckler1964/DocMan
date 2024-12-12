//---------------------------------------------------------------------------

#ifndef DocManDBfrmH
#define DocManDBfrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "dbConvertFrm.h"

//---------------------------------------------------------------------------
class TDocManDbConvertForm : public TDbConvertForm
{
__published:	// IDE-managed Components
private:	// User declarations
	virtual const char *getAliasName( void ) const;
	virtual gak::vcl::DatabaseSchema *createSchema( void ) const;
public:		// User declarations
	__fastcall TDocManDbConvertForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TDocManDbConvertForm *DocManDbConvertForm;
//---------------------------------------------------------------------------
#endif
