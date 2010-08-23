# Auto-generated by EclipseNSIS Script Wizard
# Aug 22, 2010 8:45:05 PM

Name Chameleon

# General Symbol Definitions
!define REGKEY "SOFTWARE\$(^Name)"
!define VERSIONMAJOR 0
!define VERSIONMINOR 5
!define VERSION "${VERSIONMAJOR}.${VERSIONMINOR}"
!define SHORTVERSION "${VERSIONMAJOR}${VERSIONMINOR}"

!define COMPANY "ISquared Software"
!define URL http://www.isquaredsoftware.com

# MultiUser Symbol Definitions
!define MULTIUSER_EXECUTIONLEVEL Power
!define MULTIUSER_MUI
!define MULTIUSER_INSTALLMODE_DEFAULT_REGISTRY_KEY "${REGKEY}"
!define MULTIUSER_INSTALLMODE_DEFAULT_REGISTRY_VALUENAME MultiUserInstallMode
!define MULTIUSER_INSTALLMODE_COMMANDLINE
!define MULTIUSER_INSTALLMODE_INSTDIR Chameleon
!define MULTIUSER_INSTALLMODE_INSTDIR_REGISTRY_KEY "${REGKEY}"
!define MULTIUSER_INSTALLMODE_INSTDIR_REGISTRY_VALUE "Path"

# MUI Symbol Definitions
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install-colorful.ico"
!define MUI_FINISHPAGE_NOAUTOCLOSE
!define MUI_STARTMENUPAGE_REGISTRY_ROOT HKLM
!define MUI_STARTMENUPAGE_NODISABLE
!define MUI_STARTMENUPAGE_REGISTRY_KEY ${REGKEY}
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME StartMenuGroup
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "ISquared Software\Chameleon"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall-colorful.ico"
!define MUI_UNFINISHPAGE_NOAUTOCLOSE

# Included files
!include MultiUser.nsh
!include Sections.nsh
!include MUI2.nsh

# Variables
Var StartMenuGroup

# Installer pages
!insertmacro MUI_PAGE_WELCOME
#!insertmacro MULTIUSER_PAGE_INSTALLMODE
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_STARTMENU Application $StartMenuGroup
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

# Installer languages
!insertmacro MUI_LANGUAGE English

# Installer attributes
OutFile "output\Chameleon${SHORTVERSION}Setup.exe"
InstallDir Chameleon
CRCCheck on
XPStyle on
ShowInstDetails show
VIProductVersion 0.5.0.0
VIAddVersionKey ProductName Chameleon
VIAddVersionKey ProductVersion "${VERSION}"
VIAddVersionKey CompanyName "${COMPANY}"
VIAddVersionKey CompanyWebsite "${URL}"
VIAddVersionKey FileVersion "${VERSION}"
VIAddVersionKey FileDescription ""
VIAddVersionKey LegalCopyright ""
InstallDirRegKey HKLM "${REGKEY}" Path
ShowUninstDetails show

RequestExecutionLevel user

# Installer sections
Section -Main SEC0000
    SetOutPath $INSTDIR
    SetOverwrite on
    File input\ANTLRParser.dll
    File input\AStyle.dll
    File input\Chameleon.exe
    File input\codelite_indexer.exe
    File input\CodeLiteParser.dll
    File input\CodeLiteParserNative.dll
    File input\ExceptionReporter.WinForms.dll
    File input\Guifreaks.Common.dll
    File input\Guifreaks.NavigationBar.dll
    File input\libvt100.dll
    File input\PSTaskDialog.dll
    File input\Routrek.Granados.dll
    File input\SciLexer.dll
    File input\ScintillaNet.dll
    File input\sqlite3.dll
    File input\TabStrip.dll
    File input\wwAppConfiguration.dll
    File input\wxbase290u.dll
    File input\wxbase290u_xml.dll
    File input\wxmsw290u_adv.dll
    File input\wxmsw290u_core.dll
    File input\wxmsw290u_html.dll
    File input\wxmsw290u_xrc.dll
    
    var ChameleonData
     # Find the user's AppData directory
    ReadRegStr $0 HKCU "${SHELLFOLDERS}" AppData
    StrCmp $0 "" 0 +2
    ReadRegStr $0 HKLM "${SHELLFOLDERS}" "Common AppData"
    StrCmp $0 "" 0 +2
    StrCpy $0 "$APPDATA"
    
    StrCpy $ChameleonData "$0\Chameleon"
    
    CreateDirectory $ChameleonData
    
    SetOutPath $ChameleonData
    File input\Chameleon.xml
    WriteRegStr HKLM "${REGKEY}\Components" Main 1
SectionEnd

