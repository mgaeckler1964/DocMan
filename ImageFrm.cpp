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
#include <vcl/jpeg.hpp>

#include <gak/ci_string.h>
#pragma hdrstop

#include "ImageFrm.h"
#include "DocManMain.h"
#include "ActionManager.h"
#include "Folder.h"
#include "RemoteItems.h"

//---------------------------------------------------------------------------
using namespace gak;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TImageBrowserForm *ImageBrowserForm;
//---------------------------------------------------------------------------

#pragma options -RT-
class ACTION_IMAGE_BROWSER : public ACTION_BASE_PROPERTIES
{
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual const char *getLabel( void ) const;
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
};
#pragma options -RT.

//---------------------------------------------------------------------------
bool ACTION_IMAGE_BROWSER::acceptItem( THE_ITEM *theItem )
{
	THE_FOLDER *theFolder = dynamic_cast<THE_FOLDER *>(theItem);
	return theFolder ? ACTION_BASE::acceptItem( theItem ) : false;
}

//---------------------------------------------------------------------------
const char *ACTION_IMAGE_BROWSER::getLabel( void ) const
{
	return "Image Browser";
}

static ACTION_IMAGE_BROWSER				theImageBrowserAction;
//---------------------------------------------------------------------------
REFRESH_TYPE ACTION_IMAGE_BROWSER::perform( PTR_ITEM theItem )
{
	PTR_FILE_REF	theFileRef;
	PTR_REMOTE_FILE	theRemoteFile;
	PTR_FILE        theFile;

	bool	hasDownloadPath;
	STRING	title = theItem->getDownloadPath();
	if( title.isEmpty() )
	{
		title = theItem->getPath();
		hasDownloadPath = false;
	}
	else
		hasDownloadPath = true;

	// Application->CreateForm(__classid(TImageBrowserForm), &ImageBrowserForm);
	ImageBrowserForm = new TImageBrowserForm( (TComponent*)NULL );
	// ::SetParent( ImageBrowserForm->Handle, NULL );
	// ::SetWindowLong( ImageBrowserForm->Handle, GWL_HWNDPARENT, NULL );

	size_t	numImages = 0;
	ITEM_CONTENT *theContent = theItem->getContent();
	for(
		ITEM_CONTENT::const_iterator it = theContent->cbegin(), endIT = theContent->cend();
		it != endIT;
		++it
	)
	{
		if( theFileRef = *it )
		{
			if( hasDownloadPath && ImageBrowserForm->addImage(
				theFileRef->getID(), theFileRef->getDownloadPath( theItem )
			))
			{
				numImages++;
			}
		}
		else if( theFile = *it )
		{
			if( hasDownloadPath )
			{
				if( ImageBrowserForm->addImage(
					theFile->getID(), theFile->getDownloadFile( theItem )
				))
				{
					numImages++;
				}
			}
			else
			{
				if( ImageBrowserForm->addImage( theFile	) )
				{
					numImages++;
				}
			}
		}
		else if( theRemoteFile = *it )
		{
			if( ImageBrowserForm->addImage(
				theRemoteFile->getID(), theRemoteFile->download()
			))
			{
				numImages++;
			}
		}
	}
	if( numImages )
	{
		ImageBrowserForm->setParentItem( theItem );
		ImageBrowserForm->Caption = (const char *)title;
		ImageBrowserForm->Show();
		ImageBrowserForm->SetFocus();
		ImageBrowserForm->BringToFront();
	}
	else
	{
		Application->MessageBox(
			"No Images found",
			"Image Browser",
			MB_OK|MB_ICONSTOP
		);
		delete ImageBrowserForm;
	}

	return REFRESH_NONE;
}
//---------------------------------------------------------------------------
__fastcall TImageBrowserForm::TImageBrowserForm(TComponent* Owner)
	: TForm(Owner)
{
	maxSize = 150;
	lastRow = lastCol = 0;
}
//---------------------------------------------------------------------------

