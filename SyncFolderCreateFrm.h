//---------------------------------------------------------------------------

#ifndef SyncFolderCreateFrmH
#define SyncFolderCreateFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ItemCreateFrm.h"
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TSyncFolderCreateForm : public TItemCreateForm
{
__published:	// Von der IDE verwaltete Komponenten
	TLabel *Label2;
	TEdit *EditLocalPath;
	TSpeedButton *SpeedButton;
	void __fastcall SpeedButtonClick(TObject *Sender);
private:	// Anwender-Deklarationen
public:		// Anwender-Deklarationen
	__fastcall TSyncFolderCreateForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSyncFolderCreateForm *SyncFolderCreateForm;
//---------------------------------------------------------------------------
#endif
