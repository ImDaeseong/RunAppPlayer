#include "stdafx.h"
#include "RunAppPlayer.h"
#include "RunAppPlayerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


typedef struct {
   char* chClassName;
   char* chWindowTitle;
   char* chExeName;
   int   nPlayer;
} Class_t;


#define NumEnt 4
static const Class_t ClassEnts[NumEnt] = 
{
   {"LDPlayerMainFrame", "LDPlayer", "dnplayer.exe", 1},
   {"Qt5QWindowIcon", "NoxPlayer", "_Nox.exe", 2},
   {"Qt5QWindowIcon", "³ì½º", "_Nox.exe", 2},   
   {"HwndWrapper[Bluestacks.exe", "", "Bluestacks.exe", 3}
};

typedef struct _FIND_WINDOW_PARAM
{
	LPCTSTR pszClassName;
	LPCTSTR pszWindowName;
	DWORD dwProcessID;
	HWND hwnd;

} FIND_WINDOW_PARAM, *LPFIND_WINDOW_PARAM;


BOOL CALLBACK FindWindowLikeProc(HWND hwnd, LPARAM lParam)
{
	BOOL bRes = TRUE;
	char className[MAX_PATH + 1] = {0, };
	char wndName[MAX_PATH + 1] = {0, };
	DWORD dwProcessID = 0;

	LPFIND_WINDOW_PARAM findParam = (LPFIND_WINDOW_PARAM)lParam;
	GetClassName(hwnd, className, MAX_PATH + 1);
	GetWindowText(hwnd, wndName, MAX_PATH + 1);

	if(_stricmp(className, findParam->pszClassName) == 0)
	{
		CString szWindowTitle = wndName;
		if(szWindowTitle.Find(findParam->pszWindowName) >= 0)
		{
			findParam->hwnd = hwnd;
			GetWindowThreadProcessId(hwnd, &findParam->dwProcessID);
			bRes = FALSE;
		}
	}
	return bRes;
}

HWND FindWindowLikeTitle(LPCTSTR pszClassName, LPCTSTR pszWindowName, LPDWORD pRetProcessID)
{
	HWND hwnd = NULL;
	FIND_WINDOW_PARAM param;
	param.pszClassName = pszClassName;
	param.pszWindowName = pszWindowName;
	param.dwProcessID = 0;
	param.hwnd = NULL;

	::EnumWindows(FindWindowLikeProc, (LPARAM)&param);

	if(pRetProcessID != NULL)
		*pRetProcessID = param.dwProcessID;

	return param.hwnd;
}


BOOL CALLBACK FindClassLikeProc(HWND hwnd, LPARAM lParam)
{
	BOOL bRes = TRUE;
	char className[MAX_PATH + 1] = {0, };
	DWORD dwProcessID = 0;

	LPFIND_WINDOW_PARAM findParam = (LPFIND_WINDOW_PARAM)lParam;
	GetClassName(hwnd, className, MAX_PATH + 1);

	CString szClassName = className;
	if(szClassName.Find(findParam->pszClassName) >= 0)
	{
		findParam->hwnd = hwnd;
		GetWindowThreadProcessId(hwnd, &findParam->dwProcessID);
		bRes = FALSE;
	}
	return bRes;
}

HWND FindWindowLikeClass(LPCTSTR pszClassName, LPDWORD pRetProcessID)
{
	HWND hwnd = NULL;
	FIND_WINDOW_PARAM param;
	param.pszClassName = pszClassName;
	param.pszWindowName = "";
	param.dwProcessID = 0;
	param.hwnd = NULL;

	::EnumWindows(FindClassLikeProc, (LPARAM)&param);

	if(pRetProcessID != NULL)
		*pRetProcessID = param.dwProcessID;

	return param.hwnd;
}

CString CRunAppPlayerDlg::GetPrcessFileName(DWORD processID)
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

void CRunAppPlayerDlg::IsAppPlayerRunning()
{
	for(int i=0; i< NumEnt; i++)
	{
		DWORD dwWndProcessID = 0;
		HWND hAppPlayer = NULL;

		if( ClassEnts[i].nPlayer == 3)
			hAppPlayer = FindWindowLikeClass(ClassEnts[i].chClassName, &dwWndProcessID);
		else
			hAppPlayer = FindWindowLikeTitle(ClassEnts[i].chClassName, ClassEnts[i].chWindowTitle, &dwWndProcessID);

		if(hAppPlayer)
		{			
			HWND hwnd = ::GetForegroundWindow();//::GetActiveWindow();		
			if(hAppPlayer != hwnd )
				continue;

			if( ClassEnts[i].nPlayer == 1)
			{
				playerInfo obj;
				obj.RunDnplayer("com.kakaogames.moonlight");
			}
			else if( ClassEnts[i].nPlayer == 2)
			{
				playerInfo obj;
				obj.RunNox("com.kakaogames.moonlight");
			}
			else if( ClassEnts[i].nPlayer == 3)
			{
				playerInfo obj;
				obj.RunBlueStacks("com.kakaogames.moonlight", "com.kakaogame.KGUnityPlayerActivity");
			}		

			//CString sFileName = GetPrcessFileName(dwWndProcessID);
		
		}
	}

}


CRunAppPlayerDlg::CRunAppPlayerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRunAppPlayerDlg::IDD, pParent)
{
}

void CRunAppPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRunAppPlayerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

BOOL CRunAppPlayerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();	


	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW, 0);
	ShowWindow(SW_HIDE);
	MoveWindow(-3333, -3333, 1, 1);

	SetTimer(1, 10000, NULL);

	return TRUE;  
}

void CRunAppPlayerDlg::OnDestroy() 
{
	KillTimer(1);

	CDialog::OnDestroy();
}

void CRunAppPlayerDlg::OnPaint()
{
	CPaintDC dc(this);
}

void CRunAppPlayerDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == 1)
	{
		IsAppPlayerRunning();
	}

	CDialog::OnTimer(nIDEvent);
}
