//---------------------------------------------------------------------------

#ifndef ConfigFrmH
#define ConfigFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TConfigForm : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TEdit *EditExternalStorage;
	TButton *ButtonOk;
	TButton *ButtonCancel;
	TEdit *EditReports;
	TLabel *Label2;
	TGroupBox *GroupBoxNetwork;
	TLabel *Label3;
	TLabel *Label4;
	TEdit *EditProxyServer;
	TLabel *Label5;
	TEdit *EditProxyPort;
	TUpDown *UpDownProxyPort;
	TMemo *MemoNoProxy;
	TSpeedButton *SpeedButtonExternalStorage;
	TSpeedButton *SpeedButtonReports;
	void __fastcall SpeedButtonExternalStorageClick(TObject *Sender);
	void __fastcall SpeedButtonReportsClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TConfigForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TConfigForm *ConfigForm;
//---------------------------------------------------------------------------
#endif
