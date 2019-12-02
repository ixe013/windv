// DVToolsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "process.h"
#include "WinDV.h"
#include "DropFilesEdit.h"
#include "DShow.h"
#include "ToolTab.h"
#include "DVToolsDlg.h"
#include "VideoDeviceSel.h"
#include "CaptureCfg.h"
#include "RecordCfg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	afx_msg void OnEmail();
	afx_msg void OnUrl();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT

}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_COMMAND(IDC_EMAIL, OnEmail)
	ON_COMMAND(IDC_URL, OnUrl)
	ON_WM_CTLCOLOR()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void UrlThread(void *ptr)
{
	ShellExecute(NULL, "open", (char *)ptr, NULL, NULL, SW_SHOWNORMAL);
}

void CAboutDlg::OnEmail() 
{
	_beginthread(UrlThread, 0, "mailto:petr@mourek.cz?subject=WinDV");
}

void CAboutDlg::OnUrl() 
{
	_beginthread(UrlThread, 0, "http://windv.mourek.cz/");
}

HBRUSH CAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (pWnd->GetDlgCtrlID()) {
	case IDC_EMAIL:
	case IDC_URL:
		pDC->SetTextColor(RGB(0,0,192));
	}
	
	return hbr;
}

BOOL CAboutDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	switch (pWnd->GetDlgCtrlID()) {
	case IDOK:
	case IDC_EMAIL:
	case IDC_URL:
		SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(32649)));
		return TRUE;
	}
	
	
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

/////////////////////////////////////////////////////////////////////////////
// CDVToolsDlg dialog

#define IDC_TAB_CHANGE 0x100

enum {TAB_ALL=-1, TAB_NONE=0, TAB_CAPTURE=1, TAB_RECORD=2};

#define XL 25
#define XR 75

static struct CtrlProperties {int id; int dx, dw, dy, dh; int tabMask;} ctrlProperties[] = 
{
	{IDC_VIDEO,		  0,100,  0,100,	TAB_ALL},
	{IDC_PICTURE,	 XR, XR,100,100,	TAB_ALL},
	{IDC_TOOL_TAB,	 XL, XR,100,100,	TAB_ALL},
	{IDC_VSRC_L,	 XL, XL,100,100,	TAB_CAPTURE},
	{IDC_VSRC,		 XL, XR,100,100,	TAB_CAPTURE},
	{IDC_VSRC_SEL,	 XR, XR,100,100,	TAB_CAPTURE},
	{IDC_FSRC_L,	 XL, XL,100,100,	TAB_RECORD},
	{IDC_FSRC,		 XL, XR,100,100,	TAB_RECORD},
	{IDC_FSRC_SEL,	 XR, XR,100,100,	TAB_RECORD},
	{IDC_FDST_L,	 XL, XL,100,100,	TAB_CAPTURE},
	{IDC_FDST,		 XL, XR,100,100,	TAB_CAPTURE},
	{IDC_FDST_SEL,	 XR, XR,100,100,	TAB_CAPTURE},
	{IDC_VDST_L,	 XL, XL,100,100,	TAB_RECORD},
	{IDC_VDST,		 XL, XR,100,100,	TAB_RECORD},
	{IDC_VDST_SEL,	 XR, XR,100,100,	TAB_RECORD},
	{IDC_CONFIG,	 XR, XR,100,100,	TAB_CAPTURE | TAB_RECORD},
	{IDC_DVCTRL,	 XR, XR,100,100,	TAB_ALL},
	{IDC_CAPTURE,	 XR, XR,100,100,	TAB_CAPTURE},
	{IDC_RECORD,	 XR, XR,100,100,	TAB_RECORD},
	{IDCANCEL,		 XR, XR,100,100,	TAB_CAPTURE | TAB_RECORD},
	{IDC_COUNTER,	 XR, XR,100,100,	TAB_ALL},
	{IDC_STATUS,	 XL, XR,100,100,	TAB_ALL},
	{IDC_STATUS2,	 XR, XR,100,100,	TAB_ALL},
	{IDC_STATUS3,	 XR, XR,100,100,	TAB_ALL}
};

#define NControls (sizeof ctrlProperties/sizeof (struct CtrlProperties))

bool CaptureFilenameExtractBase(CString &file)
{
	int pos = file.Find(".");
	if (pos>=0) file = file.Mid(0, pos);
	return ! file.IsEmpty();
}


