; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CToolTab
LastTemplate=CTabCtrl
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "WinDV.h"

ClassCount=9
Class1=CWinDVApp
Class2=CDVToolsDlg
Class3=CAboutDlg
Class4=CDropFilesEdit
Class5=CDV
Class6=CVideoDeviceSel


ResourceCount=13
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_DVTOOLS_DIALOG
Resource4=IDD_ABOUTBOX (English (U.S.))
Resource5=IDD_DVFILTCFG (English (U.S.))
Class6=CVideoDeviceSel
Resource6=IDD_DVTOOLS_DIALOG (English (U.S.))
Resource7=IDD_COMPRCFG (English (U.S.))
Resource8=IDD_VIDEODEVICESEL (English (U.S.))
Resource9=IDD_CAPTURE_CONFIG (English (U.S.))
Class7=CRecordCfg
Class8=CCaptureCfg
Class9=CToolTab
Resource10=IDD_RECORD_CONFIG (English (U.S.))
Resource11=IDD_CAPTURE_CONFIG
Resource12=IDD_VIDEODEVICESEL
Resource13=IDD_RECORD_CONFIG

[CLS:CWinDVApp]
Type=0
HeaderFile=WinDV.h
ImplementationFile=WinDV.cpp
Filter=N
LastObject=CWinDVApp

[CLS:CDVToolsDlg]
Type=0
HeaderFile=DVToolsDlg.h
ImplementationFile=DVToolsDlg.cpp
Filter=D
LastObject=CDVToolsDlg
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=DVToolsDlg.h
ImplementationFile=DVToolsDlg.cpp
Filter=D
LastObject=CAboutDlg
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CDropFilesEdit]
Type=0
HeaderFile=DropFilesEdit.h
ImplementationFile=DropFilesEdit.cpp
BaseClass=CEdit
Filter=W
VirtualFilter=WC
LastObject=CDropFilesEdit

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=7
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308482
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EMAIL,static,1342308608
Control6=IDC_URL,static,1342308608
Control7=IDOK,static,1342374147

[DLG:IDD_DVTOOLS_DIALOG]
Type=1
Class=CDVToolsDlg
ControlCount=24
Control1=IDC_PICTURE,static,1342308611
Control2=IDC_VSRC_L,static,1342308354
Control3=IDC_VSRC,static,1342308865
Control4=IDC_VSRC_SEL,button,1342373888
Control5=IDC_FSRC_L,static,1342308354
Control6=IDC_FSRC,edit,1350762624
Control7=IDC_FSRC_SEL,button,1342373888
Control8=IDC_CONFIG,button,1342373888
Control9=IDC_FDST_L,static,1342308354
Control10=IDC_FDST,edit,1350762624
Control11=IDC_FDST_SEL,button,1342373888
Control12=IDC_VDST_L,static,1342308354
Control13=IDC_VDST,static,1342308865
Control14=IDC_VDST_SEL,button,1342373888
Control15=IDC_DVCTRL,button,1342373891
Control16=IDC_CAPTURE,button,1342373888
Control17=IDC_RECORD,button,1342373888
Control18=IDCANCEL,button,1342373888
Control19=IDC_COUNTER,static,1342308353
Control20=IDC_STATUS,static,1342308993
Control21=IDC_STATUS2,static,1342308993
Control22=IDC_STATUS3,static,1342308993
Control23=IDC_TOOL_TAB,SysTabControl32,1342350336
Control24=IDC_VIDEO,static,1342308865

