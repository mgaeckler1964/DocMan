object DocManBgMainForm: TDocManBgMainForm
  Left = 730
  Top = 377
  Width = 775
  Height = 421
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
    Top = 363
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
