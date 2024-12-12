object LocalDirsForm: TLocalDirsForm
  Left = 470
  Top = 521
  Width = 790
  Height = 292
  Caption = 'Local Directories'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnCreate = FormCreate
  OnHide = FormHide
  OnShow = ReloadQuery
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 782
    Height = 41
    Align = alTop
    TabOrder = 0
    object SpeedButton: TSpeedButton
      Left = 108
      Top = 8
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
      OnClick = ReloadQuery
    end
    object SpeedButtonDelete: TSpeedButton
      Left = 134
      Top = 8
      Width = 25
      Height = 25
      Glyph.Data = {
        76010000424D7601000000000000760000002800000020000000100000000100
        04000000000000010000120B0000120B00001000000000000000000000000000
        800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333000000000
        3333333777777777F3333330F777777033333337F3F3F3F7F3333330F0808070
        33333337F7F7F7F7F3333330F080707033333337F7F7F7F7F3333330F0808070
        33333337F7F7F7F7F3333330F080707033333337F7F7F7F7F3333330F0808070
        333333F7F7F7F7F7F3F33030F080707030333737F7F7F7F7F7333300F0808070
        03333377F7F7F7F773333330F080707033333337F7F7F7F7F333333070707070
        33333337F7F7F7F7FF3333000000000003333377777777777F33330F88877777
        0333337FFFFFFFFF7F3333000000000003333377777777777333333330777033
        3333333337FFF7F3333333333000003333333333377777333333}
      NumGlyphs = 2
      OnClick = SpeedButtonDeleteClick
    end
    object DBNavigator1: TDBNavigator
      Left = 8
      Top = 8
      Width = 100
      Height = 25
      DataSource = DataSource
      VisibleButtons = [nbFirst, nbPrior, nbNext, nbLast]
      TabOrder = 0
    end
    object CheckBoxAllDirectories: TCheckBox
      Left = 168
      Top = 8
      Width = 97
      Height = 17
      Caption = 'All Directories'
      TabOrder = 1
      OnClick = ReloadQuery
    end
  end
  object DBGrid1: TDBGrid
    Left = 0
    Top = 41
    Width = 782
    Height = 224
    Align = alClient
    DataSource = DataSource
    TabOrder = 1
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'MS Sans Serif'
    TitleFont.Style = []
  end
  object DataSource: TDataSource
    DataSet = QueryLocalDirs
    Left = 24
    Top = 48
  end
  object QueryLocalDirs: TQuery
    OnCalcFields = QueryLocalDirsCalcFields
    DatabaseName = 'docManDB'
    SQL.Strings = (
      
        'select lp.ID, lp.FOLDER_ID, lp.USER_ID, it.NAME, lp.MACHINE, lp.' +
        'LOCAL_PATH, ut.USERNAME'
      'from item_tree it, user_tab ut, i_local_folder lp'
      'where it.ID = lp.FOLDER_ID'
      'and lp.USER_ID = ut.ID')
    Left = 136
    Top = 48
    object QueryLocalDirsPATH: TStringField
      DisplayLabel = 'DocMan Path'
      DisplayWidth = 45
      FieldKind = fkCalculated
      FieldName = 'PATH'
      LookupCache = True
      Size = 255
      Calculated = True
    end
    object QueryLocalDirsSHORT_PATH: TStringField
      DisplayLabel = 'Local Path'
      DisplayWidth = 45
      FieldKind = fkCalculated
      FieldName = 'SHORT_PATH'
      Size = 255
      Calculated = True
    end
    object QueryLocalDirsMACHINE: TStringField
      DisplayLabel = 'Client Machine'
      DisplayWidth = 16
      FieldName = 'MACHINE'
      Origin = 'DOCMANDB."I_LOCAL_FOLDER.DB".MACHINE'
      Size = 255
    end
    object QueryLocalDirsUSERNAME: TStringField
      DisplayLabel = 'Username'
      DisplayWidth = 16
      FieldName = 'USERNAME'
      Size = 64
    end
    object QueryLocalDirsID: TIntegerField
      FieldName = 'ID'
      Origin = 'DOCMANDB."I_LOCAL_FOLDER.DB".ID'
      Visible = False
    end
    object QueryLocalDirsFOLDER_ID: TIntegerField
      FieldName = 'FOLDER_ID'
      Visible = False
    end
    object QueryLocalDirsNAME: TStringField
      DisplayWidth = 32
      FieldName = 'NAME'
      Visible = False
      Size = 255
    end
    object QueryLocalDirsLOCAL_PATH: TMemoField
      FieldName = 'LOCAL_PATH'
      Origin = 'DOCMANDB."I_LOCAL_FOLDER.DB".LOCAL_PATH'
      Visible = False
      BlobType = ftMemo
      Size = 240
    end
    object QueryLocalDirsUSER_ID: TIntegerField
      FieldName = 'USER_ID'
      Origin = 'DOCMANDB."I_LOCAL_FOLDER.DB".USER_ID'
      Visible = False
    end
  end
end
