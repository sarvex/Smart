!include "MUI2.nsh"
!include InstallOptions.nsh
!include "nsProcess.nsh"

OutFile "..\cdrom\setup.exe"
Name "SMARTFREN"
BrandingText "SMARTFREN Dialer"
InstallDir "$PROGRAMFILES32\SMARTFREN\"
RequestExecutionLevel admin

!define MUI_ICON "resource\logo.ico"
!define MUI_UNICON "resource\logo.ico"

!define MUI_WELCOMEFINISHPAGE_BITMAP "resource\splash.bmp"
!define MUI_UNWELCOMEFINISHPAGE_BITMAP "resource\splash.bmp"

;Pages
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES  
!insertmacro MUI_PAGE_FINISH
!define MUI_ABORTWARNING

!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH
!define MUI_UNABORTWARNING

!insertmacro MUI_LANGUAGE "English"

LangString CLOSE_SMARTFREN 1033 'Close "SMARTFREN" before continue'

LangString ALREADY_SMARTFREN_RUNNING_ENG 1033 'SMARTFREN is already installed. $\n$\nClick `OK` to remove the \ previous version or `Cancel` to cancel this upgrade'


LangString ALREADY_SMARTFREN_RUNNING_ENG 1049 'SMARTFREN is already installed. $\n$\nClick `OK` to remove the \ previous version or `Cancel` to cancel this upgrade'


LangString ALREADY_SMARTFREN_RUNNING_ENG 1058 'SMARTFREN is already installed. $\n$\nClick `OK` to remove the \ previous version or `Cancel` to cancel this upgrade'


Function .onInit
ReadRegDWORD $language HKLM "SOFTWARE\SMARTFREN" "Language"
BringToFront
; Check if already running
System::Call "kernel32::CreateMutexA(i 0, i 0, t '$(^Name)') i .r0 ?e"
Pop $0
StrCmp $0 0 launch
StrLen $0 "$(^Name)"
IntOp $0 $0 + 1
loop:
FindWindow $1 '#32770' '' 0 $1
IntCmp $1 0 +5
System::Call "user32::GetWindowText(i r1, t .r2, i r0) i."
StrCmp $2 "$(^Name)" 0 loop
System::Call "user32::ShowWindow(i r1,i 9) i."         ; If minimized then restore
System::Call "user32::SetForegroundWindow(i r1) i."    ; Bring to front
Abort
launch:

advsplash::show 1000 600 400 0x04025C resource\splash.bmp

ReadRegStr $R0 HKLM \
			   "Software\Microsoft\Windows\CurrentVersion\Uninstall\SMARTFREN" \
			   "UninstallString"
			   StrCmp $R0 "" done


			   ${If} $language == "1033"
			   MessageBox MB_OKCANCEL|MB_ICONEXCLAMATION \
				   "$(ALREADY_SMARTFREN_RUNNING_ENG)" \
				   IDOK uninst
				   Abort
				   ${EndIf}

				   ${If} $language == "1049"
				   MessageBox MB_OKCANCEL|MB_ICONEXCLAMATION \
					   "$(ALREADY_SMARTFREN_RUNNING_ENG)" \
					   IDOK uninst
					   Abort
					   ${EndIf}

					   ${If} $language == "1058"
					   MessageBox MB_OKCANCEL|MB_ICONEXCLAMATION \
						   "$(ALREADY_SMARTFREN_RUNNING_ENG)" \
						   IDOK uninst
						   Abort
						   ${EndIf}

						   ;Run the uninstaller
						   uninst:
						   ClearErrors
						   ExecWait '$R0 _?=$INSTDIR' ;Do not copy the uninstaller to a temp file

						   IfErrors no_remove_uninstaller done
						   ;You can either use Delete /REBOOTOK in the uninstaller or add some code
						   ;here to remove the uninstaller. Use a registry key to check
						   ;whether the user has chosen to uninstall. If you are using an uninstaller
						   ;components page, make sure all sections are uninstalled.
						   no_remove_uninstaller:

