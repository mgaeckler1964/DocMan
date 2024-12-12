inherited TestStepExecForm: TTestStepExecForm
  Width = 636
  Height = 598
  Caption = 'Test Step Execution'
  PixelsPerInch = 96
  TextHeight = 13
  inherited Label9: TLabel
    Top = 504
  end
  object Actual: TLabel [4]
    Left = 8
    Top = 368
    Width = 30
    Height = 13
    Caption = 'Actual'
  end
  inherited EditName: TEdit
    ReadOnly = True
    TabOrder = 7
  end
  inherited ButtonOk: TButton
    Top = 536
  end
  object MemoActualResult: TMemo [7]
    Left = 88
    Top = 360
    Width = 526
    Height = 137
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 1
  end
  object ButtonFailed: TButton [8]
    Left = 88
    Top = 536
    Width = 75
    Height = 25
    Caption = 'Failed'
    ModalResult = 7
    TabOrder = 2
  end
  inherited ButtonCancel: TButton
    Left = 544
    Top = 536
    Anchors = [akTop, akRight]
  end
  inherited MemoDescription: TMemo
    ReadOnly = True
    TabOrder = 6
  end
  inherited MemoExpected: TMemo
    ReadOnly = True
    TabOrder = 3
  end
  inherited ComboBoxAssignedTo: TComboBox
    Top = 504
    Enabled = False
    TabOrder = 0
  end
end
