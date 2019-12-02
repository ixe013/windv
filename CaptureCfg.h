#if !defined(AFX_CAPTURECFG_H__88AB6830_E82C_4984_9B13_1C854FA927CE__INCLUDED_)
#define AFX_CAPTURECFG_H__88AB6830_E82C_4984_9B13_1C854FA927CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CaptureCfg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCaptureCfg dialog

class CCaptureCfg : public CPropertyPage
{
// Construction
public:
	CCaptureCfg();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCaptureCfg)
	enum { IDD = IDD_CAPTURE_CONFIG };
	CStatic	m_fexample;
	CComboBox	m_dtformatctl;
	CComboBox	m_ndigitsctl;
	UINT	m_discontinuityTreshold;
	UINT	m_everyNth;
	UINT	m_maxAVIFrames;
	int		m_type12;
	CString	m_dtformat;
	int		m_ndigits;
	//}}AFX_DATA

	CString m_dtformathistory;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCaptureCfg)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCaptureCfg)
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAPTURECFG_H__88AB6830_E82C_4984_9B13_1C854FA927CE__INCLUDED_)