!insertmacro MUI_LANGDLL_DISPLAY
done:
FunctionEnd

Function un.onInit
ReadRegDWORD $language HKLM "SOFTWARE\SMARTFREN" "Language"
!insertmacro MUI_UNGETLANGUAGE
FunctionEnd 

Section "Desktop Shortcut" SectionX
SetOutPath $INSTDIR
SetShellVarContext current    
CreateShortCut "$DESKTOP\SMARTFREN.lnk" "$INSTDIR\SMARTFREN.exe" "" "$INSTDIR\SMARTFREN.exe" 0
SectionEnd

Section "Quick Launch Shortcut" SectionY
SetOutPath $INSTDIR
SetShellVarContext current   
CreateShortCut "$QUICKLAUNCH\SMARTFREN.lnk" "$INSTDIR\SMARTFREN.exe" "" "$INSTDIR\SMARTFREN.exe" 0
SectionEnd

Section "SMARTFREN"
!include "FileFunc.nsh"

;Add registry keys
WriteRegStr HKLM "SOFTWARE\SMARTFREN" "InstallationDirectory" $INSTDIR
WriteRegStr HKLM "SOFTWARE\SMARTFREN" "Language" $language
WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\SMARTFREN" "DisplayName" "SMARTFREN"
WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\SMARTFREN" "UninstallString" '"$INSTDIR\uninstall.exe"'
WriteRegDWORD HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\SMARTFREN" "NoModify" 1
WriteRegDWORD HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\SMARTFREN" "NoRepair" 1

SetOutPath $INSTDIR

CreateDirectory "$SMPROGRAMS\SMARTFREN"
CreateShortCut "$SMPROGRAMS\SMARTFREN\SMARTFREN.lnk" "$INSTDIR\SMARTFREN.exe" "" "$INSTDIR\SMARTFREN.exe" 0
CreateShortCut "$SMPROGRAMS\SMARTFREN\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\SMARTFREN.exe" 0

File ..\release\SMARTFREN.exe
File C:\QtSdk\Desktop\Qt\4.8.0\mingw\bin\libgcc_s_dw2-1.dll
File C:\QtSdk\Desktop\Qt\4.8.0\mingw\bin\mingwm10.dll        
File C:\QtSdk\Desktop\Qt\4.8.0\mingw\bin\QtCore4.dll         
File C:\QtSdk\Desktop\Qt\4.8.0\mingw\bin\QtGui4.dll
File C:\QtSdk\Desktop\Qt\4.8.0\mingw\bin\QtNetwork4.dll     
File C:\QtSdk\Desktop\Qt\4.8.0\mingw\bin\QtSql4.dll

WriteUninstaller "uninstall.exe"
File resource\help.chm
File resource\logo.ico

SetOutPath $INSTDIR\sqldrivers
File C:\QtSdk\Desktop\Qt\4.8.0\mingw\plugins\sqldrivers\qsqlite4.dll


SetOutPath $INSTDIR\drivers\x86
File drivers\x86\Oliveusbser.sys

SetOutPath $INSTDIR\drivers\amd64
File drivers\amd64\Oliveusbser.sys

SetOutPath $INSTDIR\drivers
File drivers\Oliveusbmdm.inf
File drivers\Oliveusbser.inf
File drivers\oliveusbser.cat 
File drivers\dpinst.xml
File drivers\dpinst64.exe  
File drivers\dpinst32.exe

WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\SMARTFREN" "DisplayIcon" "$INSTDIR\logo.ico"


SectionEnd

Section "Drivers"
!include "x64.nsh"
${If} ${RunningX64}
ExecWait $INSTDIR\drivers\x64\dpinst.exe
${Else}
ExecWait $INSTDIR\drivers\x86\dpinst.exe
${EndIf} 
ExecWait $INSTDIR\drivers\modem.exe
SectionEnd

Section "Uninstall"

; Close application before uninstallation 
${nsProcess::FindProcess} "SMARTFREN.exe" $R0
StrCmp $R0 0 0 +2
MessageBox MB_OKCANCEL|MB_ICONEXCLAMATION "$(CLOSE_SMARTFREN)" IDOK kill IDCANCEL end
kill:
${nsProcess::KillProcess} "SMARTFREN.exe" $R0

