object TestPrepareForm: TTestPrepareForm
  Left = 775
  Top = 575
  Width = 510
  Height = 153
  Caption = 'Prepare Test'
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
    Width = 59
    Height = 13
    Caption = 'Assigned To'
  end
  object Label2: TLabel
    Left = 8
    Top = 48
    Width = 43
    Height = 13
    Caption = 'Task List'
  end
  object ComboBoxAssignedTo: TComboBox
    Left = 80
    Top = 14
    Width = 161
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 0
  end
  object EditTaskList: TEdit
    Left = 80
    Top = 48
    Width = 329
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    ReadOnly = True
    TabOrder = 1
  end
  object ButtonBrowse: TButton
    Left = 416
    Top = 48
    Width = 75
    Height = 25
    Anchors = [akTop, akRight]
    Caption = 'Browse...'
    TabOrder = 2
    OnClick = ButtonBrowseClick
  end
  object ButtonOK: TButton
    Left = 8
    Top = 88
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 3
  end
  object ButtonCancel: TButton
    Left = 96
    Top = 88
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 4
  end
end