CDVToolsDlg::CDVToolsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDVToolsDlg::IDD, pParent), m_FSRC(" | "), m_FDST(NULL, CaptureFilenameExtractBase)
{
	//{{AFX_DATA_INIT(CDVToolsDlg)
	//}}AFX_DATA_INIT

	m_hIcon      = (HICON)LoadImage(AfxGetResourceHandle(), 
	                         MAKEINTRESOURCE(IDR_MAINFRAME), 
							 IMAGE_ICON,
							 0, 0, LR_DEFAULTSIZE);
	m_hIconSmall = (HICON)LoadImage(AfxGetResourceHandle(), 
	                         MAKEINTRESOURCE(IDR_MAINFRAME), 
							 IMAGE_ICON,
							 16, 16, 0);

	tabChangeBtns = NULL;
	m_originalRects = NULL;
	m_originalRect.right = 0;
	m_minWidth = m_minHeight = 1;
	m_exitOnFinish = 0;
}

CDVToolsDlg::~CDVToolsDlg()
{
	delete[] tabChangeBtns;
	delete[] m_originalRects;
}

void CDVToolsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDVToolsDlg)
	DDX_Control(pDX, IDC_TOOL_TAB, m_toolTab);
	DDX_Control(pDX, IDC_DVCTRL, m_DVCtrl);
	DDX_Control(pDX, IDC_COUNTER, m_counter);
	DDX_Control(pDX, IDC_STATUS3, m_status3);
	DDX_Control(pDX, IDC_STATUS2, m_status2);
	DDX_Control(pDX, IDC_VIDEO, m_video);
	DDX_Control(pDX, IDC_VDST, m_VDST);
	DDX_Control(pDX, IDC_VSRC, m_VSRC);
	DDX_Control(pDX, IDC_FSRC, m_FSRC);
	DDX_Control(pDX, IDC_FDST, m_FDST);
	DDX_Control(pDX, IDC_STATUS, m_status);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDVToolsDlg, CDialog)
	//{{AFX_MSG_MAP(CDVToolsDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TOOL_TAB, OnSelchangeToolTab)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_VSRC_SEL, OnVsrcSel)
	ON_BN_CLICKED(IDC_VDST_SEL, OnVdstSel)
	ON_WM_CTLCOLOR()
	ON_WM_MOVE()
	ON_BN_CLICKED(IDC_FDST_SEL, OnFdstSel)
	ON_BN_CLICKED(IDC_FSRC_SEL, OnFsrcSel)
	ON_BN_CLICKED(IDC_CONFIG, OnConfig)
	ON_BN_CLICKED(IDC_CAPTURE, OnCapture)
	ON_BN_CLICKED(IDC_RECORD, OnRecord)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_PICTURE, OnPicture)
	ON_BN_CLICKED(IDC_DVCTRL, OnDvctrl)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDC_TAB_CHANGE, IDC_TAB_CHANGE+99, OnCmdTabChange)
	ON_MESSAGE(WM_DV_TIMECHANGE, OnDVTimeChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDVToolsDlg message handlers

BOOL CDVToolsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIconSmall, FALSE);		// Set small icon
	
	GetClientRect(&m_originalRect);
	GetWindowRect(&m_lastRect);

	m_minWidth = m_lastRect.right - m_lastRect.left;
	m_minHeight = m_lastRect.bottom - m_lastRect.top;
	m_originalRects = new RECT[NControls];
	int pgNo = 0;
	CString tmp;
	RECT rc = {0,0,0,0};
	tabChangeBtns = new CButton[2];

	tmp.LoadString(IDS_TAB_VIDEO_CAPTURE);
	tabChangeBtns[pgNo].Create(tmp, WS_CHILD, rc, this, pgNo + IDC_TAB_CHANGE);
	m_toolTab.InsertItem(pgNo++, tmp);
	tmp.LoadString(IDS_TAB_VIDEO_RECORDING);
	tabChangeBtns[pgNo].Create(tmp, WS_CHILD, rc, this, pgNo + IDC_TAB_CHANGE);
	m_toolTab.InsertItem(pgNo++, tmp);
	
	{
		int i;
		for(i=0; i<NControls; i++)
		{
			GetDlgItem(ctrlProperties[i].id)->GetWindowRect(&m_originalRects[i]);
			ScreenToClient(&m_originalRects[i]);
		}
	}

	SetToolTabItemSize();

	int wx, wy, ww, wh;
	wx = AfxGetApp()->GetProfileInt("MainWindow", "X", 0);
	wy = AfxGetApp()->GetProfileInt("MainWindow", "Y", 0);
	ww = AfxGetApp()->GetProfileInt("MainWindow", "W", 0);
	wh = AfxGetApp()->GetProfileInt("MainWindow", "H", 0);

	m_video.m_DVctrl = AfxGetApp()->GetProfileInt("MainWindow", "DVControlEnabled", m_video.m_DVctrl) > 0;

	m_DVCtrl.SetCheck(m_video.m_DVctrl);

	CString wdir = AfxGetApp()->GetProfileString("MainWindow", "WorkingDirectory", ".");
	SetCurrentDirectory(wdir);

	if (ww && wh) 
		SetWindowPos(NULL, wx, wy, ww, wh, SWP_NOZORDER);
	else 
		PostMessage(WM_SYSCOMMAND, IDM_ABOUTBOX, 0);


	m_VSRCname = AfxGetApp()->GetProfileString("Capture", "DVDevice", "Microsoft DV Camera and VCR");
	m_VDSTname = AfxGetApp()->GetProfileString("Record", "DVDevice", "Microsoft DV Camera and VCR");
	m_VSRC.SetWindowText(m_VSRCname);
	m_VDST.SetWindowText(m_VDSTname);

	m_FSRC.SetWindowText(AfxGetApp()->GetProfileString("Record", "File", ""));
	m_FDST.SetWindowText(AfxGetApp()->GetProfileString("Capture", "File", ""));

	m_toolTab.SetCurSel(AfxGetApp()->GetProfileInt("MainWindow", "SelectedTool", 0));

	m_AVIPrefix = AfxGetApp()->GetProfileString("Record", "AVIPrefix", "");
	m_AVISuffix = AfxGetApp()->GetProfileString("Record", "AVISuffix", "");
	m_video.m_recordPreview = AfxGetApp()->GetProfileInt("Record", "Preview", m_video.m_recordPreview) > 0;

	m_video.m_type2AVI = AfxGetApp()->GetProfileInt("Capture", "Type2AVI", m_video.m_type2AVI) > 0;
	m_video.m_discontinuityTreshold = AfxGetApp()->GetProfileInt("Capture", "DiscontinuityTreshold", m_video.m_discontinuityTreshold);
	m_video.m_maxAVIFrames = AfxGetApp()->GetProfileInt("Capture", "MaxAVIFrames", m_video.m_maxAVIFrames);
	m_video.m_everyNth = AfxGetApp()->GetProfileInt("Capture", "EveryNth", m_video.m_everyNth);

	m_DTFormat = AfxGetApp()->GetProfileString("Capture", "DateTimeFormat", "%y-%m-%d_%H-%M");
	m_DTFormatHistory = AfxGetApp()->GetProfileString("Capture", "DateTimeFormatHistory", "%y-%m-%d_%H-%M-%S\n%Y-%m-%d_%H-%M\n%Y-%m-%d_%H-%M-%S\n%Y%m%d-%H%M%S\n%a_%H-%M-%S");
	m_nSuffixDigits = AfxGetApp()->GetProfileInt("Capture", "SuffixDigits", 2);

	char *delim = " \t\n";
	CString err;
	LPSTR arg = strtok(AfxGetApp()->m_lpCmdLine, delim);
	if (arg) {
		if (strcmp(arg,"capture")==0) {
			m_toolTab.SetCurSel(0);
			LRESULT result;
			OnSelchangeToolTab(NULL, &result);
			arg = strtok(NULL, delim);
			if (arg && strcmp(arg,"-exit")==0) {
				m_exitOnFinish = 1;
				arg = strtok(NULL, delim);
			}
			if (!arg) {
				CString tmp; tmp.LoadString(IDS_USAGE);				
				err += tmp;
			}
			else {
				int hh = 0, mi = 0, ss = 0, us = 0; 
				for(;isdigit(*arg);arg++) {
					hh = hh * 10 + (*arg - '0');	
				}
				if (*arg == 0 || *arg == '.') {
					ss = hh; hh = 0;
					goto timusec;
				}
				if (*arg == ':') arg++;
				else goto timerr;
				for(;isdigit(*arg);arg++) {
					mi = mi * 10 + (*arg - '0');	
				}
				if (*arg == 0 || *arg == '.') {
					ss = mi; mi = hh; hh = 0;
					goto timusec;
				}
				if (*arg == ':') arg++;
				else goto timerr;
				for(;isdigit(*arg);arg++) {
					ss = ss * 10 + (*arg - '0');	
				}
timusec:
				if (*arg) {
					if (*arg == '.') arg++;
					else goto timerr;

					int i = 1000000;
					for(;isdigit(*arg);arg++) {
						us += i*(*arg - '0');	
						i /= 10;
					}
					if (*arg) goto timerr;
				}
				
				arg = strtok(NULL, delim);
				if (!arg) {
timerr:
					CString tmp; tmp.LoadString(IDS_USAGE);				
					err += tmp;
				}
				else {
					REFERENCE_TIME t = (hh*60+mi)*60+ss;
					t = t * 10000000 + us;

					CString file = arg;
					arg = strtok(NULL, delim);
					if (arg) {
						CString tmp; tmp.LoadString(IDS_USAGE);				
						err += tmp;
					}
					else {
						TRY {
							m_FDST.SetWindowText(file);
							m_video.BuildCapturing(m_VSRCname);
							m_video.StartCapturing(file, m_DTFormat, m_nSuffixDigits, t);
							SetTimer(1, 200, NULL);
						}
						CATCH_ALL(e) {
							InitVideo();
							Exception2Status(e);
						}
						END_CATCH_ALL;
					}
				}
			}
		}
		else if (strcmp(arg,"record")==0) {
			m_toolTab.SetCurSel(1);
			LRESULT result;
			OnSelchangeToolTab(NULL, &result);
			arg = strtok(NULL, delim);
			if (arg && strcmp(arg,"-exit")==0) {
				m_exitOnFinish = 1;
				arg = strtok(NULL, delim);
			}
			if (!arg) {
				CString tmp; tmp.LoadString(IDS_USAGE);				
				err += tmp;
			}
			else {
				CString files;
				while (arg) {
					if (!files.IsEmpty()) files += " | ";
					files += arg;
					arg = strtok(NULL, delim);
				}
				TRY {
					m_FSRC.SetWindowText(files);
					m_video.BuildRecording(m_AVIPrefix + '|' + files + '|' + m_AVISuffix,  m_VDSTname);
					m_video.StartRecording();
					SetTimer(1, 200, NULL);
				}
				CATCH_ALL(e) {
					InitVideo();
					Exception2Status(e);
				}
				END_CATCH_ALL;
			}
		}
		else {
			CString tmp; tmp.LoadString(IDS_USAGE);				
			err += tmp;
		}
		if (!err.IsEmpty()) {
			MessageBox(err,NULL,MB_OK | MB_ICONEXCLAMATION);
			m_video.Destroy();
			CDialog::OnCancel();
		}
	}
	else {
		LRESULT result;
		OnSelchangeToolTab(NULL, &result);
	}

	return TRUE;  
}

void CDVToolsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDVToolsDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDVToolsDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CDVToolsDlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnGetMinMaxInfo(lpMMI);
	lpMMI->ptMinTrackSize.x = m_minWidth;
	lpMMI->ptMinTrackSize.y = m_minHeight;
}

void CDVToolsDlg::OnSize(UINT nType, int cx, int cy) 
{
	if (nType == SIZE_RESTORED) GetWindowRect(&m_lastRect);
	if (m_originalRect.right) 
	{
		int dx = cx - m_originalRect.right;
		int dy = cy - m_originalRect.bottom;

		int i;

		for(i=0; i<NControls; i++)
		{
			GetDlgItem(ctrlProperties[i].id)->MoveWindow(
				(ctrlProperties[i].dx*dx)/100+m_originalRects[i].left,
				(ctrlProperties[i].dy*dy)/100+m_originalRects[i].top,
				((ctrlProperties[i].dw-ctrlProperties[i].dx)*dx)/100+m_originalRects[i].right-m_originalRects[i].left,
				((ctrlProperties[i].dh-ctrlProperties[i].dy)*dy)/100+m_originalRects[i].bottom-m_originalRects[i].top,
				false);
		}

		SetToolTabItemSize();
		
		InvalidateRect(NULL);
		UpdateWindow();
	}
}


