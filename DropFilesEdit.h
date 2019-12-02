#if !defined(AFX_DROPFILESEDIT_H__B0489045_2D71_4D76_B7DC_EE835A73F6F0__INCLUDED_)
#define AFX_DROPFILESEDIT_H__B0489045_2D71_4D76_B7DC_EE835A73F6F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DropFilesEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDropFilesEdit window

class CDropFilesEdit : public CEdit
{
// Construction
public:
	CDropFilesEdit(LPCSTR multidrop_separator=NULL, bool (*filter)(CString &) = NULL);

// Attributes
public:
	CString m_separator;
	bool (*m_filter)(CString &);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDropFilesEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDropFilesEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDropFilesEdit)
	afx_msg void OnDropFiles(HDROP hDropInfo);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DROPFILESEDIT_H__B0489045_2D71_4D76_B7DC_EE835A73F6F0__INCLUDED_)
