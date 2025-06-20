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

#include <fstream>

#include <vcl.h>

#include <winlib/winlib.h>

#include <gak/vcl_tools.h>
#include <gak/md5.h>
#include <gak/directory.h>

#pragma hdrstop

#include "DocManDM.h"
#include "File.h"
#include "MimeTypesFrm.h"
#include "LoginDlg.h"
#include "PasswordDlg.h"

//---------------------------------------------------------------------------
using namespace gak;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDocManDataModule *DocManDataModule;
char registryKey[] = "\\Software\\gak\\DocMan";
//---------------------------------------------------------------------------

const char *THREAD_CHECK_DB::getTitle( void ) const
{
	return "Check Database";
}

//---------------------------------------------------------------------------
void THREAD_CHECK_DB::perform( void )
{
	DocManDataModule->checkDB( false );
}
//---------------------------------------------------------------------------
const char *THREAD_REFRESH_EXIFS::getTitle( void ) const
{
	return "Refresh Exifs";
}

//---------------------------------------------------------------------------
void THREAD_REFRESH_EXIFS::perform( void )
{
	DocManDataModule->refreshExifs();
}
//---------------------------------------------------------------------------
const char *THREAD_REFRESH_MIME_TYPES::getTitle( void ) const
{
	return "Refresh Mime Types";
}

//---------------------------------------------------------------------------
void THREAD_REFRESH_MIME_TYPES::perform( void )
{
	DocManDataModule->refreshMimeTypes();
}

//---------------------------------------------------------------------------
__fastcall TDocManDataModule::TDocManDataModule(TComponent* Owner)
	: TDataModule(Owner)
{
	defaultTaskType = defaultTaskStatus = 0;
}
//---------------------------------------------------------------------------

#if 0
STRING TDocManDataModule::md5Str( unsigned char md5[16] )
{
	ArrayOfData	md5Array;

	md5Array.addElements( (char *)md5, 16 );
	STRING md5Base64 = encodeBase64( md5Array );

	return md5Base64;
}
#endif

STRING TDocManDataModule::md5file( const STRING &filePath )
{
	ArrayOfData		md5Array;
	unsigned char	fileMD5[16];
	const int 		err = md5_file_ptr( strFopen(filePath,"rb"), fileMD5 );
	if( err == 1 )
	{
		throw OpenReadError(filePath);
	}
	if( err )
	{
		throw ReadError(filePath);
	}
	md5Array.addElements( (char *)fileMD5, 16 );
	STRING md5Base64 = encodeBase64( md5Array );

	return md5Base64;
}

