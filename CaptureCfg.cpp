// CaptureCfg.cpp : implementation file
//

#include "stdafx.h"
#include "WinDV.h"
#include "CaptureCfg.h"
#include "DShow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCaptureCfg dialog


CCaptureCfg::CCaptureCfg()
	: CPropertyPage(CCaptureCfg::IDD)
{
	//{{AFX_DATA_INIT(CCaptureCfg)
	m_discontinuityTreshold = 0;
	m_everyNth = 0;
	m_maxAVIFrames = 0;
	m_type12 = -1;
	m_dtformat = _T("");
	m_ndigits = -1;
	//}}AFX_DATA_INIT
}


void CCaptureCfg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCaptureCfg)
	DDX_Control(pDX, IDC_FEXAMPLE, m_fexample);
	DDX_Control(pDX, IDC_DTFORMAT, m_dtformatctl);
	DDX_Control(pDX, IDC_NDIGITS, m_ndigitsctl);
	DDX_Text(pDX, IDC_DISCONTINUITY_TRESHOLD, m_discontinuityTreshold);
	DDV_MinMaxUInt(pDX, m_discontinuityTreshold, 0, 1000000);
	DDX_Text(pDX, IDC_EVERY_NTH, m_everyNth);
	DDV_MinMaxUInt(pDX, m_everyNth, 1, 1000000);
	DDX_Text(pDX, IDC_MAX_FRAMES, m_maxAVIFrames);
	DDV_MinMaxUInt(pDX, m_maxAVIFrames, 10, 1000000);
	DDX_Radio(pDX, IDC_TYPE_1, m_type12);
	DDX_CBString(pDX, IDC_DTFORMAT, m_dtformat);
	DDX_CBIndex(pDX, IDC_NDIGITS, m_ndigits);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCaptureCfg, CPropertyPage)
	//{{AFX_MSG_MAP(CCaptureCfg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCaptureCfg message handlers

void CCaptureCfg::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == 1) {
		CString tmp;
		m_dtformatctl.GetWindowText(tmp);
		CString tmp2 = FormatTime(tmp, time(NULL));
		tmp = "...example";
		if (!tmp2.IsEmpty()) {
			tmp += ".";
			tmp += tmp2;
		}
		int n = m_ndigitsctl.GetCurSel();
		tmp2.Empty();
		if (n) tmp2.Format(".%0*d", n, 0);
		m_fexample.SetWindowText(tmp+tmp2+".avi");
	}
	else {
		CPropertyPage::OnTimer(nIDEvent);
	}
}

BOOL CCaptureCfg::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CString tmp;
	int i;
	for(i = 0; i<=4; i++) {
		tmp.Format("%d", i);
		m_ndigitsctl.AddString(tmp);
	}
	m_dtformatctl.AddString(m_dtformat);
	while (!m_dtformathistory.IsEmpty()) {
		tmp = m_dtformathistory.SpanExcluding("\n");
		int l = tmp.GetLength();
		if (l < m_dtformathistory.GetLength())
			m_dtformathistory = m_dtformathistory.Mid(l+1);
		else 
			m_dtformathistory.Empty();

		m_dtformatctl.AddString(tmp);
	}
	if (!m_dtformat.IsEmpty()) m_dtformatctl.AddString("");
	
	m_ndigitsctl.SetCurSel(m_ndigits);
	OnTimer(1);
	SetTimer(1, 500, NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCaptureCfg::OnOK() 
{
	CPropertyPage::OnOK();
	int i,n;
	i = m_dtformatctl.FindStringExact(-1,m_dtformat);
	if (i>=0) m_dtformatctl.DeleteString(i);
	m_dtformathistory.Empty();
	n = m_dtformatctl.GetCount();
	if (n > 10) n = 10;
	for (i=0; i<n; i++) {
		CString tmp;
		m_dtformatctl.GetLBText(i, tmp);
		if (!tmp.IsEmpty()) {
			if (!m_dtformathistory.IsEmpty()) m_dtformathistory += "\n";
			m_dtformathistory += tmp;
		}
	}
}
