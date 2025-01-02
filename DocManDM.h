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

#ifndef DocManDMH
#define DocManDMH
#include <Classes.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
//---------------------------------------------------------------------------

#include <iostream>

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBTables.hpp>

#include <gak/map.h>
#include <gak/exif.h>
#include <gak/fieldSet.h>
#include <gak/iostream.h>
#include <gak/DirectoryEntry.h>
#include <gak/vcl_tools.h>
#include <gak/rsa.h>

#include "DocManThreads.h"
#include "ItemManager.h"

//---------------------------------------------------------------------------
typedef Array<TTabSheet*>	TTabSheets;
//---------------------------------------------------------------------------
enum AttributeType
{
	ATTR_String, ATTR_Date, ATTR_Time, ATTR_Number, ATTR_Integer
};

struct AttributeValue
{
	gak::DynamicVar	value;
	TWinControl		*editor;
	TButton			*addButton,
					*delButton;
};
typedef Array<AttributeValue>	AttributeValues;

struct AttrDefinition
{
	STRING				name;
	AttributeType		type;
	size_t				minCount, maxCount;
	int					ID, attrID;
	bool				required;

	TLabel				*label;

	AttributeValues		values;
};
typedef Array<AttrDefinition>	AttrDefinitions;

struct CategoryData
{
	STRING			name;
	int				catID, version;
	AttrDefinitions	attrDefinition;
};
typedef Array<CategoryData>	CategoryDatas;

struct StorageKey
{
	int		itemID;
	int		fileID;
	int		versionID;
	int		storageID;
	int		volumeID;
	int		curVersion;
	int		version;

	int compare( const StorageKey &other ) const
	{
		int result = gak::compare( this->itemID, other.itemID );
		if( !result )
		{
			result = gak::compare( this->fileID, other.fileID );
		}
		if( !result )
		{
			result = gak::compare( this->versionID, other.versionID );
		}
		return result;
	}
};

namespace gak
{
	template <>
	struct is_binary<StorageKey>
	{
		static const bool value = true;
	};

	template <>
	inline void toBinaryStream( std::ostream &stream, const StorageKey &value )
	{
		binaryToBinaryStream( stream, value );
	}

	template <>
	inline void fromBinaryStream( std::istream &stream, StorageKey *value )
	{
		binaryFromBinaryStream( stream, value );
	}
}

struct StorageInfo : public StorageKey
{
	STRING			name;
	gak::F_STRING	storageFile;
	STRING			mimeType;
};
typedef Array<StorageInfo>	StorageInfos;

