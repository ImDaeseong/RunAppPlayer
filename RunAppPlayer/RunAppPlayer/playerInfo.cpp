#include "StdAfx.h"
#include "playerInfo.h"

playerInfo::playerInfo(void)
{
}

playerInfo::~playerInfo(void)
{
}

BOOL playerInfo::RunBlueStacks(CString strpackage, CString stractivity, DWORD dwWndProcessID)
{
	CString sFullPath = GetPrcessFilePath(dwWndProcessID);
	if(sFullPath == "")
	{
		CString strInstallDir = IsExistBlueStacksInfo("InstallDir");
		
		CString strAppExe;
		strAppExe.Format("%s\\HD-RunApp.exe", GetFilePath(strInstallDir));
		if(!::PathFileExists(strAppExe)) 
			return FALSE;

		CString strParams;	
		strParams.Format(" -p %s -a %s", strpackage, stractivity);
		HINSTANCE hInstance = ::ShellExecute(NULL, "open", strAppExe, strParams, NULL, SW_SHOW);
	}
	else
	{
		CString strAppExe;
		strAppExe.Format("%s", sFullPath);
		if(!::PathFileExists(strAppExe)) 
			return FALSE;

		CString strParams;	
		strParams.Format(" -p %s -a %s", strpackage, stractivity);
		HINSTANCE hInstance = ::ShellExecute(NULL, "open", strAppExe, strParams, NULL, SW_SHOW);
	}
	
	return TRUE;
}

BOOL playerInfo::RunDnplayer(CString strPackageName, DWORD dwWndProcessID)
{
	CString sFullPath = GetPrcessFilePath(dwWndProcessID);
	if(sFullPath == "")
	{
		CString strAppExe;
		strAppExe.Format("D:\\LDPlayer\\dnplayer.exe");
		if(!::PathFileExists(strAppExe)) 
			return FALSE;

		CString strParams;
		strParams.Format("package=%s", strPackageName);
		HINSTANCE hInstance = ::ShellExecute(NULL, "open", strAppExe, strParams, NULL, SW_SHOW);
	}
	else
	{
		CString strAppExe;
		strAppExe.Format("%s", sFullPath);
		if(!::PathFileExists(strAppExe)) 
			return FALSE;

		CString strParams;
		strParams.Format("package=%s", strPackageName);
		HINSTANCE hInstance = ::ShellExecute(NULL, "open", strAppExe, strParams, NULL, SW_SHOW);	
	}

	return TRUE;
}

BOOL playerInfo::RunNox(CString strPackageName, DWORD dwWndProcessID)
{
	CString sFullPath = GetPrcessFilePath(dwWndProcessID);
	if(sFullPath == "")
	{
		CString strInstallDir = IsExistNoxInfo("UninstallString");

		CString strAppExe;
		strAppExe.Format("%s\\_Nox.exe", GetFilePath(strInstallDir));
		if(!::PathFileExists(strAppExe)) 
			return FALSE;

		CString strParams;
		strParams.Format("-package:%s", strPackageName);
		HINSTANCE hInstance = ::ShellExecute(NULL, "open", strAppExe, strParams, NULL, SW_SHOW);
	}
	else
	{
		CString strAppExe;
		strAppExe.Format("%s", sFullPath);
		if(!::PathFileExists(strAppExe)) 
			return FALSE;

		CString strParams;
		strParams.Format("-package:%s", strPackageName);
		HINSTANCE hInstance = ::ShellExecute(NULL, "open", strAppExe, strParams, NULL, SW_SHOW);	
	}
	
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

CString playerInfo::GetPrcessFilePath(DWORD processID)
{
	CString strFilePath;

	PROCESSENTRY32 proc;
	proc.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	::Process32First(snapshot, &proc);

	while (TRUE == ::Process32Next(snapshot, &proc))
	{
		if (proc.th32ProcessID == processID)
		{
			MODULEENTRY32 me;
			me.dwSize = sizeof(MODULEENTRY32);

			CloseHandle(snapshot);

			snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, proc.th32ProcessID);
			::Module32First(snapshot, &me);

			strFilePath.Format("%s", me.szExePath);

			break;
		}
	}
	::CloseHandle(snapshot);

	return strFilePath;
}

CString playerInfo::GetPrcessFileName(DWORD processID)
{	
	CString strFileName;

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap)
	{
		if (Process32First(hProcessSnap, &pe32))
		{
			do 
			{				
				if(pe32.th32ProcessID == processID)
				{	 
					strFileName.Format("%s", pe32.szExeFile);
					break;
				}

			} while (Process32Next(hProcessSnap, &pe32));
		}
	}
	CloseHandle(hProcessSnap);

	return strFileName;
}