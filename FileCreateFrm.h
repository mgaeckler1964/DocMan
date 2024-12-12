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

#ifndef FileCreateFrmH
#define FileCreateFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include "ItemCreateFrm.h"
#include "File.h"

//---------------------------------------------------------------------------
class TFileCreateForm : public TItemCreateForm
{
__published:	// IDE-managed Components
	TLabel *Label2;
	TEdit *EditFilePath;
	TButton *ButtonBrowse;
	TOpenDialog *OpenDialog;
	TCheckBox *CheckBoxReserve;
	TLabel *LabelTemplate;
	TComboBox *ComboBoxTemplate;
	void __fastcall ButtonBrowseClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
	Array<PTR_FILE>	templates;
public:		// User declarations
	__fastcall TFileCreateForm(TComponent* Owner);
	PTR_FILE	getTemplate( void )
	{
		return templates[ComboBoxTemplate->ItemIndex];
	}
};
//---------------------------------------------------------------------------
extern PACKAGE TFileCreateForm *FileCreateForm;
//---------------------------------------------------------------------------
#endif
