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
#include <vcl/registry.hpp>
#pragma hdrstop

#include "DocManMain.h"
#include "ActionManager.h"
#include "File.h"
#include "Folder.h"
#include "PermissionsFrm.h"

#include "CheckOut.h"
//---------------------------------------------------------------------------
using namespace gak;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TCheckOutForm *CheckOutForm;
//---------------------------------------------------------------------------
class ACTION_CHECK_OUT : public ACTION_BASE_CHECK
{
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
};

class ACTION_CHECK_OUT_TREE : public ACTION_BASE_CHECK
{
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
};

class ACTION_ASSIGNED_TASK : public ACTION_BASE_CHECK
{
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
};

class ACTION_CANCEL_CHECK_OUT : public ACTION_BASE_CHECK
{
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
};

#pragma option -RT-
class THREAD_CHECK_OUT_TREE : public ThreadDocMan
{
	int taskID;

	virtual const char *getTitle( void ) const;
	virtual void perform( void );

	public:
	THREAD_CHECK_OUT_TREE( const PTR_ITEM &theItemToHandle, int taskID )
	: ThreadDocMan(theItemToHandle)
	{
		this->taskID = taskID;
	}
};
#pragma option -RT.

//---------------------------------------------------------------------------
static ACTION_CHECK_OUT			theCheckOutAction;
static ACTION_CHECK_OUT_TREE	theCheckOutTreeAction;
static ACTION_ASSIGNED_TASK		theAssignedTaskAction;
static ACTION_CANCEL_CHECK_OUT	theCancelAction;

//---------------------------------------------------------------------------
bool ACTION_CHECK_OUT::acceptItem( THE_ITEM *theItem )
{
	const THE_FILE_BASE *theFile = dynamic_cast<const THE_FILE_BASE *>(theItem);
	if( theFile )
	{
		return theFile->canReserve() && ACTION_BASE::acceptItem( theItem );
	}
	return false;
}

const char *ACTION_CHECK_OUT::getLabel( void ) const
{
	return "Check Out...";
}

REFRESH_TYPE ACTION_CHECK_OUT::perform( PTR_ITEM theItem )
{
	PTR_FILE_BASE theFile = theItem;
	if( theFile )
	{
		CheckOutForm->Caption = "Check Out";
		CheckOutForm->CheckBoxIncludeExtensions->Visible = true;
		if( CheckOutForm->ShowModal() == mrOk )
		{
			theFile->reserve( CheckOutForm->getSelectedTaskID() );
			theFile->updateDatabase();
			if( CheckOutForm->CheckBoxIncludeExtensions->Checked )
			{
				PTR_ITEM parent = theItem->getParent();
				if( parent )
				{
					CI_STRING	childName, baseName = theItem->getName();
					size_t		dotPos = baseName.searchRChar( '.' );
					if( dotPos != baseName.no_index )
					{
						baseName.cut( dotPos +1 );
					}

					ITEM_CONTENT *theContent = parent->getContent();
					for( size_t i=0; i<theContent->size(); i++ )
					{
						theFile = parent->getContentItem( i );
						if( theFile && theFile->canReserve() )
						{
							childName = theFile->getName();
							dotPos = childName.searchRChar( '.' );
							if( dotPos != (size_t)-1 )
								childName.cut( dotPos + 1 );
							if( childName == baseName )
							{
								theFile->reserve(
									CheckOutForm->getSelectedTaskID()
								);
								theFile->updateDatabase();
							}
						}
					}
				}
			}

/***/		return REFRESH_REDRAW;
		}

	}

	return REFRESH_NONE;
}
//---------------------------------------------------------------------------

bool ACTION_CHECK_OUT_TREE::acceptItem( THE_ITEM *theItem )
{
	THE_SOURCE_FOLDER *theFolder = dynamic_cast<THE_SOURCE_FOLDER *>(theItem);
	return theFolder ? ACTION_BASE::acceptItem( theItem ) : false;
}

const char *ACTION_CHECK_OUT_TREE::getLabel( void ) const
{
	return "Check Out...";
}

const char *THREAD_CHECK_OUT_TREE::getTitle( void ) const
{
	return "Check Out";
}

void THREAD_CHECK_OUT_TREE::perform( void )
{
	PTR_SOURCE_FOLDER theFolder = theItemToHandle;
	if( theFolder )
	{
		theFolder->reserve( taskID );
	}
}