void CDVToolsDlg::OnMove(int x, int y) 
{
	CDialog::OnMove(x, y);
	
	if (!IsIconic() && !IsZoomed())	GetWindowRect(&m_lastRect);
}

void CDVToolsDlg::OnSelchangeToolTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int sel = m_toolTab.GetCurSel();

	int i;

	for(i=0; i<NControls; i++)
	{
		GetDlgItem(ctrlProperties[i].id)->ShowWindow(((1<<sel) & ctrlProperties[i].tabMask ) ? SW_SHOW : SW_HIDE);
	}

	UpdateWindow();

	TRY {
		InitVideo();
	}
	CATCH_ALL(e) {
		TCHAR buf[1024];
		e->GetErrorMessage(buf, sizeof buf);
		MessageBox(buf, NULL, MB_OK | MB_ICONERROR);
	}
	END_CATCH_ALL;

	*pResult = 0;
}


void CDVToolsDlg::SetToolTabItemSize()
{
	RECT rc;
	CSize sz;
	m_toolTab.GetItemRect(0, &rc);
	sz.cy = rc.bottom - rc.top;
	m_toolTab.GetWindowRect(&rc);
	sz.cx = (rc.right - rc.left) * 2 / (m_toolTab.GetItemCount()*2 + 1);
	m_toolTab.SetItemSize(sz);
}

