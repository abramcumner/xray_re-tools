object frmProgress: TfrmProgress
  Left = 0
  Top = 0
  Caption = 'frmProgress'
  ClientHeight = 88
  ClientWidth = 357
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object lbl1: TLabel
    Left = 8
    Top = 8
    Width = 16
    Height = 13
    Caption = 'lbl1'
  end
  object pbProgress: TProgressBar
    Left = 8
    Top = 32
    Width = 341
    Height = 17
    Smooth = True
    TabOrder = 0
  end
  object btnCancel: TButton
    Left = 136
    Top = 55
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    TabOrder = 1
    OnClick = btnCancelClick
  end
end
