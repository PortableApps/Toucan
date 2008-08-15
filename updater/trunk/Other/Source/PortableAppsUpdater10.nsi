;Copyright 2008 Marko Moeller
;Portions Copyright 2008 Ryan McCue
;Contributors/Translators Zé Pedro, lflcorreia, whibo, Aciago/Spanish team on pa.com, NeoRame

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

; === Defines ===
!define SHORTNAME "PortableAppsUpdater"
!define VERSION "0.9.0.10"
!define FILENAME "PortableAppsUpdater"
!define PRODUCT_NAME "PortableApps"
!define PRODUCT_VERSION "0.9.0"
!define NAME "${PRODUCT_NAME} ${PRODUCT_VERSION}"
!define PRODUCT_PUBLISHER "PortableApps.com"
!define PRODUCT_WEB_SITE "http://portableapps.com/"
!define SECTIONCOUNT 99 ; max sections - 1

; === App info ===
Name "${PRODUCT_NAME}"
OutFile "..\..\${FILENAME}.exe"

; === NSIS options ===
SetCompressor /SOLID lzma
CRCCheck on
AutoCloseWindow True
RequestExecutionLevel user
AllowRootDirInstall true

; === MUI start ===
!include MUI.nsh
;!include "UMUI.nsh"
!include nsDialogs.nsh
!include FileFunc.nsh
!include LogicLib.nsh
!insertmacro GetOptions
!insertmacro GetParent
!include "Time.nsh"
!include "Sections.nsh"
!include "Registry.nsh"
!include "TextFunc.nsh"
!include InstallOptions.nsh
!include "WordFunc.nsh"
!insertmacro WordFind
!insertmacro VersionCompare
; == Section sizes ==
!include "PAUPDATER_SEC_SIZE.nsh"



; MUI Settings
;!define UMUI_SKIN "gray"
!define MUI_ABORTWARNING
!define MUI_ICON "..\..\App\AppInfo\appicon.ico"
BrandingText "PortableApps.com - Your Digital Life, Anywhere™"

; Language Selection Dialog Settings
!define MUI_LANGDLL_REGISTRY_VALUENAME "NSIS:Language"
!define MUI_COMPONENTSPAGE_SMALLDESC

; == Welcome page ==
!define MUI_WELCOMEFINISHPAGE_BITMAP "PortableApps.comInstaller.bmp"
!define MUI_WELCOMEPAGE_TITLE "${NAME}"
!define MUI_WELCOMEPAGE_TEXT "$(welcome)"
!define MUI_PAGE_CUSTOMFUNCTION_PRE Pre-Welcome
!define MUI_PAGE_CUSTOMFUNCTION_LEAVE WelcomeLeave
!insertmacro MUI_PAGE_WELCOME

; == Option page == 
Page custom CustomPageA LeaveCustomPageA

; == Components page ==
!define MUI_PAGE_CUSTOMFUNCTION_PRE Pre-Components
!define MUI_PAGE_CUSTOMFUNCTION_LEAVE ComponentsLeave
!define MUI_PAGE_COMPONENTS_TITLE "Title"
!define MUI_PAGE_COMPONENTS_TEXT "Text"
!insertmacro MUI_PAGE_COMPONENTS

; == Instfiles page ==
!define MUI_PAGE_CUSTOMFUNCTION_PRE Pre-Install
!insertmacro MUI_PAGE_INSTFILES

; == Finish page ==
!define MUI_PAGE_CUSTOMFUNCTION_PRE Pre-Finish
!define MUI_FINISHPAGE_TEXT "$(finish)"
!insertmacro MUI_PAGE_FINISH

; === Localisations ===
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "French"
!insertmacro MUI_LANGUAGE "German"
!insertmacro MUI_LANGUAGE "Italian"
!insertmacro MUI_LANGUAGE "Portuguese"
!insertmacro MUI_LANGUAGE "Spanish"
!insertmacro MUI_LANGUAGE "Japanese"
!insertmacro MUI_LANGUAGE "SimpChinese"

!insertmacro MUI_RESERVEFILE_LANGDLL

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

; Reserve files
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS
; === MUI end ===

;=== Owerwrite Object of the components page with this values
ComponentText "$(components)"
DirText "$(d_subtext)" "$(d_label)" "$(d_button)"
InstallButtonText "Start"


;=== variables
var Mirror
var mirror_001
var mirror_002
var mirror_003
var mirror_004
var mirror_005
var mirror_006
var mirror_007
var mirror_008
var mirror_009
var debug
var OMirror
var OName
var SName
var OFile
var UFile
var CFile
var OSource
var OSize
var pversion
var pdel
var lversion
var ldversion
var dversion
var imod
var mversion
var APP_SET
var Proxy_Adr
var Proxy_User
var Proxy_PW
var ProxyAuth
var wpage
var cpage
var fpage
var opage
var tvar1
var tvar2
var tvar3
var tvar4
var tvar5
var tvar6
var varn
var varb
var vart
var vard
var no_ok
var pbatch
var pselect
var PortableApps.comLocaleID
var PORTABLEAPPSROOTPATH
var OCaption
var ODescription
var md5
var SHOWALLAPPS
var wget_options
InstallDir "$TEMP\Data\setup"


;=== Begin sections


Section /o "1" SEC1
	AddSize ${sec1_size}
	StrCpy $OName 1
	StrCpy $vart "true"
	Call OI_select
SectionEnd

Section /o "2" SEC2
	AddSize ${sec2_size}
	StrCpy $OName 2
	StrCpy $vart "true"
	Call OI_select
SectionEnd

Section /o "3" SEC3
	AddSize ${sec3_size}
	StrCpy $OName 3
	Call OI_select
SectionEnd

Section /o "4" SEC4
	AddSize ${sec4_size}
	StrCpy $OName 4
	Call OI_select
SectionEnd

Section /o "5" SEC5
	AddSize ${sec5_size}
	StrCpy $OName 5
	Call OI_select
SectionEnd

Section /o "6" SEC6
	AddSize ${sec6_size}
	StrCpy $OName 6
	Call OI_select
SectionEnd

Section /o "7" SEC7
	AddSize ${sec7_size}
	StrCpy $OName 7
	Call OI_select
SectionEnd

Section /o "8" SEC8
	AddSize ${sec8_size}
	StrCpy $OName 8
	Call OI_select
SectionEnd

Section /o "9" SEC9
	AddSize ${sec9_size}
	StrCpy $OName 9
	Call OI_select
SectionEnd

Section /o "10" SEC10
	AddSize ${sec10_size}
	StrCpy $OName 10
	Call OI_select
SectionEnd

Section /o "11" SEC11
	AddSize ${sec11_size}
	StrCpy $OName 11
	Call OI_select
SectionEnd

Section /o "12" SEC12
	AddSize ${sec12_size}
	StrCpy $OName 12
	Call OI_select
SectionEnd

Section /o "13" SEC13
	AddSize ${sec13_size}
	StrCpy $OName 13
	Call OI_select
SectionEnd

Section /o "14" SEC14
	AddSize ${sec14_size}
	StrCpy $OName 14
	Call OI_select
SectionEnd

Section /o "15" SEC15
	AddSize ${sec15_size}
	StrCpy $OName 15
	Call OI_select
SectionEnd

Section /o "16" SEC16
	AddSize ${sec16_size}
	StrCpy $OName 16
	Call OI_select
SectionEnd

Section /o "17" SEC17
	AddSize ${sec17_size}
	StrCpy $OName 17
	Call OI_select
SectionEnd

Section /o "18" SEC18
	AddSize ${sec18_size}
	StrCpy $OName 18
	Call OI_select
SectionEnd

Section /o "19" SEC19
	AddSize ${sec19_size}
	StrCpy $OName 19
	Call OI_select
