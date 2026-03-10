inherited SourceFolderCreateForm: TSourceFolderCreateForm
  Left = 445
  Top = 400
  Width = 813
  Height = 356
  Caption = 'SourceFolderCreateForm'
  PixelsPerInch = 96
  TextHeight = 13
  inherited SpeedButton: TSpeedButton
    Left = 767
  end
  object Label3: TLabel [3]
    Left = 8
    Top = 80
    Width = 39
    Height = 13
    Caption = 'Release'
  end
  inherited Label12: TLabel
    Top = 112
  end
  inherited EditName: TEdit
    Width = 720
  end
  inherited ButtonOk: TButton
    Top = 272
    TabOrder = 9
  end
  inherited ButtonCancel: TButton
    Top = 272
    TabOrder = 10
  end
  inherited EditLocalPath: TEdit
    Width = 697
    TabOrder = 1
  end
  object EditPatch: TEdit [9]
    Left = 152
    Top = 80
    Width = 25
    Height = 21
    ReadOnly = True
    TabOrder = 6
    Text = '0'
  end
  object EditMinor: TEdit [10]
    Left = 112
    Top = 80
    Width = 25
    Height = 21
    ReadOnly = True
    TabOrder = 4
    Text = '0'
  end
  object EditMajor: TEdit [11]
    Left = 72
    Top = 80
    Width = 25
    Height = 21
    ReadOnly = True
    TabOrder = 2
    Text = '0'
  end
  inherited MemoDescription: TMemo
    Top = 112
    Width = 720
    TabOrder = 8
  end
  object UpDownMajor: TUpDown
    Left = 97
    Top = 80
    Width = 15
    Height = 21
    Associate = EditMajor
    Min = 0
    Position = 0
    TabOrder = 3
    Wrap = False
  end
  object UpDownMinor: TUpDown
    Left = 137
    Top = 80
    Width = 15
    Height = 21
    Associate = EditMinor
    Min = 0
    Position = 0
    TabOrder = 5
    Wrap = False
  end
  object UpDownPatch: TUpDown
    Left = 177
    Top = 80
    Width = 15
    Height = 21
    Associate = EditPatch
    Min = 0
    Position = 0
    TabOrder = 7
    Wrap = False
  end
end
