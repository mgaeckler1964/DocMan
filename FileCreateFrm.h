//---------------------------------------------------------------------------

#ifndef FileCreateFrmH
#define FileCreateFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include "ItemCreateFrm.h"
#include "File.h"

//---------------------------------------------------------------------------
class TFileCreateForm : public TItemCreateForm
{
__published:	// IDE-managed Components
	TLabel *Label2;
	TEdit *EditFilePath;
	TButton *ButtonBrowse;
	TOpenDialog *OpenDialog;
	TCheckBox *CheckBoxReserve;
	TLabel *LabelTemplate;
	TComboBox *ComboBoxTemplate;
	void __fastcall ButtonBrowseClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
	Array<PTR_FILE>	templates;
public:		// User declarations
	__fastcall TFileCreateForm(TComponent* Owner);
	PTR_FILE	getTemplate( void )
	{
		return templates[ComboBoxTemplate->ItemIndex];
	}
};
//---------------------------------------------------------------------------
extern PACKAGE TFileCreateForm *FileCreateForm;
//---------------------------------------------------------------------------
#endif
