object WebWizardForm: TWebWizardForm
  Left = 89
  Top = 262
  Width = 782
  Height = 615
  Caption = 'WebWizardForm'
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
  object PaintBoxPreview: TPaintBox
    Left = 0
    Top = 137
    Width = 774
    Height = 410
    Align = alClient
    OnPaint = PaintBoxPreviewPaint
  end
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 774
    Height = 137
    ActivePage = TabSheetLayout
    Align = alTop
    TabOrder = 0
    object TabSheetLayout: TTabSheet
      Caption = 'Layout'
      object ComboBoxTopLeft: TComboBox
        Left = 8
        Top = 16
        Width = 145
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 0
        OnChange = EnableDisableLayout
        Items.Strings = (
          'Top'
          'Left'
          'TopLeft')
      end
      object ComboBoxTop: TComboBox
        Left = 168
        Top = 16
        Width = 145
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 1
        OnChange = EnableDisableLayout
        Items.Strings = (
          'Top'
          'None')
      end
      object ComboBoxLeft: TComboBox
        Left = 8
        Top = 48
        Width = 145
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 2
        OnChange = EnableDisableLayout
        Items.Strings = (
          'Left'
          'None')
      end
      object ComboBoxBottomLeft: TComboBox
        Left = 8
        Top = 80
        Width = 145
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 3
        OnChange = EnableDisableLayout
        Items.Strings = (
          'Left'
          'Bottom'
          'BottomLeft')
      end
      object ComboBoxBottom: TComboBox
        Left = 168
        Top = 80
        Width = 145
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 4
        OnChange = EnableDisableLayout
        Items.Strings = (
          'Bottom'
          'None')
      end
      object ComboBoxBottomRight: TComboBox
        Left = 328
        Top = 80
        Width = 145
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 5
        OnChange = EnableDisableLayout
        Items.Strings = (
          'Right'
          'Bottom'
          'BottomRight')
      end
      object ComboBoxTopRight: TComboBox
        Left = 328
        Top = 16
        Width = 145
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 6
        OnChange = EnableDisableLayout
        Items.Strings = (
          'Top'
          'Right'
          'TopRight')
      end
      object ComboBoxRight: TComboBox
        Left = 328
        Top = 48
        Width = 145
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 7
        OnChange = EnableDisableLayout
        Items.Strings = (
          'Right'
          'None')
      end
      object ComboBoxContent: TComboBox
        Left = 168
        Top = 48
        Width = 145
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 8
        OnChange = PaintBoxPreviewPaint
        Items.Strings = (
          'Left'
          'Center'
          'Right')
      end
    end
    object TabSheetSize: TTabSheet
      Caption = 'Size'
      ImageIndex = 1
      object EditTop: TEdit
        Left = 168
        Top = 16
        Width = 145
        Height = 21
        TabOrder = 0
        Text = '50'
        OnChange = PaintBoxPreviewPaint
      end
      object EditBottom: TEdit
        Left = 168
        Top = 80
        Width = 145
        Height = 21
        TabOrder = 1
        Text = '50'
        OnChange = PaintBoxPreviewPaint
      end
      object EditLeft: TEdit
        Left = 8
        Top = 48
        Width = 145
        Height = 21
        TabOrder = 2
        Text = '50'
        OnChange = PaintBoxPreviewPaint
      end
      object EditRight: TEdit
        Left = 328
        Top = 48
        Width = 145
        Height = 21
        TabOrder = 3
        Text = '50'
        OnChange = PaintBoxPreviewPaint
      end
      object EditContent: TEdit
        Left = 168
        Top = 48
        Width = 145
        Height = 21
        TabOrder = 4
        OnChange = PaintBoxPreviewPaint
      end
    end
    object TabSheetColor: TTabSheet
      Caption = 'Color'
      ImageIndex = 2
      object ComboBoxColorTopLeft: TComboBox
        Left = 8
        Top = 16
        Width = 145
        Height = 21
        ItemHeight = 13
        TabOrder = 0
        Text = 'blue'
        OnChange = PaintBoxPreviewPaint
        Items.Strings = (
          'white'
          'silver'
          'gray'
          'black'
          'red'
          'lime'
          'blue'
          'yellow'
          'aqua'
          'fuchsia'
          'maroon'
          'green'
          'navy'
          'olive'
          'teal'
          'purple'
          'orange')
      end
      object ComboBoxColorTop: TComboBox
        Left = 168
        Top = 16
        Width = 145
        Height = 21
        ItemHeight = 13
        TabOrder = 1
        Text = 'green'
        OnChange = PaintBoxPreviewPaint
        Items.Strings = (
          'white'
          'silver'
          'gray'
          'black'
          'red'
          'lime'
          'blue'
          'yellow'
          'aqua'
          'fuchsia'
          'maroon'
          'green'
          'navy'
          'olive'
          'teal'
          'purple'
          'orange')
      end
      object ComboBoxColorTopRight: TComboBox
        Left = 328
        Top = 16
        Width = 145
        Height = 21
        ItemHeight = 13
        TabOrder = 2
        Text = 'maroon'
        OnChange = PaintBoxPreviewPaint
        Items.Strings = (
          'white'
          'silver'
          'gray'
          'black'
          'red'
          'lime'
          'blue'
          'yellow'
          'aqua'
          'fuchsia'
          'maroon'
          'green'
          'navy'
          'olive'
          'teal'
          'purple'
          'orange')
      end
      object ComboBoxColorLeft: TComboBox
        Left = 8
        Top = 48
        Width = 145
        Height = 21
        ItemHeight = 13
        TabOrder = 3
        Text = 'aqua'
        OnChange = PaintBoxPreviewPaint
        Items.Strings = (
          'white'
          'silver'
          'gray'
          'black'
          'red'
          'lime'
          'blue'
          'yellow'
          'aqua'
          'fuchsia'
          'maroon'
          'green'
          'navy'
          'olive'
          'teal'
          'purple'
          'orange')
      end
      object ComboBoxColorContent: TComboBox
        Left = 168
        Top = 48
        Width = 145
        Height = 21
        ItemHeight = 13
        TabOrder = 4
        Text = 'white'
        OnChange = PaintBoxPreviewPaint
        Items.Strings = (
          'white'
          'silver'
          'gray'
          'black'
          'red'
          'lime'
          'blue'
          'yellow'
          'aqua'
          'fuchsia'
          'maroon'
          'green'
          'navy'
          'olive'
          'teal'
          'purple'
          'orange')
      end
      object ComboBoxColorRight: TComboBox
        Left = 328
        Top = 48
        Width = 145
        Height = 21
        ItemHeight = 13
        TabOrder = 5
        Text = 'yellow'
        OnChange = PaintBoxPreviewPaint
        Items.Strings = (
          'white'
          'silver'
          'gray'
          'black'
          'red'
          'lime'
          'blue'
          'yellow'
          'aqua'
          'fuchsia'
          'maroon'
          'green'
          'navy'
          'olive'
          'teal'
          'purple'
          'orange')
      end
      object ComboBoxColorBottomLeft: TComboBox
        Left = 8
        Top = 80
        Width = 145
        Height = 21
        ItemHeight = 13
        TabOrder = 6
        Text = 'fuchsia'
        OnChange = PaintBoxPreviewPaint
        Items.Strings = (
          'white'
          'silver'
          'gray'
          'black'
          'red'
          'lime'
          'blue'
          'yellow'
          'aqua'
          'fuchsia'
          'maroon'
          'green'
          'navy'
          'olive'
          'teal'
          'purple'
          'orange')
      end
      object ComboBoxColorBottom: TComboBox
        Left = 168
        Top = 80
        Width = 145
        Height = 21
        ItemHeight = 13
        TabOrder = 7
        Text = 'red'
        OnChange = PaintBoxPreviewPaint
        Items.Strings = (
          'white'
          'silver'
          'gray'
          'black'
          'red'
          'lime'
          'blue'
          'yellow'
          'aqua'
          'fuchsia'
          'maroon'
          'green'
          'navy'
          'olive'
          'teal'
          'purple'
          'orange')
      end
      object ComboBoxColorBottomRight: TComboBox
        Left = 328
        Top = 80
        Width = 145
        Height = 21
        ItemHeight = 13
        TabOrder = 8
        Text = 'purple'
        OnChange = PaintBoxPreviewPaint
        Items.Strings = (
          'white'
          'silver'
          'gray'
          'black'
          'red'
          'lime'
          'blue'
          'yellow'
          'aqua'
          'fuchsia'
          'maroon'
          'green'
          'navy'
          'olive'
          'teal'
          'purple'
          'orange')
      end
    end
    object TabSheetContent: TTabSheet
      Caption = 'Content'
      ImageIndex = 3
      object ComboBoxContentTopLeft: TComboBox
        Left = 8
        Top = 16
        Width = 145
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 0
        OnChange = EnableDisableLayout
        Items.Strings = (
          'Empty'
          'From Layout'
          'From Item'
          'Menu')
      end
      object ComboBoxContentTop: TComboBox
        Left = 168
        Top = 16
        Width = 145
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 1
        OnChange = EnableDisableLayout
        Items.Strings = (
          'Empty'
          'From Layout'
          'From Item'
          'Menu')
      end
      object ComboBoxContentLeft: TComboBox
        Left = 8
        Top = 48
        Width = 145
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 2
        OnChange = EnableDisableLayout
        Items.Strings = (
          'Empty'
          'From Layout'
          'From Item'
          'Menu')
      end
      object ComboBoxContentBottomLeft: TComboBox
        Left = 8
        Top = 80
        Width = 145
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 3
        OnChange = EnableDisableLayout
        Items.Strings = (
          'Empty'
          'From Layout'
          'From Item'
          'Menu')
      end
      object ComboBoxContentBottom: TComboBox
        Left = 168
        Top = 80
        Width = 145
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 4
        OnChange = EnableDisableLayout
        Items.Strings = (
          'Empty'
          'From Layout'
          'From Item'
          'Menu')
      end
      object ComboBoxContentBottomRight: TComboBox
        Left = 328
        Top = 80
        Width = 145
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 5
        OnChange = EnableDisableLayout
        Items.Strings = (
          'Empty'
          'From Layout'
          'From Item'
          'Menu')
      end
      object ComboBoxContentTopRight: TComboBox
        Left = 328
        Top = 16
        Width = 145
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 6
        OnChange = EnableDisableLayout
        Items.Strings = (
          'Empty'
          'From Layout'
          'From Item'
          'Menu')
      end
      object ComboBoxContentRight: TComboBox
        Left = 328
        Top = 48
        Width = 145
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 7
        OnChange = EnableDisableLayout
        Items.Strings = (
          'Empty'
          'From Layout'
          'From Item'
          'Menu')
      end
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 547
    Width = 774
    Height = 41
    Align = alBottom
    TabOrder = 1
    object ButtonOK: TButton
      Left = 8
      Top = 8
      Width = 75
      Height = 25
      Caption = 'OK'
      Default = True
      ModalResult = 1
      TabOrder = 0
    end
    object ButtonCancel: TButton
      Left = 88
      Top = 8
      Width = 75
      Height = 25
      Cancel = True
      Caption = 'Cancel'
      ModalResult = 2
      TabOrder = 1
    end
    object ButtonEdit: TButton
      Left = 168
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Edit'
      TabOrder = 2
      OnClick = ButtonEditClick
    end
    object CheckBoxNoExample: TCheckBox
      Left = 256
      Top = 8
      Width = 97
      Height = 17
      Caption = 'No Example'
      TabOrder = 3
    end
  end
end