SectionEnd

Section /o "20" SEC20
	AddSize ${sec20_size}
	StrCpy $OName 20
	Call OI_select
SectionEnd

Section /o "21" SEC21
	AddSize ${sec21_size}
	StrCpy $OName 21
	Call OI_select
SectionEnd

Section /o "22" SEC22
	AddSize ${sec22_size}
	StrCpy $OName 22
	Call OI_select
SectionEnd

Section /o "23" SEC23
	AddSize ${sec23_size}
	StrCpy $OName 23
	Call OI_select
SectionEnd

Section /o "24" SEC24
	AddSize ${sec24_size}
	StrCpy $OName 24
	Call OI_select
SectionEnd

Section /o "25" SEC25
	AddSize ${sec25_size}
	StrCpy $OName 25
	Call OI_select
SectionEnd

Section /o "26" SEC26
	AddSize ${sec26_size}
	StrCpy $OName 26
	Call OI_select
SectionEnd

Section /o "27" SEC27
	AddSize ${sec27_size}
	StrCpy $OName 27
	Call OI_select
SectionEnd

Section /o "28" SEC28
	AddSize ${sec28_size}
	StrCpy $OName 28
	Call OI_select
SectionEnd

Section /o "29" SEC29
	AddSize ${sec29_size}
	StrCpy $OName 29
	Call OI_select
SectionEnd

Section /o "30" SEC30
	AddSize ${sec30_size}
	StrCpy $OName 30
	Call OI_select
SectionEnd

Section /o "31" SEC31
	AddSize ${sec31_size}
	StrCpy $OName 31
	Call OI_select
SectionEnd

Section /o "32" SEC32
	AddSize ${sec32_size}
	StrCpy $OName 32
	Call OI_select
SectionEnd

Section /o "33" SEC33
	AddSize ${sec33_size}
	StrCpy $OName 33
	Call OI_select
SectionEnd

Section /o "34" SEC34
	AddSize ${sec34_size}
	StrCpy $OName 34
	Call OI_select
SectionEnd

Section /o "35" SEC35
	AddSize ${sec35_size}
	StrCpy $OName 35
	Call OI_select
SectionEnd

Section /o "36" SEC36
	AddSize ${sec36_size}
	StrCpy $OName 36
	Call OI_select
SectionEnd

Section /o "37" SEC37
	AddSize ${sec37_size}
	StrCpy $OName 37
	Call OI_select
SectionEnd

Section /o "38" SEC38
	AddSize ${sec38_size}
	StrCpy $OName 38
	Call OI_select
SectionEnd

Section /o "39" SEC39
	AddSize ${sec39_size}
	StrCpy $OName 39
	Call OI_select
SectionEnd

Section /o "40" SEC40
	AddSize ${sec40_size}
	StrCpy $OName 40
	Call OI_select
SectionEnd

Section /o "41" SEC41
	AddSize ${sec41_size}
	StrCpy $OName 41
	Call OI_select
SectionEnd

Section /o "42" SEC42
	AddSize ${sec42_size}
	StrCpy $OName 42
	Call OI_select
SectionEnd

Section /o "43" SEC43
	AddSize ${sec43_size}
	StrCpy $OName 43
	Call OI_select
SectionEnd

Section /o "44" SEC44
	AddSize ${sec44_size}
	StrCpy $OName 44
	Call OI_select
SectionEnd

Section /o "45" SEC45
	AddSize ${sec45_size}
	StrCpy $OName 45
	Call OI_select
SectionEnd

Section /o "46" SEC46
	AddSize ${sec46_size}
	StrCpy $OName 46
	Call OI_select
SectionEnd

Section /o "47" SEC47
	AddSize ${sec47_size}
	StrCpy $OName 47
	Call OI_select
SectionEnd

Section /o "48" SEC48
	AddSize ${sec48_size}
	StrCpy $OName 48
	Call OI_select
SectionEnd

Section /o "49" SEC49
	AddSize ${sec49_size}
	StrCpy $OName 49
	Call OI_select
SectionEnd

Section /o "50" SEC50
	AddSize ${sec50_size}
	StrCpy $OName 50
	Call OI_select
SectionEnd

Section /o "51" SEC51
	AddSize ${sec51_size}
	StrCpy $OName 51
	StrCpy $vart "true"
	Call OI_select
SectionEnd

Section /o "52" SEC52
	AddSize ${sec52_size}
	StrCpy $OName 52
	StrCpy $vart "true"
	Call OI_select
SectionEnd

Section /o "53" SEC53
	AddSize ${sec53_size}
	StrCpy $OName 53
	Call OI_select
SectionEnd

Section /o "54" SEC54
	AddSize ${sec54_size}
	StrCpy $OName 54
	Call OI_select
SectionEnd

Section /o "55" SEC55
	AddSize ${sec55_size}
	StrCpy $OName 55
	Call OI_select
SectionEnd

Section /o "56" SEC56
	AddSize ${sec56_size}
	StrCpy $OName 56
	Call OI_select
SectionEnd

Section /o "57" SEC57
	AddSize ${sec57_size}
	StrCpy $OName 57
	Call OI_select
SectionEnd

Section /o "58" SEC58
	AddSize ${sec58_size}
	StrCpy $OName 58
	Call OI_select
SectionEnd

Section /o "59" SEC59
	AddSize ${sec59_size}
	StrCpy $OName 59
	Call OI_select
SectionEnd

Section /o "60" SEC60
	AddSize ${sec60_size}
	StrCpy $OName 60
	Call OI_select
SectionEnd

Section /o "61" SEC61
	AddSize ${sec61_size}
	StrCpy $OName 61
	Call OI_select
SectionEnd

Section /o "62" SEC62
	AddSize ${sec62_size}
	StrCpy $OName 62
	Call OI_select
SectionEnd

Section /o "63" SEC63
	AddSize ${sec63_size}
	StrCpy $OName 63
	Call OI_select
SectionEnd

Section /o "64" SEC64
	AddSize ${sec64_size}
	StrCpy $OName 64
	Call OI_select
SectionEnd

Section /o "65" SEC65
	AddSize ${sec65_size}
	StrCpy $OName 65
	Call OI_select
SectionEnd

Section /o "66" SEC66
	AddSize ${sec66_size}
	StrCpy $OName 66
	Call OI_select
SectionEnd

Section /o "67" SEC67
	AddSize ${sec67_size}
	StrCpy $OName 67
	Call OI_select
SectionEnd

Section /o "68" SEC68
	AddSize ${sec68_size}
	StrCpy $OName 68
	Call OI_select
SectionEnd

Section /o "69" SEC69
	AddSize ${sec69_size}
	StrCpy $OName 69
	Call OI_select
SectionEnd

Section /o "70" SEC70
	AddSize ${sec70_size}
	StrCpy $OName 70
	Call OI_select
SectionEnd

Section /o "71" SEC71
	AddSize ${sec71_size}
	StrCpy $OName 71
	Call OI_select
SectionEnd

Section /o "72" SEC72
	AddSize ${sec72_size}
	StrCpy $OName 72
	Call OI_select
SectionEnd

Section /o "73" SEC73
	AddSize ${sec73_size}
	StrCpy $OName 73
	Call OI_select
SectionEnd

Section /o "74" SEC74
	AddSize ${sec74_size}
	StrCpy $OName 74
	Call OI_select
SectionEnd

Section /o "75" SEC75
	AddSize ${sec75_size}
	StrCpy $OName 75
	Call OI_select
SectionEnd

Section /o "76" SEC76
	AddSize ${sec76_size}
	StrCpy $OName 76
	Call OI_select
SectionEnd