[DLG:IDD_DVTOOLS_DIALOG (English (U.S.))]
Type=1
Class=CDVToolsDlg
ControlCount=24
Control1=IDC_PICTURE,static,1342308611
Control2=IDC_VSRC_L,static,1342308354
Control3=IDC_VSRC,static,1342308865
Control4=IDC_VSRC_SEL,button,1342373888
Control5=IDC_FSRC_L,static,1342308354
Control6=IDC_FSRC,edit,1350762624
Control7=IDC_FSRC_SEL,button,1342373888
Control8=IDC_CONFIG,button,1342373888
Control9=IDC_FDST_L,static,1342308354
Control10=IDC_FDST,edit,1350762624
Control11=IDC_FDST_SEL,button,1342373888
Control12=IDC_VDST_L,static,1342308354
Control13=IDC_VDST,static,1342308865
Control14=IDC_VDST_SEL,button,1342373888
Control15=IDC_DVCTRL,button,1342373891
Control16=IDC_CAPTURE,button,1342373888
Control17=IDC_RECORD,button,1342373888
Control18=IDCANCEL,button,1342373888
Control19=IDC_COUNTER,static,1342308353
Control20=IDC_STATUS,static,1342308993
Control21=IDC_STATUS2,static,1342308993
Control22=IDC_STATUS3,static,1342308993
Control23=IDC_TOOL_TAB,SysTabControl32,1342350336
Control24=IDC_VIDEO,static,1342308865

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=7
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308482
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EMAIL,static,1342308608
Control6=IDC_URL,static,1342308608
Control7=IDOK,static,1342374147

[CLS:CVideoDeviceSel]
Type=0
HeaderFile=VideoDeviceSel.h
ImplementationFile=VideoDeviceSel.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CVideoDeviceSel

[CLS:CDV]
Type=0
HeaderFile=DShow.h
ImplementationFile=DShow.cpp
BaseClass=CStatic
Filter=W
VirtualFilter=WC
LastObject=CDV

[DLG:IDD_DVFILTCFG (English (U.S.))]
Type=1
Class=?
ControlCount=18
Control1=IDC_FILTER_ENABLED,button,1342373891
Control2=IDC_STATIC,button,1342308359
Control3=IDC_DEINTERLACE0,button,1342373897
Control4=IDC_DEINTERLACE1,button,1342242825
Control5=IDC_DEINTERLACE2,button,1342242825
Control6=IDC_DEINTERLACE3,button,1342242825
Control7=IDC_STATIC,button,1342308359
Control8=IDC_ROTATE0,button,1342374409
Control9=IDC_ROTATE90,button,1342243337
Control10=IDC_ROTATE180,button,1342243337
Control11=IDC_ROTATE270,button,1342243337
Control12=IDC_STATIC,static,1342308352
Control13=IDC_VZOOM,edit,1350770818
Control14=IDC_STATIC,static,1342308352
Control15=IDC_MIRROR,button,1476591651
Control16=IDC_STATIC,static,1342308352
Control17=IDC_GAMMA,edit,1350770818
Control18=IDC_STATIC,static,1342308352

[DLG:IDD_VIDEODEVICESEL (English (U.S.))]
Type=1
Class=CVideoDeviceSel
ControlCount=3
Control1=IDC_DEVLIST,listbox,1352859905
Control2=IDOK,button,1342373889
Control3=IDCANCEL,button,1342242816

[DLG:IDD_COMPRCFG (English (U.S.))]
Type=1
Class=?
ControlCount=13
Control1=IDC_COMPR_ENABLED,button,1342373891
Control2=IDC_STATIC,static,1342308352
Control3=IDC_SKIP,edit,1350770818
Control4=IDC_SKIPSPIN,msctls_updown32,1342177462
Control5=IDC_STATIC,button,1342308359
Control6=IDC_VCOMPRESSOR,combobox,1344471299
Control7=IDC_VCONFIG,button,1342242816
Control8=IDC_FULLSIZE,button,1342373897
Control9=IDC_HALFSIZE,button,1342177289
Control10=IDC_STATIC,button,1342308359
Control11=IDC_ACOMPRESOR,combobox,1344471299
Control12=IDC_48000,button,1342373897
Control13=IDC_44100,button,1342177289

