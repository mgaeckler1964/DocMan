//---------------------------------------------------------------------------

#ifndef EditFrmH
#define EditFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

#include <gak/string.h>

#include "File.h"

//---------------------------------------------------------------------------
class TEditFileForm : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
	TPanel *Panel1;
	TButton *Button1;
	TButton *Button2;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
private:	// Anwender-Deklarationen
	PTR_FILE	theFile;
	STRING		destFile;
	bool		createVersion;
protected:
	void setTitle( const STRING &fileName )
	{
		AnsiString	nCaption = "Edit ";
		nCaption += (const char *)fileName;

		Caption = nCaption;
	}

public:		// Anwender-Deklarationen
	__fastcall TEditFileForm(TComponent* Owner);

	void setDestination( const PTR_FILE &theFile, const STRING &destFile, bool createVersion )
	{
		this->theFile = theFile;
		this->destFile = destFile;
		this->createVersion = createVersion;

		setTitle( theFile->getPath() );
	}
	virtual void setText( const PTR_FILE &theFile, const STRING &data ) = 0;
	virtual STRING getText( void ) = 0;
};
//---------------------------------------------------------------------------
extern PACKAGE TEditFileForm *EditFileForm;
//---------------------------------------------------------------------------
#endif
