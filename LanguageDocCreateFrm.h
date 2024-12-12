//---------------------------------------------------------------------------

#ifndef LanguageDocCreateFrmH
#define LanguageDocCreateFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "FileCreateFrm.h"
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TLanguageDocCreateForm : public TFileCreateForm
{
__published:	// Von der IDE verwaltete Komponenten
	TLabel *Label3;
	TEdit *EditDefaultLanguage;
private:	// Anwender-Deklarationen
public:		// Anwender-Deklarationen
	__fastcall TLanguageDocCreateForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TLanguageDocCreateForm *LanguageDocCreateForm;
//---------------------------------------------------------------------------
#endif
