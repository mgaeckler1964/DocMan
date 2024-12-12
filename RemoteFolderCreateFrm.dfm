inherited RemoteFolderCreateForm: TRemoteFolderCreateForm
  Left = 353
  Top = 353
  Width = 855
  Height = 332
  Caption = 'RemoteFolderCreateForm'
  PixelsPerInch = 96
  TextHeight = 13
  inherited Label2: TLabel
    Top = 80
  end
  inherited SpeedButton: TSpeedButton
    Left = 816
    Top = 80
  end
  object Label3: TLabel [3]
    Left = 8
    Top = 48
    Width = 65
    Height = 13
    Caption = 'Server/Folder'
  end
  inherited Label12: TLabel
    Top = 112
  end
  inherited EditName: TEdit
    Width = 765
  end
  inherited ButtonOk: TButton
    Top = 272
    TabOrder = 5
  end
  inherited ButtonCancel: TButton
    Top = 272
    TabOrder = 6
  end
  inherited EditLocalPath: TEdit
    Top = 80
    Width = 744
    TabOrder = 3
  end
  inherited MemoDescription: TMemo
    Top = 112
    Width = 765
  end
  object ComboBoxRemoteServer: TComboBox
    Left = 72
    Top = 48
    Width = 265
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 1
  end
  object EditRemoteFolderID: TEdit
    Left = 344
    Top = 48
    Width = 121
    Height = 21
    TabOrder = 2
  end
end
