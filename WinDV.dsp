# Microsoft Developer Studio Project File - Name="WinDV" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=WinDV - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "WinDV.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "WinDV.mak" CFG="WinDV - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "WinDV - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "WinDV - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "WinDV - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "C:\Program Files\Microsoft SDK\include" /I "C:\Program Files\Microsoft SDK\Samples\Multimedia\DirectShow\BaseClasses" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_WIN32_DCOM" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x405 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 strmbase.lib msvcrt.lib quartz.lib winmm.lib kernel32.lib advapi32.lib version.lib largeint.lib user32.lib gdi32.lib comctl32.lib ole32.lib olepro32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libcmt" /libpath:"C:\Program Files\Microsoft SDK\Samples\Multimedia\DirectShow\BaseClasses\Release"
# SUBTRACT LINK32 /pdb:none /debug

!ELSEIF  "$(CFG)" == "WinDV - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "C:\Program Files\Microsoft SDK\include" /I "C:\Program Files\Microsoft SDK\Samples\Multimedia\DirectShow\BaseClasses" /D "_DEBUG" /D "DEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_WIN32_DCOM" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x405 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 strmbasd.lib msvcrtd.lib quartz.lib winmm.lib kernel32.lib advapi32.lib version.lib largeint.lib user32.lib gdi32.lib comctl32.lib ole32.lib olepro32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcmtd" /pdbtype:sept /libpath:"C:\Program Files\Microsoft SDK\Samples\Multimedia\DirectShow\BaseClasses\Debug"
# SUBTRACT LINK32 /pdb:none /nodefaultlib

!ENDIF 

# Begin Target

# Name "WinDV - Win32 Release"
# Name "WinDV - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CaptureCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DropFilesEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\DShow.cpp
# End Source File
# Begin Source File

SOURCE=.\DV.cpp
# End Source File
# Begin Source File

SOURCE=.\DVToolsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RecordCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\ToolTab.cpp
# End Source File
# Begin Source File

SOURCE=.\VideoDeviceSel.cpp
# End Source File
# Begin Source File

SOURCE=.\WinDV.cpp
# End Source File
# Begin Source File

SOURCE=.\WinDV.rc
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CaptureCfg.h
# End Source File
# Begin Source File

SOURCE=.\DropFilesEdit.h
# End Source File
# Begin Source File

SOURCE=.\DShow.h
# End Source File
# Begin Source File

SOURCE=.\DV.h
# End Source File
# Begin Source File

SOURCE=.\DVToolsDlg.h
# End Source File
# Begin Source File

SOURCE=.\RecordCfg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\ToolTab.h
# End Source File
# Begin Source File

SOURCE=.\VideoDeviceSel.h
# End Source File
# Begin Source File

SOURCE=.\WinDV.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\DVTool.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\WinDV.ico
# End Source File
# Begin Source File

SOURCE=.\res\WinDV.rc2
# End Source File
# Begin Source File

SOURCE=.\res\WinDVlogo.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\WinDV.exe.manifest
# End Source File
# End Target
# End Project
