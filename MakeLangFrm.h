//---------------------------------------------------------------------------

#ifndef MakeLangFrmH
#define MakeLangFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TMakeLanguageForm : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
	TLabel *Label1;
	TEdit *EditLanguage;
	TButton *Button1;
	TButton *Button2;
private:	// Anwender-Deklarationen
public:		// Anwender-Deklarationen
	__fastcall TMakeLanguageForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMakeLanguageForm *MakeLanguageForm;
//---------------------------------------------------------------------------
#endif
