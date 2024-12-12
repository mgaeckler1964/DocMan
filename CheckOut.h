//---------------------------------------------------------------------------

#ifndef CheckOutH
#define CheckOutH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBTables.hpp>

#include <gak/array.h>

//---------------------------------------------------------------------------
class TCheckOutForm : public TForm
{
__published:	// IDE-managed Components
	TButton *ButtonOK;
	TButton *ButtonCancel;
	TLabel *Label1;
	TComboBox *ComboBoxTasks;
	TQuery *QueryOpenTasks;
	TCheckBox *CheckBoxIncludeExtensions;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ButtonOKClick(TObject *Sender);
private:	// User declarations
	gak::ArrayOfInts	ids;
	int		   	lastTaskID;
public:		// User declarations
	__fastcall TCheckOutForm(TComponent* Owner);
	int getSelectedTaskID( void )
	{
		if( ComboBoxTasks->ItemIndex >= 0 )
			return ids[ComboBoxTasks->ItemIndex];

		return 0;
	}
	void setLastTask( taskID )
	{
		lastTaskID = taskID;
	}
};
//---------------------------------------------------------------------------
extern PACKAGE TCheckOutForm *CheckOutForm;
//---------------------------------------------------------------------------
#endif
