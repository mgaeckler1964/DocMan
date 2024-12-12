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

#ifndef OpenTasksFrmH
#define OpenTasksFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DBCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Db.hpp>
#include <DBGrids.hpp>
#include <DBTables.hpp>
#include <Grids.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TOpenTasksForm : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TDBNavigator *DBNavigator1;
	TDBGrid *DBGrid;
	TDataSource *DataSource;
	TQuery *QueryOpenTasks;
	TIntegerField *QueryOpenTasksID;
	TIntegerField *QueryOpenTasksParentID;
	TStringField *QueryOpenTasksName;
	TIntegerField *QueryOpenTasksAssigned_To;
	TStringField *QueryOpenTasksUsername;
	TStringField *QueryOpenTasksPATH;
	TSpeedButton *SpeedButton;
	TCheckBox *CheckBoxAllTasks;
	TStringField *QueryOpenTasksTask_Status;
	TIntegerField *QueryOpenTasksstatus;
	void __fastcall ReloadQuery(TObject *Sender);
	void __fastcall FormHide(TObject *Sender);
	void __fastcall QueryOpenTasksCalcFields(TDataSet *DataSet);
	void __fastcall DBGridDblClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TOpenTasksForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TOpenTasksForm *OpenTasksForm;
//---------------------------------------------------------------------------
#endif