void CDVToolsDlg::OnCmdTabChange(UINT nID)
{
	int newTab = nID - IDC_TAB_CHANGE;
	if (m_toolTab.IsWindowEnabled() && m_toolTab.GetCurSel() != newTab) {
		m_toolTab.SetCurSel(newTab);
		LRESULT result;
		OnSelchangeToolTab(NULL, &result);
	}
}

HBRUSH CDVToolsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->GetDlgCtrlID() == IDC_VIDEO) {
		hbr = (HBRUSH)GetStockObject(BLACK_BRUSH);
	}
	
	return hbr;
}

void CDVToolsDlg::OnCancel() 
{
	InitVideo();
}

void CDVToolsDlg::OnClose() 
{
	m_video.Destroy();
	 
	CString tmp;

	AfxGetApp()->WriteProfileInt("MainWindow", "X", m_lastRect.left);
	AfxGetApp()->WriteProfileInt("MainWindow", "Y", m_lastRect.top);
	AfxGetApp()->WriteProfileInt("MainWindow", "W", m_lastRect.right - m_lastRect.left);
	AfxGetApp()->WriteProfileInt("MainWindow", "H", m_lastRect.bottom - m_lastRect.top);
	AfxGetApp()->WriteProfileInt("MainWindow", "DVControlEnabled", m_video.m_DVctrl);
	AfxGetApp()->WriteProfileInt("MainWindow", "SelectedTool", m_toolTab.GetCurSel());
	char wdir[1024];
	GetCurrentDirectory(sizeof wdir, wdir);
	AfxGetApp()->WriteProfileString("MainWindow", "WorkingDirectory", wdir);
	AfxGetApp()->WriteProfileString("Capture", "DVDevice", m_VSRCname);
	AfxGetApp()->WriteProfileString("Record", "DVDevice", m_VDSTname);
	m_FSRC.GetWindowText(tmp);
	AfxGetApp()->WriteProfileString("Record", "File", tmp);
	m_FDST.GetWindowText(tmp);
	AfxGetApp()->WriteProfileString("Capture", "File", tmp);

	AfxGetApp()->WriteProfileInt("Capture", "Type2AVI", m_video.m_type2AVI);
	AfxGetApp()->WriteProfileInt("Capture", "DiscontinuityTreshold", m_video.m_discontinuityTreshold);
	AfxGetApp()->WriteProfileInt("Capture", "MaxAVIFrames", m_video.m_maxAVIFrames);
	AfxGetApp()->WriteProfileInt("Capture", "EveryNth", m_video.m_everyNth);

	AfxGetApp()->WriteProfileString("Capture", "DateTimeFormat", m_DTFormat);
	AfxGetApp()->WriteProfileString("Capture", "DateTimeFormatHistory", m_DTFormatHistory);
	AfxGetApp()->WriteProfileInt("Capture", "SuffixDigits", m_nSuffixDigits);


	AfxGetApp()->WriteProfileString("Record", "AVIPrefix", m_AVIPrefix);
	AfxGetApp()->WriteProfileString("Record", "AVISuffix", m_AVISuffix);
	AfxGetApp()->WriteProfileInt("Record", "Preview", m_video.m_recordPreview);

	CDialog::OnCancel();
}

void CDVToolsDlg::OnOK() 
{
}

