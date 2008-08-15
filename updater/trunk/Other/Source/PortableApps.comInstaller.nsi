;Copyright 2007-2008 John T. Haller

;Website: http://PortableApps.com/

;This software is OSI Certified Open Source Software.
;OSI Certified is a certification mark of the Open Source Initiative.

;This program is free software; you can redistribute it and/or
;modify it under the terms of the GNU General Public License
;as published by the Free Software Foundation; either version 2
;of the License, or (at your option) any later version.

;This program is distributed in the hope that it will be useful,
;but WITHOUT ANY WARRANTY; without even the implied warranty of
;MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;GNU General Public License for more details.

;You should have received a copy of the GNU General Public License
;along with this program; if not, write to the Free Software
;Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

;EXCEPTION: Can be used with non-GPL apps distributed by PortableApps.com

;=== BEGIN: BASIC INFORMATION ===
;This section contains the basic information about the app
!define NAME "PortableApps.com Updater"
!define SHORTNAME "PortableAppsUpdater"
!define VERSION "0.9.1.0"
!define FILENAME "PA_UPDATER_1.0_Beta1"
!define CHECKRUNNING "PortableAppsUpdater.exe"
!define CLOSENAME "PortableApps.com Updater"
!define ADDONSDIRECTORYPRESERVE "NONE"
!define PORTABLEAPPSINSTALLERVERSION "0.9.9.7"
!define INSTALLERCOMMENTS "For additional details, visit PortableApps.com"
!define INSTALLERADDITIONALTRADEMARKS "Firefox is a Trademark of The Mozilla Foundation. " ;end this entry with a period and a space if used
!define INSTALLERLEGALCOPYRIGHT "PortableApps.com and contributors"

;== License.  For no license agreement, comment out the next line by placing a semicolon at the start of it
;!define LICENSEAGREEMENT "eula.rtf"

;== Multi-Installer.  If making an installer with no options (like additional languages), comment out the next line by placing a semicolon at the start of it
;!define MAINSECTIONTITLE "AppName Portable (English) [Required]"
!ifdef MAINSECTIONTITLE
	!define MAINSECTIONDESCRIPTION "Install the portable app"
	!define OPTIONALSECTIONTITLE "Additional Languages"
	!define OPTIONALSECTIONDESCRIPTION "Add multilingual support for this app"
	!define OPTIONALSECTIONSELECTEDAPPINFOSUFFIX "(Multilingual)"
	!define OPTIONALSECTIONNOTSELECTEDAPPINFOSUFFIX "(English)"
!endif
;=== END: BASIC INFORMATION ===

!define MAINSECTIONIDX 0
!ifdef MAINSECTIONTITLE
	!define OPTIONALSECTIONIDX 1
!endif

;=== Program Details
Name "${NAME}"
OutFile "..\..\..\${FILENAME}.paf.exe"
InstallDir "\${SHORTNAME}"
Caption "${NAME} | PortableApps.com Installer"
VIProductVersion "${VERSION}"
VIAddVersionKey ProductName "${NAME}"
VIAddVersionKey Comments "${INSTALLERCOMMENTS}"
VIAddVersionKey CompanyName "PortableApps.com"
VIAddVersionKey LegalCopyright "${INSTALLERLEGALCOPYRIGHT}"
VIAddVersionKey FileDescription "${NAME}"
VIAddVersionKey FileVersion "${VERSION}"
VIAddVersionKey ProductVersion "${VERSION}"
VIAddVersionKey InternalName "${NAME}"
VIAddVersionKey LegalTrademarks "${INSTALLERADDITIONALTRADEMARKS}PortableApps.com is a Trademark of Rare Ideas, LLC."
VIAddVersionKey OriginalFilename "${FILENAME}.paf.exe"
VIAddVersionKey PortableApps.comInstallerVersion "${PORTABLEAPPSINSTALLERVERSION}"
;VIAddVersionKey PrivateBuild ""
;VIAddVersionKey SpecialBuild ""

