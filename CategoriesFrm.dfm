object CategoriesForm: TCategoriesForm
  Left = 496
  Top = 92
  Width = 707
  Height = 470
  Caption = 'Categories'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 699
    Height = 41
    Align = alTop
    TabOrder = 0
    object DBNavigator1: TDBNavigator
      Left = 8
      Top = 8
      Width = 240
      Height = 25
      DataSource = DataSourceCategories
      TabOrder = 0
    end
  end
  object DBGrid1: TDBGrid
    Left = 0
    Top = 41
    Width = 265
    Height = 402
    Align = alLeft
    DataSource = DataSourceCategories
    TabOrder = 1
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'MS Sans Serif'
    TitleFont.Style = []
  end
  object Panel2: TPanel
    Left = 265
    Top = 41
    Width = 434
    Height = 402
    Align = alClient
    TabOrder = 2
    object Label1: TLabel
      Left = 168
      Top = 40
      Width = 24
      Height = 13
      Caption = 'Type'
    end
    object Label2: TLabel
      Left = 168
      Top = 64
      Width = 51
      Height = 13
      Caption = 'Min Count:'
    end
    object Label3: TLabel
      Left = 168
      Top = 88
      Width = 54
      Height = 13
      Caption = 'Max. Count'
    end
    object ComboBoxCatVersion: TComboBox
      Left = 8
      Top = 8
      Width = 145
      Height = 21
      Style = csDropDownList
      ItemHeight = 13
      TabOrder = 0
      OnChange = ComboBoxCatVersionChange
    end
    object StringGridAttributes: TStringGrid
      Left = 8
      Top = 40
      Width = 145
      Height = 353
      Anchors = [akLeft, akTop, akBottom]
      ColCount = 1
      FixedCols = 0
      FixedRows = 0
      Options = [goVertLine, goHorzLine, goEditing]
      TabOrder = 2
      OnSelectCell = StringGridAttributesSelectCell
      OnSetEditText = StringGridAttributesSetEditText
    end
    object CheckBoxRequired: TCheckBox
      Left = 168
      Top = 112
      Width = 97
      Height = 17
      Caption = 'Required'
      TabOrder = 6
      OnClick = CheckBoxRequiredClick
    end
    object ButtonUp: TButton
      Left = 168
      Top = 136
      Width = 75
      Height = 25
      Caption = 'Up'
      TabOrder = 7
      OnClick = ButtonUpClick
    end
    object ButtonDown: TButton
      Left = 248
      Top = 136
      Width = 75
      Height = 25
      Caption = 'Down'
      TabOrder = 8
      OnClick = ButtonDownClick
    end
    object ButtonNew: TButton
      Left = 168
      Top = 168
      Width = 75
      Height = 25
      Caption = 'New'
      TabOrder = 9
      OnClick = ButtonNewClick
    end
    object ButtonRemove: TButton
      Left = 248
      Top = 168
      Width = 75
      Height = 25
      Caption = 'Remove'
      TabOrder = 10
      OnClick = ButtonRemoveClick
    end
    object ButtonSave: TButton
      Left = 168
      Top = 200
      Width = 75
      Height = 25
      Caption = 'Save'
      TabOrder = 11
      OnClick = ButtonSaveClick
    end
    object ComboBoxType: TComboBox
      Left = 248
      Top = 40
      Width = 145
      Height = 21
      Style = csDropDownList
      ItemHeight = 13
      TabOrder = 3
      OnChange = ComboBoxTypeChange
      Items.Strings = (
        'String'
        'Date'
        'Time'
        'Number'
        'Integer')
    end
    object EditMinCount: TEdit
      Left = 248
      Top = 64
      Width = 145
      Height = 21
      TabOrder = 4
      OnChange = EditMinCountChange
    end
    object EditMaxCount: TEdit
      Left = 248
      Top = 88
      Width = 145
      Height = 21
      TabOrder = 5
      OnChange = EditMaxCountChange
    end
    object ButtonPurgeVersions: TButton
      Left = 160
      Top = 8
      Width = 89
      Height = 25
      Caption = 'Purge Versions'
      TabOrder = 1
      OnClick = ButtonPurgeVersionsClick
    end
  end
  object TableCategories: TTable
    AfterInsert = TableCategoriesAfterInsert
    BeforeDelete = TableCategoriesBeforeDelete
    BeforeScroll = TableCategoriesBeforeScroll
    AfterScroll = TableCategoriesAfterScroll
    DatabaseName = 'docManDB'
    IndexName = 'CAT_NAME_IDX'
    TableName = 'CATEGORIES'
    Left = 40
    Top = 160
    object TableCategoriesID: TIntegerField
      FieldName = 'ID'
      Required = True
      Visible = False
    end
    object TableCategoriesNAME: TStringField
      DisplayLabel = 'Name'
      FieldName = 'NAME'
      Required = True
      Size = 255
    end
    object TableCategoriesMAX_ATTR_ID: TIntegerField
      FieldName = 'MAX_ATTR_ID'
      Required = True
      Visible = False
    end
  end
  object DataSourceCategories: TDataSource
    DataSet = TableCategories
    Left = 56
    Top = 96
  end
  object QuerySaveAttributes: TQuery
    DatabaseName = 'docManDB'
    SQL.Strings = (
      'insert into C_ATTR_DEFS'
      '('
      #9'ID,'
      #9'CATEGORY_ID, CATEGORY_VERSION,'
      #9'ATTR_ID,'
      #9'NAME, TYPE, ORDER_NR,'
      #9'REQUIRED,'
      #9'MIN_COUNT, MAX_COUNT'
      ')'
      'values'
      '('
      #9':ID,'
      #9':CATEGORY_ID, :CATEGORY_VERSION,'
      #9':ATTR_ID,'
      #9':NAME, :TYPE, :ORDER_NR,'
      #9':REQUIRED,'
      #9':MIN_COUNT, :MAX_COUNT'
      ')'
      ' '
      ' ')
    Left = 48
    Top = 368
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'ID'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'CATEGORY_ID'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'CATEGORY_VERSION'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'ATTR_ID'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'NAME'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'TYPE'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'ORDER_NR'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'REQUIRED'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'MIN_COUNT'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'MAX_COUNT'
        ParamType = ptUnknown
      end>
  end
  object QueryDeleteAttributes: TQuery
    DatabaseName = 'docManDB'
    SQL.Strings = (
      'delete from C_ATTR_DEFS d'
      'where CATEGORY_ID = :theCategory '
      ' ')
    Left = 80
    Top = 288
    ParamData = <
      item
        DataType = ftInteger
        Name = 'theCategory'
        ParamType = ptInput
      end>
  end
  object QueryPurgeAttributes: TQuery
    DatabaseName = 'docManDB'
    SQL.Strings = (
      'delete from C_ATTR_DEFS d'
      'where CATEGORY_ID = :theCategory'
      'and not exists'
      '('
      #9'select * from C_ATTR_VALUES v'
      #9'where v.ATTR_ID = d.ID'
      ')'
      'and d.category_version != :theVersion'
      ' '
      ' '
      ' ')
    Left = 176
    Top = 280
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
  end
  object QueryCatVersions: TQuery
    DatabaseName = 'docManDB'
    SQL.Strings = (
      'select distinct CATEGORY_VERSION'
      'from C_ATTR_DEFS'
      'where CATEGORY_ID = :theCategory'
      'order by CATEGORY_VERSION desc')
    Left = 40
    Top = 232
    ParamData = <
      item
        DataType = ftInteger
        Name = 'theCategory'
        ParamType = ptInput
      end>
    object QueryCatVersionsCATEGORY_VERSION: TIntegerField
      FieldName = 'CATEGORY_VERSION'
      Origin = 'DOCMANDB."C_ATTR_DEFS.DB".CATEGORY_VERSION'
    end
  end
  object QueryItems: TQuery
    DatabaseName = 'docManDB'
    SQL.Strings = (
      'select distinct ID'
      'from ITEM_TREE it'
      'where exists'
      '('
      #9'select *'
      #9'from C_ATTR_DEFS cd, C_ATTR_VALUES cv'
      #9'where cv.ATTR_ID = cd.ID'
      #9'and it.ID = cv.ITEM_ID'
      #9'and cd.category_version <> :theVersion1'
      ')'
      'and not exists'
      '('
      #9'select *'
      #9'from C_ATTR_DEFS cd, C_ATTR_VALUES cv'
      #9'where cv.ATTR_ID = cd.ID'
      #9'and it.ID = cv.ITEM_ID'
      #9'and cd.category_version = :theVersion2'
      ')'
      ' '
      ' ')
    Left = 193
    Top = 169
    ParamData = <
      item
        DataType = ftInteger
        Name = 'theVersion1'
        ParamType = ptInput
      end
      item
        DataType = ftInteger
        Name = 'theVersion2'
        ParamType = ptInput
      end>
    object QueryItemsID: TIntegerField
      FieldName = 'ID'
    end
  end
  object QueryCountUpgradeItems: TQuery
    DatabaseName = 'docManDB'
    SQL.Strings = (
      'select count(*) as COUNTER'
      'from ITEM_TREE it'
      'where exists'
      '('
      #9'select *'
      #9'from C_ATTR_DEFS cd, C_ATTR_VALUES cv'
      #9'where cv.ATTR_ID = cd.ID'
      #9'and it.ID = cv.ITEM_ID'
      #9'and cd.CATEGORY_VERSION <> :theVersion1'
      #9'and cd.CATEGORY_ID = :theCatId1'
      ')'
      'and not exists'
      '('
      #9'select *'
      #9'from C_ATTR_DEFS cd, C_ATTR_VALUES cv'
      #9'where cv.ATTR_ID = cd.ID'
      #9'and it.ID = cv.ITEM_ID'
      #9'and cd.CATEGORY_VERSION = :theVersion2'
      #9'and cd.CATEGORY_ID = :theCatId2'
      ')'
      ' '
      ' '
      ' '
      ' '
      ' ')
    Left = 201
    Top = 105
    ParamData = <
      item
        DataType = ftInteger
        Name = 'theVersion1'
        ParamType = ptInput
      end
      item
        DataType = ftInteger
        Name = 'theCatId1'
        ParamType = ptInput
      end
      item
        DataType = ftInteger
        Name = 'theVersion2'
        ParamType = ptInput
      end
      item
        DataType = ftInteger
        Name = 'theCatId2'
        ParamType = ptInput
      end>
    object QueryCountUpgradeItemsCOUNTER: TIntegerField
      FieldName = 'COUNTER'
    end
  end
end
