inherited FileCreateForm: TFileCreateForm
  Left = 482
  Top = 140
  Height = 367
  Caption = ''
  OldCreateOrder = True
  PixelsPerInch = 96
  TextHeight = 13
  object Label2: TLabel [2]
    Left = 8
    Top = 216
    Width = 16
    Height = 13
    Caption = 'File'
  end
  object LabelTemplate: TLabel [3]
    Left = 8
    Top = 248
    Width = 44
    Height = 13
    Caption = 'Template'
  end
  inherited ButtonOk: TButton
    Top = 296
    TabOrder = 6
  end
  inherited ButtonCancel: TButton
    Top = 296
    TabOrder = 7
  end
  object EditFilePath: TEdit
    Left = 88
    Top = 208
    Width = 438
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    ReadOnly = True
    TabOrder = 2
  end
  object ButtonBrowse: TButton
    Left = 533
    Top = 208
    Width = 75
    Height = 25
    Anchors = [akTop, akRight]
    Caption = 'Browse...'
    TabOrder = 3
    OnClick = ButtonBrowseClick
  end
  object CheckBoxReserve: TCheckBox
    Left = 88
    Top = 272
    Width = 97
    Height = 17
    Caption = 'Reserve'
    TabOrder = 5
  end
  object ComboBoxTemplate: TComboBox
    Left = 88
    Top = 240
    Width = 523
    Height = 21
    Style = csDropDownList
    Anchors = [akLeft, akTop, akRight]
    ItemHeight = 13
    TabOrder = 4
  end
  object OpenDialog: TOpenDialog
    Options = [ofHideReadOnly, ofNoChangeDir, ofPathMustExist, ofFileMustExist, ofEnableSizing]
    Left = 208
    Top = 256
  end
end
