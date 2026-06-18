object IndexForm: TIndexForm
  Left = 559
  Top = 268
  Width = 750
  Height = 773
  Caption = 'Index Search'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 734
    Height = 105
    Align = alTop
    TabOrder = 0
    object Label1: TLabel
      Left = 8
      Top = 8
      Width = 44
      Height = 13
      Caption = 'Keyword:'
    end
    object Label9: TLabel
      Left = 8
      Top = 40
      Width = 38
      Height = 13
      Caption = 'Volume:'
    end
    object Label2: TLabel
      Left = 8
      Top = 72
      Width = 39
      Height = 13
      Caption = 'Options:'
    end
    object HitCountLabel: TLabel
      Left = 656
      Top = 40
      Width = 48
      Height = 13
      Anchors = [akTop, akRight]
      Caption = '___ File(s)'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object EditKeyword: TEdit
      Left = 64
      Top = 8
      Width = 585
      Height = 21
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 0
    end
    object ButtonSearch: TButton
      Left = 652
      Top = 8
      Width = 75
      Height = 25
      Anchors = [akTop, akRight]
      Caption = 'Search'
      Default = True
      TabOrder = 1
      OnClick = ButtonSearchClick
    end
    object RadioButtonAny: TRadioButton
      Left = 64
      Top = 40
      Width = 41
      Height = 17
      Caption = 'Any'
      Checked = True
      TabOrder = 2
      TabStop = True
    end
    object RadioButtonCompany: TRadioButton
      Left = 112
      Top = 40
      Width = 49
      Height = 17
      Caption = 'Public'
      TabOrder = 3
    end
    object RadioButtonPrivate: TRadioButton
      Left = 168
      Top = 40
      Width = 57
      Height = 17
      Caption = 'Private'
      TabOrder = 4
    end
    object RadioButtonArchive: TRadioButton
      Left = 232
      Top = 40
      Width = 57
      Height = 17
      Caption = 'Archive'
      TabOrder = 5
    end
    object CheckBoxCurrentVersion: TCheckBox
      Left = 72
      Top = 72
      Width = 97
      Height = 17
      Caption = 'Current Version'
      Checked = True
      State = cbChecked
      TabOrder = 6
    end
    object CheckBoxCaseSensitive: TCheckBox
      Left = 176
      Top = 72
      Width = 97
      Height = 17
      Caption = 'Case Sensitive'
      Checked = True
      State = cbChecked
      TabOrder = 7
    end
    object CheckBoxFuzzy: TCheckBox
      Left = 280
      Top = 72
      Width = 97
      Height = 17
      Caption = 'Fuzzy Search'
      TabOrder = 8
    end
    object CheckBoxWildcard: TCheckBox
      Left = 384
      Top = 72
      Width = 97
      Height = 17
      Caption = 'Wildcard Search'
      TabOrder = 9
    end
    object MoreBttn: TButton
      Left = 652
      Top = 64
      Width = 75
      Height = 25
      Anchors = [akTop, akRight]
      Caption = 'More'
      Enabled = False
      TabOrder = 10
      OnClick = MoreBttnClick
    end
  end
  object GridResult: TStringGrid
    Left = 0
    Top = 105
    Width = 734
    Height = 629
    Align = alClient
    ColCount = 6
    FixedCols = 0
    RowCount = 2
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goColSizing]
    TabOrder = 1
    OnDblClick = GridResultDblClick
    ColWidths = (
      144
      301
      64
      64
      64
      64)
  end
end
