object PermissionsForm: TPermissionsForm
  Left = 544
  Top = 651
  Width = 408
  Height = 301
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
  object DBGrid1: TDBGrid
    Left = 8
    Top = 40
    Width = 241
    Height = 225
    DataSource = DataSourceACLs
    TabOrder = 0
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'MS Sans Serif'
    TitleFont.Style = []
  end
  object DBNavigator: TDBNavigator
    Left = 8
    Top = 8
    Width = 240
    Height = 25
    DataSource = DataSourceACLs
    TabOrder = 1
  end
  object CheckBoxSeeContents: TCheckBox
    Left = 256
    Top = 48
    Width = 97
    Height = 17
    Caption = 'See Contents'
    TabOrder = 2
    OnClick = PermClicked
  end
  object CheckBoxModify: TCheckBox
    Left = 280
    Top = 72
    Width = 97
    Height = 17
    Caption = 'Modify'
    TabOrder = 3
    OnClick = PermClicked
  end
  object CheckBoxAddVersions: TCheckBox
    Left = 280
    Top = 144
    Width = 89
    Height = 17
    Caption = 'Add Version'
    TabOrder = 4
    OnClick = PermClicked
  end
  object CheckBoxAddItems: TCheckBox
    Left = 280
    Top = 168
    Width = 97
    Height = 17
    Caption = 'Add Items'
    TabOrder = 5
    OnClick = PermClicked
  end
  object CheckBoxDeleteVersions: TCheckBox
    Left = 280
    Top = 216
    Width = 97
    Height = 17
    Caption = 'Delete Version'
    TabOrder = 6
    OnClick = PermClicked
  end
  object CheckBoxEditPermissions: TCheckBox
    Left = 280
    Top = 192
    Width = 97
    Height = 17
    Caption = 'Permissions'
    TabOrder = 7
    OnClick = PermClicked
  end
  object CheckBoxMove: TCheckBox
    Left = 280
    Top = 120
    Width = 97
    Height = 17
    Caption = 'Move'
    TabOrder = 8
    OnClick = PermClicked
  end
  object CheckBoxDelete: TCheckBox
    Left = 296
    Top = 240
    Width = 73
    Height = 17
    Caption = 'Delete'
    TabOrder = 9
    OnClick = PermClicked
  end
  object CheckBoxComplete: TCheckBox
    Left = 304
    Top = 96
    Width = 73
    Height = 17
    Caption = 'Complete'
    TabOrder = 10
    OnClick = PermClicked
  end
  object TableACLs: TTable
    AfterInsert = TableACLsAfterInsert
    BeforePost = TableACLsBeforePost
    AfterScroll = TableACLsAfterScroll
    DatabaseName = 'docManDB'
    TableName = 'I_ACLS.DB'
    Left = 24
    Top = 152
    object TableACLsITEM_ID: TIntegerField
      FieldName = 'ITEM_ID'
      Required = True
      Visible = False
    end
    object TableACLsUSER_ID: TIntegerField
      FieldName = 'USER_ID'
      Required = True
      Visible = False
    end
    object TableACLsPERMISSIONS: TIntegerField
      FieldName = 'PERMISSIONS'
      Required = True
      Visible = False
    end
    object TableACLsUSERNAME: TStringField
      DisplayLabel = 'User or Group'
      FieldKind = fkLookup
      FieldName = 'USERNAME'
      LookupDataSet = QueryAll
      LookupKeyFields = 'id'
      LookupResultField = 'username'
      KeyFields = 'USER_ID'
      Lookup = True
    end
  end
  object DataSourceACLs: TDataSource
    DataSet = TableACLs
    Left = 88
    Top = 160
  end
  object QueryAll: TQuery
    DatabaseName = 'docManDB'
    SQL.Strings = (
      'select id, username from user_tab order by username')
    Left = 168
    Top = 168
  end
end
