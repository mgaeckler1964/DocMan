//---------------------------------------------------------------------------

#ifndef EditPlainFileFrmH
#define EditPlainFileFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "EditFrm.h"
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TEditPlainFileForm : public TEditFileForm
{
__published:	// Von der IDE verwaltete Komponenten
	TMemo *MemoPlainFile;
private:	// Anwender-Deklarationen
public:		// Anwender-Deklarationen
	__fastcall TEditPlainFileForm(TComponent* Owner);
	virtual void setText( const PTR_FILE &theFile, const STRING &data );
	virtual STRING getText( void );
};
//---------------------------------------------------------------------------
extern PACKAGE TEditPlainFileForm *EditPlainFileForm;
//---------------------------------------------------------------------------
#endif
