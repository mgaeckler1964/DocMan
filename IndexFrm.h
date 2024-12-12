//---------------------------------------------------------------------------

#ifndef IndexFrmH
#define IndexFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

#include "DocManBG.h"
#include <Grids.hpp>

//---------------------------------------------------------------------------
class TIndexForm : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TLabel *Label1;
	TLabel *Label9;
	TEdit *EditKeyword;
	TButton *ButtonSearch;
	TRadioButton *RadioButtonAny;
	TRadioButton *RadioButtonCompany;
	TRadioButton *RadioButtonPrivate;
	TRadioButton *RadioButtonArchive;
	TStringGrid *GridResult;
	TLabel *Label2;
	TCheckBox *CheckBoxCurrentVersion;
	TCheckBox *CheckBoxCaseSensitive;
	TCheckBox *CheckBoxFuzzy;
	TCheckBox *CheckBoxWildcard;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ButtonSearchClick(TObject *Sender);
	void __fastcall GridResultDblClick(TObject *Sender);
private:	// User declarations
	DocManIndex			m_globalIndex;
	gak::PODarray<int>	m_itemIDs;

public:		// User declarations
	__fastcall TIndexForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TIndexForm *IndexForm;
//---------------------------------------------------------------------------
#endif

