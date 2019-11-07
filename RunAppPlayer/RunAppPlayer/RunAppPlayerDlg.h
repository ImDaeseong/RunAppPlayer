#pragma once

class CRunAppPlayerDlg : public CDialog
{
public:
	CRunAppPlayerDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_RUNAPPPLAYER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()

private:
	CString GetPrcessFileName(DWORD processID);
	void IsAppPlayerRunning();	
};
