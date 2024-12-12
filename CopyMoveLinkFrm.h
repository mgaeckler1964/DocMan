//---------------------------------------------------------------------------

#ifndef CopyMoveLinkFrmH
#define CopyMoveLinkFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class THE_ITEM;
class TCopyMoveLinkForm : public TForm
{
__published:	// IDE-managed Components
	TLabel *Destination;
	TEdit *EditTarget;
	TButton *ButtonBrowse;
	TLabel *Label1;
	TEdit *EditName;
	TButton *ButtonOK;
	TButton *ButtonCancel;
	void __fastcall ButtonBrowseClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
	PTR_ITEM	theDestination;

public:		// User declarations
	void setItem( const STRING &name, const PTR_ITEM &theDestination )
	{
		this->theDestination = theDestination;
		EditName->Text = (const char *)name;

		if( theDestination )
		{
			EditTarget->Text = (const char *)(*theDestination).getPath();
		}
		else
		{
			EditTarget->Text = "";
		}
	}
	__fastcall TCopyMoveLinkForm(TComponent* Owner);
	PTR_ITEM getDestination( void ) const
	{
		return theDestination;
	}
};
//---------------------------------------------------------------------------
extern PACKAGE TCopyMoveLinkForm *CopyMoveLinkForm;
//---------------------------------------------------------------------------
#endif
