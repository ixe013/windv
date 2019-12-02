#if !defined(AFX_VIDEODEVICESEL_H__5B9B5B7A_A184_467F_B190_5447284D9A80__INCLUDED_)
#define AFX_VIDEODEVICESEL_H__5B9B5B7A_A184_467F_B190_5447284D9A80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VideoDeviceSel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVideoDeviceSel dialog

class CVideoDeviceSel : public CDialog
{
// Construction
public:
	CVideoDeviceSel(CArray<CString,CString&> &list, LPCSTR selName, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CVideoDeviceSel)
	enum { IDD = IDD_VIDEODEVICESEL };
	CListBox	m_listbox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVideoDeviceSel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CArray<CString,CString&> *m_list;
	CString m_selName;
	int m_selected;

	// Generated message map functions
	//{{AFX_MSG(CVideoDeviceSel)
	afx_msg void OnDblclkDevlist();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public: 
	int GetSelection() {return m_selected;}
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIDEODEVICESEL_H__5B9B5B7A_A184_467F_B190_5447284D9A80__INCLUDED_)
