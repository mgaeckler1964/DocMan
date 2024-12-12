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

#ifndef TaskFrmH
#define TaskFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ItemCreateFrm.h"
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TTaskForm : public TItemCreateForm
{
__published:	// Von der IDE verwaltete Komponenten
	TEdit *EditCustomerRef;
	TLabel *Label2;
	TLabel *Label3;
	TComboBox *ComboBoxTaskType;
	TLabel *Label4;
	TComboBox *ComboBoxTaskStatus;
	TLabel *Label5;
	TComboBox *ComboBoxModule;
	TComboBox *ComboBoxFunction;
	TLabel *Label6;
	TLabel *Label7;
	TEdit *EditRelease;
	TLabel *Label8;
	TEdit *EditPriority;
	TUpDown *UpDownPriority;
	TLabel *Label9;
	TComboBox *ComboBoxAssignedTo;
	TLabel *Label10;
	TEdit *EditEstEffort;
	TLabel *Label11;
	TEdit *EditActEffort;
	TMemo *MemoInternalRemarks;
	TMemo *MemoExternalRemarks;
	TLabel *Label13;
	TLabel *LabelCompleted;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// Anwender-Deklarationen
public:		// Anwender-Deklarationen
	TDateTime	completed;
	__fastcall TTaskForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TTaskForm *TaskForm;
//---------------------------------------------------------------------------
#endif
