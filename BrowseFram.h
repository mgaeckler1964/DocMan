//---------------------------------------------------------------------------


#ifndef BrowseFramH
#define BrowseFramH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <Menus.hpp>
#include <ImgList.hpp>

#include <gak/array.h>

#include "ItemManager.h"

//---------------------------------------------------------------------------
class THE_ITEM;
class ACTION_BASE;
class TBrowserFrame : public TFrame
{
__published:	// IDE-managed Components
	TDrawGrid *DrawGridContent;
	TPopupMenu *PopupMenuActions;
	TMenuItem *Action;
	TImageList *ImageListSort;
	void __fastcall DrawGridContentDrawCell(TObject *Sender, int ACol,
          int ARow, TRect &Rect, TGridDrawState State);
	void __fastcall DrawGridContentMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall DrawGridContentMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall ActionClick(TObject *Sender);
	void __fastcall FrameResize(TObject *Sender);
	void __fastcall DrawGridContentSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
protected:	// User declarations
	size_t					colCount, numStrechingCols, fixedWidth;
	int						*colWidths;
	PTR_ITEM				currentItem;
	PTR_ITEM				selectedItem;
	Array<ACTION_BASE *>	popupActions;
	virtual int				getVisibleItemIndex( int ARow );

public:		// User declarations
	void selectItem( int selectID );
	virtual void fillContents( PTR_ITEM currentItem, int selectID );
	__fastcall TBrowserFrame(TComponent* Owner );
};
//---------------------------------------------------------------------------
extern PACKAGE TBrowserFrame *BrowserFrame;
//---------------------------------------------------------------------------
#endif
