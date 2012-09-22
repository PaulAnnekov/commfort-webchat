object ConnectionsList: TConnectionsList
  Left = 0
  Top = 0
  Margins.Left = 0
  Margins.Top = 0
  Margins.Right = 0
  Margins.Bottom = 0
  BorderStyle = bsSizeToolWin
  Caption = #1040#1082#1090#1080#1074#1085#1099#1077' '#1089#1086#1077#1076#1080#1085#1077#1085#1080#1103
  ClientHeight = 350
  ClientWidth = 624
  Color = clWindow
  Constraints.MinHeight = 130
  Constraints.MinWidth = 640
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object no_connections_label: TLabel
    Left = 8
    Top = 8
    Width = 105
    Height = 13
    Caption = 'No active connections'
  end
  object connections_grid: TKGrid
    Left = 0
    Top = 0
    Width = 624
    Height = 350
    Align = alClient
    BorderStyle = bsNone
    ColCount = 6
    FixedCols = 0
    FixedRows = 0
    MouseCellHintTime = 500
    Options = [goAlignLastCol, goAlwaysShowEditor, goColSizing, goDoubleBufferedCells, goEnterMoves, goFixedHorzLine, goFixedVertLine, goHeader, goHeaderAlignment, goHorzLine, goIndicateHiddenCells, goMouseCanHideCells, goMouseOverCells, goRangeSelect, goRowSelect, goThemes, goThemedCells, goVertLine]
    ParentShowHint = False
    PopupMenu = conn_actions_pop
    RowCount = 1
    ScrollBars = ssVertical
    ShowHint = True
    TabOrder = 0
    Visible = False
    OnDrawCell = connections_gridDrawCell
    OnMouseCellHint = connections_gridMouseCellHint
    OnMouseDown = connections_gridMouseDown
    OnResize = connections_gridResize
    ColWidths = (
      64
      64
      64
      64
      64
      299)
    RowHeights = (
      21)
  end
  object conn_actions_pop: TPopupMenu
    AutoPopup = False
    Left = 312
    Top = 304
    object DisconnectClientItem: TMenuItem
      Caption = #1054#1090#1082#1083#1102#1095#1080#1090#1100
      OnClick = DisconnectClientItemClick
    end
    object CopyIDItem: TMenuItem
      Caption = #1050#1086#1087#1080#1088#1086#1074#1072#1090#1100' ID'
      OnClick = CopyIDItemClick
    end
    object CopyNameItem: TMenuItem
      Caption = #1050#1086#1087#1080#1088#1086#1074#1072#1090#1100' '#1053#1080#1082
      OnClick = CopyNameItemClick
    end
    object CopyIPItem: TMenuItem
      Caption = #1050#1086#1087#1080#1088#1086#1074#1072#1090#1100' IP'
      OnClick = CopyIPItemClick
    end
    object CopyProgramItem: TMenuItem
      Caption = #1050#1086#1087#1080#1088#1086#1074#1072#1090#1100' '#1055#1088#1080#1083#1086#1078#1077#1085#1080#1077
      OnClick = CopyProgramItemClick
    end
  end
  object upd_timer: TTimer
    Interval = 3000
    OnTimer = upd_timerTimer
    Left = 224
    Top = 304
  end
end
