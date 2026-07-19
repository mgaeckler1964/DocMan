inherited EditXmlFileForm: TEditXmlFileForm
  Left = 703
  Top = 153
  Width = 627
  Height = 475
  Caption = 'EditXmlFileForm'
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  inherited Panel1: TPanel
    Top = 395
    Width = 611
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
    Width = 611
    Height = 395
    Align = alClient
    TabOrder = 1
    inherited Splitter2: TSplitter
      Height = 376
    end
    inherited XmlTreeView: TTreeView
      Height = 376
    end
    inherited Panel1: TPanel
      Width = 389
      Height = 376
      inherited Splitter1: TSplitter
        Width = 387
      end
      inherited AttributeGrid: TStringGrid
        Width = 387
        OnContextPopup = xmlEditorFrameAttributeGridContextPopup
      end
      inherited ValueMemo: TMemo
        Width = 387
        Height = 185
      end
    end
    inherited StatusBar: TStatusBar
      Top = 376
      Width = 611
    end
  end
end
