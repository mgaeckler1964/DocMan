inherited CheckInForm: TCheckInForm
  Left = 490
  Top = 256
  Height = 367
  Caption = 'Check In'
  OldCreateOrder = True
  PixelsPerInch = 96
  TextHeight = 13
  inherited LabelFile: TLabel
    Top = 40
  end
  inherited LabelDescription: TLabel
    Top = 80
  end
  inherited ButtonOK: TButton
    Top = 304
    TabOrder = 4
  end
  inherited ButtonCancel: TButton
    Top = 304
    TabOrder = 5
  end
  inherited EditFilePath: TEdit
    Top = 40
    TabOrder = 1
  end
  inherited ButtonBrowseFile: TButton
    Top = 40
    TabOrder = 2
  end
  inherited MemoDescription: TMemo
    Top = 72
    Height = 225
    TabOrder = 3
  end
  object CheckBoxAddVersion: TCheckBox [7]
    Left = 8
    Top = 8
    Width = 97
    Height = 17
    Caption = 'Add Version'
    Checked = True
    State = cbChecked
    TabOrder = 0
    OnClick = CheckBoxAddVersionClick
  end
  object CheckBoxBranch: TCheckBox [8]
    Left = 96
    Top = 8
    Width = 97
    Height = 17
    Caption = 'Branch'
    TabOrder = 6
  end
end
