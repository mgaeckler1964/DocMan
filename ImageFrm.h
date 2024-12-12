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
