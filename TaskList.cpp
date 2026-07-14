/*
		Project:		DocMan
		Module:			TaskList.cpp
		Description:	The task list items
		Author:			Martin Gäckler
		Address:		Hofmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 2011-2026 Martin Gäckler

		This program is free software: you can redistribute it and/or modify
		it under the terms of the GNU General Public License as published by
		the Free Software Foundation, version 3.

		You should have received a copy of the GNU General Public License
		along with this program. If not, see <http://www.gnu.org/licenses/>.

		THIS SOFTWARE IS PROVIDED BY Martin Gäckler, Linz, Austria ``AS IS''
		AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
		TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
		PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR
		CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
		SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
		LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
		USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
		ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
		OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
		OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
		SUCH DAMAGE.
*/

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/numericString.h>
#include <gak/memory>

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
	virtual int getItemType() const;
	virtual bool acceptParent( const THE_ITEM *parent ) const;
	virtual PTR_ITEM createItem( int id ) const;
	virtual const char *getName() const;
	virtual TItemCreateForm *getForm() const;
	virtual PTR_ITEM createItemFromForm( const PTR_ITEM &parentId ) const;
};

class FACTORY_TASK : public FACTORY_BASE
{
	virtual int getItemType() const;
	virtual bool acceptParent( const THE_ITEM *parent ) const;
	virtual PTR_ITEM createItem( int id ) const;
	virtual const char *getName() const;
	virtual TItemCreateForm *getForm() const;
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

int FACTORY_TASKLIST::getItemType() const
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

const char *FACTORY_TASKLIST::getName() const
{
	return "Task List";
}

TItemCreateForm *FACTORY_TASKLIST::getForm() const
{
	return ItemCreateForm;
}

PTR_ITEM FACTORY_TASKLIST::createItemFromForm( const PTR_ITEM &parent ) const
{
	PTR_ITEM newList = createItem( 0 );

	newList->setData(
		parent,
		ItemCreateForm->EditName->Text.c_str(),
		ItemCreateForm->MemoDescription->Text.c_str()
	);
	newList->updateDatabase();

	return newList;
}

// --------------------------------------------------------------------- //

int FACTORY_TASK::getItemType() const
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

const char *FACTORY_TASK::getName() const
{
	return "Task";
}

TItemCreateForm *FACTORY_TASK::getForm() const
{
#ifndef DOCMANBG
	return TaskForm;
#else
	return NULL;
#endif
}

PTR_ITEM FACTORY_TASK::createItemFromForm( const PTR_ITEM &parent ) const
{
	PTR_TASK	newTask = createItem( 0 );

#ifndef DOCMANBG
	newTask->setData(
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
		gak::getValueN<double>( TaskForm->EditEstEffort->Text.c_str() ),
		gak::getValueN<double>( TaskForm->EditActEffort->Text.c_str() ),
		TaskForm->ComboBoxTaskType->Tag,
		TaskForm->ComboBoxTaskStatus->Tag,
		TaskForm->ComboBoxAssignedTo->Tag,
		TaskForm->completed
	);
	newTask->updateDatabase();
#endif

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

TGraphic *THE_TASKLIST::getItemPicture() const
{
	static Graphics::TBitmap *thePic = NULL;

	if( !thePic )
	{
		thePic = new Graphics::TBitmap();
		thePic->LoadFromResourceName((unsigned int)HInstance, "TASKLIST_BITMAP");
	}
	return thePic;
}

STRING THE_TASKLIST::getSize()
{
	int		childCount = getChildCount();
	STRING	size = formatNumber( childCount );

	size += " Tasks";

	return size;
};

int THE_TASKLIST::getColCount() const
{
	return 7;
}

int *THE_TASKLIST::getColWidth() const
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

ColumnTitle *THE_TASKLIST::getColumnTitles() const
{
	static ColumnTitle colTitles[] =
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

void THE_TASKLIST::sort()
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
			text = DocManDataModule->getStatus( theTask->getStatusID() );
	}
	else
		text = THE_ITEM::drawCell( col+1, row, canvas, Rect );

	return text;
}

TBrowserFrame *THE_TASKLIST::getFrame() const
{
#ifndef DOCMANBG
	static TTasklistBrowserFrame *theFrame = NULL;

	if( !theFrame )
		theFrame = new TTasklistBrowserFrame( NULL );

	return theFrame;
#else
	return NULL;
#endif
}

// --------------------------------------------------------------------- //

TGraphic *THE_TASK::getItemPicture() const
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

