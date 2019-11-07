#pragma once

#define MAX_WAIT_TIME	5000


typedef UINT (WINAPI	*FunGetSystemWow64Directory)(LPTSTR lpBuffer, UINT uSize);


class InstallUtil
{
public:
	InstallUtil(void);
	virtual ~InstallUtil(void);

public:
	
	void Setup();
	CString IsSetupInfo(LPCTSTR szKey);
	void CopyFiles();
	void SetRegInfos();	
	BOOL RunBatchfile();
	
	CString GetProgramFilesBlueStacksFolder();
	CString GetProgramFilesFolder();	
	CString GetCommonAppDataPath();
	CString GetMyPicturePath();
	CString GetMyDocumentPath();
	CString GetCommonPicturePath();
	CString GetCommonDocumentPath();
	CString GetSystemPath();
	CString GetAppDataPath();
	CString GetAppLocalDataPath();
	CString GetModulePath(LPCTSTR pszSubPath = NULL);

	BOOL IsWindows64Bit();

	void SetRegDateMultiString(LPTSTR lpRegKey, LPCTSTR lpKey, CString strValue);
	void SetRegDateString(LPTSTR lpRegKey, LPCTSTR lpKey, CString strValue);
	void SetRegDateDword(LPCTSTR lpRegKey, LPCTSTR lpKey, int nValue);
	void CreateRegKey(LPCTSTR lpRegKey);	

	BOOL InstallService(CString strServiceName,CString strDisplayName, CString strServicePath);
	BOOL UnInstallService(CString strServiceName);
	BOOL StartService(CString strServiceName);
	BOOL StopService(CString strServiceName);

	BOOL GetReourceFiles(CString strFileName, CString strPart, UINT nID);
	void SetRegInstall();

	BOOL GetSysWow64Dir(LPTSTR pszDir, int nLen);
	BOOL RunRegfile();

private:
	CString strPackageFolder;
};
