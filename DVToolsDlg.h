// DVToolsDlg.h : header file
//

#if !defined(AFX_DVTOOLSDLG_H__0633EC27_A5A4_4B4B_8547_D2DE0ADC9AC9__INCLUDED_)
#define AFX_DVTOOLSDLG_H__0633EC27_A5A4_4B4B_8547_D2DE0ADC9AC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDVToolsDlg dialog

class CDVToolsDlg : public CDialog
{
// Construction
public:
	CDVToolsDlg(CWnd* pParent = NULL);	// standard constructor
	virtual  ~CDVToolsDlg();

// Dialog Data
	//{{AFX_DATA(CDVToolsDlg)
	enum { IDD = IDD_DVTOOLS_DIALOG };
	CToolTab	m_toolTab;
	CButton	m_DVCtrl;
	CStatic	m_counter;
	CStatic	m_status3;
	CStatic	m_status2;
	CDV	m_video;
	CStatic	m_VDST;
	CStatic	m_VSRC;
	CDropFilesEdit	m_FSRC;
	CDropFilesEdit	m_FDST;
	CStatic	m_status;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDVToolsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void Exception2Status(CException *e);
	void InitVideo();
	void SetToolTabItemSize();

	HICON m_hIcon, m_hIconSmall;

	RECT *m_originalRects;
	RECT m_originalRect;
	RECT m_lastRect;
	int m_minWidth, m_minHeight;
	CButton *tabChangeBtns;

	CString m_VSRCname;
	CString m_VDSTname;

	CString m_AVIPrefix, m_AVISuffix;

	CString m_DTFormat; CString m_DTFormatHistory;
	int m_nSuffixDigits;

	enum {Iddle, CapturePaused, Capturing, RecordPaused, Recording};

	bool m_exitOnFinish;

	afx_msg void OnCmdTabChange(UINT nID);
	afx_msg LRESULT OnDVTimeChange(WPARAM, LPARAM);
	// Generated message map functions
	//{{AFX_MSG(CDVToolsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnSelchangeToolTab(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnCancel();
	afx_msg void OnClose();
	virtual void OnOK();
	afx_msg void OnVsrcSel();
	afx_msg void OnVdstSel();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnFdstSel();
	afx_msg void OnFsrcSel();
	afx_msg void OnConfig();
	afx_msg void OnCapture();
	afx_msg void OnRecord();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPicture();
	afx_msg void OnDvctrl();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DVTOOLSDLG_H__0633EC27_A5A4_4B4B_8547_D2DE0ADC9AC9__INCLUDED_)
