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


