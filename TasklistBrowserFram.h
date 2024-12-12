//---------------------------------------------------------------------------

#ifndef TasklistBrowserFramH
#define TasklistBrowserFramH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BrowseFram.h"
#include <Grids.hpp>
#include <Menus.hpp>
#include <ExtCtrls.hpp>
#include <ImgList.hpp>

//---------------------------------------------------------------------------

class TTasklistBrowserFrame : public TBrowserFrame
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TCheckBox *CheckBoxOwner;
	TComboBox *ComboBoxStatusFilter;
	TComboBox *ComboBoxTypeFilter;
	void __fastcall FilterChange(TObject *Sender);
	void __fastcall DrawGridContentMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
private:	// User declarations
	gak::ArrayOfInts	visibleItems;

	void refreshVisibleItems( void );
	virtual int		getVisibleItemIndex( int ARow );

public:		// User declarations
	__fastcall TTasklistBrowserFrame(TComponent* Owner);
	virtual void fillContents( PTR_ITEM currentItem, int selectID );
};
//---------------------------------------------------------------------------
extern PACKAGE TTasklistBrowserFrame *TasklistBrowserFrame;
//---------------------------------------------------------------------------
#endif
