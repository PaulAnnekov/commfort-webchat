#include <vcl.h>

//const
//  unsigned char PM_PLUGIN_SNDMSG_PUB          = 20, //plugin -> commfort: ������������ ��������� � �����: �����(�����)+�����(�����)+�����(���������) ������: 0 - ����� ����������� ������ 1 - ����� ����������� ���������� (F9)
//  PM_PLUGIN_SNDMSG_PRIV         = 21, //plugin -> commfort: ������������ ��������� � ������: �����(�����)+�����(��� ������������)+�����(���������) ������: 0 - ����� ����������� ������ 1 - ����� ����������� ���������� (F9)
//  PM_PLUGIN_SNDMSG_PM           = 22, //plugin -> commfort: ��������� ������ ���������: �����(��� ������������)+�����(���������)
//  PM_PLUGIN_THEME_CHANGE        = 23, //plugin -> commfort: �������� ���� ������: �����(�����)+�����(����� ����)
//  PM_PLUGIN_GREETING_CHANGE     = 24, //plugin -> commfort: �������� ����������� ������: �����(�����)+�����(����� �����������)
//  PM_PLUGIN_STATUS_CHANGE       = 25, //plugin -> commfort: �������� ���������: �����(����� ���������)
//  PM_PLUGIN_CHANNEL_JOIN        = 26, //plugin -> commfort: �������/������������ � ������ ������: �����(�����)+�����(���������)+�����(����� �����) ���������: 0 - ����� ������� � ������ ������� 1 - ����� �������� � ������ ������� ����� �����: 0 - ���� �������� ���� ������������� 1 - ���� �������� ������ �� ����������� ��������! ���� ������������ ����� ������������ ���������� �� ����� ��� � 16 ����� �������.
//  PM_PLUGIN_CHANNEL_LEAVE       = 27, //plugin -> commfort: �������� ����� �����: �����(�����)
//  PM_PLUGIN_RESTRICT_SET        = 28, //plugin -> commfort: �������� �����������: �����(��� �����������)+�����(��� �������������)+�����(��� �����������)+�����(����� � �������)+�����(������� ������, ���� IP-�����, ���� �������� IP-�������)+�����(�������� ������)+�����(������� �����������) ��� �����������: 0 - ��������� ������ � ����, 1 - ��������� ������ � ������, 2 - ��������� ���������� � ������, 3 - ��������� ���������� ��������, 4 - ��������� ���������� ����������. ��� �������������: 0 - ������� ������ 1 - IP-����� 2 - �������� IP-������� (������: 192.168.0.0-192.168.0.255) ��� �����������: 0 - ������� ����������� 1 - ��������� �����������
//  PM_PLUGIN_CHANNEL_DEL         = 29, //plugin -> commfort: ������� (�������) �����: �����(�����)
//  PM_PLUGIN_ANNOUNCMENT_ADD     = 30, //plugin -> commfort: ������������ ����������: �����(���� �������� � �������)+�����(����� ����������)
//  PM_PLUGIN_ACCOUNT_ADD         = 31, //plugin -> commfort: ���������������� ������� ������: �����(���)+�����(������)
//  PM_PLUGIN_ACCOUNT_DEL         = 32, //plugin -> commfort: ������� ������� ������: �����(���)
//  PM_PLUGIN_ACCOUNT_AGREE       = 33, //plugin -> commfort: ������� ��������� ������� ������: �����(���)
//  PM_PLUGIN_ACCOUNT_DISAGREE    = 34, //plugin -> commfort: ��������� ��������� ������� ������: �����(���)+�����(�������)
//  PM_PLUGIN_CHANNELS_GET        = 40, //plugin -> commfort: ��������� ������ �������
//  PM_PLUGIN_USERS_GET           = 41, //plugin -> commfort: ��������� ������ ������������ �������������
//  PM_PLUGIN_REGISTERED_GET      = 42, //plugin -> commfort: ��������� ������ ������������������ �������������
//  PM_PLUGIN_ACTIVATIONWAIT_GET  = 43, //plugin -> commfort: ��������� ������ �������������, ��������� ��������� �����������
//  PM_PLUGIN_MSG_PRIV            = 60, //commfort -> plugin: ��������� � ������: ������������()+�����(�����)+�����(���������) ������: 0 - ����� ����������� ������ 1 - ����� ����������� ���������� (F9)
//  PM_PLUGIN_MSG_PM              = 61, //commfort -> plugin: ������ ���������: ������������()+�����(���������)
//  PM_PLUGIN_JOIN_BOT            = 62, //commfort -> plugin: ����������� � ������ ����: �����(�����)+�����(����)+�����(�����������)
//  PM_PLUGIN_MSG_PUB             = 70, //commfort -> plugin: ���������� ��������� � �����: ������������()+�����(�����)+�����(�����)+�����(���������) ������: 0 - ����� ����������� ������ 1 - ����� ����������� ���������� (F9)
//  PM_PLUGIN_THEME_CHANGED       = 71, //commfort -> plugin: ����� ���� ������: ������������()+�����(�����)+�����(����� ����)
//  PM_PLUGIN_USER_JOINEDCHANNEL  = 72, //commfort -> plugin: ����������� � ������ ������� ������������: ������������()+�����(�����)
//  PM_PLUGIN_USER_LEAVEDCHANNEL  = 73, //commfort -> plugin: ����� �� ������ ������� ������������: ������������()+�����(�����)
//  PM_PLUGIN_NAME_CHANGED        = 75, //commfort -> plugin: ����� �����: ������������()+�����(����� ���)+�����(����� ������)
//  PM_PLUGIN_ICON_CHANGED        = 76, //commfort -> plugin: ����� ������: ������������()+�����(����� ����� ������)
//  PM_PLUGIN_STATUS_CHANGED      = 77, //commfort -> plugin: ����� ���������: ������������()+�����(����� ���������)
//  PM_PLUGIN_USER_JOINED         = 78, //commfort -> plugin: ������������ ������������� � ����: ������������()
//  PM_PLUGIN_USERLEAVED          = 79, //commfort -> plugin: ������������ ������� ���: ������������()
//  PM_PLUGIN_ANNOUNCMENT_ADDED   = 80, //commfort -> plugin: ���������� ����������: ������������()+�����(���� �������� � �������)+�����(����� ����������)
//  PM_PLUGIN_USER_REGISTERED     = 81, //commfort -> plugin: ����������� ������ ������������: �����(���)+�����(IP-�����)
//  PM_PLUGIN_ACTIVATION_REQUEST  = 82, //commfort -> plugin: ������ �� ��������� ����������� ������ ������������: �����(���)+�����(IP-�����)+�����(��������� ����������)
//  PM_PLUGIN_CHANNELS_LIST       = 90, //commfort -> plugin: ������ ������� (����� �� ������ � ID=40): �����(���������� �������)+(�����(�����)+�����(���������� ������������� � ������)+�����(���� ������))*���������� �������
//  PM_PLUGIN_USERS_LIST          = 91, //commfort -> plugin: ������ ������������ ������������� (����� �� ������ � ID=41): �����(���������� ������������ �������������)+������������()*���������� ������������ �������������
//  PM_PLUGIN_REGISTERED_LIST     = 92, //commfort -> plugin: ������ ������������������ ������������� (����� �� ������ � ID=42): �����(���������� ������������������ �������������)+(�����(���)+�����(IP-�����))*���������� ������������������ �������������
//  PM_PLUGIN_ACTIVATIONWAIT_LIST = 93, //commfort -> plugin: ������ ������������� ��������� ��������� ����������� (����� �� ������ � ID=43): �����(���������� ������������� ��������� ��������� �����������)+(�����(���)+�����(IP-�����)+�����(���� � ����� ������ ������ �� ���������)+�����(��������� ����������))*���������� ������������� ��������� ��������� �����������
//
//  PRE_PLUGIN_MSG                = 0,  //commfort -> plugin: ������������ ���������
//  PRE_PLUGIN_THEME              = 1,  //commfort -> plugin: ������������ ���
//  PRE_PLUGIN_ANNOUNCMENT        = 2;  //commfort -> plugin: ������������ ����������
//
//
//  String table_users_name       = "cf_users",
//  table_messages_name           = "cf_messages",
//
//  PLU_NAME     = "Commfort WebChat by...",
//  PLU_VER      = "0.0.0.1";
//
//	AnsiString PLU_DESC, BOT_NAME, BOT_PASS;
//	boolean BOT_ISFEMALE;
//	String  config_ini_path, log_file_path;
//	int keep_conn = 0, remaining_time, elapsed_time;
