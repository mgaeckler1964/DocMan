//---------------------------------------------------------------------------

#ifndef EditXmlFileFrmH
#define EditXmlFileFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "EditFrm.h"
#include <ExtCtrls.hpp>
#include "xmlEditFram.h"

#include <gak/ChangeManager.h>

//---------------------------------------------------------------------------
class TXmlViewerForm;

class TEditXmlFileForm : public TEditFileForm
{
__published:	// Von der IDE verwaltete Komponenten
	TxmlEditorFrame *xmlEditorFrame;
	TButton *ButtonTransform;
	TButton *ButtonCheck;
	TButton *ButtonRenderer;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall ButtonCheckClick(TObject *Sender);
	void __fastcall ButtonTransformClick(TObject *Sender);
	void __fastcall xmlEditorFrameAttributeGridContextPopup(TObject *Sender,
		  TPoint &MousePos, bool &Handled);
	void __fastcall SelectPopupClick(TObject *Sender);
	void __fastcall ButtonRendererClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall Button1Click(TObject *Sender);

private:	// Anwender-Deklarationen
	PTR_FILE			theFile;
	gak::ChangeManager	theManager;
	TXmlViewerForm		*XmlViewerForm;
	gak::css::Rules		editorRules;
	gak::xml::Document	*theDocument;

	void styleChangedCB( const STRING &styleSheetFile, const STRING &stylesheetType );
public:		// Anwender-Deklarationen
	__fastcall TEditXmlFileForm(TComponent* Owner);
	virtual void setText( const PTR_FILE &theFile, const STRING &data );
	void setDocument( gak::xml::Document *theDocument );
	virtual STRING getText( void );
};
//---------------------------------------------------------------------------
extern PACKAGE TEditXmlFileForm *EditXmlFileForm;
//---------------------------------------------------------------------------
#endif
