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
#include <dos.h>
#include <io.h>
#include <sys/stat.h>

#include <vcl.h>
#include <gak/vcl_tools.h>
#pragma hdrstop

#include "DirectoryDiffFrm.h"
#include "ActionManager.h"
#include "Folder.h"
#include "File.h"
#include "RemoteItems.h"
#include "CreateVersionFrm.h"
#include "CheckInFrm.h"
#include "Delete.h"
#include "DiffFrm.h"

//---------------------------------------------------------------------------
using namespace gak;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDirectoryDiffForm *DirectoryDiffForm;
//---------------------------------------------------------------------------
class ACTION_DIR_DIFF : public ACTION_BASE_PROPERTIES
{
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual RefhreshType perform( PTR_ITEM theItem );
};
//---------------------------------------------------------------------------
bool ACTION_DIR_DIFF::acceptItem( THE_ITEM *theItem )
{
	const THE_LOCAL_FOLDER *theLocalFolder = dynamic_cast<
		const THE_LOCAL_FOLDER *
	>(
		theItem
	);
	if( !theLocalFolder )
		return false;

	bool accepted = theItem->acceptChildTypes( TYPE_DOCUMENT, TYPE_REMOTE_FILE, -1 );

	return accepted ? ACTION_BASE::acceptItem( theItem ) : false;
}
//---------------------------------------------------------------------------
const char *ACTION_DIR_DIFF::getLabel( void ) const
{
	return "Show Diff...";
}
//---------------------------------------------------------------------------
RefhreshType ACTION_DIR_DIFF::perform( PTR_ITEM theItem )
{
	PTR_LOCAL_FOLDER theFolder = theItem;
	if( theFolder )
	{
		if( DirectoryDiffForm->setItem( theFolder ) )
			DirectoryDiffForm->ShowModal();
		else
			Application->MessageBox(
				"There are no changes or nothing to compare.",
				"DocMan", MB_ICONSTOP
			);

		return rtRELOAD;
	}

	return rtNONE;
}
//---------------------------------------------------------------------------
static ACTION_DIR_DIFF theAction;
//---------------------------------------------------------------------------
__fastcall TDirectoryDiffForm::TDirectoryDiffForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TDirectoryDiffForm::FormResize(TObject *)
{
	int	newWidth = StringGrid->Width-25;

	for( int i=1; i<StringGrid->ColCount; i++ )
		newWidth -= StringGrid->ColWidths[i];

	StringGrid->ColWidths[0] = newWidth;
}
//---------------------------------------------------------------------------
void TDirectoryDiffForm::refreshGrid( void )
{
	char		buffer[32];
	size_t		col, row, numEntries;
	TStringGrid	*theGrid = StringGrid;

	numEntries = m_theList.size();

	for( size_t i=0; i<numEntries; i++ )
	{
		col=0; row=i+1;
		const FolderCompareEntry &theEntry = m_theList[i];
		theGrid->Cells[col++][row] = static_cast<const char *>(theEntry.name);
		if( !theEntry.theFile )
		{
			theGrid->Cells[col++][row] = "";
			theGrid->Cells[col++][row] = "";
		}
		else
		{
			theGrid->Cells[col++][row] = theEntry.dbModTime.DateTimeString();
			sprintf( buffer, "%lu", theEntry.dbSize );
			theGrid->Cells[col++][row] = buffer;
		}
		if( theEntry.inFS )
		{
			theGrid->Cells[col++][row] = theEntry.localModTime.DateTimeString();
			sprintf( buffer, "%lu", theEntry.localSize );
			theGrid->Cells[col++][row] = buffer;
		}
		else
		{
			theGrid->Cells[col++][row] = "";
			theGrid->Cells[col++][row] = "";
		}

		theGrid->Cells[col++][row] = static_cast<const char *>(theEntry.statusSTR);
	}
}