Section /o "77" SEC77
	AddSize ${sec77_size}
	StrCpy $OName 77
	Call OI_select
SectionEnd

Section /o "78" SEC78
	AddSize ${sec78_size}
	StrCpy $OName 78
	Call OI_select
SectionEnd

Section /o "79" SEC79
	AddSize ${sec79_size}
	StrCpy $OName 79
	Call OI_select
SectionEnd

Section /o "80" SEC80
	AddSize ${sec80_size}
	StrCpy $OName 80
	Call OI_select
SectionEnd

Section /o "81" SEC81
	AddSize ${sec81_size}
	StrCpy $OName 81
	Call OI_select
SectionEnd

Section /o "82" SEC82
	AddSize ${sec82_size}
	StrCpy $OName 82
	Call OI_select
SectionEnd

Section /o "83" SEC83
	AddSize ${sec83_size}
	StrCpy $OName 83
	Call OI_select
SectionEnd

Section /o "84" SEC84
	AddSize ${sec84_size}
	StrCpy $OName 84
	Call OI_select
SectionEnd

Section /o "85" SEC85
	AddSize ${sec85_size}
	StrCpy $OName 85
	Call OI_select
SectionEnd

Section /o "86" SEC86
	AddSize ${sec86_size}
	StrCpy $OName 86
	Call OI_select
SectionEnd

Section /o "87" SEC87
	AddSize ${sec87_size}
	StrCpy $OName 87
	Call OI_select
SectionEnd

Section /o "88" SEC88
	AddSize ${sec88_size}
	StrCpy $OName 88
	Call OI_select
SectionEnd

Section /o "89" SEC89
	AddSize ${sec89_size}
	StrCpy $OName 89
	Call OI_select
SectionEnd

Section /o "90" SEC90
	AddSize ${sec90_size}
	StrCpy $OName 90
	Call OI_select
SectionEnd

Section /o "91" SEC91
	AddSize ${sec91_size}
	StrCpy $OName 91
	Call OI_select
SectionEnd

Section /o "92" SEC92
	AddSize ${sec92_size}
	StrCpy $OName 92
	Call OI_select
SectionEnd

Section /o "93" SEC93
	AddSize ${sec93_size}
	StrCpy $OName 93
	Call OI_select
SectionEnd

Section /o "94" SEC94
	AddSize ${sec94_size}
	StrCpy $OName 94
	Call OI_select
SectionEnd

Section /o "95" SEC95
	AddSize ${sec95_size}
	StrCpy $OName 95
	Call OI_select
SectionEnd

Section /o "96" SEC96
	AddSize ${sec96_size}
	StrCpy $OName 96
	Call OI_select
SectionEnd

Section /o "97" SEC97
	AddSize ${sec97_size}
	StrCpy $OName 97
	Call OI_select
SectionEnd

Section /o "98" SEC98
	AddSize ${sec98_size}
	StrCpy $OName 98
	Call OI_select
SectionEnd

Section /o "99" SEC99
	AddSize ${sec99_size}
	StrCpy $OName 99
	Call OI_select
SectionEnd

Section /o "100" SEC100
	AddSize ${sec100_size}
	StrCpy $OName 110
	Call OI_select
SectionEnd

Section -Post
	call Clean_Up
SectionEnd

;=== End sections

ShowInstDetails show

Function .onInit
	;=== read default settings for the variables
	Call default_settings


	;=== read Commandline parameters
	Call GET_COMMAND_LINE_PARAMETERS

	ReadINIStr $0 "$CFile" "Show" "AllApps"
	StrCmp $0 "" +2
	StrCpy "$SHOWALLAPPS" $0

	;=== read PortableAppsrootpath
	Push $EXEDIR
	Call GetParent
	Pop $PORTABLEAPPSROOTPATH

	;=== check language
	Call LanguageInit

	;=== check proxy settings
	Call CheckProxySettings

	${Select} $opage 
	${Case} "1"
		;=== no run for config mode!

	${CaseElse}
		;=== call Main sub (net or local usage, state of the installed Apps) 
		Call sstart

		;=== Call PrepareList to fill the list with the Apps for download and install
		Call PrepareAppList
	${EndSelect}
	;call Clean_Up
FunctionEnd

Function LanguageInit
	;=== set language from ini if present
	ReadINIStr $0 "$CFile" "General" "Language"
	StrCmp $0 "" +2
	SetEnv::SetEnvVar "PortableApps.comLocaleID" "$0"
	;BEGIN: Init Language Detection Code
	ReadEnvStr $PortableApps.comLocaleID "PortableApps.comLocaleID"
	StrCmp $PortableApps.comLocaleID "1031" SetLanguageFromEnvironment ;German
	StrCmp $PortableApps.comLocaleID "1034" SetLanguageFromEnvironment ;Spanish
	StrCmp $PortableApps.comLocaleID "1036" SetLanguageFromEnvironment ;French
	StrCmp $PortableApps.comLocaleID "1040" SetLanguageFromEnvironment ;Italian
	StrCmp $PortableApps.comLocaleID "2070" SetLanguageFromEnvironment ;Portuguese
	StrCmp $PortableApps.comLocaleID "1041" SetLanguageFromEnvironment ;Japanese
	StrCmp $PortableApps.comLocaleID "2052" SetLanguageFromEnvironment ;SIMPCHINESE
	StrCmp $PortableApps.comLocaleID "1033" SetLanguageFromEnvironment ShowLanguageSelector ;English

	;END: Init Language Detection Code
	
	SetLanguageFromEnvironment:
		
		StrCpy $LANGUAGE $PortableApps.comLocaleID
		Goto No_LANG_SELECT

	ShowLanguageSelector:
		!insertmacro MUI_LANGDLL_DISPLAY
	No_LANG_SELECT:
FunctionEnd

Function GET_COMMAND_LINE_PARAMETERS
	;=== start read the parameters

	;=== read the parameter for option page
	${GetOptions} "$CMDLINE" "/SHOWOP" $R0
	IfErrors +2
		StrCpy $opage "1"



	${GetOptions} "$CMDLINE" "/PROFILE=" $R0
	IfErrors step0
		StrCpy $CFile $R0
	
	step0:
	
	;=== read ini settings
	Call read_settings_ini
	
	
	

FunctionEnd

Function ReadSelComp
StrCpy $vart "false"
${For} $R1 0 ${SECTIONCOUNT}
 SectionGetFlags $R1 $R2
 StrCmp $R2 1 "" +2
 StrCpy $vart "true"

${Next}

FunctionEnd



Function ReadSelComp2
;=== hide not installed apps from the components page if ShowAllApps is 0
StrCmp $SHOWALLAPPS "1" "" rsc_1
${For} $R1 0 ${SECTIONCOUNT}
 SectionGetFlags $R1 $R2
 StrCmp $R2 257 +2
 SectionSetText $R1 ""
${Next}
rsc_1:

;=== if any section is still visible (has text) count control variable
${For} $R1 0 ${SECTIONCOUNT}
 SectionGetText $R1 $R2
 StrCmp $R2 "" +2
 IntOp $vard $vard + 1
${Next}

FunctionEnd



Function PrepareAppList 
;=== fill the components page
  StrCpy "$vard" 0
  ${For} $R1 0 ${SECTIONCOUNT}
    	SectionGetText $R1 $R2
    	ReadINIStr $R0 "$UFile" "$R2" "Name" 
    	StrCpy $OName $R2
	;=== call sub for App state / version check
    	call OU_select
    	StrCmp $varb "1" step1
	;=== set section text from ini
    	SectionSetText $R1 "$OCaption $dversion --> (local:$ldversion)"
	
	;=== set preselect state from ini
    	StrCmp $pselect "false" step2
	!insertmacro SelectSection $R1
	;=== set section mandantory from ini
	StrCmp $pselect "must" "" step2
	SectionSetFlags $R1 17
	Goto step2
	step1:
	;=== set section text empty (hide empty section)
	SectionSetText $R1 ""
	step2:	
  ${Next}
	call ReadSelComp2
	;=== if a section is visible jump to step4 (show components page)
	StrCmp $vard "0" "" step4
	StrCmp $no_ok "true" +2
	;=== show allok message if wanted
	MessageBox MB_OK "$(allok)"
	StrCpy $cpage "0"
	call Clean_Up
	Quit
	step4:
