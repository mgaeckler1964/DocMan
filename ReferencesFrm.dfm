object ReferencesForm: TReferencesForm
  Left = 630
  Top = 242
  Width = 562
  Height = 340
  Caption = 'References'
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
  object QueryReferences: TQuery
    OnCalcFields = QueryReferencesCalcFields
    DatabaseName = 'docManDB'
    SQL.Strings = (
      'select it.ID'
      'from item_tree it'
      'where it.original_id = :theID'
      ' '
      ' ')
    Left = 48
    Top = 24
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'theID'
        ParamType = ptUnknown
      end>
    object QueryReferencesID: TIntegerField
      FieldName = 'ID'
      Origin = 'DOCMANDB."item_tree.DB".ID'
      Visible = False
    end
    object QueryReferencesPATH: TStringField
      DisplayLabel = 'Name'
      FieldKind = fkCalculated
      FieldName = 'PATH'
      Size = 255
      Calculated = True
    end
  end
  object DataSourceLinks: TDataSource
    DataSet = QueryReferences
    Left = 128
    Top = 24
  end
end
