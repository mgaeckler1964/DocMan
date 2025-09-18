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
#include <vcl.h>
#pragma hdrstop

#include "TargetBrowseFrm.h"
#include "ItemManager.h"
#include "Folder.h"

//---------------------------------------------------------------------------
using namespace gak;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TTargetBrowseForm *TargetBrowseForm;
//---------------------------------------------------------------------------
__fastcall TTargetBrowseForm::TTargetBrowseForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
struct THE_ITEM_IMAGE
{
	int			itemIndex;
	TGraphic	*thePicture;
};
//---------------------------------------------------------------------------
int TTargetBrowseForm::getImageIndex( const THE_ITEM *theContentItem )
{
	static Array<THE_ITEM_IMAGE>	images;

	TGraphic	*thePic = theContentItem->getItemPicture();
	if( !thePic )
	{
		return -1;
	}

	for(
		Array<THE_ITEM_IMAGE>::const_iterator it = images.cbegin(),
			endIT = images.cend();
		it != endIT;
		++it
	)
	{
		const THE_ITEM_IMAGE &img = *it;
		if( img.thePicture == thePic )
		{
/***/		return img.itemIndex;
		}
	}

	THE_ITEM_IMAGE	&newImg = images.createElement();

	newImg.itemIndex = ImageList->AddMasked( (Graphics::TBitmap*)thePic, TColor( 0 ) );
	newImg.thePicture = thePic;

	return newImg.itemIndex;
}
//---------------------------------------------------------------------------
TTreeNode *TTargetBrowseForm::findItem( TTreeNode *parent, int itemId )
{
	TTreeNode	*child = parent->getFirstChild();
	while( child )
	{
		int	childId = (int)child->Data;
		if( childId == itemId )
			break;

		child = child->getNextSibling();
	}
	return child;
}

//---------------------------------------------------------------------------
void TTargetBrowseForm::collectItems( TTreeNode *node )
{
	STRING	name;
	int 	itemID = (int)node->Data;

	PTR_ITEM theItem = getItem( itemID );
	if( theItem )
	{
		ITEM_CONTENT *content = theItem->getContent();
		for( size_t i=0; i<content->size(); i++ )
		{
			PTR_ITEM theContentItem = theItem->getContentItem( i );
			if( all
			|| theContentItem->getItemType() == selectableType
			|| (theContentItem->isContainer() && theContentItem->isBrowseable()) )
			{
				name = (*theContentItem).getName();
				if( name.isEmpty() )
					name = "<root>";

				if( !findItem( node, (*theContentItem).getID() ) )
				{
					TTreeNode *childNode = TreeView->Items->AddChild( node, (const char *)name );
					int imageIndex = getImageIndex( theContentItem );
					childNode->ImageIndex = imageIndex;
					childNode->SelectedIndex = imageIndex;
					childNode->Data = (void *)(*theContentItem).getID();
					childNode->HasChildren = true;
				}
			}
		}
	}
}

//---------------------------------------------------------------------------
void TTargetBrowseForm::setItem( const PTR_ITEM &iStartItem, bool all, enum itemTypes selectableType )
{
	this->all = all;
	this->selectableType = selectableType;

	TTreeNode	*node, *subNode;

	PTR_ITEM	startItem = iStartItem
		? iStartItem
		: getPersonalItem( TYPE_PERSONAL_FOLDER )
	;

	TreeView->Items->Clear();

	node = TreeView->Items->AddChild( NULL, "<root>" );
	node->Data = (void *)-1;
	node->HasChildren = true;
	collectItems( node );

	ArrayOfInts	*ancestors = startItem->getAncestors();
	size_t	   	ancestorCount = ancestors->size();

	for( size_t i=0; i<=ancestorCount; i++ )
	{
		PTR_ITEM theItem =
			i<ancestorCount ? startItem->getAncestorItem( i ) : startItem
		;
		STRING	name = theItem->getName();

		subNode = node ? findItem( node, theItem->getID() ) : NULL;
		if( !subNode )
		{
			node = TreeView->Items->AddChild( node, (const char *)name );
			node->Data = (void *)theItem->getID();
			int imageIndex = getImageIndex( theItem );
			node->ImageIndex = imageIndex;
			node->SelectedIndex = imageIndex;
		}
		else
			node = subNode;

		collectItems( node );
	}
	if( node )
		TreeView->Selected = node;
}
//---------------------------------------------------------------------------
void __fastcall TTargetBrowseForm::TreeViewExpanding(TObject *,
	  TTreeNode *Node, bool &AllowExpansion)
{
	TTreeNode	*child = Node->getFirstChild();
	if( !child )
	{
		collectItems( Node );
	}
	child = Node->getFirstChild();
	if( !child )
	{
		Node->HasChildren = false;
		AllowExpansion = false;
	}
}
//---------------------------------------------------------------------------

void __fastcall TTargetBrowseForm::Button1Click(TObject *)
{
	TTreeNode *selected = TreeView->Selected;
	if( selected )
	{
		PTR_ITEM	theItem = getItem( (int)selected->Data );
		if( theItem
		&& (selectableType < 0 || theItem->getItemType() == selectableType) )
		{
			ModalResult = mrOk;
		}
		else
			MessageBeep( MB_ICONEXCLAMATION );
	}
	else
		MessageBeep( MB_ICONASTERISK );
}
//---------------------------------------------------------------------------

void __fastcall TTargetBrowseForm::FormShow(TObject *)
{
	vcl::bringWindowToFront( this );
}
//---------------------------------------------------------------------------

