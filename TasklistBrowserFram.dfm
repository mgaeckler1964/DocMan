inherited TasklistBrowserFrame: TTasklistBrowserFrame
  inherited DrawGridContent: TDrawGrid
    Top = 33
    Height = 244
  end
  object Panel1: TPanel [1]
    Left = 0
    Top = 0
    Width = 443
    Height = 33
    Align = alTop
    TabOrder = 1
    object CheckBoxOwner: TCheckBox
      Left = 8
      Top = 8
      Width = 97
      Height = 17
      Caption = 'Your Own'
      TabOrder = 0
      OnClick = FilterChange
    end
    object ComboBoxStatusFilter: TComboBox
      Left = 296
      Top = 8
      Width = 201
      Height = 21
      Style = csDropDownList
      ItemHeight = 13
      TabOrder = 1
      OnChange = FilterChange
    end
    object ComboBoxTypeFilter: TComboBox
      Left = 80
      Top = 8
      Width = 201
      Height = 21
      Style = csDropDownList
      ItemHeight = 13
      TabOrder = 2
      OnChange = FilterChange
    end
  end
end