class TDocManDataModule : public TDataModule
{
__published:	// IDE-managed Components
	TTable *TableTaskTypes;
	TIntegerField *TableTaskTypesID;
	TStringField *TableTaskTypesTASK_TYPE;
	TStringField *TableTaskTypesDESCRIPTION;
	TTable *TableTaskStatus;
	TIntegerField *TableTaskStatusID;
	TStringField *TableTaskStatusTASK_STATUS;
	TStringField *TableTaskStatusDESCRIPTION;
	TIntegerField *TableTaskStatusCOMPLETED;
	TTable *TableIstorage;
	TIntegerField *TableIstorageID;
	TIntegerField *TableIstorageUSAGE_COUNT;
	TStringField *TableIstorageFILE_PATH;
	TTable *TableIfiles;
	TIntegerField *TableIfilesID;
	TIntegerField *TableIfilesUSAGECOUNT;
	TIntegerField *TableIfilesVERSION;
	TTable *TableIfileVersions;
	TIntegerField *TableIfileVersionsID;
	TIntegerField *TableIfileVersionsFILE_ID;
	TIntegerField *TableIfileVersionsVERSION;
	TStringField *TableIfileVersionsMIME_TYPE;
	TStringField *TableIfileVersionsFILENAME;
	TIntegerField *TableIfileVersionsFILESIZE;
	TIntegerField *TableIfileVersionsCREATEDBY;
	TDateTimeField *TableIfileVersionsCREATEDDATE;
	TDateTimeField *TableIfileVersionsMODIFIEDDATE;
	TDateTimeField *TableIfileVersionsFILECREATEDDATE;
	TDateTimeField *TableIfileVersionsFILEMODIFIEDDATE;
	TIntegerField *TableIfileVersionsLOCKED;
	TIntegerField *TableIfileVersionsSTORAGE_ID;
	TMemoField *TableIfileVersionsDESCRIPTION;
	TTable *TableItemTree;
	TIntegerField *TableItemTreeID;
	TIntegerField *TableItemTreePARENTID;
	TIntegerField *TableItemTreeCOPYID;
	TIntegerField *TableItemTreeITEM_TYPE;
	TStringField *TableItemTreeNAME;
	TMemoField *TableItemTreeDESCRIPTION;
	TIntegerField *TableItemTreeCREATEDBY;
	TDateTimeField *TableItemTreeCREATEDDATE;
	TDateTimeField *TableItemTreeMODIFIEDDATE;
	TIntegerField *TableItemTreeOLD_PARENT;
	TStringField *TableItemTreeOLD_NAME;
	TIntegerField *TableItemTreeFILE_ID;
	TIntegerField *TableItemTreeMAJOR_RELEASE;
	TIntegerField *TableItemTreeMINOR_RELEASE;
	TIntegerField *TableItemTreePATCH_RELEASE;
	TIntegerField *TableItemTreeRESERVEDBY;
	TStringField *TableItemTreeRESERVEDON;
	TStringField *TableItemTreeCUSTOMER_REF;
	TIntegerField *TableItemTreeTYPE;
	TIntegerField *TableItemTreeSTATUS;
	TStringField *TableItemTreeMODULE;
	TStringField *TableItemTreeBAD_FUNCTION;
	TStringField *TableItemTreeRELEASE;
	TIntegerField *TableItemTreePRIORITY;
	TIntegerField *TableItemTreeASSIGNED_TO;
	TDateTimeField *TableItemTreeCOMPLETED;
	TStringField *TableItemTreeEXTERNAL_REMARKS;
	TStringField *TableItemTreeINTERNAL_REMARKS;
	TFloatField *TableItemTreeESTIMATED_EFFORT;
	TFloatField *TableItemTreeACTUAL_EFFORT;
	TTable *TableIexifData;
	TIntegerField *TableIexifDataITEM_ID;
	TIntegerField *TableIexifDataVERSION;
	TStringField *TableIexifDataMANUFACTURER;
	TStringField *TableIexifDataCAMERA;
	TStringField *TableIexifDataLENS;
	TIntegerField *TableIexifDataISO_SPEED;
	TFloatField *TableIexifDataFNUMBER;
	TFloatField *TableIexifDataEXPOSURE_TIME;
	TIntegerField *TableIexifDataFOCAL_LENGTH;
	TQuery *QueryCatAttributes;
	TIntegerField *QueryCatAttributesID;
	TIntegerField *QueryCatAttributesCATEGORY_ID;
	TIntegerField *QueryCatAttributesCATEGORY_VERSION;
	TStringField *QueryCatAttributesNAME;
	TIntegerField *QueryCatAttributesTYPE;
	TIntegerField *QueryCatAttributesORDER_NR;
	TIntegerField *QueryCatAttributesREQUIRED;
	TIntegerField *QueryCatAttributesMIN_COUNT;
	TIntegerField *QueryCatAttributesMAX_COUNT;
	TIntegerField *QueryCatAttributesATTR_ID;
	TQuery *QueryAttrValues;
	TIntegerField *QueryAttrValuesID;
	TIntegerField *QueryAttrValuesITEM_ID;
	TIntegerField *QueryAttrValuesITEM_VERSION;
	TIntegerField *QueryAttrValuesATTR_ID;
	TIntegerField *QueryAttrValuesORDER_NR;
	TDateTimeField *QueryAttrValuesDATE_VALUE;
	TIntegerField *QueryAttrValuesINTEGER_VALUE;
	TFloatField *QueryAttrValuesFLOAT_VALUE;
	TStringField *QueryAttrValuesSTRING_VALUE;
	TTable *TableAttrValues;
	TIntegerField *TableAttrValuesID;
	TIntegerField *TableAttrValuesITEM_ID;
	TIntegerField *TableAttrValuesITEM_VERSION;
	TIntegerField *TableAttrValuesATTR_ID;
	TIntegerField *TableAttrValuesORDER_NR;
	TDateTimeField *TableAttrValuesDATE_VALUE;
	TIntegerField *TableAttrValuesINTEGER_VALUE;
	TFloatField *TableAttrValuesFLOAT_VALUE;
	TStringField *TableAttrValuesSTRING_VALUE;
	TQuery *QueryCategory;
	TIntegerField *QueryCategoryID;
	TStringField *QueryCategoryNAME;
	TIntegerField *QueryCategoryCATEGORY_VERSION;
	TStringField *TableIexifDataSOFTWARE;
	TStringField *TableIexifDataARTIST;
	TStringField *TableIexifDataCOPYRIGHT;
	TStringField *TableIexifDataUSERCOMMENT;
	TMemoField *TableIexifDataIMAGE_DESCRIPTION;
	TIntegerField *TableIexifDataEXPOSURE_PROGRAM;
	TIntegerField *TableIexifDataMETERING_MODE;
	TFloatField *TableIexifDataEXPOSURE_COMPENSATION;
	TDateTimeField *TableIexifDataORIGINAL_DATE_TIME;
	TStringField *TableIexifDataFLASH;
	TStringField *TableIexifDataRED_EYE;
	TQuery *QueryDocVersions;
	TIntegerField *QueryDocVersionsVERSION;
	TIntegerField *QueryDocVersionsID;
	TIntegerField *QueryDocVersionsITEM_TYPE;
	TStringField *QueryDocVersionsNAME;
	TIntegerField *TableTaskStatusDEFAULT_STATUS;
	TIntegerField *TableTaskTypesDEFAULT_TYPE;
	TIntegerField *TableItemTreeORIGINAL_ID;
	TIntegerField *TableItemTreePERMISSION_ID;
	TIntegerField *TableItemTreeVOLUME_ID;
	TIntegerField *TableItemTreeRESERVEDFOR;
	TStringField *TableItemTreeEXPECTED_RESULT;
	TStringField *TableItemTreeACTUAL_RESULT;
	TIntegerField *TableItemTreeTEST_STATUS;
	TIntegerField *TableItemTreeTASK_LIST;
	TIntegerField *TableItemTreeORDERING;
	TStringField *TableIstorageMD5_CHECKSUM;
	TStringField *TableItemTreeMD5_CHECKSUM;
	TIntegerField *TableTaskStatusIN_PROGRESS;
	TQuery *QueryStorageItems;
	TQuery *QueryAllStorageItems;
	TIntegerField *QueryStorageItemsITEM_ID;
	TStringField *QueryStorageItemsNAME;
	TIntegerField *QueryStorageItemsVOLUME_ID;
	TIntegerField *QueryStorageItemsFILE_ID;
	TIntegerField *QueryStorageItemsCUR_VERSION;
	TIntegerField *QueryStorageItemsVERSION_ID;
	TIntegerField *QueryStorageItemsVERSION;
	TStringField *QueryStorageItemsMIME_TYPE;
	TIntegerField *QueryStorageItemsSTORAGE_ID;
	TStringField *QueryStorageItemsFILE_PATH;
	TIntegerField *QueryAllStorageItemsITEM_ID;
	TStringField *QueryAllStorageItemsNAME;
	TIntegerField *QueryAllStorageItemsVOLUME_ID;
	TIntegerField *QueryAllStorageItemsFILE_ID;
	TIntegerField *QueryAllStorageItemsCUR_VERSION;
	TIntegerField *QueryAllStorageItemsVERSION_ID;
	TIntegerField *QueryAllStorageItemsVERSION;
	TStringField *QueryAllStorageItemsMIME_TYPE;
	TIntegerField *QueryAllStorageItemsSTORAGE_ID;
	TStringField *QueryAllStorageItemsFILE_PATH;
	TIntegerField *TableItemTreeREMOTE_SERVER_ID;
	TIntegerField *TableItemTreeREMOTE_ID;
	TDatabase *theDatabase;
	TIntegerField *TableItemTreeREMINDERDATE;
	void __fastcall TableTaskTypesBeforePost(TDataSet *DataSet);
private:	// User declarations
	gak::IntStrMap 	idPathMapping;
	gak::IntStrMap	taskStatusList;
	gak::IntStrMap	taskTypeList;
	int		 		defaultTaskType, defaultTaskStatus;

