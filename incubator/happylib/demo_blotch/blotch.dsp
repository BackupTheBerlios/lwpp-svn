# Microsoft Developer Studio Project File - Name="blotch" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=blotch - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "blotch.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "blotch.mak" CFG="blotch - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "blotch - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "blotch - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "blotch - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BLOTCH_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BLOTCH_EXPORTS" /D "_WIN32" /D "_X86_" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"C:\LightWave\Plugins\Texture\blotch_demo.p"

!ELSEIF  "$(CFG)" == "blotch - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BLOTCH_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BLOTCH_EXPORTS" /D "_WIN32" /D "_X86_" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"C:\LightWave\Programs\Plugins\Texture\blotch_demo.p" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "blotch - Win32 Release"
# Name "blotch - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\blotch.cpp
# End Source File
# Begin Source File

SOURCE=..\LWPPlugin.cpp
# End Source File
# Begin Source File

SOURCE=..\LWPRender.cpp
# End Source File
# Begin Source File

SOURCE=..\LWPSerialization.cpp
# End Source File
# Begin Source File

SOURCE=..\LWPShader.cpp
# End Source File
# Begin Source File

SOURCE=..\..\lwsdk6\source\serv.def
# End Source File
# Begin Source File

SOURCE=..\..\lwsdk6\source\servmain.c
# End Source File
# Begin Source File

SOURCE=..\..\lwsdk6\source\server.lib
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\HLDefs.h
# End Source File
# Begin Source File

SOURCE=..\HLMatrix.h
# End Source File
# Begin Source File

SOURCE=..\HLMatrix3x3.h
# End Source File
# Begin Source File

SOURCE=..\HLMatrix4x4.h
# End Source File
# Begin Source File

SOURCE=..\HLMutex.h
# End Source File
# Begin Source File

SOURCE=..\HLPoint.h
# End Source File
# Begin Source File

SOURCE=..\HLString.h
# End Source File
# Begin Source File

SOURCE=..\HLTemplates.h
# End Source File
# Begin Source File

SOURCE=..\HLUtils.h
# End Source File
# Begin Source File

SOURCE=..\HLWeefloat.h
# End Source File
# Begin Source File

SOURCE=..\..\lwsdk6\include\lwdialog.h
# End Source File
# Begin Source File

SOURCE=..\..\lwsdk6\include\lwdyna.h
# End Source File
# Begin Source File

SOURCE=..\..\lwsdk6\include\lwenvel.h
# End Source File
# Begin Source File

SOURCE=..\..\lwsdk6\include\lwglobsrv.h
# End Source File
# Begin Source File

SOURCE=..\..\lwsdk6\include\lwhandler.h
# End Source File
# Begin Source File

SOURCE=..\..\lwsdk6\include\lwhost.h
# End Source File
# Begin Source File

SOURCE=..\..\lwsdk6\include\lwimage.h
# End Source File
# Begin Source File

SOURCE=..\..\lwsdk6\include\lwimageio.h
# End Source File
# Begin Source File

SOURCE=..\..\lwsdk6\include\lwio.h
# End Source File
# Begin Source File

SOURCE=..\..\lwsdk6\include\lwmeshes.h
# End Source File
# Begin Source File

SOURCE=..\..\lwsdk6\include\lwmodule.h
# End Source File
# Begin Source File

SOURCE=..\..\lwsdk6\include\lwmonitor.h
# End Source File
# Begin Source File

SOURCE=..\..\lwsdk6\include\lwmtutil.h
# End Source File
# Begin Source File

SOURCE=..\..\lwsdk6\include\lwpanel.h
# End Source File
# Begin Source File

SOURCE=..\LWPIO.h
# End Source File
# Begin Source File

SOURCE=..\LWPPlugin.h
# End Source File
# Begin Source File

SOURCE=..\LWPRender.h
# End Source File
# Begin Source File

SOURCE=..\..\lwsdk6\include\lwpreview.h
# End Source File
# Begin Source File

SOURCE=..\..\lwsdk6\include\lwprtcl.h
# End Source File
# Begin Source File

SOURCE=..\LWPSerialization.h
# End Source File
# Begin Source File

SOURCE=..\LWPShader.h
# End Source File
# Begin Source File

SOURCE=..\LWPTextureUtils.h
# End Source File
# Begin Source File

SOURCE=..\LWPUtils.h
# End Source File
# Begin Source File

SOURCE=..\..\lwsdk6\include\lwrender.h
# End Source File
# Begin Source File

SOURCE=..\..\lwsdk6\include\lwserver.h
# End Source File
# Begin Source File

SOURCE=..\..\lwsdk6\include\lwshader.h
# End Source File
# Begin Source File

SOURCE=..\..\lwsdk6\include\lwshelf.h
# End Source File
# Begin Source File

SOURCE=..\..\lwsdk6\include\lwsurf.h
# End Source File
# Begin Source File

SOURCE=..\..\lwsdk6\include\lwsurfed.h
# End Source File
# Begin Source File

SOURCE=..\..\lwsdk6\include\lwtxtr.h
# End Source File
# Begin Source File

SOURCE=..\..\lwsdk6\include\lwtxtred.h
# End Source File
# Begin Source File

SOURCE=..\..\lwsdk6\include\lwtypes.h
# End Source File
# Begin Source File

SOURCE=..\..\lwsdk6\include\lwvparm.h
# End Source File
# Begin Source File

SOURCE=..\..\lwsdk6\include\lwxpanel.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