FunctionEnd

Function OU_select
	StrCpy "$varb" "0"
	call read_update_ini_OName
	;=== check for local appinfo.ini
	IfFileExists "$PORTABLEAPPSROOTPATH\$SName\App\AppInfo\appinfo.ini" "" read_local_ini
	;=== skipp updater session1
	StrCmp $OName "1" +2
	StrCpy "$pselect" "true"
	read_local_ini:
	;=== read the version for the App from local appinfo.ini
	ReadINIStr $0 "$PORTABLEAPPSROOTPATH\$SName\App\AppInfo\appinfo.ini" "Version" "PackageVersion"
	StrCpy "$lversion" $0
	;=== read the long version description for the App from local appinfo.ini
	ReadINIStr $0 "$PORTABLEAPPSROOTPATH\$SName\App\AppInfo\appinfo.ini" "Version" "DisplayVersion"
	StrCpy "$ldversion" $0
	${VersionCompare} "$pversion" "$lversion" $R0
	;=== compare version entries
	;MessageBox MB_OK "$OName $lversion $pversion"
	${Select} $R0
	${Case} 1
		;=== server version is newer
		;=== check if this app on the useres blacklist, than set control variable
		ReadINIStr $0 "$CFile" "Blacklist" "$SName"
		${Select} $0
		${Case} "true"
			StrCpy "$varb" "1"
		${CaseElse}
		${EndSelect}
	${CaseElse}
		;=== same version or older server version set control variable
		StrCpy "$varb" "1"
	${EndSelect}
FunctionEnd

Function sstart
	;=== delete old updater.ini
	Delete "$EXEDIR\Data\updater.ini"

	;=== compare value of ini file
	${Select} $UFile

	;=== value for net use found, download the ini
	${Case} "$EXEDIR\Data\updater.ini"
		StrCmp $debug "false" +2
		MessageBox MB_OK "Net"
		SetOutPath "$EXEDIR\Data\"
		;Call Show_DL-Splash
		;StrCpy $DLTEXT "$(plswait)"
		call Show_DL-BANNER
		nsexec::exectolog `"$EXEDIR\App\wget\wget" --passive-ftp $ProxyAuth $wget_options "$OFile"` 
		BringToFront
		;=== inetc etc don't work with proxies on all win systems!
		;inetc::get /caption "Database Download" /banner "$(plswait)$\nPlease wait..." /nocancel "$OFile"  "$EXEDIR\Data\updater.ini" /end
		;Pop $0 # return value = exit code, "OK" means OK
		;StrCmp $debug "false" +2
		;MessageBox MB_OK "Download Status:  $0"	
	${CaseElse}
		StrCmp $debug "false" +2
		MessageBox MB_OK "Local"
	${EndSelect}
	;=== if db.ini not exist show error and quit updater
	IfFileExists "$UFile" +4
	MessageBox MB_OK "$(db_error) $0"
	BringToFront
    	Abort
	SetOutPath "$INSTDIR"

	StrCmp $debug "false" +2
	MessageBox MB_OK "$INSTDIR"

	;=== read ini settings for OName
 	Call read_update_ini_OName
	BringToFront
FunctionEnd

Function DirectoryLeave
  ;===  Check if any spaces exist in $INSTDIR.
  ${WordFind} "$INSTDIR" ' ' "*" $R0
  	IntCmp $R0 0 NoSpaces
        ;=== use short path if blanks
	GetFullPathName /SHORT $0 $INSTDIR
	StrCpy $INSTDIR $0  
  	NoSpaces:
FunctionEnd



Function Pre-Welcome
;=== check for comp page back button, if clicked show welcome page

StrCmp $wpage "1" +2
	Abort
FunctionEnd

Function WelcomeLeave
	;WriteINIStr $EXEDIR\Data\Settings.ini "Show" "Welcome" "false"
FunctionEnd

Function Pre-Install
;StrCmp $ipage "true" +2
	;Abort
FunctionEnd
 
Function Pre-Finish
StrCmp $fpage "1" +2
	Abort
FunctionEnd

Function Pre-Components
${GetOptions} "$CMDLINE" "/APP=" $R0
IfErrors noskip04
	StrCpy $wpage "1"
	StrCmp $cpage "1" +2
	Abort
noskip04:
StrCpy $APP_SET "SHOW"
FunctionEnd

Function ComponentsLeave
StrCpy $vart "false"
call ReadSelComp
StrCmp 	$vart "true" +3
	MessageBox MB_OK "$(no_comp)"
	Abort
FunctionEnd




Function default_settings
	StrCpy "$Proxy_Adr" ""
	StrCpy "$wpage" "1"
	StrCpy $CFile "$EXEDIR\Data\settings.ini"
	StrCpy "$cpage" "1"
	StrCpy "$fpage" "1"
	StrCpy "$pdel" "1"
	StrCpy $UFile "$EXEDIR\Data\updater.ini"
	StrCpy $OFile "http://update.portableapps.com/platform/updater.ini"
	StrCpy $varn "0"
	StrCpy $debug "false"
	StrCpy $no_ok "false"
	StrCpy $pbatch "false"
	StrCpy $pselect "false"
	StrCpy $mversion "sf_mirrors"
	StrCpy $SHOWALLAPPS "1"
FunctionEnd




Function ORandom
 ${Select} $mversion
	${Case} "sf_mirrors"
	;=== no randomize for sf mirror
		StrCpy $Mirror $mirror_001
	${CaseElse}
 		${time::GetLocalTime} $0
 		${time::TimeString} "$0" $tvar1 $tvar2 $tvar3 $tvar4 $tvar5 $tvar6

 		${If} $tvar6 <= 5
    			StrCpy $Mirror $mirror_009
 		${EndIf}
 		${If} $tvar6 >= 5
    			StrCpy $Mirror $mirror_001
 		${EndIf}
 		${If} $tvar6 >= 10
    			StrCpy $Mirror $mirror_002
 		${EndIf}
 		${If} $tvar6 >= 15
    			StrCpy $Mirror $mirror_003
 		${EndIf}
 		${If} $tvar6 >= 20
    			StrCpy $Mirror $mirror_004
 		${EndIf}
 		${If} $tvar6 >= 30
    			StrCpy $Mirror $mirror_005
 		${EndIf}
 		${If} $tvar6 >= 40
    			StrCpy $Mirror $mirror_006
 		${EndIf}
 		${If} $tvar6 >= 45
    			StrCpy $Mirror $mirror_007
 		${EndIf}
 		${If} $tvar6 >= 50
    			StrCpy $Mirror $mirror_008
 		${EndIf}
	${EndSelect}
FunctionEnd




Function ODownload
	;=== If file exists do nothing
	IfFileExists "$INSTDIR\$OFile" +2
	;nsexec::exectolog `"$EXEDIR\App\wget\wget" --passive-ftp $ProxyAuth $wget_options "$OMirror/$OSource/$OFile"`  
	DetailPrint "Download: $SNAME..."
	ExecWait `"$EXEDIR\App\wget\wget" --passive-ftp $ProxyAuth $wget_options "$OMirror/$OSource/$OFile"` $0
	StrCmp $0 "0" +4
	MessageBox MB_YESNO|MB_ICONSTOP "$(db_stop)" IDNO +3
	call Clean_Up
	Quit
