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

#include "PurgeVersionsFrm.h"
#include "ActionManager.h"
#include "Folder.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TPurgeVersionsForm *PurgeVersionsForm;
//---------------------------------------------------------------------------
class ACTION_PURGE_VERSIONS : public ACTION_BASE_VERSIONS
{
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
};
//---------------------------------------------------------------------------
#pragma option -RT-
class THREAD_PURGE_VERSIONS : public ThreadDocMan
{
	int	numVersions;

	virtual const char *getTitle( void ) const;
	virtual void perform( void );

	public:
	THREAD_PURGE_VERSIONS( const PTR_ITEM &theItemToHandle, int numVersions )
	: ThreadDocMan( theItemToHandle )
	{
		this->numVersions = numVersions;
	};
};
#pragma option -RT+
//---------------------------------------------------------------------------
bool ACTION_PURGE_VERSIONS::acceptItem( THE_ITEM *theItem )
{
	bool accepted = theItem->acceptChildTypes( TYPE_DOCUMENT, TYPE_REMOTE_FILE, -1 );

	return accepted ? ACTION_BASE_VERSIONS::acceptItem( theItem ) : false;
}
//---------------------------------------------------------------------------
const char *ACTION_PURGE_VERSIONS::getLabel( void ) const
{
	return "Purge Versions...";
}
//---------------------------------------------------------------------------
REFRESH_TYPE ACTION_PURGE_VERSIONS::perform( PTR_ITEM theItem )
{
	if( PurgeVersionsForm->ShowModal() == mrOk )
	{
		int	numVersions = atoi(
			PurgeVersionsForm->EditNumVersions->Text.c_str()
		);
		if( numVersions >= 1 )
		{
			theItem->getContent();
			THREAD_PURGE_VERSIONS *theThread = new THREAD_PURGE_VERSIONS(
				theItem, numVersions
			);
			theThread->StartThread();
		}
	}

	return REFRESH_NONE;
}
//---------------------------------------------------------------------------

const char *THREAD_PURGE_VERSIONS::getTitle( void ) const
{
	return "Purge Versions";
}

void THREAD_PURGE_VERSIONS::perform( void )
{
	theItemToHandle->purgeVersions( numVersions );
}

//---------------------------------------------------------------------------
static ACTION_PURGE_VERSIONS theAction;
//---------------------------------------------------------------------------
__fastcall TPurgeVersionsForm::TPurgeVersionsForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TPurgeVersionsForm::EditNumVersionsKeyPress(
	  TObject *, char &Key)
{
	if( (Key < 0 || Key >= ' ') && (Key < '0' || Key > '9') )
		Key = 0;
}
//---------------------------------------------------------------------------
void __fastcall TPurgeVersionsForm::EditNumVersionsChange(TObject *)
{
	int	numVersion = atoi( EditNumVersions->Text.c_str() );
	ButtonOK->Enabled = ( numVersion >= 1 );
}
//---------------------------------------------------------------------------
