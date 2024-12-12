//---------------------------------------------------------------------------

#include <vcl.h>

#include <gak/exif.h>
#include <graphix/magic.h>
#include <gak/fmtNumber.h>

#pragma hdrstop

#include "ExifFrm.h"
#include "DocManDM.h"
#include "ActionManager.h"
#include "File.h"

//---------------------------------------------------------------------------
using namespace gak;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TExifForm *ExifForm;
//---------------------------------------------------------------------------
class ACTION_EXIF : public ACTION_BASE_PROPERTIES
{
	virtual bool acceptItem( THE_ITEM *theItem );
	virtual REFRESH_TYPE perform( PTR_ITEM theItem );
	virtual const char *getLabel( void ) const;
};
//---------------------------------------------------------------------------
bool ACTION_EXIF::acceptItem( THE_ITEM *theItem )
{
	THE_FILE *theFile = dynamic_cast<THE_FILE*>(theItem);
	if( theFile )
		return true;
	THE_FILE_REF *theFileRef = dynamic_cast<THE_FILE_REF*>(theItem);
	if( theFileRef )
		return true;

	return false;
}
//---------------------------------------------------------------------------
REFRESH_TYPE ACTION_EXIF::perform( PTR_ITEM theItem )
{
	ExifForm->setItem( theItem, 0 );
	ExifForm->ShowModal();
	return REFRESH_NONE;
}
//---------------------------------------------------------------------------
const char *ACTION_EXIF::getLabel( void ) const
{
	return "Exif...";
}
//---------------------------------------------------------------------------
static ACTION_EXIF theAction;
//---------------------------------------------------------------------------
__fastcall TExifForm::TExifForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TExifForm::FormShow(TObject *)
{
	PTR_FILE	theFile = theItem;
	bool		success = false;
	ImageMetaData	metaData;

	if( (THE_FILE*)theFile )
	{
		success = (*theFile).updateImageMetaData( &metaData, theVersion );
	}
	else
	{
		PTR_FILE_REF theFileRef = theItem;
		if( (THE_FILE_REF*)theFileRef )
			success = (*theFileRef).updateImageMetaData( &metaData );
	}
	if( success )
	{
		size_t		i=0;
		STRING		exif;

		StringGrid->Cells[1][i++] = (const char *)metaData.tiffData.Manufacturer;
		StringGrid->Cells[1][i++] = (const char *)metaData.tiffData.Model;
		StringGrid->Cells[1][i++] = (const char *)metaData.getLens();

		exif = "";
		if( metaData.exifData.FNumber.numerator > 0
		&&  metaData.exifData.FNumber.denominator > 0)
		{
			double FNumber = metaData.exifData.FNumber.getAsDouble();
			exif = formatFloat( FNumber, 0, 1 );
		}
		StringGrid->Cells[1][i++] = (const char *)exif;

		exif = "";
		if( metaData.exifData.ExposureTime.numerator > 0
		&&  metaData.exifData.ExposureTime.denominator > 0)
		{
			double ExposureTime = metaData.exifData.ExposureTime.getAsDouble();
			if(
				metaData.exifData.ExposureTime.numerator >=
				metaData.exifData.ExposureTime.denominator
			)
			{
				exif = formatFloat( ExposureTime, 0, 3 );
			}
			else
			{
				exif = formatNumber(
					metaData.exifData.ExposureTime.numerator
				);
				exif += '/';
				exif = formatNumber(
					metaData.exifData.ExposureTime.denominator
				);
				exif += " (";
				exif += formatFloat( ExposureTime, 0, 3 );
				exif += ')';
			}
		}
		StringGrid->Cells[1][i++] = (const char *)exif;

		exif = "";
		long ISO = metaData.getISO();
		if( ISO > 0 )
			exif = formatNumber( ISO );
		StringGrid->Cells[1][i++] = (const char *)exif;


		exif = "";
		if( metaData.exifData.ExposureBiasValue.denominator )
		{
			metaData.exifData.ExposureBiasValue.reduce();

			long numerator = metaData.exifData.ExposureBiasValue.numerator;
			long denominator = metaData.exifData.ExposureBiasValue.denominator;

			if( numerator < 0 && denominator < 0 )
			{
				numerator = abs( numerator );
				denominator = abs( denominator );
			}
			else if( numerator < 0 )
			{
				exif += '-';
				numerator = abs( numerator );
			}
			else if( denominator < 0 )
			{
				exif += '-';
				denominator = abs( denominator );
			}
			short ExposureBiasValue = short(numerator / denominator);
			if( ExposureBiasValue )
			{
				exif += formatNumber( ExposureBiasValue );
				exif += ' ';
			}
			ExposureBiasValue = short(numerator % denominator);
			if( ExposureBiasValue )
			{
				exif += formatNumber( ExposureBiasValue );
				exif += '/';
				exif += formatNumber( denominator );
			}

			if( exif.isEmpty() )
				exif = "0";
		}
		StringGrid->Cells[1][i++] = (const char *)exif;

		StringGrid->Cells[1][i++] = metaData.getExposureProgram();
		StringGrid->Cells[1][i++] = metaData.getMeteringMode();

		exif = "";
		if( metaData.exifData.Flash > 0 )
		{
			if( !metaData.getFlashFired() )
				exif = "Off";
			else if( metaData.getRedEyeMode() )
				exif = "Red-Eye";
			else
				exif = "On";
		}
		StringGrid->Cells[1][i++] = (const char *)exif;

		exif = "";
		if( metaData.exifData.FocalLength.numerator > 0
		&&  metaData.exifData.FocalLength.denominator > 0)
		{
			long focalLength = metaData.exifData.FocalLength.getAsLong();
			exif = formatNumber( focalLength );
		}
		StringGrid->Cells[1][i++] = (const char *)exif;

		if( !metaData.exifData.DateTimeOriginal.isEmpty() )
		{
			try
			{
				TDateTime DateTimeOriginal = parseExifTimestamp( metaData.exifData.DateTimeOriginal );
				StringGrid->Cells[1][i] = DateTimeOriginal.DateTimeString();
			}
			catch( ... )
			{
				// ignore
			}
		}
		i++;

		StringGrid->Cells[1][i++] = (const char *)metaData.tiffData.Software;
		StringGrid->Cells[1][i++] = (const char *)metaData.tiffData.Artist;
		StringGrid->Cells[1][i++] = (const char *)metaData.tiffData.Copyright;
		StringGrid->Cells[1][i++] = (const char *)metaData.exifData.UserComment;
		StringGrid->Cells[1][i++] = (const char *)metaData.tiffData.Description;
	}
	else
	{
		StringGrid->Cells[1][0] = "No exif data";
		for( size_t i=1; i<StringGrid->RowCount; i++ )
			StringGrid->Cells[1][i] = "";
	}
}
//---------------------------------------------------------------------------

