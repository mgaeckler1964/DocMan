/*
		Project:		DocMan
		Module:			
		Description:	
		Author:			Martin Gäckler
		Address:		Hofmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2024 Martin Gäckler

		This program is free software: you can redistribute it and/or modify  
		it under the terms of the GNU General Public License as published by  
		the Free Software Foundation, version 3.

		You should have received a copy of the GNU General Public License 
		along with this program. If not, see <http://www.gnu.org/licenses/>.

		THIS SOFTWARE IS PROVIDED BY Martin Gäckler, Austria, Linz ``AS IS''
		AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
		TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
		PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR
		CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
		SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
		LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
		USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
		ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
		OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
		OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
		SUCH DAMAGE.
*/

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