void CDVToolsDlg::Exception2Status(CException *e)
{
	TCHAR buf[1024];
	e->GetErrorMessage(buf, sizeof buf);
	CString tmp;
	tmp.Format("Error: %s", buf);
	m_status.SetWindowText(tmp);
}

void CDVToolsDlg::OnVsrcSel() 
{
	CArray<CString,CString&> list;
	GetVideoSrcList(list);

	BOOL doInit;

	CVideoDeviceSel devSel(list, m_VSRCname);
	if ((doInit = (devSel.DoModal() == IDOK))) {
		m_VSRCname = list[devSel.GetSelection()];
		m_VSRC.SetWindowText(m_VSRCname);
	}

	if (doInit) InitVideo();
}

void CDVToolsDlg::OnVdstSel() 
{
	CArray<CString,CString&> list;
	GetVideoDstList(list);

	BOOL doInit;

	CVideoDeviceSel devSel(list, m_VDSTname);
	if ((doInit = (devSel.DoModal() == IDOK))) {
		m_VDSTname = list[devSel.GetSelection()];
		m_VDST.SetWindowText(m_VDSTname);
	}

	if (doInit) InitVideo();
}

void CDVToolsDlg::InitVideo()
{
	m_exitOnFinish = 0;

	KillTimer(1);
	int sel = m_toolTab.GetCurSel();
	m_status.SetWindowText("Initializing...");
	m_status2.SetWindowText("");
	m_status3.SetWindowText("");
	m_counter.SetWindowText("");

	switch(sel) {
	case 0:
		TRY {
			CString filename;
			m_video.BuildCapturing(m_VSRCname);
			SetTimer(1, 200, NULL);
		}
		CATCH_ALL(e) {
			Exception2Status(e);
		}
		END_CATCH_ALL;
		break;
	case 1:
		m_video.Destroy();
		m_status.SetWindowText("Select file and press <Record>");
		break;
	}
}


void CDVToolsDlg::OnFsrcSel() 
{
	SelectFile(TRUE, &m_FSRC);
}


void CDVToolsDlg::OnFdstSel() 
{
	SelectFile(FALSE, &m_FDST);
	CString filename;
	m_FDST.GetWindowText(filename);
	CaptureFilenameExtractBase(filename);
	m_FDST.SetWindowText(filename);
}

void CDVToolsDlg::OnCapture() 
{
	if (m_video.GetState() == CDV::CapturePaused) {
		TRY {
			CString filename;
			m_FDST.GetWindowText(filename);
			m_video.StartCapturing(filename, m_DTFormat, m_nSuffixDigits);
		}
		CATCH_ALL(e) {
			InitVideo();
			Exception2Status(e);
		}
		END_CATCH_ALL;
	}
	else if (m_video.GetState() == CDV::Capturing) {
		TRY {
			m_video.StopCapturing();
		}
		CATCH_ALL(e) {
			InitVideo();
			Exception2Status(e);
		}
		END_CATCH_ALL;
	}
	else 
		InitVideo();
}

void CDVToolsDlg::OnRecord() 
{
	if (m_video.GetState() == CDV::RecordPaused) {
		TRY {
			m_video.StartRecording();
		}
		CATCH_ALL(e) {
			InitVideo();
			Exception2Status(e);
		}
		END_CATCH_ALL;
	}
	else if (m_video.GetState() == CDV::Recording) {
		TRY {
			m_video.StopRecording();
		}
		CATCH_ALL(e) {
			InitVideo();
			Exception2Status(e);
		}
		END_CATCH_ALL;
	}
	else {
		TRY {
			CString filename;
			m_FSRC.GetWindowText(filename);
			m_video.BuildRecording(m_AVIPrefix + '|' +filename + '|' + m_AVISuffix,  m_VDSTname);
			SetTimer(1, 200, NULL);
		}
		CATCH_ALL(e) {
			InitVideo();
			Exception2Status(e);
		}
		END_CATCH_ALL;
	}
}

