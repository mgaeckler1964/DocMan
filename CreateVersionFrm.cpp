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

#include <gak/directory.h>

#pragma hdrstop

#include "CreateVersionFrm.h"
#include "PermissionsFrm.h"
#include "File.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TVersionCreateForm *VersionCreateForm;
ACTION_ADD_VERSION theCreateVersionAction;
//---------------------------------------------------------------------------

REFRESH_TYPE ACTION_ADD_VERSION::perform( PTR_ITEM theItem )
{
	PTR_FILE_BASE theFile = theItem;
	if( theFile )
	{
		STRING	downloadPath = theFile->getDownloadPath();
		VersionCreateForm->OpenDialog->InitialDir = (const char *)downloadPath;
		downloadPath += theFile->getName();

		if( !isFile( downloadPath ) )
			downloadPath = "";

		VersionCreateForm->EditFilePath->Text = (const char *)downloadPath;
		VersionCreateForm->OpenDialog->FileName = (const char *)downloadPath;

		if( VersionCreateForm->ShowModal() == mrOk )
		{
			theFile->createVersion(
				VersionCreateForm->EditFilePath->Text.c_str(),
				VersionCreateForm->MemoDescription->Text.c_str()
			);
			theFile->updateDatabase();

			return REFRESH_REDRAW;
		}
	}

	return REFRESH_NONE;
}

//---------------------------------------------------------------------------
const char *ACTION_ADD_VERSION::getLabel( void ) const
{
	return "Add Version...";
}
//---------------------------------------------------------------------------
bool ACTION_ADD_VERSION::acceptItem( THE_ITEM *theItem )
{
	doEnterFunctionEx(gakLogging::llDetail, "ACTION_ADD_VERSION::acceptItem");
	bool	accepted = false;

	const THE_FILE_BASE *theFile = dynamic_cast<const THE_FILE_BASE *>(theItem);
	if( theFile )
	{
		accepted = theFile->canCreateVersion() && ACTION_BASE::acceptItem( theItem );
	}

	return accepted;
}
//---------------------------------------------------------------------------
__fastcall TVersionCreateForm::TVersionCreateForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TVersionCreateForm::ButtonBrowseFileClick(TObject *)
{
	if( OpenDialog->Execute() )
		EditFilePath->Text = OpenDialog->FileName;
}
//---------------------------------------------------------------------------