	void loadAttributeValues(
		int itemId, int itemVersion, AttrDefinition *attrDef
	);
	bool updateExifData( const gak::ImageMetaData *metaData );

	const UserOrGroup		*m_actUser;
	gak::CryptoRSA			m_privateKey;

public:		// User declarations
	__fastcall TDocManDataModule(TComponent* Owner);

	gak::CryptoRSA &getPrivateKey( void );

	const UserOrGroup *login( void );
	const UserOrGroup *getActUser( void ) const
	{
		return m_actUser;
	}
	void getUserById( int userId, UserOrGroup *result ) const
	{
		gak::vcl::getUserById( theDatabase->DatabaseName, userId, result );
	}

	// static STRING md5Str( unsigned char md5[16] );
	static STRING md5file( const STRING &filePath );

	void checkDB( bool silent );
	void refreshExifs( void );
	void refreshMimeTypes( void );

	void refreshTaskTypes( TStrings *items=NULL );
	void refreshTaskStatus( TStrings *items=NULL );
	int getDefaultTaskType( void )
	{
		if( !defaultTaskType )
			refreshTaskTypes();

		return defaultTaskType;
	}
	int getDefaultTaskStatus( void )
	{
		if( !defaultTaskStatus )
			refreshTaskStatus();

		return defaultTaskStatus;
	}
	void initPathMapping( void )
	{
		idPathMapping.clear();
	}
	STRING getPathForID( int theID );

