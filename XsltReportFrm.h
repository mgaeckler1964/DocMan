//---------------------------------------------------------------------------

#ifndef XsltReportFrmH
#define XsltReportFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TXsltReportForm : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
	TLabel *Label1;
	TComboBox *ComboBoxStylesheet;
	TButton *Button1;
	TButton *Button2;
private:	// Anwender-Deklarationen
public:		// Anwender-Deklarationen
	__fastcall TXsltReportForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TXsltReportForm *XsltReportForm;
//---------------------------------------------------------------------------
void initXsltVolume( void );
//---------------------------------------------------------------------------
#endif
