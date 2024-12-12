inherited ReleaseForm: TReleaseForm
  Left = 463
  Top = 257
  Width = 624
  Height = 268
  Caption = 'ReleaseForm'
  OldCreateOrder = True
  PixelsPerInch = 96
  TextHeight = 13
  inherited Label1: TLabel
    Visible = False
  end
  inherited Label12: TLabel
    Top = 40
  end
  object Label2: TLabel [2]
    Left = 8
    Top = 8
    Width = 39
    Height = 13
    Caption = 'Release'
  end
  inherited EditName: TEdit
    Left = 208
    Top = 8
    Width = 193
    TabOrder = 6
  end
  inherited ButtonOk: TButton
    Top = 208
    TabOrder = 9
  end
  inherited ButtonCancel: TButton
    Top = 208
    TabOrder = 10
  end
  inherited MemoDescription: TMemo
    Left = 80
    Top = 40
    Width = 529
    TabOrder = 8
  end
  object EditPatch: TEdit
    Left = 160
    Top = 8
    Width = 25
    Height = 21
    ReadOnly = True
    TabOrder = 4
    Text = '0'
  end
  object EditMinor: TEdit
    Left = 120
    Top = 8
    Width = 25
    Height = 21
    ReadOnly = True
    TabOrder = 2
    Text = '0'
  end
  object EditMajor: TEdit
    Left = 80
    Top = 8
    Width = 25
    Height = 21
    ReadOnly = True
    TabOrder = 0
    Text = '0'
  end
  object UpDownMajor: TUpDown
    Left = 105
    Top = 8
    Width = 15
    Height = 21
    Associate = EditMajor
    Min = 0
    Position = 0
    TabOrder = 1
    Wrap = False
    OnClick = UpDownClick
  end
  object UpDownMinor: TUpDown
    Left = 145
    Top = 8
    Width = 15
    Height = 21
    Associate = EditMinor
    Min = 0
    Position = 0
    TabOrder = 3
    Wrap = False
    OnClick = UpDownClick
  end
  object UpDownPatch: TUpDown
    Left = 185
    Top = 8
    Width = 15
    Height = 21
    Associate = EditPatch
    Min = 0
    Position = 0
    TabOrder = 5
    Wrap = False
    OnClick = UpDownClick
  end
  object CheckBoxCreateLinks: TCheckBox
    Left = 408
    Top = 8
    Width = 97
    Height = 17
    Anchors = [akTop, akRight]
    Caption = 'Create Links'
    TabOrder = 7
  end
end
