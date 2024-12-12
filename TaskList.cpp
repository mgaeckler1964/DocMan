/*
		Project:
		Module:
		Description:
		Author:			Martin Gäckler
		Company:		CRESD GmbH
		Address:		Arminiusstr. 3a, D-8543 München
		Phone:			+49 - 89 - 65 30 95 63
		Telefax:		+49 - 89 - 65 30 95 64
		Copyright:		Siemens AG
		Date:			xx. xx. 20xx
		History:
*/

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include "ItemManager.h"
#include "Folder.h"
#include "ItemCreateFrm.h"
#include "TaskFrm.h"
#include "TaskList.h"
#include "TasklistBrowserFram.h"
#include "DocManDM.h"

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT-
#	ifdef __WIN32__
#		pragma option -a4
#		pragma option -pc
#	else
#		pragma option -po
#		pragma option -a2
#	endif
#endif

using namespace gak;

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

class FACTORY_TASKLIST : public FACTORY_BASE
{
	virtual int getItemType( void ) const;
	virtual bool acceptParent( const THE_ITEM *parent ) const;
	virtual PTR_ITEM createItem( int id ) const;
	virtual const char *getName( void ) const;
	virtual TItemCreateForm *getForm( void ) const;
	virtual PTR_ITEM createItemFromForm( const PTR_ITEM &parentId ) const;
};

class FACTORY_TASK : public FACTORY_BASE
{
	virtual int getItemType( void ) const;
	virtual bool acceptParent( const THE_ITEM *parent ) const;
	virtual PTR_ITEM createItem( int id ) const;
	virtual const char *getName( void ) const;
	virtual TItemCreateForm *getForm( void ) const;
	virtual PTR_ITEM createItemFromForm( const PTR_ITEM &parent ) const;
	virtual PTR_ITEM createItemFromTemplate(
		const PTR_ITEM &parent,
		const THE_ITEM *src,
		const STRING &newName=""
	) const;
};

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static FACTORY_TASKLIST	theListFactory;
static FACTORY_TASK		theTaskFactory;

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class static data --------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- prototypes ---------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module functions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class inlines ------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class constructors/destructors -------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class static functions ---------------------------------------- //
// --------------------------------------------------------------------- //

int THE_TASKLIST::itemCompare(
	const PTR_ITEM &e1, const PTR_ITEM &e2, int theSort
)
{
	PTR_TASK task1 = e1;
	PTR_TASK task2 = e2;

	int	compareResult = 0;

	if( task1 && task2 )
	{
		switch( abs( theSort ) )
		{
			case SORT_PRIORITY:
				compareResult = task1->getPriority() - task2->getPriority();
				break;

			case SORT_TASK_TYPE:
				compareResult = strcmpi(
					DocManDataModule->getType( task1->getTypeID() ),
					DocManDataModule->getType( task2->getTypeID() )
				);
				break;

			case SORT_MODULE:
				compareResult = strcmpi(
					task1->getModule(), task2->getModule()
				);
				break;

			case SORT_STATUS:
				compareResult = strcmpi(
					DocManDataModule->getStatus( task1->getStatusID() ),
					DocManDataModule->getStatus( task2->getStatusID() )
				);
				break;
		}
	}

	if( !compareResult )
		return THE_ITEM::itemCompare( e1, e2, theSort );

	return theSort < 0 ? -compareResult : compareResult;
}

// --------------------------------------------------------------------- //
// ----- class privates ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //

int FACTORY_TASKLIST::getItemType( void ) const
{
	return (int)TYPE_TASKLIST;
}

bool FACTORY_TASKLIST::acceptParent( const THE_ITEM *parent ) const
{
	const THE_FOLDER *theFolder = dynamic_cast<const THE_FOLDER *>(parent);
	return theFolder ? true : false;
}

PTR_ITEM FACTORY_TASKLIST::createItem( int id ) const
{
	return new THE_TASKLIST( id, this );
}

