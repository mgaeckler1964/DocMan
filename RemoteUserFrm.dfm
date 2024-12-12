object RemoteUserForm: TRemoteUserForm
  Left = 167
  Top = 438
  Width = 629
  Height = 338
  Caption = 'Remote Users'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnHide = FormHide
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object DBGridUserTable: TDBGrid
    Left = 0
    Top = 65
    Width = 621
    Height = 246
    Align = alClient
    DataSource = DataSourceUserTable
    TabOrder = 0
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'MS Sans Serif'
    TitleFont.Style = []
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 621
    Height = 65
    Align = alTop
    TabOrder = 1
    object SpeedButtonRefresh: TSpeedButton
      Left = 232
      Top = 32
      Width = 25
      Height = 25
      Glyph.Data = {
        42010000424D4201000000000000420000002800000010000000100000000100
        08000000000000010000C30E0000C30E00000300000000000000000000008080
        8000C8D0D4000202020202020202020202020202020202020202020202020202
        0202020202020202020202020202020202020202020202020202020201000102
        0202020202020202020201000001020202020202020202020202000001020202
        0202020202020202020100010202020202020202020202020200000202020202
        0202020202020202020000020202000000000002020202020200000102020201
        0000000202020202020100000102010000000002020202020202000000000000
        0001000202020202020202010000000102020002020202020202020202020202
        0202020202020202020202020202020202020202020202020202020202020202
        020202020202}
      OnClick = SpeedButtonRefreshClick
    end
    object DBNavigator: TDBNavigator
      Left = 8
      Top = 32
      Width = 225
      Height = 25
      DataSource = DataSourceUserTable
      VisibleButtons = [nbFirst, nbPrior, nbNext, nbLast, nbInsert, nbDelete, nbEdit, nbPost, nbCancel]
      TabOrder = 1
    end
    object DBCheckBoxGuest: TDBCheckBox
      Left = 272
      Top = 40
      Width = 57
      Height = 17
      Caption = 'Guest'
      DataField = 'GUEST'
      DataSource = DataSourceUserTable
      TabOrder = 2
      ValueChecked = 'X'
      ValueUnchecked = '-'
    end
    object DBCheckBoxLoginEnabled: TDBCheckBox
      Left = 328
      Top = 40
      Width = 57
      Height = 17
      Caption = 'Login'
      DataField = 'LOGINENABLED'
      DataSource = DataSourceUserTable
      TabOrder = 3
      ValueChecked = 'X'
      ValueUnchecked = '-'
    end
    object DBCheckBoxAdmin: TDBCheckBox
      Left = 384
      Top = 40
      Width = 97
      Height = 17
      Caption = 'Admin'
      DataField = 'ADMINISTRATOR'
      DataSource = DataSourceUserTable
      TabOrder = 4
      ValueChecked = 'X'
      ValueUnchecked = '-'
    end
    object ComboBoxRemoteServer: TComboBox
      Left = 8
      Top = 8
      Width = 249
      Height = 21
      Style = csDropDownList
      ItemHeight = 13
      TabOrder = 0
      OnChange = ComboBoxRemoteServerChange
    end
  end
  object ClientDataSetUserTable: TClientDataSet
    Aggregates = <>
    FieldDefs = <>
    IndexDefs = <>
    Params = <>
    StoreDefs = True
    AfterOpen = ClientDataSetUserTableAfterOpen
    AfterInsert = ClientDataSetUserTableAfterInsert
    BeforePost = ClientDataSetUserTableBeforePost
    BeforeDelete = ClientDataSetUserTableBeforeDelete
    Left = 384
    Top = 48
    object ClientDataSetUserTableID: TAutoIncField
      DisplayWidth = 3
      FieldName = 'ID'
      ReadOnly = True
      Visible = False
    end
    object ClientDataSetUserTableVORNAME: TStringField
      DisplayWidth = 32
      FieldName = 'VORNAME'
      Size = 128
    end
    object ClientDataSetUserTableNACHNAME: TStringField
      DisplayWidth = 32
      FieldName = 'NACHNAME'
      Size = 128
    end
    object ClientDataSetUserTableSTRASSE: TStringField
      DisplayWidth = 32
      FieldName = 'STRASSE'
      Size = 255
    end
    object ClientDataSetUserTablePOSTFACH: TStringField
      DisplayWidth = 16
      FieldName = 'POSTFACH'
      Size = 255
    end
    object ClientDataSetUserTableLAND: TStringField
      DisplayWidth = 4
      FieldName = 'LAND'
      Size = 16
    end
    object ClientDataSetUserTablePLZ: TStringField
      DisplayWidth = 6
      FieldName = 'PLZ'
      Size = 16
    end
    object ClientDataSetUserTableORT: TStringField
      DisplayWidth = 32
      FieldName = 'ORT'
      Size = 64
    end
    object ClientDataSetUserTableEMAIL: TStringField
      DisplayWidth = 32
      FieldName = 'EMAIL'
      Required = True
      Size = 128
    end
    object ClientDataSetUserTableGUEST: TStringField
      FieldName = 'GUEST'
      Visible = False
      Size = 1
    end
    object ClientDataSetUserTableLOGINENABLED: TStringField
      FieldName = 'LOGINENABLED'
      Visible = False
      Size = 1
    end
    object ClientDataSetUserTableADMINISTRATOR: TStringField
      FieldName = 'ADMINISTRATOR'
      Visible = False
      Size = 1
    end
  end
  object DataSourceUserTable: TDataSource
    DataSet = ClientDataSetUserTable
    Left = 200
    Top = 48
  end
end
