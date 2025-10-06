object ChangeOrderForm: TChangeOrderForm
  Left = 526
  Top = 206
  Width = 517
  Height = 389
  Caption = 'Change Order'
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
    Top = 309
    Width = 501
    Height = 41
    Align = alBottom
    TabOrder = 0
    object Button1: TButton
      Left = 8
      Top = 8
      Width = 75
      Height = 25
      Caption = 'OK'
      Default = True
      ModalResult = 1
      TabOrder = 0
    end
    object Button2: TButton
      Left = 168
      Top = 8
      Width = 75
      Height = 25
      Cancel = True
      Caption = 'Cancel'
      ModalResult = 2
      TabOrder = 1
    end
    object Button3: TButton
      Left = 88
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Clear'
      ModalResult = 9
      TabOrder = 2
    end
  end
  object ListBoxItems: TListBox
    Left = 0
    Top = 0
    Width = 501
    Height = 309
    Align = alClient
    DragMode = dmAutomatic
    ItemHeight = 13
    TabOrder = 1
    OnDragOver = ListBoxItemsDragOver
    OnEndDrag = ListBoxItemsEndDrag
  end
end
