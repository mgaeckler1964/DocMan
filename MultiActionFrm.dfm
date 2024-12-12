object MultiActionForm: TMultiActionForm
  Left = 526
  Top = 206
  Width = 517
  Height = 389
  Caption = 'Actions'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 321
    Width = 509
    Height = 41
    Align = alBottom
    TabOrder = 0
    object ButtonCopy: TButton
      Left = 8
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Copy'
      TabOrder = 0
      OnClick = DoAction
    end
    object Button2: TButton
      Left = 248
      Top = 8
      Width = 75
      Height = 25
      Cancel = True
      Caption = 'Cancel'
      ModalResult = 2
      TabOrder = 1
    end
    object ButtonMove: TButton
      Left = 88
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Move'
      TabOrder = 2
      OnClick = DoAction
    end
    object ButtonDelete: TButton
      Left = 168
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Delete'
      TabOrder = 3
      OnClick = DoAction
    end
  end
  object ListBoxItems: TListBox
    Left = 0
    Top = 0
    Width = 509
    Height = 321
    Align = alClient
    DragMode = dmAutomatic
    ItemHeight = 13
    MultiSelect = True
    TabOrder = 1
    OnClick = ListBoxItemsClick
  end
end
