/*
		Project:		DocMan
		Module:			CryptoKeysFrm.h
		Description:	Form to manage the key for the (de)cryption
		Author:			Martin Gäckler
		Address:		Hofmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2025 Martin Gäckler

		This program is free software: you can redistribute it and/or modify
		it under the terms of the GNU General Public License as published by
		the Free Software Foundation, version 3.

		You should have received a copy of the GNU General Public License
		along with this program. If not, see <http://www.gnu.org/licenses/>.

		THIS SOFTWARE IS PROVIDED BY Martin Gäckler, Linz, Austria ``AS IS''
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

#ifndef CryptoKeysFrmH
#define CryptoKeysFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include <gak/cryptoShared.h>

#include "ItemManager.h"

//---------------------------------------------------------------------------
class TCryptoKeysForm : public TForm
{
__published:	// IDE-managed Components
	TListBox *ListBoxSelected;
	TListBox *ListBoxAvailable;
	TButton *ButtonAdd;
	TButton *ButtonRemove;
	TButton *ButtonOK;
	TButton *ButtonCancel;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ButtonAddClick(TObject *Sender);
	void __fastcall ButtonOKClick(TObject *Sender);
	void __fastcall ButtonRemoveClick(TObject *Sender);
private:	// User declarations
	gak::CryptoShared	crypto;
	PTR_ITEM			theItem;
	STRING				fileName;
public:		// User declarations
	__fastcall TCryptoKeysForm(TComponent* Owner);
	void setItem( const PTR_ITEM &newItem )
	{
		theItem = newItem;
	}
};
//---------------------------------------------------------------------------
extern PACKAGE TCryptoKeysForm *CryptoKeysForm;
//---------------------------------------------------------------------------
#endif