!include "x64.nsh"
${If} ${RunningX64}
ExecWait '"$INSTDIR\drivers\x64\dpinst.exe" /U "$INSTDIR\drivers\x64\Olive_USB_BusEnum.inf" /S /D'
ExecWait '"$INSTDIR\drivers\x64\dpinst.exe" /U "$INSTDIR\drivers\x64\Olive_USB_ETS.inf" /S /D'
ExecWait '"$INSTDIR\drivers\x64\dpinst.exe" /U "$INSTDIR\drivers\x64\Olive_USB_MODEM.inf" /S /D'
ExecWait '"$INSTDIR\drivers\x64\dpinst.exe" /U "$INSTDIR\drivers\x64\Olive_USB_WinMux.inf" /S /D'
${Else}
ExecWait '"$INSTDIR\drivers\x86\dpinst.exe" /U "$INSTDIR\drivers\x86\Olive_USB_BusEnum.inf" /S /D'
ExecWait '"$INSTDIR\drivers\x86\dpinst.exe" /U "$INSTDIR\drivers\x86\Olive_USB_ETS.inf" /S /D'
ExecWait '"$INSTDIR\drivers\x86\dpinst.exe" /U "$INSTDIR\drivers\x86\Olive_USB_MODEM.inf" /S /D'
ExecWait '"$INSTDIR\drivers\x86\dpinst.exe" /U "$INSTDIR\drivers\x86\Olive_USB_WinMux.inf" /S /D'
${EndIf}

; Remove registry keys
DeleteRegKey HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\SMARTFREN"
DeleteRegKey HKLM "SOFTWARE\SMARTFREN" 

; Remove files and uninstaller
Delete $INSTDIR\olive
Delete $INSTDIR\uninstall.exe
Delete $INSTDIR\SMARTFREN.exe
Delete $INSTDIR\libgcc_s_dw2-1.dll
Delete $INSTDIR\mingwm10.dll 
Delete $INSTDIR\QtCore4.dll        
Delete $INSTDIR\QtGui4.dll  
Delete $INSTDIR\QtSql4.dll  
Delete $INSTDIR\QtNetwork4.dll   
Delete $INSTDIR\SMARTFREN.chm
Delete $INSTDIR\logo.ico
Delete $INSTDIR\sqldrivers\qsqlite4.dll

;Driver Files
Delete $INSTDIR\drivers\Oliveusbmdm.inf
Delete $INSTDIR\drivers\Oliveusbser.inf
Delete $INSTDIR\drivers\oliveusbser.cat  
Delete $INSTDIR\drivers\dpinst32.exe 
Delete $INSTDIR\drivers\dpinst64.exe
Delete $INSTDIR\drivers\dpinst.xml
Delete $INSTDIR\drivers\x86\Oliveusbser.sys  
Delete $INSTDIR\drivers\amd64\Oliveusbser.sys
  

; Remove shortcuts, if any
Delete "$SMPROGRAMS\SMARTFREN\Olive\SMARTFRENDialer\SMARTFREN.lnk"
Delete "$SMPROGRAMS\SMARTFREN\Olive\SMARTFRENDialer\Uninstall.lnk"
Delete "$DESKTOP\SMARTFREN.lnk"
Delete "$QUICKLAUNCH\SMARTFREN.lnk"

; Remove directories used
RMDir "$SMPROGRAMS\SMARTFREN\Olive\SMARTFRENDialer"
RMDir "$SMPROGRAMS\SMARTFREN\Olive"
RMDir "$SMPROGRAMS\SMARTFREN"

RMDir "$INSTDIR\drivers\x86"
RMDir "$INSTDIR\drivers\amd64"
RMDir "$INSTDIR\drivers"
RMDir "$INSTDIR\sqldrivers"
RMDir "$INSTDIR"
RMDir "$PROGRAMFILES32\SMARTFREN\"

end:
SectionEnd
