object ImageBrowserForm: TImageBrowserForm
  Left = 299
  Top = 392
  Width = 589
  Height = 415
  Caption = 'ImageBrowserForm'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnClose = FormClose
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object ScrollBox: TScrollBox
    Left = 0
    Top = 57
    Width = 581
    Height = 331
    Align = alClient
    TabOrder = 0
    OnResize = ScrollBoxResize
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 581
    Height = 57
    Align = alTop
    Caption = 'Panel1'
    TabOrder = 1
    object TrackBar: TTrackBar
      Left = 8
      Top = 8
      Width = 561
      Height = 33
      Anchors = [akLeft, akTop, akRight]
      Max = 1000
      Min = 50
      Orientation = trHorizontal
      Frequency = 50
      Position = 150
      SelEnd = 0
      SelStart = 0
      TabOrder = 0
      TickMarks = tmBottomRight
      TickStyle = tsAuto
      OnChange = TrackBarChange
    end
  end
end