//---------------------------------------------------------------------------
size_t TDirectoryDiffForm::setItem( const PTR_LOCAL_FOLDER &theFolder )
{
	size_t		col, numEntries;

	m_theList.clear();
	::SetCursor( LoadCursor( NULL, IDC_WAIT ) );
	theFolder->compare( &m_theList );
	::SetCursor( LoadCursor( NULL, IDC_ARROW ) );

	numEntries = m_theList.size();
	if( numEntries )
	{
		TStringGrid	*theGrid = StringGrid;
		STRING		newCaption = "Changes in ";

		newCaption += theFolder->getPath();

		Caption = static_cast<const char *>(newCaption);

		theGrid->ColCount = 6;

		theGrid->RowCount = numEntries+1;

		col = 0;
		theGrid->ColWidths[col++] = 100;
		theGrid->ColWidths[col++] = 110;
		theGrid->ColWidths[col++] = 80;
		theGrid->ColWidths[col++] = 110;
		theGrid->ColWidths[col++] = 80;
		theGrid->ColWidths[col++] = 120;

		col = 0;
		theGrid->Cells[col++][0] = "File";
		theGrid->Cells[col++][0] = "Repository Time";
		theGrid->Cells[col++][0] = "Repository Size";
		theGrid->Cells[col++][0] = "Local Time";
		theGrid->Cells[col++][0] = "Local Size";
		theGrid->Cells[col++][0] = "Status";

		col = 0;
		theGrid->Cells[col++][1] = "";
		theGrid->Cells[col++][1] = "";
		theGrid->Cells[col++][1] = "";
		theGrid->Cells[col++][1] = "";
		theGrid->Cells[col++][1] = "";
		theGrid->Cells[col++][1] = "";

		refreshGrid();

		FormResize( NULL );
	}

	return numEntries;
}
//---------------------------------------------------------------------------


