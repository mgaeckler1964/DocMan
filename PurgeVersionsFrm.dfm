object PurgeVersionsForm: TPurgeVersionsForm
  Left = 575
  Top = 301
  BorderStyle = bsDialog
  Caption = 'Purge Versions'
  ClientHeight = 73
  ClientWidth = 190
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 8
    Width = 97
    Height = 13
    Caption = '# Versions To Keep:'
  end
  object EditNumVersions: TEdit
    Left = 122
    Top = 8
    Width = 57
    Height = 21
    TabOrder = 0
    Text = '1'
    OnChange = EditNumVersionsChange
    OnKeyPress = EditNumVersionsKeyPress
  end
  object ButtonOK: TButton
    Left = 8
    Top = 40
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 1
  end
  object ButtonCancel: TButton
    Left = 88
    Top = 40
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 2
  end
end
