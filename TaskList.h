/*
		Project:		DocMan
		Module:			TaskList.h
		Description:	The task list items
		Author:			Martin Gäckler
		Address:		Hofmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2026 Martin Gäckler

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

#ifndef TASK_LIST_H
#define TASK_LIST_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include "ItemManager.h"

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

class THE_TASKLIST : public THE_ITEM
{
	virtual ColumnTitle *getColumnTitles() const;

	static int itemCompare( const PTR_ITEM &e1, const PTR_ITEM &e2, int crit );

	public:
	THE_TASKLIST( int id, const FACTORY_BASE *theFactory )
	: THE_ITEM(
		ITEM_IS_CONTAINER|ITEM_IS_BROWSEABLE|ITEM_IS_MOVEABLE|ITEM_IS_COPYABLE,
		id, theFactory
	) {}
	virtual TGraphic *getItemPicture() const;
	virtual STRING getSize();

	virtual TBrowserFrame *getFrame() const;
	virtual int getColCount() const;
	virtual int *getColWidth() const;
	virtual void sort();
	virtual STRING drawCell( int col, int row, TCanvas *canvas, TRect &Rect );
};

typedef PTR_TEMPLATE<THE_TASKLIST> PTR_TASKLIST;

class THE_TASK : public THE_ITEM
{
	STRING		m_customerRef, m_module, m_badFunction, m_release,
				m_externalRemarks, m_internalRemarks;
	int			m_priority;
	double		m_estEffort, m_actEffort;
	int			m_typeID, m_statusID;
	TDateTime	m_completed;

	public:
	THE_TASK( int id, const FACTORY_BASE *theFactory )
	: THE_ITEM(
		ITEM_IS_BROWSEABLE|ITEM_IS_MOVEABLE|ITEM_IS_COPYABLE|ITEM_IS_COMPLETABLE,
		id, theFactory
	) {}
	void setData(
		const PTR_ITEM &parent, const STRING &name, const STRING &description,
		const STRING &customerRef, const STRING &module,
		const STRING &badFunction, const STRING &release,
		const STRING &externalRemarks, const STRING &internalRemarks,
		int priority, double estEffort, double actEffort,
		int typeID, int statusID, int assignedTo,
		TDateTime completed
	)
	{
		THE_ITEM::setData( parent, name, description );
		m_customerRef = customerRef;
		m_module = module;
		m_badFunction = badFunction;
		m_release = release;
		m_externalRemarks = externalRemarks;
		m_internalRemarks = internalRemarks;
		m_priority = priority;
		m_estEffort = estEffort;
		m_actEffort = actEffort;
		m_typeID = typeID;
		m_statusID = statusID;
		m_completed = completed;
		setAssignedTo( assignedTo );
	}
	virtual TGraphic *getItemPicture() const;
	virtual void loadFields( TQuery *query );
	virtual void updateDatabase();
	virtual void open();
	STRING getCustomerRef() const
	{
		return m_customerRef;
	}
	STRING getModule() const
	{
		return m_module;
	}
	STRING getBadFunction() const
	{
		return m_badFunction;
	}
	STRING getRelease() const
	{
		return m_release;
	}
	STRING getExternalRemarks() const
	{
		return m_externalRemarks;
	}
	STRING getInternalRemarks() const
	{
		return m_internalRemarks;
	}
	int getPriority() const
	{
		return m_priority;
	}
	double getEstEffort() const
	{
		return m_estEffort;
	}
	double getActEffort() const
	{
		return m_actEffort;
	}
	int getTypeID() const
	{
		return m_typeID;
	}
	int getStatusID() const
	{
		return m_statusID;
	}
	TDateTime getCompleted() const
	{
		return m_completed;
	}
	virtual void createXMLattributes( gak::xml::Element *target );
};
typedef PTR_TEMPLATE<THE_TASK> PTR_TASK;

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

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

// --------------------------------------------------------------------- //
// ----- class privates ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //

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

#endif
