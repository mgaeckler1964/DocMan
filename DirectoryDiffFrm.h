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
