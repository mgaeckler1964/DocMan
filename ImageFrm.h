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

#ifndef ImageFrmH
#define ImageFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>

#include <gak/array.h>

#include "ItemManager.h"
#include "File.h"

//---------------------------------------------------------------------------
struct ThumbNailViewer
{
	int		itemID;
	TImage	*Image;
	TLabel	*Label;
};
typedef Array<ThumbNailViewer>	ThumbNailViewers;

//---------------------------------------------------------------------------
class TImageBrowserForm : public TForm
{
__published:	// IDE-managed Components
	TScrollBox *ScrollBox;
	TPanel *Panel1;
	TTrackBar *TrackBar;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall ScrollBoxResize(TObject *Sender);
	void __fastcall TrackBarChange(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
	Array<ThumbNailViewer>	thumbNails;

	int maxSize;
	int lastRow, lastCol;

	PTR_ITEM	parentItem;

	void __fastcall ItemClick( TObject *Sender );

public:		// User declarations
	__fastcall TImageBrowserForm(TComponent* Owner);
	void setParentItem( const PTR_ITEM &theParent )
	{
		parentItem = theParent;
	}
	bool addImage( int itemID, const STRING &file, TGraphic *graphic=NULL );
	bool addImage( const PTR_FILE &theFile );
};
//---------------------------------------------------------------------------
extern PACKAGE TImageBrowserForm *ImageBrowserForm;
//---------------------------------------------------------------------------
#endif
