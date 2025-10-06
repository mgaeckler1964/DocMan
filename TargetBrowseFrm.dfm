object TargetBrowseForm: TTargetBrowseForm
  Left = 674
  Top = 413
  Width = 454
  Height = 281
  Caption = 'Select Target'
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
  object TreeView: TTreeView
    Left = 0
    Top = 0
    Width = 438
    Height = 201
    Align = alClient
    HideSelection = False
    Images = ImageList
    Indent = 19
    TabOrder = 0
    OnExpanding = TreeViewExpanding
  end
  object Panel1: TPanel
    Left = 0
    Top = 201
    Width = 438
    Height = 41
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 1
    object Button1: TButton
      Left = 8
      Top = 8
      Width = 75
      Height = 25
      Caption = 'OK'
      Default = True
      TabOrder = 0
      OnClick = Button1Click
    end
    object Button2: TButton
      Left = 88
      Top = 8
      Width = 75
      Height = 25
      Cancel = True
      Caption = 'Cancel'
      ModalResult = 2
      TabOrder = 1
    end
  end
  object ImageList: TImageList
    Left = 16
    Top = 13
  end
end
