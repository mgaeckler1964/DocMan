//---------------------------------------------------------------------------

#ifndef LanguageItemCreateFrmH
#define LanguageItemCreateFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "FileCreateFrm.h"
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TLanguageItemCreateForm : public TFileCreateForm
{
__published:	// Von der IDE verwaltete Komponenten
private:	// Anwender-Deklarationen
public:		// Anwender-Deklarationen
	__fastcall TLanguageItemCreateForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TLanguageItemCreateForm *LanguageItemCreateForm;
//---------------------------------------------------------------------------
#endif
