//---------------------------------------------------------------------------

#ifndef CheckInFrmH
#define CheckInFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>

#include "CreateVersionFrm.h"
#include "ActionManager.h"
//---------------------------------------------------------------------------
class TCheckInForm : public TVersionCreateForm
{
__published:	// IDE-managed Components
	TCheckBox *CheckBoxAddVersion;
	TCheckBox *CheckBoxBranch;
	void __fastcall CheckBoxAddVersionClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TCheckInForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
class ACTION_CHECK_IN : public ACTION_BASE_CHECK
{
	public:
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
};
//---------------------------------------------------------------------------
extern PACKAGE TCheckInForm *CheckInForm;
extern ACTION_CHECK_IN 	theCheckInAction;
//---------------------------------------------------------------------------
#endif
