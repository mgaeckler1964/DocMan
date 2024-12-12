object RemoteServerForm: TRemoteServerForm
  Left = 304
  Top = 399
  Width = 802
  Height = 330
  Caption = 'Remote Server'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnClose = FormClose
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 794
    Height = 41
    Align = alTop
    TabOrder = 0
    object DBNavigatorRemoteServer: TDBNavigator
      Left = 8
      Top = 8
      Width = 240
      Height = 25
      DataSource = DataSourceRemoteServer
      TabOrder = 0
    end
  end
  object DBGridRemoteServer: TDBGrid
    Left = 0
    Top = 41
    Width = 794
    Height = 262
    Align = alClient
    DataSource = DataSourceRemoteServer
    TabOrder = 1
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'MS Sans Serif'
    TitleFont.Style = []
  end
  object DataSourceRemoteServer: TDataSource
    DataSet = TableRemoteServer
    Left = 72
    Top = 72
  end
  object TableRemoteServer: TTable
    AfterInsert = TableRemoteServerAfterInsert
    DatabaseName = 'docManDB'
    TableName = 'REMOTE_SERVER'
    Left = 152
    Top = 72
    object TableRemoteServerID: TIntegerField
      FieldName = 'ID'
      Required = True
      Visible = False
    end
    object TableRemoteServerREMOTE_SERVER_NAME: TStringField
      DisplayLabel = 'Server Name'
      FieldName = 'REMOTE_SERVER_NAME'
      Required = True
      Size = 32
    end
    object TableRemoteServerREMOTE_SERVER_URL: TStringField
      DisplayLabel = 'Server URL'
      FieldName = 'REMOTE_SERVER_URL'
      Required = True
      Size = 255
    end
  end
end
