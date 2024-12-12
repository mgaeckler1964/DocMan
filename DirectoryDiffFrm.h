//---------------------------------------------------------------------------

#ifndef DirectoryDiffFrmH
#define DirectoryDiffFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>

#include "Folder.h"
#include <Menus.hpp>
//---------------------------------------------------------------------------

class TDirectoryDiffForm : public TForm
{
__published:	// IDE-managed Components
	TStringGrid *StringGrid;
	TPopupMenu *PopupMenu;
	TMenuItem *MenuAdd;
	TMenuItem *MenuRemoveFromRepository;
	TMenuItem *MenuRemoveFromFileSystem;
	TMenuItem *MenuRefresh;
	TMenuItem *MenuAddVersion;
	TMenuItem *MenuCheckIn;
	TMenuItem *MenuShowDiff;
	TMenuItem *N1;
	TMenuItem *MenuAddReserved;
	TMenuItem *N2;
	TMenuItem *N3;
	TMenuItem *N4;
	void __fastcall FormResize(TObject *Sender);
	void __fastcall StringGridContextPopup(TObject *Sender, TPoint &MousePos,
		  bool &Handled);
	void __fastcall MenuRefreshClick(TObject *Sender);
	void __fastcall MenuRemoveFromFileSystemClick(TObject *Sender);
	void __fastcall MenuRemoveFromRepositoryClick(TObject *Sender);
	void __fastcall MenuAddClick(TObject *Sender);
	void __fastcall MenuAddVersionClick(TObject *Sender);
	void __fastcall PopupMenuPopup(TObject *Sender);
	void __fastcall MenuCheckInClick(TObject *Sender);
	void __fastcall MenuShowDiffClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
	FolderCompareList	m_theList;
	void refreshGrid( void );
public:		// User declarations
	__fastcall TDirectoryDiffForm(TComponent* Owner);
	size_t setItem( const PTR_LOCAL_FOLDER &theFolder );
};
//---------------------------------------------------------------------------
extern PACKAGE TDirectoryDiffForm *DirectoryDiffForm;
//---------------------------------------------------------------------------
#endif
