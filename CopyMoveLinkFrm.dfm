object CopyMoveLinkForm: TCopyMoveLinkForm
  Left = 587
  Top = 150
  Width = 626
  Height = 133
  Caption = 'CopyMoveLinkForm'
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
  object Destination: TLabel
    Left = 8
    Top = 32
    Width = 53
    Height = 13
    Caption = 'Destination'
  end
  object Label1: TLabel
    Left = 8
    Top = 8
    Width = 56
    Height = 13
    Caption = 'New Name:'
  end
  object EditTarget: TEdit
    Left = 80
    Top = 32
    Width = 450
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 1
  end
  object ButtonBrowse: TButton
    Left = 530
    Top = 32
    Width = 71
    Height = 21
    Anchors = [akTop, akRight]
    Caption = 'Browse'
    TabOrder = 2
    OnClick = ButtonBrowseClick
  end
  object EditName: TEdit
    Left = 80
    Top = 8
    Width = 450
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 0
  end
  object ButtonOK: TButton
    Left = 8
    Top = 64
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 3
  end
  object ButtonCancel: TButton
    Left = 88
    Top = 64
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 4
  end
end
