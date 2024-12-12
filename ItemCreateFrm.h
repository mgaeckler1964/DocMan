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

#ifndef ItemCreateFrmH
#define ItemCreateFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include "ItemManager.h"

//---------------------------------------------------------------------------
class TItemCreateForm : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TEdit *EditName;
	TButton *ButtonOk;
	TButton *ButtonCancel;
	TLabel *Label12;
	TMemo *MemoDescription;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ButtonOkClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

protected:	// User declarations
	PTR_ITEM	theItem, theParent;

public:		// User declarations
	void setItem( const PTR_ITEM &theItem )	// for properties page
	{
		this->theItem = theItem;
		theParent = NULL;
	}
	void setParent( const PTR_ITEM &theParent )	// for creation page
	{
		this->theParent = theParent;
		theItem = NULL;
	}
	const PTR_ITEM	&getParent( void ) const
	{
		return theParent;
	}
	__fastcall TItemCreateForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TItemCreateForm *ItemCreateForm;
//---------------------------------------------------------------------------
#endif
