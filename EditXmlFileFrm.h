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

#ifndef EditXmlFileFrmH
#define EditXmlFileFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "EditFrm.h"
#include <ExtCtrls.hpp>
#include "xmlEditFram.h"

#include <gak/ChangeManager.h>

//---------------------------------------------------------------------------
class TXmlViewerForm;

class TEditXmlFileForm : public TEditFileForm
{
__published:	// Von der IDE verwaltete Komponenten
	TxmlEditorFrame *xmlEditorFrame;
	TButton *ButtonTransform;
	TButton *ButtonCheck;
	TButton *ButtonRenderer;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall ButtonCheckClick(TObject *Sender);
	void __fastcall ButtonTransformClick(TObject *Sender);
	void __fastcall xmlEditorFrameAttributeGridContextPopup(TObject *Sender,
		  TPoint &MousePos, bool &Handled);
	void __fastcall SelectPopupClick(TObject *Sender);
	void __fastcall ButtonRendererClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall Button1Click(TObject *Sender);

private:	// Anwender-Deklarationen
	PTR_FILE			theFile;
	gak::ChangeManager	theManager;
	TXmlViewerForm		*XmlViewerForm;
	gak::css::Rules		editorRules;
	gak::xml::Document	*theDocument;

	void styleChangedCB( const STRING &styleSheetFile, const STRING &stylesheetType );
public:		// Anwender-Deklarationen
	__fastcall TEditXmlFileForm(TComponent* Owner);
	virtual void setText( const PTR_FILE &theFile, const STRING &data );
	void setDocument( gak::xml::Document *theDocument );
	virtual STRING getText( void );
};
//---------------------------------------------------------------------------
extern PACKAGE TEditXmlFileForm *EditXmlFileForm;
//---------------------------------------------------------------------------
#endif
