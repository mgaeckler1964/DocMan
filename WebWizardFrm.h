//---------------------------------------------------------------------------

#ifndef WebWizardFrmH
#define WebWizardFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TWebWizardForm : public TForm
{
__published:	// IDE-managed Components
	TPageControl *PageControl1;
	TTabSheet *TabSheetLayout;
	TComboBox *ComboBoxTopLeft;
	TComboBox *ComboBoxTop;
	TComboBox *ComboBoxLeft;
	TComboBox *ComboBoxBottomLeft;
	TComboBox *ComboBoxBottom;
	TComboBox *ComboBoxBottomRight;
	TComboBox *ComboBoxTopRight;
	TComboBox *ComboBoxRight;
	TPaintBox *PaintBoxPreview;
	TTabSheet *TabSheetSize;
	TEdit *EditTop;
	TEdit *EditBottom;
	TEdit *EditLeft;
	TEdit *EditRight;
	TComboBox *ComboBoxContent;
	TEdit *EditContent;
	TPanel *Panel1;
	TButton *ButtonOK;
	TButton *ButtonCancel;
	TTabSheet *TabSheetColor;
	TComboBox *ComboBoxColorTopLeft;
	TComboBox *ComboBoxColorTop;
	TComboBox *ComboBoxColorTopRight;
	TComboBox *ComboBoxColorLeft;
	TComboBox *ComboBoxColorContent;
	TComboBox *ComboBoxColorRight;
	TComboBox *ComboBoxColorBottomLeft;
	TComboBox *ComboBoxColorBottom;
	TComboBox *ComboBoxColorBottomRight;
	TTabSheet *TabSheetContent;
	TComboBox *ComboBoxContentTopLeft;
	TComboBox *ComboBoxContentTop;
	TComboBox *ComboBoxContentLeft;
	TComboBox *ComboBoxContentBottomLeft;
	TComboBox *ComboBoxContentBottom;
	TComboBox *ComboBoxContentBottomRight;
	TComboBox *ComboBoxContentTopRight;
	TComboBox *ComboBoxContentRight;
	TButton *ButtonEdit;
	TCheckBox *CheckBoxNoExample;
	void __fastcall EnableDisableLayout(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall PaintBoxPreviewPaint(TObject *Sender);
	void __fastcall ButtonEditClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TWebWizardForm(TComponent* Owner);
	bool dataLoaded;
	gak::xml::Document	*configDocument;
};
//---------------------------------------------------------------------------
extern PACKAGE TWebWizardForm *WebWizardForm;
//---------------------------------------------------------------------------
#endif

