object AttributesForm: TAttributesForm
  Left = 746
  Top = 248
  Width = 495
  Height = 429
  Caption = 'Attributes'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 487
    Height = 41
    Align = alTop
    TabOrder = 0
    object ComboBoxCategories: TComboBox
      Left = 8
      Top = 8
      Width = 145
      Height = 21
      Style = csDropDownList
      ItemHeight = 13
      TabOrder = 0
      OnChange = ComboBoxCategoriesChange
    end
    object ButtonAdd: TButton
      Left = 160
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Add'
      TabOrder = 1
      OnClick = ButtonAddClick
    end
    object ButtonRemove: TButton
      Left = 240
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Remove'
      TabOrder = 2
      OnClick = ButtonRemoveClick
    end
    object ButtonUpgrade: TButton
      Left = 320
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Upgrade'
      TabOrder = 3
      OnClick = ButtonUpgradeClick
    end
    object ButtonApplyDown: TButton
      Left = 400
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Apply Down'
      TabOrder = 4
      OnClick = ButtonApplyDownClick
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 361
    Width = 487
    Height = 41
    Align = alBottom
    TabOrder = 1
    object ButtonSave: TButton
      Left = 8
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Save'
      Default = True
      TabOrder = 0
      OnClick = ButtonSaveClick
    end
    object ButtonCancel: TButton
      Left = 88
      Top = 8
      Width = 75
      Height = 25
      Cancel = True
      Caption = 'Cancel'
      ModalResult = 2
      TabOrder = 1
    end
  end
  object PageControl: TPageControl
    Left = 0
    Top = 41
    Width = 487
    Height = 320
    Align = alClient
    TabOrder = 2
    OnChange = PageControlChange
  end
  object TableCategories: TTable
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
  object delQuery: TQuery
    DatabaseName = 'docManDB'
    SQL.Strings = (
      'delete from  C_ATTR_VALUES'
      'where ITEM_ID = :theID'
      'and ITEM_VERSION = :theVersion ')
    Left = 144
    Top = 144
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'theID'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'theVersion'
        ParamType = ptUnknown
      end>
  end
  object insertQuery: TQuery
    DatabaseName = 'docManDB'
    SQL.Strings = (
      'insert into C_ATTR_VALUES'
      '('
      #9'ID, ITEM_ID, ITEM_VERSION,'
      #9'ATTR_ID, ORDER_NR,'
      #9'DATE_VALUE,'
      #9'INTEGER_VALUE, FLOAT_VALUE,'
      #9'STRING_VALUE'
      ')'
      'values'
      '('
      #9':ID, :ITEM_ID, :ITEM_VERSION,'
      #9':ATTR_ID, :ORDER_NR,'
      #9':DATE_VALUE,'
      #9':INTEGER_VALUE, :FLOAT_VALUE,'
      #9':STRING_VALUE'
      ')')
    Left = 264
    Top = 152
    ParamData = <
      item
        DataType = ftInteger
        Name = 'ID'
        ParamType = ptInput
      end
      item
        DataType = ftInteger
        Name = 'ITEM_ID'
        ParamType = ptInput
      end
      item
        DataType = ftInteger
        Name = 'ITEM_VERSION'
        ParamType = ptInput
      end
      item
        DataType = ftInteger
        Name = 'ATTR_ID'
        ParamType = ptInput
      end
      item
        DataType = ftInteger
        Name = 'ORDER_NR'
        ParamType = ptInput
      end
      item
        DataType = ftDateTime
        Name = 'DATE_VALUE'
        ParamType = ptInput
      end
      item
        DataType = ftInteger
        Name = 'INTEGER_VALUE'
        ParamType = ptInput
      end
      item
        DataType = ftFloat
        Name = 'FLOAT_VALUE'
        ParamType = ptInput
      end
      item
        DataType = ftString
        Name = 'STRING_VALUE'
        ParamType = ptInput
      end>
  end
  object QueryCategoryForItem: TQuery
    DatabaseName = 'docManDB'
    SQL.Strings = (
      'select distinct c.ID, c.NAME, d.CATEGORY_VERSION'
      'from categories c, c_attr_defs d, c_attr_values v'
      'where c.id = d.category_id'
      'and d.id = v.attr_id'
      'and v.item_id = :theId'
      'and v.item_version = :theVersion'
      'order by name')
    Left = 88
    Top = 232
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
    object QueryCategoryForItemID: TIntegerField
      FieldName = 'ID'
      Origin = 'DOCMANDB."categories.DB".ID'
    end
    object QueryCategoryForItemNAME: TStringField
      FieldName = 'NAME'
      Origin = 'DOCMANDB."categories.DB".NAME'
      Size = 255
    end
    object QueryCategoryForItemCATEGORY_VERSION: TIntegerField
      FieldName = 'CATEGORY_VERSION'
      Origin = 'DOCMANDB."C_ATTR_DEFS.DB".CATEGORY_VERSION'
    end
  end
  object QueryCatVersion: TQuery
    DatabaseName = 'docManDB'
    SQL.Strings = (
      'select max( d.CATEGORY_VERSION ) as CATEGORY_VERSION'
      'from C_ATTR_DEFS d'
      'where d.CATEGORY_ID = :theCategory')
    Left = 344
    Top = 240
    ParamData = <
      item
        DataType = ftInteger
        Name = 'theCategory'
        ParamType = ptInput
      end>
  end
end
