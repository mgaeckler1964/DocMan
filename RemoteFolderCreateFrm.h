//---------------------------------------------------------------------------

#ifndef RemoteFolderCreateFrmH
#define RemoteFolderCreateFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "SyncFolderCreateFrm.h"
#include <Buttons.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TRemoteFolderCreateForm : public TSyncFolderCreateForm
{
__published:	// Von der IDE verwaltete Komponenten
	TLabel *Label3;
	TComboBox *ComboBoxRemoteServer;
	TEdit *EditRemoteFolderID;
	void __fastcall FormShow(TObject *Sender);
private:	// Anwender-Deklarationen
public:		// Anwender-Deklarationen
	__fastcall TRemoteFolderCreateForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TRemoteFolderCreateForm *RemoteFolderCreateForm;
//---------------------------------------------------------------------------
#endif