const char *FACTORY_TASKLIST::getName( void ) const
{
	return "Task List";
}

TItemCreateForm *FACTORY_TASKLIST::getForm( void ) const
{
	return ItemCreateForm;
}

PTR_ITEM FACTORY_TASKLIST::createItemFromForm( const PTR_ITEM &parent ) const
{
	PTR_ITEM newList = createItem( 0 );

	(*newList).setData(
		parent,
		ItemCreateForm->EditName->Text.c_str(),
		ItemCreateForm->MemoDescription->Text.c_str()
	);
	(*newList).updateDatabase();

	return newList;
}

// --------------------------------------------------------------------- //

int FACTORY_TASK::getItemType( void ) const
{
	return (int)TYPE_TASK;
}

bool FACTORY_TASK::acceptParent( const THE_ITEM *parent ) const
{
	const THE_TASKLIST *theList = dynamic_cast<const THE_TASKLIST *>(parent);
	return theList ? true : false;
}

PTR_ITEM FACTORY_TASK::createItem( int id ) const
{
	return new THE_TASK( id, this );
}

const char *FACTORY_TASK::getName( void ) const
{
	return "Task";
}

TItemCreateForm *FACTORY_TASK::getForm( void ) const
{
	return TaskForm;
}

PTR_ITEM FACTORY_TASK::createItemFromForm( const PTR_ITEM &parent ) const
{
	PTR_TASK	newTask = createItem( 0 );

	(*newTask).setData(
		parent,
		TaskForm->EditName->Text.c_str(),
		TaskForm->MemoDescription->Text.c_str(),
		TaskForm->EditCustomerRef->Text.c_str(),
		TaskForm->ComboBoxModule->Text.c_str(),
		TaskForm->ComboBoxFunction->Text.c_str(),
		TaskForm->EditRelease->Text.c_str(),
		TaskForm->MemoExternalRemarks->Text.c_str(),
		TaskForm->MemoInternalRemarks->Text.c_str(),
		TaskForm->UpDownPriority->Position,
		atof( TaskForm->EditEstEffort->Text.c_str() ),
		atof( TaskForm->EditActEffort->Text.c_str() ),
		TaskForm->ComboBoxTaskType->Tag,
		TaskForm->ComboBoxTaskStatus->Tag,
		TaskForm->ComboBoxAssignedTo->Tag,
		TaskForm->completed
	);
	(*newTask).updateDatabase();

	return newTask;
}

PTR_ITEM FACTORY_TASK::createItemFromTemplate(
	const PTR_ITEM &parent,
	const THE_ITEM *src,
	const STRING &newName
) const
{
	PTR_TASK	newTask;
	const THE_TASK *taskSrc = dynamic_cast<const THE_TASK *>(src);
	if( taskSrc )
	{
		newTask = createItem( 0 );
		if( newTask )
		{
			newTask->setData(
				parent,
				newName.isEmpty() ? taskSrc->getName() : newName,
				taskSrc->getDescription(),
				taskSrc->getCustomerRef(),
				taskSrc->getModule(),
				taskSrc->getBadFunction(),
				taskSrc->getRelease(),
				taskSrc->getExternalRemarks(),
				taskSrc->getInternalRemarks(),
				taskSrc->getPriority(),
				taskSrc->getEstEffort(),
				taskSrc->getActEffort(),
				taskSrc->getTypeID(),
				taskSrc->getStatusID(),
				taskSrc->getAssignedTo(),
				taskSrc->getCompleted()
			);
			newTask->setCopyID( src->getID() );
			newTask->updateDatabase();
		}
	}

	return newTask;
}

// --------------------------------------------------------------------- //

TGraphic *THE_TASKLIST::getItemPicture( void ) const
{
	static Graphics::TBitmap *thePic = NULL;

	if( !thePic )
	{
		thePic = new Graphics::TBitmap();
		thePic->LoadFromResourceName((unsigned int)HInstance, "TASKLIST_BITMAP");
	}
	return thePic;
}