FunctionEnd

Function OInstall
	;=== Install Function for paf installer packages
	${Select} $pbatch
	${Case} "true"
		;=== batch mode, silent install
		StrCmp $debug "false" +2
		MessageBox MB_OK "batch"
		ExecWait `"$INSTDIR\$OFile" /DESTINATION="$PORTABLEAPPSROOTPATH\" /S` 
	${CaseElse}
		StrCmp $debug "false" +2
		MessageBox MB_OK "normal"
		${Select} $OName
		${Case} "1"
			BringToFront
			MessageBox MB_OK "$(self_update)"
			Delete "$EXEDIR\Data\updater.ini"
			Exec `"$INSTDIR\$OFile" /DESTINATION="$PORTABLEAPPSROOTPATH\"`
			call Clean_Up
			Quit
		${CaseElse}
			DetailPrint "Install: $SNAME..."
			ExecWait `"$INSTDIR\$OFile" /DESTINATION="$PORTABLEAPPSROOTPATH\"` 
		${EndSelect}
	${EndSelect}
	;=== check if downloaded files should be deleted
	StrCmp $pdel "false" +3
	Sleep 2000
	Delete "$INSTDIR\$OFile"
	BringToFront
FunctionEnd

Function OExtract
	;=== Install Function for 7-Zip installer packages
	${Select} $pbatch
	${Case} "true"
		;=== batch mode, silent install
		StrCmp $debug "false" +2
		MessageBox MB_OK "Batch"
		ExecWait `"$INSTDIR\$OFile" -y -o"$PORTABLEAPPSROOTPATH\"`
	${CaseElse}
		StrCmp $debug "false" +2
		MessageBox MB_OK "Normal"
		ExecWait `"$INSTDIR\$OFile" -o"$PORTABLEAPPSROOTPATH\"` 
	${EndSelect}
	StrCmp $pdel "false" +3
	Sleep 2000
	Delete "$INSTDIR\$OFile"
	BringToFront
FunctionEnd


Function read_update_ini_mirror
	ReadINIStr $0 "$UFile" "$mversion" "001"
	StrCpy $mirror_001 $0

	ReadINIStr $0 "$UFile" "$mversion" "002"
	StrCpy $mirror_002 $0

	ReadINIStr $0 "$UFile" "$mversion" "003"
	StrCpy $mirror_003 $0

	ReadINIStr $0 "$UFile" "$mversion" "004"
	StrCpy $mirror_004 $0

	ReadINIStr $0 "$UFile" "$mversion" "005"
	StrCpy $mirror_005 $0

	ReadINIStr $0 "$UFile" "$mversion" "006"
	StrCpy $mirror_006 $0

	ReadINIStr $0 "$UFile" "$mversion" "007"
	StrCpy $mirror_007 $0

	ReadINIStr $0 "$UFile" "$mversion" "008"
	StrCpy $mirror_008 $0

	ReadINIStr $0 "$UFile" "$mversion" "009"
	StrCpy $mirror_009 $0
FunctionEnd


Function read_update_ini_OName
	;reset $pselect
	StrCpy "$pselect" "false"	

	ReadINIStr $0 "$UFile" $OName "SName"
	StrCpy "$SName" $0	

	ReadINIStr $0 "$UFile" $OName "PackageVersion"
	StrCpy "$pversion" $0	

	ReadINIStr $0 "$UFile" $OName "DisplayVersion"
	StrCpy "$dversion" $0

	ReadINIStr $0 "$UFile" $OName "Source"
	StrCpy "$OSource" $0

	ReadINIStr $0 "$UFile" $OName "imod"
	StrCpy "$imod" $0

	ReadINIStr $0 "$UFile" $OName "mversion"
	${Select} $0
	${Case} ""
		;=== use default mirrors
		StrCpy "$mversion" "sf_mirrors"
	${CaseElse}
		StrCpy "$mversion" $0
	${EndSelect}

	ReadINIStr $0 "$UFile" $OName "size"
	StrCpy "$OSize" $0

	ReadINIStr $0 "$UFile" $OName "Name"
	StrCpy "$OCaption" $0

	ReadINIStr $0 "$UFile" $OName "description$LANGUAGE"
	${Select} $0 
	${Case} "" 
		ReadINIStr $0 "$UFile" $OName "description"
	${EndSelect}
	StrCpy "$ODescription" $0

	ReadINIStr $0 "$UFile" $OName "pselect"
	StrCmp $0 "" +2
	StrCpy "$pselect" $0

	ReadINIStr $0 "$UFile" $OName "md5sum"
	StrCmp $0 "" +2
	StrCpy "$md5" $0

	call read_update_ini_OFile
FunctionEnd

Function read_settings_ini
	ReadINIStr $0 "$CFile" "Update" "ofile"
	StrCmp $0 "" +2
	StrCpy "$OFile" $0

	ReadINIStr $0 "$CFile" "Update" "batch"
	StrCmp $0 "" +2
	StrCpy "$pbatch" $0

	ReadINIStr $0 "$CFile" "Update" "pselect"
	StrCmp $0 "" +2
	StrCpy "$pselect" $0

	ReadINIStr $0 "$CFile" "Update" "ufile"
	StrCmp $0 "" +2
	StrCpy "$UFile" $0

	ReadINIStr $0 "$CFile" "Update" "tempdir"
	StrCmp $0 "" +2
	StrCpy "$INSTDIR" $0

	ReadINIStr $0 "$CFile" "Update" "wget_options"
	StrCmp $0 "" +2
	StrCpy "$wget_options" $0
	
	ReadINIStr $0 "$CFile" "Cleanup" "pdel"
	StrCmp $0 "" +2
	StrCpy "$pdel" $0

	ReadINIStr $0 "$CFile" "Proxy" "Proxy_Adr"
	StrCmp $0 "" +2
	StrCpy "$Proxy_Adr" $0

	ReadINIStr $0 "$CFile" "Proxy" "Proxy_User"
	StrCmp $0 "" +2
	StrCpy "$Proxy_User" $0

	ReadINIStr $0 "$CFile" "Proxy" "Proxy_PW"
	StrCmp $0 "" +2
	StrCpy "$Proxy_PW" $0

	ReadINIStr $0 "$CFile" "Show" "Welcome"
	StrCmp $0 "" +2
	StrCpy "$wpage" $0

	ReadINIStr $0 "$CFile" "Show" "Components"
	StrCmp $0 "" +2
	StrCpy "$cpage" $0

	ReadINIStr $0 "$CFile" "Show" "Finish"
	StrCmp $0 "" +2
	StrCpy "$fpage" $0

	ReadINIStr $0 "$CFile" "Show" "Debug"
	StrCmp $0 "" +2
	StrCpy "$debug" $0

	ReadINIStr $0 "$CFile" "Show" "NOOK"
	StrCmp $0 "" +2
	StrCpy "$no_ok" $0
	
	ReadINIStr $0 "$CFile" "Show" "OnlyInstalledApps"
	StrCmp $0 "" +2
	StrCpy "$SHOWALLAPPS" $0

	ReadINIStr $0 "$CFile" "General" "Language"
	StrCmp $0 "" +2
	SetEnv::SetEnvVar "PortableApps.comLocaleID" "$0"
FunctionEnd


