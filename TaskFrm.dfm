inherited TaskForm: TTaskForm
  Left = 615
  Top = 232
  Width = 649
  Height = 529
  Caption = 'TaskForm'
  OldCreateOrder = True
  PixelsPerInch = 96
  TextHeight = 13
  object Label2: TLabel [1]
    Left = 8
    Top = 40
    Width = 20
    Height = 13
    Caption = 'Ref.'
  end
  object Label3: TLabel [2]
    Left = 8
    Top = 72
    Width = 24
    Height = 13
    Caption = 'Type'
  end
  object Label4: TLabel [3]
    Left = 272
    Top = 72
    Width = 30
    Height = 13
    Caption = 'Status'
  end
  object Label5: TLabel [4]
    Left = 8
    Top = 104
    Width = 35
    Height = 13
    Caption = 'Module'
  end
  object Label6: TLabel [5]
    Left = 8
    Top = 136
    Width = 41
    Height = 13
    Caption = 'Function'
  end
  object Label7: TLabel [6]
    Left = 8
    Top = 168
    Width = 39
    Height = 13
    Caption = 'Release'
  end
  object Label8: TLabel [7]
    Left = 272
    Top = 168
    Width = 31
    Height = 13
    Caption = 'Priority'
  end
  object Label9: TLabel [8]
    Left = 8
    Top = 200
    Width = 59
    Height = 13
    Caption = 'Assigned To'
  end
  object Label10: TLabel [9]
    Left = 8
    Top = 232
    Width = 46
    Height = 13
    Caption = 'Est. Effort'
  end
  object Label11: TLabel [10]
    Left = 272
    Top = 232
    Width = 47
    Height = 13
    Caption = 'Act. Effort'
  end
  inherited Label12: TLabel
    Top = 264
  end
  object Label13: TLabel [12]
    Left = 8
    Top = 376
    Width = 73
    Height = 49
    AutoSize = False
    Caption = 'Internal / External Remarks'
    WordWrap = True
  end
  object LabelCompleted: TLabel [13]
    Left = 480
    Top = 72
    Width = 76
    Height = 13
    Caption = 'LabelCompleted'
  end
  inherited EditName: TEdit
    Width = 540
  end
  inherited ButtonOk: TButton
    Top = 464
    TabOrder = 15
  end
  inherited ButtonCancel: TButton
    Top = 464
    TabOrder = 16
  end
  object EditCustomerRef: TEdit [17]
    Left = 88
    Top = 40
    Width = 540
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 1
  end
  object ComboBoxTaskType: TComboBox [18]
    Left = 88
    Top = 72
    Width = 145
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 2
  end
  object ComboBoxTaskStatus: TComboBox [19]
    Left = 328
    Top = 72
    Width = 145
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 3
  end
  object ComboBoxModule: TComboBox [20]
    Left = 88
    Top = 104
    Width = 540
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    ItemHeight = 13
    TabOrder = 4
  end
  object ComboBoxFunction: TComboBox [21]
    Left = 88
    Top = 136
    Width = 540
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    ItemHeight = 13
    TabOrder = 5
  end
  object EditRelease: TEdit [22]
    Left = 88
    Top = 168
    Width = 145
    Height = 21
    TabOrder = 6
  end
  object EditPriority: TEdit [23]
    Left = 328
    Top = 168
    Width = 129
    Height = 21
    ReadOnly = True
    TabOrder = 7
    Text = '1'
  end
  object UpDownPriority: TUpDown [24]
    Left = 457
    Top = 168
    Width = 15
    Height = 21
    Associate = EditPriority
    Min = 1
    Max = 9
    Position = 1
    TabOrder = 8
    Wrap = False
  end
  object ComboBoxAssignedTo: TComboBox [25]
    Left = 88
    Top = 200
    Width = 145
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 9
  end
  object EditEstEffort: TEdit [26]
    Left = 88
    Top = 232
    Width = 145
    Height = 21
    TabOrder = 10
  end
  object EditActEffort: TEdit [27]
    Left = 328
    Top = 232
    Width = 145
    Height = 21
    TabOrder = 11
  end
  inherited MemoDescription: TMemo
    Top = 264
    Width = 540
    Height = 97
    TabOrder = 12
  end
  object MemoInternalRemarks: TMemo
    Left = 88
    Top = 368
    Width = 265
    Height = 89
    TabOrder = 13
  end
  object MemoExternalRemarks: TMemo
    Left = 360
    Top = 368
    Width = 265
    Height = 89
    TabOrder = 14
  end
end
