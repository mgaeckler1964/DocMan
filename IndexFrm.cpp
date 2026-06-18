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

void __fastcall TIndexForm::ButtonSearchClick(TObject *Sender)
{
	doEnterFunctionEx(gakLogging::llInfo, "TIndexForm::ButtonSearchClick");
	ButtonSearch->Enabled = false;

	m_result = m_globalIndex.getRelevantHits(
		EditKeyword->Text.c_str(),
		CheckBoxFuzzy->Checked, !CheckBoxCaseSensitive->Checked,
		CheckBoxWildcard->Checked
	);
	const std::size_t 	hitCount = m_result.size();
	m_cRow = 0;

	HitCountLabel->Caption = gak::formatNumber(hitCount).add(" File(s)");

	if( hitCount )
	{
		m_pageSize = hitCount+1;
		if( m_pageSize > 26 )
		{
			m_pageSize = 26;
		}
	}
	else
	{
		m_pageSize = 2;
	}
	GridResult->RowCount = int(m_pageSize--);

	if( hitCount )
	{
		m_currentVersion = CheckBoxCurrentVersion->Checked;
		if( RadioButtonCompany->Checked )
			m_volume = getCompanyVolume();
		else if( RadioButtonArchive->Checked )
			m_volume = getArchiveVolume();
		else if( RadioButtonPrivate->Checked )
			m_volume = getPersonalItem( TYPE_PERSONAL_FOLDER );
		else
			m_volume = nullptr;

		m_it = m_result.cbegin();
		m_endIT = m_result.cend();
		MoreBttnClick( Sender );
	}	// if( hitCount )
	else
	{
		MoreBttn->Enabled = false;
	}

	if( !m_cRow )	// nothing found
	{
		GridResult->RowCount = 2;
		for( int i=0; i<GridResult->ColCount; ++i )
		{
			GridResult->Cells[i][1] = "-";
		}
	}

	ButtonSearch->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TIndexForm::MoreBttnClick(TObject *)
{
	GridResult->RowCount = GridResult->RowCount + m_pageSize;
	size_t	rowCount = m_pageSize;
	for(; m_it != m_endIT; ++m_it )
	{
		const StorageKey	&storage = m_it->m_source;
		if( m_volume && m_volume->getID() != storage.volumeID )
		{
/*^*/		continue;
		}
		if( m_currentVersion && storage.curVersion != storage.version )
		{
			doLogValueEx( gakLogging::llInfo, storage.itemID );
			doLogValueEx( gakLogging::llInfo, storage.version );
			doLogValueEx( gakLogging::llInfo, storage.curVersion );
/*^*/		continue;
		}

		PTR_FILE	hit = getItem( storage.itemID );
		if( hit )
		{
			if( m_currentVersion && hit->getVersionNum() != storage.curVersion )
			{
				doLogValueEx( gakLogging::llInfo, storage.itemID );
				doLogValueEx( gakLogging::llInfo, hit->getName() );
				doLogValueEx( gakLogging::llInfo, hit->getVersionNum() );
				doLogValueEx( gakLogging::llInfo, storage.curVersion );
/*^*/			continue;
			}
			if( m_volume && m_volume->getID() != hit->getVolumeID() )
			{
/*^*/			continue;
			}

			doLogValueEx( gakLogging::llInfo, storage.itemID );
			doLogValueEx( gakLogging::llInfo, hit->getName() );
			doLogValueEx( gakLogging::llInfo, storage.curVersion );
			doLogValueEx( gakLogging::llInfo, hit->getVersionNum() );

			int i=0;
			GridResult->Cells[i++][++m_cRow] = hit->getName();
			GridResult->Cells[i++][m_cRow] = static_cast<const char*>(
				hit->getParent()->getPath()
			);
			GridResult->Cells[i++][m_cRow] = hit->getVersionNum();
			GridResult->Cells[i++][m_cRow] = storage.version;
			GridResult->Cells[i++][m_cRow] = m_it->m_positions.size();
			GridResult->Cells[i][m_cRow] = m_it->m_relevance*100;
			m_itemIDs[m_cRow] = storage.itemID;
			if( !--rowCount )
			{
/*v*/			break;
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
	MoreBttn->Enabled = (m_it != m_endIT);
	GridResult->RowCount = m_cRow+1;	// include head line
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

