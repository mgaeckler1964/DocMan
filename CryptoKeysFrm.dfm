object CryptoKeysForm: TCryptoKeysForm
  Left = 120
  Top = 279
  BorderStyle = bsDialog
  Caption = 'Crypto Keys'
  ClientHeight = 255
  ClientWidth = 398
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
  object ListBoxSelected: TListBox
    Left = 248
    Top = 16
    Width = 137
    Height = 185
    ItemHeight = 13
    Sorted = True
    TabOrder = 3
  end
  object ListBoxAvailable: TListBox
    Left = 16
    Top = 16
    Width = 137
    Height = 185
    ItemHeight = 13
    Sorted = True
    TabOrder = 0
  end
  object ButtonAdd: TButton
    Left = 160
    Top = 16
    Width = 75
    Height = 25
    Caption = 'Add >>'
    TabOrder = 1
    OnClick = ButtonAddClick
  end
  object ButtonRemove: TButton
    Left = 160
    Top = 48
    Width = 75
    Height = 25
    Caption = 'Remove <<'
    TabOrder = 2
    OnClick = ButtonRemoveClick
  end
  object ButtonOK: TButton
    Left = 16
    Top = 216
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 4
    OnClick = ButtonOKClick
  end
  object ButtonCancel: TButton
    Left = 96
    Top = 216
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 5
  end
end
