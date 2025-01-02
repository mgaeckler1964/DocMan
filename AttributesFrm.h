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

#ifndef AttributesFrmH
#define AttributesFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include <ExtCtrls.hpp>

#include <gak/array.h>

#include "DocManDM.h"

//---------------------------------------------------------------------------
class TAttributesForm : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
	TPanel *Panel1;
	TComboBox *ComboBoxCategories;
	TButton *ButtonAdd;
	TButton *ButtonRemove;
	TPanel *Panel2;
	TButton *ButtonSave;
	TButton *ButtonCancel;
	TButton *ButtonUpgrade;
	TPageControl *PageControl;
	TTable *TableCategories;
	TIntegerField *TableCategoriesID;
	TStringField *TableCategoriesNAME;
	TIntegerField *TableCategoriesMAX_ATTR_ID;
	TQuery *delQuery;
	TQuery *insertQuery;
	TQuery *QueryCategoryForItem;
	TIntegerField *QueryCategoryForItemID;
	TStringField *QueryCategoryForItemNAME;
	TIntegerField *QueryCategoryForItemCATEGORY_VERSION;
	TButton *ButtonApplyDown;
	TQuery *QueryCatVersion;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ButtonAddClick(TObject *Sender);
	void __fastcall ButtonRemoveClick(TObject *Sender);
	void __fastcall ComboBoxCategoriesChange(TObject *Sender);
	void __fastcall ButtonSaveClick(TObject *Sender);
	void __fastcall PageControlChange(TObject *Sender);
	void __fastcall ButtonUpgradeClick(TObject *Sender);
	void __fastcall ButtonApplyDownClick(TObject *Sender);
private:	// Anwender-Deklarationen
	gak::ArrayOfStrings	selectedCategories;
	TTabSheets			tabSheets;
	CategoryDatas		attrData;
	PTR_ITEM			theItem;
	int					theVersion;
	bool				editable;

	bool findAttributeControl(
		TObject *control, size_t *catIdx, size_t *attrIdx, size_t *valueIdx
	);
	void createControl(
		TWinControl *tab, const AttrDefinition &def, AttributeValue *val, int numValues, int yPos
	);
	void checkAddDelButtons( size_t catIdx, size_t attrIdx );
	void __fastcall addButtonClick( TObject *Sender );
	void __fastcall delButtonClick( TObject *Sender );

	void buildCategoryUI( TWinControl *tab, CategoryData *data );

public:		// User declarations
	void saveCategory( const CategoryData &data, int itemId, int itemVersion );

	void setItem( const PTR_ITEM &theItem, int theVersion = 0 )
	{
		this->theItem = theItem;
		this->theVersion = theVersion;
	}
	__fastcall TAttributesForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TAttributesForm *AttributesForm;
//---------------------------------------------------------------------------
#endif
