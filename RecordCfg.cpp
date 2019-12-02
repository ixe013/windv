// RecordCfg.cpp : implementation file
//

#include "stdafx.h"
#include "WinDV.h"
#include "DropFilesEdit.h"
#include "RecordCfg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRecordCfg dialog


CRecordCfg::CRecordCfg()
	: CPropertyPage(CRecordCfg::IDD), m_aviPrefixCtl(" | "), m_aviSuffixCtl(" | ")
{
	//{{AFX_DATA_INIT(CRecordCfg)
	m_recordPreview = FALSE;
	m_aviPrefix = _T("");
	m_aviSuffix = _T("");
	//}}AFX_DATA_INIT
}


void CRecordCfg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecordCfg)
	DDX_Control(pDX, IDC_AVI_SUFFIX, m_aviSuffixCtl);
	DDX_Control(pDX, IDC_AVI_PREFIX, m_aviPrefixCtl);
	DDX_Check(pDX, IDC_RECORDPREVIEW, m_recordPreview);
	DDX_Text(pDX, IDC_AVI_PREFIX, m_aviPrefix);
	DDX_Text(pDX, IDC_AVI_SUFFIX, m_aviSuffix);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRecordCfg, CPropertyPage)
	//{{AFX_MSG_MAP(CRecordCfg)
	ON_BN_CLICKED(IDC_PREFIX_SEL, OnPrefixSel)
	ON_BN_CLICKED(IDC_SUFFIX_SEL, OnSuffixSel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecordCfg message handlers


void CRecordCfg::OnPrefixSel() 
{
	SelectFile(TRUE, GetDlgItem(IDC_AVI_PREFIX));
}

void CRecordCfg::OnSuffixSel() 
{
	SelectFile(TRUE, GetDlgItem(IDC_AVI_SUFFIX));
}