//---------------------------------------------------------------------------
void TDocManDataModule::checkDB( bool silent )
{
	STRING 			md5Base64;

	STRING			errorLog = Session->PrivateDir.c_str();
	STRING			externalStorage = THE_FILE::getExternalStorageBase();
	STRING			externalFile, status;
	int				usageCount, maxVersion;
	bool			hasError;
	size_t			errorCount = 0;

	std::auto_ptr<TQuery> ucCheckQuery( new TQuery( NULL ) );
	ucCheckQuery->DatabaseName = "docManDB";
	std::auto_ptr<TQuery> versionCheckQuery( new TQuery( NULL ) );
	versionCheckQuery->DatabaseName = "docManDB";
	std::auto_ptr<TQuery> parentCheckQuery( new TQuery( NULL ) );
	parentCheckQuery->DatabaseName = "docManDB";
	std::auto_ptr<TQuery> fileCheckQuery( new TQuery( NULL ) );
	fileCheckQuery->DatabaseName = "docManDB";

	std::auto_ptr<TQuery> findStorageQuery( new TQuery( NULL ) );
	findStorageQuery->DatabaseName = "docManDB";
	std::auto_ptr<TQuery> findFileQuery( new TQuery( NULL ) );
	findFileQuery->DatabaseName = "docManDB";

	PTR_ITEM theCompanyVolume = getCompanyVolume();

	ucCheckQuery->SQL->Add(
		"select count(*) from i_file_vers where storage_id=:theStorage"
	);
//	ucCheckQuery->Prepare();

	findStorageQuery->SQL->Add(
		"select it.ID "
		"from item_tree it, i_file_vers fv "
		"where it.file_id = fv.file_id and fv.storage_id=:theStorage"

	);

	findFileQuery->SQL->Add(
		"select it.ID "
		"from item_tree it "
		"where it.file_id =  :theFileID"

	);

	errorLog += DIRECTORY_DELIMITER_STRING "checkDB.log";

	std::ofstream	fp( static_cast<const char *>(errorLog) );

	for(
		TableIstorage->Open();
		!TableIstorage->Eof;
		TableIstorage->Next()
	)
	{
		hasError = false;
		externalFile = externalStorage;
		externalFile += TableIstorageFILE_PATH->AsString.c_str();

		status = externalFile;
		if( errorCount )
		{
			status += ' ';
			status += formatNumber( errorCount );
		}

		if( StatusForm->setStatus( "Checking", status	) )
			break;

#ifndef _DEBUG
		if( silent && StatusForm->waitForUserSleep( 60000 ) )
			break;
#endif

		if( !isFile( externalFile ) )
		{
			fp << "Missing " << externalFile << '\n';
			hasError = true;
			errorCount++;
		}

		if( !hasError )
		{
			try
			{
				md5Base64 = md5file( externalFile );

				if( TableIstorageMD5_CHECKSUM->AsString.Length() == 0 )
				{
					TableIstorage->Edit();
					TableIstorageMD5_CHECKSUM->AsString = (const char *)md5Base64;
					TableIstorage->Post();
				}
				else if( md5Base64 != TableIstorageMD5_CHECKSUM->AsString.c_str() )
				{
					fp << "Bad checksum " << externalFile << '\n';
					errorCount++;
					hasError = true;
				}
			}
			catch( std::exception &e )
			{
				fp << "Cannot read checksum " << externalFile << ": " << e.what() << '\n';
				errorCount++;
				hasError = true;
			}
		}

		if( !TableIstorageUSAGE_COUNT->AsInteger )
		{
			fp << "No usage for " << externalFile << '\n';
			errorCount++;
			hasError = true;
		}

		try
		{
			usageCount = -1;
			ucCheckQuery->Params->Items[0]->AsInteger = TableIstorageID->AsInteger;
			ucCheckQuery->Open();
			if( !ucCheckQuery->Eof )
				usageCount = ucCheckQuery->Fields->Fields[0]->AsInteger;
			ucCheckQuery->Close();

			if( usageCount != TableIstorageUSAGE_COUNT->AsInteger )
			{
				STRING error = "Bad usage count for ";
				error += externalFile;
				error += ". expected ";
				error += formatNumber(
					TableIstorageUSAGE_COUNT->AsInteger
				);
				error += " found ";
				error += formatNumber( usageCount );

				fp << error << '\n';
				errorCount++;
				hasError = true;
			}
		}
		catch( Exception &e )
		{
			STRING error = "Failed to determine usage count for ";
			error += externalFile;
			error += ". expected ";
			error += formatNumber( TableIstorageUSAGE_COUNT->AsInteger );
			error += ". ";
			error += e.Message.c_str();

			fp << error << '\n';
			errorCount++;
			hasError = true;
		}

		if( hasError )
		{
			findStorageQuery->Params->Items[0]->AsInteger = TableIstorageID->AsInteger;
			for(
				findStorageQuery->Open();
				!findStorageQuery->Eof;
				findStorageQuery->Next()
			)
			{
				PTR_ITEM theItem = getItem( findStorageQuery->Fields->Fields[0]->AsInteger );
				if( theItem )
				{
					fp << "Storage used for " << (*theItem).getPath() << '\n';
				}
			}
			findStorageQuery->Close();

		}
	}
	TableIstorage->Close();

	ucCheckQuery->SQL->Clear();
	ucCheckQuery->SQL->Add(
		"select count(*) from item_tree where file_id=:theFileId"
	);
//	ucCheckQuery->Prepare();

	versionCheckQuery->SQL->Add(
		"select max( version ) from i_file_vers where file_id = :theFileId"
	);
//	versionCheckQuery->Prepare();

	for(
		TableIfiles->Open();
		!TableIfiles->Eof;
		TableIfiles->Next()
	)
	{
		hasError = false;
		externalFile = "File ID ";
		externalFile += formatNumber( TableIfilesID->AsInteger );

		status = externalFile;
		if( errorCount )
		{
			status += ' ';
			status += formatNumber( errorCount );
		}

		if( StatusForm->setStatus( "Checking", status	) )
			break;

#ifndef _DEBUG
		if( silent && StatusForm->waitForUserSleep( 60000 ) )
			break;
#endif

		if( !TableIfilesUSAGECOUNT->AsInteger )
		{
			fp << "No usage for " << externalFile << '\n';
			errorCount++;
			hasError = true;
		}

		try
		{
			usageCount = -1;
			ucCheckQuery->Params->Items[0]->AsInteger = TableIfilesID->AsInteger;
			ucCheckQuery->Open();
			if( !ucCheckQuery->Eof )
				usageCount = ucCheckQuery->Fields->Fields[0]->AsInteger;
			ucCheckQuery->Close();

			if( usageCount != TableIfilesUSAGECOUNT->AsInteger )
			{
				fp <<
					"Bad usage count for " <<
					externalFile <<
					". Expected " <<
					TableIfilesUSAGECOUNT->AsInteger <<
					" found " <<
					usageCount <<
					'\n'
				;
				errorCount++;
				hasError = true;
			}
		}
		catch( Exception &e )
		{
			fp <<
				"Failed to determine usage count for " <<
				externalFile <<
				". Expected " <<
				TableIfilesUSAGECOUNT->AsInteger <<
				". " <<
				e.Message.c_str() <<
				'\n'
			;
			errorCount++;
			hasError = true;
		}

		try
		{
			maxVersion = -1;
			versionCheckQuery->Params->Items[0]->AsInteger = TableIfilesID->AsInteger;
			versionCheckQuery->Open();
			if( !versionCheckQuery->Eof )
				maxVersion = versionCheckQuery->Fields->Fields[0]->AsInteger;
			versionCheckQuery->Close();
			if( maxVersion != TableIfilesVERSION->AsInteger )
			{
				fp <<
					"Bad version for " <<
					externalFile <<
					". Expected " <<
					TableIfilesVERSION->AsInteger <<
					" found " <<
					maxVersion <<
					'\n'
				;
				errorCount++;
				hasError = true;
			}
		}
		catch( Exception &e )
		{
			fp <<
				"Failed to determine version for " <<
				externalFile <<
				". Expected " <<
				TableIfilesVERSION->AsInteger <<
				". " <<
				e.Message.c_str() <<
				'\n'
			;
			errorCount++;
			hasError = true;
		}

		if( hasError )
		{
			findFileQuery->Params->Items[0]->AsInteger = TableIfilesID->AsInteger;
			for(
				findFileQuery->Open();
				!findFileQuery->Eof;
				findFileQuery->Next()
			)
			{
				PTR_ITEM theItem = getItem( findFileQuery->Fields->Fields[0]->AsInteger );
				if( theItem )
				{
					fp << "File ID used for " << (*theItem).getPath() << '\n';
				}
			}
			findFileQuery->Close();
		}
	}
	TableIfiles->Close();

	parentCheckQuery->SQL->Add(
		"select count(*) from item_tree where id = :theParent"
	);
	fileCheckQuery->SQL->Add(
		"select count(*) from i_files where id = :theFileId"
	);

	for(
		TableItemTree->Open();
		!TableItemTree->Eof;
		TableItemTree->Next()
	)
	{
		externalFile = "Item ";
		externalFile += TableItemTreeNAME->AsString.c_str();
		externalFile += '(';
		externalFile += formatNumber( TableItemTreeID->AsInteger );
		externalFile += ')';

		status = externalFile;
		if( errorCount )
		{
			status += ' ';
			status += formatNumber( errorCount );
		}

		if( StatusForm->setStatus( "Checking", status	) )
			break;

#ifndef _DEBUG
		if( silent && StatusForm->waitForUserSleep( 60000 ) )
			break;
#endif

		if( TableItemTreePARENTID->AsInteger != -1 )
		{
			try
			{
				usageCount = 0;
				parentCheckQuery->Params->Items[0]->AsInteger =
					TableItemTreePARENTID->AsInteger
				;
				parentCheckQuery->Open();
				if( !parentCheckQuery->Eof )
					usageCount = parentCheckQuery->Fields->Fields[0]->AsInteger;
				parentCheckQuery->Close();

				if( usageCount != 1 )
				{
					fp << "Bad parent for " << externalFile << " - moved to company folder\n";
					errorCount++;
					TableItemTree->Edit();
					TableItemTreePARENTID->AsInteger = (*theCompanyVolume).getID();
					externalFile = TableItemTreeNAME->AsString.c_str();
					externalFile += '(';
					externalFile += formatNumber(
						TableItemTreeID->AsInteger
					);
					externalFile += ')';
					TableItemTreeNAME->AsString = (const char *)externalFile;
					TableItemTree->Post();
				}
			}
			catch( Exception &e )
			{
				fp <<
					"Failed to determine parent for " <<
					externalFile <<
					". " <<
					e.Message.c_str() <<
					'\n'
				;
				errorCount++;
			}
		}
		if( TableItemTreeFILE_ID->AsInteger != 0 )
		{
			try
			{
				usageCount = 0;
				fileCheckQuery->Params->Items[0]->AsInteger =
					TableItemTreeFILE_ID->AsInteger
				;
				fileCheckQuery->Open();
				if( !fileCheckQuery->Eof )
					usageCount = fileCheckQuery->Fields->Fields[0]->AsInteger;
				fileCheckQuery->Close();

				if( usageCount != 1 )
				{
					fp << "Bad fileid for " << externalFile << '\n';
					errorCount++;
				}
			}
			catch( Exception &e )
			{
				fp <<
					"Failed to determine fileid for " <<
					externalFile <<
					". " <<
					e.Message.c_str() <<
					'\n'
				;
				errorCount++;
			}
		}

		if( TableItemTreeITEM_TYPE->AsInteger == TYPE_FILE_REF )
		{
			PTR_FILE_REF theFile = getItem( TableItemTreeID->AsInteger );
			if( theFile )
			{
				externalFile = theFile->getDownloadPath();
				if( !externalFile.isEmpty() )
				{
					DirectoryEntry	externalEntry;
					externalEntry.findFile( externalFile );

					DateTime	dbTime = TableItemTreeMODIFIEDDATE->AsDateTime;
					if( dbTime == externalEntry.modifiedDate )
					{
						try
						{
							md5Base64 = md5file( externalFile );

							if( TableItemTreeMD5_CHECKSUM->AsString.Length() == 0 )
							{
								TableItemTree->Edit();
								TableItemTreeMD5_CHECKSUM->AsString = (const char *)md5Base64;
								TableItemTree->Post();
							}
							else if( md5Base64 != TableItemTreeMD5_CHECKSUM->AsString.c_str() )
							{
								fp << "Bad checksum " << externalFile << '\n';
								errorCount++;
							}
						}
						catch( std::exception &e )
						{
							fp << "Cannot read checksum " << externalFile << ": " << e.what() << '\n';
							errorCount++;
							hasError = true;
						}
					}
				}
			}
			else
			{
				fp <<
					"Failed to load data for " <<
					externalFile <<
					".\n"
				;
				errorCount++;
			}
		}
	}
	fp.close();

	if( errorCount )
		ShellExecute( NULL, NULL, errorLog, NULL, NULL, SW_SHOW );
	else if( !silent )
		Application->MessageBox( "No errors", "Doc Man", MB_OK|MB_ICONINFORMATION );
}
//---------------------------------------------------------------------------
void __fastcall TDocManDataModule::TableTaskTypesBeforePost(
	  TDataSet *)
{
	if( TableTaskTypesID->IsNull )
	{
		vcl::setNewMaxValue( TableTaskTypes, "ID" );
	}
}
//---------------------------------------------------------------------------
void TDocManDataModule::refreshTaskTypes( TStrings *items )
{
	taskTypeList.clear();
	defaultTaskType = 0;
	for(
		TableTaskTypes->Open();
		!TableTaskTypes->Eof;
		TableTaskTypes->Next()
	)
	{
		if( !defaultTaskType || TableTaskTypesDEFAULT_TYPE->AsInteger )
			defaultTaskType = TableTaskTypesID->AsInteger;

		taskTypeList[TableTaskTypesID->AsInteger] =
			TableTaskTypesTASK_TYPE->AsString.c_str()
		;
		if( items )
			items->Add( TableTaskTypesTASK_TYPE->AsString );
	}
	TableTaskTypes->Close();
}

