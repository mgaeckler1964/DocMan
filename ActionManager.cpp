/*
		Project:		
		Module:			
		Description:	
		Author:			Martin G�ckler
		Company:		CRESD GmbH
		Address:		Arminiusstr. 3a, D-8543 M�nchen
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

#include <gak/array.h>

#include "ActionManager.h"
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

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

/*
	this is dynamicaly generated, because otherwise, the contructor
	of ACTION_BASE may use this array, before the array itself is
	contructed
*/
static Array<ACTION_PTR> *allActions = NULL;

// --------------------------------------------------------------------- //
// ----- class static data --------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- prototypes ---------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module functions ---------------------------------------------- //
// --------------------------------------------------------------------- //

static int actionCompare(
	const ACTION_PTR &a1,
	const ACTION_PTR &a2
)
{
	int compareResult = a1->getGroup() - a2->getGroup();

	if( !compareResult )
		compareResult = strcmp( a1->getLabel(), a2->getLabel() );

	return compareResult;
}

// --------------------------------------------------------------------- //
// ----- class inlines ------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class constructors/destructors -------------------------------- //
// --------------------------------------------------------------------- //

ACTION_BASE::ACTION_BASE()
{
	if( !allActions )
	{
		allActions = new Array<ACTION_PTR>;
	}

	allActions->addElement( this );
}

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

bool ACTION_BASE::acceptItem( THE_ITEM *theItem )
{
	doEnterFunction( "ACTION_BASE::acceptItem()" );

	return theItem->getID() && !theItem->isDeleted();
}

bool ACTION_BASE::acceptUser( const UserOrGroup & )
{
	return true;
}

int ACTION_BASE_PROPERTIES::getGroup( void ) const
{
	return 0;
}

int ACTION_BASE_PERMISSIONS::getGroup( void ) const
{
	return 10;
}

int ACTION_BASE_MOVE::getGroup( void ) const
{
	return 20;
}

int ACTION_BASE_CHECK::getGroup( void ) const
{
	return 30;
}

int ACTION_BASE_VERSIONS::getGroup( void ) const
{
	return 40;
}

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

void getItemActions(
	const UserOrGroup &actUser,
	THE_ITEM *theItem,
	Array<ACTION_BASE *> *actions
)
{
	doEnterFunction( "getItemActions()" );
	doLogValue( allActions->size() );

	actions->clear();

	for(
		Array<ACTION_BASE *>::const_iterator it = allActions->cbegin(),
			endIT = allActions->cend();
		it != endIT;
		++it
	)
	{
		ACTION_BASE *theAction = *it;

		if( theAction->acceptUser( actUser )
		&& theAction->acceptItem( theItem ) )
		{
			actions->addElement( theAction );
		}
	}

	actions->sort( actionCompare );
}

#ifdef __BORLANDC__
#	pragma option -p.
#	pragma option -a.
#endif

