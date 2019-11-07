#pragma once

class playerInfo
{
public:
	playerInfo(void);
	~playerInfo(void);

public:

	BOOL RunBlueStacks(CString strpackage, CString stractivity);
	BOOL RunDnplayer(CString strPackageName);
	BOOL RunNox(CString strPackageName);

	CString IsExistNoxInfo(LPCTSTR szKey);
	CString GetFilePath(CString strFilename);
	CString GetModulePath(LPCTSTR pszSubPath = NULL);
	CString IsExistBlueStacksInfo(LPCTSTR szKey);

	BOOL IsWindows64Bit();
};
