inherited EditXmlFileForm: TEditXmlFileForm
  Left = 490
  Top = 312
  Width = 627
  Height = 475
  Caption = 'EditXmlFileForm'
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  inherited Panel1: TPanel
    Top = 407
    Width = 619
    object ButtonTransform: TButton
      Left = 168
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Transform'
      Enabled = False
      TabOrder = 2
      OnClick = ButtonTransformClick
    end
    object ButtonCheck: TButton
      Left = 248
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Check'
      TabOrder = 3
      OnClick = ButtonCheckClick
    end
    object ButtonRenderer: TButton
      Left = 328
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Renderer'
      TabOrder = 4
      OnClick = ButtonRendererClick
    end
  end
  inline xmlEditorFrame: TxmlEditorFrame
    Width = 619
    Height = 407
    Align = alClient
    TabOrder = 1
    inherited Splitter2: TSplitter
      Height = 388
    end
    inherited XmlTreeView: TTreeView
      Height = 388
    end
    inherited Panel1: TPanel
      Width = 397
      Height = 388
      inherited Splitter1: TSplitter
        Width = 395
      end
      inherited AttributeGrid: TStringGrid
        Width = 395
        OnContextPopup = xmlEditorFrameAttributeGridContextPopup
      end
      inherited ValueMemo: TMemo
        Width = 395
        Height = 197
      end
    end
    inherited StatusBar: TStatusBar
      Top = 388
      Width = 619
    end
  end
end