STRING THE_TASKLIST::getSize( void )
{
	int		childCount = getChildCount();
	STRING	size = formatNumber( childCount );

	size += " Tasks";

	return size;
};

int THE_TASKLIST::getColCount( void ) const
{
	return 7;
}

int *THE_TASKLIST::getColWidth( void ) const
{
	static int colWidth[] =
	{
		50,
		0,
		120,
		50,
		120,
		80,
		80
	};
	return colWidth;
}

COL_TITLE *THE_TASKLIST::getColumnTitles( void ) const
{
	static COL_TITLE	colTitles[] =
	{
		{ "Type",			SORT_TYPE },
		{ "Name",			SORT_NAME },
		{ "Task Type",		SORT_TASK_TYPE },
		{ "Priority",		SORT_PRIORITY },
		{ "Module",			SORT_MODULE },
		{ "Assigned To",	SORT_ASSIGNED },
		{ "Status",			SORT_STATUS }
	};

	return colTitles;
}

void THE_TASKLIST::sort( void )
{
	getContent()->sort( itemCompare, getSortType() );
}

STRING THE_TASKLIST::drawCell( int col, int row, TCanvas *canvas, TRect &Rect )
{
	STRING text;

	if( col == 2 )
	{
		PTR_TASK	theTask = getContentItem( row );
		if( theTask )
			text = DocManDataModule->getType( theTask->getTypeID() );
	}
	else if( col == 3 )
	{
		PTR_TASK	theTask = getContentItem( row );
		if( theTask )
		{
			int prio = theTask->getPriority();
			text = formatNumber( prio );
		}
	}
	else if( col == 4 )
	{
		PTR_TASK	theTask = getContentItem( row );
		if( theTask )
			text = theTask->getModule();
	}
	else if( col == 5 )
	{
		PTR_TASK	theTask = getContentItem( row );
		if( theTask )
			text = theTask->getAssignedToUserName();
	}
	else if( col == 6 )
	{
		PTR_TASK	theTask = getContentItem( row );
		if( theTask )
			text = DocManDataModule->getStatus( (*theTask).getStatusID() );
	}
	else
		text = THE_ITEM::drawCell( col+1, row, canvas, Rect );

	return text;
}

TBrowserFrame *THE_TASKLIST::getFrame( void ) const
{
	static TTasklistBrowserFrame *theFrame = NULL;

	if( !theFrame )
		theFrame = new TTasklistBrowserFrame( NULL );

	return theFrame;
}

// --------------------------------------------------------------------- //

TGraphic *THE_TASK::getItemPicture( void ) const
{
	static Graphics::TBitmap *thePic = NULL;

	if( !thePic )
	{
		thePic = new Graphics::TBitmap();
		thePic->LoadFromResourceName((unsigned int)HInstance, "TASK_BITMAP");
	}
	return thePic;
}

void THE_TASK::loadFields( TQuery *query )
{
	THE_ITEM::loadFields( query );

	this->customerRef = query->FieldByName( "customer_ref" )->AsString.c_str();
	this->module = query->FieldByName( "module" )->AsString.c_str();
	this->badFunction = query->FieldByName( "bad_function" )->AsString.c_str();
	this->release = query->FieldByName( "release" )->AsString.c_str();
	this->externalRemarks = query->FieldByName( "external_remarks" )->AsString.c_str();
	this->internalRemarks = query->FieldByName( "internal_remarks" )->AsString.c_str();
	this->priority = query->FieldByName( "priority" )->AsInteger;
	this->estEffort = query->FieldByName( "estimated_effort" )->AsFloat;
	this->actEffort = query->FieldByName( "actual_effort" )->AsFloat;
	this->typeID = query->FieldByName( "type" )->AsInteger;
	this->statusID = query->FieldByName( "status" )->AsInteger;
	this->completed = query->FieldByName( "completed" )->AsDateTime;
}

