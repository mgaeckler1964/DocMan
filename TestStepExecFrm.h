//---------------------------------------------------------------------------

#ifndef TestStepExecFrmH
#define TestStepExecFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TestStepFrm.h"
//---------------------------------------------------------------------------
class TTestStepExecForm : public TTestStepForm
{
__published:	// IDE-managed Components
	TLabel *Actual;
	TMemo *MemoActualResult;
	TButton *ButtonFailed;
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TTestStepExecForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TTestStepExecForm *TestStepExecForm;
//---------------------------------------------------------------------------
class THE_TEST_EXEC_STEP : public THE_TEST_STEP
{
	STRING	actualResult;
	int		testStatus;

	virtual TGraphic *getItemPicture( void ) const;

	public:
	THE_TEST_EXEC_STEP(
		int id, const FACTORY_BASE *theFactory,
		int flags=ITEM_IS_MOVEABLE|ITEM_IS_COPYABLE
	)
	: THE_TEST_STEP( id, theFactory, flags )
	{
		testStatus = 0;
	}
	void setData(
		const PTR_ITEM &parent,
		const STRING &name, const STRING &description,
		const STRING &expectedResult, int assignedTo,
		int order, int copyID
	)
	{
		THE_TEST_STEP::setData(
			parent, name, description, expectedResult, assignedTo
		);
		setOrder( order );
		setCopyID( copyID );
	}
	virtual void loadFields( TQuery *query );
	virtual void updateDatabase( void );
	virtual void open( void );

	const STRING &getActualResult( void ) const
	{
		return actualResult;
	}
	const char *getStatus( void ) const
	{
		if( testStatus < 0 )
			return "failed";
		else if( testStatus == 0 )
			return "untested";
		else
			return "success";
	}
	int getTestStatus( void ) const
	{
		return testStatus;
	}
};
typedef PTR_TEMPLATE<THE_TEST_EXEC_STEP> PTR_TEST_EXEC_STEP;
//---------------------------------------------------------------------------
#endif

