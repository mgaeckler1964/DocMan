object TaskFilesForm: TTaskFilesForm
  Left = 305
  Top = 324
  BorderStyle = bsDialog
  Caption = 'Task Files'
  ClientHeight = 298
  ClientWidth = 825
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 8
    Width = 70
    Height = 13
    Caption = 'Available Files:'
  end
  object Label2: TLabel
    Left = 456
    Top = 8
    Width = 70
    Height = 13
    Caption = 'Assigned Files:'
  end
  object DBGridOpen: TDBGrid
    Left = 8
    Top = 32
    Width = 361
    Height = 257
    DataSource = OpenDataSource
    TabOrder = 0
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'MS Sans Serif'
    TitleFont.Style = []
  end
  object DBGridSelected: TDBGrid
    Left = 456
    Top = 32
    Width = 361
    Height = 257
    DataSource = SelectedDataSource
    TabOrder = 5
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'MS Sans Serif'
    TitleFont.Style = []
  end
  object ButtonAdd: TButton
    Left = 376
    Top = 16
    Width = 75
    Height = 25
    Caption = 'Add >>'
    TabOrder = 1
    OnClick = ButtonAddClick
  end
  object ButtonRemove: TButton
    Left = 376
    Top = 48
    Width = 75
    Height = 25
    Caption = 'Remove <<'
    TabOrder = 2
    OnClick = ButtonRemoveClick
  end
  object ButtonClose: TButton
    Left = 376
    Top = 256
    Width = 75
    Height = 25
    Caption = 'Close'
    ModalResult = 1
    TabOrder = 4
  end
  object ButtonCheckIn: TButton
    Left = 376
    Top = 224
    Width = 75
    Height = 25
    Caption = 'Check In'
    TabOrder = 3
    OnClick = ButtonCheckInClick
  end
  object CheckBoxBranch: TCheckBox
    Left = 376
    Top = 200
    Width = 81
    Height = 17
    Caption = 'Branch'
    TabOrder = 6
  end
  object OpenQuery: TQuery
    OnCalcFields = OpenQueryCalcFields
    DatabaseName = 'docManDB'
    SQL.Strings = (
      'select ID, PARENTID, NAME'
      'from ITEM_TREE'
      'where RESERVEDBY=:actUser'
      'and RESERVEDON = :actMachine'
      'and (RESERVEDFOR = 0 or RESERVEDFOR is null)'
      'order by NAME')
    Left = 16
    Top = 152
    ParamData = <
      item
        DataType = ftInteger
        Name = 'actUser'
        ParamType = ptInput
      end
      item
        DataType = ftString
        Name = 'actMachine'
        ParamType = ptInput
      end>
    object OpenQueryID: TIntegerField
      FieldName = 'ID'
      Origin = 'DOCMANDB."ITEM_TREE.DB".ID'
      Visible = False
    end
    object OpenQueryPARENTID: TIntegerField
      FieldName = 'PARENTID'
      Origin = 'DOCMANDB."ITEM_TREE.DB".PARENTID'
      Visible = False
    end
    object OpenQueryNAME: TStringField
      FieldName = 'NAME'
      Origin = 'DOCMANDB."ITEM_TREE.DB".NAME'
      Visible = False
      Size = 255
    end
    object OpenQueryPATH: TStringField
      DisplayLabel = 'File'
      FieldKind = fkCalculated
      FieldName = 'PATH'
      Size = 255
      Calculated = True
    end
  end
  object OpenDataSource: TDataSource
    DataSet = OpenQuery
    Left = 88
    Top = 160
  end
  object SelectedQuery: TQuery
    OnCalcFields = SelectedQueryCalcFields
    DatabaseName = 'docManDB'
    SQL.Strings = (
      'select ID, PARENTID, NAME'
      'from ITEM_TREE'
      'where RESERVEDFOR = :actTask'
      'order by NAME')
    Left = 488
    Top = 56
    ParamData = <
      item
        DataType = ftInteger
        Name = 'actTask'
        ParamType = ptInput
      end>
    object SelectedQueryID: TIntegerField
      FieldName = 'ID'
      Origin = 'DOCMANDB."ITEM_TREE.DB".ID'
      Visible = False
    end
    object SelectedQueryPARENTID: TIntegerField
      FieldName = 'PARENTID'
      Origin = 'DOCMANDB."ITEM_TREE.DB".PARENTID'
      Visible = False
    end
    object SelectedQueryNAME: TStringField
      FieldName = 'NAME'
      Origin = 'DOCMANDB."ITEM_TREE.DB".NAME'
      Visible = False
      Size = 255
    end
    object SelectedQueryPATH: TStringField
      DisplayLabel = 'File'
      FieldKind = fkCalculated
      FieldName = 'PATH'
      Size = 255
      Calculated = True
    end
  end
  object SelectedDataSource: TDataSource
    DataSet = SelectedQuery
    Left = 480
    Top = 128
  end
  object AddQuery: TQuery
    DatabaseName = 'docManDB'
    SQL.Strings = (
      'update ITEM_TREE'
      'set RESERVEDFOR=:theTask'
      'where ID=:theFile'
      ' ')
    Left = 216
    Top = 80
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'theTask'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'theFile'
        ParamType = ptUnknown
      end>
  end
  object RemoveQuery: TQuery
    DatabaseName = 'docManDB'
    SQL.Strings = (
      'update ITEM_TREE'
      'set RESERVEDFOR = null'
      'where ID=:theFile'
      ' ')
    Left = 216
    Top = 136
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'theFile'
        ParamType = ptUnknown
      end>
  end
end
