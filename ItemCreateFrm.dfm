object ItemCreateForm: TItemCreateForm
  Left = 476
  Top = 516
  Width = 635
  Height = 288
  Caption = 'ItemCreateForm'
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
  object Label1: TLabel
    Left = 8
    Top = 16
    Width = 28
    Height = 13
    Caption = 'Name'
  end
  object Label12: TLabel
    Left = 8
    Top = 48
    Width = 53
    Height = 13
    Caption = 'Description'
  end
  object EditName: TEdit
    Left = 88
    Top = 16
    Width = 523
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 0
  end
  object ButtonOk: TButton
    Left = 8
    Top = 216
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 2
    OnClick = ButtonOkClick
  end
  object ButtonCancel: TButton
    Left = 88
    Top = 216
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 3
  end
  object MemoDescription: TMemo
    Left = 88
    Top = 48
    Width = 523
    Height = 153
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 1
  end
end
