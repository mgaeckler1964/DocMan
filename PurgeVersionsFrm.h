//---------------------------------------------------------------------------

#ifndef PurgeVersionsFrmH
#define PurgeVersionsFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TPurgeVersionsForm : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
	TEdit *EditNumVersions;
	TButton *ButtonOK;
	TLabel *Label1;
	TButton *ButtonCancel;
	void __fastcall EditNumVersionsKeyPress(TObject *Sender, char &Key);
	void __fastcall EditNumVersionsChange(TObject *Sender);
private:	// Anwender-Deklarationen
public:		// Anwender-Deklarationen
	__fastcall TPurgeVersionsForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TPurgeVersionsForm *PurgeVersionsForm;
//---------------------------------------------------------------------------
#endif
