/*
		Project:		DocMan
		Module:			
		Description:	
		Author:			Martin G�ckler
		Address:		Hofmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2024 Martin G�ckler

		This program is free software: you can redistribute it and/or modify  
		it under the terms of the GNU General Public License as published by  
		the Free Software Foundation, version 3.

		You should have received a copy of the GNU General Public License 
		along with this program. If not, see <http://www.gnu.org/licenses/>.

		THIS SOFTWARE IS PROVIDED BY Martin G�ckler, Austria, Linz ``AS IS''
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

#include "RemoteFolderCreateFrm.h"
#include "RemoteServerFrm.h"
#include "RemoteItems.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SyncFolderCreateFrm"
#pragma resource "*.dfm"
TRemoteFolderCreateForm *RemoteFolderCreateForm;
//---------------------------------------------------------------------------
__fastcall TRemoteFolderCreateForm::TRemoteFolderCreateForm(TComponent* Owner)
	: TSyncFolderCreateForm(Owner)
{
}
//---------------------------------------------------------------------------


void __fastcall TRemoteFolderCreateForm::FormShow(TObject *Sender)
{
	int	RemoteServerID;
	TSyncFolderCreateForm::FormShow( Sender );

	PTR_REMOTE_FOLDER	parent = getParent();
	if( parent )
	{
		RemoteServerID = parent->getServerID();
		ComboBoxRemoteServer->Enabled = false;
	}
	else
		ComboBoxRemoteServer->Enabled = true;

	RemoteServerForm->FillCombobox( ComboBoxRemoteServer, RemoteServerID );
}
//---------------------------------------------------------------------------


