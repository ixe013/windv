// VideoDeviceSel.cpp : implementation file
//

#include "stdafx.h"
#include "WinDV.h"
#include "DShow.h"
#include "VideoDeviceSel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVideoDeviceSel dialog


CVideoDeviceSel::CVideoDeviceSel(CArray<CString,CString &> &list, LPCSTR selName, CWnd* pParent /*=NULL*/)
	: CDialog(CVideoDeviceSel::IDD, pParent)
{
	m_list = &list;
	m_selected = -1;
	m_selName = selName;

	//{{AFX_DATA_INIT(CVideoDeviceSel)
	//}}AFX_DATA_INIT
}

void CVideoDeviceSel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVideoDeviceSel)
	DDX_Control(pDX, IDC_DEVLIST, m_listbox);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVideoDeviceSel, CDialog)
	//{{AFX_MSG_MAP(CVideoDeviceSel)
	ON_LBN_DBLCLK(IDC_DEVLIST, OnDblclkDevlist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVideoDeviceSel message handlers

void CVideoDeviceSel::OnDblclkDevlist() 
{
	OnOK();
}


void CVideoDeviceSel::OnOK() 
{
	// TODO: Add extra validation here
	m_selected = m_listbox.GetCurSel();
	if (m_selected >= 0)
		CDialog::OnOK();
}

BOOL CVideoDeviceSel::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	int i,n=m_list->GetSize();
	for(i=0; i<n; i++) {
		m_listbox.AddString(m_list->GetAt(i));
		if (m_list->GetAt(i) == m_selName) {
			m_selected = i;
		}
		i++;
	}
	m_listbox.SetCurSel(m_selected);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
