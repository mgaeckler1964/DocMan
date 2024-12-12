//---------------------------------------------------------------------------

#ifndef DiffFrmH
#define DiffFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>

#include "ActionManager.h"
//---------------------------------------------------------------------------
class THE_FILE_BASE;
class TDiffForm : public TForm
{
__published:	// IDE-managed Components
	TMemo *MemoDiff;
	TPanel *Panel1;
	TSpeedButton *SpeedButtonPrev;
	TSpeedButton *SpeedButtonNext;
	void __fastcall SpeedButtonPrevClick(TObject *Sender);
	void __fastcall SpeedButtonNextClick(TObject *Sender);
private:	// User declarations
	void FindChange( int direction );
public:		// User declarations
	__fastcall TDiffForm(TComponent* Owner);
	void ShowDiff( THE_FILE_BASE *theFile, int firstVersion=0, int secondVersion=-1 );
};
//---------------------------------------------------------------------------
class ACTION_DIFF : public ACTION_BASE_PROPERTIES
{ 
	public:
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
};
//---------------------------------------------------------------------------
extern PACKAGE TDiffForm *DiffForm;
extern ACTION_DIFF theDiffAction;
//---------------------------------------------------------------------------
#endif
