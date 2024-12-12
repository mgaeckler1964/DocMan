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

#ifndef CategoriesFrmH
#define CategoriesFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBCtrls.hpp>
#include <DBGrids.hpp>
#include <DBTables.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>

#include "DocManDM.h"

//---------------------------------------------------------------------------
class TCategoriesForm : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TTable *TableCategories;
	TIntegerField *TableCategoriesID;
	TStringField *TableCategoriesNAME;
	TDBGrid *DBGrid1;
	TDBNavigator *DBNavigator1;
	TDataSource *DataSourceCategories;
	TPanel *Panel2;
	TComboBox *ComboBoxCatVersion;
	TStringGrid *StringGridAttributes;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TCheckBox *CheckBoxRequired;
	TButton *ButtonUp;
	TButton *ButtonDown;
	TButton *ButtonNew;
	TButton *ButtonRemove;
	TButton *ButtonSave;
	TComboBox *ComboBoxType;
	TEdit *EditMinCount;
	TEdit *EditMaxCount;
	TQuery *QuerySaveAttributes;
	TIntegerField *TableCategoriesMAX_ATTR_ID;
	TButton *ButtonPurgeVersions;
	TQuery *QueryDeleteAttributes;
	TQuery *QueryPurgeAttributes;
	TQuery *QueryCatVersions;
	TIntegerField *QueryCatVersionsCATEGORY_VERSION;
	TQuery *QueryItems;
	TIntegerField *QueryItemsID;
	TQuery *QueryCountUpgradeItems;
	TIntegerField *QueryCountUpgradeItemsCOUNTER;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall TableCategoriesAfterInsert(TDataSet *DataSet);
	void __fastcall TableCategoriesAfterScroll(TDataSet *DataSet);
	void __fastcall ComboBoxCatVersionChange(TObject *Sender);
	void __fastcall StringGridAttributesSelectCell(TObject *Sender, int ACol,
		  int ARow, bool &CanSelect);
	void __fastcall ButtonNewClick(TObject *Sender);
	void __fastcall ComboBoxTypeChange(TObject *Sender);
	void __fastcall ButtonRemoveClick(TObject *Sender);
	void __fastcall ButtonUpClick(TObject *Sender);
	void __fastcall ButtonDownClick(TObject *Sender);
	void __fastcall StringGridAttributesSetEditText(TObject *Sender, int ACol,
		  int ARow, const AnsiString Value);
	void __fastcall ButtonSaveClick(TObject *Sender);
	void __fastcall CheckBoxRequiredClick(TObject *Sender);
	void __fastcall EditMinCountChange(TObject *Sender);
	void __fastcall EditMaxCountChange(TObject *Sender);
	void __fastcall TableCategoriesBeforeScroll(TDataSet *DataSet);
	void __fastcall TableCategoriesBeforeDelete(TDataSet *DataSet);
	void __fastcall ButtonPurgeVersionsClick(TObject *Sender);
private:	// User declarations
	int				maxVersion, maxAttrId;
	bool			changed;
	CategoryData	attrDefinition;

	void setChanged( void )
	{
		changed = true;
		ButtonSave->Enabled = true;
	}
	void clrChanged( void )
	{
		changed = false;
		ButtonSave->Enabled = false;
	}

	void refreshGrid( void );
	void checkChanged( void );

public:		// User declarations
	__fastcall TCategoriesForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TCategoriesForm *CategoriesForm;
//---------------------------------------------------------------------------
#endif
