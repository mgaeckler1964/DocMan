//---------------------------------------------------------------------------

#ifndef CreateVersionFrmH
#define CreateVersionFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>

#include "ActionManager.h"
//---------------------------------------------------------------------------
class TVersionCreateForm : public TForm
{
__published:	// IDE-managed Components
	TButton *ButtonOK;
	TButton *ButtonCancel;
	TLabel *LabelFile;
	TEdit *EditFilePath;
	TButton *ButtonBrowseFile;
	TOpenDialog *OpenDialog;
	TLabel *LabelDescription;
	TMemo *MemoDescription;
	void __fastcall ButtonBrowseFileClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TVersionCreateForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
class ACTION_ADD_VERSION : public ACTION_BASE_VERSIONS
{
	public:
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
	virtual const char *getLabel( void ) const;
	virtual bool acceptItem( THE_ITEM *theItem );
};
//---------------------------------------------------------------------------
extern PACKAGE TVersionCreateForm *VersionCreateForm;
extern ACTION_ADD_VERSION theCreateVersionAction;
//---------------------------------------------------------------------------
#endif
