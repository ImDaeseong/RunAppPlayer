#include "StdAfx.h"
#include "InstallUtil.h"

InstallUtil::InstallUtil(void)
{
	strPackageFolder = GetModulePath();
}

InstallUtil::~InstallUtil(void)
{
}

void InstallUtil::Setup()
{	

	//프로그램 설치 유무 확인
	CString strAppExe;
	strAppExe.Format("%s\\BlueStacks\\Client\\Bluestacks.exe", GetCommonAppDataPath());
	//strAppExe.Format("%s\\HD-RunApp.exe", IsSetupInfo("InstallDir"));
	if(!::PathFileExists(strAppExe))
	{	
		//설치 파일을 원도우 각 폴더에 복사
		CopyFiles();

		//레지스트리 정보 설정
		SetRegInfos();

		//서비스 등록 및 기타 설정 작업
		RunBatchfile();
	}

	//완료시 종료
	exit(-1);
}

CString InstallUtil::IsSetupInfo(LPCTSTR szKey)
{	
	CString value;

	REGSAM sam = KEY_READ | KEY_WRITE;
	if (IsWindows64Bit())
		sam |= KEY_WOW64_64KEY;
	else
		sam |= KEY_WOW64_32KEY;
	
	CRegKey regKey;
	
	if ( regKey.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\BlueStacks"), sam) == ERROR_SUCCESS )
	{
		ULONG len = 0;
		if (ERROR_SUCCESS == regKey.QueryStringValue(szKey, NULL, &len) )
		{
			regKey.QueryStringValue(szKey, value.GetBuffer(len), &len);
			value.ReleaseBuffer();		
		}	
	}
	regKey.Close();

	return value;


	/*
	char szBuffer [MAX_PATH + 1] = {0, };
	DWORD dwCount = 0;

	CRegKey regKey;

	CString strRegPath;
	strRegPath = _T("SOFTWARE\\BlueStacks");		

	if (regKey.Open(HKEY_LOCAL_MACHINE, strRegPath, KEY_READ) != ERROR_SUCCESS)
	{
		regKey.Close();
		return FALSE;
	}

	dwCount = REG_BUFF_SIZE + 1;
	
	if (ERROR_SUCCESS != regKey.QueryStringValue(szKey, szBuffer, &dwCount))
	{
		regKey.Close();
		return FALSE;
	}

	regKey.Close();

	return (CString)szBuffer;
	*/
}

void InstallUtil::CopyFiles()
{
	CString strZipPath, strUnZipPath; 

	//첫번째 zip 파일 압축해제 및 복사
	strZipPath.Format("%s\\BlueStacks-data.zip", strPackageFolder);
	strUnZipPath.Format("%s\\BlueStacks", GetCommonAppDataPath());

	HZIP hZip;
	ZIPENTRY zipEntry;
	ZIPENTRY zipItem;
	CString strOutputFile;
		
	hZip = OpenZip(strZipPath, "");
	if(hZip)
	{
		GetZipItem(hZip, -1, &zipEntry);
		for (int i = 0; i < zipEntry.index; i++)
		{			
			GetZipItem(hZip, i, &zipItem);
			strOutputFile.Format("%s\\%s", strUnZipPath,  CString(zipItem.name));
			UnzipItem(hZip, i, strOutputFile);
		}
		CloseZip(hZip);
	}

	//두번째 zip 파일 압축해제 및 복사
	strZipPath.Format("%s\\BlueStacks-run.zip", strPackageFolder);
	strUnZipPath.Format("%s\\BlueStacks", GetProgramFilesBlueStacksFolder());
	hZip = OpenZip(strZipPath, "");
	if(hZip)
	{
		GetZipItem(hZip, -1, &zipEntry);
		for (int i = 0; i < zipEntry.index; i++)
		{			
			GetZipItem(hZip, i, &zipItem);
			strOutputFile.Format("%s\\%s", strUnZipPath,  CString(zipItem.name));
			UnzipItem(hZip, i, strOutputFile);
		}
		CloseZip(hZip);
	}
}

void InstallUtil::SetRegInfos()
{	
	CString strData; 


	//---------------------------------------------------------------------SOFTWARE\\BlueStacks

    //key
	CreateRegKey(_T("SOFTWARE\\BlueStacks"));

	//ApiToken
	SetRegDateString(_T("SOFTWARE\\BlueStacks"), "ApiToken", "9d84fd8e-fcae-4c5b-921b-b1ea2f18d862");

	//ClientVersion
	SetRegDateString(_T("SOFTWARE\\BlueStacks"), "ClientVersion", "4.140.12.1002");

	//DataDir
	strData = "";
	strData.Format("%s\\BlueStacks\\Engine\\", GetCommonAppDataPath());
	SetRegDateString(_T("SOFTWARE\\BlueStacks"), "DataDir", strData);

	//InstallDir
	strData = "";
	strData.Format("%s\\BlueStacks\\", GetProgramFilesBlueStacksFolder());
	SetRegDateString(_T("SOFTWARE\\BlueStacks"), "InstallDir", strData);

	//IsUpgrade
	SetRegDateDword(_T("SOFTWARE\\BlueStacks"), "IsUpgrade", 0);

	//LogDir
	strData = "";
	strData.Format("%s\\BlueStacks\\Logs\\", GetCommonAppDataPath());
	SetRegDateString(_T("SOFTWARE\\BlueStacks"), "LogDir", strData);

	//USER_GUID
	SetRegDateString(_T("SOFTWARE\\BlueStacks"), "USER_GUID", "e49b3cd8-e35a-f81e-29b6-74af7701c04b");
	
	//UserDefinedDir
	strData = "";
	strData.Format("%s\\BlueStacks", GetCommonAppDataPath());
	SetRegDateString(_T("SOFTWARE\\BlueStacks"), "UserDefinedDir", strData);

	//Version
	SetRegDateString(_T("SOFTWARE\\BlueStacks"), "Version", "4.140.12.1002");

	//---------------------------------------------------------------------SOFTWARE\\BlueStacks
			

	
	//---------------------------------------------------------------------SOFTWARE\\BlueStacks\\BTV\\Filters
	CreateRegKey(_T("SOFTWARE\\BlueStacks\\BTV\\Filters"));
	//---------------------------------------------------------------------SOFTWARE\\BlueStacks\\BTV\\Filters



	//---------------------------------------------------------------------SOFTWARE\\BlueStacks\\Client

	//key
	CreateRegKey(_T("SOFTWARE\\BlueStacks\\Client"));

	//CampaignJson
	strData = "";
	strData += "{\"app_url\": \"\", ";
	strData += "\"app_name\": \"\ub77c\uc774\uc988 \uc624\ube0c \ud0b9\ub364\uc988\", ";
	strData += "\"incompatible\": \"false\", ";
    strData += "\"app_icon_url\": \"http://cloud.bluestacks.com/app/icon?pkg=com.lilithgames.rok.gpkr\", ";
    strData += "\"fle_tag\": \"blank\", ";
	strData += "\"campaign_name\": \"aw-ded-AndroidonPC-ko-kr-s-1\", ";
    strData += "\"exit_utm_campaign\": \"homepage-dl-button-ko\", ";
	strData += "\"app_pkg\": \"com.lilithgames.rok.gpkr\", ";
	strData += "\"promoter_id\": \"\"}";	
	SetRegDateString(_T("SOFTWARE\\BlueStacks\\Client"), "CampaignJson", "");//SetRegDateString(_T("SOFTWARE\\BlueStacks\\Client"), "CampaignJson", strData);

	//CampaignMD5
	SetRegDateString(_T("SOFTWARE\\BlueStacks\\Client"), "CampaignMD5", "3de10261deee36e3c40ab42c7552c55d");

	//CefDataPath
	strData = "";
	strData.Format("%s\\BlueStacks\\CefData", GetCommonAppDataPath());
	SetRegDateString(_T("SOFTWARE\\BlueStacks\\Client"), "CefDataPath", strData);

	//ClientInstallDir
	strData = "";
	strData.Format("%s\\BlueStacks\\Client", GetCommonAppDataPath());
	SetRegDateString(_T("SOFTWARE\\BlueStacks\\Client"), "ClientInstallDir", strData);

	//DeviceProfileFromCloud
	SetRegDateString(_T("SOFTWARE\\BlueStacks\\Client"), "DeviceProfileFromCloud", "ptxg");

	//EngineDataDir
	strData = "";
	strData.Format("%s\\BlueStacks\\Engine", GetCommonAppDataPath());
	SetRegDateString(_T("SOFTWARE\\BlueStacks\\Client"), "EngineDataDir", strData);

	//IsClientFirstLaunch
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Client"), "IsClientFirstLaunch", 1);

	//SetupFolder
	strData = "";
	strData.Format("%s\\BlueStacksSetup", GetAppLocalDataPath());
	SetRegDateString(_T("SOFTWARE\\BlueStacks\\Client"), "SetupFolder", strData);

	//ShowKeyControlsOverlay
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Client"), "ShowKeyControlsOverlay", 1);

	//UserSelectedLocale
	SetRegDateString(_T("SOFTWARE\\BlueStacks\\Client"), "UserSelectedLocale", "ko-KR");

	//---------------------------------------------------------------------SOFTWARE\\BlueStacks\\Client



	//---------------------------------------------------------------------SOFTWARE\\BlueStacks\\Config

	//key
	CreateRegKey(_T("SOFTWARE\\BlueStacks\\Config"));

	//AgentServerPort
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Config"), "AgentServerPort", 2861);
	
	//CampaignName
	SetRegDateString(_T("SOFTWARE\\BlueStacks\\Config"), "CampaignName", "aw-ded-AndroidonPC-ko-kr-s-1");

	//CurrentEngine
	SetRegDateString(_T("SOFTWARE\\BlueStacks\\Config"), "CurrentEngine", "plus");
	
	//DefaultShortcuts
	strData = "";
	strData += "{";
	strData += "\"Shortcut\": [{";
	strData += "\"ShortcutCategory\": \"STRING_WINDOW_ACTIONS\",";
	strData += "\"ShortcutName\": \"STRING_BOSSKEY_SETTING\",";
	strData += "\"ShortcutKey\": \"Ctrl + Shift + X\"";
	strData += "},";
	strData += "{";
	strData += "\"ShortcutCategory\": \"STRING_WINDOW_ACTIONS\",";
	strData += "\"ShortcutName\": \"STRING_UPDATED_FULLSCREEN_BUTTON_TOOLTIP\",";
	strData += "\"ShortcutKey\": \"F11\"";
	strData += "},";
	strData += "{";
	strData += "\"ShortcutCategory\": \"STRING_WINDOW_ACTIONS\",";
	strData += "\"ShortcutName\": \"STRING_SETTINGS\",";
	strData += "\"ShortcutKey\": \"Ctrl + Shift + I\"";
	strData += "},";
	strData += "{";
	strData += "\"ShortcutCategory\": \"STRING_SYSTEM_ACTIONS\",";
	strData += "\"ShortcutName\": \"STRING_INCREASE_VOLUME\",";
	strData += "\"ShortcutKey\": \"Ctrl + Shift + Up\"";
	strData += "},";
	strData += "{";
	strData += "\"ShortcutCategory\": \"STRING_SYSTEM_ACTIONS\",";
	strData += "\"ShortcutName\": \"STRING_DECREASE_VOLUME\",";
	strData += "\"ShortcutKey\": \"Ctrl + Shift + Down\"";
	strData += "},";
	strData += "{";
	strData += "\"ShortcutCategory\": \"STRING_SYSTEM_ACTIONS\",";
	strData += "\"ShortcutName\": \"STRING_TOGGLE_MUTE_STATE\",";
	strData += "\"ShortcutKey\": \"Ctrl + Shift + M\"";
	strData += "},";
	strData += "{";
	strData += "\"ShortcutCategory\": \"STRING_SYSTEM_ACTIONS\",";
	strData += "\"ShortcutName\": \"STRING_TOOLBAR_CAMERA\",";
	strData += "\"ShortcutKey\": \"Ctrl + Shift + S\"";
	strData += "},";
	strData += "{";
	strData += "\"ShortcutCategory\": \"STRING_SYSTEM_ACTIONS\",";
	strData += "\"ShortcutName\": \"STRING_RECORD_SCREEN\",";
	strData += "\"ShortcutKey\": \"Ctrl + Shift + R\"";
	strData += "},";
	strData += "{";
	strData += "\"ShortcutCategory\": \"STRING_ANDROID_ACTIONS\",";
	strData += "\"ShortcutName\": \"STRING_HOME\",";
	strData += "\"ShortcutKey\": \"Ctrl + Shift + 1\"";
	strData += "},";
	strData += "{";
	strData += "\"ShortcutCategory\": \"STRING_ANDROID_ACTIONS\",";
	strData += "\"ShortcutName\": \"STRING_BACK\",";
	strData += "\"ShortcutKey\": \"Ctrl + Shift + 2\"";
	strData += "},";
	strData += "{";
	strData += "\"ShortcutCategory\": \"STRING_ANDROID_ACTIONS\",";
	strData += "\"ShortcutName\": \"STRING_SHAKE\",";
	strData += "\"ShortcutKey\": \"Ctrl + Shift + 3\"";
	strData += "},";
	strData += "{";
	strData += "\"ShortcutCategory\": \"STRING_ANDROID_ACTIONS\",";
	strData += "\"ShortcutName\": \"STRING_ROTATE\",";
	strData += "\"ShortcutKey\": \"Ctrl + Shift + 4\"";
	strData += "},";
	strData += "{";
	strData += "\"ShortcutCategory\": \"STRING_GAME_CONTROLS\",";
	strData += "\"ShortcutName\": \"STRING_TOGGLE_KEYMAPPING_STATE\",";
	strData += "\"ShortcutKey\": \"Ctrl + Shift + F10\"";
	strData += "},";
	strData += "{";
	strData += "\"ShortcutCategory\": \"STRING_GAME_CONTROLS\",";
	strData += "\"ShortcutName\": \"STRING_TOGGLE_KEYMAP_WINDOW\",";
	strData += "\"ShortcutKey\": \"Ctrl + Shift + H\"";
	strData += "},";
	strData += "{";
	strData += "\"ShortcutCategory\": \"STRING_GAME_CONTROLS\",";
	strData += "\"ShortcutName\": \"STRING_TOGGLE_FARM_MODE\",";
	strData += "\"ShortcutKey\": \"Ctrl + Shift + F\"";
	strData += "},";
	strData += "{";
	strData += "\"ShortcutCategory\": \"STRING_GAME_CONTROLS\",";
	strData += "\"ShortcutName\": \"STRING_TOGGLE_OVERLAY\",";
	strData += "\"ShortcutKey\": \"Ctrl + Shift + F6\"";
	strData += "},";
	strData += "{";
	strData += "\"ShortcutCategory\": \"STRING_GAME_CONTROLS\",";
	strData += "\"ShortcutName\": \"STRING_TOGGLE_LOCK_CURSOR\",";
	strData += "\"ShortcutKey\": \"Ctrl + Shift + F8\"";
	strData += "},";
	strData += "{";
	strData += "\"ShortcutCategory\": \"STRING_GAME_CONTROLS\",";
	strData += "\"ShortcutName\": \"STRING_GAMEPAD_CONTROLS\",";
	strData += "\"ShortcutKey\": \"Ctrl + Shift + G\"";
	strData += "},";
	strData += "{";
	strData += "\"ShortcutCategory\": \"STRING_MISCELLANEOUS\",";
	strData += "\"ShortcutName\": \"STRING_OPEN_MEDIA_FOLDER\",";
	strData += "\"ShortcutKey\": \"Ctrl + Shift + 6\"";
	strData += "},";
	strData += "{";
	strData += "\"ShortcutCategory\": \"STRING_MISCELLANEOUS\",";
	strData += "\"ShortcutName\": \"STRING_MACRO_RECORDER\",";
	strData += "\"ShortcutKey\": \"Ctrl + Shift + 7\"";
	strData += "},";
	strData += "{";
	strData += "\"ShortcutCategory\": \"STRING_MISCELLANEOUS\",";
	strData += "\"ShortcutName\": \"STRING_TOGGLE_MULTIINSTANCE_WINDOW\",";
	strData += "\"ShortcutKey\": \"Ctrl + Shift + 8\"";
	strData += "},";
	strData += "{";
	strData += "\"ShortcutCategory\": \"STRING_MISCELLANEOUS\",";
	strData += "\"ShortcutName\": \"STRING_SYNCHRONISER\",";
	strData += "\"ShortcutKey\": \"Ctrl + Shift + 9\"";
	strData += "},";
	strData += "{";
	strData += "\"ShortcutCategory\": \"STRING_MISCELLANEOUS\",";
	strData += "\"ShortcutName\": \"STRING_SET_LOCATION\",";
	strData += "\"ShortcutKey\": \"Ctrl + Shift + K\"";
	strData += "},";
	strData += "{";
	strData += "\"ShortcutCategory\": \"STRING_MISCELLANEOUS\",";
	strData += "\"ShortcutName\": \"STRING_TRANSLATOR_TOOL\",";
	strData += "\"ShortcutKey\": \"Ctrl + Shift + L\"";
	strData += "}";
	strData += "],";
	strData += "\"DefaultModifier\": \"LeftCtrl,LeftShift\"";
    strData += "}";
	SetRegDateString(_T("SOFTWARE\\BlueStacks\\Config"), "DefaultShortcuts", strData);

	//DeviceCaps
	strData = "";
	strData += "{\"engine_enabled\":\"plus\",";
	strData += "\"legacy_reason\":\"\",";
	strData += "\"cpu_hvm\":\"True\",";
	strData += "\"bios_hvm\":\"True\"}";
	SetRegDateString(_T("SOFTWARE\\BlueStacks\\Config"), "DeviceCaps", strData);

	//EnginePreference
	SetRegDateString(_T("SOFTWARE\\BlueStacks\\Config"), "EnginePreference", "plus");

	//Features
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Config"), "Features", 1211955071);

	//FeaturesHigh
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Config"), "FeaturesHigh", 2147483637);

	//ForceDedicatedGPU
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Config"), "ForceDedicatedGPU", 1);

	//GlLegacyTransportConfig
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Config"), "GlLegacyTransportConfig", 0);

	//GlPlusTransportConfig
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Config"), "GlPlusTransportConfig", 3);

	//InstallationType
	SetRegDateString(_T("SOFTWARE\\BlueStacks\\Config"), "InstallationType", "FullEdition");

	//InstallID
	SetRegDateString(_T("SOFTWARE\\BlueStacks\\Config"), "InstallID", "e3511801-d5a5-4c4d-ad94-22f85af0fb2b");

	//Oem
	SetRegDateString(_T("SOFTWARE\\BlueStacks\\Config"), "Oem", "bgp");

	//PartnerExePath
	strData = "";
	strData.Format("%s\\BlueStacks\\Client\\BlueStacks.exe", GetCommonAppDataPath());
	SetRegDateString(_T("SOFTWARE\\BlueStacks\\Config"), "PartnerExePath", strData);

	//ProposedGUID
	SetRegDateString(_T("SOFTWARE\\BlueStacks\\Config"), "ProposedGUID", "a59a7cdc-a790-6571-7557-db2e86e9cef1");

	//SystemInfoStats2
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Config"), "SystemInfoStats2", 1);

	//SystemStats
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Config"), "SystemStats", 1);

	//UpdateBstConfig
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Config"), "UpdateBstConfig", 0);

	//VmList
	strData = "";
	strData = "Android\r\n";
	SetRegDateMultiString(_T("SOFTWARE\\BlueStacks\\Config"), "VmList", strData);

	//---------------------------------------------------------------------SOFTWARE\\BlueStacks\\Config

	


	//---------------------------------------------------------------------SOFTWARE\\BlueStacks\\Guests\\Android
	
	//key
	CreateRegKey(_T("SOFTWARE\\BlueStacks\\Guests\\Android"));
	
	//BootParameters
	SetRegDateString(_T("SOFTWARE\\BlueStacks\\Guests\\Android"), "BootParameters", "ROOT=/dev/sda1 SRC=/android DATA=/dev/sdb1 PREBUNDLEDAPPSFS=/dev/sdc1 HOST=WIN bstandroidport=9999 GlMode=1 VERSION=4.140.12.1002 OEM=bgp LANG=ko-KR country=KR caCode=410 pcode=ptxg OEMFEATURES=538983425 DNS2=8.8.8.8 GUID=e49b3cd8-e35a-f81e-29b6-74af7701c04b EngineState=plus caSelector=se_45008 DPI=240 GlTransport=3 appsfeatures=16592382 installId=e3511801-d5a5-4c4d-ad94-22f85af0fb2b machineId=0a5c77ee-a5e4-4634-941a-23e2dd713476 versionMachineId=291e65d3-ac4e-42ea-b06d-9080c2b5071c ApiToken=9d84fd8e-fcae-4c5b-921b-b1ea2f18d862 ssse3=1");
         
	//DisableRobustness
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Guests\\Android"), "DisableRobustness", 1);

	//Memory
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Guests\\Android"), "Memory", 2048);
	
	//VirtType
	SetRegDateString(_T("SOFTWARE\\BlueStacks\\Guests\\Android"), "VirtType", "legacy");
	
	//---------------------------------------------------------------------SOFTWARE\\BlueStacks\\Guests\\Android

	
	

	//---------------------------------------------------------------------SOFTWARE\\BlueStacks\\Guests\\Android\\BlockDevice\\0
	
	//key
	CreateRegKey(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\BlockDevice\\0"));
	
	//Name
	SetRegDateString(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\BlockDevice\\0"), "Name", "sda1");

	//Path
	strData = "";
	strData.Format("%s\\BlueStacks\\Engine\\Android\\Root.vdi", GetCommonAppDataPath());
	SetRegDateString(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\BlockDevice\\0"), "Path", strData);
	//---------------------------------------------------------------------SOFTWARE\\BlueStacks\\Guests\\Android\\BlockDevice\\0



	//---------------------------------------------------------------------SOFTWARE\\BlueStacks\\Guests\\Android\\BlockDevice\\1
	
	//key
	CreateRegKey(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\BlockDevice\\1"));
	
	//Name
	SetRegDateString(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\BlockDevice\\1"), "Name", "sdb1");

	//Path
	strData = "";
	strData.Format("%s\\BlueStacks\\Engine\\Android\\Data.vdi", GetCommonAppDataPath());
	SetRegDateString(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\BlockDevice\\1"), "Path", strData);
	//---------------------------------------------------------------------SOFTWARE\\BlueStacks\\Guests\\Android\\BlockDevice\\1



	//---------------------------------------------------------------------SOFTWARE\\BlueStacks\\Guests\\Android\\BlockDevice\\2
	
	//key
	CreateRegKey(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\BlockDevice\\2"));
	
	//Name
	SetRegDateString(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\BlockDevice\\2"), "Name", "sdc1");

	//Path
	strData = "";
	strData.Format("%s\\BlueStacks\\Engine\\Android\\SDCard.vdi", GetCommonAppDataPath());
	SetRegDateString(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\BlockDevice\\2"), "Path", strData);
	//---------------------------------------------------------------------SOFTWARE\\BlueStacks\\Guests\\Android\\BlockDevice\\2



	//---------------------------------------------------------------------SOFTWARE\\BlueStacks\\Guests\\Android\\BlockDevice\\3
	
	//key
	CreateRegKey(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\BlockDevice\\3"));
	
	//Name
	SetRegDateString(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\BlockDevice\\3"), "Name", "sdd1");

	//Path
	strData = "";
	strData.Format("%s\\BlueStacks\\Engine\\Android\\Prebundled.vdi", GetCommonAppDataPath());
	SetRegDateString(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\BlockDevice\\3"), "Path", strData);
	//---------------------------------------------------------------------SOFTWARE\\BlueStacks\\Guests\\Android\\BlockDevice\\3



	//---------------------------------------------------------------------SOFTWARE\\BlueStacks\\Guests\\Android\\BlockDevice\\4
	CreateRegKey(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\BlockDevice\\4"));
	//---------------------------------------------------------------------SOFTWARE\\BlueStacks\\Guests\\Android\\BlockDevice\\4

	

	//---------------------------------------------------------------------SOFTWARE\\BlueStacks\\Guests\\Android\\Config
	
	//key
	CreateRegKey(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\Config"));

	//BstAndroidPort
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\Config"), "BstAndroidPort", 9999);

	//Camera
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\Config"), "Camera", 1);

	//ConfigSynced
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\Config"), "ConfigSynced", 0);

	//DisableDWM
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\Config"), "DisableDWM", 0);

	//DisablePcIme
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\Config"), "DisablePcIme", 0);

	//EnableBSTVC
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\Config"), "EnableBSTVC", 0);

	//FenceSyncType
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\Config"), "FenceSyncType", 1);

	//FileSystem
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\Config"), "FileSystem", 1);

	//ForceVMLegacyMode
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\Config"), "ForceVMLegacyMode", 1);

	//FrontendNoClose
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\Config"), "FrontendNoClose", 0);

	//FrontendServerPort
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\Config"), "FrontendServerPort", 2881);

	//GlMode
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\Config"), "GlMode", 1);

	//GlPort
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\Config"), "GlPort", 3901);

	//GlRenderMode
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\Config"), "GlRenderMode", 1);

	//GpsLatitude
	SetRegDateString(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\Config"), "GpsLatitude", "");

	//GpsLongitude
	SetRegDateString(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\Config"), "GpsLongitude", "");

	//GpsMode
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\Config"), "GpsMode", 1);

	//GpsSource
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\Config"), "GpsSource", 0);

	//GrabKeyboard
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\Config"), "GrabKeyboard", 0);

	//HostSensorPort
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\Config"), "HostSensorPort", 3866);

	//HScroll
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\Config"), "HScroll", 1);
	
	//IsGoogleSigninDone
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\Config"), "IsGoogleSigninDone", 0);

	//SmartControlsEnabledApps
	strData = "";
	strData = "com.tencent.tmgp.kr.codm\r\ncom.garena.game.codm\r\ncom.activision.callofduty.shooter\r\ncom.dts.freefireth\r\n";
	SetRegDateMultiString(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\Config"), "SmartControlsEnabledApps", strData);

	//SoftControlBarHeightLandscape
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\Config"), "SoftControlBarHeightLandscape", 0);

	//SoftControlBarHeightPortrait
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\Config"), "SoftControlBarHeightPortrait", 0);

	//StopZygoteOnClose
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\Config"), "StopZygoteOnClose", 0);

	//TriggerMemoryTrimThreshold
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\Config"), "TriggerMemoryTrimThreshold", 700);

	//TriggerMemoryTrimTimerInterval
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\Config"), "TriggerMemoryTrimTimerInterval", 60000);

	//VCPUs
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\Config"), "VCPUs", 2);

	//---------------------------------------------------------------------SOFTWARE\\BlueStacks\\Guests\\Android\\Config

	


	//---------------------------------------------------------------------SOFTWARE\\BlueStacks\\Guests\\Android\\FrameBuffer\\0
	
	//key
	CreateRegKey(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\FrameBuffer\\0"));
	
	//Depth
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\FrameBuffer\\0"), "Depth", 16);

	//HideBootProgress
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\FrameBuffer\\0"), "HideBootProgress", 1);

	//WindowWidth
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\FrameBuffer\\0"), "WindowWidth", 1504);

	//WindowHeight
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\FrameBuffer\\0"), "WindowHeight", 846);

	//GuestWidth
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\FrameBuffer\\0"), "GuestWidth", 1600);

	//GuestHeight
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\FrameBuffer\\0"), "GuestHeight", 900);	
	
	//---------------------------------------------------------------------SOFTWARE\\BlueStacks\\Guests\\Android\\FrameBuffer\\0



	//---------------------------------------------------------------------SOFTWARE\\BlueStacks\\Guests\\Android\\Network\\0
	
	//key
	CreateRegKey(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\Network\\0"));

	//InboundRules
	strData = "";
	strData = "tcp:5555:5555\r\ntcp:6666:6666\r\ntcp:7777:7777\r\ntcp:9999:9999\r\nudp:12000:12000\r\n";
	SetRegDateMultiString(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\Network\\0"), "InboundRules", strData);
     //---------------------------------------------------------------------SOFTWARE\\BlueStacks\\Guests\\Android\\Network\\0



	//---------------------------------------------------------------------SOFTWARE\\BlueStacks\\Guests\\Android\\Network\\Redirect
	
	//key
	CreateRegKey(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\Network\\Redirect"));
	
	//tcp/5555
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\Network\\Redirect"), "tcp/5555", 5555);

	//tcp/6666
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\Network\\Redirect"), "tcp/6666", 6666);

	//tcp/7777
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\Network\\Redirect"), "tcp/7777", 7777);

	//tcp/9999
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\Network\\Redirect"), "tcp/9999", 9999);

	//udp/12000
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\Network\\Redirect"), "udp/12000", 12000);
	//---------------------------------------------------------------------SOFTWARE\\BlueStacks\\Guests\\Android\\Network\\Redirect



	//---------------------------------------------------------------------SOFTWARE\\BlueStacks\\Guests\\Android\\SharedFolder\\0
	
	//key
	CreateRegKey(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\SharedFolder\\0"));
	
	//Name
	SetRegDateString(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\SharedFolder\\0"), "Name", "BstSharedFolder");

	//Path
	strData = "";
	strData.Format("%s\\BlueStacks\\Engine\\UserData\\SharedFolder\\", GetCommonAppDataPath());
	SetRegDateString(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\SharedFolder\\0"), "Path", strData);

	//Writable
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\SharedFolder\\0"), "Writable", 1);
	//---------------------------------------------------------------------SOFTWARE\\BlueStacks\\Guests\\Android\\SharedFolder\\0



	//---------------------------------------------------------------------SOFTWARE\\BlueStacks\\Guests\\Android\\SharedFolder\\1
	
	//key
	CreateRegKey(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\SharedFolder\\1"));
	
	//Name
	SetRegDateString(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\SharedFolder\\1"), "Name", "Pictures");

	//Path
	strData = "";
	strData.Format("%s", GetMyPicturePath());
	SetRegDateString(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\SharedFolder\\1"), "Path", strData);

	//Writable
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\SharedFolder\\1"), "Writable", 1);
	//---------------------------------------------------------------------SOFTWARE\\BlueStacks\\Guests\\Android\\SharedFolder\\1



	//---------------------------------------------------------------------SOFTWARE\\BlueStacks\\Guests\\Android\\SharedFolder\\2
	
	//key
	CreateRegKey(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\SharedFolder\\2"));
	
	//Name
	SetRegDateString(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\SharedFolder\\2"), "Name", "PublicPictures");

	//Path
	strData = "";
	strData.Format("%s", GetCommonPicturePath());
	SetRegDateString(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\SharedFolder\\2"), "Path", strData);

	//Writable
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\SharedFolder\\2"), "Writable", 1);
	//---------------------------------------------------------------------SOFTWARE\\BlueStacks\\Guests\\Android\\SharedFolder\\2



	//---------------------------------------------------------------------SOFTWARE\\BlueStacks\\Guests\\Android\\SharedFolder\\3
	
	//key
	CreateRegKey(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\SharedFolder\\3"));

	//Name
	SetRegDateString(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\SharedFolder\\3"), "Name", "Documents");

	//Path
	strData = "";
	strData.Format("%s", GetMyDocumentPath());
	SetRegDateString(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\SharedFolder\\3"), "Path", strData);

	//Writable
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\SharedFolder\\3"), "Writable", 1);
	//---------------------------------------------------------------------SOFTWARE\\BlueStacks\\Guests\\Android\\SharedFolder\\3



	//---------------------------------------------------------------------SOFTWARE\\BlueStacks\\Guests\\Android\\SharedFolder\\4
	
	//key
	CreateRegKey(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\SharedFolder\\4"));
	
	//Name
	SetRegDateString(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\SharedFolder\\4"), "Name", "PublicDocuments");

	//Path
	strData = "";
	strData.Format("%s", GetCommonDocumentPath());
	SetRegDateString(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\SharedFolder\\4"), "Path", strData);

	//Writable
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\SharedFolder\\4"), "Writable", 1);
	//---------------------------------------------------------------------SOFTWARE\\BlueStacks\\Guests\\Android\\SharedFolder\\4



	//---------------------------------------------------------------------SOFTWARE\\BlueStacks\\Guests\\Android\\SharedFolder\\5
	
	//key
	CreateRegKey(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\SharedFolder\\5"));
	
	//Name
	SetRegDateString(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\SharedFolder\\5"), "Name", "InputMapper");

	//Path
	strData = "";
	strData.Format("%s\\BlueStacks\\Engine\\UserData\\InputMapper", GetCommonAppDataPath());
	SetRegDateString(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\SharedFolder\\5"), "Path", strData);

	//Writable
	SetRegDateDword(_T("SOFTWARE\\BlueStacks\\Guests\\Android\\SharedFolder\\5"), "Writable", 1);
	//---------------------------------------------------------------------SOFTWARE\\BlueStacks\\Guests\\Android\\SharedFolder\\5



	//---------------------------------------------------------------------SOFTWARE\\BlueStacks\\Monitors
	
	//key
	CreateRegKey(_T("SOFTWARE\\BlueStacks\\Monitors"));
	
    //---------------------------------------------------------------------SOFTWARE\\BlueStacks\\Monitors



	//---------------------------------------------------------------------SOFTWARE\\BlueStacks\\User
	//key
	CreateRegKey(_T("SOFTWARE\\BlueStacks\\User"));
	//---------------------------------------------------------------------SOFTWARE\\BlueStacks\\User

}

BOOL InstallUtil::RunBatchfile()
{
	/*
	//StopService("bluestacksdrv");
	//UnInstallService("bluestacksdrv");

	//Install service
	CString binPath; 
	binPath.Format("%s\\BlueStacks\\BstkDrv.sys", GetProgramFilesBlueStacksFolder());
	InstallService("bluestacksdrv", "BlueStacks Hypervisor", binPath);

	//start service
	StartService("bluestacksdrv");
	*/


	CString strCommand;


	//REM Install the service
	CString binPath; 
	binPath.Format("%s\\BlueStacks\\BstkDrv.sys", GetProgramFilesBlueStacksFolder());

	strCommand += "REM Install the service\r\n";
	strCommand += "sc.exe create bluestacksdrv type=kernel start=demand error=normal binPath=";
    strCommand += "\"" + binPath + "\" " + "DisplayName=""\"BlueStacks Hypervisor""\"  \r\n";
	strCommand += "IF ERRORLEVEL 1 (  \r\n";
	strCommand += "        ECHO Some error occured while installing the kernel driver  \r\n";
	strCommand += "        REM EXIT /b %ERRORLEVEL%  \r\n";
	strCommand += "        )  \r\n";


	//REM Create the firewall rule for our service
	CString programPath; 
	programPath.Format("%s\\BlueStacks\\HD-Player.exe", GetProgramFilesBlueStacksFolder());

	strCommand += "REM Create the firewall rule for our service\r\n";
	strCommand += "netsh.exe advfirewall firewall add rule name=\"BlueStacks Service\"";
	strCommand += " dir=in action=allow program=\"" + programPath +  "\" " + "enable=yes";


	//REM Give ACL to our HTTP ports
	strCommand += "REM Give ACL to our HTTP ports\r\n";
    strCommand += "FOR /L %%G IN (2861,1,2910) DO (\r\n";
    strCommand += "        REM netsh.exe http add urlacl url=http://*:%%G/ User=\"Everyone\r\n";
	strCommand += "        netsh.exe http add urlacl url=http://*:%%G/ User=Everyone\r\n";
    strCommand += "        )  \r\n";


	//REM COM registration
	CString UnRegRegComPath; 
	UnRegRegComPath.Format("%s\\BlueStacks\\HD-UnRegRegCom.exe", GetProgramFilesBlueStacksFolder());

	strCommand += "REM COM registration\r\n";
	strCommand += "\" " + UnRegRegComPath + "\" \r\n";


	//REM programdata permission 
	CString permissionPath; 
	permissionPath.Format("%s\\BlueStacks", GetProgramFilesBlueStacksFolder());

	strCommand += "REM programdata permission\r\n";
    strCommand += "icacls \" " + permissionPath + "\" " + "/grant Everyone:(OI)(CI)(F) /t \r\n";


	//REM registry key permission
	CString HKLMPath1, HKLMPath2; 
	HKLMPath1.Format("HKLM:\SOFTWARE\BlueStacks");
	HKLMPath2.Format("HKLM:\SOFTWARE\BlueStacks");

	strCommand += "REM registry key permission\r\n";
	strCommand += "powershell -Command \"& {$acl = Get-Acl  \"" + HKLMPath1 + "\"" + "; $sid = New-Object System.Security.Principal.SecurityIdentifier ([System.Security.Principal.WellKnownSidType]::WorldSid, $null); $rule = New-Object System.Security.AccessControl.RegistryAccessRule($sid, 'FullControl', 'ObjectInherit,ContainerInherit', 'None', 'Allow'); $acl.AddAccessRule($rule); Set-Acl \"" + HKLMPath2 + "\"" + " $acl;}\r\n";


	//REM permission to Users to interact with service
	strCommand += "REM permission to Users to interact with service\r\n";
	strCommand += "sc sdset bluestacksdrv D:(A;;CCDCLCSWRPWPDTLOCRSDRCWDWO;;;IU)(A;;CCLCSWLOCRRC;;;SU)(A;;CCLCSWRPWPDTLOCRRC;;;SY)(A;;CCDCLCSWRPWPDTLOCRSDRCWDWO;;;BA)\r\n";


	//write batch file
	CString strBatchFile;
	strBatchFile.Format("%s\\install.bat", GetModulePath());

	FILE *outFile;
	char outFileName[MAX_PATH + 1] = "";
	wsprintf(outFileName, "%s", strBatchFile);
	outFile = fopen(outFileName,"w+");
	if(outFile) 
	{
		fprintf(outFile,"%s\r\n", strCommand);
		fclose(outFile);	
	}

	//run batch file
	if (PathFileExists(strBatchFile))
	{
		char cmd32[MAX_PATH + 1] = {0};
		wsprintf(cmd32, "%s\\cmd.exe", GetSystemPath());

		CString csText;
		csText.Format("/c %s", strBatchFile);

		SHELLEXECUTEINFO SHInfo = {0};
		SHInfo.cbSize = sizeof (SHELLEXECUTEINFO);
		SHInfo.fMask = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_FLAG_NO_UI;
		SHInfo.lpVerb = _T ("open");
		SHInfo.lpFile = cmd32;
		SHInfo.lpParameters = _T (csText);
		SHInfo.hProcess = NULL;
		SHInfo.nShow = SW_HIDE;
		BOOL bExec = ShellExecuteEx (&SHInfo);
		if(bExec)
		{
			::WaitForSingleObject(SHInfo.hProcess, 5000);
		}
		return TRUE;
	}
	return FALSE;	
}

CString InstallUtil::GetProgramFilesBlueStacksFolder()
{
	CString strProgramFilesPath;
	strProgramFilesPath.Format("%s:\\Program Files", GetSystemPath().Left(1));
	return strProgramFilesPath;
}

CString InstallUtil::GetProgramFilesFolder()
{
	char szPath[MAX_PATH];
	::SHGetSpecialFolderPath(NULL, szPath, CSIDL_PROGRAM_FILES, FALSE);
	return (CString)szPath;
}

CString InstallUtil::GetCommonAppDataPath()
{
	char szPath[MAX_PATH];
	::SHGetSpecialFolderPath(NULL, szPath, CSIDL_COMMON_APPDATA, FALSE);
	return (CString)szPath;
}

CString InstallUtil::GetMyPicturePath()
{
	char szPath[MAX_PATH];
	::SHGetSpecialFolderPath(NULL, szPath, CSIDL_MYPICTURES, FALSE);
	return (CString)szPath;
}

CString InstallUtil::GetMyDocumentPath()
{
	char szPath[MAX_PATH];
	::SHGetSpecialFolderPath(NULL, szPath, CSIDL_MYDOCUMENTS, FALSE);
	return (CString)szPath;
}

CString InstallUtil::GetCommonPicturePath()
{
	char szPath[MAX_PATH];
	::SHGetSpecialFolderPath(NULL, szPath, CSIDL_COMMON_PICTURES, FALSE);
	return (CString)szPath;
}

CString InstallUtil::GetCommonDocumentPath()
{
	char szPath[MAX_PATH];
	::SHGetSpecialFolderPath(NULL, szPath, CSIDL_COMMON_DOCUMENTS, FALSE);
	return (CString)szPath;
}

CString InstallUtil::GetSystemPath()
{
	char szPath[MAX_PATH];
	::SHGetSpecialFolderPath(NULL, szPath, CSIDL_SYSTEM, FALSE);
	return (CString)szPath;
}

CString InstallUtil::GetAppDataPath()
{
	char szPath[MAX_PATH];
	::SHGetSpecialFolderPath(NULL, szPath, CSIDL_APPDATA, FALSE);
	return (CString)szPath;
}

CString InstallUtil::GetAppLocalDataPath()
{
	char szPath[MAX_PATH];
	::SHGetSpecialFolderPath(NULL, szPath, CSIDL_LOCAL_APPDATA, FALSE);
	return (CString)szPath;
}

CString InstallUtil::GetModulePath(LPCTSTR pszSubPath )
{
	CString szFullPath;
	char buf[MAX_PATH + 1];

	if(GetModuleFileName(NULL, buf, MAX_PATH + 1) > 0)
	{
		CString szTemp = buf;
		int pos = szTemp.ReverseFind('\\');
		if(pos >= 0)
		{
			szTemp = szTemp.Left(pos);
			if(pszSubPath != NULL && pszSubPath[0] != '\0')
			{
				if(pszSubPath[0] == '\\')
					szFullPath.Format("%s%s", szTemp, pszSubPath);
				else
					szFullPath.Format("%s\\%s", szTemp, pszSubPath);
			}
			else
				szFullPath = szTemp;
		}
		else
			szFullPath = szTemp;
	}

	return szFullPath;
}

BOOL InstallUtil::IsWindows64Bit()
{
	if (sizeof(int*) == 8) return TRUE;
	BOOL b = FALSE;
	IsWow64Process(GetCurrentProcess(), &b);
	return b;
}

void InstallUtil::SetRegDateMultiString(LPTSTR lpRegKey, LPCTSTR lpKey, CString strValue)
{	
	REGSAM sam = KEY_READ | KEY_WRITE;
	if (IsWindows64Bit())
		sam |= KEY_WOW64_64KEY;
	else
		sam |= KEY_WOW64_32KEY;
	
	/*
	TCHAR szValue[MAX_PATH];
	sprintf(szValue, "%s", strValue);

	CRegKey regKey;
	
	if ( regKey.Open(HKEY_LOCAL_MACHINE, lpRegKey, sam) == ERROR_SUCCESS )
	{
		if (ERROR_SUCCESS == regKey.SetMultiStringValue(lpKey, CT2A(szValue) ) )
		{
		}	
	}
	regKey.Close();
	*/

	HKEY hkey = NULL;	
	RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpRegKey, 0, sam, &hkey);
	RegSetValueEx(hkey, lpKey, 0, REG_MULTI_SZ, (BYTE*)strValue.GetBuffer(256), strValue.GetLength());	
	RegCloseKey(hkey);
}

void InstallUtil::SetRegDateString(LPTSTR lpRegKey, LPCTSTR lpKey, CString strValue)
{	
	REGSAM sam = KEY_READ | KEY_WRITE;
	if (IsWindows64Bit())
		sam |= KEY_WOW64_64KEY;
	else
		sam |= KEY_WOW64_32KEY;
	
	CRegKey regKey;
	
	if ( regKey.Open(HKEY_LOCAL_MACHINE, lpRegKey, sam) == ERROR_SUCCESS )
	{
		if (ERROR_SUCCESS == regKey.SetStringValue(lpKey, strValue, REG_SZ) )
		{
		}	
	}
	regKey.Close();

	/*
	CRegKey	regKey;	
	if (ERROR_SUCCESS != regKey.Open(HKEY_LOCAL_MACHINE, lpRegKey))
		regKey.Create(HKEY_LOCAL_MACHINE, lpRegKey);

	regKey.SetValue(strValue, lpKey);
	regKey.Close();
	*/
}

void InstallUtil::SetRegDateDword(LPCTSTR lpRegKey, LPCTSTR lpKey, int nValue)
{
	REGSAM sam = KEY_READ | KEY_WRITE;
	if (IsWindows64Bit())
		sam |= KEY_WOW64_64KEY;
	else
		sam |= KEY_WOW64_32KEY;
	
	CRegKey regKey;
	
	if ( regKey.Open(HKEY_LOCAL_MACHINE, lpRegKey, sam) == ERROR_SUCCESS )
	{
		if (ERROR_SUCCESS == regKey.SetDWORDValue(lpKey, nValue) )
		{
		}	
	}
	regKey.Close();

	/*
	CRegKey	regKey;	
	if (ERROR_SUCCESS != regKey.Open(HKEY_LOCAL_MACHINE, lpRegKey))
		regKey.Create(HKEY_LOCAL_MACHINE, lpRegKey);

	regKey.SetValue((DWORD)nValue, lpKey);
	regKey.Close();
	*/
}

void InstallUtil::CreateRegKey(LPCTSTR lpRegKey)
{
	REGSAM sam = KEY_READ | KEY_WRITE;
	if (IsWindows64Bit())
		sam |= KEY_WOW64_64KEY;
	else
		sam |= KEY_WOW64_32KEY;

	HKEY hKey;
    if( RegCreateKeyEx(HKEY_LOCAL_MACHINE, lpRegKey, 0, NULL, 0, sam, NULL, &hKey, NULL)  == ERROR_SUCCESS)
	{	
		RegCloseKey(hKey);
	}

	/*
	CRegKey regKey;
	if (regKey.Create(HKEY_LOCAL_MACHINE, lpRegKey) == ERROR_SUCCESS)
	{
		regKey.Close();
	}
	*/
}

BOOL InstallUtil::InstallService(CString strServiceName, CString strDisplayName, CString strServicePath)
{
	SC_HANDLE hServiceManager = NULL;
	SC_HANDLE hService = NULL;

	hServiceManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
	if(hServiceManager == NULL)
		return FALSE;
	
	hService = CreateService (hServiceManager, 
						   strServiceName,
						   strDisplayName,
						   SERVICE_ALL_ACCESS,
						   SERVICE_WIN32_OWN_PROCESS,
						   SERVICE_AUTO_START,
						   SERVICE_ERROR_IGNORE,
						   strServicePath,
						   NULL,
						   NULL,
						   NULL,
						   NULL,
						   NULL);
	if(hService == NULL)
	{
		CloseServiceHandle(hServiceManager);
		return FALSE;
	}

	CloseServiceHandle(hService);
	CloseServiceHandle(hServiceManager);

	return TRUE;
}

BOOL InstallUtil::UnInstallService(CString strServiceName)
{
	SERVICE_STATUS status;
	DWORD dwTickStart = 0;
	DWORD dwTickCurrent = 0;
	
	SC_HANDLE hServiceManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if(hServiceManager == NULL)
		return FALSE;
	
	SC_HANDLE hService = OpenService(hServiceManager, strServiceName, SERVICE_ALL_ACCESS);
	if(hService == NULL)
	{
		CloseServiceHandle(hServiceManager);
		return FALSE;
	}

	memset(&status, 0x00, sizeof(SERVICE_STATUS));

	BOOL bRes = QueryServiceStatus(hService, &status);
	if(bRes)
	{
		if(status.dwCurrentState != SERVICE_STOPPED)
			bRes = ControlService(hService, SERVICE_CONTROL_STOP, &status);
	}

	dwTickStart = GetTickCount();	
	while(bRes != FALSE && status.dwCurrentState != SERVICE_STOPPED)
	{		
		dwTickCurrent = GetTickCount();
		
		if(dwTickCurrent < dwTickStart) 
			dwTickStart = dwTickCurrent;
	
		if(dwTickCurrent - dwTickStart >= MAX_WAIT_TIME)
			break;

		Sleep(100);

		bRes = QueryServiceStatus(hService, &status);
	}

	bRes = DeleteService(hService);

	CloseServiceHandle(hService);
	CloseServiceHandle(hServiceManager);

	return bRes;
}

BOOL InstallUtil::StartService(CString strServiceName)
{
	SERVICE_STATUS status;
	DWORD dwTickStart = 0;
	DWORD dwTickCurrent = 0;
	
	SC_HANDLE hServiceManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if(hServiceManager == NULL)
		return FALSE;
	
	SC_HANDLE hService = OpenService(hServiceManager, strServiceName, SERVICE_ALL_ACCESS);
	if(hService == NULL)
	{
		CloseServiceHandle(hServiceManager);
		return FALSE;
	}

	memset(&status, 0x00, sizeof(SERVICE_STATUS));

	BOOL bRes = QueryServiceStatus(hService, &status);
	if(bRes)
	{
		if(status.dwCurrentState == SERVICE_STOPPED)
			bRes = ::StartService(hService, 0, NULL);
	}

	CloseServiceHandle(hService);
	CloseServiceHandle(hServiceManager);

	return bRes;
}

BOOL InstallUtil::StopService(CString strServiceName)
{
	SERVICE_STATUS status;
	DWORD dwTickStart = 0;
	DWORD dwTickCurrent = 0;
	
	SC_HANDLE hServiceManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if(hServiceManager == NULL)
		return FALSE;
	
	SC_HANDLE hService = OpenService(hServiceManager, strServiceName, SERVICE_ALL_ACCESS);
	if(hService == NULL)
	{
		CloseServiceHandle(hServiceManager);
		return FALSE;
	}

	memset(&status, 0x00, sizeof(SERVICE_STATUS));

	BOOL bRes = QueryServiceStatus(hService, &status);
	if(bRes)
	{
		if(status.dwCurrentState != SERVICE_STOPPED)
			bRes = ControlService(hService, SERVICE_CONTROL_STOP, &status);
	}

	dwTickStart = GetTickCount();	
	while(bRes != FALSE && status.dwCurrentState != SERVICE_STOPPED)
	{		
		dwTickCurrent = GetTickCount();
		
		if(dwTickCurrent < dwTickStart) 
			dwTickStart = dwTickCurrent;
	
		if(dwTickCurrent - dwTickStart >= MAX_WAIT_TIME)
			break;

		Sleep(100);

		bRes = QueryServiceStatus(hService, &status);
	}

	CloseServiceHandle(hService);
	CloseServiceHandle(hServiceManager);

	if(status.dwCurrentState != SERVICE_STOPPED)
		return FALSE;

	return TRUE;
}

BOOL InstallUtil::GetReourceFiles(CString strFileName, CString strPart, UINT nID)
{
	HINSTANCE hInst = AfxGetInstanceHandle();
	HRSRC hExeFile;
	hExeFile = ::FindResource(hInst, MAKEINTRESOURCE(nID), strPart);
	HGLOBAL hRes = ::LoadResource(hInst, hExeFile);
	DWORD dwSize = ::SizeofResource(hInst, hExeFile);
	if (hRes != NULL)
	{
		UINT FAR* lpnRes = (UINT FAR*)::LockResource(hRes);
		TRY
		{
			CFile file(strFileName, CFile::modeCreate|CFile::modeWrite);
			file.Write(lpnRes, dwSize);
			file.Flush();
		}
		CATCH(CFileException, e)
		{
		}
		END_CATCH
		::FreeResource(hRes);
		return TRUE;
	} 
	return FALSE;
}

void InstallUtil::SetRegInstall()
{
	CString strRegFile;
	strRegFile.Format("%s\\bluestacks.reg", strPackageFolder);	
	if(GetReourceFiles(strRegFile, "REG", IDR_REG))
	{
		if (PathFileExists(strRegFile))
		{
			CString strParameter;
			strParameter.Format("%s:\\",strRegFile.Left(1));

			CString strRegExe;
			strRegExe.Format("-s %s", strRegFile);

			::ShellExecute(NULL, _T("open"), (LPCTSTR)_T("regedit"), (LPCTSTR) strRegExe, (LPCTSTR)strParameter, SW_HIDE);
		}
	}
}

BOOL InstallUtil::GetSysWow64Dir(LPTSTR pszDir, int nLen)
{
	if (!IsWindows64Bit())
		return FALSE;

	HMODULE hMod = LoadLibrary(_T("Kernel32.dll"));
#ifdef _UNICODE
	FunGetSystemWow64Directory fGetSystemWow64Directory = (FunGetSystemWow64Directory)GetProcAddress(hMod, "GetSystemWow64DirectoryW");
#else
	FunGetSystemWow64Directory fGetSystemWow64Directory = (FunGetSystemWow64Directory)GetProcAddress(hMod, "GetSystemWow64DirectoryA");
#endif

	if (!fGetSystemWow64Directory) return FALSE;
	fGetSystemWow64Directory(pszDir, nLen);
	FreeLibrary(hMod);
	return TRUE;
}

BOOL InstallUtil::RunRegfile()
{
	CString strRegPath = "";
	CString strSysRegPath = "";


	//64bit 시스템 경로
	TCHAR szSysWow64[MAX_PATH] = { 0 };
	if(GetSysWow64Dir(szSysWow64, MAX_PATH))
	{
		strSysRegPath.Format("%s\\regedit.exe", szSysWow64);
	}

	//등록할 레지스트리 파일
	CString strRegFile;
	strRegFile.Format("%s\\bluestacks.reg", strPackageFolder);	
	if(GetReourceFiles(strRegFile, "REG", IDR_REG))
	{
		strRegPath.Format("-s %s", strRegFile);
	}

    //배치 명령어
	CString strCommand;
	strCommand += "REM reg info\r\n";
	strCommand += strSysRegPath + " " + strRegPath;
	strCommand += "\r\n";

	//배치 파일 생성
	CString strBatchFile;
	strBatchFile.Format("%s\\reg.bat", GetModulePath());

	FILE *outFile;
	char outFileName[MAX_PATH + 1] = "";
	wsprintf(outFileName, "%s", strBatchFile);
	outFile = fopen(outFileName,"w+");
	if(outFile) 
	{
		fprintf(outFile,"%s\r\n", strCommand);
		fclose(outFile);	
	}

	//배치 파일 실행
	if (PathFileExists(strBatchFile))
	{
		char cmd32[MAX_PATH + 1] = {0};
		wsprintf(cmd32, "%s\\cmd.exe", szSysWow64);

		CString csText;
		csText.Format("/c %s", strBatchFile);

		SHELLEXECUTEINFO SHInfo = {0};
		SHInfo.cbSize = sizeof (SHELLEXECUTEINFO);
		SHInfo.fMask = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_FLAG_NO_UI;
		SHInfo.lpVerb = _T ("open");
		SHInfo.lpFile = cmd32;
		SHInfo.lpParameters = _T (csText);
		SHInfo.hProcess = NULL;
		SHInfo.nShow = SW_HIDE;
		BOOL bExec = ShellExecuteEx (&SHInfo);
		if(bExec)
		{
			::WaitForSingleObject(SHInfo.hProcess, 5000);
		}
		return TRUE;
	}
	return FALSE;
}