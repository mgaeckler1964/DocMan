//---------------------------------------------------------------------------

#ifndef MultiActionFrmH
#define MultiActionFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

#include "ItemManager.h"

//---------------------------------------------------------------------------
class TMultiActionForm : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TListBox *ListBoxItems;
	TButton *ButtonCopy;
	TButton *Button2;
	TButton *ButtonMove;
	TButton *ButtonDelete;
	void __fastcall DoAction(TObject *Sender);
	void __fastcall ListBoxItemsClick(TObject *Sender);
private:	// User declarations
	PTR_ITEM theItem;
public:		// User declarations
	__fastcall TMultiActionForm(TComponent* Owner);
	void setItem( PTR_ITEM theItem );
};
//---------------------------------------------------------------------------
extern PACKAGE TMultiActionForm *MultiActionForm;
//---------------------------------------------------------------------------
#endif
