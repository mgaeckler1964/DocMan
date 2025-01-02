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

#ifndef IndexFrmH
#define IndexFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

#include "DocManBGThread.h"
#include <Grids.hpp>

//---------------------------------------------------------------------------
class TIndexForm : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TLabel *Label1;
	TLabel *Label9;
	TEdit *EditKeyword;
	TButton *ButtonSearch;
	TRadioButton *RadioButtonAny;
	TRadioButton *RadioButtonCompany;
	TRadioButton *RadioButtonPrivate;
	TRadioButton *RadioButtonArchive;
	TStringGrid *GridResult;
	TLabel *Label2;
	TCheckBox *CheckBoxCurrentVersion;
	TCheckBox *CheckBoxCaseSensitive;
	TCheckBox *CheckBoxFuzzy;
	TCheckBox *CheckBoxWildcard;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ButtonSearchClick(TObject *Sender);
	void __fastcall GridResultDblClick(TObject *Sender);
private:	// User declarations
	DocManIndex			m_globalIndex;
	gak::PODarray<int>	m_itemIDs;

public:		// User declarations
	__fastcall TIndexForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TIndexForm *IndexForm;
//---------------------------------------------------------------------------
#endif

