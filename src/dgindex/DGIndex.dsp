# Microsoft Developer Studio Project File - Name="DGIndex" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=DGIndex - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DGIndex.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DGIndex.mak" CFG="DGIndex - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DGIndex - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "DGIndex - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=xicl6.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DGIndex - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /G5 /MT /W3 /Ox /Ot /Oa /Og /Oi /Gf /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x404 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 ddraw.lib dxguid.lib vfw32.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib shlwapi.lib /nologo /stack:0x3e8000 /subsystem:windows /machine:I386
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "DGIndex - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x404 /d "_DEBUG"
# ADD RSC /l 0x404 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ddraw.lib dxguid.lib vfw32.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib shlwapi.lib /nologo /stack:0x3e8000 /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "DGIndex - Win32 Release"
# Name "DGIndex - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\d2vparse.cpp
# End Source File
# Begin Source File

SOURCE=.\getbit.cpp
# End Source File
# Begin Source File

SOURCE=.\gethdr.cpp
# End Source File
# Begin Source File

SOURCE=.\getpic.cpp
# End Source File
# Begin Source File

SOURCE=.\gui.cpp

!IF  "$(CFG)" == "DGIndex - Win32 Release"

# ADD CPP /Oi /Op

!ELSEIF  "$(CFG)" == "DGIndex - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\idctfpu.cpp
# End Source File
# Begin Source File

SOURCE=.\idctref.cpp
# End Source File
# Begin Source File

SOURCE=.\initial_parse.cpp
# End Source File
# Begin Source File

SOURCE=.\misc.cpp
# End Source File
# Begin Source File

SOURCE=.\motion.cpp
# End Source File
# Begin Source File

SOURCE=.\mpeg2dec.cpp
# End Source File
# Begin Source File

SOURCE=.\norm.cpp
# End Source File
# Begin Source File

SOURCE=.\pat.cpp
# End Source File
# Begin Source File

SOURCE=.\store.cpp
# End Source File
# Begin Source File

SOURCE=.\strverscmp.cpp
# End Source File
# Begin Source File

SOURCE=.\wavefs44.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\filter.h
# End Source File
# Begin Source File

SOURCE=.\getbit.h
# End Source File
# Begin Source File

SOURCE=.\global.h
# End Source File
# Begin Source File

SOURCE=.\misc.h
# End Source File
# Begin Source File

SOURCE=.\pat.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\skl_nasm.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\gui.rc
# End Source File
# Begin Source File

SOURCE=.\movie.ico
# End Source File
# End Group
# Begin Group "AC3Dec"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AC3Dec\ac3.h
# End Source File
# Begin Source File

SOURCE=.\AC3Dec\bit_allocate.cpp
# End Source File
# Begin Source File

SOURCE=.\AC3Dec\bitstream.cpp
# End Source File
# Begin Source File

SOURCE=.\AC3Dec\bitstream.h
# End Source File
# Begin Source File

SOURCE=.\AC3Dec\coeff.cpp
# End Source File
# Begin Source File

SOURCE=.\AC3Dec\crc.cpp
# End Source File
# Begin Source File

SOURCE=.\AC3Dec\decode.cpp
# End Source File
# Begin Source File

SOURCE=.\AC3Dec\downmix.cpp
# End Source File
# Begin Source File

SOURCE=.\AC3Dec\exponent.cpp
# End Source File
# Begin Source File

SOURCE=.\AC3Dec\imdct.cpp
# End Source File
# Begin Source File

SOURCE=.\AC3Dec\parse.cpp
# End Source File
# Begin Source File

SOURCE=.\AC3Dec\rematrix.cpp
# End Source File
# Begin Source File

SOURCE=.\AC3Dec\sanity_check.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\idctmmx.asm

!IF  "$(CFG)" == "DGIndex - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=.\idctmmx.asm
InputName=idctmmx

"$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	ml /c /coff /Cx /nologo $(InputName).asm

# End Custom Build

!ELSEIF  "$(CFG)" == "DGIndex - Win32 Debug"

# Begin Custom Build
InputPath=.\idctmmx.asm
InputName=idctmmx

"$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	ml /c /coff /Cx /nologo $(InputName).asm

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\simple_idct_mmx.asm

!IF  "$(CFG)" == "DGIndex - Win32 Release"

# Begin Custom Build
IntDir=.\Release
InputPath=.\simple_idct_mmx.asm
InputName=simple_idct_mmx

"$(IntDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	d:\dosbin\nasm -f win32 -DPREFIX -DWIN32 -o $(IntDir)\$(InputName).obj $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "DGIndex - Win32 Debug"

# Begin Custom Build
IntDir=.\Debug
InputPath=.\simple_idct_mmx.asm
InputName=simple_idct_mmx

"$(IntDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	d:\dosbin\nasm -f win32 -DPREFIX -DWIN32 -o $(IntDir)\$(InputName).obj $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\skl_dct_sse.asm

!IF  "$(CFG)" == "DGIndex - Win32 Release"

# Begin Custom Build
IntDir=.\Release
InputPath=.\skl_dct_sse.asm
InputName=skl_dct_sse

"$(IntDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	d:\dosbin\nasm -f win32 -DPREFIX -DWIN32 -o $(IntDir)\$(InputName).obj $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "DGIndex - Win32 Debug"

# Begin Custom Build
IntDir=.\Debug
InputPath=.\skl_dct_sse.asm
InputName=skl_dct_sse

"$(IntDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	d:\dosbin\nasm -f win32 -DPREFIX -DWIN32 -o $(IntDir)\$(InputName).obj $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# End Target
# End Project
