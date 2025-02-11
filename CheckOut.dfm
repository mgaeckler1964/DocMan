object CheckOutForm: TCheckOutForm
  Left = 473
  Top = 451
  BorderStyle = bsDialog
  Caption = 'Check Out'
  ClientHeight = 115
  ClientWidth = 480
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
  object Label1: TLabel
    Left = 8
    Top = 8
    Width = 24
    Height = 13
    Caption = 'Task'
  end
  object ButtonOK: TButton
    Left = 8
    Top = 80
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 2
    OnClick = ButtonOKClick
  end
  object ButtonCancel: TButton
    Left = 96
    Top = 80
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 3
  end
  object ComboBoxTasks: TComboBox
    Left = 48
    Top = 8
    Width = 417
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 0
  end
  object CheckBoxFlag: TCheckBox
    Left = 8
    Top = 40
    Width = 265
    Height = 17
    Caption = 'Include Other File Extensions'
    TabOrder = 1
  end
  object QueryOpenTasks: TQuery
    DatabaseName = 'docManDB'
    SQL.Strings = (
      'select it.ID, p.name || '#39':'#39' || it.Name as Name, s.Task_Status'
      'from item_tree it, i_task_status s, item_tree p'
      'where it.Assigned_To = :actUser'
      'and it.status = s.id'
      'and s.completed = 0'
      'and s.in_progress = 1'
      'and it.parentId = p.id'
      'order by 2'
      ''
      ' '
      ' '
      ' '
      ' '
      ' ')
    Left = 216
    Top = 8
    ParamData = <
      item
        DataType = ftInteger
        Name = 'actUser'
        ParamType = ptInput
      end>
  end
end
