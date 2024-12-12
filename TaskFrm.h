//---------------------------------------------------------------------------

#ifndef TaskFrmH
#define TaskFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ItemCreateFrm.h"
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TTaskForm : public TItemCreateForm
{
__published:	// Von der IDE verwaltete Komponenten
	TEdit *EditCustomerRef;
	TLabel *Label2;
	TLabel *Label3;
	TComboBox *ComboBoxTaskType;
	TLabel *Label4;
	TComboBox *ComboBoxTaskStatus;
	TLabel *Label5;
	TComboBox *ComboBoxModule;
	TComboBox *ComboBoxFunction;
	TLabel *Label6;
	TLabel *Label7;
	TEdit *EditRelease;
	TLabel *Label8;
	TEdit *EditPriority;
	TUpDown *UpDownPriority;
	TLabel *Label9;
	TComboBox *ComboBoxAssignedTo;
	TLabel *Label10;
	TEdit *EditEstEffort;
	TLabel *Label11;
	TEdit *EditActEffort;
	TMemo *MemoInternalRemarks;
	TMemo *MemoExternalRemarks;
	TLabel *Label13;
	TLabel *LabelCompleted;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// Anwender-Deklarationen
public:		// Anwender-Deklarationen
	TDateTime	completed;
	__fastcall TTaskForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TTaskForm *TaskForm;
//---------------------------------------------------------------------------
#endif
