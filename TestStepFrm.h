//---------------------------------------------------------------------------

#ifndef TestStepFrmH
#define TestStepFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ItemCreateFrm.h"
#include "ItemManager.h"
#include "DocManMain.h"
//---------------------------------------------------------------------------
class THE_TEST_STEP : public THE_ITEM
{
	STRING	expectedResult;

	public:
	THE_TEST_STEP(
		int id, const FACTORY_BASE *theFactory,
		int flags=ITEM_IS_MOVEABLE|ITEM_IS_COPYABLE
	)
	: THE_ITEM(flags, id, theFactory )
	{}
	void setData(
		const PTR_ITEM &parent,
		const STRING &name, const STRING &description,
		const STRING &expectedResult, int assignedTo
	)
	{
		THE_ITEM::setData( parent, name, description );
		this->expectedResult = expectedResult;
		setAssignedTo( assignedTo );
		setOrder( parent->getChildCount() + 1 );
	}
	virtual void loadFields( TQuery *query );
	virtual void updateDatabase( void );
	virtual void open( void );
	virtual TGraphic *getItemPicture( void ) const;

	const STRING &getExpectedResult( void ) const
	{
		return expectedResult;
	}
};
typedef PTR_TEMPLATE<THE_TEST_STEP> PTR_TEST_STEP;
//---------------------------------------------------------------------------
class TTestStepForm : public TItemCreateForm
{
__published:	// IDE-managed Components
	TMemo *MemoExpected;
	TLabel *Label2;
	TComboBox *ComboBoxAssignedTo;
	TLabel *Label9;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
	__fastcall TTestStepForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TTestStepForm *TestStepForm;
//---------------------------------------------------------------------------
#endif

