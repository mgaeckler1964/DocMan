object DirectoryDiffForm: TDirectoryDiffForm
  Left = 596
  Top = 242
  Width = 891
  Height = 333
  Caption = 'DirectoryDiffForm'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object StringGrid: TStringGrid
    Left = 0
    Top = 0
    Width = 875
    Height = 294
    Align = alClient
    DefaultRowHeight = 20
    FixedCols = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goColSizing, goRowSelect]
    PopupMenu = PopupMenu
    TabOrder = 0
    OnContextPopup = StringGridContextPopup
  end
  object PopupMenu: TPopupMenu
    OnPopup = PopupMenuPopup
    Left = 424
    Top = 104
    object MenuShowDiff: TMenuItem
      Caption = 'Show Diff...'
      OnClick = MenuShowDiffClick
    end
    object N1: TMenuItem
      Caption = '-'
    end
    object MenuAdd: TMenuItem
      Caption = 'Add to repository'
      OnClick = MenuAddClick
    end
    object MenuAddReserved: TMenuItem
      Caption = 'Add reserved'
      OnClick = MenuAddClick
    end
    object N2: TMenuItem
      Caption = '-'
    end
    object MenuRemoveFromRepository: TMenuItem
      Caption = 'Remove from Repository'
      OnClick = MenuRemoveFromRepositoryClick
    end
    object MenuRemoveFromFileSystem: TMenuItem
      Caption = 'Remove From File System'
      OnClick = MenuRemoveFromFileSystemClick
    end
    object N3: TMenuItem
      Caption = '-'
    end
    object MenuRefresh: TMenuItem
      Caption = 'Refresh'
      OnClick = MenuRefreshClick
    end
    object N4: TMenuItem
      Caption = '-'
    end
    object MenuAddVersion: TMenuItem
      Caption = 'Add Version...'
      OnClick = MenuAddVersionClick
    end
    object MenuCheckIn: TMenuItem
      Caption = 'Check In...'
      OnClick = MenuCheckInClick
    end
  end
end
