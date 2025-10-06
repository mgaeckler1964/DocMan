object VersionsForm: TVersionsForm
  Left = 407
  Top = 275
  Width = 830
  Height = 307
  Caption = 'VersionsForm'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  ShowHint = True
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter1: TSplitter
    Left = 0
    Top = 174
    Width = 814
    Height = 5
    Cursor = crVSplit
    Align = alBottom
  end
  object DBGridVersions: TDBGrid
    Left = 0
    Top = 41
    Width = 814
    Height = 133
    Align = alClient
    DataSource = DataSourceVersions
    DefaultDrawing = False
    TabOrder = 0
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'MS Sans Serif'
    TitleFont.Style = []
    OnDrawColumnCell = DBGridVersionsDrawColumnCell
    OnDblClick = DBGridVersionsDblClick
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 814
    Height = 41
    Align = alTop
    TabOrder = 1
    object SpeedButtonLock: TSpeedButton
      Left = 8
      Top = 8
      Width = 23
      Height = 25
      Hint = 'Lock'
      Glyph.Data = {
        76010000424D7601000000000000760000002800000020000000100000000100
        04000000000000010000120B0000120B00001000000000000000000000000000
        800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333000003
        333333333F777773FF333333008888800333333377333F3773F3333077870787
        7033333733337F33373F3308888707888803337F33337F33337F330777880887
        7703337F33337FF3337F3308888000888803337F333777F3337F330777700077
        7703337F33377733337FB3088888888888033373FFFFFFFFFF733B3000000000
        0033333777777777773333BBBB3333080333333333F3337F7F33BBBB707BB308
        03333333373F337F7F3333BB08033308033333337F7F337F7F333B3B08033308
        033333337F73FF737F33B33B778000877333333373F777337333333B30888880
        33333333373FFFF73333333B3300000333333333337777733333}
      NumGlyphs = 2
      OnClick = SpeedButtonLockClick
    end
    object SpeedButtonUnlock: TSpeedButton
      Left = 8
      Top = 8
      Width = 23
      Height = 25
      Hint = 'Unlock'
      Glyph.Data = {
        76010000424D7601000000000000760000002800000020000000100000000100
        04000000000000010000120B0000120B00001000000000000000000000000000
        800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333000003
        333333333F777773FF333333008888800333333377333F3773F3333077870787
        7033333733337F33373F3308888707888803337F33337F33337F330777880887
        7703337F33337FF3337F3308888000888803337F333777F3337F330777700077
        7703337F33377733337F33088888888888033373FFFFFFFFFF73333000000000
        00333337777777777733333308033308033333337F7F337F7F33333308033308
        033333337F7F337F7F33333308033308033333337F73FF737F33333377800087
        7333333373F77733733333333088888033333333373FFFF73333333333000003
        3333333333777773333333333333333333333333333333333333}
      NumGlyphs = 2
      OnClick = SpeedButtonLockClick
    end
    object SpeedButtonMark: TSpeedButton
      Left = 32
      Top = 8
      Width = 23
      Height = 25
      Hint = 'Mark for Compare'
      Glyph.Data = {
        76010000424D7601000000000000760000002800000020000000100000000100
        04000000000000010000120B0000120B00001000000000000000000000000000
        800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00555555555555
        555555555555555555555555555555555555555555FF55555555555559055555
        55555555577FF5555555555599905555555555557777F5555555555599905555
        555555557777FF5555555559999905555555555777777F555555559999990555
        5555557777777FF5555557990599905555555777757777F55555790555599055
        55557775555777FF5555555555599905555555555557777F5555555555559905
        555555555555777FF5555555555559905555555555555777FF55555555555579
        05555555555555777FF5555555555557905555555555555777FF555555555555
        5990555555555555577755555555555555555555555555555555}
      NumGlyphs = 2
      OnClick = SpeedButtonMarkClick
    end
    object SpeedButtonCompare: TSpeedButton
      Left = 56
      Top = 8
      Width = 23
      Height = 25
      Hint = 'Compare with marked version'
      Enabled = False
      Glyph.Data = {
        76010000424D7601000000000000760000002800000020000000100000000100
        04000000000000010000120B0000120B00001000000000000000000000000000
        800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF003FFFFFFFFFFF
        FFFF33333333333FFFFF3FFFFFFFFF00000F333333333377777F33FFFFFFFF09
        990F33333333337F337F333FFFFFFF09990F33333333337F337F3333FFFFFF09
        990F33333333337FFF7F33333FFFFF00000F3333333333777773333333FFFFFF
        FFFF3333333333333F333333333FFFFF0FFF3333333333337FF333333333FFF0
        00FF33333333333777FF333333333F00000F33FFFFF33777777F300000333000
        0000377777F33777777730EEE033333000FF37F337F3333777F330EEE0333330
        00FF37F337F3333777F330EEE033333000FF37FFF7F333F77733300000333000
        03FF3777773337777333333333333333333F3333333333333333}
      NumGlyphs = 2
      OnClick = SpeedButtonCompareClick
    end
    object SpeedButtonMakeCurrent: TSpeedButton
      Left = 79
      Top = 8
      Width = 23
      Height = 25
      Hint = 'Make Current'
      Glyph.Data = {
        76010000424D7601000000000000760000002800000020000000100000000100
        04000000000000010000120B0000120B00001000000000000000000000000000
        800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333000333
        3333333333777F33333333333309033333333333337F7F333333333333090333
        33333333337F7F33333333333309033333333333337F7F333333333333090333
        33333333337F7F33333333333309033333333333FF7F7FFFF333333000090000
        3333333777737777F333333099999990333333373F3333373333333309999903
        333333337F33337F33333333099999033333333373F333733333333330999033
        3333333337F337F3333333333099903333333333373F37333333333333090333
        33333333337F7F33333333333309033333333333337373333333333333303333
        333333333337F333333333333330333333333333333733333333}
      NumGlyphs = 2
      OnClick = SpeedButtonMakeCurrentClick
    end
    object DBNavigator: TDBNavigator
      Left = 102
      Top = 8
      Width = 230
      Height = 25
      DataSource = DataSourceVersions
      TabOrder = 0
    end
    object ButtonPurgeVersions: TButton
      Left = 416
      Top = 8
      Width = 89
      Height = 25
      Caption = 'Purge Versions'
      TabOrder = 2
      OnClick = ButtonPurgeVersionsClick
    end
    object EditNumVersions: TEdit
      Left = 360
      Top = 8
      Width = 57
      Height = 21
      TabOrder = 1
      Text = '1'
      OnChange = EditNumVersionsChange
      OnKeyPress = EditNumVersionsKeyPress
    end
    object ButtonExif: TButton
      Left = 520
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Exif...'
      TabOrder = 3
      OnClick = ButtonExifClick
    end
    object ButtonAttributes: TButton
      Left = 600
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Attributes...'
      TabOrder = 4
      OnClick = ButtonAttributesClick
    end
  end
  object DBMemoDescription: TDBMemo
    Left = 0
    Top = 179
    Width = 814
    Height = 89
    Align = alBottom
    DataField = 'DESCRIPTION'
    DataSource = DataSourceVersions
    TabOrder = 2
  end
  object TableVersions: TTable
    BeforeInsert = CreateFileVersion
    BeforePost = TableVersionsBeforePost
    BeforeDelete = DeleteFileVersion
    AfterScroll = TableVersionsAfterScroll
    DatabaseName = 'docManDB'
    IndexName = 'FILE_VERSION_IDX'
    TableName = 'I_FILE_VERS.DB'
    Left = 48
    Top = 80
    object TableVersionsVERSION: TIntegerField
      DisplayLabel = 'Version'
      FieldName = 'VERSION'
      ReadOnly = True
      Required = True
    end
    object TableVersionsMIME_TYPE: TStringField
      DisplayLabel = 'Mimetype'
      DisplayWidth = 32
      FieldName = 'MIME_TYPE'
      KeyFields = 'MIME_TYPE'
      Required = True
      Size = 255
    end
    object TableVersionsFILENAME: TStringField
      DisplayLabel = 'Filename'
      DisplayWidth = 32
      FieldName = 'FILENAME'
      ReadOnly = True
      Size = 255
    end
    object TableVersionsFILESIZE: TIntegerField
      DisplayLabel = 'File Size'
      FieldName = 'FILESIZE'
      Required = True
    end
    object TableVersionsCREATORNAME: TStringField
      DisplayLabel = 'Created By'
      FieldKind = fkLookup
      FieldName = 'CREATORNAME'
      LookupDataSet = QueryUser
      LookupKeyFields = 'id'
      LookupResultField = 'username'
      KeyFields = 'CREATEDBY'
      ReadOnly = True
      Lookup = True
    end
    object TableVersionsCREATEDDATE: TDateTimeField
      DisplayLabel = 'Creation Date'
      FieldName = 'CREATEDDATE'
      ReadOnly = True
      Required = True
    end
    object TableVersionsMODIFIEDDATE: TDateTimeField
      DisplayLabel = 'Modified Date'
      FieldName = 'MODIFIEDDATE'
      ReadOnly = True
      Required = True
    end
    object TableVersionsFILECREATEDDATE: TDateTimeField
      DisplayLabel = 'File Creation Date'
      FieldName = 'FILECREATEDDATE'
      Required = True
    end
    object TableVersionsFILEMODIFIEDDATE: TDateTimeField
      DisplayLabel = 'File Modified Date'
      FieldName = 'FILEMODIFIEDDATE'
      Required = True
    end
    object TableVersionsID: TIntegerField
      FieldName = 'ID'
      Required = True
      Visible = False
    end
    object TableVersionsFILE_ID: TIntegerField
      FieldName = 'FILE_ID'
      Required = True
      Visible = False
    end
    object TableVersionsSTORAGE_ID: TIntegerField
      FieldName = 'STORAGE_ID'
      Required = True
      Visible = False
    end
    object TableVersionsCREATEDBY: TIntegerField
      FieldName = 'CREATEDBY'
      ReadOnly = True
      Required = True
      Visible = False
    end
    object TableVersionsLOCKED: TIntegerField
      DisplayLabel = 'Locked'
      FieldName = 'LOCKED'
      Visible = False
    end
    object TableVersionsDESCRIPTION: TMemoField
      FieldName = 'DESCRIPTION'
      Visible = False
      BlobType = ftMemo
      Size = 1
    end
  end
  object DataSourceVersions: TDataSource
    DataSet = TableVersions
    Left = 304
    Top = 88
  end
  object QueryUser: TQuery
    DatabaseName = 'docManDB'
    SQL.Strings = (
      'select id, username from user_tab')
    Left = 56
    Top = 160
  end
  object QueryMimeTypes: TQuery
    DatabaseName = 'docManDB'
    SQL.Strings = (
      'select distinct MIMETYPE'
      'from I_MIME_TYPES'
      'order by MIMETYPE')
    Left = 184
    Top = 160
    object QueryMimeTypesMIMETYPE: TStringField
      FieldName = 'MIMETYPE'
      Origin = 'DOCMANDB."I_MIME_TYPES.DB".MIMETYPE'
      Size = 255
    end
  end
  object ClientDataSetVersions: TClientDataSet
    Aggregates = <>
    Params = <>
    AfterOpen = ClientDataSetVersionsAfterOpen
    BeforeInsert = CreateFileVersion
    BeforePost = ClientDataSetVersionsBeforePost
    BeforeDelete = DeleteFileVersion
    Left = 160
    Top = 80
    object ClientDataSetVersionsID: TIntegerField
      FieldName = 'ID'
      ReadOnly = True
      Visible = False
    end
    object ClientDataSetVersionsitemID: TIntegerField
      FieldName = 'itemID'
      ReadOnly = True
      Visible = False
    end
    object ClientDataSetVersionscreateBy: TIntegerField
      DisplayLabel = 'Created By'
      FieldName = 'createBy'
      ReadOnly = True
    end
    object ClientDataSetVersionsmimeType: TStringField
      DisplayLabel = 'Mimetype'
      DisplayWidth = 32
      FieldName = 'mimeType'
      Size = 255
    end
    object ClientDataSetVersionscreateDate: TDateTimeField
      DisplayLabel = 'Creation Date'
      FieldName = 'createDate'
      ReadOnly = True
    end
    object ClientDataSetVersionsmodifiedDate: TDateTimeField
      DisplayLabel = 'Modified Date'
      FieldName = 'modifiedDate'
      ReadOnly = True
    end
    object ClientDataSetVersionsDESCRIPTION: TMemoField
      FieldName = 'DESCRIPTION'
      Visible = False
      BlobType = ftMemo
    end
  end
end
