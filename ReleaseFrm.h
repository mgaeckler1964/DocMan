//---------------------------------------------------------------------------

#ifndef ReleaseFrmH
#define ReleaseFrmH
//---------------------------------------------------------------------------
#include <stdio.h>

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ItemCreateFrm.h"
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TReleaseForm : public TItemCreateForm
{
__published:	// IDE-managed Components
	TEdit *EditPatch;
	TEdit *EditMinor;
	TEdit *EditMajor;
	TLabel *Label2;
	TUpDown *UpDownMajor;
	TUpDown *UpDownMinor;
	TUpDown *UpDownPatch;
	TCheckBox *CheckBoxCreateLinks;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall UpDownClick(TObject *Sender, TUDBtnType Button);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
	bool createBranchFlag;

	void createName( void )
	{
		int major = UpDownMajor->Position;
		int minor = UpDownMinor->Position;
		int patch = UpDownPatch->Position;

		char buffer[32];

		sprintf( buffer, "%d.%d.%d", major, minor, patch );
		EditName->Text = buffer;
	}
public:		// User declarations
	void createBranch( THE_ITEM *theItem )
	{
		setItem( theItem );
		createBranchFlag = true;
	}
	void updateRelease( THE_ITEM *theItem )
	{
		setItem( theItem );
		createBranchFlag = false;
	}
	__fastcall TReleaseForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TReleaseForm *ReleaseForm;
//---------------------------------------------------------------------------
#endif
