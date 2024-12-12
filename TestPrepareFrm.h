//---------------------------------------------------------------------------

#ifndef TestPrepareFrmH
#define TestPrepareFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include "TaskList.h"

//---------------------------------------------------------------------------
class THE_TEST_EXEC_FOLDER : public THE_ITEM
{
	using THE_ITEM::sort;

	int		testStatus, taskListID;

	static int itemCompare( const PTR_ITEM &e1, const PTR_ITEM &e2, int crit );

	virtual COL_TITLE	*getColumnTitles( void ) const;
	virtual int getColCount( void ) const;
	virtual int *getColWidth( void ) const;
	virtual void sort( void );
	virtual STRING drawCell( int col, int row, TCanvas *canvas, TRect &Rect );

	virtual TGraphic *getItemPicture( void ) const;


	public:
	THE_TEST_EXEC_FOLDER(
		int id, const FACTORY_BASE *theFactory,
		int flags=ITEM_IS_CONTAINER|ITEM_IS_BROWSEABLE|ITEM_IS_MOVEABLE|ITEM_IS_COPYABLE
	)
	: THE_ITEM( flags, id, theFactory )
	{
		taskListID = testStatus = 0;
	}
	void setData(
		const PTR_ITEM &parent, const STRING &name, const STRING &description,
		int assignedTo, int taskListID
	)
	{
		THE_ITEM::setData( parent, name, description );
		setAssignedTo( assignedTo );
		this->taskListID = taskListID;
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
	void setTestStatus( int newStatus )
	{
		testStatus = newStatus;
	}
	int getTaskListID( void ) const
	{
		return taskListID;
	}
	virtual void loadFields( TQuery *query );
	virtual void updateDatabase( void );
	virtual void open( void );
};
typedef PTR_TEMPLATE<THE_TEST_EXEC_FOLDER> PTR_TEST_EXEC_FOLDER;

//---------------------------------------------------------------------------
class TTestPrepareForm : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TComboBox *ComboBoxAssignedTo;
	TLabel *Label2;
	TEdit *EditTaskList;
	TButton *ButtonBrowse;
	TButton *ButtonOK;
	TButton *ButtonCancel;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ButtonBrowseClick(TObject *Sender);
private:	// User declarations
	PTR_TASKLIST	theTaskList;
public:		// User declarations
	__fastcall TTestPrepareForm(TComponent* Owner);
	int getTaskListID( void ) const
	{
		return theTaskList ? (*theTaskList).getID() : 0;
	}
	int getAssignedTo( void ) const
	{
		return gak::vcl::getUserByIndex(ComboBoxAssignedTo->ItemIndex);
	}
	void setAssignedTo( int userID )
	{
		ComboBoxAssignedTo->Tag = userID;
	}
	void setTaskList( const PTR_TASKLIST &taskList )
	{
		theTaskList = taskList;
		if( taskList )
			EditTaskList->Text = (const char *)(*taskList).getPath();
		else
			EditTaskList->Text = "";
	}
};
//---------------------------------------------------------------------------
extern PACKAGE TTestPrepareForm *TestPrepareForm;
//---------------------------------------------------------------------------
#endif

