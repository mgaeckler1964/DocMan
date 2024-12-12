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

#ifndef EditFrmH
#define EditFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

#include <gak/string.h>

#include "File.h"

//---------------------------------------------------------------------------
class TEditFileForm : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
	TPanel *Panel1;
	TButton *Button1;
	TButton *Button2;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
private:	// Anwender-Deklarationen
	PTR_FILE	theFile;
	STRING		destFile;
	bool		createVersion;
protected:
	void setTitle( const STRING &fileName )
	{
		AnsiString	nCaption = "Edit ";
		nCaption += (const char *)fileName;

		Caption = nCaption;
	}

public:		// Anwender-Deklarationen
	__fastcall TEditFileForm(TComponent* Owner);

	void setDestination( const PTR_FILE &theFile, const STRING &destFile, bool createVersion )
	{
		this->theFile = theFile;
		this->destFile = destFile;
		this->createVersion = createVersion;

		setTitle( theFile->getPath() );
	}
	virtual void setText( const PTR_FILE &theFile, const STRING &data ) = 0;
	virtual STRING getText( void ) = 0;
};
//---------------------------------------------------------------------------
extern PACKAGE TEditFileForm *EditFileForm;
//---------------------------------------------------------------------------
#endif