Section -post SEC0001
    WriteRegStr HKLM "${REGKEY}" Path $INSTDIR
    SetOutPath $INSTDIR
    WriteUninstaller $INSTDIR\ChameleonUninstall.exe
    !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    SetOutPath $SMPROGRAMS\$StartMenuGroup
    CreateShortcut "$SMPROGRAMS\$StartMenuGroup\Uninstall $(^Name).lnk" $INSTDIR\ChameleonUninstall.exe
    !insertmacro MUI_STARTMENU_WRITE_END
    WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" DisplayName "$(^Name)"
    WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" DisplayVersion "${VERSION}"
    WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" Publisher "${COMPANY}"
    WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" URLInfoAbout "${URL}"
    WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" DisplayIcon $INSTDIR\ChameleonUninstall.exe
    WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" UninstallString $INSTDIR\ChameleonUninstall.exe
    WriteRegDWORD HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" NoModify 1
    WriteRegDWORD HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" NoRepair 1
SectionEnd

# Macro for selecting uninstaller sections
!macro SELECT_UNSECTION SECTION_NAME UNSECTION_ID
    Push $R0
    ReadRegStr $R0 HKLM "${REGKEY}\Components" "${SECTION_NAME}"
    StrCmp $R0 1 0 next${UNSECTION_ID}
    !insertmacro SelectSection "${UNSECTION_ID}"
    GoTo done${UNSECTION_ID}
next${UNSECTION_ID}:
    !insertmacro UnselectSection "${UNSECTION_ID}"
done${UNSECTION_ID}:
    Pop $R0
!macroend

# Uninstaller sections
Section /o -un.Main UNSEC0000
    var ChameleonData
     # Find the user's AppData directory
    ReadRegStr $0 HKCU "${SHELLFOLDERS}" AppData
    StrCmp $0 "" 0 +2
    ReadRegStr $0 HKLM "${SHELLFOLDERS}" "Common AppData"
    StrCmp $0 "" 0 +2
    StrCpy $0 "$APPDATA"
    
    StrCpy $ChameleonData "$0\Chameleon"

    Delete /REBOOTOK $ChameleonData\Chameleon.xml
    Delete /REBOOTOK $INSTDIR\wxmsw290u_xrc.dll
    Delete /REBOOTOK $INSTDIR\wxmsw290u_html.dll
    Delete /REBOOTOK $INSTDIR\wxmsw290u_core.dll
    Delete /REBOOTOK $INSTDIR\wxmsw290u_adv.dll
    Delete /REBOOTOK $INSTDIR\wxbase290u_xml.dll
    Delete /REBOOTOK $INSTDIR\wxbase290u.dll
    Delete /REBOOTOK $INSTDIR\wwAppConfiguration.dll
    Delete /REBOOTOK $INSTDIR\TabStrip.dll
    Delete /REBOOTOK $INSTDIR\sqlite3.dll
    Delete /REBOOTOK $INSTDIR\ScintillaNet.dll
    Delete /REBOOTOK $INSTDIR\SciLexer.dll
    Delete /REBOOTOK $INSTDIR\Routrek.Granados.dll
    Delete /REBOOTOK $INSTDIR\PSTaskDialog.dll
    Delete /REBOOTOK $INSTDIR\libvt100.dll
    Delete /REBOOTOK $INSTDIR\Guifreaks.NavigationBar.dll
    Delete /REBOOTOK $INSTDIR\Guifreaks.Common.dll
    Delete /REBOOTOK $INSTDIR\ExceptionReporter.WinForms.dll
    Delete /REBOOTOK $INSTDIR\CodeLiteParserNative.dll
    Delete /REBOOTOK $INSTDIR\CodeLiteParser.dll
    Delete /REBOOTOK $INSTDIR\codelite_indexer.exe
    Delete /REBOOTOK $INSTDIR\Chameleon.exe
    Delete /REBOOTOK $INSTDIR\AStyle.dll
    Delete /REBOOTOK $INSTDIR\ANTLRParser.dll
    DeleteRegValue HKLM "${REGKEY}\Components" Main
SectionEnd

Section -un.post UNSEC0001
    DeleteRegKey HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)"
    Delete /REBOOTOK "$SMPROGRAMS\$StartMenuGroup\Uninstall $(^Name).lnk"
    Delete /REBOOTOK $INSTDIR\ChameleonUninstall.exe
    DeleteRegValue HKLM "${REGKEY}" StartMenuGroup
    DeleteRegValue HKLM "${REGKEY}" Path
    DeleteRegKey /IfEmpty HKLM "${REGKEY}\Components"
    DeleteRegKey /IfEmpty HKLM "${REGKEY}"
    RmDir /REBOOTOK $SMPROGRAMS\$StartMenuGroup
    RmDir /REBOOTOK $INSTDIR
SectionEnd

# Installer functions
Function .onInit
    InitPluginsDir
    !insertmacro MULTIUSER_INIT
FunctionEnd

# Uninstaller functions
Function un.onInit
    !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuGroup
    !insertmacro MULTIUSER_UNINIT
    !insertmacro SELECT_UNSECTION Main ${UNSEC0000}
FunctionEnd

