//---------------------------------------------------------------------------

#ifndef PermApplyDownFrmH
#define PermApplyDownFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <ImgList.hpp>

#include "ItemManager.h"
//---------------------------------------------------------------------------

class TPermApplyDownForm : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TButton *ButtonOK;
	TButton *ButtonCancel;
	TRadioButton *RadioButtonMerge;
	TRadioButton *RadioButtonOverwrite;
	TStringGrid *StringGrid;
	TImageList *ImageList;
	TRadioButton *RadioButtonDelete;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall StringGridDrawCell(TObject *Sender, int ACol, int ARow,
		  TRect &Rect, TGridDrawState State);
	void __fastcall StringGridMouseUp(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y);
private:	// User declarations
	THE_ITEM		*theItem;
public:		// User declarations
	ITEM_PERMLIST	acl;
	__fastcall TPermApplyDownForm(TComponent* Owner);
	void setItem( THE_ITEM *theItem )
	{
		this->theItem = theItem;
	}
};
//---------------------------------------------------------------------------
extern PACKAGE TPermApplyDownForm *PermApplyDownForm;
//---------------------------------------------------------------------------
#endif
