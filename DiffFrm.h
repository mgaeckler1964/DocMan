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

#ifndef DiffFrmH
#define DiffFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>

#include "ActionManager.h"
//---------------------------------------------------------------------------
class THE_FILE_BASE;
class TDiffForm : public TForm
{
__published:	// IDE-managed Components
	TMemo *MemoDiff;
	TPanel *Panel1;
	TSpeedButton *SpeedButtonPrev;
	TSpeedButton *SpeedButtonNext;
	void __fastcall SpeedButtonPrevClick(TObject *Sender);
	void __fastcall SpeedButtonNextClick(TObject *Sender);
private:	// User declarations
	void FindChange( int direction );
public:		// User declarations
	__fastcall TDiffForm(TComponent* Owner);
	void ShowDiff( THE_FILE_BASE *theFile, int firstVersion=0, int secondVersion=-1 );
};
//---------------------------------------------------------------------------
class ACTION_DIFF : public ACTION_BASE_PROPERTIES
{ 
	public:
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
};
//---------------------------------------------------------------------------
extern PACKAGE TDiffForm *DiffForm;
extern ACTION_DIFF theDiffAction;
//---------------------------------------------------------------------------
#endif
