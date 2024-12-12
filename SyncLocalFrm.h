//---------------------------------------------------------------------------

#ifndef SyncLocalFrmH
#define SyncLocalFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TSyncLocalForm : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
	TLabel *Label1;
	TLabel *LabelMachine;
	TLabel *Label2;
	TEdit *EditLocalPath;
	TButton *ButtonCancel;
	TButton *ButtonOK;
	TSpeedButton *SpeedButton;
	void __fastcall SpeedButtonClick(TObject *Sender);
private:	// Anwender-Deklarationen
public:		// Anwender-Deklarationen
	__fastcall TSyncLocalForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSyncLocalForm *SyncLocalForm;
//---------------------------------------------------------------------------
#endif
