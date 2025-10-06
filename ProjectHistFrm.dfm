object ProjectHistoryForm: TProjectHistoryForm
  Left = 435
  Top = 621
  Width = 812
  Height = 307
  Caption = 'ProjectHistoryForm'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  ShowHint = True
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter1: TSplitter
    Left = 0
    Top = 174
    Width = 796
    Height = 5
    Cursor = crVSplit
    Align = alBottom
  end
  object DBGridVersions: TDBGrid
    Left = 0
    Top = 41
    Width = 796
    Height = 133
    Align = alClient
    DataSource = DataSourceVersions
    DefaultDrawing = False
    Options = [dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgTabs, dgConfirmDelete, dgCancelOnExit]
    TabOrder = 0
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'MS Sans Serif'
    TitleFont.Style = []
    OnDrawColumnCell = DBGridVersionsDrawColumnCell
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 796
    Height = 41
    Align = alTop
    TabOrder = 1
    object DBNavigator: TDBNavigator
      Left = 6
      Top = 8
      Width = 100
      Height = 25
      DataSource = DataSourceVersions
      VisibleButtons = [nbFirst, nbPrior, nbNext, nbLast]
      TabOrder = 0
    end
  end
  object DBMemoDescription: TDBMemo
    Left = 0
    Top = 179
    Width = 796
    Height = 89
    Align = alBottom
    DataField = 'DESCRIPTION'
    DataSource = DataSourceVersions
    TabOrder = 2
  end
  object DataSourceVersions: TDataSource
    DataSet = ClientDataSetVersions
    Left = 240
    Top = 80
  end
  object QueryVersions: TQuery
    DatabaseName = 'docManDB'
    SQL.Strings = (
      'select it.ID, it.NAME,'
      #9'fv.VERSION, fv.MIME_TYPE, fv.FILENAME, fv.FILESIZE,'
      
        #9'fv.FILEMODIFIEDDATE, fv.CREATEDDATE, fv.LOCKED, fv.DESCRIPTION,' +
        ' fv.STORAGE_ID,'
      #9'u.USERNAME'
      'from i_file_vers fv, item_tree it, user_tab u'
      'where fv.file_id = it.file_id'
      'and it.parentid = :theFolderID'
      'and fv.createdBy = u.ID'
      'order by fv.FILEMODIFIEDDATE DESC'
      ''
      ' '
      ' '
      ' '
      ' ')
    Left = 32
    Top = 88
    ParamData = <
      item
        DataType = ftInteger
        Name = 'theFolderID'
        ParamType = ptInput
      end>
    object QueryVersionsID: TIntegerField
      FieldName = 'ID'
      Origin = 'DOCMANDB."item_tree.DB".ID'
    end
    object QueryVersionsNAME: TStringField
      DisplayLabel = 'Name'
      DisplayWidth = 32
      FieldName = 'NAME'
      Origin = 'DOCMANDB."item_tree.DB".NAME'
      Size = 255
    end
    object QueryVersionsVERSION: TIntegerField
      DisplayLabel = 'Version'
      FieldName = 'VERSION'
      Origin = 'DOCMANDB."i_file_vers.DB".VERSION'
    end
    object QueryVersionsMIME_TYPE: TStringField
      DisplayLabel = 'Mime Type'
      DisplayWidth = 32
      FieldName = 'MIME_TYPE'
      Origin = 'DOCMANDB."i_file_vers.DB".MIME_TYPE'
      Size = 255
    end
    object QueryVersionsFILENAME: TStringField
      DisplayLabel = 'Filename'
      DisplayWidth = 32
      FieldName = 'FILENAME'
      Origin = 'DOCMANDB."i_file_vers.DB".FILENAME'
      Size = 255
    end
    object QueryVersionsFILESIZE: TIntegerField
      DisplayLabel = 'Size'
      FieldName = 'FILESIZE'
      Origin = 'DOCMANDB."i_file_vers.DB".FILESIZE'
    end
    object QueryVersionsFILEMODIFIEDDATE: TDateTimeField
      DisplayLabel = 'Modified Date'
      FieldName = 'FILEMODIFIEDDATE'
      Origin = 'DOCMANDB."i_file_vers.DB".FILEMODIFIEDDATE'
    end
    object QueryVersionsUSERNAME: TStringField
      DisplayLabel = 'Created By'
      DisplayWidth = 32
      FieldName = 'USERNAME'
      Origin = 'DOCMANDB."USER_TAB.DB".USERNAME'
      Size = 64
    end
    object QueryVersionsLOCKED: TIntegerField
      FieldName = 'LOCKED'
      Origin = 'DOCMANDB."i_file_vers.DB".LOCKED'
      Visible = False
    end
    object QueryVersionsDESCRIPTION: TMemoField
      FieldName = 'DESCRIPTION'
      Origin = 'DOCMANDB."i_file_vers.DB".DESCRIPTION'
      Visible = False
      BlobType = ftMemo
      Size = 1
    end
    object QueryVersionsSTORAGE_ID: TIntegerField
      FieldName = 'STORAGE_ID'
      Origin = 'DOCMANDB."i_file_vers.DB".STORAGE_ID'
    end
    object QueryVersionsCREATEDDATE: TDateTimeField
      FieldName = 'CREATEDDATE'
      Origin = 'DOCMANDB."i_file_vers.DB".CREATEDDATE'
    end
  end
  object ClientDataSetVersions: TClientDataSet
    Aggregates = <>
    FieldDefs = <>
    IndexDefs = <>
    Params = <>
    StoreDefs = True
    Left = 400
    Top = 72
    object ClientDataSetVersionsNAME: TStringField
      DisplayLabel = 'Name'
      DisplayWidth = 32
      FieldName = 'NAME'
      Size = 255
    end
    object ClientDataSetVersionsVERSION: TIntegerField
      DisplayLabel = 'Version'
      FieldName = 'VERSION'
    end
    object ClientDataSetVersionsMIME_TYPE: TStringField
      DisplayLabel = 'Mime Type'
      DisplayWidth = 32
      FieldName = 'MIME_TYPE'
      Size = 255
    end
    object ClientDataSetVersionsFILENAME: TStringField
      DisplayLabel = 'Filename'
      DisplayWidth = 32
      FieldName = 'FILENAME'
      Size = 255
    end
    object ClientDataSetVersionsFILESIZE: TIntegerField
      DisplayLabel = 'Size'
      FieldName = 'FILESIZE'
    end
    object ClientDataSetVersionsFILEMODIFIEDDATE: TDateTimeField
      DisplayLabel = 'Modified Date'
      FieldName = 'FILEMODIFIEDDATE'
    end
    object ClientDataSetVersionsUSERNAME: TStringField
      DisplayLabel = 'Created By'
      DisplayWidth = 32
      FieldName = 'USERNAME'
      Size = 64
    end
    object ClientDataSetVersionsLOCKED: TIntegerField
      FieldName = 'LOCKED'
      Visible = False
    end
    object ClientDataSetVersionsDESCRIPTION: TMemoField
      FieldName = 'DESCRIPTION'
      Visible = False
      BlobType = ftMemo
      Size = 1
    end
  end
end