void __fastcall TImageBrowserForm::FormCreate(TObject *)
{
	::SetWindowLong( Handle, GWL_HWNDPARENT, NULL );

	TPicture::RegisterFileFormat( NULL, "JPG", "JPEG Files", __classid( TJPEGImage ) );
	TPicture::RegisterFileFormat( NULL, "BMP", "Bitmaps", __classid( Graphics::TBitmap ) );
}
//---------------------------------------------------------------------------
static void scaleImage( TImage *Image, int maxSize )
{
	int imgWidth = Image->Picture->Width;
	int imgHeight = Image->Picture->Height;

	if( imgWidth > 0 && imgHeight > 0 )
	{
		double	ratio = (double)imgWidth/(double)imgHeight;

		double newWidth = maxSize;
		double newHeight = newWidth / ratio;

		if( newHeight > maxSize )
		{
			newHeight = maxSize;
			newWidth = newHeight * ratio;
		}

		Image->Top += maxSize-newHeight;
		Image->Left += (maxSize-newWidth)/2;
		Image->Width = newWidth;
		Image->Height = newHeight;
	}
}
//---------------------------------------------------------------------------
void __fastcall TImageBrowserForm::ItemClick( TObject *Sender )
{
	ThumbNailViewer theViewer = thumbNails[((TImage*)Sender)->Tag];
	PTR_ITEM		parentItem = this->parentItem;

	if( !parentItem )
	{
		PTR_ITEM	theItem = getItem( theViewer.itemID );
		parentItem = (*theItem).getParent();
	}

	if( parentItem )
	{
		DocManMainForm->openItem( parentItem, theViewer.itemID );
		DocManMainForm->SetFocus();
		DocManMainForm->BringToFront();

	}
}
//---------------------------------------------------------------------------
bool TImageBrowserForm::addImage( const PTR_FILE &theFile )
{
	bool added = false;

	STRING	mimeType = (*theFile).getMimeType();
	if( mimeType == "image/jpeg" || mimeType == "image/bmp" )
	{
		STRING	storageFile = (*theFile).getExternalFile();
		TGraphic *theImage = mimeType == "image/jpeg"
			? (TGraphic *)new TJPEGImage()
			: (TGraphic *)new Graphics::TBitmap()
		;

		try
		{
			theImage->LoadFromFile( (const char *)storageFile );
			added = addImage( (*theFile).getID(), (*theFile).getName(), theImage );
		}
		catch( ... )
		{
			delete theImage;
		}
	}

	return added;
}

bool TImageBrowserForm::addImage( int itemID, const STRING &file, TGraphic *img )
{
	if( !img )
	{
		CI_STRING ext = file;
		ext += ext.searchRChar( '.' ) +1;
		if( ext != "JPG" && ext != "BMP" )
/*@*/		return false;
	}

	maxSize = TrackBar->Position;

	bool added = false;

	TImage	*Image = new TImage( this );
	Image->Parent = ScrollBox;
	Image->Width = maxSize;
	Image->Height = maxSize;
	Image->Stretch = true;

	Image->Left = lastCol * maxSize;
	Image->Top = lastRow * (maxSize + 20);

	if( img )
		Image->Picture->Graphic = img;
	else
	{
		try
		{
			Image->Picture->LoadFromFile( (const char *)file );
		}
		catch( ... )
		{
			delete Image;
			Image = NULL;
		}
	}

	if( Image )
	{
		int imgWidth = Image->Picture->Width;
		int imgHeight = Image->Picture->Height;

		if( imgWidth > 0 && imgHeight > 0 )
		{
			scaleImage( Image, maxSize );

			TLabel *Label = new TLabel( this );
			STRING	fileName = file;
			fileName += (file.searchRChar( DIRECTORY_DELIMITER ) +1);
			Label->Caption = (const char *)fileName;
			Label->Parent = ScrollBox;
			Label->Width = maxSize;
			Label->Height = 20;
			Label->Left = lastCol * maxSize;
			Label->Top = lastRow * (maxSize + 20) + maxSize;

			if( (lastCol+2) * maxSize > ScrollBox->Width )
			{
				lastCol = 0;
				lastRow++;
			}
			else
				lastCol++;

			Image->OnClick = ItemClick;
			Image->Tag = thumbNails.size();
			ThumbNailViewer newViewer;
			newViewer.itemID = itemID;
			newViewer.Image = Image;
			newViewer.Label = Label;
			thumbNails.addElement( newViewer );
			added = true;
		}
	}

	return added;
}
//---------------------------------------------------------------------------

void __fastcall TImageBrowserForm::ScrollBoxResize(TObject *)
{
	lastRow = lastCol = 0;
	for(
		ThumbNailViewers::const_iterator it = thumbNails.cbegin(),
			endIT = thumbNails.cend();
		it != endIT;
		++it )
	{
		const ThumbNailViewer &theViewer = *it;
		TImage	*Image = theViewer.Image;
		TLabel	*Label = theViewer.Label;

		Image->Left = lastCol * maxSize;
		Image->Top = lastRow * (maxSize + 20);

		scaleImage( Image, maxSize );

		Label->Left = lastCol * maxSize;
		Label->Top = lastRow * (maxSize + 20) + maxSize;
		Label->Width = maxSize;

		if( (lastCol+2) * maxSize > ScrollBox->Width )
		{
			lastCol = 0;
			lastRow++;
		}
		else
		{
			lastCol++;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TImageBrowserForm::TrackBarChange(TObject *Sender)
{
	maxSize = TrackBar->Position;
	ScrollBoxResize( Sender );
}
//---------------------------------------------------------------------------
void __fastcall TImageBrowserForm::FormClose(TObject *,
	  TCloseAction &Action)
{
	Action = caFree;
}
//---------------------------------------------------------------------------