void __fastcall TExifForm::FormCreate(TObject *)
{
	size_t i=0;

	StringGrid->Cells[0][i++] = "Manufacturer";
	StringGrid->Cells[0][i++] = "Camera";
	StringGrid->Cells[0][i++] = "Lens";
	StringGrid->Cells[0][i++] = "FNumber";
	StringGrid->Cells[0][i++] = "Exposure Time";
	StringGrid->Cells[0][i++] = "ISO Speed";
	StringGrid->Cells[0][i++] = "Exposure Compensation";
	StringGrid->Cells[0][i++] = "Exposure Program";
	StringGrid->Cells[0][i++] = "Metering Mode";
	StringGrid->Cells[0][i++] = "Flash";
	StringGrid->Cells[0][i++] = "Focal Length";
	StringGrid->Cells[0][i++] = "Original Date Time";
	StringGrid->Cells[0][i++] = "Software";
	StringGrid->Cells[0][i++] = "Artist";
	StringGrid->Cells[0][i++] = "Copyright";
	StringGrid->Cells[0][i++] = "User Comment";
	StringGrid->Cells[0][i++] = "Image Description";

	StringGrid->ColWidths[0] = 130;
	StringGrid->ColWidths[1] = StringGrid->Width - 130;
}
//---------------------------------------------------------------------------

