object WebServerForm: TWebServerForm
  Left = 336
  Top = 659
  Width = 613
  Height = 141
  Caption = 'Web Server'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 16
    Width = 49
    Height = 13
    Caption = 'Path/Port:'
  end
  object LabelLastUrl: TLabel
    Left = 8
    Top = 48
    Width = 585
    Height = 13
    AutoSize = False
    Caption = 'Last URL'
  end
  object LabelStatus: TLabel
    Left = 192
    Top = 72
    Width = 36
    Height = 13
    Caption = 'Waiting'
  end
  object ButtonStart: TButton
    Left = 16
    Top = 72
    Width = 75
    Height = 25
    Caption = 'Start'
    TabOrder = 2
    OnClick = ButtonStartClick
  end
  object ButtonStop: TButton
    Left = 96
    Top = 72
    Width = 75
    Height = 25
    Caption = 'Stop'
    TabOrder = 3
    OnClick = ButtonStopClick
  end
  object EditItemPath: TEdit
    Left = 64
    Top = 16
    Width = 465
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 0
  end
  object EditPort: TEdit
    Left = 536
    Top = 16
    Width = 57
    Height = 21
    Anchors = [akTop, akRight]
    TabOrder = 1
    Text = '12345'
  end
end
