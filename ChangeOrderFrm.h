//---------------------------------------------------------------------------

#ifndef ChangeOrderFrmH
#define ChangeOrderFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TChangeOrderForm : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TListBox *ListBoxItems;
	TButton *Button1;
	TButton *Button2;
	TButton *Button3;
	void __fastcall ListBoxItemsEndDrag(TObject *Sender, TObject *Target, int X,
          int Y);
	void __fastcall ListBoxItemsDragOver(TObject *Sender, TObject *Source, int X,
          int Y, TDragState State, bool &Accept);
private:	// User declarations
public:		// User declarations
	__fastcall TChangeOrderForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TChangeOrderForm *ChangeOrderForm;
//---------------------------------------------------------------------------
#endif
