object DocManMainForm: TDocManMainForm
  Left = 677
  Top = 174
  Width = 775
  Height = 421
  Caption = 'Document Manager'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu
  OldCreateOrder = False
  Position = poScreenCenter
  ShowHint = True
  OnClose = FormClose
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 767
    Height = 41
    Align = alTop
    TabOrder = 0
    object SpeedButtonUp: TSpeedButton
      Left = 232
      Top = 8
      Width = 23
      Height = 22
      Hint = 'Up'
      Glyph.Data = {
        76010000424D7601000000000000760000002800000020000000100000000100
        04000000000000010000120B0000120B00001000000000000000000000000000
        800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333000333
        3333333333777F33333333333309033333333333337F7F333333333333090333
        33333333337F7F33333333333309033333333333337F7F333333333333090333
        33333333337F7F33333333333309033333333333FF7F7FFFF333333000090000
        3333333777737777F333333099999990333333373F3333373333333309999903
        333333337F33337F33333333099999033333333373F333733333333330999033
        3333333337F337F3333333333099903333333333373F37333333333333090333
        33333333337F7F33333333333309033333333333337373333333333333303333
        333333333337F333333333333330333333333333333733333333}
      NumGlyphs = 2
      OnClick = SpeedButtonUpClick
    end
    object LabelAddItem: TLabel
      Left = 565
      Top = 8
      Width = 45
      Height = 13
      Anchors = [akTop, akRight]
      Caption = 'AddI tem:'
    end
    object SpeedButtonPopup: TSpeedButton
      Left = 8
      Top = 8
      Width = 23
      Height = 22
      Hint = 'Functions'
      Glyph.Data = {
        76010000424D7601000000000000760000002800000020000000100000000100
        04000000000000010000120B0000120B00001000000000000000000000000000
        800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00555550FF0559
        1950555FF75F7557F7F757000FF055591903557775F75557F77570FFFF055559
        1933575FF57F5557F7FF0F00FF05555919337F775F7F5557F7F700550F055559
        193577557F7F55F7577F07550F0555999995755575755F7FFF7F5570F0755011
        11155557F755F777777555000755033305555577755F75F77F55555555503335
        0555555FF5F75F757F5555005503335505555577FF75F7557F55505050333555
        05555757F75F75557F5505000333555505557F777FF755557F55000000355557
        07557777777F55557F5555000005555707555577777FF5557F55553000075557
        0755557F7777FFF5755555335000005555555577577777555555}
      NumGlyphs = 2
      OnClick = SpeedButtonPopupClick
    end
    object SpeedButtonRefresh: TSpeedButton
      Left = 304
      Top = 8
      Width = 23
      Height = 22
      Hint = 'Refresh'
      Glyph.Data = {
        42010000424D4201000000000000420000002800000010000000100000000100
        08000000000000010000C30E0000C30E00000300000000000000000000008080
        8000C8D0D4000202020202020202020202020202020202020202020202020202
        0202020202020202020202020202020202020202020202020202020201000102
        0202020202020202020201000001020202020202020202020202000001020202
        0202020202020202020100010202020202020202020202020200000202020202
        0202020202020202020000020202000000000002020202020200000102020201
        0000000202020202020100000102010000000002020202020202000000000000
        0001000202020202020202010000000102020002020202020202020202020202
        0202020202020202020202020202020202020202020202020202020202020202
        020202020202}
      OnClick = SpeedButtonRefreshClick
    end
    object SpeedButtonBack: TSpeedButton
      Left = 256
      Top = 8
      Width = 23
      Height = 22
      Hint = 'Back'
      Enabled = False
      Glyph.Data = {
        76010000424D7601000000000000760000002800000020000000100000000100
        04000000000000010000120B0000120B00001000000000000000000000000000
        800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
        3333333333333333333333333333333333333333333333333333333333333333
        3333333333333FF3333333333333003333333333333F77F33333333333009033
        333333333F7737F333333333009990333333333F773337FFFFFF330099999000
        00003F773333377777770099999999999990773FF33333FFFFF7330099999000
        000033773FF33777777733330099903333333333773FF7F33333333333009033
        33333333337737F3333333333333003333333333333377333333333333333333
        3333333333333333333333333333333333333333333333333333333333333333
        3333333333333333333333333333333333333333333333333333}
      NumGlyphs = 2
      OnClick = SpeedButtonBackClick
    end
    object SpeedButtonForward: TSpeedButton
      Left = 280
      Top = 8
      Width = 23
      Height = 22
      Hint = 'Forward'
      Enabled = False
      Glyph.Data = {
        76010000424D7601000000000000760000002800000020000000100000000100
        04000000000000010000120B0000120B00001000000000000000000000000000
        800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
        3333333333333333333333333333333333333333333333333333333333333333
        3333333333333333333333333333333333333333333FF3333333333333003333
        3333333333773FF3333333333309003333333333337F773FF333333333099900
        33333FFFFF7F33773FF30000000999990033777777733333773F099999999999
        99007FFFFFFF33333F7700000009999900337777777F333F7733333333099900
        33333333337F3F77333333333309003333333333337F77333333333333003333
        3333333333773333333333333333333333333333333333333333333333333333
        3333333333333333333333333333333333333333333333333333}
      NumGlyphs = 2
      OnClick = SpeedButtonForwardClick
    end
    object ComboBoxAddItemTypes: TComboBox
      Left = 611
      Top = 8
      Width = 145
      Height = 19
      Style = csOwnerDrawFixed
      Anchors = [akTop, akRight]
      DropDownCount = 16
      ItemHeight = 13
      TabOrder = 0
      OnChange = ComboBoxAddItemTypesChange
    end
    object ComboBoxParents: TComboBox
      Left = 40
      Top = 8
      Width = 185
      Height = 21
      Style = csDropDownList
      ItemHeight = 13
      TabOrder = 1
      OnChange = ComboBoxParentsChange
    end
  end
  object StatusBar: TStatusBar
    Left = 0
    Top = 356
    Width = 767
    Height = 19
    Panels = <>
    SimplePanel = True
  end
  object theDatabase: TDatabase
    AliasName = 'DOCMAN'
    DatabaseName = 'docManDB'
    SessionName = 'Default'
    Left = 16
    Top = 320
  end
  object MainMenu: TMainMenu
    Left = 88
    Top = 320
    object File: TMenuItem
      Caption = '&File'
      object FileLogin: TMenuItem
        Caption = '&Login...'
        OnClick = FileLoginClick
      end
      object N2: TMenuItem
        Caption = '-'
      end
      object FileUser: TMenuItem
        Caption = '&User and Groups...'
        OnClick = FileUserClick
      end
      object RemoteUsers: TMenuItem
        Caption = '&Remote Users...'
        OnClick = RemoteUsersClick
      end
      object N6: TMenuItem
        Caption = '-'
      end
      object OpenFiles: TMenuItem
        Caption = 'Open &Files'
        OnClick = OpenFilesClick
      end
      object MenuOpenTests: TMenuItem
        Caption = '&Open Tests'
        OnClick = MenuOpenTestsClick
      end
      object OpenTasks: TMenuItem
        Caption = 'Open &Tasks'
        OnClick = OpenTasksClick
      end
      object LocalDirectories1: TMenuItem
        Caption = 'Local &Directories'
        OnClick = LocalDirectories1Click
      end
      object N5: TMenuItem
        Caption = '-'
      end
      object MenuSearch: TMenuItem
        Caption = '&Search...'
        OnClick = MenuSearchClick
      end
      object IdexSearch1: TMenuItem
        Caption = '&Index Search...'
        OnClick = IdexSearch1Click
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object FileExit: TMenuItem
        Caption = '&Exit'
        OnClick = FileExitClick
      end
    end
    object Personal: TMenuItem
      Caption = '&Personal'
      object PersonalFolder: TMenuItem
        Caption = '&Personal Folder'
        OnClick = PersonalFolderClick
      end
      object Bookmarks: TMenuItem
        Caption = '&Bookmarks'
        OnClick = BookmarksClick
      end
      object Trash: TMenuItem
        Caption = '&Trash'
        OnClick = TrashClick
      end
      object PersonalTemplateFolder: TMenuItem
        Caption = 'Template &Folder'
        OnClick = PersonalTemplateFolderClick
      end
      object N3: TMenuItem
        Caption = '-'
      end
      object ChangePassword: TMenuItem
        Caption = '&Change Password'
        OnClick = ChangePasswordClick
      end
      object ChangeDecryptionPassword: TMenuItem
        Caption = 'Change &Decryption Password'
        OnClick = ChangeDecryptionPasswordClick
      end
    end
    object Company1: TMenuItem
      Caption = '&Company'
      object Workspace: TMenuItem
        Caption = '&Workspace'
        OnClick = WorkspaceClick
      end
      object XSLTFolder: TMenuItem
        Caption = '&XSLT Folder'
        OnClick = XSLTFolderClick
      end
      object PublicTemplateFolder: TMenuItem
        Caption = '&Template Folder'
        OnClick = PublicTemplateFolderClick
      end
      object Archive1: TMenuItem
        Caption = '&Archive'
        OnClick = Archive1Click
      end
    end
    object Administration: TMenuItem
      Caption = '&Administration'
      object AdminConfigure: TMenuItem
        Caption = '&Configure...'
        OnClick = AdminConfigureClick
      end
      object N7: TMenuItem
        Caption = '-'
      end
      object AdminTaskTypes: TMenuItem
        Caption = '&Task Types...'
        OnClick = AdminTaskTypesClick
      end
      object AdminTaskStatus: TMenuItem
        Caption = 'Task &Status...'
        OnClick = AdminTaskStatusClick
      end
      object AdminMimeTypes: TMenuItem
        Caption = '&Mime Types...'
        OnClick = AdminMimeTypesClick
      end
      object N8: TMenuItem
        Caption = '-'
      end
      object AdminCheckDatabase: TMenuItem
        Caption = 'Check &Database'
        OnClick = AdminCheckDatabaseClick
      end
      object RefreshExifs1: TMenuItem
        Caption = 'Refresh &Exifs'
        OnClick = RefreshExifs1Click
      end
      object RefreshMimeTypes1: TMenuItem
        Caption = '&Refresh Mime Types'
        OnClick = RefreshMimeTypes1Click
      end
      object AdminReindexDatabase: TMenuItem
        Caption = 'Re&index Database'
        OnClick = AdminReindexDatabaseClick
      end
      object N9: TMenuItem
        Caption = '-'
      end
      object Categories1: TMenuItem
        Caption = 'C&ategories...'
        OnClick = Categories1Click
      end
      object RemoteServer: TMenuItem
        Caption = 'Remote Server...'
        OnClick = RemoteServerClick
      end
    end
    object MenuReports: TMenuItem
      AutoHotkeys = maManual
      Caption = '&Reports'
    end
    object Help1: TMenuItem
      Caption = '&Help'
      object About1: TMenuItem
        Caption = '&About...'
        OnClick = About1Click
      end
    end
  end
  object PopupMenuActions: TPopupMenu
    AutoPopup = False
    Left = 16
    Top = 32
    object test1: TMenuItem
      Caption = 'test'
      OnClick = ActionClick
    end
  end
end
