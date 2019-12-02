#if !defined(AFX_TOOLTAB_H__AB98922A_AE7E_4572_8094_F2F1BCE5E107__INCLUDED_)
#define AFX_TOOLTAB_H__AB98922A_AE7E_4572_8094_F2F1BCE5E107__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolTab.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CToolTab window

class CToolTab : public CTabCtrl
{
// Construction
public:
	CToolTab();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolTab)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CToolTab();

protected:
	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_TOOLTAB_H__AB98922A_AE7E_4572_8094_F2F1BCE5E107__INCLUDED_)