;=== Runtime Switches
SetCompress Auto
SetCompressor /SOLID lzma
SetCompressorDictSize 32
SetDatablockOptimize On
CRCCheck on
AutoCloseWindow True
RequestExecutionLevel user

;=== Include
!include MUI.nsh
!include FileFunc.nsh
!include LogicLib.nsh
!insertmacro DriveSpace
!insertmacro GetOptions
!insertmacro GetDrives
!insertmacro GetRoot
!insertmacro GetSize
!insertmacro GetParent
!include TextFunc.nsh
;=== BEGIN: OPTIONAL INCLUDE COMPONENTS
;If the app needs to read or write config files (not INI files), uncomment the needed options
;!insertmacro ConfigRead
;!insertmacro ConfigReadS
;!insertmacro ConfigWrite
;!insertmacro ConfigWriteS
;=== END: OPTIONAL INCLUDE COMPONENTS

;=== Program Icon
Icon "..\..\App\AppInfo\appicon.ico"

;=== Icon & Stye ===
!define MUI_ICON "..\..\App\AppInfo\appicon.ico"
BrandingText "PortableApps.com - Your Digital Life, Anywhere™"

;=== Pages
!define MUI_WELCOMEFINISHPAGE_BITMAP "PortableApps.comInstaller.bmp"
!define MUI_WELCOMEPAGE_TITLE "${NAME}"
!define MUI_WELCOMEPAGE_TEXT "$(welcome)"
!define MUI_COMPONENTSPAGE_SMALLDESC
!insertmacro MUI_PAGE_WELCOME
!ifdef LICENSEAGREEMENT
	!define MUI_LICENSEPAGE_CHECKBOX
	!insertmacro MUI_PAGE_LICENSE "${LICENSEAGREEMENT}"
!endif
!ifdef MAINSECTIONTITLE
	!insertmacro MUI_PAGE_COMPONENTS
!endif
!define MUI_DIRECTORYPAGE_VERIFYONLEAVE
!define MUI_PAGE_CUSTOMFUNCTION_LEAVE LeaveDirectory
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!define MUI_FINISHPAGE_TEXT "$(finish)"
!insertmacro MUI_PAGE_FINISH

;=== Languages
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "French"
!insertmacro MUI_LANGUAGE "German"
!insertmacro MUI_LANGUAGE "Italian"
!insertmacro MUI_LANGUAGE "Portuguese"
!insertmacro MUI_LANGUAGE "Spanish"
!insertmacro MUI_LANGUAGE "Japanese"
!insertmacro MUI_LANGUAGE "Simpchinese"

; == German translations ==
!include "LANG_GERMAN.nsh"

; == English translations ==
!include "LANG_ENGLISH.nsh"

; == French translations ==
!include "LANG_FRENCH.nsh"

; == Italian translations ==
!include "LANG_ITALIAN.nsh"

; == Portuguese translations ==
!include "LANG_PORTUGUESE.nsh"

; == Spanish translations ==
!include "LANG_SPANISH.nsh"

; == Japanese translations ==
!include "LANG_JAPANESE.nsh"

; == Traditional Chinese translations ==
!include "LANG_SIMPCHINESE.nsh"

!insertmacro MUI_RESERVEFILE_LANGDLL

;=== Variables
Var FOUNDPORTABLEAPPSPATH
Var PortableApps.comLocaleID
!ifdef MAINSECTIONTITLE
	Var OPTIONAL1DONE
!endif

