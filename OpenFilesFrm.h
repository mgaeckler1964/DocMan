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

#ifndef OpenFilesFrmH
#define OpenFilesFrmH
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
class TOpenFilesForm : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TDBNavigator *DBNavigator1;
	TDBGrid *DBGrid;
	TDataSource *DataSource;
	TQuery *QueryOpenFiles;
	TIntegerField *QueryOpenFilesID;
	TIntegerField *QueryOpenFilesParentID;
	TStringField *QueryOpenFilesName;
	TIntegerField *QueryOpenFilesReservedBy;
	TStringField *QueryOpenFilesReservedOn;
	TStringField *QueryOpenFilesUsername;
	TStringField *QueryOpenFilesPATH;
	TSpeedButton *SpeedButton;
	TCheckBox *CheckBoxAllFiles;
	TIntegerField *QueryOpenFilesReservedFor;
	TStringField *QueryOpenFilesTASK_PATH;
	void __fastcall ReloadQuery(TObject *Sender);
	void __fastcall FormHide(TObject *Sender);
	void __fastcall QueryOpenFilesCalcFields(TDataSet *DataSet);
	void __fastcall DBGridDblClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
	void reloadQuery( void );
public:		// User declarations
	__fastcall TOpenFilesForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TOpenFilesForm *OpenFilesForm;
//---------------------------------------------------------------------------
#endif