	m_customerRef = query->FieldByName( "customer_ref" )->AsString.c_str();
	m_module = query->FieldByName( "module" )->AsString.c_str();
	m_badFunction = query->FieldByName( "bad_function" )->AsString.c_str();
	m_release = query->FieldByName( "release" )->AsString.c_str();
	m_externalRemarks = query->FieldByName( "external_remarks" )->AsString.c_str();
	m_internalRemarks = query->FieldByName( "internal_remarks" )->AsString.c_str();
	m_priority = query->FieldByName( "priority" )->AsInteger;
	m_estEffort = query->FieldByName( "estimated_effort" )->AsFloat;
	m_actEffort = query->FieldByName( "actual_effort" )->AsFloat;
	m_typeID = query->FieldByName( "type" )->AsInteger;
	m_statusID = query->FieldByName( "status" )->AsInteger;
	m_completed = query->FieldByName( "completed" )->AsDateTime;
}

void THE_TASK::updateDatabase()
{
	THE_ITEM::updateDatabase();

	std::unique_ptr<TQuery>	theQuery( new TQuery( NULL ) );
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
	theQuery->Params->Items[col++]->AsString = m_customerRef;
	theQuery->Params->Items[col++]->AsString = m_module;
	theQuery->Params->Items[col++]->AsString = m_badFunction;
	theQuery->Params->Items[col++]->AsString = m_release;
	theQuery->Params->Items[col++]->AsString = m_externalRemarks;
	theQuery->Params->Items[col++]->AsString = m_internalRemarks;
	theQuery->Params->Items[col++]->AsInteger = m_priority;
	theQuery->Params->Items[col++]->AsFloat = m_estEffort;
	theQuery->Params->Items[col++]->AsFloat = m_actEffort;
	theQuery->Params->Items[col++]->AsInteger = m_typeID;
	theQuery->Params->Items[col++]->AsInteger = m_statusID;
	theQuery->Params->Items[col++]->AsDateTime = m_completed;
	theQuery->Params->Items[col++]->AsInteger = getID();

	theQuery->ExecSQL();
}

void THE_TASK::open()
{
#ifndef DOCMANBG
	TaskForm->setItem( this );

	if( TaskForm->ShowModal() == mrOk )
	{
		setName(TaskForm->EditName->Text.c_str());
		setDescription(TaskForm->MemoDescription->Text.c_str());
		setAssignedTo( TaskForm->ComboBoxAssignedTo->Tag );

		m_customerRef = TaskForm->EditCustomerRef->Text.c_str();
		m_module = TaskForm->ComboBoxModule->Text.c_str();
		m_badFunction = TaskForm->ComboBoxFunction->Text.c_str();
		m_release = TaskForm->EditRelease->Text.c_str();
		m_externalRemarks = TaskForm->MemoExternalRemarks->Text.c_str();
		m_internalRemarks = TaskForm->MemoInternalRemarks->Text.c_str();
		m_priority = TaskForm->UpDownPriority->Position;
		m_estEffort = gak::getValueN<double>( TaskForm->EditEstEffort->Text.c_str() );
		m_actEffort = gak::getValueN<double>( TaskForm->EditActEffort->Text.c_str() );
		m_typeID = TaskForm->ComboBoxTaskType->Tag;
		m_statusID = TaskForm->ComboBoxTaskStatus->Tag;
		m_completed = TaskForm->completed;

		updateDatabase();
	}
#endif
}

void THE_TASK::createXMLattributes( xml::Element *theElement )
{
	THE_ITEM::createXMLattributes( theElement );

	theElement->setStringAttribute( "customerRef", m_customerRef );
	theElement->setStringAttribute( "module", m_module );
	theElement->setStringAttribute( "badFunction", m_badFunction );
	theElement->setStringAttribute( "release", m_release );
	theElement->addObject(
		new xml::Any( "externalRemarks", m_externalRemarks )
	);
	theElement->addObject(
		new xml::Any( "internalRemarks", m_internalRemarks )
	);

	theElement->setStringAttribute( "status",
		DocManDataModule->getStatus( m_statusID )
	);
	theElement->setStringAttribute( "type",
		DocManDataModule->getType( m_typeID )
	);

	theElement->setIntegerAttribute( "priority", m_priority );
	theElement->setFloatAttribute( "estEffort", m_estEffort );
	theElement->setFloatAttribute( "actEffort", m_actEffort );
	theElement->setStringAttribute( "assignedTo", getAssignedToUserName() );
	theElement->setStringAttribute( "completed", m_completed.DateTimeString().c_str() );
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


