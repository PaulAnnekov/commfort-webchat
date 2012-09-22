object SettingsForm: TSettingsForm
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080
  ClientHeight = 407
  ClientWidth = 539
  Color = clBtnFace
  Constraints.MaxHeight = 435
  Constraints.MaxWidth = 545
  Constraints.MinHeight = 435
  Constraints.MinWidth = 545
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Icon.Data = {
    0000010001001010000001000800680500001600000028000000100000002000
    0000010008000000000040010000000000000000000000010000000000000000
    0000FFFFFF005D99CD0070A7D6005193CA007AAFDC0074AAD8005B98CB004F90
    C70072A7D40076AAD70077ACD90078AEDB003D7DB400598DBB004180B700558C
    BA004B88BC003F80B7005B8FBC00518BBC003B7DB4003A7DB600256399003167
    950025659C0033679400326897002B679B00386C9900236399001A507E001C52
    8000124E800016548A001A4E7B001B5383001751820026669C00356996002866
    9C00386D99002B689B002567A00034689600286296003F7FB5005089BB00387B
    B40048FF85006CFF9D0090FFB500B4FFCD00003E0000005D0000007C0000009B
    000000BA000000D9000000F0000024FF240048FF48006CFF6C0090FF9000B4FF
    B400143E00001E5D0000287C0000329B00003CBA000046D9000055F000006DFF
    240085FF48009DFF6C00B5FF9000CDFFB4002A3E00003F5D0000547C0000699B
    00007EBA000093D90000AAF00000B6FF2400C2FF4800CEFF6C00DAFF9000E6FF
    B4003E3E00005D5D00007C7C00009B9B0000BABA0000D9D90000F0F00000FFFF
    2400FFFF4800FFFF6C00FFFF9000FFFFB4003E2A00005D3F00007C5400009B69
    0000BA7E0000D9930000F0AA0000FFB62400FFC24800FFCE6C00FFDA9000FFE6
    B4003E1400005D1E00007C2800009B320000BA3C0000D9460000F0550000FF6D
    2400FF854800FF9D6C00FFB59000FFCDB4003E0000005D0000007C0000009B00
    0000BA000000D9000000F0000000FF242400FF484800FF6C6C00FF909000FFB4
    B4003E0014005D001E007C0028009B003200BA003C00D9004600F0005500FF24
    6D00FF488500FF6C9D00FF90B500FFB4CD003E002A005D003F007C0054009B00
    6900BA007E00D9009300F000AA00FF24B600FF48C200FF6CCE00FF90DA00FFB4
    E6003E003E005D005D007C007C009B009B00BA00BA00D900D900F000F000FF24
    FF00FF48FF00FF6CFF00FF90FF00FFB4FF002A003E003F005D0054007C006900
    9B007E00BA009300D900AA00F000B624FF00C248FF00CE6CFF00DA90FF00E6B4
    FF0014003E001E005D0028007C0032009B003C00BA004600D9005500F0006D24
    FF008548FF009D6CFF00B590FF00CDB4FF0000000000C8A06700C8A06700B804
    0000D0A06700B004000000000000000000000000000000000000000000000000
    00000000000000000000000000000000000004A1670004A167007C0400001000
    0000010001000000000040000000000000000000000002000000000000000000
    0000FFFFFF00481B49004B0000004C0000003B3C0E00003E3F00134100004300
    4400323334000035040037380000090A0B000000000000000000000000000000
    0000000000000000000000000000000000000000000000000000000000000000
    0000000000000000000000000000000000000000000000000000000000000000
    0000000000000000000000000000000000000000000000000000000000000000
    0000000000000000000000000000000000000000000000000000000000000000
    00002E002F000000000000300000000000262728292A0000002B2C1B2D000000
    001F20212223000000242500000000001718191A001B1C00001D1E000000000D
    0E0F001011121300001415000016000203000004050006070008090A0B0C0000
    0000000000000000000000000000000000000000000000000000000000000000
    0000000000000000000000000000000000000000000000000000000000000000
    000000000000000000000000000000000000000000000000000000000000FFFF
    0000FFFF0000FFFF0000FFFF0000F5FB0000E0E10000E0E70000C26700008866
    000099200000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000}
  OldCreateOrder = False
  Position = poDesigned
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object load_first_config_but: TButton
    Left = 176
    Top = 477
    Width = 219
    Height = 25
    Caption = #1047#1072#1087#1091#1089#1090#1080#1090#1100' '#1084#1072#1089#1090#1077#1088' '#1087#1077#1088#1074#1080#1095#1085#1086#1081' '#1085#1072#1089#1090#1088#1086#1081#1082#1080
    TabOrder = 0
    Visible = False
    OnClick = load_first_config_butClick
  end
  object pages: TPanel
    Left = 129
    Top = 0
    Width = 410
    Height = 407
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 1
    object wht_s_page: TPanel
      Left = 0
      Top = 0
      Width = 410
      Height = 371
      Align = alClient
      BevelOuter = bvNone
      Padding.Left = 10
      Padding.Top = 10
      Padding.Right = 10
      Padding.Bottom = 10
      TabOrder = 3
      object wht_main_lbl: TLabel
        AlignWithMargins = True
        Left = 13
        Top = 13
        Width = 129
        Height = 13
        Margins.Bottom = 10
        Align = alTop
        Alignment = taCenter
        Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080' '#1073#1077#1083#1086#1075#1086' '#1089#1087#1080#1089#1082#1072
        Layout = tlCenter
      end
      object wht_tip_lbl: TLabel
        Left = 10
        Top = 36
        Width = 390
        Height = 36
        Align = alTop
        AutoSize = False
        Caption = 
          #1042#1074#1077#1076#1080#1090#1077' '#1085#1080#1082#1080' '#1087#1086#1083#1100#1079#1086#1074#1072#1090#1077#1083#1077#1081', IP '#1082#1086#1090#1086#1088#1099#1093' '#1085#1077' '#1089#1083#1077#1076#1091#1077#1090' '#1086#1090#1086#1073#1088#1072#1078#1072#1090#1100' '#1074' '#1074 +
          #1077#1073'-'#1080#1085#1090#1077#1088#1092#1077#1081#1089#1077'. '#1055#1086' '#1086#1076#1085#1086#1084#1091' '#1085#1080#1082#1091' '#1085#1072' '#1089#1090#1088#1086#1095#1082#1091':'
        WordWrap = True
        ExplicitTop = 38
        ExplicitWidth = 388
      end
      object wht_nick_list: TListBox
        Left = 10
        Top = 102
        Width = 388
        Height = 154
        ExtendedSelect = False
        ItemHeight = 13
        Sorted = True
        TabOrder = 0
        OnClick = wht_nick_listClick
        OnKeyDown = wht_nick_listKeyDown
      end
      object add_nick_edit: TEdit
        Left = 8
        Top = 76
        Width = 361
        Height = 21
        TabOrder = 1
        OnEnter = add_nick_editEnter
        OnKeyPress = add_nick_editKeyPress
      end
      object add_del_nick_btn: TButton
        Left = 375
        Top = 76
        Width = 23
        Height = 20
        Caption = '+'
        TabOrder = 2
        OnClick = add_del_nick_btnClick
      end
    end
    object add_s_page: TPanel
      Left = 0
      Top = 0
      Width = 410
      Height = 371
      Align = alClient
      BevelOuter = bvNone
      Locked = True
      Padding.Left = 10
      Padding.Top = 10
      Padding.Right = 10
      Padding.Bottom = 10
      TabOrder = 0
      object add_main_lbl: TLabel
        AlignWithMargins = True
        Left = 13
        Top = 13
        Width = 144
        Height = 13
        Margins.Bottom = 10
        Align = alTop
        Alignment = taCenter
        Caption = #1044#1086#1087#1086#1083#1085#1080#1090#1077#1083#1100#1085#1099#1077' '#1085#1072#1089#1090#1088#1086#1081#1082#1080
        Layout = tlCenter
      end
      object auto_start_check: TCheckBox
        Left = 10
        Top = 76
        Width = 390
        Height = 20
        Hint = #1040#1074#1090#1086#1084#1072#1090#1080#1095#1077#1089#1082#1080' '#1074#1082#1083#1102#1095#1072#1077#1090#1089#1103' '#1087#1086#1083#1085#1072#1103' '#1088#1072#1073#1086#1090#1072' '#1087#1083#1072#1075#1080#1085#1072
        Align = alTop
        Caption = #1040#1074#1090#1086#1074#1082#1083#1102#1095#1077#1085#1080#1077' '#1087#1083#1072#1075#1080#1085#1072' '#1087#1088#1080' '#1087#1077#1088#1077#1079#1072#1075#1088#1091#1079#1082#1077' '#1089#1077#1088#1074#1077#1088#1072
        ParentShowHint = False
        ShowHint = True
        TabOrder = 0
      end
      object file_log_check: TCheckBox
        Left = 10
        Top = 36
        Width = 390
        Height = 20
        Align = alTop
        Caption = #1051#1086#1075#1080#1088#1086#1074#1072#1085#1080#1077' '#1074' '#1092#1072#1081#1083' ('#1087#1072#1087#1082#1072' logs)'
        TabOrder = 1
      end
      object show_work_time_check: TCheckBox
        Left = 10
        Top = 96
        Width = 390
        Height = 20
        Hint = 
          #1054#1087#1088#1077#1076#1077#1083#1103#1077#1090' '#1089#1086#1089#1090#1086#1103#1085#1080#1103' '#1087#1086#1082#1072#1079#1072' '#1074' '#1075#1083#1072#1074#1085#1086#1084' '#1086#1082#1085#1077' '#1074#1088#1077#1084#1077#1085#1080' '#1088#1072#1073#1086#1090#1099' '#1087#1083#1072#1075#1080#1085 +
          #1072' '#1089' '#1084#1086#1084#1077#1085#1090#1072' '#1079#1072#1087#1091#1089#1082#1072
        Margins.Left = 10
        Margins.Top = 0
        Margins.Right = 0
        Margins.Bottom = 0
        Align = alTop
        Caption = #1055#1086#1082#1072#1079#1099#1074#1072#1090#1100' '#1074#1088#1077#1084#1103' '#1088#1072#1073#1086#1090#1099' '#1087#1083#1072#1075#1080#1085#1072' '#1089' '#1084#1086#1084#1077#1085#1090#1072' '#1079#1072#1087#1091#1089#1082#1072' '
        ParentShowHint = False
        ShowHint = True
        TabOrder = 2
      end
      object check_version_check: TCheckBox
        Left = 10
        Top = 116
        Width = 390
        Height = 20
        Hint = #1056#1072#1079' '#1074' '#1089#1091#1090#1082#1080' '#1087#1088#1086#1074#1077#1088#1103#1077#1090' '#1085#1072#1083#1080#1095#1080#1077' '#1085#1086#1074#1086#1081' '#1074#1077#1088#1089#1080#1080
        Align = alTop
        Caption = #1040#1074#1090#1086#1084#1072#1090#1080#1095#1077#1089#1082#1080' '#1087#1088#1086#1074#1077#1088#1103#1090#1100' '#1086#1073#1085#1086#1074#1083#1077#1085#1080#1103
        ParentShowHint = False
        ShowHint = True
        TabOrder = 3
        OnClick = check_version_checkClick
      end
      object auto_update_check: TCheckBox
        AlignWithMargins = True
        Left = 20
        Top = 136
        Width = 380
        Height = 20
        Hint = #1053#1086#1074#1072#1103' '#1074#1077#1088#1089#1080#1103' '#1072#1074#1090#1086#1084#1072#1090#1080#1095#1077#1089#1082#1080' '#1079#1072#1075#1088#1091#1078#1072#1077#1090#1089#1103' '#1089' '#1080#1085#1090#1077#1088#1085#1077#1090#1072
        Margins.Left = 10
        Margins.Top = 0
        Margins.Right = 0
        Margins.Bottom = 0
        Align = alTop
        Caption = #1040#1074#1090#1086#1084#1072#1090#1080#1095#1077#1089#1082#1080' '#1086#1073#1085#1086#1074#1083#1103#1090#1100#1089#1103' '#1076#1086' '#1087#1086#1089#1083#1077#1076#1085#1077#1081' '#1074#1077#1088#1089#1080#1080
        ParentShowHint = False
        ShowHint = True
        TabOrder = 4
      end
      object log_notifications_check: TCheckBox
        Left = 10
        Top = 56
        Width = 390
        Height = 20
        Hint = #1042#1082#1083#1102#1095#1072#1077#1090' '#1083#1086#1075#1080#1088#1086#1074#1072#1085#1080#1077' '#1076#1077#1081#1089#1090#1074#1080#1081' '#1087#1088#1086#1080#1089#1093#1086#1076#1103#1097#1080#1093' '#1074' '#1095#1072#1090#1077
        Align = alTop
        Caption = #1051#1086#1075#1080#1088#1086#1074#1072#1090#1100' '#1076#1077#1081#1089#1090#1074#1080#1103' '#1074' '#1095#1072#1090#1077
        TabOrder = 5
      end
    end
    object images_s_page: TPanel
      Left = 0
      Top = 0
      Width = 410
      Height = 371
      Align = alClient
      BevelOuter = bvNone
      Padding.Left = 10
      Padding.Top = 10
      Padding.Right = 10
      Padding.Bottom = 10
      TabOrder = 1
      object bot_main_lbl: TLabel
        AlignWithMargins = True
        Left = 13
        Top = 13
        Width = 384
        Height = 23
        Align = alTop
        Alignment = taCenter
        Anchors = [akLeft, akTop, akRight, akBottom]
        AutoSize = False
        Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080' '#1080#1079#1086#1073#1088#1072#1078#1077#1085#1080#1081
      end
      object images_folder_label: TLabel
        Left = 13
        Top = 38
        Width = 101
        Height = 13
        Caption = #1055#1072#1087#1082#1072' '#1080#1079#1086#1073#1088#1072#1078#1077#1085#1080#1081
      end
      object images_count_lbl: TLabel
        Left = 147
        Top = 76
        Width = 3
        Height = 13
      end
      object total_images_label: TLabel
        Left = 13
        Top = 76
        Width = 98
        Height = 13
        Caption = #1042#1089#1077#1075#1086' '#1080#1079#1086#1073#1088#1072#1078#1077#1085#1080#1081
      end
      object choose_folder_button: TButton
        Left = 323
        Top = 33
        Width = 75
        Height = 25
        Caption = #1042#1099#1073#1088#1072#1090#1100
        TabOrder = 0
        OnClick = choose_folder_buttonClick
      end
      object image_folder_edit: TEdit
        Left = 147
        Top = 35
        Width = 169
        Height = 21
        TabOrder = 1
        OnEnter = add_nick_editEnter
        OnKeyPress = add_nick_editKeyPress
      end
      object clear_images_button: TButton
        Left = 323
        Top = 71
        Width = 75
        Height = 25
        Caption = #1054#1095#1080#1089#1090#1080#1090#1100
        TabOrder = 2
        OnClick = clear_images_buttonClick
      end
    end
    object db_s_page: TPanel
      Left = 0
      Top = 0
      Width = 410
      Height = 371
      Align = alClient
      BevelOuter = bvNone
      Padding.Left = 10
      Padding.Top = 10
      Padding.Right = 10
      Padding.Bottom = 10
      TabOrder = 2
      object db_prf_lbl: TLabel
        Left = 10
        Top = 146
        Width = 100
        Height = 13
        Hint = #1055#1088#1080#1089#1090#1072#1074#1082#1072' '#1082' '#1082#1072#1078#1076#1086#1081' '#1090#1072#1073#1083#1080#1094#1077', '#1089#1086#1079#1076#1072#1074#1072#1077#1084#1086#1081' '#1087#1083#1072#1075#1080#1085#1086#1084' webchat'
        Caption = #1055#1088#1077#1092#1080#1082#1089' '#1090#1072#1073#1083#1080#1094' '#1041#1044
      end
      object db_pass_lbl: TLabel
        Left = 10
        Top = 119
        Width = 37
        Height = 13
        Caption = #1055#1072#1088#1086#1083#1100
      end
      object db_user_lbl: TLabel
        Left = 10
        Top = 92
        Width = 93
        Height = 13
        Caption = #1048#1084#1103' '#1087#1086#1083#1100#1079#1086#1074#1072#1090#1077#1083#1103
      end
      object db_name_lbl: TLabel
        Left = 10
        Top = 65
        Width = 65
        Height = 13
        Caption = #1053#1072#1079#1074#1072#1085#1080#1077' '#1041#1044
      end
      object db_host_lbl: TLabel
        Left = 10
        Top = 38
        Width = 63
        Height = 13
        Caption = #1040#1076#1088#1077#1089' '#1093#1086#1089#1090#1072
      end
      object db_main_lbl: TLabel
        Left = 10
        Top = 10
        Width = 178
        Height = 13
        Align = alTop
        Alignment = taCenter
        Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080' '#1089#1086#1077#1076#1080#1085#1077#1085#1080#1103' '#1089' '#1041#1044' MySQL'
      end
      object test_conn_but: TButton
        Left = 81
        Top = 170
        Width = 110
        Height = 25
        Hint = #1058#1077#1089#1090' '#1087#1086#1076#1082#1083#1102#1095#1077#1085#1080#1103' '#1082' '#1041#1044
        Caption = #1058#1077#1089#1090' '#1087#1086#1076#1082#1083#1102#1095#1077#1085#1080#1103
        TabOrder = 0
        OnClick = test_conn_butClick
      end
      object db_prf_edit: TEdit
        Left = 215
        Top = 143
        Width = 181
        Height = 21
        TabOrder = 1
      end
      object db_pass_edit: TMaskEdit
        Left = 215
        Top = 116
        Width = 181
        Height = 21
        AutoSize = False
        PasswordChar = '*'
        TabOrder = 2
      end
      object db_user_edit: TEdit
        Left = 215
        Top = 89
        Width = 181
        Height = 21
        TabOrder = 3
      end
      object db_name_edit: TEdit
        Left = 215
        Top = 62
        Width = 181
        Height = 21
        TabOrder = 4
      end
      object db_host_edit: TEdit
        Left = 215
        Top = 35
        Width = 181
        Height = 21
        ParentShowHint = False
        ShowHint = True
        TabOrder = 5
      end
      object cr_db_struct_btn: TButton
        Left = 197
        Top = 170
        Width = 142
        Height = 25
        Hint = #1058#1088#1077#1073#1091#1077#1090#1089#1103' '#1076#1083#1103' '#1089#1086#1079#1076#1072#1085#1080#1103' '#1073#1072#1079#1086#1074#1086#1081' '#1089#1090#1088#1091#1082#1090#1091#1088#1099' '#1041#1044
        Caption = #1055#1086#1089#1090#1088#1086#1080#1090#1100' '#1089#1090#1088#1091#1082#1090#1091#1088#1091' '#1041#1044
        ParentShowHint = False
        ShowHint = True
        TabOrder = 6
        OnClick = cr_db_struct_btnClick
      end
    end
    object Panel1: TPanel
      Left = 0
      Top = 371
      Width = 410
      Height = 36
      Align = alBottom
      BevelOuter = bvNone
      Caption = 'btns_panel'
      TabOrder = 4
      object ok_btn: TButton
        Left = 85
        Top = 2
        Width = 75
        Height = 25
        Caption = 'OK'
        TabOrder = 0
        OnClick = ok_btnClick
      end
      object cancel_btn: TButton
        Left = 166
        Top = 2
        Width = 75
        Height = 25
        Caption = #1054#1090#1084#1077#1085#1072
        TabOrder = 1
        OnClick = cancel_btnClick
      end
      object save_btn: TButton
        Left = 247
        Top = 2
        Width = 75
        Height = 25
        Caption = #1055#1088#1080#1084#1077#1085#1080#1090#1100
        TabOrder = 2
        OnClick = save_btnClick
      end
    end
  end
  object settings_list: TListBox
    Left = 0
    Top = 0
    Width = 129
    Height = 407
    Align = alLeft
    ExtendedSelect = False
    ItemHeight = 13
    Items.Strings = (
      #1057#1086#1077#1076#1080#1085#1077#1085#1080#1077' '#1089' '#1041#1044
      #1048#1079#1086#1073#1088#1072#1078#1077#1085#1080#1103
      #1044#1086#1087#1086#1083#1085#1080#1090#1077#1083#1100#1085#1086)
    TabOrder = 2
    OnClick = settings_listClick
  end
end
