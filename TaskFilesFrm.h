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

#ifndef TaskFilesFrmH
#define TaskFilesFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include "ItemManager.h"
#include <Db.hpp>
#include <DBTables.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>

//---------------------------------------------------------------------------
class TTaskFilesForm : public TForm
{
__published:	// IDE-managed Components
	TQuery *OpenQuery;
	TIntegerField *OpenQueryID;
	TDataSource *OpenDataSource;
	TDBGrid *DBGridOpen;
	TDBGrid *DBGridSelected;
	TQuery *SelectedQuery;
	TIntegerField *SelectedQueryID;
	TDataSource *SelectedDataSource;
	TButton *ButtonAdd;
	TButton *ButtonRemove;
	TButton *ButtonClose;
	TQuery *AddQuery;
	TQuery *RemoveQuery;
	TButton *ButtonCheckIn;
	TLabel *Label1;
	TLabel *Label2;
	TCheckBox *CheckBoxBranch;
	TStringField *OpenQueryPATH;
	TStringField *SelectedQueryPATH;
	TIntegerField *SelectedQueryPARENTID;
	TStringField *SelectedQueryNAME;
	TIntegerField *OpenQueryPARENTID;
	TStringField *OpenQueryNAME;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall ButtonAddClick(TObject *Sender);
	void __fastcall ButtonRemoveClick(TObject *Sender);
	void __fastcall ButtonCheckInClick(TObject *Sender);
	void __fastcall SelectedQueryCalcFields(TDataSet *DataSet);
	void __fastcall OpenQueryCalcFields(TDataSet *DataSet);
private:	// User declarations
	PTR_ITEM	theItem;
	void refresh( void );
	static void gotoID( int ID, TQuery *query, TIntegerField *idField );
	static void gotoRecord( int recNO, TQuery *query );
public:		// User declarations
	void setItem( const PTR_ITEM &theItem )
	{
		this->theItem = theItem;
	}
	__fastcall TTaskFilesForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TTaskFilesForm *TaskFilesForm;
//---------------------------------------------------------------------------
#endif
