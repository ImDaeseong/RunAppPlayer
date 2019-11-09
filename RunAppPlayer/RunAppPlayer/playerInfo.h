#pragma once

class playerInfo
{
public:
	playerInfo(void);
	~playerInfo(void);

public:

	BOOL RunBlueStacks(CString strpackage, CString stractivity, DWORD dwWndProcessID = 0);
	BOOL RunDnplayer(CString strPackageName, DWORD dwWndProcessID = 0);
	BOOL RunNox(CString strPackageName, DWORD dwWndProcessID = 0);

	CString IsExistNoxInfo(LPCTSTR szKey);
	CString GetFilePath(CString strFilename);
	CString GetModulePath(LPCTSTR pszSubPath = NULL);
	CString IsExistBlueStacksInfo(LPCTSTR szKey);

	BOOL IsWindows64Bit();
	CString GetPrcessFilePath(DWORD processID);
	CString GetPrcessFileName(DWORD processID);
};
