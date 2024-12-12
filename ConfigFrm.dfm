object ConfigForm: TConfigForm
  Left = 375
  Top = 447
  BorderStyle = bsDialog
  Caption = 'Configure'
  ClientHeight = 276
  ClientWidth = 611
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
    Width = 81
    Height = 13
    Caption = 'External Storage:'
  end
  object Label2: TLabel
    Left = 8
    Top = 40
    Width = 37
    Height = 13
    Caption = 'Reports'
  end
  object SpeedButtonExternalStorage: TSpeedButton
    Left = 577
    Top = 8
    Width = 23
    Height = 22
    Anchors = [akTop, akRight]
    Glyph.Data = {
      76010000424D7601000000000000760000002800000020000000100000000100
      04000000000000010000120B0000120B00001000000000000000000000000000
      800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
      FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00555555555555
      55555555FFFFFFFFFF55555000000000055555577777777775F55500B8B8B8B8
      B05555775F555555575F550F0B8B8B8B8B05557F75F555555575550BF0B8B8B8
      B8B0557F575FFFFFFFF7550FBF0000000000557F557777777777500BFBFBFBFB
      0555577F555555557F550B0FBFBFBFBF05557F7F555555FF75550F0BFBFBF000
      55557F75F555577755550BF0BFBF0B0555557F575FFF757F55550FB700007F05
      55557F557777557F55550BFBFBFBFB0555557F555555557F55550FBFBFBFBF05
      55557FFFFFFFFF7555550000000000555555777777777755555550FBFB055555
      5555575FFF755555555557000075555555555577775555555555}
    NumGlyphs = 2
    OnClick = SpeedButtonExternalStorageClick
  end
  object SpeedButtonReports: TSpeedButton
    Left = 577
    Top = 40
    Width = 23
    Height = 22
    Anchors = [akTop, akRight]
    Glyph.Data = {
      76010000424D7601000000000000760000002800000020000000100000000100
      04000000000000010000120B0000120B00001000000000000000000000000000
      800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
      FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00555555555555
      55555555FFFFFFFFFF55555000000000055555577777777775F55500B8B8B8B8
      B05555775F555555575F550F0B8B8B8B8B05557F75F555555575550BF0B8B8B8
      B8B0557F575FFFFFFFF7550FBF0000000000557F557777777777500BFBFBFBFB
      0555577F555555557F550B0FBFBFBFBF05557F7F555555FF75550F0BFBFBF000
      55557F75F555577755550BF0BFBF0B0555557F575FFF757F55550FB700007F05
      55557F557777557F55550BFBFBFBFB0555557F555555557F55550FBFBFBFBF05
      55557FFFFFFFFF7555550000000000555555777777777755555550FBFB055555
      5555575FFF755555555557000075555555555577775555555555}
    NumGlyphs = 2
    OnClick = SpeedButtonReportsClick
  end
  object EditExternalStorage: TEdit
    Left = 104
    Top = 8
    Width = 465
    Height = 21
    TabOrder = 0
  end
  object ButtonOk: TButton
    Left = 8
    Top = 248
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 3
  end
  object ButtonCancel: TButton
    Left = 88
    Top = 248
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 4
  end
  object EditReports: TEdit
    Left = 104
    Top = 40
    Width = 465
    Height = 21
    TabOrder = 1
  end
  object GroupBoxNetwork: TGroupBox
    Left = 8
    Top = 72
    Width = 593
    Height = 161
    Caption = 'Network'
    TabOrder = 2
    object Label3: TLabel
      Left = 8
      Top = 16
      Width = 60
      Height = 13
      Caption = 'Proxy Server'
    end
    object Label4: TLabel
      Left = 8
      Top = 40
      Width = 19
      Height = 13
      Caption = 'Port'
    end
    object Label5: TLabel
      Left = 8
      Top = 65
      Width = 61
      Height = 13
      Caption = 'No Proxy For'
    end
    object EditProxyServer: TEdit
      Left = 96
      Top = 16
      Width = 489
      Height = 21
      TabOrder = 0
    end
    object EditProxyPort: TEdit
      Left = 96
      Top = 40
      Width = 121
      Height = 21
      TabOrder = 1
      Text = '0'
    end
    object UpDownProxyPort: TUpDown
      Left = 217
      Top = 40
      Width = 16
      Height = 21
      Associate = EditProxyPort
      Min = 0
      Max = 32767
      Position = 0
      TabOrder = 2
      Thousands = False
      Wrap = False
    end
    object MemoNoProxy: TMemo
      Left = 96
      Top = 64
      Width = 489
      Height = 89
      TabOrder = 3
    end
  end
end
