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