REFRESH_TYPE ACTION_CHECK_OUT_TREE::perform( PTR_ITEM theItem )
{
	PTR_SOURCE_FOLDER theFolder = theItem;
	if( theFolder )
	{
		STRING	localPath = theFolder->getDownloadPath();
		if( localPath.isEmpty() )
/*@*/		throw Exception( "Don't know local path" );

		CheckOutForm->Caption = "Check Out";
		CheckOutForm->CheckBoxIncludeExtensions->Visible = false;

		if( CheckOutForm->ShowModal() == mrOk )
		{
			THREAD_CHECK_OUT_TREE *theThread = new THREAD_CHECK_OUT_TREE(
				theFolder,
				CheckOutForm->getSelectedTaskID()
			);
			theThread->StartThread();
			return REFRESH_RELOAD;
		}
	}

	return REFRESH_NONE;
}
//---------------------------------------------------------------------------
bool ACTION_ASSIGNED_TASK::acceptItem( THE_ITEM *theItem )
{
	const THE_FILE_BASE *theFile = dynamic_cast<const THE_FILE_BASE *>(theItem);
	if( theFile )
		return theFile->canUnreserve( true ) && ACTION_BASE::acceptItem( theItem );

	return false;
}

const char *ACTION_ASSIGNED_TASK::getLabel( void ) const
{
	return "Assinged Task...";
}

REFRESH_TYPE ACTION_ASSIGNED_TASK::perform( PTR_ITEM theItem )
{
	PTR_FILE_BASE theFile = theItem;
	if( theFile )
	{
		CheckOutForm->Caption = "Assigned Task";
		CheckOutForm->CheckBoxIncludeExtensions->Visible = false;
		CheckOutForm->setLastTask( theFile->getTask() );

		if( CheckOutForm->ShowModal() == mrOk )
		{
			theFile->setTask( CheckOutForm->getSelectedTaskID() );
			theFile->updateDatabase();
		}
 	}

	return REFRESH_NONE;
}
//---------------------------------------------------------------------------
bool ACTION_CANCEL_CHECK_OUT::acceptItem( THE_ITEM *theItem )
{
	const THE_FILE_BASE *theFile = dynamic_cast<const THE_FILE_BASE *>(theItem);
	if( theFile )
		return theFile->canUnreserve( true ) && ACTION_BASE::acceptItem( theItem );

	return false;
}
const char *ACTION_CANCEL_CHECK_OUT::getLabel( void ) const
{
	return "Cancel";
}

REFRESH_TYPE ACTION_CANCEL_CHECK_OUT::perform( PTR_ITEM theItem )
{
	PTR_FILE_BASE theFile = theItem;
	if( theFile )
	{
		theFile->cancelReserve();
		theFile->updateDatabase();

		return REFRESH_REDRAW;
	}

	return REFRESH_NONE;
}


//---------------------------------------------------------------------------
__fastcall TCheckOutForm::TCheckOutForm(TComponent* Owner)
	: TForm(Owner)
{
	lastTaskID = 0;
}
//---------------------------------------------------------------------------

void __fastcall TCheckOutForm::FormShow(TObject *)
{
	int			lastItemIndex = 0;
	AnsiString	taskTitle;

	if( !lastTaskID )
	{
		std::auto_ptr<TRegistry> reg( new TRegistry );

		if( reg->OpenKey( registryKey, false ) )
		{
			if( reg->ValueExists( "lastTaskID" ) )
				lastTaskID = reg->ReadInteger( "lastTaskID" );

			if( reg->ValueExists( "CheckBoxIncludeExtensions" ) )
				CheckBoxIncludeExtensions->Checked = reg->ReadBool( "CheckBoxIncludeExtensions" );

			reg->CloseKey();
		}
	}

	ComboBoxTasks->Items->Clear();
	ids.clear();

	ids.addElement( 0 );
	ComboBoxTasks->Items->Add( "None" );

	QueryOpenTasks->ParamByName( "actUser" )->AsInteger = vcl::getActUserID();
	for(
		QueryOpenTasks->Open();
		!QueryOpenTasks->Eof;
		QueryOpenTasks->Next()
	)
	{
		taskTitle = QueryOpenTasks->FieldByName( "Name" )->AsString;
		taskTitle += " (";
		taskTitle += QueryOpenTasks->FieldByName( "Task_Status" )->AsString;
		taskTitle += ')';

		ComboBoxTasks->Items->Add( taskTitle );
		ids.addElement( QueryOpenTasks->FieldByName( "ID" )->AsInteger );

		if( QueryOpenTasks->FieldByName( "ID" )->AsInteger == lastTaskID )
			lastItemIndex = ComboBoxTasks->Items->Count -1;
	}
	ComboBoxTasks->ItemIndex = lastItemIndex;
	QueryOpenTasks->Close();

	ActiveControl = ComboBoxTasks;
	vcl::bringWindowToFront( this );
}
//---------------------------------------------------------------------------
void __fastcall TCheckOutForm::ButtonOKClick(TObject *)
{
	lastTaskID = ids[ComboBoxTasks->ItemIndex];
	std::auto_ptr<TRegistry> reg( new TRegistry );

	reg->OpenKey( registryKey, true );
	reg->WriteInteger( "lastTaskID", lastTaskID );
	reg->WriteBool( "CheckBoxIncludeExtensions", CheckBoxIncludeExtensions->Checked );
	reg->CloseKey();
}
//---------------------------------------------------------------------------


