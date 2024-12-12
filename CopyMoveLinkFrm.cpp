/*
		Project:		DocMan
		Module:			
		Description:	
		Author:			Martin Gäckler
		Address:		Hofmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2024 Martin Gäckler

		This program is free software: you can redistribute it and/or modify  
		it under the terms of the GNU General Public License as published by  
		the Free Software Foundation, version 3.

		You should have received a copy of the GNU General Public License 
		along with this program. If not, see <http://www.gnu.org/licenses/>.

		THIS SOFTWARE IS PROVIDED BY Martin Gäckler, Austria, Linz ``AS IS''
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

//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ActionManager.h"
#include "PermissionsFrm.h"
#include "CopyMoveLinkFrm.h"
#include "TargetBrowseFrm.h"
#include "Folder.h"
#include "File.h"
#include "Alias.h"

//---------------------------------------------------------------------------
using namespace gak;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TCopyMoveLinkForm *CopyMoveLinkForm;
//---------------------------------------------------------------------------
class ACTION_MOVE : public ACTION_BASE_MOVE
{
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
};
//---------------------------------------------------------------------------
class ACTION_COPY : public ACTION_BASE_MOVE
{
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
};
//---------------------------------------------------------------------------
class ACTION_LINK : public ACTION_BASE_MOVE
{
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
};
//---------------------------------------------------------------------------
class ACTION_ALIAS : public ACTION_BASE_MOVE
{
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
};
//---------------------------------------------------------------------------
class ACTION_BOOKMARK : public ACTION_BASE_MOVE
{
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
};
//---------------------------------------------------------------------------
bool ACTION_MOVE::acceptItem( THE_ITEM *theItem )
{
	if( !theItem->isMoveable() )
/***/	return false;

	int perms = theItem->getUserPermissions();
	return perms & ITEM_PERM_MOVE ? ACTION_BASE::acceptItem( theItem ) : false;
}
//---------------------------------------------------------------------------
bool ACTION_COPY::acceptItem( THE_ITEM *theItem )
{
	if( !theItem->isCopyable() )
/***/	return false;

	THE_RELEASE_FOLDER *theFolder = dynamic_cast<THE_RELEASE_FOLDER *>(theItem);
	return theFolder ? false : ACTION_BASE::acceptItem( theItem );
}
//---------------------------------------------------------------------------
bool ACTION_LINK::acceptItem( THE_ITEM *theItem )
{
	THE_FILE *theFile = dynamic_cast<THE_FILE *>(theItem);
	return theFile ? ACTION_BASE::acceptItem( theFile ) : false;
}
//---------------------------------------------------------------------------
bool ACTION_ALIAS::acceptItem( THE_ITEM *theItem )
{
	THE_ALIAS *theAlias = dynamic_cast<THE_ALIAS *>(theItem);
	return theAlias ? false : ACTION_BASE::acceptItem( theItem );
}
//---------------------------------------------------------------------------
bool ACTION_BOOKMARK::acceptItem( THE_ITEM *theItem )
{
	return theItem->isContainer() ? ACTION_BASE::acceptItem( theItem ) : false;
}
//---------------------------------------------------------------------------
const char *ACTION_MOVE::getLabel( void ) const
{
	return "Move...";
}
//---------------------------------------------------------------------------
const char *ACTION_COPY::getLabel( void ) const
{
	return "Copy...";
}
//---------------------------------------------------------------------------
const char *ACTION_LINK::getLabel( void ) const
{
	return "Link...";
}
//---------------------------------------------------------------------------
const char *ACTION_ALIAS::getLabel( void ) const
{
	return "Make Alias...";
}
//---------------------------------------------------------------------------
const char *ACTION_BOOKMARK::getLabel( void ) const
{
	return "Make Bookmark...";
}
//---------------------------------------------------------------------------
REFRESH_TYPE ACTION_MOVE::perform( PTR_ITEM theItem )
{
	REFRESH_TYPE	refresh;
	PTR_ITEM 		parent = theItem->getParent();

	CopyMoveLinkForm->setItem( theItem->getName(), parent );
	CopyMoveLinkForm->Caption = "Move Item";
	if( CopyMoveLinkForm->ShowModal() == mrOk )
	{
		theItem->moveTo(
			CopyMoveLinkForm->getDestination(),
			CopyMoveLinkForm->EditName->Text.c_str()
		);
		theItem->updateDatabase();
		refresh = REFRESH_RELOAD;
	}
	else
		refresh = REFRESH_NONE;

	return refresh;
}
//---------------------------------------------------------------------------
REFRESH_TYPE ACTION_COPY::perform( PTR_ITEM theItem )
{
	REFRESH_TYPE	refresh;
	PTR_ITEM		parent = theItem->getParent();

	CopyMoveLinkForm->setItem( theItem->getName(), parent );
	CopyMoveLinkForm->Caption = "Copy Item";
	if( CopyMoveLinkForm->ShowModal() == mrOk )
	{
		ThreadCopying *theThread = new ThreadCopying(
			theItem,
			CopyMoveLinkForm->getDestination(),
			false, false,
			CopyMoveLinkForm->EditName->Text.c_str()
		);
		theThread->StartThread();

		refresh = REFRESH_RELOAD;
	}
	else
	{
		refresh = REFRESH_NONE;
	}
	return refresh;
}
//---------------------------------------------------------------------------
REFRESH_TYPE ACTION_LINK::perform( PTR_ITEM theItem )
{
	REFRESH_TYPE	refresh = REFRESH_NONE;
	PTR_ITEM		parent = theItem->getParent();
	PTR_FILE		theFile = theItem;

	if( theFile )
	{
		CopyMoveLinkForm->setItem( theItem->getName(), parent );
		CopyMoveLinkForm->Caption = "Link Item";
		if( CopyMoveLinkForm->ShowModal() == mrOk )
		{
			theFile->link(
				CopyMoveLinkForm->getDestination(),
				CopyMoveLinkForm->EditName->Text.c_str()
			);
			refresh = REFRESH_RELOAD;
		}
	}
	return refresh;
}
//---------------------------------------------------------------------------
REFRESH_TYPE ACTION_ALIAS::perform( PTR_ITEM theItem )
{
	REFRESH_TYPE	refresh = REFRESH_NONE;
	PTR_ITEM		parent = theItem->getParent();

	CopyMoveLinkForm->setItem( theItem->getName(), parent );
	CopyMoveLinkForm->Caption = "Make Alias";
	if( CopyMoveLinkForm->ShowModal() == mrOk )
	{
		PTR_ALIAS	theAlias = createItem( TYPE_ALIAS );

		theAlias->setData(
			CopyMoveLinkForm->getDestination(),
			CopyMoveLinkForm->EditName->Text.c_str(),
			theItem->getDescription(),
			theItem->getID()
		);
		theAlias->updateDatabase();

		refresh = REFRESH_RELOAD;
	}
	return refresh;
}
//---------------------------------------------------------------------------
REFRESH_TYPE ACTION_BOOKMARK::perform( PTR_ITEM theItem )
{
	REFRESH_TYPE	refresh = REFRESH_NONE;
	PTR_ITEM		thePersonal = getPersonalItem( TYPE_BOOKMARK_FOLDER );

	CopyMoveLinkForm->setItem( theItem->getName(), thePersonal );
	CopyMoveLinkForm->Caption = "Make Bookmark";
	if( CopyMoveLinkForm->ShowModal() == mrOk )
	{
		PTR_ALIAS	theAlias = createItem( TYPE_ALIAS );

		theAlias->setData(
			CopyMoveLinkForm->getDestination(),
			CopyMoveLinkForm->EditName->Text.c_str(),
			theItem->getDescription(),
			theItem->getID()
		);
		theAlias->updateDatabase();

		refresh = REFRESH_RELOAD;
	}

	return refresh;
}
//---------------------------------------------------------------------------
static ACTION_MOVE		theMoveAction;
static ACTION_COPY		theCopyAction;
static ACTION_LINK		theLinkAction;
static ACTION_ALIAS		theAliasAction;
static ACTION_BOOKMARK	theBookmarkAction;
//---------------------------------------------------------------------------
__fastcall TCopyMoveLinkForm::TCopyMoveLinkForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TCopyMoveLinkForm::ButtonBrowseClick(TObject *)
{
	TargetBrowseForm->setItem( theDestination, false );
	if( TargetBrowseForm->ShowModal() == mrOk )
	{
		TTreeNode *selected = TargetBrowseForm->TreeView->Selected;
		if( selected )
		{
			theDestination = getItem( (int)selected->Data );
			STRING		Text;
			while( selected )
			{
				if( selected->Parent )	// ignore the root
					Text = (STRING)"\\" + selected->Text.c_str() + Text;
				selected = selected->Parent;
			}
			if( Text.isEmpty() )
				Text = "<root>";
			EditTarget->Text = (const char*)Text;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TCopyMoveLinkForm::FormShow(TObject *)
{
	ActiveControl = EditName;
	vcl::bringWindowToFront( this );
}
//---------------------------------------------------------------------------

