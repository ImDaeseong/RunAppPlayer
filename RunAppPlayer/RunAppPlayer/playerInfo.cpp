#include "StdAfx.h"
#include "playerInfo.h"

playerInfo::playerInfo(void)
{
}

playerInfo::~playerInfo(void)
{
}

BOOL playerInfo::RunBlueStacks(CString strpackage, CString stractivity)
{
	CString strInstallDir = IsExistBlueStacksInfo("InstallDir");

	
	CString strAppExe;
	strAppExe.Format("%s\\HD-RunApp.exe", GetFilePath(strInstallDir));
	if(!::PathFileExists(strAppExe)) 
		return FALSE;

	CString strParams;	
	strParams.Format(" -p %s -a %s", strpackage, stractivity);
	HINSTANCE hInstance = ::ShellExecute(NULL, "open", strAppExe, strParams, NULL, SW_SHOW);
	
	return TRUE;
}

BOOL playerInfo::RunDnplayer(CString strPackageName)
{
	CString strAppExe;
	strAppExe.Format("D:\\LDPlayer\\dnplayer.exe");

	CString strParams;
	strParams.Format("package=%s", strPackageName);

	HINSTANCE hInstance = ::ShellExecute(NULL, "open", strAppExe, strParams, NULL, SW_SHOW);

	return TRUE;
}

BOOL playerInfo::RunNox(CString strPackageName)
{
	CString strInstallDir = IsExistNoxInfo("UninstallString");

	CString strAppExe;
	strAppExe.Format("%s\\_Nox.exe", GetFilePath(strInstallDir));
	if(!::PathFileExists(strAppExe)) 
		return FALSE;

	CString strParams;
	strParams.Format("%s -package:%s", strAppExe, strPackageName);
	HINSTANCE hInstance = ::ShellExecute(NULL, "open", strAppExe, strParams, NULL, SW_SHOW);
	
	return TRUE;
}

CString playerInfo::IsExistNoxInfo(LPCTSTR szKey)
{
	char szBuffer [MAX_PATH + 1] = {0, };
	DWORD dwCount = 0;

	CRegKey regKey;

	if (regKey.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Nox"), KEY_READ) != ERROR_SUCCESS)
		return "";

	dwCount = REG_BUFF_SIZE + 1;

	if (ERROR_SUCCESS != regKey.QueryStringValue(szKey, szBuffer, &dwCount))
	{
		regKey.Close();
		return "";
	}

	regKey.Close();

	return (CString)szBuffer;
}

CString playerInfo::GetFilePath(CString strFilename)
{
	int nPos = strFilename.ReverseFind('\\');
	if(nPos > 0)
		return strFilename.Left(nPos);

	return strFilename;
}

CString playerInfo::GetModulePath(LPCTSTR pszSubPath )
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

CString playerInfo::IsExistBlueStacksInfo(LPCTSTR szKey)
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

BOOL playerInfo::IsWindows64Bit()
{
	if (sizeof(int*) == 8) return TRUE;
	BOOL b = FALSE;
	IsWow64Process(GetCurrentProcess(), &b);
	return b;
}