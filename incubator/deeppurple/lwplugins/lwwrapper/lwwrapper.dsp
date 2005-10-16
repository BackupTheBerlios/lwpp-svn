# Microsoft Developer Studio Project File - Name="lwwrapper" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=lwwrapper - Win32 Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "lwwrapper.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "lwwrapper.mak" CFG="lwwrapper - Win32 Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "lwwrapper - Win32 Release" (basierend auf  "Win32 (x86) Static Library")
!MESSAGE "lwwrapper - Win32 Debug" (basierend auf  "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=xicl6.exe
RSC=rc.exe

!IF  "$(CFG)" == "lwwrapper - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../lwinclude" /I "../../patterns/C++Patterns" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "_WIN32" /D "_X86_" /YX /FD /c
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=xilink6.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Release\lwwrappervc6.lib"

!ELSEIF  "$(CFG)" == "lwwrapper - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../lwinclude" /I "../../patterns/C++Patterns" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "_WIN32" /D "_X86_" /YX /FD /GZ /c
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=xilink6.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug\lwwrappervc6.lib"

!ENDIF 

# Begin Target

# Name "lwwrapper - Win32 Release"
# Name "lwwrapper - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\stdafx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\stdafx.h
# End Source File
# End Group
# Begin Group "LWCommon"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\LWcommon\FileLib.cpp
# End Source File
# Begin Source File

SOURCE=.\LWcommon\FileLib.h
# End Source File
# Begin Source File

SOURCE=.\LWcommon\Lightwave3D.h
# End Source File
# Begin Source File

SOURCE=.\LWcommon\LWChannel.cpp
# End Source File
# Begin Source File

SOURCE=.\LWcommon\LWChannel.h
# End Source File
# Begin Source File

SOURCE=.\LWcommon\LWEnvelope.cpp
# End Source File
# Begin Source File

SOURCE=.\LWcommon\LWEnvelope.h
# End Source File
# Begin Source File

SOURCE=.\LWcommon\LWlib.cpp
# End Source File
# Begin Source File

SOURCE=.\LWcommon\LWlib.h
# End Source File
# Begin Source File

SOURCE=.\LWcommon\LWLoad.cpp
# End Source File
# Begin Source File

SOURCE=.\LWcommon\LWLoad.h
# End Source File
# Begin Source File

SOURCE=.\LWcommon\LWSave.cpp
# End Source File
# Begin Source File

SOURCE=.\LWcommon\LWSave.h
# End Source File
# Begin Source File

SOURCE=.\LWcommon\LWSceneGraph.cpp
# End Source File
# Begin Source File

SOURCE=.\LWcommon\LWSceneGraph.h
# End Source File
# Begin Source File

SOURCE=.\LWcommon\ProgressMonitor.cpp
# End Source File
# Begin Source File

SOURCE=.\LWcommon\ProgressMonitor.h
# End Source File
# Begin Source File

SOURCE=.\LWcommon\SceneItem.cpp
# End Source File
# Begin Source File

SOURCE=.\LWcommon\SceneItem.h
# End Source File
# End Group
# Begin Group "Panel"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Panel\Control.cpp
# End Source File
# Begin Source File

SOURCE=.\Panel\Control.h
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlAngle.cpp
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlAngle.h
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlBoolButton.cpp
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlBoolButton.h
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlBoolChoice.cpp
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlBoolChoice.h
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlBorder.cpp
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlBorder.h
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlButton.cpp
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlButton.h
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlCanvas.cpp
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlCanvas.h
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlCheckBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlCheckBox.h
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlChoices.cpp
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlChoices.h
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlColor.cpp
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlColor.h
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlCustPopUp.cpp
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlCustPopUp.h
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlDistance.cpp
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlDistance.h
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlDouble.cpp
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlDouble.h
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlFile.cpp
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlFile.h
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlFileButton.cpp
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlFileButton.h
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlInteger.cpp
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlInteger.h
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlListBox.h
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlMultiListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlMultiListBox.h
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlOpenGL.cpp
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlOpenGL.h
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlPercent.cpp
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlPercent.h
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlPopUp.cpp
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlPopUp.h
# End Source File
# Begin Source File

SOURCE=.\Panel\Controls.h
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlSlider.cpp
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlSlider.h
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlString.cpp
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlString.h
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlTab.cpp
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlTab.h
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlText.cpp
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlText.h
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlXColor.cpp
# End Source File
# Begin Source File

SOURCE=.\Panel\ControlXColor.h
# End Source File
# Begin Source File

SOURCE=.\Panel\DoubleMinislider.cpp
# End Source File
# Begin Source File

SOURCE=.\Panel\DoubleMinislider.h
# End Source File
# Begin Source File

SOURCE=.\Panel\Panel.cpp
# End Source File
# Begin Source File

SOURCE=.\Panel\Panel.h
# End Source File
# End Group
# Begin Group "XPanel"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\XPanel\XBoolean.cpp
# End Source File
# Begin Source File

SOURCE=.\XPanel\XBoolean.h
# End Source File
# Begin Source File

SOURCE=.\XPanel\XChoice.cpp
# End Source File
# Begin Source File

SOURCE=.\XPanel\XChoice.h
# End Source File
# Begin Source File

SOURCE=.\XPanel\XColorVparam.cpp
# End Source File
# Begin Source File

SOURCE=.\XPanel\XColorVparam.h
# End Source File
# Begin Source File

SOURCE=.\XPanel\XControl.cpp
# End Source File
# Begin Source File

SOURCE=.\XPanel\XControl.h
# End Source File
# Begin Source File

SOURCE=.\XPanel\XControlButton.cpp
# End Source File
# Begin Source File

SOURCE=.\XPanel\XControlButton.h
# End Source File
# Begin Source File

SOURCE=.\XPanel\XControls.h
# End Source File
# Begin Source File

SOURCE=.\XPanel\XDouble.cpp
# End Source File
# Begin Source File

SOURCE=.\XPanel\XDouble.h
# End Source File
# Begin Source File

SOURCE=.\XPanel\XFloatVparam.cpp
# End Source File
# Begin Source File

SOURCE=.\XPanel\XFloatVparam.h
# End Source File
# Begin Source File

SOURCE=.\XPanel\XInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\XPanel\XInfo.h
# End Source File
# Begin Source File

SOURCE=.\XPanel\XPanel.cpp
# End Source File
# Begin Source File

SOURCE=.\XPanel\XPanel.h
# End Source File
# Begin Source File

SOURCE=.\XPanel\XPercent.cpp
# End Source File
# Begin Source File

SOURCE=.\XPanel\XPercent.h
# End Source File
# Begin Source File

SOURCE=.\XPanel\XPercentVparam.cpp
# End Source File
# Begin Source File

SOURCE=.\XPanel\XPercentVparam.h
# End Source File
# End Group
# Begin Group "PluginTemplates"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PluginTemplates\Activator.h
# End Source File
# Begin Source File

SOURCE=.\PluginTemplates\CustomObject.cpp
# End Source File
# Begin Source File

SOURCE=.\PluginTemplates\CustomObject.h
# End Source File
# Begin Source File

SOURCE=.\PluginTemplates\CustomObject_Adaptor.h
# End Source File
# Begin Source File

SOURCE=.\PluginTemplates\GenericPlugin.h
# End Source File
# Begin Source File

SOURCE=.\PluginTemplates\GenericPluginPanel.h
# End Source File
# Begin Source File

SOURCE=.\PluginTemplates\Handler.h
# End Source File
# Begin Source File

SOURCE=.\PluginTemplates\HandlerImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\PluginTemplates\HandlerImpl.h
# End Source File
# Begin Source File

SOURCE=.\PluginTemplates\HandlerItem.h
# End Source File
# Begin Source File

SOURCE=.\PluginTemplates\HandlerItemRend.h
# End Source File
# Begin Source File

SOURCE=.\PluginTemplates\Interface.cpp
# End Source File
# Begin Source File

SOURCE=.\PluginTemplates\Interface.h
# End Source File
# Begin Source File

SOURCE=.\PluginTemplates\Interface_Adaptor.h
# End Source File
# Begin Source File

SOURCE=.\PluginTemplates\Master.h
# End Source File
# Begin Source File

SOURCE=.\PluginTemplates\Master_Adaptor.h
# End Source File
# Begin Source File

SOURCE=.\PluginTemplates\ProceduralTexture.cpp
# End Source File
# Begin Source File

SOURCE=.\PluginTemplates\ProceduralTexture.h
# End Source File
# Begin Source File

SOURCE=.\PluginTemplates\ProceduralTexture_Adaptor.h
# End Source File
# Begin Source File

SOURCE=.\PluginTemplates\VolumeHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\PluginTemplates\VolumeHandler.h
# End Source File
# Begin Source File

SOURCE=.\PluginTemplates\VolumeHandler_Adaptor.h
# End Source File
# End Group
# End Target
# End Project
