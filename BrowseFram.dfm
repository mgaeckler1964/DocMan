object BrowserFrame: TBrowserFrame
  Left = 0
  Top = 0
  Width = 443
  Height = 277
  Align = alClient
  TabOrder = 0
  OnResize = FrameResize
  object DrawGridContent: TDrawGrid
    Left = 0
    Top = 0
    Width = 443
    Height = 277
    Align = alClient
    DefaultRowHeight = 20
    FixedCols = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRowSelect, goThumbTracking]
    TabOrder = 0
    OnDrawCell = DrawGridContentDrawCell
    OnMouseDown = DrawGridContentMouseDown
    OnMouseUp = DrawGridContentMouseUp
    OnSelectCell = DrawGridContentSelectCell
    ColWidths = (
      142
      123
      118
      94
      127)
  end
  object PopupMenuActions: TPopupMenu
    AutoPopup = False
    Left = 176
    Top = 64
    object Action: TMenuItem
      Caption = 'Default'
      OnClick = ActionClick
    end
  end
  object ImageListSort: TImageList
    Height = 9
    Masked = False
    Width = 9
    Left = 160
    Top = 176
    Bitmap = {
      494C010102000400040009000900FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
      0000000000003600000028000000240000000900000001002000000000001005
      0000000000000000000000000000000000004747470041414100616161005151
      5100494949004343430041414100454545001F1F1F0000000000000000000000
      0000FCFCFC00727272006E6E6E00FAFAFA000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00009494940087878700FDFDFD00FAFAFA00FAFAFA00F9F9F900F8F8F800EDED
      ED0064646400000000000000000000000000C6C6C600656565005F5F5F00DDDD
      DD00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000EAEAEA0067676700EBEBEB000000
      0000000000000000000000000000A7A7A7008C8C8C000000000000000000F7F7
      F7006E6E6E00CFCFCF00B0B0B0008A8A8A000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000099999900A4A4A400000000000000000000000000EFEFEF006767
      6700DEDEDE000000000000000000B9B9B9007979790000000000E8E8E8006A6A
      6A00E8E8E8000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000D5D5D5006D6D6D00F6F6
      F60000000000000000009A9A9A00959595000000000000000000F4F4F4006B6B
      6B00D6D6D600000000000000000085858500B0B0B00000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000072727200C6C6C60000000000E6E6E60065656500E8E8
      E8000000000000000000B2B2B20083838300000000000000000000000000D4D4
      D4006D6D6D00FCFCFC0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000BFBFBF007878
      7800000000008F8F8F009F9F9F000000000000000000EAEAEA0068686800DFDF
      DF00000000000000000000000000FCFCFC0075757500C8C8C800000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000F0F0F00071717100B4B4B40069696900EFEFEF000000
      000000000000838383009A9A9A00000000000000000000000000000000000000
      0000B9B9B9008B8B8B0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000A4A4
      A40037373700B6B6B6000000000000000000000000001F1F1F00818181009B9B
      9B009B9B9B009E9E9E00A2A2A200A8A8A8009F9F9F0049494900000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000424D3E000000000000003E00000028000000240000000900000001000100
      00000000480000000000000000000000000000000000000000000000FFFFFF00
      0070C000000000000070C000000000001E60C000000000009C64400000000000
      8CC6400000000000C8CE000000000000C98E000000000000C19F000000000000
      E38000000000000000000000000000000000000000000000000000000000}
  end
end
