object VersionCreateForm: TVersionCreateForm
  Left = 480
  Top = 194
  Width = 776
  Height = 279
  Caption = 'Create Version'
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
  object LabelFile: TLabel
    Left = 8
    Top = 8
    Width = 16
    Height = 13
    Caption = 'File'
  end
  object LabelDescription: TLabel
    Left = 8
    Top = 40
    Width = 53
    Height = 13
    Caption = 'Description'
  end
  object ButtonOK: TButton
    Left = 8
    Top = 216
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 3
  end
  object ButtonCancel: TButton
    Left = 88
    Top = 216
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 4
  end
  object EditFilePath: TEdit
    Left = 88
    Top = 8
    Width = 579
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 0
  end
  object ButtonBrowseFile: TButton
    Left = 682
    Top = 8
    Width = 75
    Height = 25
    Anchors = [akTop, akRight]
    Caption = 'Browse...'
    TabOrder = 1
    OnClick = ButtonBrowseFileClick
  end
  object MemoDescription: TMemo
    Left = 88
    Top = 40
    Width = 577
    Height = 169
    Anchors = [akLeft, akTop, akRight, akBottom]
    TabOrder = 2
  end
  object OpenDialog: TOpenDialog
    Options = [ofHideReadOnly, ofNoChangeDir, ofPathMustExist, ofFileMustExist, ofEnableSizing]
    Left = 304
    Top = 8
  end
end
