//---------------------------------------------------------------------------

#ifndef GeneralFrmH
#define GeneralFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ItemCreateFrm.h"
//---------------------------------------------------------------------------
class TGeneralForm : public TItemCreateForm
{
__published:	// IDE-managed Components
	TLabel *Label2;
	TLabel *LabelType;
	TLabel *Label3;
	TLabel *LabelCreator;
	TLabel *LabelReserved;
	TLabel *LabelReservedFor;
	TLabel *Label5;
	TLabel *LabelCreatedDate;
	TLabel *Label6;
	TLabel *LabelModifiedDate;
	TLabel *Label4;
	TLabel *LabelLocalPath;
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TGeneralForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TGeneralForm *GeneralForm;
//---------------------------------------------------------------------------
#endif
