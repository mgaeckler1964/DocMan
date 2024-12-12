inherited TestStepForm: TTestStepForm
  Width = 632
  Height = 460
  Caption = 'Test Step'
  OldCreateOrder = True
  PixelsPerInch = 96
  TextHeight = 13
  object Label2: TLabel [2]
    Left = 8
    Top = 216
    Width = 45
    Height = 13
    Caption = 'Expected'
  end
  object Label9: TLabel [3]
    Left = 8
    Top = 368
    Width = 59
    Height = 13
    Caption = 'Assigned To'
  end
  inherited EditName: TEdit
    Width = 523
  end
  inherited ButtonOk: TButton
    Top = 400
    TabOrder = 4
  end
  inherited ButtonCancel: TButton
    Top = 400
    TabOrder = 5
  end
  inherited MemoDescription: TMemo
    Width = 523
  end
  object MemoExpected: TMemo
    Left = 88
    Top = 216
    Width = 526
    Height = 137
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 2
  end
  object ComboBoxAssignedTo: TComboBox
    Left = 88
    Top = 368
    Width = 145
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 3
  end
end
