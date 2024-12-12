//---------------------------------------------------------------------------

#ifndef CryptoKeysFrmH
#define CryptoKeysFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include <gak/cryptoShared.h>

#include "ItemManager.h"

//---------------------------------------------------------------------------
class TCryptoKeysForm : public TForm
{
__published:	// IDE-managed Components
	TListBox *ListBoxSelected;
	TListBox *ListBoxAvailable;
	TButton *ButtonAdd;
	TButton *ButtonRemove;
	TButton *ButtonOK;
	TButton *ButtonCancel;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ButtonAddClick(TObject *Sender);
	void __fastcall ButtonOKClick(TObject *Sender);
	void __fastcall ButtonRemoveClick(TObject *Sender);
private:	// User declarations
	gak::CryptoShared	crypto;
	PTR_ITEM			theItem;
	STRING				fileName;
public:		// User declarations
	__fastcall TCryptoKeysForm(TComponent* Owner);
	void setItem( const PTR_ITEM &newItem )
	{
		theItem = newItem;
	}
};
//---------------------------------------------------------------------------
extern PACKAGE TCryptoKeysForm *CryptoKeysForm;
//---------------------------------------------------------------------------
#endif

