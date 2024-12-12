object SearchForm: TSearchForm
  Left = 280
  Top = 292
  Width = 575
  Height = 503
  Caption = 'Search'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 559
    Height = 73
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
    object EditKeyword: TEdit
      Left = 64
      Top = 8
      Width = 334
      Height = 21
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 0
    end
    object ButtonSearch: TButton
      Left = 485
      Top = 8
      Width = 75
      Height = 25
      Anchors = [akTop, akRight]
      Caption = 'Search'
      Default = True
      TabOrder = 2
      OnClick = ButtonSearchClick
    end
    object ButtonAdvanced: TButton
      Left = 405
      Top = 8
      Width = 75
      Height = 25
      Anchors = [akTop, akRight]
      Caption = 'Advanced'
      TabOrder = 1
      OnClick = ButtonAdvancedClick
    end
    object RadioButtonAny: TRadioButton
      Left = 64
      Top = 40
      Width = 41
      Height = 17
      Caption = 'Any'
      Checked = True
      TabOrder = 3
      TabStop = True
    end
    object RadioButtonCompany: TRadioButton
      Left = 112
      Top = 40
      Width = 49
      Height = 17
      Caption = 'Public'
      TabOrder = 4
    end
    object RadioButtonPrivate: TRadioButton
      Left = 168
      Top = 40
      Width = 57
      Height = 17
      Caption = 'Private'
      TabOrder = 5
    end
    object RadioButtonArchive: TRadioButton
      Left = 232
      Top = 40
      Width = 57
      Height = 17
      Caption = 'Archive'
      TabOrder = 6
    end
    object ButtonImages: TButton
      Left = 485
      Top = 40
      Width = 75
      Height = 25
      Anchors = [akTop, akRight]
      Caption = 'Images'
      Enabled = False
      TabOrder = 7
      OnClick = ButtonImagesClick
    end
  end
  object DBGrid: TDBGrid
    Left = 0
    Top = 385
    Width = 559
    Height = 79
    Align = alClient
    DataSource = DataSourceSearch
    TabOrder = 1
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'MS Sans Serif'
    TitleFont.Style = []
    Visible = False
    OnDblClick = DBGridDblClick
  end
  object PageControlAdvanced: TPageControl
    Left = 0
    Top = 73
    Width = 559
    Height = 312
    ActivePage = TabSheetExif
    Align = alTop
    TabOrder = 2
    Visible = False
    object TabSheetExif: TTabSheet
      Caption = 'Exif'
      object Label5: TLabel
        Left = 8
        Top = 224
        Width = 65
        Height = 13
        Caption = 'Focal Length;'
      end
      object Label8: TLabel
        Left = 8
        Top = 200
        Width = 55
        Height = 13
        Caption = 'ISO Speed:'
      end
      object Label7: TLabel
        Left = 8
        Top = 176
        Width = 50
        Height = 13
        Caption = 'Exp. Time;'
      end
      object Label6: TLabel
        Left = 8
        Top = 152
        Width = 46
        Height = 13
        Caption = 'FNumber:'
      end
      object Label2: TLabel
        Left = 8
        Top = 56
        Width = 26
        Height = 13
        Caption = 'Lens:'
      end
      object Label4: TLabel
        Left = 8
        Top = 32
        Width = 39
        Height = 13
        Caption = 'Camera:'
      end
      object Label3: TLabel
        Left = 8
        Top = 8
        Width = 66
        Height = 13
        Caption = 'Manufacturer:'
      end
      object Label10: TLabel
        Left = 8
        Top = 80
        Width = 52
        Height = 13
        Caption = 'Exp. Prog.:'
      end
      object Label11: TLabel
        Left = 8
        Top = 104
        Width = 44
        Height = 13
        Caption = 'Metering:'
      end
      object Label12: TLabel
        Left = 8
        Top = 256
        Width = 54
        Height = 13
        Caption = 'Date/Time:'
      end
      object Label13: TLabel
        Left = 8
        Top = 128
        Width = 28
        Height = 13
        Caption = 'Flash:'
      end
      object EditEditMaxFocalLength: TEdit
        Left = 216
        Top = 224
        Width = 121
        Height = 21
        TabOrder = 13
      end
      object EditMinFocalLength: TEdit
        Left = 88
        Top = 224
        Width = 121
        Height = 21
        TabOrder = 12
      end
      object EditMinIso: TEdit
        Left = 88
        Top = 200
        Width = 121
        Height = 21
        TabOrder = 10
      end
      object EditMaxIso: TEdit
        Left = 216
        Top = 200
        Width = 121
        Height = 21
        TabOrder = 11
      end
      object EditMaxTime: TEdit
        Left = 216
        Top = 176
        Width = 121
        Height = 21
        TabOrder = 9
      end
      object EditMinTime: TEdit
        Left = 88
        Top = 176
        Width = 121
        Height = 21
        TabOrder = 8
      end
      object EditMinFnumber: TEdit
        Left = 88
        Top = 152
        Width = 121
        Height = 21
        TabOrder = 6
      end
      object EditMaxFnumber: TEdit
        Left = 216
        Top = 152
        Width = 121
        Height = 21
        TabOrder = 7
      end
      object ComboBoxLens: TComboBox
        Left = 88
        Top = 56
        Width = 470
        Height = 21
        Anchors = [akLeft, akTop, akRight]
        ItemHeight = 13
        TabOrder = 2
      end
      object ComboBoxCamera: TComboBox
        Left = 88
        Top = 32
        Width = 470
        Height = 21
        Anchors = [akLeft, akTop, akRight]
        ItemHeight = 13
        TabOrder = 1
      end
      object ComboBoxManufacturer: TComboBox
        Left = 88
        Top = 8
        Width = 470
        Height = 21
        Anchors = [akLeft, akTop, akRight]
        ItemHeight = 13
        TabOrder = 0
      end
      object ComboBoxExposureProgram: TComboBox
        Left = 89
        Top = 80
        Width = 470
        Height = 21
        Style = csDropDownList
        Anchors = [akLeft, akTop, akRight]
        ItemHeight = 13
        TabOrder = 3
        Items.Strings = (
          '<Unknown>'
          'Not defined'
          'Manual'
          'Normal program'
          'Aperture priority'
          'Shutter priority'
          'Creative program'
          'Action program'
          'Portrait mode'
          'Landscape mode')
      end
      object ComboBoxMeteringMode: TComboBox
        Left = 89
        Top = 104
        Width = 470
        Height = 21
        Style = csDropDownList
        Anchors = [akLeft, akTop, akRight]
        ItemHeight = 13
        TabOrder = 4
        Items.Strings = (
          '<Unknown>'
          'Unknown'
          'Average'
          'Center-weighted average'
          'Spot'
          'Multi-Spot'
          'Multi-Segment'
          'Partial'
          '')
      end
      object DateTimePickerMinDate: TDateTimePicker
        Left = 88
        Top = 256
        Width = 81
        Height = 21
        CalAlignment = dtaLeft
        Date = 1
        Time = 1
        Checked = False
        DateFormat = dfShort
        DateMode = dmComboBox
        Kind = dtkDate
        ParseInput = False
        TabOrder = 14
      end
      object DateTimePickerMinTime: TDateTimePicker
        Left = 168
        Top = 256
        Width = 73
        Height = 21
        CalAlignment = dtaLeft
        Date = 1
        Time = 1
        Checked = False
        DateFormat = dfShort
        DateMode = dmComboBox
        Kind = dtkTime
        ParseInput = False
        TabOrder = 15
      end
      object DateTimePickerMaxDate: TDateTimePicker
        Left = 256
        Top = 256
        Width = 81
        Height = 21
        CalAlignment = dtaLeft
        Date = 1
        Time = 1
        Checked = False
        DateFormat = dfShort
        DateMode = dmComboBox
        Kind = dtkDate
        ParseInput = False
        TabOrder = 16
      end
      object DateTimePickerMaxTime: TDateTimePicker
        Left = 336
        Top = 256
        Width = 73
        Height = 21
        CalAlignment = dtaLeft
        Date = 1
        Time = 1
        Checked = False
        DateFormat = dfShort
        DateMode = dmComboBox
        Kind = dtkTime
        ParseInput = False
        TabOrder = 17
      end
      object ComboBoxFlash: TComboBox
        Left = 89
        Top = 128
        Width = 470
        Height = 21
        Style = csDropDownList
        Anchors = [akLeft, akTop, akRight]
        ItemHeight = 13
        TabOrder = 5
        Items.Strings = (
          '<Unknown>'
          'Off'
          'On'
          'On, No Red-Eye'
          'On, Red-Eye')
      end
    end
  end
  object QuerySearch: TQuery
    AfterOpen = QuerySearchAfterOpen
    BeforeClose = QuerySearchBeforeClose
    OnCalcFields = QuerySearchCalcFields
    DatabaseName = 'docManDB'
    SQL.Strings = (
      'select ID, ParentID, NAME from ITEM_TREE')
    Left = 192
    Top = 392
    object QuerySearchID: TIntegerField
      FieldName = 'ID'
      Origin = 'DOCMANDB."ITEM_TREE.DB".ID'
      Visible = False
    end
    object QuerySearchNAME: TStringField
      DisplayLabel = 'Name'
      DisplayWidth = 64
      FieldName = 'NAME'
      Origin = 'DOCMANDB."ITEM_TREE.DB".NAME'
      Size = 255
    end
    object QuerySearchParentID: TIntegerField
      FieldName = 'ParentID'
      Origin = 'DOCMANDB."ITEM_TREE.DB".PARENTID'
      Visible = False
    end
    object QuerySearchPATH: TStringField
      DisplayLabel = 'Path'
      DisplayWidth = 64
      FieldKind = fkCalculated
      FieldName = 'PATH'
      Size = 255
      Calculated = True
    end
  end
  object DataSourceSearch: TDataSource
    DataSet = QuerySearch
    Left = 392
    Top = 416
  end
  object TableCategories: TTable
    DatabaseName = 'docManDB'
    IndexName = 'CAT_NAME_IDX'
    TableName = 'CATEGORIES'
    Left = 72
    Top = 384
    object TableCategoriesID: TIntegerField
      FieldName = 'ID'
      Required = True
      Visible = False
    end
    object TableCategoriesNAME: TStringField
      DisplayLabel = 'Name'
      FieldName = 'NAME'
      Required = True
      Size = 255
    end
    object TableCategoriesMAX_ATTR_ID: TIntegerField
      FieldName = 'MAX_ATTR_ID'
      Required = True
      Visible = False
    end
  end
end
