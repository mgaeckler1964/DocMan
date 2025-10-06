object FileLinksForm: TFileLinksForm
  Left = 435
  Top = 218
  Width = 562
  Height = 340
  Caption = 'File Links'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object DBGridLinks: TDBGrid
    Left = 0
    Top = 0
    Width = 546
    Height = 301
    Align = alClient
    DataSource = DataSourceLinks
    TabOrder = 0
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'MS Sans Serif'
    TitleFont.Style = []
  end
  object QueryLinks: TQuery
    OnCalcFields = QueryLinksCalcFields
    DatabaseName = 'docManDB'
    SQL.Strings = (
      'select it.ID'
      'from item_tree it'
      'where it.file_id = :theFileId'
      ' ')
    Left = 48
    Top = 24
    ParamData = <
      item
        DataType = ftInteger
        Name = 'theFileId'
        ParamType = ptInput
      end>
    object QueryLinksID: TIntegerField
      FieldName = 'ID'
      Origin = 'DOCMANDB."item_tree.DB".ID'
      Visible = False
    end
    object QueryLinksPATH: TStringField
      DisplayLabel = 'Name'
      FieldKind = fkCalculated
      FieldName = 'PATH'
      Size = 255
      Calculated = True
    end
  end
  object DataSourceLinks: TDataSource
    DataSet = QueryLinks
    Left = 128
    Top = 24
  end
end
