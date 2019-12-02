// WinDV.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "WinDV.h"
#include "DropFilesEdit.h"
#include "DShow.h"
#include "ToolTab.h"
#include "DVToolsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWinDVApp

BEGIN_MESSAGE_MAP(CWinDVApp, CWinApp)
	//{{AFX_MSG_MAP(CWinDVApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
//	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWinDVApp construction

CWinDVApp::CWinDVApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CWinDVApp object

CWinDVApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CWinDVApp initialization

BOOL CWinDVApp::InitInstance()
{
	AfxEnableControlContainer();

	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	SetRegistryKey("Petr Mourek");

	CDVToolsDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	CoUninitialize();
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////

void SelectFile(BOOL open, CWnd *ctrl)
{
	CFileDialog dlg(open, NULL, NULL, (open ? OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY : 0),
		"*.avi|*.avi||");

	dlg.m_ofn.lpstrInitialDir=".";
	char fbuf[16384] = "";
	dlg.m_ofn.lpstrFile = fbuf;
	dlg.m_ofn.nMaxFile = sizeof fbuf;
	if (dlg.DoModal() == IDOK) {
		CString txt;
		if (open) {
			POSITION p = dlg.GetStartPosition();
			while (p) {
				txt += dlg.GetNextPathName(p);
				if (p) txt += " | ";
			}

		}
		else
			txt = dlg.GetPathName();

		ctrl->SetWindowText(txt);
	}

}

/////////////////////////////////////////////////////////////////////////////
