object MimeTypesForm: TMimeTypesForm
  Left = 327
  Top = 436
  Width = 590
  Height = 286
  Caption = 'Mime Types'
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
    Width = 582
    Height = 41
    Align = alTop
    TabOrder = 0
    object DBNavigatorMimeTypes: TDBNavigator
      Left = 8
      Top = 8
      Width = 240
      Height = 25
      DataSource = DataSourceMimeTypes
      TabOrder = 0
    end
  end
  object DBGridMimeTypes: TDBGrid
    Left = 0
    Top = 41
    Width = 582
    Height = 218
    Align = alClient
    DataSource = DataSourceMimeTypes
    TabOrder = 1
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'MS Sans Serif'
    TitleFont.Style = []
  end
  object DataSourceMimeTypes: TDataSource
    DataSet = TableMimeTypes
    Left = 24
    Top = 48
  end
  object TableMimeTypes: TTable
    BeforePost = TableMimeTypesBeforePost
    DatabaseName = 'docManDB'
    TableName = 'I_MIME_TYPES'
    Left = 112
    Top = 48
    object TableMimeTypesEXTENSION: TStringField
      DisplayLabel = 'Extension'
      DisplayWidth = 10
      FieldName = 'EXTENSION'
      Required = True
      Size = 16
    end
    object TableMimeTypesMIMETYPE: TStringField
      DisplayLabel = 'Mime Type'
      DisplayWidth = 80
      FieldName = 'MIMETYPE'
      Required = True
      Size = 255
    end
  end
end