	STRING getStatus( int statusID )
	{
		return taskStatusList[statusID];
	}
	STRING getType( int typeID )
	{
		return taskTypeList[typeID];
	}
	bool updateExifData(
		const PTR_ITEM &theItem, int theVersion, const gak::ImageMetaData *metaData
	);

	void getCategoryID(
		const STRING &categoryName, int *categoryId, int *categoryVersion
	);
	void loadCategoryDefinition(
		int categoryId, int version, CategoryData *attrDefinition,
		int itemId = 0, int itemVersion = 0
	);
	void loadCategoryDefinition(
		const STRING &categoryName, CategoryData *attrDefinition,
		int itemId = 0, int itemVersion = 0
	);
	void loadCategoryData(
		int itemId, int itemVersion, CategoryDatas *attrData
	);

	void copyAttributeValues(
		int srcID, int srcVersion, int destID, int destVersion
	);

	void loadStorageInfos( int storageID, StorageInfos *result );
	void loadAllStorageInfos( int storageID, StorageInfos *result );

	static const STRING &getMachine( void );
};
//---------------------------------------------------------------------------
#pragma option -RT-
class THREAD_CHECK_DB : public ThreadDocMan
{
	virtual const char *getTitle( void ) const;
	virtual void perform( void );
};

class THREAD_REFRESH_EXIFS : public ThreadDocMan
{
	virtual const char *getTitle( void ) const;
	virtual void perform( void );
};

class THREAD_REFRESH_MIME_TYPES : public ThreadDocMan
{
	virtual const char *getTitle( void ) const;
	virtual void perform( void );
};



#pragma option -RT+
//---------------------------------------------------------------------------
extern PACKAGE TDocManDataModule *DocManDataModule;
extern char registryKey[];
//---------------------------------------------------------------------------
#endif
