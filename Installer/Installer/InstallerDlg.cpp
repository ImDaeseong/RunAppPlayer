#include "stdafx.h"
#include "Installer.h"
#include "InstallerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CInstallerDlg::CInstallerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInstallerDlg::IDD, pParent)
{
}

void CInstallerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CInstallerDlg, CDialog)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CInstallerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;  
}

void CInstallerDlg::OnPaint()
{
	CPaintDC dc(this);
}