void __fastcall TDirectoryDiffForm::StringGridContextPopup(TObject *,
	  TPoint &MousePos, bool &Handled)
{
	int col, row;
	StringGrid->MouseToCell( MousePos.x, MousePos.y, col, row );
	if( row > 0 )
	{
		StringGrid->Row = row;
	}
	else
	{
		Handled = true;
	}

	if( row-1 >= int(m_theList.size()) )
	{
		Handled = true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TDirectoryDiffForm::PopupMenuPopup(TObject *)
{ 
	const FolderCompareEntry	&theEntry = m_theList[StringGrid->Row-1];
	const PTR_FILE_BASE			&theFile = theEntry.theFile;

	// Add Version, if local file is newer, only
	MenuAddVersion->Enabled =
		(theEntry.status == csLOCAL_NEWER)
		? theCreateVersionAction.acceptItem( theFile )
		: false
	;

	MenuCheckIn->Enabled =
		theEntry.reserved
		? theCheckInAction.acceptItem( theFile )
		: false
	;

	// Refresh, if file is in repository, only
	// Add, if file is not in repository, only
	if( theEntry.status == csDB_MISSING )
	{
		MenuRefresh->Enabled = false;
		MenuAdd->Enabled = true;
		MenuAddReserved->Enabled = true;
		MenuRemoveFromFileSystem->Enabled = true;
	}
	else if( theEntry.status == csFOLDER )
	{
		MenuRefresh->Enabled = false;
		MenuAdd->Enabled = false;
		MenuAddReserved->Enabled = false;
		MenuRemoveFromFileSystem->Enabled = false;
	}
	else
	{
		MenuRefresh->Enabled = true;
		MenuAdd->Enabled = false;
		MenuAddReserved->Enabled = false;
		MenuRemoveFromFileSystem->Enabled = false;
	}
	if( theEntry.status == csDB_MISSING
	|| theEntry.status == csLOCAL_MISSING
	|| theEntry.status == csFOLDER )
		MenuShowDiff->Enabled = false;
	else
		MenuShowDiff->Enabled = true;

	// RemoveFromRepository, only if local file is missing
	MenuRemoveFromRepository->Enabled =
		( theEntry.status == csLOCAL_MISSING )
		? theDeleteAction.acceptItem( theFile ) || thePurgeAction.acceptItem( theFile )
		: false
	;
}
//---------------------------------------------------------------------------

void __fastcall TDirectoryDiffForm::MenuRefreshClick(TObject *)
{
	try
	{
		FolderCompareEntry	&theEntry = m_theList[StringGrid->Row-1];
		const PTR_FILE_BASE	&theFile = theEntry.theFile;

		if( theFile )
		{
			const STRING &filePath = theEntry.filePath;
			theFile->download( 0, true, filePath );

			theEntry.status = csOK;
			theEntry.statusSTR = "File refreshed";
			refreshGrid();
		}
	}
	catch( std::exception &e )
	{
		throw Exception( e.what() );
	}
}
//---------------------------------------------------------------------------

void __fastcall TDirectoryDiffForm::MenuRemoveFromFileSystemClick(
	  TObject *)
{
	try
	{
		FolderCompareEntry	&theEntry = m_theList[StringGrid->Row-1];
		const F_STRING		&filePath = theEntry.filePath;

		chmod( filePath, S_IREAD|S_IWRITE );
		unlink( filePath );

		theEntry.status = csOK;
		theEntry.statusSTR = "File deleted";
		refreshGrid();
	}
	catch( std::exception &e )
	{
		throw Exception( e.what() );
	}
}
//---------------------------------------------------------------------------

void __fastcall TDirectoryDiffForm::MenuRemoveFromRepositoryClick(
	  TObject *)
{
	try
	{
		FolderCompareEntry	&theEntry = m_theList[StringGrid->Row-1];
		const PTR_FILE_BASE	&theFile = theEntry.theFile;

		if( theFile )
		{
			// do not use action delete, because this runs a second thread
			theFile->deleteItem();

			theEntry.status = csOK;
			theEntry.statusSTR = "Item deleted";
			refreshGrid();
		}
	}
	catch( std::exception &e )
	{
		throw Exception( e.what() );
	}
}
//---------------------------------------------------------------------------

void __fastcall TDirectoryDiffForm::MenuAddClick( TObject *Sender )
{
	try
	{
		FolderCompareEntry		&theEntry = m_theList[StringGrid->Row-1];
		const PTR_LOCAL_FOLDER	&theFolder = theEntry.theParent;
		STRING name = theEntry.name;

		if( theFolder->acceptChildType( TYPE_DOCUMENT ) )
		{
			PTR_FILE theFile = createItem( TYPE_DOCUMENT );

			if( theFile )
			{
				STRING	localFile = (Sender == MenuAdd)
					? theFolder->getDownloadPath() + name
					: NULL_STRING;
				theFile->setData(
					theFolder,
					name,
					"",
					localFile
				);

				if( localFile.isEmpty() )
					theFile->reserve();

				theFile->updateDatabase();

				if( !localFile.isEmpty() )
					chmod( localFile, S_IREAD );

				theEntry.status = csOK;
				theEntry.statusSTR = "Item added";
				refreshGrid();
			}
		}
		else if( theFolder->acceptChildType( TYPE_REMOTE_FILE ) )
		{
			PTR_REMOTE_FILE theFile = createItem( TYPE_REMOTE_FILE );

			if( theFile )
			{
				DateTime	modDateTime = theEntry.localModTime;
				time_t		modTime = modDateTime.getUtcUnixSeconds();
				STRING	localFile = (Sender == MenuAdd)
					? theFolder->getDownloadPath() + name
					: NULL_STRING;
				theFile->setData(
					theFolder,
					name,
					"",
					localFile,
					"",
					0,
					modTime, modTime
				);

				theFile->updateDatabase();

				if( localFile.isEmpty() )
					theFile->reserve();

				theFile->updateDatabase();

				if( !localFile.isEmpty() )
					chmod( localFile, S_IREAD );

				theEntry.status = csOK;
				theEntry.statusSTR = "Item added";
				refreshGrid();
			}
		}
	}
	catch( std::exception &e )
	{
		throw Exception( e.what() );
	}
}
//---------------------------------------------------------------------------

void __fastcall TDirectoryDiffForm::MenuAddVersionClick(TObject *)
{
	try
	{
		FolderCompareEntry	&theEntry = m_theList[StringGrid->Row-1];
		const PTR_FILE_BASE	&theFile = theEntry.theFile;

		if( theFile )
		{
			if( theCreateVersionAction.perform( theFile ) )
			{
				theEntry.status = csOK;
				theEntry.statusSTR = "Version added";
				refreshGrid();
			}
		}
	}
	catch( std::exception &e )
	{
		throw Exception( e.what() );
	}
}
//---------------------------------------------------------------------------

void __fastcall TDirectoryDiffForm::MenuCheckInClick(TObject *)
{
	try
	{
		FolderCompareEntry	&theEntry = m_theList[StringGrid->Row-1];
		const PTR_FILE_BASE	&theFile = theEntry.theFile;

		if( theFile )
		{
			if( theCheckInAction.perform( theFile ) )
			{
				theEntry.status = csOK;
				theEntry.statusSTR = "Item checked in";
				refreshGrid();
			}
		}
	}
	catch( std::exception &e )
	{
		throw Exception( e.what() );
	}
}
//---------------------------------------------------------------------------

void __fastcall TDirectoryDiffForm::MenuShowDiffClick(TObject *)
{
	try
	{
		const FolderCompareEntry	&theEntry = m_theList[StringGrid->Row-1];
		const PTR_FILE_BASE			&theFile = theEntry.theFile;

		if( theFile )
		{
			theDiffAction.perform( theFile );
		}
	}
	catch( std::exception &e )
	{
		throw Exception( e.what() );
	}
}
//---------------------------------------------------------------------------

void __fastcall TDirectoryDiffForm::FormShow(TObject *)
{
	gak::vcl::bringWindowToFront( this );
}
//---------------------------------------------------------------------------

