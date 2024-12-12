//---------------------------------------------------------------------------

#ifndef SourceFolderCreateFrmH
#define SourceFolderCreateFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "SyncFolderCreateFrm.h"
#include <Buttons.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TSourceFolderCreateForm : public TSyncFolderCreateForm
{
__published:	// Von der IDE verwaltete Komponenten
	TEdit *EditPatch;
	TEdit *EditMinor;
	TEdit *EditMajor;
	TLabel *Label3;
	TUpDown *UpDownMajor;
	TUpDown *UpDownMinor;
	TUpDown *UpDownPatch;
	void __fastcall FormShow(TObject *Sender);
private:	// Anwender-Deklarationen
public:		// Anwender-Deklarationen
	__fastcall TSourceFolderCreateForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSourceFolderCreateForm *SourceFolderCreateForm;
//---------------------------------------------------------------------------
#endif
