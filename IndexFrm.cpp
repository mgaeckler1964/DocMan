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
/*^*/			continue;
			}

			PTR_FILE	hit = getItem( storage.itemID );
			if( hit )
			{
				if( currentVersion && hit->getVersionNum() != storage.curVersion )
				{
/*^*/				continue;
				}
				if( volume && volume->getID() != hit->getVolumeID() )
				{
/*^*/				continue;
				}

				int i=0;
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

