//---------------------------------------------------------------------------

#ifndef TargetBrowseFrmH
#define TargetBrowseFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ImgList.hpp>
//---------------------------------------------------------------------------

#include "ItemManager.h"

class TTargetBrowseForm : public TForm
{
__published:	// IDE-managed Components
	TTreeView *TreeView;
	TPanel *Panel1;
	TButton *Button1;
	TButton *Button2;
	TImageList *ImageList;
	void __fastcall TreeViewExpanding(TObject *Sender, TTreeNode *Node,
		  bool &AllowExpansion);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
	bool			all;
	enum itemTypes	selectableType;

	TTreeNode *findItem( TTreeNode *parent, int itemId );
	void collectItems( TTreeNode *node );
	int getImageIndex( const THE_ITEM *theContentItem );
	void setItem( const PTR_ITEM &startItem, bool all, enum itemTypes selectableType );
public:		// User declarations

	void setItem( const PTR_ITEM &startItem, bool all )
	{
		setItem( startItem, all, TYPE_ROOT );
	}
	void setItem( const PTR_ITEM &startItem, enum itemTypes selectableType )
	{
		setItem( startItem, false, selectableType );
	}
	__fastcall TTargetBrowseForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TTargetBrowseForm *TargetBrowseForm;
//---------------------------------------------------------------------------
#endif
