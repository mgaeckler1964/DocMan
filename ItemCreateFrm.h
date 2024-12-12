//---------------------------------------------------------------------------

#ifndef ItemCreateFrmH
#define ItemCreateFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include "ItemManager.h"

//---------------------------------------------------------------------------
class TItemCreateForm : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TEdit *EditName;
	TButton *ButtonOk;
	TButton *ButtonCancel;
	TLabel *Label12;
	TMemo *MemoDescription;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ButtonOkClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

protected:	// User declarations
	PTR_ITEM	theItem, theParent;

public:		// User declarations
	void setItem( const PTR_ITEM &theItem )	// for properties page
	{
		this->theItem = theItem;
		theParent = NULL;
	}
	void setParent( const PTR_ITEM &theParent )	// for creation page
	{
		this->theParent = theParent;
		theItem = NULL;
	}
	const PTR_ITEM	&getParent( void ) const
	{
		return theParent;
	}
	__fastcall TItemCreateForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TItemCreateForm *ItemCreateForm;
//---------------------------------------------------------------------------
#endif
