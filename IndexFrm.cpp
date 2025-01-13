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

#include <gak/fmtNumber.h>
#pragma hdrstop

#include "IndexFrm.h"
#include "DocManDM.h"
#include "DocManMain.h"
#include "file.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TIndexForm *IndexForm;
//---------------------------------------------------------------------------
__fastcall TIndexForm::TIndexForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------


void __fastcall TIndexForm::FormShow(TObject *)
{
	int i=0;
	GridResult->Cells[i++][0] = "Name";
	GridResult->Cells[i++][0] = "Path";
	GridResult->Cells[i++][0] = "Max Version";
	GridResult->Cells[i++][0] = "Version";
	GridResult->Cells[i++][0] = "Hit Count";
	GridResult->Cells[i++][0] = "Relevance";
	readDocManIndex( &m_globalIndex );
}
//---------------------------------------------------------------------------

void __fastcall TIndexForm::ButtonSearchClick(TObject *)
{
	doEnterFunctionEx(gakLogging::llInfo, "TIndexForm::ButtonSearchClick");
	ButtonSearch->Enabled = false;

	DocManSearchResult result = m_globalIndex.getRelevantHits(
		EditKeyword->Text.c_str(),
		CheckBoxFuzzy->Checked, !CheckBoxCaseSensitive->Checked,
		CheckBoxWildcard->Checked
	);
	const std::size_t 	hitCount = result.size();
	std::size_t 		rowCount;
	std::size_t			row = 0;

	if( hitCount )
	{
		rowCount = hitCount+1;
		if( rowCount > 26 )
		{
			rowCount = 26;
		}
	}
	else
	{
		rowCount = 2;
	}
	GridResult->RowCount = int(rowCount--);

	if( hitCount )
	{
		bool		currentVersion = CheckBoxCurrentVersion->Checked;
		PTR_ITEM 	volume;
		if( RadioButtonCompany->Checked )
			volume = getCompanyVolume();
		else if( RadioButtonArchive->Checked )
			volume = getArchiveVolume();
		else if( RadioButtonPrivate->Checked )
			volume = getPersonalItem( TYPE_PERSONAL_FOLDER );
		for(
			DocManSearchResult::const_iterator it = result.cbegin(),
				endIT = result.cend();
			it != endIT;
			++it
		)
		{
			const StorageKey	&storage = it->m_source;
			if( volume && volume->getID() != storage.volumeID )
			{
/*^*/			continue;
			}
			if( currentVersion && storage.curVersion != storage.version )
			{
				doLogValueEx( gakLogging::llInfo, storage.itemID );
				doLogValueEx( gakLogging::llInfo, storage.version );
				doLogValueEx( gakLogging::llInfo, storage.curVersion );
/*^*/			continue;
			}

			PTR_FILE	hit = getItem( storage.itemID );
			if( hit )
			{
				if( currentVersion && hit->getVersionNum() != storage.curVersion )
				{
					doLogValueEx( gakLogging::llInfo, storage.itemID );
					doLogValueEx( gakLogging::llInfo, hit->getName() );
					doLogValueEx( gakLogging::llInfo, hit->getVersionNum() );
					doLogValueEx( gakLogging::llInfo, storage.curVersion );
/*^*/				continue;
				}
				if( volume && volume->getID() != hit->getVolumeID() )
				{
/*^*/				continue;
				}

				int i=0;
				doLogValueEx( gakLogging::llInfo, storage.itemID );
				doLogValueEx( gakLogging::llInfo, hit->getName() );

				GridResult->Cells[i++][++row] = hit->getName();
				GridResult->Cells[i++][row] = static_cast<const char*>(
					hit->getParent()->getPath()
				);
				GridResult->Cells[i++][row] = storage.curVersion;
				GridResult->Cells[i++][row] = storage.version;
				GridResult->Cells[i++][row] = it->m_positions.size();
				GridResult->Cells[i][row] = it->m_relevance*100;
				m_itemIDs[row] = storage.itemID;
				if( !--rowCount )
				{
/*v*/				break;
				}
			}
			else
			{
				doLogMessageEx( gakLogging::llWarn, "Search Hit not found" );
				doLogValueEx( gakLogging::llWarn, storage.itemID );
				doLogValueEx( gakLogging::llWarn, storage.version );
				doLogValueEx( gakLogging::llWarn, storage.curVersion );
			}
		}
	}	// if( hitCount )


	if( !row )	// nothing found
	{
		GridResult->RowCount = 2;
		for( int i=0; i<GridResult->ColCount; ++i )
		{
			GridResult->Cells[i][1] = "-";
		}
	}
	else
	{
		GridResult->RowCount = row+1;
	}

	ButtonSearch->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TIndexForm::GridResultDblClick(TObject *)
{
	int			itemID = m_itemIDs[GridResult->Row];
	if( itemID > 0 )
	{
		PTR_ITEM	theItem = getItem( itemID );
		if( theItem )
		{
			DocManMainForm->openItem( theItem->getParent(), theItem->getID() );
			DocManMainForm->BringToFront();
			DocManMainForm->SetFocus();
		}
	}
}
//---------------------------------------------------------------------------

