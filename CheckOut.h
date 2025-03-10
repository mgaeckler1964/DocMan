/*
		Project:		DocMan
		Module:			
		Description:	
		Author:			Martin G�ckler
		Address:		Hofmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2024 Martin G�ckler

		This program is free software: you can redistribute it and/or modify  
		it under the terms of the GNU General Public License as published by  
		the Free Software Foundation, version 3.

		You should have received a copy of the GNU General Public License 
		along with this program. If not, see <http://www.gnu.org/licenses/>.

		THIS SOFTWARE IS PROVIDED BY Martin G�ckler, Austria, Linz ``AS IS''
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

#ifndef CheckOutH
#define CheckOutH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBTables.hpp>

#include <gak/array.h>

//---------------------------------------------------------------------------
enum FormMode
{
	CHECKOUT_FILE,
	CHECKOUT_TREE,
	CHANGE_TASK
};

//---------------------------------------------------------------------------
class TCheckOutForm : public TForm
{
__published:	// IDE-managed Components
	TButton *ButtonOK;
	TButton *ButtonCancel;
	TLabel *Label1;
	TComboBox *ComboBoxTasks;
	TQuery *QueryOpenTasks;
	TCheckBox *CheckBoxFlag;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ButtonOKClick(TObject *Sender);
private:	// User declarations
	gak::ArrayOfInts	m_ids;
	int		   	m_lastTaskID;
	char		*m_regKey;
public:		// User declarations
	__fastcall TCheckOutForm(TComponent* Owner);
	int getSelectedTaskID( void )
	{
		if( ComboBoxTasks->ItemIndex >= 0 )
		{
			return m_ids[ComboBoxTasks->ItemIndex];
		}

		return 0;
	}
	int ShowModal( FormMode mode );
	void setLastTask( taskID )
	{
		m_lastTaskID = taskID;
	}
};
//---------------------------------------------------------------------------
extern PACKAGE TCheckOutForm *CheckOutForm;
//---------------------------------------------------------------------------
#endif
