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

#ifndef TestStepFrmH
#define TestStepFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ItemCreateFrm.h"
#include "ItemManager.h"
#include "DocManMain.h"
//---------------------------------------------------------------------------
class THE_TEST_STEP : public THE_ITEM
{
	STRING	expectedResult;

	public:
	THE_TEST_STEP(
		int id, const FACTORY_BASE *theFactory,
		int flags=ITEM_IS_MOVEABLE|ITEM_IS_COPYABLE
	)
	: THE_ITEM(flags, id, theFactory )
	{}
	void setData(
		const PTR_ITEM &parent,
		const STRING &name, const STRING &description,
		const STRING &expectedResult, int assignedTo
	)
	{
		THE_ITEM::setData( parent, name, description );
		this->expectedResult = expectedResult;
		setAssignedTo( assignedTo );
		setOrder( parent->getChildCount() + 1 );
	}
	virtual void loadFields( TQuery *query );
	virtual void updateDatabase( void );
	virtual void open( void );
	virtual TGraphic *getItemPicture( void ) const;

	const STRING &getExpectedResult( void ) const
	{
		return expectedResult;
	}
};
typedef PTR_TEMPLATE<THE_TEST_STEP> PTR_TEST_STEP;
//---------------------------------------------------------------------------
class TTestStepForm : public TItemCreateForm
{
__published:	// IDE-managed Components
	TMemo *MemoExpected;
	TLabel *Label2;
	TComboBox *ComboBoxAssignedTo;
	TLabel *Label9;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
	__fastcall TTestStepForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TTestStepForm *TestStepForm;
//---------------------------------------------------------------------------
#endif

