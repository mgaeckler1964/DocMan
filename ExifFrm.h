//---------------------------------------------------------------------------

#ifndef ExifFrmH
#define ExifFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include "ItemManager.h"
#include <Grids.hpp>

//---------------------------------------------------------------------------
class TExifForm : public TForm
{
__published:	// IDE-managed Components
	TStringGrid *StringGrid;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
	PTR_ITEM	theItem;
	int			theVersion;
public:		// User declarations
	__fastcall TExifForm(TComponent* Owner);

	void setItem( const PTR_ITEM &selectedItem, int selectedVersion )
	{
		theItem = selectedItem;
		theVersion = selectedVersion;
	}
};
//---------------------------------------------------------------------------
extern PACKAGE TExifForm *ExifForm;
//---------------------------------------------------------------------------
#endif