Function CheckProxySettings
	;Check for proxy settings in ini
	${Select} $Proxy_Adr 
	${Case} "" 
		;no proxy wanted
		StrCmp $debug "false" +2
		MessageBox MB_OK "no proxy"
		GoTo end_proxy
	${Case} "auto"
		;Check for Proxy in user registry
		;If proxy disabled don't use it
		${registry::Read} "HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Internet Settings" "ProxyEnable" $R0 $R1
		StrCmp $R0 "0" end_proxy
		;if Proxyserver empty don't use it
		${registry::Read} "HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Internet Settings" "ProxyServer" $R0 $R1
		StrCmp $R0 "" end_proxy
		${registry::Unload}
		;set proxy to registry proxy
		StrCpy $Proxy_Adr $R0	
	${EndSelect}

	;Check if ftp proxy environment is allready set
	ReadEnvStr $R0 "ftp_proxy"
	StrCmp $R0 "" set_ftp_proxy
	GoTo http_proxy

	;set ftp proxy environment
	set_ftp_proxy:
	SetEnv::SetEnvVar "ftp_proxy" $Proxy_Adr


	http_proxy:
	;Ckeck if ftp proxy environment is allready set
	ReadEnvStr $R0 "http_proxy"
	
	StrCmp $R0 "" set_http_proxy
	GoTo end_proxy
	;set http proxy environment
	set_http_proxy:
	SetEnv::SetEnvVar "http_proxy" $Proxy_Adr

	;===== Set proxy user
	StrCmp $Proxy_User "" +2
	StrCpy "$ProxyAuth" "--proxy-user=$Proxy_User"

	;===== Set proxy PW
	StrCmp $Proxy_PW "" +2
	StrCpy "$ProxyAuth" "$ProxyAuth --proxy-password=$Proxy_PW"

	end_proxy:
	ReadEnvStr $R0 "http_proxy"
	StrCmp $debug "false" +2
	MessageBox MB_OK "function readproxy proxy:$R0"
FunctionEnd

Function read_update_ini_OFile
	StrCmp $debug "false" +2
	MessageBox MB_OK "function read_update_ini_OFile Language: $LANGUAGE UFILE $UFile OName $OName $OName"
	ReadINIStr $0 "$UFile" $OName "$LANGUAGE"
	${Select} "$0"
	${Case} ""
		;No langcode set, use US English
		ReadINIStr $0 "$UFile" $OName "file"
		StrCpy "$OFile" $0
		ReadINIStr $0 "$UFile" $OName "md5sum"
		StrCpy "$md5" $0
	${CaseElse}
		;use langcode from ini
		StrCpy "$OFile" $0
		ReadINIStr $0 "$UFile" $OName "md5sum$LANGUAGE"
		StrCpy "$md5" $0
	${EndSelect}
	
	
	StrCmp $debug "false" +2
	MessageBox MB_OK "function read_update_ini_OFile OFile: $OFile md5:$md5 Odescription:$ODescription"
FunctionEnd

Function OI_select
	BringToFront
	SetOutPath "$INSTDIR"
	call DirectoryLeave
	call read_update_ini_OName
	call read_update_ini_mirror
	call ORandom
	StrCpy $OMirror $Mirror 
	call ODownload
	
	# generate MD5sum of file
	StrCpy $0 "$INSTDIR\$OFile"
	md5dll::GetMD5File "$0"
	Pop $1
	${Select} $1
	${Case} "$md5"
		${Select} "$imod"
		${Case} "e"
			call OExtract
		${CaseElse}
			call OInstall
		${EndSelect}
		;n_ee:
		BringToFront
	${CaseElse}
		MessageBox MB_OK|MB_ICONEXCLAMATION "The MD5 checksums of $OFile do not match. Your download may have been corrupted. Package $INSTDIR\$OFile deleted."
		Delete "$INSTDIR\$OFile"
		
	${EndSelect}
FunctionEnd


Function Show_DL-BANNER 
	;=== Show the banner with default text
	Banner::show /NOUNLOAD "Database Download"
	;=== get the handle for the banner window
	Banner::getWindow /NOUNLOAD
	Pop $1
	;=== wait
	Sleep 1500
	;=== get the handle for the banner text	
	GetDlgItem $2 $1 1030
	;=== set the banner text with new text from variable
	SendMessage $2 ${WM_SETTEXT} 0 "STR:Please wait / $(plswait)..."
	;=== wait
	Sleep 1500
	Banner::destroy
FunctionEnd

