#if !defined(AFX_RECORDCFG_H__C6EDDD3E_4F72_4E92_939C_B69F736C5612__INCLUDED_)
#define AFX_RECORDCFG_H__C6EDDD3E_4F72_4E92_939C_B69F736C5612__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecordCfg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRecordCfg dialog

class CRecordCfg : public CPropertyPage
{
// Construction
public:
	CRecordCfg();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRecordCfg)
	enum { IDD = IDD_RECORD_CONFIG };
	CDropFilesEdit	m_aviSuffixCtl;
	CDropFilesEdit	m_aviPrefixCtl;
	BOOL	m_recordPreview;
	CString	m_aviPrefix;
	CString	m_aviSuffix;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRecordCfg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRecordCfg)
	afx_msg void OnPrefixSel();
	afx_msg void OnSuffixSel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECORDCFG_H__C6EDDD3E_4F72_4E92_939C_B69F736C5612__INCLUDED_)