//---------------------------------------------------------------------------
void TDocManDataModule::refreshTaskStatus( TStrings *items )
{
	taskStatusList.clear();
	defaultTaskStatus = 0;
	for(
		TableTaskStatus->Open();
		!TableTaskStatus->Eof;
		TableTaskStatus->Next()
	)
	{
		if( !defaultTaskStatus || TableTaskStatusDEFAULT_STATUS->AsInteger )
			defaultTaskStatus = TableTaskStatusID->AsInteger;

		taskStatusList[TableTaskStatusID->AsInteger] =
			TableTaskStatusTASK_STATUS->AsString.c_str()
		;
		if( items )
			items->Add( TableTaskStatusTASK_STATUS->AsString );
	}
	TableTaskStatus->Close();
}

//---------------------------------------------------------------------------
STRING TDocManDataModule::getPathForID( int theID )
{
	doEnterFunctionEx(gakLogging::llDetail, "TDocManDataModule::getPathForID");
	STRING thePath = idPathMapping[theID];

	if( thePath.isEmpty() )
	{
		PTR_ITEM	theItem = getItem( theID );
		if( theItem )
			thePath = theItem->getPath();
		else
		{
			thePath = "???";
			idPathMapping[theID] = thePath;
		}
	}

	return thePath;
}
//---------------------------------------------------------------------------
bool TDocManDataModule::updateExifData( const ImageMetaData *metaData )
{
	bool dataOK = false;

	if( !metaData->tiffData.Manufacturer.isEmpty() )
	{
		TableIexifDataMANUFACTURER->AsString = (const char *)metaData->tiffData.Manufacturer;
		dataOK = true;
	}
	if( !metaData->tiffData.Model.isEmpty() )
	{
		TableIexifDataCAMERA->AsString = (const char *)metaData->tiffData.Model;
		dataOK = true;
	}
	STRING	lens = metaData->getLens();
	if( !lens.isEmpty() )
	{
		TableIexifDataLENS->AsString = (const char *)lens;
		dataOK = true;
	}
	if( metaData->exifData.ISOSpeed > 0
	||  metaData->exifData.PhotographicSensitivity > 0 )
	{
		if( metaData->exifData.ISOSpeed > 0 )
			TableIexifDataISO_SPEED->AsInteger = metaData->exifData.ISOSpeed;
		else
			TableIexifDataISO_SPEED->AsInteger = metaData->exifData.PhotographicSensitivity;
		dataOK = true;
	}
	if( metaData->exifData.FNumber.numerator > 0
	&&  metaData->exifData.FNumber.denominator > 0)
	{
		TableIexifDataFNUMBER->AsFloat = metaData->exifData.FNumber.getAsDouble();
		dataOK = true;
	}
	if( metaData->exifData.ExposureTime.numerator > 0
	&&  metaData->exifData.ExposureTime.denominator > 0)
	{
		TableIexifDataEXPOSURE_TIME->AsFloat = metaData->exifData.ExposureTime.getAsDouble();
		dataOK = true;
	}
	if( metaData->exifData.FocalLength.numerator > 0
	&&  metaData->exifData.FocalLength.denominator > 0)
	{
		TableIexifDataFOCAL_LENGTH->AsInteger = metaData->exifData.FocalLength.getAsLong();
		dataOK = true;
	}

	if( !metaData->tiffData.Software.isEmpty() )
	{
		TableIexifDataSOFTWARE->AsString = (const char *)metaData->tiffData.Software;
		dataOK = true;
	}

	if( !metaData->tiffData.Artist.isEmpty() )
	{
		TableIexifDataARTIST->AsString = (const char *)metaData->tiffData.Artist;
		dataOK = true;
	}

	if( !metaData->tiffData.Copyright.isEmpty() )
	{
		TableIexifDataCOPYRIGHT->AsString = (const char *)metaData->tiffData.Copyright;
		dataOK = true;
	}

	if( !metaData->exifData.UserComment.isEmpty() )
	{
		TableIexifDataUSERCOMMENT->AsString = (const char *)metaData->exifData.UserComment;
		dataOK = true;
	}

	if( !metaData->tiffData.Description.isEmpty() )
	{
		TableIexifDataIMAGE_DESCRIPTION->AsString = (const char *)metaData->tiffData.Description;
		dataOK = true;
	}


	if( metaData->exifData.ExposureProgram >= 0 )
	{
		TableIexifDataEXPOSURE_PROGRAM->AsInteger = metaData->exifData.ExposureProgram;
		dataOK = true;
	}
	if( metaData->exifData.MeteringMode >= 0 )
	{
		TableIexifDataMETERING_MODE->AsInteger = metaData->exifData.MeteringMode;
		dataOK = true;
	}

	if( metaData->exifData.ExposureBiasValue.denominator )
	{
		TableIexifDataEXPOSURE_COMPENSATION->AsFloat = metaData->exifData.ExposureBiasValue.getAsDouble();
		dataOK = true;
	}

	if( !metaData->exifData.DateTimeOriginal.isEmpty() )
	{
		try
		{
			TableIexifDataORIGINAL_DATE_TIME->AsDateTime = parseExifTimestamp( metaData->exifData.DateTimeOriginal );
			dataOK = true;
		}
		catch( ... )
		{
			// ignore errors
		}
	}

	if( metaData->exifData.Flash > 0 )
	{
		TableIexifDataFLASH->AsString = metaData->getFlashFired() ? "T" : "F";
		TableIexifDataRED_EYE->AsString = metaData->getRedEyeMode() ? "T" : "F";
		dataOK = true;
	}

	if( dataOK )
		TableIexifData->Post();
	else
		TableIexifData->Cancel();

	return dataOK;
}
//---------------------------------------------------------------------------
bool TDocManDataModule::updateExifData( const PTR_ITEM &theItem, int version, const ImageMetaData *metaData )
{
	doEnterFunctionEx(gakLogging::llInfo, "TDocManDataModule::updateExifData");
	int itemID = (*theItem).getID();

	if( !version )
	{
		PTR_FILE theFile = theItem;
		if( (THE_FILE*)theFile )
			version = (*theFile).getVersionNum();
	}

	STRING filter = "ITEM_ID=";
	filter += formatNumber( itemID );
	filter += " and version=";
	filter += formatNumber( version );

	TableIexifData->Filter = (const char *)filter;
	TableIexifData->Filtered = true;

	TableIexifData->Open();
	if( TableIexifDataITEM_ID->AsInteger != itemID || TableIexifDataVERSION->AsInteger != version )
	{
		TableIexifData->Insert();
		TableIexifDataITEM_ID->AsInteger = itemID;
		TableIexifDataVERSION->AsInteger = version;
	}
	else
		TableIexifData->Edit();

	bool dataOK = updateExifData( metaData );

	TableIexifData->Close();

	return dataOK;
}
//---------------------------------------------------------------------------
void TDocManDataModule::refreshExifs( void )
{
	doEnterFunctionEx(gakLogging::llInfo, "TDocManDataModule::refreshExifs");
	PTR_FILE		theFile;
	PTR_FILE_REF	theFileRef;
	ImageMetaData 	*metaData = NULL;
	int				itemID;
	int				version;
	bool			success;

#if 1
	PTR_ITEM		theItem;

	for(
		TableIexifData->Open();
		!TableIexifData->Eof;
		TableIexifData->Next()
	)
	{
		itemID = TableIexifDataITEM_ID->AsInteger;
		version = TableIexifDataVERSION->AsInteger;

		theItem = getItem( itemID );


		if( theItem )
		{
			if( StatusForm->setStatus( "Checking", (*theItem).getName() ) )
				break;

			theFile = theItem;
			if( theFile )
			{
				metaData = new ImageMetaData;
				success = theFile->loadImageMetaData( metaData, version );

			}
			else
			{
				theFileRef = theItem;
				if( theFileRef )
				{
					metaData = new ImageMetaData;
					success = theFileRef->loadImageMetaData( metaData );
				}
			}

			if( success )
			{
				TableIexifData->Edit();
				updateExifData( metaData );
			}
			if( metaData )
			{
				delete metaData;
				metaData = NULL;
			}
		}
	}
	TableIexifData->Close();

#else

	size_t	countRecs = 0;
	size_t	countVersions = 0;
	size_t	countFiles = 0;
	size_t	countImages = 0;
	STRING	status;

	TableIexifData->EmptyTable();

	TableIexifData->Open();
	for(
		QueryDocVersions->Open();
		!QueryDocVersions->Eof;
		QueryDocVersions->Next()
	)
	{
		countRecs++;
		itemID = QueryDocVersionsID->AsInteger;
		version = QueryDocVersionsVERSION->AsInteger;
		status = "";
		countRecs >> status;
		status += '/';
		countVersions >> status;
		status += '/';
		countFiles >> status;
		status += '/';
		countImages >> status;
		status += "/ ";
		status += QueryDocVersionsNAME->AsString.c_str();
		if( StatusForm->setStatus( "Checking", status ) )
			break;

		if( QueryDocVersionsITEM_TYPE->AsInteger == TYPE_DOCUMENT )
		{
			countVersions++;
			theFile = getItem( itemID );
			if( theFile )
			{
				metaData = new ImageMetaData;
				success = (*theFile).loadImageMetaData( metaData, version );
				doLogValue( "%d", (int)success );
			}
		}
		else if( QueryDocVersionsITEM_TYPE->AsInteger == TYPE_FILE_REF )
		{
			countFiles++;
			theFileRef = getItem( itemID );
			if( theFileRef )
			{
				metaData = new ImageMetaData;
				success = (*theFileRef).loadImageMetaData( metaData );
				doLogValue( "%d", (int)success );
			}
		}
		else
			success = false;

		if( success )
		{
			countImages++;
			TableIexifData->Insert();
			TableIexifDataITEM_ID->AsInteger = itemID;
			TableIexifDataVERSION->AsInteger = version;
			updateExifData( metaData );
		}
		if( metaData )
		{
			delete metaData;
			metaData = NULL;
		}
	}
	QueryDocVersions->Close();
	TableIexifData->Close();


#endif
}
//---------------------------------------------------------------------------
void TDocManDataModule::refreshMimeTypes( void )
{
	doEnterFunctionEx(gakLogging::llInfo, "TDocManDataModule::refreshMimeTypes");
	STRING fileName, oldMimeType, newMimeType;
	for(
		TableIfileVersions->Open();
		!TableIfileVersions->Eof;
		TableIfileVersions->Next()
	)
	{
		fileName = TableIfileVersionsFILENAME->AsString.c_str();
		if( StatusForm->setStatus( "Checking", fileName ) )
			break;
		oldMimeType = TableIfileVersionsMIME_TYPE->AsString.c_str();
		newMimeType = MimeTypesForm->GetMimeType( fileName );
		if( oldMimeType != newMimeType )
		{
			TableIfileVersions->Edit();
			TableIfileVersionsMIME_TYPE->AsString = (const char *)newMimeType;
			TableIfileVersions->Post();
		}
	}
	TableIfileVersions->Close();
}
//---------------------------------------------------------------------------
void TDocManDataModule::loadAttributeValues(
	int itemId, int itemVersion, AttrDefinition *attrDef
)
{
	AnsiString	filter;

	QueryAttrValues->Params->Items[0]->AsInteger = itemId;
	QueryAttrValues->Params->Items[1]->AsInteger = itemVersion;

	filter.printf( "ATTR_ID=%d", attrDef->ID );
	QueryAttrValues->Filter = filter;
	QueryAttrValues->Filtered = true;

	for(
		QueryAttrValues->Open();
		!QueryAttrValues->Eof;
		QueryAttrValues->Next()
	)
	{
		AttributeValue	&val = attrDef->values.createElement();
		if( attrDef->type == ATTR_String )
		{
			val.value = QueryAttrValuesSTRING_VALUE->AsString.c_str();
		}
		else if( attrDef->type == ATTR_Integer )
		{
			if( !QueryAttrValuesINTEGER_VALUE->IsNull )
				val.value = (long)QueryAttrValuesINTEGER_VALUE->AsInteger;
		}
		else if( attrDef->type == ATTR_Number )
		{
			if( !QueryAttrValuesFLOAT_VALUE->IsNull )
				val.value = QueryAttrValuesFLOAT_VALUE->AsFloat;
		}
		else if( attrDef->type == ATTR_Date )
		{
			gak::Date	dateValue( QueryAttrValuesDATE_VALUE->AsDateTime );
			val.value = dateValue;
		}
		else if( attrDef->type == ATTR_Time )
		{
			gak::Time	timeValue( QueryAttrValuesDATE_VALUE->AsDateTime );
			val.value = timeValue;
		}

	}
	QueryAttrValues->Close();
}
//---------------------------------------------------------------------------
void TDocManDataModule::loadCategoryDefinition(
	int categoryId, int version, CategoryData *attrDefinition,
	int itemId, int itemVersion
)
{
	QueryCatAttributes->Params->Items[0]->AsInteger = categoryId;
	QueryCatAttributes->Params->Items[1]->AsInteger = version;

	attrDefinition->catID = categoryId;
	attrDefinition->version = version;
	attrDefinition->attrDefinition.clear();
	for(
		QueryCatAttributes->Open();
		!QueryCatAttributes->Eof;
		QueryCatAttributes->Next()
	)
	{
		AttrDefinition	&attrDef = attrDefinition->attrDefinition.createElement();;

		attrDef.ID = QueryCatAttributesID->AsInteger;
		attrDef.attrID = QueryCatAttributesATTR_ID->AsInteger;
		attrDef.name = QueryCatAttributesNAME->AsString.c_str();
		attrDef.type = AttributeType(QueryCatAttributesTYPE->AsInteger);
		attrDef.minCount = QueryCatAttributesMIN_COUNT->AsInteger;
		attrDef.maxCount = QueryCatAttributesMAX_COUNT->AsInteger;
		attrDef.required = QueryCatAttributesREQUIRED->AsInteger ? true : false;

		if( itemId )
			loadAttributeValues( itemId, itemVersion, &attrDef );
	}
	QueryCatAttributes->Close();

}
//---------------------------------------------------------------------------
void TDocManDataModule::getCategoryID(
	const STRING &categoryName, int *categoryId, int *categoryVersion
)
{
	int		id=0, version = 0;
	std::auto_ptr<TQuery>	checkCat( new TQuery( NULL ) );

	checkCat->DatabaseName = "docManDB";
	checkCat->SQL->Add(
		"select c.ID, max( d.category_version ) "
		"from categories c, c_attr_defs d "
		"where c.ID = d.category_id "
		"and c.name = :theCategory "
		"group by c.ID"
	);
	checkCat->Params->Items[0]->AsString = (const char *)categoryName;
	checkCat->Open();
	if( !checkCat->Eof )
	{
		id = checkCat->Fields->Fields[0]->AsInteger;
		version = checkCat->Fields->Fields[1]->AsInteger;
	}

	*categoryId = id;
	*categoryVersion = version;
}
//---------------------------------------------------------------------------
void TDocManDataModule::loadCategoryDefinition(
	const STRING &categoryName, CategoryData *attrDefinition,
	int itemId, int itemVersion
)
{
	int		id=0, version = 0;
	getCategoryID( categoryName, &id, &version );
	if( id && version )
	{
		attrDefinition->name = categoryName;
		loadCategoryDefinition(
			id, version, attrDefinition, itemId, itemVersion
		);
	}
}
//---------------------------------------------------------------------------