Function CustomPageA
${Select} $opage 
${Case} "1"
	; == Set Welcome page on to fix back button issue
	StrCpy $wpage "1"
	!insertmacro MUI_INSTALLOPTIONS_EXTRACT "PortableAppsUpdaterForm.ini"
  	!insertmacro MUI_HEADER_TEXT "$(Op_title)" "$(Op_text)"
	!insertmacro MUI_INSTALLOPTIONS_WRITE "PortableAppsUpdaterForm.ini" "Field 1" "Text" "$(field1)"
	; == read proxy settings from ini pre load the dialog dependig on the ini value
	ReadINIStr $0 "$CFile" "Proxy" "Proxy_Adr"
	
	${Select} $0 
	${Case} ""
		!insertmacro MUI_INSTALLOPTIONS_WRITE "PortableAppsUpdaterForm.ini" "Field 2" "State" "1"
		!insertmacro MUI_INSTALLOPTIONS_WRITE "PortableAppsUpdaterForm.ini" "Field 3" "State" "0"
		!insertmacro MUI_INSTALLOPTIONS_WRITE "PortableAppsUpdaterForm.ini" "Field 4" "State" "0"
	${Case} "auto"
		!insertmacro MUI_INSTALLOPTIONS_WRITE "PortableAppsUpdaterForm.ini" "Field 2" "State" "1"
		!insertmacro MUI_INSTALLOPTIONS_WRITE "PortableAppsUpdaterForm.ini" "Field 3" "State" "0"
		!insertmacro MUI_INSTALLOPTIONS_WRITE "PortableAppsUpdaterForm.ini" "Field 4" "State" "0"
	${Case} "none"
		!insertmacro MUI_INSTALLOPTIONS_WRITE "PortableAppsUpdaterForm.ini" "Field 2" "State" "0"
		!insertmacro MUI_INSTALLOPTIONS_WRITE "PortableAppsUpdaterForm.ini" "Field 3" "State" "1"
		!insertmacro MUI_INSTALLOPTIONS_WRITE "PortableAppsUpdaterForm.ini" "Field 4" "State" "0"
	${CaseElse}
		!insertmacro MUI_INSTALLOPTIONS_WRITE "PortableAppsUpdaterForm.ini" "Field 2" "State" "0"
		!insertmacro MUI_INSTALLOPTIONS_WRITE "PortableAppsUpdaterForm.ini" "Field 3" "State" "0"
		!insertmacro MUI_INSTALLOPTIONS_WRITE "PortableAppsUpdaterForm.ini" "Field 4" "State" "1"
		;!insertmacro MUI_INSTALLOPTIONS_WRITE "PortableAppsUpdaterForm.ini" "Field 5" "Flags" ""
		ReadINIStr $0 "$CFile" "Proxy" "Proxy_Adr"
		!insertmacro MUI_INSTALLOPTIONS_WRITE "PortableAppsUpdaterForm.ini" "Field 5" "State" $0
		;!insertmacro MUI_INSTALLOPTIONS_WRITE "PortableAppsUpdaterForm.ini" "Field 6" "Flags" ""
		ReadINIStr $0 "$CFile" "Proxy" "Proxy_User"
		!insertmacro MUI_INSTALLOPTIONS_WRITE "PortableAppsUpdaterForm.ini" "Field 6" "State" $0
		;!insertmacro MUI_INSTALLOPTIONS_WRITE "PortableAppsUpdaterForm.ini" "Field 6" "Flags" ""
		ReadINIStr $0 "$CFile" "Proxy" "Proxy_PW"
		!insertmacro MUI_INSTALLOPTIONS_WRITE "PortableAppsUpdaterForm.ini" "Field 7" "State" $0
	${EndSelect}
	; == fill all other form objects with values from ini an lang files
	!insertmacro MUI_INSTALLOPTIONS_WRITE "PortableAppsUpdaterForm.ini" "Field 2" "Text" "$(field2)"
	!insertmacro MUI_INSTALLOPTIONS_WRITE "PortableAppsUpdaterForm.ini" "Field 3" "Text" "$(field3)"
	!insertmacro MUI_INSTALLOPTIONS_WRITE "PortableAppsUpdaterForm.ini" "Field 4" "Text" "$(field4)"
	;!insertmacro MUI_INSTALLOPTIONS_WRITE "PortableAppsUpdaterForm.ini" "Field 5" "State" "$(field5)"
	;!insertmacro MUI_INSTALLOPTIONS_WRITE "PortableAppsUpdaterForm.ini" "Field 6" "State" "$(field6)"
	;!insertmacro MUI_INSTALLOPTIONS_WRITE "PortableAppsUpdaterForm.ini" "Field 7" "State" "$(field7)"
	!insertmacro MUI_INSTALLOPTIONS_WRITE "PortableAppsUpdaterForm.ini" "Field 8" "Text" "$(field8)"
	!insertmacro MUI_INSTALLOPTIONS_WRITE "PortableAppsUpdaterForm.ini" "Field 9" "Text" "$(field9)"
	ReadINIStr $0 "$CFile" "Update" "tempdir"
	StrCmp $0 "" +2
		!insertmacro MUI_INSTALLOPTIONS_WRITE "PortableAppsUpdaterForm.ini" "Field 10" "State" "$0"
	!insertmacro MUI_INSTALLOPTIONS_WRITE "PortableAppsUpdaterForm.ini" "Field 11" "Text" "$(field11)"
	ReadINIStr $0 "$CFile" "Show" "OnlyInstalledApps"
	!insertmacro MUI_INSTALLOPTIONS_WRITE "PortableAppsUpdaterForm.ini" "Field 11" "State" $0
	!insertmacro MUI_INSTALLOPTIONS_WRITE "PortableAppsUpdaterForm.ini" "Field 12" "Text" "$(field12)"
	ReadINIStr $0 "$CFile" "Cleanup" "pdel"	
	!insertmacro MUI_INSTALLOPTIONS_WRITE "PortableAppsUpdaterForm.ini" "Field 12" "State" $0
	!insertmacro MUI_INSTALLOPTIONS_WRITE "PortableAppsUpdaterForm.ini" "Field 13" "Text" "$(field13)"
	ReadINIStr $0 "$CFile" "Update" "ufile"
	!insertmacro MUI_INSTALLOPTIONS_WRITE "PortableAppsUpdaterForm.ini" "Field 14" "State" $0
	!insertmacro MUI_INSTALLOPTIONS_WRITE "PortableAppsUpdaterForm.ini" "Field 15" "Text" "$(field15)"
	ReadINIStr $0 "$CFile" "Update" "ofile"
	!insertmacro MUI_INSTALLOPTIONS_WRITE "PortableAppsUpdaterForm.ini" "Field 16" "State" $0
	!insertmacro MUI_INSTALLOPTIONS_WRITE "PortableAppsUpdaterForm.ini" "Field 17" "Text" "$(field17)"
	!insertmacro MUI_INSTALLOPTIONS_WRITE "PortableAppsUpdaterForm.ini" "Field 18" "Text" "$(field18)"
	ReadINIStr $0 "$CFile" "Show" "Welcome"
	!insertmacro MUI_INSTALLOPTIONS_WRITE "PortableAppsUpdaterForm.ini" "Field 18" "State" $0
	!insertmacro MUI_INSTALLOPTIONS_WRITE "PortableAppsUpdaterForm.ini" "Field 19" "Text" "$(field19)"
	ReadINIStr $0 "$CFile" "Show" "Options"
	!insertmacro MUI_INSTALLOPTIONS_WRITE "PortableAppsUpdaterForm.ini" "Field 19" "State" $0
	!insertmacro MUI_INSTALLOPTIONS_WRITE "PortableAppsUpdaterForm.ini" "Field 20" "Text" "$(field20)"
	ReadINIStr $0 "$CFile" "Show" "Components"
	!insertmacro MUI_INSTALLOPTIONS_WRITE "PortableAppsUpdaterForm.ini" "Field 20" "State" $0

	!insertmacro MUI_INSTALLOPTIONS_WRITE "PortableAppsUpdaterForm.ini" "Field 22" "Text" "$(field22)"
	ReadINIStr $0 "$CFile" "Update" "wget_options"
	!insertmacro MUI_INSTALLOPTIONS_WRITE "PortableAppsUpdaterForm.ini" "Field 23" "State" $0

	!insertmacro MUI_INSTALLOPTIONS_WRITE "PortableAppsUpdaterForm.ini" "Field 21" "Text" "$(field21)"
	ReadINIStr $0 "$CFile" "Show" "Finish"
	!insertmacro MUI_INSTALLOPTIONS_WRITE "PortableAppsUpdaterForm.ini" "Field 21" "State" $0
	  ;=== check if options page should be shown
  	;ReadINIStr $0 "$CFile" "Show" "Options"



		!insertmacro MUI_INSTALLOPTIONS_DISPLAY "PortableAppsUpdaterForm.ini"
	${CaseElse}
  		Abort	
	${EndSelect}

FunctionEnd

Function LeaveCustomPageA
	; == read all values from the dialog and set them to the variables/write them to ini
	!insertmacro MUI_INSTALLOPTIONS_READ $R0 "PortableAppsUpdaterForm.ini" "Field 2" "State"
	StrCmp $R0 "0" +4
		WriteINIStr $CFile "Proxy" "Proxy_Adr" "auto"
		WriteINIStr $CFile "Proxy" "Proxy_User" ""
		WriteINIStr $CFile "Proxy" "Proxy_PW" ""

	!insertmacro MUI_INSTALLOPTIONS_READ $R0 "PortableAppsUpdaterForm.ini" "Field 3" "State"
	StrCmp $R0 "0" +2
		WriteINIStr $CFile "Proxy" "Proxy_Adr" ""
		WriteINIStr $CFile "Proxy" "Proxy_User" ""
		!insertmacro MUI_INSTALLOPTIONS_READ $R0 "PortableAppsUpdaterForm.ini" "Field 7" "State"
		WriteINIStr $CFile "Proxy" "Proxy_PW" ""
	!insertmacro MUI_INSTALLOPTIONS_READ $R0 "PortableAppsUpdaterForm.ini" "Field 4" "State"
	StrCmp $R0 "0" +7
		!insertmacro MUI_INSTALLOPTIONS_READ $R0 "PortableAppsUpdaterForm.ini" "Field 5" "State"
		WriteINIStr $CFile "Proxy" "Proxy_Adr" "$R0"
		!insertmacro MUI_INSTALLOPTIONS_READ $R0 "PortableAppsUpdaterForm.ini" "Field 6" "State"
		WriteINIStr $CFile "Proxy" "Proxy_User" "$R0"
		!insertmacro MUI_INSTALLOPTIONS_READ $R0 "PortableAppsUpdaterForm.ini" "Field 7" "State"
		WriteINIStr $CFile "Proxy" "Proxy_PW" "$R0"

	!insertmacro MUI_INSTALLOPTIONS_READ $R0 "PortableAppsUpdaterForm.ini" "Field 10" "State"
	StrCmp $R0 "" +2
		;StrCpy "$INSTDIR" $R0
		WriteINIStr $CFile "Update" "tempdir" "$R0"
	Call DirectoryLeave
	!insertmacro MUI_INSTALLOPTIONS_READ $R0 "PortableAppsUpdaterForm.ini" "Field 11" "State"
	WriteINIStr $CFile "Show" "OnlyInstalledApps" "$R0"

	!insertmacro MUI_INSTALLOPTIONS_READ $R0 "PortableAppsUpdaterForm.ini" "Field 12" "State"
	WriteINIStr $CFile "Cleanup" "pdel" "$R0"

	!insertmacro MUI_INSTALLOPTIONS_READ $R0 "PortableAppsUpdaterForm.ini" "Field 14" "State"
	WriteINIStr $CFile "Update" "ufile" "$R0"

	!insertmacro MUI_INSTALLOPTIONS_READ $R0 "PortableAppsUpdaterForm.ini" "Field 16" "State"
	WriteINIStr $CFile "Update" "ofile" "$R0"

	!insertmacro MUI_INSTALLOPTIONS_READ $R0 "PortableAppsUpdaterForm.ini" "Field 18" "State"
	WriteINIStr $CFile "Show" "Welcome" "$R0"

	!insertmacro MUI_INSTALLOPTIONS_READ $R0 "PortableAppsUpdaterForm.ini" "Field 19" "State"
	WriteINIStr $CFile "Show" "Options" "$R0"

	!insertmacro MUI_INSTALLOPTIONS_READ $R0 "PortableAppsUpdaterForm.ini" "Field 20" "State"
	WriteINIStr $CFile "Show" "Components" "$R0"

	!insertmacro MUI_INSTALLOPTIONS_READ $R0 "PortableAppsUpdaterForm.ini" "Field 21" "State"
	WriteINIStr $CFile "Show" "Finish" "$R0"

	!insertmacro MUI_INSTALLOPTIONS_READ $R0 "PortableAppsUpdaterForm.ini" "Field 23" "State"
	WriteINIStr $CFile "Update" "wget_options" "$R0"
	;Call Updater_Init
	;=== wait
	Sleep 1500

  	${Select} $opage 
	${Case} "1"
		;=== no run for config mode!
		Quit
	${CaseElse}
  		;=== read ini again
		Call read_settings_ini
	${EndSelect}
	;=== write Language to ini
	WriteINIStr $CFile "General" "Language" "$LANGUAGE"
