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

#ifndef TargetBrowseFrmH
#define TargetBrowseFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ImgList.hpp>
//---------------------------------------------------------------------------

#include "ItemManager.h"

class TTargetBrowseForm : public TForm
{
__published:	// IDE-managed Components
	TTreeView *TreeView;
	TPanel *Panel1;
	TButton *Button1;
	TButton *Button2;
	TImageList *ImageList;
	void __fastcall TreeViewExpanding(TObject *Sender, TTreeNode *Node,
		  bool &AllowExpansion);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
	bool			all;
	enum itemTypes	selectableType;

	TTreeNode *findItem( TTreeNode *parent, int itemId );
	void collectItems( TTreeNode *node );
	int getImageIndex( const THE_ITEM *theContentItem );
	void setItem( const PTR_ITEM &startItem, bool all, enum itemTypes selectableType );
public:		// User declarations

	void setItem( const PTR_ITEM &startItem, bool all )
	{
		setItem( startItem, all, TYPE_ROOT );
	}
	void setItem( const PTR_ITEM &startItem, enum itemTypes selectableType )
	{
		setItem( startItem, false, selectableType );
	}
	__fastcall TTargetBrowseForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TTargetBrowseForm *TargetBrowseForm;
//---------------------------------------------------------------------------
#endif