Function .onInit
	;BEGIN: Init Language Detection Code
	ReadEnvStr $PortableApps.comLocaleID "PortableApps.comLocaleID"
	StrCmp $PortableApps.comLocaleID "1031" SetLanguageFromEnvironment ;German
	StrCmp $PortableApps.comLocaleID "1034" SetLanguageFromEnvironment ;Spain
	StrCmp $PortableApps.comLocaleID "1036" SetLanguageFromEnvironment ;French
	StrCmp $PortableApps.comLocaleID "1040" SetLanguageFromEnvironment ;Italian
	StrCmp $PortableApps.comLocaleID "2070" SetLanguageFromEnvironment ;Portuguese
	StrCmp $PortableApps.comLocaleID "1041" SetLanguageFromEnvironment ;Japanese
	StrCmp $PortableApps.comLocaleID "2052" SetLanguageFromEnvironment ;SIMPCHINESE
	StrCmp $PortableApps.comLocaleID "1033" SetLanguageFromEnvironment ShowLanguageSelector ;English
	;END: Init Language Detection Code
	
	SetLanguageFromEnvironment:
		StrCpy $LANGUAGE $PortableApps.comLocaleID
		Goto GetCommandLineOptions

	ShowLanguageSelector:
		!insertmacro MUI_LANGDLL_DISPLAY

	GetCommandLineOptions:
	${GetOptions} "$CMDLINE" "/DESTINATION=" $R0

	IfErrors CheckLegacyDestination
		StrCpy $INSTDIR "$R0${SHORTNAME}"
		Goto InitDone

	CheckLegacyDestination:
		ClearErrors
		${GetOptions} "$CMDLINE" "-o" $R0
		IfErrors NoDestination
			StrCpy $INSTDIR "$R0${SHORTNAME}"
			Goto InitDone

	NoDestination:
		ClearErrors
		${GetDrives} "HDD+FDD" GetDrivesCallBack
		StrCmp $FOUNDPORTABLEAPPSPATH "" DefaultDestination
			StrCpy $INSTDIR "$FOUNDPORTABLEAPPSPATH\${SHORTNAME}"
			Goto InitDone
		
	DefaultDestination:
		StrCpy $INSTDIR "\${SHORTNAME}"

	InitDone:
FunctionEnd