void CDVToolsDlg::OnConfig() 
{
	int sel = m_toolTab.GetCurSel();

	CCaptureCfg captureCfg;
	CRecordCfg recordCfg;

	captureCfg.m_type12 = m_video.m_type2AVI ? 1 : 0;
	captureCfg.m_discontinuityTreshold = m_video.m_discontinuityTreshold;
	captureCfg.m_maxAVIFrames = m_video.m_maxAVIFrames;
	captureCfg.m_everyNth = m_video.m_everyNth;
	captureCfg.m_dtformat = m_DTFormat;
	captureCfg.m_dtformathistory = m_DTFormatHistory;
	captureCfg.m_ndigits = m_nSuffixDigits;

	recordCfg.m_aviPrefix = m_AVIPrefix;
	recordCfg.m_aviSuffix = m_AVISuffix;
	recordCfg.m_recordPreview = m_video.m_recordPreview;

	CPropertySheet cfgDlg(IDS_CONFIG_DLG);

	cfgDlg.m_psh.dwFlags |= PSH_NOAPPLYNOW;

	cfgDlg.AddPage(&captureCfg);
	cfgDlg.AddPage(&recordCfg);
	cfgDlg.SetActivePage(m_toolTab.GetCurSel());

	if (cfgDlg.DoModal() == IDOK) {
		m_video.m_type2AVI  = captureCfg.m_type12 == 1;
		m_video.m_discontinuityTreshold = captureCfg.m_discontinuityTreshold;
		m_video.m_maxAVIFrames = captureCfg.m_maxAVIFrames;
		m_video.m_everyNth = captureCfg.m_everyNth;
		m_DTFormat = captureCfg.m_dtformat;
		m_DTFormatHistory = captureCfg.m_dtformathistory;
		m_nSuffixDigits = captureCfg.m_ndigits;

		m_AVIPrefix = recordCfg.m_aviPrefix;
		m_AVISuffix = recordCfg.m_aviSuffix;
		m_video.m_recordPreview = recordCfg.m_recordPreview > 0;
//		InitVideo();
	}
}


void CDVToolsDlg::OnTimer(UINT nIDEvent) 
{
	SetThreadExecutionState(ES_DISPLAY_REQUIRED);

	if (m_video.GetState() == CDV::Finished && m_exitOnFinish) {
		OnClose();
	}

	CString txt, txt2, txt3;

	switch (m_video.GetState()) {
	case CDV::Capturing: {
		txt += "Capturing...  Press <Capture> for pause.";
		CString dropped;
		dropped.Format(" (%d frames dropped)", m_video.GetDropped());
		txt += dropped;
		break;
	}
	case CDV::CapturePaused: txt += "Paused... Press <Capture> for Capturing.";
		break;
	case CDV::Recording: txt += "Recording...  Press <Record> for pause.";
		break;
	case CDV::RecordPaused: txt += "Paused... Press <Record> for recording.";
		break;
	case CDV::Finished: txt += "Finished.";
		break;
	}

	
	switch (m_video.GetState()) {
	case CDV::Capturing:
	case CDV::CapturePaused:
	case CDV::Recording:
	case CDV::RecordPaused:
	case CDV::Finished:
		REFERENCE_TIME t = m_video.GetTime();
		if (t >= 0) {
			t /= 1000000;
			int ss = (int)(t % 10); t /= 10;
			int s = (int)(t % 60); t /= 60;
			int m = (int)(t % 60); t /= 60;
			txt2.Format("%d:%02d:%02d.%01d", (int)t, m, s, ss);
		}
	}

	switch (m_video.GetState()) {
	case CDV::Capturing:
	case CDV::Recording:
	case CDV::RecordPaused:
		txt3.Format(" Q:%i", m_video.GetQueueLoad());
	}

	CString tmp;
	m_status.GetWindowText(tmp);
	if (tmp != txt) m_status.SetWindowText(txt);
	m_counter.GetWindowText(tmp);
	if (tmp != txt2) m_counter.SetWindowText(txt2);
	m_status3.GetWindowText(tmp);
	if (tmp != txt3) m_status3.SetWindowText(txt3);

	CDialog::OnTimer(nIDEvent);
}

LRESULT CDVToolsDlg::OnDVTimeChange(WPARAM, LPARAM lParam)
{
	char buf[100] = "";
	if (lParam > 0) {
		strftime(buf, sizeof buf, "%d.%m.'%y %H:%M:%S", localtime(&lParam));
	}
	m_status2.SetWindowText(buf);
	return 0;
}

void CDVToolsDlg::OnPicture() 
{
	OnSysCommand(IDM_ABOUTBOX, 0);
}

void CDVToolsDlg::OnDvctrl() 
{
	m_video.m_DVctrl = m_DVCtrl.GetCheck() > 0;
}
