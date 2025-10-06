object DocManBgMainForm: TDocManBgMainForm
  Left = 452
  Top = 372
  Width = 775
  Height = 443
  Caption = 'Document Manager BG'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  ShowHint = True
  OnClose = FormClose
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object StatusBar: TStatusBar
    Left = 0
    Top = 385
    Width = 759
    Height = 19
    Panels = <>
    SimplePanel = True
  end
  object ReminderTimer: TTimer
    Enabled = False
    Interval = 10000
    OnTimer = ReminderTimerTimer
    Left = 24
    Top = 56
  end
end