void TDocManDataModule::copyAttributeValues(
	int srcID, int srcVersion, int destID, int destVersion
)
{
	doEnterFunctionEx(gakLogging::llInfo, "TDocManDataModule::copyAttributeValues");

	QueryAttrValues->Params->Items[0]->AsInteger = srcID;
	QueryAttrValues->Params->Items[1]->AsInteger = srcVersion;
	QueryAttrValues->Filter = "";
	QueryAttrValues->Filtered = false;
	TableAttrValues->Open();

	for(
		QueryAttrValues->Open();
		!QueryAttrValues->Eof;
		QueryAttrValues->Next()
	)
	{
		TableAttrValues->Insert();
		TableAttrValuesITEM_ID->AsInteger = destID;
		TableAttrValuesITEM_VERSION->AsInteger = destVersion;
		TableAttrValuesATTR_ID->AsInteger = QueryAttrValuesATTR_ID->AsInteger;
		TableAttrValuesORDER_NR->AsInteger = QueryAttrValuesORDER_NR->AsInteger;
		if( !QueryAttrValuesDATE_VALUE->IsNull )
			TableAttrValuesDATE_VALUE->AsDateTime = QueryAttrValuesDATE_VALUE->AsDateTime;
		if( !QueryAttrValuesINTEGER_VALUE->IsNull )
			TableAttrValuesINTEGER_VALUE->AsInteger = QueryAttrValuesINTEGER_VALUE->AsInteger;
		if( !QueryAttrValuesFLOAT_VALUE->IsNull )
			TableAttrValuesFLOAT_VALUE->AsFloat = QueryAttrValuesFLOAT_VALUE->AsFloat;
		TableAttrValuesSTRING_VALUE->AsString = QueryAttrValuesSTRING_VALUE->AsString;
		vcl::setNewMaxValue( TableAttrValues, TableAttrValuesID );
		TableAttrValues->Post();
	}
	QueryAttrValues->Close();
	TableAttrValues->Close();
}
//---------------------------------------------------------------------------
void TDocManDataModule::loadCategoryData(
	int itemId, int itemVersion, CategoryDatas	*attrData
)
{
	QueryCategory->Params->Items[0]->AsInteger = itemId;
	QueryCategory->Params->Items[1]->AsInteger = itemVersion;
	for(
		QueryCategory->Open();
		!QueryCategory->Eof;
		QueryCategory->Next()
	)
	{
		CategoryData	&data = attrData->createElement();
		data.name = QueryCategoryNAME->AsString.c_str();
		loadCategoryDefinition(
			QueryCategoryID->AsInteger, QueryCategoryCATEGORY_VERSION->AsInteger,
			&data
		);
	}
	QueryCategory->Close();
}
//---------------------------------------------------------------------------
void TDocManDataModule::loadStorageInfos( int storageID, StorageInfos *result )
{
	QueryStorageItems->Params->Items[0]->AsInteger = storageID;
	for(
		QueryStorageItems->Open();
		!QueryStorageItems->Eof;
		QueryStorageItems->Next()
	)
	{
		StorageInfo	&info = result->createElement();
		info.itemID = QueryStorageItemsITEM_ID->AsInteger;
		info.fileID = QueryStorageItemsFILE_ID->AsInteger;
		info.versionID = QueryStorageItemsVERSION_ID->AsInteger;
		info.storageID = QueryStorageItemsSTORAGE_ID->AsInteger;
		info.name = QueryStorageItemsNAME->AsString.c_str();
		info.volumeID = QueryStorageItemsVOLUME_ID->AsInteger;
		info.curVersion = QueryStorageItemsCUR_VERSION->AsInteger;
		info.version = QueryStorageItemsVERSION->AsInteger;
		info.mimeType = QueryStorageItemsMIME_TYPE->AsString.c_str();
		info.storageFile = QueryStorageItemsFILE_PATH->AsString.c_str();
	}
	QueryStorageItems->Close();
}

