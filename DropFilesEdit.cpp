// DropFilesEdit.cpp : implementation file
//

#include "stdafx.h"
#include "DropFilesEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static bool NoFilter(CString &)
{
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// CDropFilesEdit

CDropFilesEdit::CDropFilesEdit(LPCSTR multidrop_separator, bool (*filter)(CString &))
: m_separator(multidrop_separator), m_filter(filter)
{
	if (!m_filter) m_filter = NoFilter;
}

CDropFilesEdit::~CDropFilesEdit()
{
}


BEGIN_MESSAGE_MAP(CDropFilesEdit, CEdit)
	//{{AFX_MSG_MAP(CDropFilesEdit)
	ON_WM_DROPFILES()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDropFilesEdit message handlers

void CDropFilesEdit::OnDropFiles(HDROP hDropInfo) 
{
	// TODO: Add your message handler code here and/or call default

	TCHAR buf[256];
	int n = 1;

	if (!m_separator.IsEmpty()) {
		n = DragQueryFile(hDropInfo, -1, NULL, 0);
	}
	CString file, files;

	int i;
	for(i=0; i<n; i++) {
		DragQueryFile(hDropInfo, i, buf, sizeof buf / sizeof (TCHAR));
		file = buf;
		if (m_filter(file)) {
			if (!files.IsEmpty()) {
				files += m_separator;
			}
			files += file;
		}
	}
	DragFinish(hDropInfo);

	if (!files.IsEmpty()) SetWindowText(files);
}
