#pragma once

class CInstallerDlg : public CDialog
{
public:
	CInstallerDlg(CWnd* pParent = NULL);	

	enum { IDD = IDD_INSTALLER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};
