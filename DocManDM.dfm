object DocManDataModule: TDocManDataModule
  OldCreateOrder = False
  Left = 355
  Top = 276
  Height = 688
  Width = 1000
  object TableTaskTypes: TTable
    BeforePost = TableTaskTypesBeforePost
    DatabaseName = 'docManDB'
    FieldDefs = <
      item
        Name = 'ID'
        Attributes = [faRequired]
        DataType = ftInteger
      end
      item
        Name = 'TASK_TYPE'
        Attributes = [faRequired]
        DataType = ftString
        Size = 32
      end
      item
        Name = 'DESCRIPTION'
        DataType = ftString
        Size = 255
      end
      item
        Name = 'DEFAULT_TYPE'
        DataType = ftInteger
      end>
    IndexDefs = <
      item
        Name = 'TableTaskTypesIndex1'
        Fields = 'ID'
        Options = [ixPrimary, ixUnique]
      end
      item
        Name = 'TASK_TYPE_IDX'
        Fields = 'TASK_TYPE'
        Options = [ixUnique, ixCaseInsensitive]
      end>
    IndexName = 'TASK_TYPE_IDX'
    StoreDefs = True
    TableName = 'I_TASK_TYPES'
    Left = 32
    Top = 16
    object TableTaskTypesID: TIntegerField
      FieldName = 'ID'
      Visible = False
    end
    object TableTaskTypesTASK_TYPE: TStringField
      DisplayLabel = 'Task Type'
      DisplayWidth = 16
      FieldName = 'TASK_TYPE'
      Required = True
      Size = 32
    end
    object TableTaskTypesDESCRIPTION: TStringField
      DisplayLabel = 'Description'
      DisplayWidth = 60
      FieldName = 'DESCRIPTION'
      Size = 255
    end
    object TableTaskTypesDEFAULT_TYPE: TIntegerField
      DisplayLabel = 'Default'
      FieldName = 'DEFAULT_TYPE'
    end
  end
  object TableTaskStatus: TTable
    DatabaseName = 'docManDB'
    FieldDefs = <
      item
        Name = 'ID'
        Attributes = [faRequired]
        DataType = ftInteger
      end
      item
        Name = 'TASK_STATUS'
        Attributes = [faRequired]
        DataType = ftString
        Size = 32
      end
      item
        Name = 'DESCRIPTION'
        DataType = ftString
        Size = 255
      end
      item
        Name = 'COMPLETED'
        Attributes = [faRequired]
        DataType = ftInteger
      end
      item
        Name = 'DEFAULT_STATUS'
        DataType = ftInteger
      end
      item
        Name = 'IN_PROGRESS'
        DataType = ftInteger
      end>
    IndexDefs = <
      item
        Name = 'TableTaskStatusIndex1'
        Fields = 'ID'
        Options = [ixPrimary, ixUnique]
      end
      item
        Name = 'TASK_STATUS_IDX'
        Fields = 'TASK_STATUS'
        Options = [ixUnique, ixCaseInsensitive]
      end>
    StoreDefs = True
    TableName = 'I_TASK_STATUS'
    Left = 32
    Top = 80
    object TableTaskStatusID: TIntegerField
      DisplayWidth = 3
      FieldName = 'ID'
      Required = True
    end
    object TableTaskStatusTASK_STATUS: TStringField
      DisplayLabel = 'Status'
      DisplayWidth = 16
      FieldName = 'TASK_STATUS'
      Required = True
      Size = 32
    end
    object TableTaskStatusDESCRIPTION: TStringField
      DisplayLabel = 'Description'
      DisplayWidth = 45
      FieldName = 'DESCRIPTION'
      Size = 255
    end
    object TableTaskStatusCOMPLETED: TIntegerField
      DefaultExpression = '0'
      DisplayLabel = 'Complete Task'
      FieldName = 'COMPLETED'
      Required = True
      MaxValue = 1
    end
    object TableTaskStatusDEFAULT_STATUS: TIntegerField
      DisplayLabel = 'Default'
      FieldName = 'DEFAULT_STATUS'
    end
    object TableTaskStatusIN_PROGRESS: TIntegerField
      DisplayLabel = 'In Progress'
      FieldName = 'IN_PROGRESS'
    end
  end
  object TableIstorage: TTable
    DatabaseName = 'docManDB'
    FieldDefs = <
      item
        Name = 'ID'
        Attributes = [faRequired]
        DataType = ftInteger
      end
      item
        Name = 'USAGE_COUNT'
        DataType = ftInteger
      end
      item
        Name = 'FILE_PATH'
        Attributes = [faRequired]
        DataType = ftString
        Size = 255
      end
      item
        Name = 'MD5_CHECKSUM'
        DataType = ftString
        Size = 32
      end>
    IndexDefs = <
      item
        Name = 'TableIstorageIndex1'
        Fields = 'ID'
        Options = [ixPrimary, ixUnique]
      end>
    StoreDefs = True
    TableName = 'I_STORAGE'
    Left = 32
    Top = 152
    object TableIstorageID: TIntegerField
      FieldName = 'ID'
      Required = True
    end
    object TableIstorageUSAGE_COUNT: TIntegerField
      FieldName = 'USAGE_COUNT'
    end
    object TableIstorageFILE_PATH: TStringField
      FieldName = 'FILE_PATH'
      Required = True
      Size = 255
    end
    object TableIstorageMD5_CHECKSUM: TStringField
      FieldName = 'MD5_CHECKSUM'
      Size = 32
    end
  end
  object TableIfiles: TTable
    DatabaseName = 'docManDB'
    FieldDefs = <
      item
        Name = 'ID'
        Attributes = [faRequired]
        DataType = ftInteger
      end
      item
        Name = 'USAGECOUNT'
        DataType = ftInteger
      end
      item
        Name = 'VERSION'
        Attributes = [faRequired]
        DataType = ftInteger
      end>
    IndexDefs = <
      item
        Name = 'TableIfilesIndex1'
        Fields = 'ID'
        Options = [ixPrimary, ixUnique]
      end>
    StoreDefs = True
    TableName = 'I_FILES'
    Left = 160
    Top = 16
    object TableIfilesID: TIntegerField
      FieldName = 'ID'
      Required = True
    end
    object TableIfilesUSAGECOUNT: TIntegerField
      FieldName = 'USAGECOUNT'
    end
    object TableIfilesVERSION: TIntegerField
      FieldName = 'VERSION'
      Required = True
    end
  end
  object TableIfileVersions: TTable
    DatabaseName = 'docManDB'
    FieldDefs = <
      item
        Name = 'ID'
        Attributes = [faRequired]
        DataType = ftInteger
      end
      item
        Name = 'FILE_ID'
        Attributes = [faRequired]
        DataType = ftInteger
      end
      item
        Name = 'VERSION'
        Attributes = [faRequired]
        DataType = ftInteger
      end
      item
        Name = 'MIME_TYPE'
        Attributes = [faRequired]
        DataType = ftString
        Size = 255
      end
      item
        Name = 'FILENAME'
        DataType = ftString
        Size = 255
      end
      item
        Name = 'FILESIZE'
        Attributes = [faRequired]
        DataType = ftInteger
      end
      item
        Name = 'CREATEDBY'
        Attributes = [faRequired]
        DataType = ftInteger
      end
      item
        Name = 'CREATEDDATE'
        Attributes = [faRequired]
        DataType = ftDateTime
      end
      item
        Name = 'MODIFIEDDATE'
        Attributes = [faRequired]
        DataType = ftDateTime
      end
      item
        Name = 'FILECREATEDDATE'
        Attributes = [faRequired]
        DataType = ftDateTime
      end
      item
        Name = 'FILEMODIFIEDDATE'
        Attributes = [faRequired]
        DataType = ftDateTime
      end
      item
        Name = 'LOCKED'
        DataType = ftInteger
      end
      item
        Name = 'STORAGE_ID'
        Attributes = [faRequired]
        DataType = ftInteger
      end
      item
        Name = 'DESCRIPTION'
        DataType = ftMemo
        Size = 1
      end>
    IndexDefs = <
      item
        Name = 'TableIfileVersionsIndex1'
        Fields = 'ID'
        Options = [ixPrimary, ixUnique]
      end
      item
        Name = 'CREATEDBY'
        Fields = 'CREATEDBY'
      end
      item
        Name = 'FILE_ID'
        Fields = 'FILE_ID'
      end
      item
        Name = 'FILE_VERSION_IDX'
        Fields = 'FILE_ID;VERSION'
        Options = [ixUnique, ixDescending, ixCaseInsensitive]
      end
      item
        Name = 'STORAGE_ID'
        Fields = 'STORAGE_ID'
      end>
    StoreDefs = True
    TableName = 'I_FILE_VERS'
    Left = 152
    Top = 88
    object TableIfileVersionsID: TIntegerField
      FieldName = 'ID'
      Required = True
    end
    object TableIfileVersionsFILE_ID: TIntegerField
      FieldName = 'FILE_ID'
      Required = True
    end
    object TableIfileVersionsVERSION: TIntegerField
      FieldName = 'VERSION'
      Required = True
    end
    object TableIfileVersionsMIME_TYPE: TStringField
      FieldName = 'MIME_TYPE'
      Required = True
      Size = 255
    end
    object TableIfileVersionsFILENAME: TStringField
      FieldName = 'FILENAME'
      Size = 255
    end
    object TableIfileVersionsFILESIZE: TIntegerField
      FieldName = 'FILESIZE'
      Required = True
    end
    object TableIfileVersionsCREATEDBY: TIntegerField
      FieldName = 'CREATEDBY'
      Required = True
    end
    object TableIfileVersionsCREATEDDATE: TDateTimeField
      FieldName = 'CREATEDDATE'
      Required = True
    end
    object TableIfileVersionsMODIFIEDDATE: TDateTimeField
      FieldName = 'MODIFIEDDATE'
      Required = True
    end
    object TableIfileVersionsFILECREATEDDATE: TDateTimeField
      FieldName = 'FILECREATEDDATE'
      Required = True
    end
    object TableIfileVersionsFILEMODIFIEDDATE: TDateTimeField
      FieldName = 'FILEMODIFIEDDATE'
      Required = True
    end
    object TableIfileVersionsLOCKED: TIntegerField
      FieldName = 'LOCKED'
    end
    object TableIfileVersionsSTORAGE_ID: TIntegerField
      FieldName = 'STORAGE_ID'
      Required = True
    end
    object TableIfileVersionsDESCRIPTION: TMemoField
      FieldName = 'DESCRIPTION'
      BlobType = ftMemo
      Size = 1
    end
  end
  object TableItemTree: TTable
    DatabaseName = 'docManDB'
    FieldDefs = <
      item
        Name = 'ID'
        Attributes = [faRequired]
        DataType = ftInteger
      end
      item
        Name = 'PARENTID'
        Attributes = [faRequired]
        DataType = ftInteger
      end
      item
        Name = 'COPYID'
        Attributes = [faRequired]
        DataType = ftInteger
      end
      item
        Name = 'ORIGINAL_ID'
        DataType = ftInteger
      end
      item
        Name = 'PERMISSION_ID'
        DataType = ftInteger
      end
      item
        Name = 'VOLUME_ID'
        Attributes = [faRequired]
        DataType = ftInteger
      end
      item
        Name = 'ITEM_TYPE'
        Attributes = [faRequired]
        DataType = ftInteger
      end
      item
        Name = 'NAME'
        Attributes = [faRequired]
        DataType = ftString
        Size = 255
      end
      item
        Name = 'DESCRIPTION'
        DataType = ftMemo
        Size = 1
      end
      item
        Name = 'CREATEDBY'
        Attributes = [faRequired]
        DataType = ftInteger
      end
      item
        Name = 'CREATEDDATE'
        Attributes = [faRequired]
        DataType = ftDateTime
      end
      item
        Name = 'MODIFIEDDATE'
        Attributes = [faRequired]
        DataType = ftDateTime
      end
      item
        Name = 'OLD_PARENT'
        DataType = ftInteger
      end
      item
        Name = 'OLD_NAME'
        DataType = ftString
        Size = 255
      end
      item
        Name = 'FILE_ID'
        DataType = ftInteger
      end
      item
        Name = 'MAJOR_RELEASE'
        DataType = ftInteger
      end
      item
        Name = 'MINOR_RELEASE'
        DataType = ftInteger
      end
      item
        Name = 'PATCH_RELEASE'
        DataType = ftInteger
      end
      item
        Name = 'RESERVEDBY'
        DataType = ftInteger
      end
      item
        Name = 'RESERVEDON'
        DataType = ftString
        Size = 255
      end
      item
        Name = 'RESERVEDFOR'
        DataType = ftInteger
      end
      item
        Name = 'CUSTOMER_REF'
        DataType = ftString
        Size = 32
      end
      item
        Name = 'TYPE'
        DataType = ftInteger
      end
      item
        Name = 'STATUS'
        DataType = ftInteger
      end
      item
        Name = 'MODULE'
        DataType = ftString
        Size = 128
      end
      item
        Name = 'BAD_FUNCTION'
        DataType = ftString
        Size = 128
      end
      item
        Name = 'RELEASE'
        DataType = ftString
        Size = 32
      end
      item
        Name = 'PRIORITY'
        DataType = ftInteger
      end
      item
        Name = 'ASSIGNED_TO'
        DataType = ftInteger
      end
      item
        Name = 'COMPLETED'
        DataType = ftDateTime
      end
      item
        Name = 'EXTERNAL_REMARKS'
        DataType = ftString
        Size = 255
      end
      item
        Name = 'INTERNAL_REMARKS'
        DataType = ftString
        Size = 255
      end
      item
        Name = 'ESTIMATED_EFFORT'
        DataType = ftFloat
      end
      item
        Name = 'ACTUAL_EFFORT'
        DataType = ftFloat
      end
      item
        Name = 'EXPECTED_RESULT'
        DataType = ftString
        Size = 255
      end
      item
        Name = 'ACTUAL_RESULT'
        DataType = ftString
        Size = 255
      end
      item
        Name = 'TEST_STATUS'
        DataType = ftInteger
      end
      item
        Name = 'TASK_LIST'
        DataType = ftInteger
      end
      item
        Name = 'MD5_CHECKSUM'
        DataType = ftString
        Size = 32
      end
      item
        Name = 'ORDERING'
        DataType = ftInteger
      end>
    IndexDefs = <
      item
        Name = 'TableItemTreeIndex1'
        Fields = 'ID'
        Options = [ixPrimary, ixUnique]
      end
      item
        Name = 'ITEM_PARENT_NAME_IDX'
        Fields = 'PARENTID;NAME'
        Options = [ixUnique]
      end
      item
        Name = 'CREATEDBY'
        Fields = 'CREATEDBY'
      end>
    StoreDefs = True
    TableName = 'ITEM_TREE'
    Left = 152
    Top = 152
    object TableItemTreeID: TIntegerField
      FieldName = 'ID'
      Required = True
    end
    object TableItemTreePARENTID: TIntegerField
      FieldName = 'PARENTID'
      Required = True
    end
    object TableItemTreeCOPYID: TIntegerField
      FieldName = 'COPYID'
      Required = True
    end
    object TableItemTreeITEM_TYPE: TIntegerField
      FieldName = 'ITEM_TYPE'
      Required = True
    end
    object TableItemTreeNAME: TStringField
      FieldName = 'NAME'
      Required = True
      Size = 255
    end
    object TableItemTreeDESCRIPTION: TMemoField
      FieldName = 'DESCRIPTION'
      BlobType = ftMemo
      Size = 1
    end
    object TableItemTreeCREATEDBY: TIntegerField
      FieldName = 'CREATEDBY'
      Required = True
    end
    object TableItemTreeCREATEDDATE: TDateTimeField
      FieldName = 'CREATEDDATE'
      Required = True
    end
    object TableItemTreeMODIFIEDDATE: TDateTimeField
      FieldName = 'MODIFIEDDATE'
      Required = True
    end
    object TableItemTreeOLD_PARENT: TIntegerField
      FieldName = 'OLD_PARENT'
    end
    object TableItemTreeOLD_NAME: TStringField
      FieldName = 'OLD_NAME'
      Size = 255
    end
    object TableItemTreeFILE_ID: TIntegerField
      FieldName = 'FILE_ID'
    end
    object TableItemTreeMAJOR_RELEASE: TIntegerField
      FieldName = 'MAJOR_RELEASE'
    end
    object TableItemTreeMINOR_RELEASE: TIntegerField
      FieldName = 'MINOR_RELEASE'
    end
    object TableItemTreePATCH_RELEASE: TIntegerField
      FieldName = 'PATCH_RELEASE'
    end
    object TableItemTreeRESERVEDBY: TIntegerField
      FieldName = 'RESERVEDBY'
    end
    object TableItemTreeRESERVEDON: TStringField
      FieldName = 'RESERVEDON'
      Size = 255
    end
    object TableItemTreeCUSTOMER_REF: TStringField
      FieldName = 'CUSTOMER_REF'
      Size = 32
    end
    object TableItemTreeTYPE: TIntegerField
      FieldName = 'TYPE'
    end
    object TableItemTreeSTATUS: TIntegerField
      FieldName = 'STATUS'
    end
    object TableItemTreeMODULE: TStringField
      FieldName = 'MODULE'
      Size = 128
    end
    object TableItemTreeBAD_FUNCTION: TStringField
      FieldName = 'BAD_FUNCTION'
      Size = 128
    end
    object TableItemTreeRELEASE: TStringField
      FieldName = 'RELEASE'
      Size = 32
    end
    object TableItemTreePRIORITY: TIntegerField
      FieldName = 'PRIORITY'
    end
    object TableItemTreeASSIGNED_TO: TIntegerField
      FieldName = 'ASSIGNED_TO'
    end
    object TableItemTreeCOMPLETED: TDateTimeField
      FieldName = 'COMPLETED'
    end
    object TableItemTreeEXTERNAL_REMARKS: TStringField
      FieldName = 'EXTERNAL_REMARKS'
      Size = 255
    end
    object TableItemTreeINTERNAL_REMARKS: TStringField
      FieldName = 'INTERNAL_REMARKS'
      Size = 255
    end
    object TableItemTreeESTIMATED_EFFORT: TFloatField
      FieldName = 'ESTIMATED_EFFORT'
    end
    object TableItemTreeACTUAL_EFFORT: TFloatField
      FieldName = 'ACTUAL_EFFORT'
    end
    object TableItemTreeORIGINAL_ID: TIntegerField
      FieldName = 'ORIGINAL_ID'
    end
    object TableItemTreePERMISSION_ID: TIntegerField
      FieldName = 'PERMISSION_ID'
    end
    object TableItemTreeVOLUME_ID: TIntegerField
      FieldName = 'VOLUME_ID'
      Required = True
    end
    object TableItemTreeRESERVEDFOR: TIntegerField
      FieldName = 'RESERVEDFOR'
    end
    object TableItemTreeEXPECTED_RESULT: TStringField
      FieldName = 'EXPECTED_RESULT'
      Size = 255
    end
    object TableItemTreeACTUAL_RESULT: TStringField
      FieldName = 'ACTUAL_RESULT'
      Size = 255
    end
    object TableItemTreeTEST_STATUS: TIntegerField
      FieldName = 'TEST_STATUS'
    end
    object TableItemTreeTASK_LIST: TIntegerField
      FieldName = 'TASK_LIST'
    end
    object TableItemTreeORDERING: TIntegerField
      FieldName = 'ORDERING'
    end
    object TableItemTreeMD5_CHECKSUM: TStringField
      FieldName = 'MD5_CHECKSUM'
      Size = 32
    end
  end
  object TableIexifData: TTable
    DatabaseName = 'docManDB'
    FieldDefs = <
      item
        Name = 'ITEM_ID'
        Attributes = [faRequired]
        DataType = ftInteger
      end
      item
        Name = 'VERSION'
        Attributes = [faRequired]
        DataType = ftInteger
      end
      item
        Name = 'MANUFACTURER'
        DataType = ftString
        Size = 255
      end
      item
        Name = 'CAMERA'
        DataType = ftString
        Size = 255
      end
      item
        Name = 'LENS'
        DataType = ftString
        Size = 255
      end
      item
        Name = 'ISO_SPEED'
        DataType = ftInteger
      end
      item
        Name = 'FNUMBER'
        DataType = ftFloat
      end
      item
        Name = 'TIME_NUMERATOR'
        DataType = ftInteger
      end
      item
        Name = 'TIME_DENOMINATOR'
        DataType = ftInteger
      end
      item
        Name = 'EXPOSURE_TIME'
        DataType = ftFloat
      end
      item
        Name = 'FOCAL_LENGTH'
        DataType = ftInteger
      end
      item
        Name = 'SOFTWARE'
        DataType = ftString
        Size = 255
      end
      item
        Name = 'ARTIST'
        DataType = ftString
        Size = 255
      end
      item
        Name = 'COPYRIGHT'
        DataType = ftString
        Size = 255
      end
      item
        Name = 'USERCOMMENT'
        DataType = ftString
        Size = 255
      end
      item
        Name = 'IMAGE_DESCRIPTION'
        DataType = ftMemo
        Size = 1
      end
      item
        Name = 'EXPOSURE_PROGRAM'
        DataType = ftInteger
      end
      item
        Name = 'METERING_MODE'
        DataType = ftInteger
      end
      item
        Name = 'EXP_COMP_NUMERATOR'
        DataType = ftInteger
      end
      item
        Name = 'EXP_COMP_DENOMINATOR'
        DataType = ftInteger
      end
      item
        Name = 'EXPOSURE_COMPENSATION'
        DataType = ftFloat
      end
      item
        Name = 'ORIGINAL_DATE_TIME'
        DataType = ftDateTime
      end
      item
        Name = 'FLASH'
        DataType = ftString
        Size = 1
      end
      item
        Name = 'RED_EYE'
        DataType = ftString
        Size = 1
      end>
    IndexDefs = <
      item
        Name = 'TableIexifDataIndex1'
        Fields = 'ITEM_ID;VERSION'
        Options = [ixPrimary, ixUnique]
      end>
    StoreDefs = True
    TableName = 'I_EXIF_DATA'
    Left = 32
    Top = 224
    object TableIexifDataITEM_ID: TIntegerField
      FieldName = 'ITEM_ID'
      Required = True
    end
    object TableIexifDataVERSION: TIntegerField
      FieldName = 'VERSION'
      Required = True
    end
    object TableIexifDataMANUFACTURER: TStringField
      FieldName = 'MANUFACTURER'
      Size = 255
    end
    object TableIexifDataCAMERA: TStringField
      FieldName = 'CAMERA'
      Size = 255
    end
    object TableIexifDataLENS: TStringField
      FieldName = 'LENS'
      Size = 255
    end
    object TableIexifDataISO_SPEED: TIntegerField
      FieldName = 'ISO_SPEED'
    end
    object TableIexifDataFNUMBER: TFloatField
      FieldName = 'FNUMBER'
    end
    object TableIexifDataEXPOSURE_TIME: TFloatField
      FieldName = 'EXPOSURE_TIME'
    end
    object TableIexifDataFOCAL_LENGTH: TIntegerField
      FieldName = 'FOCAL_LENGTH'
    end
    object TableIexifDataSOFTWARE: TStringField
      FieldName = 'SOFTWARE'
      Size = 255
    end
    object TableIexifDataARTIST: TStringField
      FieldName = 'ARTIST'
      Size = 255
    end
    object TableIexifDataCOPYRIGHT: TStringField
      FieldName = 'COPYRIGHT'
      Size = 255
    end
    object TableIexifDataUSERCOMMENT: TStringField
      FieldName = 'USERCOMMENT'
      Size = 255
    end
    object TableIexifDataIMAGE_DESCRIPTION: TMemoField
      FieldName = 'IMAGE_DESCRIPTION'
      BlobType = ftMemo
      Size = 1
    end
    object TableIexifDataEXPOSURE_PROGRAM: TIntegerField
      FieldName = 'EXPOSURE_PROGRAM'
    end
    object TableIexifDataMETERING_MODE: TIntegerField
      FieldName = 'METERING_MODE'
    end
    object TableIexifDataEXPOSURE_COMPENSATION: TFloatField
      FieldName = 'EXPOSURE_COMPENSATION'
    end
    object TableIexifDataORIGINAL_DATE_TIME: TDateTimeField
      FieldName = 'ORIGINAL_DATE_TIME'
    end
    object TableIexifDataFLASH: TStringField
      FieldName = 'FLASH'
      Size = 1
    end
    object TableIexifDataRED_EYE: TStringField
      FieldName = 'RED_EYE'
      Size = 1
    end
  end
  object QueryCatAttributes: TQuery
    DatabaseName = 'docManDB'
    SQL.Strings = (
      'select *'
      'from C_ATTR_DEFS'
      'where CATEGORY_ID=:theCategory'
      'and CATEGORY_VERSION = :theVersion'
      'order by ORDER_NR')
    Left = 32
    Top = 304
    ParamData = <
      item
        DataType = ftInteger
        Name = 'theCategory'
        ParamType = ptInput
      end
      item
        DataType = ftInteger
        Name = 'theVersion'
        ParamType = ptInput
      end>
    object QueryCatAttributesID: TIntegerField
      FieldName = 'ID'
      Origin = 'DOCMANDB."C_ATTR_DEFS.DB".ID'
    end
    object QueryCatAttributesCATEGORY_ID: TIntegerField
      FieldName = 'CATEGORY_ID'
      Origin = 'DOCMANDB."C_ATTR_DEFS.DB".CATEGORY_ID'
    end
    object QueryCatAttributesCATEGORY_VERSION: TIntegerField
      FieldName = 'CATEGORY_VERSION'
      Origin = 'DOCMANDB."C_ATTR_DEFS.DB".CATEGORY_VERSION'
    end
    object QueryCatAttributesATTR_ID: TIntegerField
      FieldName = 'ATTR_ID'
      Origin = 'DOCMANDB."C_ATTR_DEFS.DB".ATTR_ID'
    end
    object QueryCatAttributesNAME: TStringField
      FieldName = 'NAME'
      Origin = 'DOCMANDB."C_ATTR_DEFS.DB".NAME'
      Size = 255
    end
    object QueryCatAttributesTYPE: TIntegerField
      FieldName = 'TYPE'
      Origin = 'DOCMANDB."C_ATTR_DEFS.DB".TYPE'
    end
    object QueryCatAttributesORDER_NR: TIntegerField
      FieldName = 'ORDER_NR'
      Origin = 'DOCMANDB."C_ATTR_DEFS.DB".ORDER_NR'
    end
    object QueryCatAttributesREQUIRED: TIntegerField
      FieldName = 'REQUIRED'
      Origin = 'DOCMANDB."C_ATTR_DEFS.DB".REQUIRED'
    end
    object QueryCatAttributesMIN_COUNT: TIntegerField
      FieldName = 'MIN_COUNT'
      Origin = 'DOCMANDB."C_ATTR_DEFS.DB".MIN_COUNT'
    end
    object QueryCatAttributesMAX_COUNT: TIntegerField
      FieldName = 'MAX_COUNT'
      Origin = 'DOCMANDB."C_ATTR_DEFS.DB".MAX_COUNT'
    end
  end
  object QueryAttrValues: TQuery
    DatabaseName = 'docManDB'
    SQL.Strings = (
      'select * from C_ATTR_VALUES'
      'where ITEM_ID = :theItem'
      'and ITEM_VERSION = :theVersion')
    Left = 144
    Top = 304
    ParamData = <
      item
        DataType = ftInteger
        Name = 'theItem'
        ParamType = ptInput
      end
      item
        DataType = ftInteger
        Name = 'theVersion'
        ParamType = ptInput
      end>
    object QueryAttrValuesID: TIntegerField
      FieldName = 'ID'
      Origin = 'DOCMANDB."C_ATTR_VALUES.DB".ID'
    end
    object QueryAttrValuesITEM_ID: TIntegerField
      FieldName = 'ITEM_ID'
      Origin = 'DOCMANDB."C_ATTR_VALUES.DB".ITEM_ID'
    end
    object QueryAttrValuesITEM_VERSION: TIntegerField
      FieldName = 'ITEM_VERSION'
      Origin = 'DOCMANDB."C_ATTR_VALUES.DB".ITEM_VERSION'
    end
    object QueryAttrValuesATTR_ID: TIntegerField
      FieldName = 'ATTR_ID'
      Origin = 'DOCMANDB."C_ATTR_VALUES.DB".ATTR_ID'
    end
    object QueryAttrValuesORDER_NR: TIntegerField
      FieldName = 'ORDER_NR'
      Origin = 'DOCMANDB."C_ATTR_VALUES.DB".ORDER_NR'
    end
    object QueryAttrValuesDATE_VALUE: TDateTimeField
      FieldName = 'DATE_VALUE'
      Origin = 'DOCMANDB."C_ATTR_VALUES.DB".DATE_VALUE'
    end
    object QueryAttrValuesINTEGER_VALUE: TIntegerField
      FieldName = 'INTEGER_VALUE'
      Origin = 'DOCMANDB."C_ATTR_VALUES.DB".INTEGER_VALUE'
    end
    object QueryAttrValuesFLOAT_VALUE: TFloatField
      FieldName = 'FLOAT_VALUE'
      Origin = 'DOCMANDB."C_ATTR_VALUES.DB".FLOAT_VALUE'
    end
    object QueryAttrValuesSTRING_VALUE: TStringField
      FieldName = 'STRING_VALUE'
      Origin = 'DOCMANDB."C_ATTR_VALUES.DB".STRING_VALUE'
      Size = 255
    end
  end
  object TableAttrValues: TTable
    DatabaseName = 'docManDB'
    TableName = 'C_ATTR_VALUES'
    Left = 232
    Top = 304
    object TableAttrValuesID: TIntegerField
      FieldName = 'ID'
      Required = True
    end
    object TableAttrValuesITEM_ID: TIntegerField
      FieldName = 'ITEM_ID'
      Required = True
    end
    object TableAttrValuesITEM_VERSION: TIntegerField
      FieldName = 'ITEM_VERSION'
    end
    object TableAttrValuesATTR_ID: TIntegerField
      FieldName = 'ATTR_ID'
      Required = True
    end
    object TableAttrValuesORDER_NR: TIntegerField
      FieldName = 'ORDER_NR'
      Required = True
    end
    object TableAttrValuesDATE_VALUE: TDateTimeField
      FieldName = 'DATE_VALUE'
    end
    object TableAttrValuesINTEGER_VALUE: TIntegerField
      FieldName = 'INTEGER_VALUE'
    end
    object TableAttrValuesFLOAT_VALUE: TFloatField
      FieldName = 'FLOAT_VALUE'
    end
    object TableAttrValuesSTRING_VALUE: TStringField
      FieldName = 'STRING_VALUE'
      Size = 255
    end
  end
  object QueryCategory: TQuery
    DatabaseName = 'docManDB'
    SQL.Strings = (
      'select distinct c.ID, c.NAME, d.CATEGORY_VERSION'
      'from categories c, c_attr_defs d, c_attr_values v'
      'where c.id = d.category_id'
      'and d.id = v.attr_id'
      'and v.item_id = :theId'
      'and v.item_version = :theVersion'
      'order by name'
      ' '
      ' ')
    Left = 152
    Top = 224
    ParamData = <
      item
        DataType = ftInteger
        Name = 'theId'
        ParamType = ptInput
      end
      item
        DataType = ftInteger
        Name = 'theVersion'
        ParamType = ptInput
      end>
    object QueryCategoryID: TIntegerField
      FieldName = 'ID'
      Origin = 'DOCMANDB."categories.DB".ID'
    end
    object QueryCategoryNAME: TStringField
      FieldName = 'NAME'
      Origin = 'DOCMANDB."categories.DB".NAME'
      Size = 255
    end
    object QueryCategoryCATEGORY_VERSION: TIntegerField
      FieldName = 'CATEGORY_VERSION'
      Origin = 'DOCMANDB."C_ATTR_DEFS.DB".CATEGORY_VERSION'
    end
  end
  object QueryDocVersions: TQuery
    DatabaseName = 'docManDB'
    SQL.Strings = (
      'select i.ID, i.ITEM_TYPE, i.NAME, fv.VERSION'
      'from ITEM_TREE i'
      'left outer join I_FILE_VERS fv on i.FILE_ID = fv.FILE_ID'
      ''
      ''
      ' ')
    Left = 32
    Top = 368
    object QueryDocVersionsID: TIntegerField
      FieldName = 'ID'
      Origin = 'DOCMANDB."ITEM_TREE.DB".ID'
    end
    object QueryDocVersionsVERSION: TIntegerField
      FieldName = 'VERSION'
      Origin = 'DOCMANDB."ITEM_TREE.DB".ID'
    end
    object QueryDocVersionsITEM_TYPE: TIntegerField
      FieldName = 'ITEM_TYPE'
      Origin = 'DOCMANDB."ITEM_TREE.DB".ITEM_TYPE'
    end
    object QueryDocVersionsNAME: TStringField
      FieldName = 'NAME'
      Origin = 'DOCMANDB."ITEM_TREE.DB".NAME'
      Size = 255
    end
  end
  object QueryStorageItems: TQuery
    DatabaseName = 'docManDB'
    SQL.Strings = (
      'select'#9'i.ID as ITEM_ID, i.NAME, i.VOLUME_ID,'
      #9#9'f.ID as FILE_ID, f.version as CUR_VERSION,'
      #9#9'v.ID as VERSION_ID, v.VERSION, v.MIME_TYPE,'
      #9#9's.ID as STORAGE_ID, s.FILE_PATH'
      'from I_STORAGE s'
      'join I_FILE_VERS v on v.STORAGE_ID = s.ID'
      'join ITEM_TREE i on v.FILE_ID = i.FILE_ID'
      'join I_FILES f on i.FILE_ID = f.ID'
      'where s.ID = :storageID'
      ' '
      ' '
      ' '
      ' '
      ' ')
    Left = 128
    Top = 368
    ParamData = <
      item
        DataType = ftInteger
        Name = 'storageID'
        ParamType = ptInput
        Value = '0'
      end>
    object QueryStorageItemsITEM_ID: TIntegerField
      FieldName = 'ITEM_ID'
    end
    object QueryStorageItemsNAME: TStringField
      FieldName = 'NAME'
      Size = 255
    end
    object QueryStorageItemsVOLUME_ID: TIntegerField
      FieldName = 'VOLUME_ID'
    end
    object QueryStorageItemsFILE_ID: TIntegerField
      FieldName = 'FILE_ID'
    end
    object QueryStorageItemsCUR_VERSION: TIntegerField
      FieldName = 'CUR_VERSION'
    end
    object QueryStorageItemsVERSION_ID: TIntegerField
      FieldName = 'VERSION_ID'
    end
    object QueryStorageItemsVERSION: TIntegerField
      FieldName = 'VERSION'
    end
    object QueryStorageItemsMIME_TYPE: TStringField
      FieldName = 'MIME_TYPE'
      Size = 255
    end
    object QueryStorageItemsSTORAGE_ID: TIntegerField
      FieldName = 'STORAGE_ID'
    end
    object QueryStorageItemsFILE_PATH: TStringField
      FieldName = 'FILE_PATH'
      Size = 255
    end
  end
  object QueryAllStorageItems: TQuery
    DatabaseName = 'docManDB'
    SQL.Strings = (
      'select'#9'i.ID as ITEM_ID, i.NAME, i.VOLUME_ID,'
      #9#9'f.ID as FILE_ID, f.version as CUR_VERSION,'
      #9#9'v.ID as VERSION_ID, v.VERSION, v.MIME_TYPE,'
      #9#9's.ID as STORAGE_ID, s.FILE_PATH'
      'from I_STORAGE s'
      'join I_FILE_VERS v on v.STORAGE_ID = s.ID'
      'join ITEM_TREE i on v.FILE_ID = i.FILE_ID'
      'join I_FILES f on i.FILE_ID = f.ID'
      'where s.ID > :storageID'
      ' '
      ' ')
    Left = 240
    Top = 368
    ParamData = <
      item
        DataType = ftInteger
        Name = 'storageID'
        ParamType = ptInput
        Value = '0'
      end>
    object QueryAllStorageItemsITEM_ID: TIntegerField
      FieldName = 'ITEM_ID'
    end
    object QueryAllStorageItemsNAME: TStringField
      FieldName = 'NAME'
      Size = 255
    end
    object QueryAllStorageItemsVOLUME_ID: TIntegerField
      FieldName = 'VOLUME_ID'
    end
    object QueryAllStorageItemsFILE_ID: TIntegerField
      FieldName = 'FILE_ID'
    end
    object QueryAllStorageItemsCUR_VERSION: TIntegerField
      FieldName = 'CUR_VERSION'
    end
    object QueryAllStorageItemsVERSION_ID: TIntegerField
      FieldName = 'VERSION_ID'
    end
    object QueryAllStorageItemsVERSION: TIntegerField
      FieldName = 'VERSION'
    end
    object QueryAllStorageItemsMIME_TYPE: TStringField
      FieldName = 'MIME_TYPE'
      Size = 255
    end
    object QueryAllStorageItemsSTORAGE_ID: TIntegerField
      FieldName = 'STORAGE_ID'
    end
    object QueryAllStorageItemsFILE_PATH: TStringField
      FieldName = 'FILE_PATH'
      Size = 255
    end
  end
end
