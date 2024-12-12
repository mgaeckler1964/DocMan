object TaskStatusForm: TTaskStatusForm
  Left = 277
  Top = 296
  Width = 650
  Height = 284
  Caption = 'Task Status'
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
    Width = 642
    Height = 41
    Align = alTop
    TabOrder = 0
    object DBNavigator: TDBNavigator
      Left = 8
      Top = 8
      Width = 240
      Height = 25
      DataSource = DataSourceTaskStatus
      TabOrder = 0
    end
  end
  object DBGrid: TDBGrid
    Left = 0
    Top = 41
    Width = 642
    Height = 216
    Align = alClient
    DataSource = DataSourceTaskStatus
    TabOrder = 1
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'MS Sans Serif'
    TitleFont.Style = []
  end
  object DataSourceTaskStatus: TDataSource
    DataSet = DocManDataModule.TableTaskStatus
    Left = 112
    Top = 56
  end
end