[DLG:IDD_CAPTURE_CONFIG (English (U.S.))]
Type=1
Class=CCaptureCfg
ControlCount=14
Control1=IDC_TYPE_1,button,1342373897
Control2=IDC_TYPE_2,button,1342177289
Control3=IDC_STATIC,static,1342308354
Control4=IDC_DISCONTINUITY_TRESHOLD,edit,1350639746
Control5=IDC_STATIC,static,1342308354
Control6=IDC_MAX_FRAMES,edit,1350639746
Control7=IDC_STATIC,static,1342308354
Control8=IDC_EVERY_NTH,edit,1350639746
Control9=IDC_STATIC,static,1342308352
Control10=IDC_FEXAMPLE,static,1342308480
Control11=IDC_STATIC,static,1342308354
Control12=IDC_DTFORMAT,combobox,1344340034
Control13=IDC_STATIC,static,1342308354
Control14=IDC_NDIGITS,combobox,1344339971

[DLG:IDD_RECORD_CONFIG (English (U.S.))]
Type=1
Class=CRecordCfg
ControlCount=7
Control1=IDC_STATIC,static,1342308354
Control2=IDC_AVI_PREFIX,edit,1350631552
Control3=IDC_PREFIX_SEL,button,1342373888
Control4=IDC_STATIC,static,1342308354
Control5=IDC_AVI_SUFFIX,edit,1350631552
Control6=IDC_SUFFIX_SEL,button,1342373888
Control7=IDC_RECORDPREVIEW,button,1342373923

[CLS:CRecordCfg]
Type=0
HeaderFile=RecordCfg.h
ImplementationFile=RecordCfg.cpp
BaseClass=CPropertyPage
Filter=D
LastObject=CRecordCfg
VirtualFilter=idWC

[CLS:CCaptureCfg]
Type=0
HeaderFile=CaptureCfg.h
ImplementationFile=CaptureCfg.cpp
BaseClass=CPropertyPage
Filter=D
LastObject=CCaptureCfg
VirtualFilter=idWC

[CLS:CToolTab]
Type=0
HeaderFile=ToolTab.h
ImplementationFile=ToolTab.cpp
BaseClass=CTabCtrl
Filter=W
VirtualFilter=UWC
LastObject=CToolTab

[DLG:IDD_VIDEODEVICESEL]
Type=1
Class=?
ControlCount=3
Control1=IDC_DEVLIST,listbox,1352859905
Control2=IDOK,button,1342373889
Control3=IDCANCEL,button,1342242816

[DLG:IDD_CAPTURE_CONFIG]
Type=1
Class=?
ControlCount=14
Control1=IDC_TYPE_1,button,1342373897
Control2=IDC_TYPE_2,button,1342177289
Control3=IDC_STATIC,static,1342308354
Control4=IDC_DISCONTINUITY_TRESHOLD,edit,1350639746
Control5=IDC_STATIC,static,1342308354
Control6=IDC_MAX_FRAMES,edit,1350639746
Control7=IDC_STATIC,static,1342308354
Control8=IDC_EVERY_NTH,edit,1350639746
Control9=IDC_STATIC,static,1342308352
Control10=IDC_FEXAMPLE,static,1342308480
Control11=IDC_STATIC,static,1342308354
Control12=IDC_DTFORMAT,combobox,1344340034
Control13=IDC_STATIC,static,1342308354
Control14=IDC_NDIGITS,combobox,1344339971

[DLG:IDD_RECORD_CONFIG]
Type=1
Class=?
ControlCount=7
Control1=IDC_STATIC,static,1342308354
Control2=IDC_AVI_PREFIX,edit,1350631552
Control3=IDC_PREFIX_SEL,button,1342373888
Control4=IDC_STATIC,static,1342308354
Control5=IDC_AVI_SUFFIX,edit,1350631552
Control6=IDC_SUFFIX_SEL,button,1342373888
Control7=IDC_RECORDPREVIEW,button,1342373923