FunctionEnd

Function Clean_Up
	;===CleanUp
	Sleep 2000
	Delete "$EXEDIR\Data\updater.ini"
	${registry::Unload}
	${time::Unload}
	
	;=== BEGIN: POST-INSTALL CODE
	;=== write Language to ini
	WriteINIStr $CFile "General" "Language" "$LANGUAGE"
	
	;EndPostInstallCode:
	;=== END: POST-INSTALL CODE

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
		;StrCmp $debug "false" +2
		;MessageBox MB_OK "POST PA found!"
		

		;=== Check that it's running
		FindProcDLL::FindProc "PortableAppsPlatform.exe"
		StrCmp $R0 "1" "" TheEnd
		
		;=== Send message for the Menu to refresh
		StrCpy $2 'PortableApps.comPlatformWindowMessageToRefresh$0\PortableApps.com\App\PortableAppsPlatform.exe'
		System::Call "user32::RegisterWindowMessage(t r2) i .r3"
		DetailPrint '$(refreshmenu)'
		SendMessage 65535 $3 0 0
	TheEnd:
FunctionEnd

; Section descriptions
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  	!insertmacro MUI_DESCRIPTION_TEXT ${SEC1} $(DESC_Section1)
  	!insertmacro MUI_DESCRIPTION_TEXT ${SEC2} $(DESC_Section2)
  	!insertmacro MUI_DESCRIPTION_TEXT ${SEC3} $(DESC_Section3)
  	!insertmacro MUI_DESCRIPTION_TEXT ${SEC4} $(DESC_Section4)
  	!insertmacro MUI_DESCRIPTION_TEXT ${SEC5} $(DESC_Section5)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC6} $(DESC_Section6)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC7} $(DESC_Section7)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC8} $(DESC_Section8)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC9} $(DESC_Section9)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC10} $(DESC_Section10)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC11} $(DESC_Section11)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC12} $(DESC_Section12)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC13} $(DESC_Section13)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC14} $(DESC_Section14)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC15} $(DESC_Section15)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC16} $(DESC_Section16)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC17} $(DESC_Section17)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC18} $(DESC_Section18)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC19} $(DESC_Section19)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC20} $(DESC_Section20)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC21} $(DESC_Section21)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC22} $(DESC_Section22)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC23} $(DESC_Section23)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC24} $(DESC_Section24)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC25} $(DESC_Section25)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC26} $(DESC_Section26)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC27} $(DESC_Section27)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC28} $(DESC_Section28)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC29} $(DESC_Section29)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC30} $(DESC_Section30)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC31} $(DESC_Section31)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC32} $(DESC_Section32)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC33} $(DESC_Section33)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC34} $(DESC_Section34)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC35} $(DESC_Section35)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC36} $(DESC_Section36)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC37} $(DESC_Section37)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC38} $(DESC_Section38)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC39} $(DESC_Section39)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC40} $(DESC_Section40)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC41} $(DESC_Section41)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC42} $(DESC_Section42)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC43} $(DESC_Section43)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC44} $(DESC_Section44)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC45} $(DESC_Section45)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC46} $(DESC_Section46)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC47} $(DESC_Section49)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC48} $(DESC_Section48)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC49} $(DESC_Section49)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC50} $(DESC_Section50)
  	!insertmacro MUI_DESCRIPTION_TEXT ${SEC51} $(DESC_Section51)
  	!insertmacro MUI_DESCRIPTION_TEXT ${SEC52} $(DESC_Section52)
  	!insertmacro MUI_DESCRIPTION_TEXT ${SEC53} $(DESC_Section53)
  	!insertmacro MUI_DESCRIPTION_TEXT ${SEC54} $(DESC_Section54)
  	!insertmacro MUI_DESCRIPTION_TEXT ${SEC55} $(DESC_Section55)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC56} $(DESC_Section56)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC57} $(DESC_Section57)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC58} $(DESC_Section58)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC59} $(DESC_Section59)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC60} $(DESC_Section60)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC61} $(DESC_Section61)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC62} $(DESC_Section62)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC63} $(DESC_Section63)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC64} $(DESC_Section64)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC65} $(DESC_Section65)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC66} $(DESC_Section66)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC67} $(DESC_Section67)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC68} $(DESC_Section68)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC69} $(DESC_Section69)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC70} $(DESC_Section70)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC71} $(DESC_Section71)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC72} $(DESC_Section72)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC73} $(DESC_Section73)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC74} $(DESC_Section74)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC75} $(DESC_Section75)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC76} $(DESC_Section76)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC77} $(DESC_Section77)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC78} $(DESC_Section78)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC79} $(DESC_Section79)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC80} $(DESC_Section80)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC81} $(DESC_Section81)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC82} $(DESC_Section82)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC83} $(DESC_Section83)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC84} $(DESC_Section84)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC85} $(DESC_Section85)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC86} $(DESC_Section86)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC87} $(DESC_Section87)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC88} $(DESC_Section88)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC89} $(DESC_Section89)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC90} $(DESC_Section90)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC91} $(DESC_Section91)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC92} $(DESC_Section92)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC93} $(DESC_Section93)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC94} $(DESC_Section94)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC95} $(DESC_Section95)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC96} $(DESC_Section96)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC97} $(DESC_Section99)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC98} $(DESC_Section98)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC99} $(DESC_Section99)
	!insertmacro MUI_DESCRIPTION_TEXT ${SEC100} $(DESC_Section100)
!insertmacro MUI_FUNCTION_DESCRIPTION_END

; Section descriptions
