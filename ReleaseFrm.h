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

#ifndef ReleaseFrmH
#define ReleaseFrmH
//---------------------------------------------------------------------------
#include <stdio.h>

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ItemCreateFrm.h"
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TReleaseForm : public TItemCreateForm
{
__published:	// IDE-managed Components
	TEdit *EditPatch;
	TEdit *EditMinor;
	TEdit *EditMajor;
	TLabel *Label2;
	TUpDown *UpDownMajor;
	TUpDown *UpDownMinor;
	TUpDown *UpDownPatch;
	TCheckBox *CheckBoxCreateLinks;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall UpDownClick(TObject *Sender, TUDBtnType Button);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
	bool createBranchFlag;

	void createName( void )
	{
		int major = UpDownMajor->Position;
		int minor = UpDownMinor->Position;
		int patch = UpDownPatch->Position;

		char buffer[32];

		sprintf( buffer, "%d.%d.%d", major, minor, patch );
		EditName->Text = buffer;
	}
public:		// User declarations
	void createBranch( THE_ITEM *theItem )
	{
		setItem( theItem );
		createBranchFlag = true;
	}
	void updateRelease( THE_ITEM *theItem )
	{
		setItem( theItem );
		createBranchFlag = false;
	}
	__fastcall TReleaseForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TReleaseForm *ReleaseForm;
//---------------------------------------------------------------------------
#endif