Function LeaveDirectory
	GetInstDirError $0
  
	;=== Does it already exist? (upgrade)
	IfFileExists "$INSTDIR" "" CheckInstallerError
		;=== Check if app is running?
		StrCmp ${CHECKRUNNING} "NONE" CheckInstallerError
			FindProcDLL::FindProc "${CHECKRUNNING}"
			StrCmp $R0 "1" "" CheckInstallerError
				MessageBox MB_OK|MB_ICONINFORMATION `$(runwarning)`
				Abort
  
	CheckInstallerError:
		${Switch} $0
		    ${Case} 0 ;=== Valid directory and enough free space
				${Break}
		    ${Case} 1
				MessageBox MB_OK `$(invaliddirectory)`
				Abort
				${Break}
		    ${Case} 2
				IfFileExists `$INSTDIR` "" NotEnoughSpaceNoUpgrade ;=== Is upgrade
					SectionGetSize ${MAINSECTIONIDX} $1 ;=== Space Required for App
					!ifdef MAINSECTIONTITLE
							SectionGetFlags ${OPTIONALSECTIONIDX} $9
							IntOp $9 $9 & ${SF_SELECTED}
							IntCmp $9 ${SF_SELECTED} "" NoOptions
								SectionGetSize ${OPTIONALSECTIONIDX} $2 ;=== Space Required for App
							IntOp $1 $1 + $2
						NoOptions:
					!endif
					${GetRoot} `$INSTDIR` $2
					${DriveSpace} `$2\` "/D=F /S=K" $3 ;=== Space Free on Device
					${GetSize} `$INSTDIR` "/M=*.* /S=0K /G=1" $4 $5 $6 ;=== Current installation size
					IntOp $3 $3 + $4 ;=== Space Free + Current Install Size
					IfFileExists `$INSTDIR\Data` "" CheckPluginsDirectory
						${GetSize} `$INSTDIR\Data` "/M=*.* /S=0K /G=1" $4 $5 $6 ;=== Size of Data directory
						IntOp $3 $3 - $4 ;=== Remove the data directory from the free space calculation

				CheckPluginsDirectory:
					StrCmp `${ADDONSDIRECTORYPRESERVE}` "NONE" CalculateSpaceLeft
						IfFileExists `$INSTDIR\${ADDONSDIRECTORYPRESERVE}` "" CalculateSpaceLeft
							${GetSize} `$INSTDIR\${ADDONSDIRECTORYPRESERVE}` "/M=*.* /S=0K /G=1" $4 $5 $6 ;=== Size of Data directory
							IntOp $3 $3 - $4 ;=== Remove the plugins directory from the free space calculation

				CalculateSpaceLeft:
					IntCmp $3 $1 NotEnoughSpaceNoUpgrade NotEnoughSpaceNoUpgrade
					Goto EndNotEnoughSpace

				NotEnoughSpaceNoUpgrade:
					MessageBox MB_OK `$(notenoughspace)`
					Abort

				EndNotEnoughSpace:
				${Break}
		${EndSwitch}
FunctionEnd

Function GetDrivesCallBack
	;=== Skip usual floppy letters
	StrCmp $8 "FDD" "" CheckForPortableAppsPath
	StrCmp $9 "A:\" End
	StrCmp $9 "B:\" End
	
	CheckForPortableAppsPath:
		IfFileExists "$9PortableApps" "" End
			StrCpy $FOUNDPORTABLEAPPSPATH "$9PortableApps"

	End:
		Push $0
FunctionEnd

!ifdef MAINSECTIONTITLE
	Section "${MAINSECTIONTITLE}"
!else
	Section "App Portable (required)"
!endif
	SectionIn RO
	SetOutPath $INSTDIR
	
!ifdef MAINSECTIONTITLE
	SectionGetFlags 1 $0
	IntOp $0 $0 & ${SF_SELECTED}
	IntCmp $0 ${SF_SELECTED} MainSkipOptionalCleanup
		;=== BEGIN: OPTIONAL NOT SELECTED CLEANUP CODE ===
		;This will be executed before install if the optional section (additional languages, etc) is not selected
		;=== END: OPTIONAL NOT SELECTED CLEANUP CODE ===
	MainSkipOptionalCleanup:
!endif
	
	;=== BEGIN: PRE-INSTALL CODE ===
	;This will be executed before the app is installed.  Useful for cleaning up files no longer used.
	 Delete '$INSTDIR\Help.chm'

	File "..\..\*.*"
	SetOutPath $INSTDIR\App
	File /r "..\..\App\*.*"
	SetOutPath $INSTDIR\Other
	File /r "..\..\Other\*.*"
	CreateDirectory "$INSTDIR\Data"
	
	;=== BEGIN: POST-INSTALL CODE ===
	;This will be executed after the app is installed.  Useful for updating configuration files.
		StrCmp $LANGUAGE "1031" 0 +3 ;German
	WriteINIStr $INSTDIR\Data\Settings.ini "General" "Language" "1031"
	Goto EndPostInstallCode
	StrCmp $LANGUAGE "1034" 0 +3 ;Spanish
	WriteINIStr $INSTDIR\Data\Settings.ini "General" "Language" "1034"
	Goto EndPostInstallCode
	StrCmp $LANGUAGE "1036" 0 +3 ;French
	WriteINIStr $INSTDIR\Data\Settings.ini "General" "Language" "1036"
	Goto EndPostInstallCode
	StrCmp $LANGUAGE "1040" 0 +3 ;Italian
	WriteINIStr $INSTDIR\Data\Settings.ini "General" "Language" "1040"
	Goto EndPostInstallCode
	StrCmp $LANGUAGE "2070" 0 +3 ;Portuguese
	WriteINIStr $INSTDIR\Data\Settings.ini "General" "Language" "2070"
	Goto EndPostInstallCode
	StrCmp $LANGUAGE "1041" 0 +3 ;Japanese
	WriteINIStr $INSTDIR\Data\Settings.ini "General" "Language" "1041"
	Goto EndPostInstallCode
	StrCmp $LANGUAGE "2052" 0 +3 ;SIMPCHINESE
	WriteINIStr $INSTDIR\Data\Settings.ini "General" "Language" "2052"
	Goto EndPostInstallCode
	WriteINIStr $INSTDIR\Data\Settings.ini "General" "Language" "1033" ;=== Fallback to English
	EndPostInstallCode:
	;=== END: POST-INSTALL CODE ===
	
	;=== Refresh PortableApps.com Menu (not final version)
	${GetParent} `$INSTDIR` $0
	;=== Check that it exists at the right location
	DetailPrint '$(checkforplatform)'
	IfFileExists `$0\PortableApps.com\App\PortableAppsPlatform.exe` "" TheEnd
		;=== Check that it's the real deal so we aren't hanging with no response
		MoreInfo::GetProductName `$0\PortableApps.com\App\PortableAppsPlatform.exe`
		Pop $1
		StrCmp $1 "PortableApps.com Platform" "" TheEnd
		MoreInfo::GetCompanyName `$0\PortableApps.com\App\PortableAppsPlatform.exe`
		Pop $1
		StrCmp $1 "PortableApps.com" "" TheEnd
		
		;=== Check that it's running
		FindProcDLL::FindProc "PortableAppsPlatform.exe"
		StrCmp $R0 "1" "" TheEnd
		
		;=== Send message for the Menu to refresh
		StrCpy $2 'PortableApps.comPlatformWindowMessageToRefresh$0\PortableApps.com\App\PortableAppsPlatform.exe'
		System::Call "user32::RegisterWindowMessage(t r2) i .r3"
		DetailPrint '$(refreshmenu)'
		SendMessage 65535 $3 0 0
	TheEnd:
SectionEnd

!ifdef MAINSECTIONTITLE
	Section /o "${OPTIONALSECTIONTITLE}"
		SetOutPath $INSTDIR
		File /r "..\..\..\${SHORTNAME}Optional1\*.*"
		StrCpy $OPTIONAL1DONE "true"
	SectionEnd

	Section "-UpdateAppInfo" SecUpdateAppInfo
		StrCmp $OPTIONAL1DONE "true" SecUpdateAppInfoOptionalSelected
			StrCmp ${OPTIONALSECTIONNOTSELECTEDAPPINFOSUFFIX} "" SecUpdateAppInfoTheEnd
				ReadINIStr $0 "$INSTDIR\App\AppInfo\appinfo.ini" "Version" "DisplayVersion"
				WriteINIStr $INSTDIR\App\AppInfo\appinfo.ini" "Version" "DisplayVersion" "$0 ${OPTIONALSECTIONNOTSELECTEDAPPINFOSUFFIX}"
			Goto SecUpdateAppInfoTheEnd

		SecUpdateAppInfoOptionalSelected:
			StrCmp ${OPTIONALSECTIONSELECTEDAPPINFOSUFFIX} "" SecUpdateAppInfoTheEnd
				ReadINIStr $0 "$INSTDIR\App\AppInfo\appinfo.ini" "Version" "DisplayVersion"
				WriteINIStr $INSTDIR\App\AppInfo\appinfo.ini" "Version" "DisplayVersion" "$0 ${OPTIONALSECTIONSELECTEDAPPINFOSUFFIX}"
		SecUpdateAppInfoTheEnd:
	SectionEnd

	!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
		!insertmacro MUI_DESCRIPTION_TEXT ${MAINSECTIONIDX} "${MAINSECTIONDESCRIPTION}"
		!insertmacro MUI_DESCRIPTION_TEXT ${OPTIONALSECTIONIDX} "${OPTIONALSECTIONDESCRIPTION}"
	!insertmacro MUI_FUNCTION_DESCRIPTION_END
!endif