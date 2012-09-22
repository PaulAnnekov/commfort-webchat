object UpdateForm: TUpdateForm
  Left = 0
  Top = 0
  BorderStyle = bsToolWindow
  Caption = 'Update'
  ClientHeight = 374
  ClientWidth = 457
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Padding.Left = 10
  Padding.Top = 10
  Padding.Right = 10
  Padding.Bottom = 10
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object downloading_state_label: TLabel
    AlignWithMargins = True
    Left = 13
    Top = 94
    Width = 431
    Height = 11
    Margins.Top = 5
    Align = alTop
    Alignment = taRightJustify
    AutoSize = False
  end
  object checking_state_label: TLabel
    AlignWithMargins = True
    Left = 13
    Top = 34
    Width = 431
    Height = 13
    Margins.Top = 5
    Align = alTop
    AutoSize = False
    ExplicitTop = 13
    ExplicitWidth = 75
  end
  object current_version_label: TLabel
    AlignWithMargins = True
    Left = 13
    Top = 13
    Width = 431
    Height = 13
    Align = alTop
    AutoSize = False
    Caption = 'Current version: 2.00 beta'
    ExplicitTop = -1
  end
  object download_progress: TProgressBar
    AlignWithMargins = True
    Left = 13
    Top = 55
    Width = 431
    Height = 31
    Margins.Top = 5
    Align = alTop
    DoubleBuffered = True
    ParentDoubleBuffered = False
    Step = 1
    TabOrder = 0
    ExplicitTop = 72
  end
  object update_actions_panel: TPanel
    AlignWithMargins = True
    Left = 13
    Top = 334
    Width = 431
    Height = 30
    Margins.Top = 5
    Margins.Bottom = 0
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 1
    object update_plugin_button: TButton
      Left = 0
      Top = 3
      Width = 121
      Height = 25
      Caption = 'Update'
      Enabled = False
      TabOrder = 0
      OnClick = update_plugin_buttonClick
    end
  end
  object ScrollBox1: TScrollBox
    AlignWithMargins = True
    Left = 13
    Top = 113
    Width = 431
    Height = 213
    Margins.Top = 5
    VertScrollBar.Tracking = True
    Align = alClient
    BorderStyle = bsNone
    DoubleBuffered = True
    ParentDoubleBuffered = False
    TabOrder = 2
    object new_version_label: TLabel
      Left = 0
      Top = 0
      Width = 431
      Height = 13
      Align = alTop
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      ExplicitWidth = 3
    end
    object new_version_info_label: TLabel
      AlignWithMargins = True
      Left = 0
      Top = 23
      Width = 431
      Height = 13
      Margins.Left = 0
      Margins.Top = 10
      Margins.Right = 0
      Margins.Bottom = 0
      Align = alTop
      WordWrap = True
      ExplicitTop = 13
      ExplicitWidth = 3
    end
  end
end
