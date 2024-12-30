object ReminderFilesForm: TReminderFilesForm
  Left = 565
  Top = 217
  Width = 790
  Height = 292
  Caption = 'Reminder Files'
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
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 774
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
      OnClick = SpeedButtonClick
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
  end
  object DBGrid: TDBGrid
    Left = 0
    Top = 41
    Width = 774
    Height = 212
    Align = alClient
    DataSource = DataSource
    TabOrder = 1
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'MS Sans Serif'
    TitleFont.Style = []
    OnDblClick = DBGridDblClick
  end
  object DataSource: TDataSource
    DataSet = QueryOpenFiles
    Left = 24
    Top = 48
  end
  object QueryOpenFiles: TQuery
    OnCalcFields = QueryOpenFilesCalcFields
    DatabaseName = 'docManDB'
    SQL.Strings = (
      'select it.ID, it.ParentID, it.Name, it.ReminderDate'
      'from item_tree it'
      'where'#9'it.ReminderDate < :now and'
      #9#9'it.ReminderDate > 0 and'
      #9#9'it.ReminderDate is not null'
      'order by it.ReminderDate'
      ''
      ' ')
    Left = 136
    Top = 48
    ParamData = <
      item
        DataType = ftInteger
        Name = 'now'
        ParamType = ptInput
        Value = 0
      end>
    object QueryOpenFilesID: TIntegerField
      FieldName = 'ID'
      Visible = False
    end
    object QueryOpenFilesParentID: TIntegerField
      FieldName = 'ParentID'
      Visible = False
    end
    object QueryOpenFilesName: TStringField
      DisplayWidth = 32
      FieldName = 'Name'
      Size = 255
    end
    object QueryOpenFilesPATH: TStringField
      DisplayLabel = 'Path'
      DisplayWidth = 56
      FieldKind = fkCalculated
      FieldName = 'PATH'
      LookupCache = True
      Size = 255
      Calculated = True
    end
    object QueryOpenFilesReminderDate: TIntegerField
      FieldName = 'ReminderDate'
      Origin = 'DOCMANDB."item_tree.DB".REMINDERDATE'
      Visible = False
    end
    object QueryOpenFilesReminder: TDateTimeField
      FieldKind = fkCalculated
      FieldName = 'Reminder'
      Calculated = True
    end
  end
end