void TDocManDataModule::loadAllStorageInfos( int minStorageID, StorageInfos *result )
{
	QueryAllStorageItems->Params->Items[0]->AsInteger = minStorageID;
	QueryAllStorageItems->Params->Items[1]->AsInteger = minStorageID+10000;
	for(
		QueryAllStorageItems->Open();
		!QueryAllStorageItems->Eof;
		QueryAllStorageItems->Next()
	)
	{
		StorageInfo	&info = result->createElement();
		info.itemID = QueryAllStorageItemsITEM_ID->AsInteger;
		info.fileID = QueryAllStorageItemsFILE_ID->AsInteger;
		info.versionID = QueryAllStorageItemsVERSION_ID->AsInteger;
		info.storageID = QueryAllStorageItemsSTORAGE_ID->AsInteger;
		info.name = QueryAllStorageItemsNAME->AsString.c_str();
		info.volumeID = QueryAllStorageItemsVOLUME_ID->AsInteger;
		info.curVersion = QueryAllStorageItemsCUR_VERSION->AsInteger;
		info.version = QueryAllStorageItemsVERSION->AsInteger;
		info.mimeType = QueryAllStorageItemsMIME_TYPE->AsString.c_str();
		info.storageFile = QueryAllStorageItemsFILE_PATH->AsString.c_str();
	}
	QueryAllStorageItems->Close();
}

