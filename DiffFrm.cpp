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
#include <gak/vcl_tools.h>
#pragma hdrstop

#include "DiffFrm.h"
#include "File.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDiffForm *DiffForm;
ACTION_DIFF theDiffAction;
//---------------------------------------------------------------------------
bool ACTION_DIFF::acceptItem( THE_ITEM *theItem )
{
	const THE_FILE_BASE *theFile = dynamic_cast<const THE_FILE_BASE *>(theItem);
	return theFile ? ACTION_BASE::acceptItem( theItem ) : false;
}
//---------------------------------------------------------------------------
const char *ACTION_DIFF::getLabel( void ) const
{
	return "Show Diff...";
}
//---------------------------------------------------------------------------
RefhreshType ACTION_DIFF::perform( PTR_ITEM theItem )
{
	PTR_FILE_BASE theFile = theItem;
	if( theFile )
	{
		DiffForm->ShowDiff( theFile );
	}

	return rtNONE;
}
//---------------------------------------------------------------------------
__fastcall TDiffForm::TDiffForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void TDiffForm::FindChange( int direction )
{
	int 			currentPosition = MemoDiff->SelStart + direction;
	AnsiString		text = MemoDiff->Text;
	AnsiString		marker;
	int				textLength = text.Length();

	while( currentPosition >= 0 && currentPosition < textLength-3 )
	{
		if( !currentPosition )
			marker = text.SubString( 0, 2 );
		else if( text[currentPosition] == '\n' )
		{
			marker = text.SubString( currentPosition+1, 2 );
		}
		else
		{
			currentPosition += direction;
/*^*/		continue;
		}
		if( marker == "+>" || marker == "-<" )
			break;
		currentPosition += direction;
	}
	if( currentPosition < 0 )
		currentPosition = 0;

	MemoDiff->SelStart = currentPosition;
	SendMessage( MemoDiff->Handle, EM_SCROLLCARET, 0, 0 );

}
//---------------------------------------------------------------------------
void __fastcall TDiffForm::SpeedButtonPrevClick(TObject *)
{
	FindChange( -1 );
}
//---------------------------------------------------------------------------

void __fastcall TDiffForm::SpeedButtonNextClick(TObject *)
{
	FindChange( 1 );
}
//---------------------------------------------------------------------------

void TDiffForm::ShowDiff( THE_FILE_BASE *theFile, int firstVersion, int secondVersion )
{
	const char *diffResult = theFile->compare( firstVersion, secondVersion );
	if( diffResult && *diffResult )
	{
		AnsiString	line;
		MemoDiff->Lines->Clear();
		for( const char *cp = diffResult; *cp; cp++ )
		{
			if( *cp == '\n' )
			{
				MemoDiff->Lines->Add( line );
				line = "";
			}
			else
				line += *cp;
		}
		if( line != "" )
			MemoDiff->Lines->Add( line );
		MemoDiff->SelStart = 0;

		STRING newCaption = "Changes of ";
		newCaption += theFile->getName();
		Caption = (const char *)newCaption;

		MemoDiff->SelStart = 0;
		SendMessage( MemoDiff->Handle, EM_SCROLLCARET, 0, 0 );

		Show();

		gak::vcl::bringWindowToFront( this );
	}
	else
	{
		Application->MessageBox( "There are no changes.", "DocMan", MB_ICONSTOP );
	}
}
//---------------------------------------------------------------------------

