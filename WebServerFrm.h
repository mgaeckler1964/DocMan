//---------------------------------------------------------------------------

#ifndef WebServerFrmH
#define WebServerFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include "ItemManager.h"

//---------------------------------------------------------------------------
class TWebServerForm : public TForm
{
__published:	// IDE-managed Components
	TButton *ButtonStart;
	TButton *ButtonStop;
	TEdit *EditItemPath;
	TLabel *Label1;
	TEdit *EditPort;
	TLabel *LabelLastUrl;
	TLabel *LabelStatus;
	void __fastcall ButtonStartClick(TObject *Sender);
	void __fastcall ButtonStopClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
	PTR_ITEM	theItem;
public:		// User declarations
	__fastcall TWebServerForm(TComponent* Owner);
	void setItem( const PTR_ITEM &newItem )
	{
		theItem = newItem;
	}
};
//---------------------------------------------------------------------------
extern PACKAGE TWebServerForm *WebServerForm;
//---------------------------------------------------------------------------
#endif