//---------------------------------------------------------------------------

const UserOrGroup *TDocManDataModule::login( void )
{
	doEnterFunctionEx(gakLogging::llInfo, "TDocManDataModule::login");
	bool	showLogin;

	if( !m_actUser )	// first call
	{
		print2StartWindow( "%s", "Autologin" );

		m_actUser = &gak::vcl::getActUser( theDatabase->DatabaseName );
		showLogin = false;
		if( m_actUser->ID )
		{
			STRING		publicKeyFile = THE_FILE::getExternalStorageBase();

			if( !publicKeyFile.isEmpty() )
			{
				publicKeyFile += "keys" DIRECTORY_DELIMITER_STRING;
				publicKeyFile += m_actUser->userName;

				CryptoRSA	publicKey( true );
				if( publicKey.hasKey() )
				{
					makePath( publicKeyFile );
					publicKey.saveCypher( publicKeyFile );
				}
			}
		}
	}
	else			// call from menu
	{
		showLogin = true;
	}

	do
	{
		if( showLogin )
		{
			if( LoginForm->ShowModal( "Doc Manager" ) == mrOk )
			{
				gak::vcl::actUser = vcl::loginUser(
					theDatabase->DatabaseName,
					LoginForm->EditUserName->Text,
					LoginForm->EditPassword->Text
				);
			}
			else
			{
/*v*/			break;
			}
		}

		if( !m_actUser->ID )
		{
			Application->MessageBox( "Unknown User or Password", "Error", MB_ICONSTOP );
		}

		if( !m_actUser->permissions & vcl::USER_LOGIN_PERM )
		{
			Application->MessageBox( "Login Denied", "Error", MB_ICONSTOP );
			m_actUser = NULL;
		}

		showLogin = true;
	} while( !m_actUser || m_actUser->ID == 0 );

	return m_actUser;
}

//---------------------------------------------------------------------------

CryptoRSA &TDocManDataModule::getPrivateKey( void )
{
	if( !m_privateKey.hasKey() )
	{
		if( PasswordForm->ShowModal() == mrOk )
		{
			STRING	txtCypher = PasswordForm->EditPassword->Text.c_str();
			m_privateKey.loadCryptedPersonalCypher( txtCypher );
		}
		else
		{
			throw EAbort( "Canceled" );
		}
	}

	return m_privateKey;
}
//---------------------------------------------------------------------------
const STRING &TDocManDataModule::getMachine( void )
{
	static STRING machine;
	if( machine.isEmpty() )
	{
		char			computerName[MAX_COMPUTERNAME_LENGTH+2];
		unsigned long	size = MAX_COMPUTERNAME_LENGTH+1;

		GetComputerName( computerName, &size );
		machine = computerName;
	}

	return machine;
}


