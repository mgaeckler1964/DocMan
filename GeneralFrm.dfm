inherited GeneralForm: TGeneralForm
  Left = 133
  Top = 148
  Height = 392
  Caption = 'GeneralForm'
  OldCreateOrder = True
  PixelsPerInch = 96
  TextHeight = 13
  object Label2: TLabel [2]
    Left = 8
    Top = 216
    Width = 27
    Height = 13
    Caption = 'Type:'
  end
  object LabelType: TLabel [3]
    Left = 88
    Top = 216
    Width = 50
    Height = 13
    Caption = 'LabelType'
  end
  object Label3: TLabel [4]
    Left = 8
    Top = 240
    Width = 55
    Height = 13
    Caption = 'Created By:'
  end
  object LabelCreator: TLabel [5]
    Left = 88
    Top = 240
    Width = 60
    Height = 13
    Caption = 'LabelCreator'
  end
  object LabelReserved: TLabel [6]
    Left = 208
    Top = 240
    Width = 64
    Height = 13
    Caption = 'Reserved By:'
  end
  object LabelReservedFor: TLabel [7]
    Left = 280
    Top = 240
    Width = 87
    Height = 13
    Caption = 'LabelReservedFor'
  end
  object Label5: TLabel [8]
    Left = 8
    Top = 264
    Width = 40
    Height = 13
    Caption = 'Created:'
  end
  object LabelCreatedDate: TLabel [9]
    Left = 88
    Top = 264
    Width = 86
    Height = 13
    Caption = 'LabelCreatedDate'
  end
  object Label6: TLabel [10]
    Left = 208
    Top = 264
    Width = 43
    Height = 13
    Caption = 'Modified:'
  end
  object LabelModifiedDate: TLabel [11]
    Left = 280
    Top = 264
    Width = 89
    Height = 13
    Caption = 'LabelModifiedDate'
  end
  object Label4: TLabel [12]
    Left = 8
    Top = 288
    Width = 29
    Height = 13
    Caption = 'Local:'
  end
  object LabelLocalPath: TLabel [13]
    Left = 88
    Top = 288
    Width = 74
    Height = 13
    Caption = 'LabelLocalPath'
  end
  object Label7: TLabel [14]
    Left = 208
    Top = 288
    Width = 45
    Height = 13
    Caption = 'Reminder'
  end
  inherited ButtonOk: TButton
    Top = 320
  end
  inherited ButtonCancel: TButton
    Top = 320
  end
  object ReminderEdit: TEdit
    Left = 280
    Top = 288
    Width = 161
    Height = 21
    TabOrder = 4
    OnEnter = ReminderEditEnter
  end
end