void THE_TASK::updateDatabase( void )
{
	THE_ITEM::updateDatabase();

	std::auto_ptr<TQuery>	theQuery( new TQuery( NULL ) );
	theQuery->DatabaseName = "docManDB";

	theQuery->SQL->Add(
		"update item_tree "
		"set customer_ref = :customerRef, "
			"module = :module, "
			"bad_function = :badFunction, "
			"release = :release, "
			"external_remarks = :externalRemarks, "
			"internal_remarks = :internalRemarks, "
			"priority = :priority, "
			"estimated_effort = :estEffort, "
			"actual_effort = :actEffort, "
			"type = :typeID, "
			"status = :statusID, "
			"completed = :completed "
		"where id=:theId"
	);

	int col = 0;
	theQuery->Params->Items[col++]->AsString = (const char *)customerRef;
	theQuery->Params->Items[col++]->AsString = (const char *)module;
	theQuery->Params->Items[col++]->AsString = (const char *)badFunction;
	theQuery->Params->Items[col++]->AsString = (const char *)release;
	theQuery->Params->Items[col++]->AsString = (const char *)externalRemarks;
	theQuery->Params->Items[col++]->AsString = (const char *)internalRemarks;
	theQuery->Params->Items[col++]->AsInteger = priority;
	theQuery->Params->Items[col++]->AsFloat = estEffort;
	theQuery->Params->Items[col++]->AsFloat = actEffort;
	theQuery->Params->Items[col++]->AsInteger = typeID;
	theQuery->Params->Items[col++]->AsInteger = statusID;
	theQuery->Params->Items[col++]->AsDateTime = completed;
	theQuery->Params->Items[col++]->AsInteger = getID();

	theQuery->ExecSQL();
}

void THE_TASK::open( void )
{
	TaskForm->setItem( this );

	if( TaskForm->ShowModal() == mrOk )
	{
		setName(TaskForm->EditName->Text.c_str());
		setDescription(TaskForm->MemoDescription->Text.c_str());
		setAssignedTo( TaskForm->ComboBoxAssignedTo->Tag );

		customerRef = TaskForm->EditCustomerRef->Text.c_str();
		module = TaskForm->ComboBoxModule->Text.c_str();
		badFunction = TaskForm->ComboBoxFunction->Text.c_str();
		release = TaskForm->EditRelease->Text.c_str();
		externalRemarks = TaskForm->MemoExternalRemarks->Text.c_str();
		internalRemarks = TaskForm->MemoInternalRemarks->Text.c_str();
		priority = TaskForm->UpDownPriority->Position;
		estEffort = atof( TaskForm->EditEstEffort->Text.c_str() );
		actEffort = atof( TaskForm->EditActEffort->Text.c_str() );
		typeID = TaskForm->ComboBoxTaskType->Tag;
		statusID = TaskForm->ComboBoxTaskStatus->Tag;
		completed = TaskForm->completed;

		updateDatabase();
	}
}

void THE_TASK::createXMLattributes( xml::Element *theElement )
{
	THE_ITEM::createXMLattributes( theElement );

	theElement->setStringAttribute( "customerRef", customerRef );
	theElement->setStringAttribute( "module", module );
	theElement->setStringAttribute( "badFunction", badFunction );
	theElement->setStringAttribute( "release", release );
	theElement->addObject(
		new xml::Any( "externalRemarks", externalRemarks )
	);
	theElement->addObject(
		new xml::Any( "internalRemarks", internalRemarks )
	);

	theElement->setStringAttribute( "status",
		DocManDataModule->getStatus( statusID )
	);
	theElement->setStringAttribute( "type",
		DocManDataModule->getType( typeID )
	);

	theElement->setIntegerAttribute( "priority", priority );
	theElement->setFloatAttribute( "estEffort", estEffort );
	theElement->setFloatAttribute( "actEffort", actEffort );
	theElement->setStringAttribute( "assignedTo", getAssignedToUserName() );
	theElement->setStringAttribute( "completed", completed.DateTimeString().c_str() );
}
// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -p.
#	pragma option -a.
#endif


