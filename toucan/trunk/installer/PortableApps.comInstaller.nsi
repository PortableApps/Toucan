;Copyright 2007-2009 John T. Haller of PortableApps.com

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

!define PORTABLEAPPSINSTALLERVERSION "0.12.4.0"

!if ${__FILE__} == "PortableApps.comInstallerPlugin.nsi"
	!include PortableApps.comInstallerPluginConfig.nsh
!else
	!include PortableApps.comInstallerConfig.nsh
!endif

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
VIAddVersionKey LegalCopyright "PortableApps.com Installer Copyright 2007-2009 PortableApps.com."
VIAddVersionKey FileDescription "${NAME}"
VIAddVersionKey FileVersion "${VERSION}"
VIAddVersionKey ProductVersion "${VERSION}"
VIAddVersionKey InternalName "${NAME}"
VIAddVersionKey LegalTrademarks "${INSTALLERADDITIONALTRADEMARKS}PortableApps.com is a registered trademark of Rare Ideas, LLC."
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
!include WordFunc.nsh
!ifdef USESVERSIONCOMPARE
	!insertmacro VersionCompare
!endif

!ifdef USESCONFIGREAD
	!insertmacro ConfigRead
!endif
!ifdef USESCONFIGREADS
	!insertmacro ConfigReadS
!endif
!ifdef USESCONFIGWRITE
	!insertmacro ConfigWrite
!endif
!ifdef USESCONFIGWRITES
	!insertmacro ConfigWriteS
!endif

;=== Program Icon
!ifdef PLUGININSTALLER
	Icon "PortableApps.comInstallerPlugin.ico"
	!define MUI_ICON "PortableApps.comInstallerPlugin.ico"
!else
	Icon "..\..\App\AppInfo\appicon.ico"
	!define MUI_ICON "..\..\App\AppInfo\appicon.ico"
!endif

;=== Icon & Stye ===
BrandingText "PortableApps.com - Your Digital Life, Anywhere®"

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
!ifndef INSTALLERMULTILINGUAL
	!insertmacro MUI_LANGUAGE "${INSTALLERLANGUAGE}"
	!include PortableApps.comInstallerLANG_${INSTALLERLANGUAGE}.nsh
!else
	!ifdef USES_ENGLISH
		!insertmacro MUI_LANGUAGE "English"
		!include PortableApps.comInstallerLANG_ENGLISH.nsh
	!endif
	!ifdef USES_AFRIKAANS
		!insertmacro MUI_LANGUAGE "Afrikaans"
		!include PortableApps.comInstallerLANG_AFRIKAANS.nsh
	!endif
	!ifdef USES_ALBANIAN
		!insertmacro MUI_LANGUAGE "Albanian"
		!include PortableApps.comInstallerLANG_ALBANIAN.nsh
	!endif
	!ifdef USES_ARABIC
		!insertmacro MUI_LANGUAGE "Arabic"
		!include PortableApps.comInstallerLANG_ARABIC.nsh
	!endif
	!ifdef USES_BASQUE
		!insertmacro MUI_LANGUAGE "Basque"
		!include PortableApps.comInstallerLANG_BASQUE.nsh
	!endif
	!ifdef USES_BELARUSIAN
		!insertmacro MUI_LANGUAGE "Belarusian"
		!include PortableApps.comInstallerLANG_BELARUSIAN.nsh
	!endif
	!ifdef USES_BOSNIAN
		!insertmacro MUI_LANGUAGE "Bosnian"
		!include PortableApps.comInstallerLANG_BOSNIAN.nsh
	!endif
	!ifdef USES_BRETON
		!insertmacro MUI_LANGUAGE "Breton"
		!include PortableApps.comInstallerLANG_BRETON.nsh
	!endif
	!ifdef USES_BULGARIAN
		!insertmacro MUI_LANGUAGE "Bulgarian"
		!include PortableApps.comInstallerLANG_BULGARIAN.nsh
	!endif
	!ifdef USES_CATALAN
		!insertmacro MUI_LANGUAGE "Catalan"
		!include PortableApps.comInstallerLANG_CATALAN.nsh
	!endif
	!ifdef USES_CROATIAN
		!insertmacro MUI_LANGUAGE "Croatian"
		!include PortableApps.comInstallerLANG_CROATIAN.nsh
	!endif
	!ifdef USES_CZECH
		!insertmacro MUI_LANGUAGE "Czech"
		!include PortableApps.comInstallerLANG_CZECH.nsh
	!endif
	!ifdef USES_DANISH
		!insertmacro MUI_LANGUAGE "Danish"
		!include PortableApps.comInstallerLANG_DANISH.nsh
	!endif
	!ifdef USES_DUTCH
		!insertmacro MUI_LANGUAGE "Dutch"
		!include PortableApps.comInstallerLANG_DUTCH.nsh
	!endif
	!ifdef USES_ESTONIAN
		!insertmacro MUI_LANGUAGE "Estonian"
		!include PortableApps.comInstallerLANG_ESTONIAN.nsh
	!endif
	!ifdef USES_FARSI
		!insertmacro MUI_LANGUAGE "Farsi"
		!include PortableApps.comInstallerLANG_FARSI.nsh
	!endif
	!ifdef USES_FINNISH
		!insertmacro MUI_LANGUAGE "Finnish"
		!include PortableApps.comInstallerLANG_FINNISH.nsh
	!endif
	!ifdef USES_FRENCH
		!insertmacro MUI_LANGUAGE "French"
		!include PortableApps.comInstallerLANG_FRENCH.nsh
	!endif
	!ifdef USES_GALICIAN
		!insertmacro MUI_LANGUAGE "Galician"
		!include PortableApps.comInstallerLANG_GALICIAN.nsh
	!endif
	!ifdef USES_GERMAN
		!insertmacro MUI_LANGUAGE "German"
		!include PortableApps.comInstallerLANG_GERMAN.nsh
	!endif
	!ifdef USES_GREEK
		!insertmacro MUI_LANGUAGE "Greek"
		!include PortableApps.comInstallerLANG_GREEK.nsh
	!endif
	!ifdef USES_HEBREW
		!insertmacro MUI_LANGUAGE "Hebrew"
		!include PortableApps.comInstallerLANG_HEBREW.nsh
	!endif
	!ifdef USES_HUNGARIAN
		!insertmacro MUI_LANGUAGE "Hungarian"
		!include PortableApps.comInstallerLANG_HUNGARIAN.nsh
	!endif
	!ifdef USES_ICELANDIC
		!insertmacro MUI_LANGUAGE "Icelandic"
		!include PortableApps.comInstallerLANG_ICELANDIC.nsh
	!endif
	!ifdef USES_INDONESIAN
		!insertmacro MUI_LANGUAGE "Indonesian"
		!include PortableApps.comInstallerLANG_INDONESIAN.nsh
	!endif
	!ifdef USES_IRISH
		!insertmacro MUI_LANGUAGE "Irish"
		!include PortableApps.comInstallerLANG_IRISH.nsh
	!endif
	!ifdef USES_ITALIAN
		!insertmacro MUI_LANGUAGE "Italian"
		!include PortableApps.comInstallerLANG_ITALIAN.nsh
	!endif
	!ifdef USES_JAPANESE
		!insertmacro MUI_LANGUAGE "Japanese"
		!include PortableApps.comInstallerLANG_JAPANESE.nsh
	!endif
	!ifdef USES_KOREAN
		!insertmacro MUI_LANGUAGE "Korean"
		!include PortableApps.comInstallerLANG_KOREAN.nsh
	!endif
	!ifdef USES_KURDISH
		!insertmacro MUI_LANGUAGE "Kurdish"
		!include PortableApps.comInstallerLANG_KURDISH.nsh
	!endif
	!ifdef USES_LATVIAN
		!insertmacro MUI_LANGUAGE "Latvian"
		!include PortableApps.comInstallerLANG_LATVIAN.nsh
	!endif
	!ifdef USES_LITHUANIAN
		!insertmacro MUI_LANGUAGE "Lithuanian"
		!include PortableApps.comInstallerLANG_LITHUANIAN.nsh
	!endif
	!ifdef USES_LUXEMBOURGISH
		!insertmacro MUI_LANGUAGE "Luxembourgish"
		!include PortableApps.comInstallerLANG_LUXEMBOURGISH.nsh
	!endif
	!ifdef USES_MACEDONIAN
		!insertmacro MUI_LANGUAGE "Macedonian"
		!include PortableApps.comInstallerLANG_MACEDONIAN.nsh
	!endif
	!ifdef USES_MALAY
		!insertmacro MUI_LANGUAGE "Malay"
		!include PortableApps.comInstallerLANG_MALAY.nsh
	!endif
	!ifdef USES_MONGOLIAN
		!insertmacro MUI_LANGUAGE "Mongolian"
		!include PortableApps.comInstallerLANG_MONGOLIAN.nsh
	!endif
	!ifdef USES_NORWEGIAN
		!insertmacro MUI_LANGUAGE "Norwegian"
		!include PortableApps.comInstallerLANG_NORWEGIAN.nsh
	!endif
	!ifdef USES_NORWEGIANNYNORSK
		!insertmacro MUI_LANGUAGE "NorwegianNynorsk"
		!include PortableApps.comInstallerLANG_NORWEGIANNYNORSK.nsh
	!endif
	!ifdef USES_POLISH
		!insertmacro MUI_LANGUAGE "Polish"
		!include PortableApps.comInstallerLANG_POLISH.nsh
	!endif
	!ifdef USES_PORTUGUESE
		!insertmacro MUI_LANGUAGE "Portuguese"
		!include PortableApps.comInstallerLANG_PORTUGUESE.nsh
	!endif
	!ifdef USES_PORTUGUESEBR
		!insertmacro MUI_LANGUAGE "PortugueseBR"
		!include PortableApps.comInstallerLANG_PORTUGUESEBR.nsh
	!endif
	!ifdef USES_ROMANIAN
		!insertmacro MUI_LANGUAGE "Romanian"
		!include PortableApps.comInstallerLANG_ROMANIAN.nsh
	!endif
	!ifdef USES_RUSSIAN
		!insertmacro MUI_LANGUAGE "Russian"
		!include PortableApps.comInstallerLANG_RUSSIAN.nsh
	!endif
	!ifdef USES_SERBIAN
		!insertmacro MUI_LANGUAGE "Serbian"
		!include PortableApps.comInstallerLANG_SERBIAN.nsh
	!endif
	!ifdef USES_SERBIANLATIN
		!insertmacro MUI_LANGUAGE "SerbianLatin"
		!include PortableApps.comInstallerLANG_SERBIANLATIN.nsh
	!endif
	!ifdef USES_SIMPCHINESE
		!insertmacro MUI_LANGUAGE "SimpChinese"
		!include PortableApps.comInstallerLANG_SIMPCHINESE.nsh
	!endif
	!ifdef USES_SLOVAK
		!insertmacro MUI_LANGUAGE "Slovak"
		!include PortableApps.comInstallerLANG_SLOVAK.nsh
	!endif
	!ifdef USES_SLOVENIAN
		!insertmacro MUI_LANGUAGE "Slovenian"
		!include PortableApps.comInstallerLANG_SLOVENIAN.nsh
	!endif
	!ifdef USES_SPANISH
		!insertmacro MUI_LANGUAGE "Spanish"
		!include PortableApps.comInstallerLANG_SPANISH.nsh
	!endif
	!ifdef USES_SPANISHINTERNATIONAL
		!insertmacro MUI_LANGUAGE "SpanishInternational"
		!include PortableApps.comInstallerLANG_SPANISHINTERNATIONAL.nsh
	!endif
	!ifdef USES_SWEDISH
		!insertmacro MUI_LANGUAGE "Swedish"
		!include PortableApps.comInstallerLANG_SWEDISH.nsh
	!endif
	!ifdef USES_THAI
		!insertmacro MUI_LANGUAGE "Thai"
		!include PortableApps.comInstallerLANG_THAI.nsh
	!endif
	!ifdef USES_TRADCHINESE
		!insertmacro MUI_LANGUAGE "TradChinese"
		!include PortableApps.comInstallerLANG_TRADCHINESE.nsh
	!endif
	!ifdef USES_TURKISH
		!insertmacro MUI_LANGUAGE "Turkish"
		!include PortableApps.comInstallerLANG_TURKISH.nsh
	!endif
	!ifdef USES_UKRANIAN
		!insertmacro MUI_LANGUAGE "Ukrainian"
		!include PortableApps.comInstallerLANG_UKRAINIAN.nsh
	!endif
	!ifdef USES_UZBEK
		!insertmacro MUI_LANGUAGE "Uzbek"
		!include PortableApps.comInstallerLANG_UZBEK.nsh
	!endif
	!ifdef USES_WELSH
		!insertmacro MUI_LANGUAGE "Welsh"
		!include PortableApps.comInstallerLANG_WELSH.nsh
	!endif
	
	!insertmacro MUI_RESERVEFILE_LANGDLL
!endif


;=== Variables
Var FOUNDPORTABLEAPPSPATH
!ifdef MAINSECTIONTITLE
	Var OPTIONAL1DONE
!endif

;=== Custom Code
!ifdef USESCUSTOMCODE
	!if ${__FILE__} == "PortableApps.comInstallerPlugin.nsi"
		!include PortableApps.comInstallerPluginCustom.nsh
	!else
		!include PortableApps.comInstallerCustom.nsh
	!endif
!endif

Function .onInit
	SetSilent normal

	!ifdef INSTALLERMULTILINGUAL
		ReadEnvStr $0 "PortableApps.comLocaleID"
		!ifdef USES_ENGLISH
			StrCmp $0 "1033" SetLanguageFromEnvironment ;ENGLISH
		!endif
		!ifdef USES_AFRIKAANS
			StrCmp $0 "1078" SetLanguageFromEnvironment ;AFRIKAANS
		!endif
		!ifdef USES_ALBANIAN
			StrCmp $0 "1052" SetLanguageFromEnvironment ;ALBANIAN
		!endif
		!ifdef USES_ARABIC
			StrCmp $0 "1025" SetLanguageFromEnvironment ;ARABIC
		!endif
		!ifdef USES_BASQUE
			StrCmp $0 "1069" SetLanguageFromEnvironment ;BASQUE
		!endif
		!ifdef USES_BELARUSIAN
			StrCmp $0 "1059" SetLanguageFromEnvironment ;BELARUSIAN
		!endif
		!ifdef USES_BOSNIAN
			StrCmp $0 "5146" SetLanguageFromEnvironment ;BOSNIAN
		!endif
		!ifdef USES_BRETON
			StrCmp $0 "1150" SetLanguageFromEnvironment ;BRETON
		!endif
		!ifdef USES_BULGARIAN
			StrCmp $0 "1026" SetLanguageFromEnvironment ;BULGARIAN
		!endif
		!ifdef USES_CATALAN
			StrCmp $0 "1027" SetLanguageFromEnvironment ;CATALAN
		!endif
		!ifdef USES_CROATIAN
			StrCmp $0 "1050" SetLanguageFromEnvironment ;CROATIAN
		!endif
		!ifdef USES_CZECH
			StrCmp $0 "1029" SetLanguageFromEnvironment ;CZECH
		!endif
		!ifdef USES_DANISH
			StrCmp $0 "1030" SetLanguageFromEnvironment ;DANISH
		!endif
		!ifdef USES_DUTCH
			StrCmp $0 "1043" SetLanguageFromEnvironment ;DUTCH
		!endif
		!ifdef USES_ESTONIAN
			StrCmp $0 "1061" SetLanguageFromEnvironment ;ESTONIAN
		!endif
		!ifdef USES_FARSI
			StrCmp $0 "1065" SetLanguageFromEnvironment ;FARSI
		!endif
		!ifdef USES_FINNISH
			StrCmp $0 "1035" SetLanguageFromEnvironment ;FINNISH
		!endif
		!ifdef USES_FRENCH
			StrCmp $0 "1036" SetLanguageFromEnvironment ;FRENCH
		!endif
		!ifdef USES_GALICIAN
			StrCmp $0 "1110" SetLanguageFromEnvironment ;GALICIAN
		!endif
		!ifdef USES_GERMAN
			StrCmp $0 "1031" SetLanguageFromEnvironment ;GERMAN
		!endif
		!ifdef USES_GREEK
			StrCmp $0 "1032" SetLanguageFromEnvironment ;GREEK
		!endif
		!ifdef USES_HEBREW
			StrCmp $0 "1037" SetLanguageFromEnvironment ;HEBREW
		!endif
		!ifdef USES_HUNGARIAN
			StrCmp $0 "1038" SetLanguageFromEnvironment ;HUNGARIAN
		!endif
		!ifdef USES_ICELANDIC
			StrCmp $0 "1039" SetLanguageFromEnvironment ;ICELANDIC
		!endif
		!ifdef USES_INDONESIAN
			StrCmp $0 "1057" SetLanguageFromEnvironment ;INDONESIAN
		!endif
		!ifdef USES_IRISH
			StrCmp $0 "2108" SetLanguageFromEnvironment ;IRISH
		!endif
		!ifdef USES_ITALIAN
			StrCmp $0 "1040" SetLanguageFromEnvironment ;ITALIAN
		!endif
		!ifdef USES_JAPANESE
			StrCmp $0 "1041" SetLanguageFromEnvironment ;JAPANESE
		!endif
		!ifdef USES_KOREAN
			StrCmp $0 "1042" SetLanguageFromEnvironment ;KOREAN
		!endif
		!ifdef USES_KURDISH
			StrCmp $0 "9999" SetLanguageFromEnvironment ;KURDISH
		!endif
		!ifdef USES_LATVIAN
			StrCmp $0 "1062" SetLanguageFromEnvironment ;LATVIAN
		!endif
		!ifdef USES_LITHUANIAN
			StrCmp $0 "1063" SetLanguageFromEnvironment ;LITHUANIAN
		!endif
		!ifdef USES_LUXEMBOURGISH
			StrCmp $0 "4103" SetLanguageFromEnvironment ;LUXEMBOURGISH
		!endif
		!ifdef USES_MACEDONIAN
			StrCmp $0 "1071" SetLanguageFromEnvironment ;MACEDONIAN
		!endif
		!ifdef USES_MALAY
			StrCmp $0 "1086" SetLanguageFromEnvironment ;MALAY
		!endif
		!ifdef USES_MONGOLIAN
			StrCmp $0 "1104" SetLanguageFromEnvironment ;MONGOLIAN
		!endif
		!ifdef USES_NORWEGIAN
			StrCmp $0 "1044" SetLanguageFromEnvironment ;NORWEGIAN
		!endif
		!ifdef USES_NORWEGIANNYNORSK
			StrCmp $0 "2068" SetLanguageFromEnvironment ;NORWEGIANNYNORSK
		!endif
		!ifdef USES_POLISH
			StrCmp $0 "1045" SetLanguageFromEnvironment ;POLISH
		!endif
		!ifdef USES_PORTUGUESE
			StrCmp $0 "2070" SetLanguageFromEnvironment ;PORTUGUESE
		!endif
		!ifdef USES_PORTUGUESEBR
			StrCmp $0 "1046" SetLanguageFromEnvironment ;PORTUGUESEBR
		!endif
		!ifdef USES_ROMANIAN
			StrCmp $0 "1048" SetLanguageFromEnvironment ;ROMANIAN
		!endif
		!ifdef USES_RUSSIAN
			StrCmp $0 "1049" SetLanguageFromEnvironment ;RUSSIAN
		!endif
		!ifdef USES_SERBIAN
			StrCmp $0 "3098" SetLanguageFromEnvironment ;SERBIAN
		!endif
		!ifdef USES_SERBIANLATIN
			StrCmp $0 "2074" SetLanguageFromEnvironment ;SERBIANLATIN
		!endif
		!ifdef USES_SIMPCHINESE
			StrCmp $0 "2052" SetLanguageFromEnvironment ;SIMPCHINESE
		!endif
		!ifdef USES_SLOVAK
			StrCmp $0 "1051" SetLanguageFromEnvironment ;SLOVAK
		!endif
		!ifdef USES_SLOVENIAN
			StrCmp $0 "1060" SetLanguageFromEnvironment ;SLOVENIAN
		!endif
		!ifdef USES_SPANISH
			StrCmp $0 "1034" SetLanguageFromEnvironment ;SPANISH
		!endif
		!ifdef USES_SPANISHINTERNATIONAL
			StrCmp $0 "3082" SetLanguageFromEnvironment ;SPANISHINTERNATIONAL
		!endif
		!ifdef USES_SWEDISH
			StrCmp $0 "1053" SetLanguageFromEnvironment ;SWEDISH
		!endif
		!ifdef USES_THAI
			StrCmp $0 "1054" SetLanguageFromEnvironment ;THAI
		!endif
		!ifdef USES_TRADCHINESE
			StrCmp $0 "1028" SetLanguageFromEnvironment ;TRADCHINESE
		!endif
		!ifdef USES_TURKISH
			StrCmp $0 "1055" SetLanguageFromEnvironment ;TURKISH
		!endif
		!ifdef USES_UKRAINIAN
			StrCmp $0 "1058" SetLanguageFromEnvironment ;UKRAINIAN
		!endif
		!ifdef USES_UZBEK
			StrCmp $0 "1091" SetLanguageFromEnvironment ;UZBEK
		!endif
		!ifdef USES_WELSH
			StrCmp $0 "1160" SetLanguageFromEnvironment ;WELSH
		!endif
		
		Goto ShowLanguageSelector ;None of the above
		
		SetLanguageFromEnvironment:
			StrCpy $LANGUAGE $0
			Goto GetCommandLineOptions

		ShowLanguageSelector:
			!insertmacro MUI_LANGDLL_DISPLAY

		GetCommandLineOptions:
	!endif
	
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
		!ifdef MAINSECTIONTITLE
			!ifdef OPTIONALSECTIONPRESELECTEDIFNONENGLISHINSTALL
				StrCmp $LANGUAGE "1033" LeaveUnselected
					SectionSetFlags 1 ${OPTIONALSECTIONIDX}

				LeaveUnselected:
			!endif
		!endif
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

					!ifndef PLUGININSTALLER ;=== If not a plugin installer, add the current install size to free space
						${GetSize} `$INSTDIR` "/M=*.* /S=0K /G=1" $4 $5 $6 ;=== Current installation size
						IntOp $3 $3 + $4 ;=== Space Free + Current Install Size
					!else
						Goto CalculateSpaceLeft
					!endif

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
		!ifmacrodef CustomCodeOptionalCleanup
			!insertmacro CustomCodeOptionalCleanup
		!endif
		;=== END: OPTIONAL NOT SELECTED CLEANUP CODE ===
	MainSkipOptionalCleanup:
!endif
	
	;=== Rename the preserved files so they're not deleted in the next part
	!ifdef PRESERVEFILE1
		Rename `$INSTDIR\${PRESERVEFILE1}` `$INSTDIR\~PRESERVEFILE1`
	!endif
	!ifdef PRESERVEFILE2
		Rename `$INSTDIR\${PRESERVEFILE2}` `$INSTDIR\~PRESERVEFILE2`
	!endif
	!ifdef PRESERVEFILE3
		Rename `$INSTDIR\${PRESERVEFILE3}` `$INSTDIR\~PRESERVEFILE3`
	!endif
	!ifdef PRESERVEFILE4
		Rename `$INSTDIR\${PRESERVEFILE4}` `$INSTDIR\~PRESERVEFILE4`
	!endif
	!ifdef PRESERVEFILE5
		Rename `$INSTDIR\${PRESERVEFILE5}` `$INSTDIR\~PRESERVEFILE5`
	!endif
	!ifdef PRESERVEFILE6
		Rename `$INSTDIR\${PRESERVEFILE6}` `$INSTDIR\~PRESERVEFILE6`
	!endif
	!ifdef PRESERVEFILE7
		Rename `$INSTDIR\${PRESERVEFILE7}` `$INSTDIR\~PRESERVEFILE7`
	!endif
	!ifdef PRESERVEFILE8
		Rename `$INSTDIR\${PRESERVEFILE8}` `$INSTDIR\~PRESERVEFILE8`
	!endif
	!ifdef PRESERVEFILE9
		Rename `$INSTDIR\${PRESERVEFILE9}` `$INSTDIR\~PRESERVEFILE9`
	!endif
	!ifdef PRESERVEFILE10
		Rename `$INSTDIR\${PRESERVEFILE10}` `$INSTDIR\~PRESERVEFILE10`
	!endif
	
	;=== Rename the preserved directories so they're not deleted in the next part
	!ifdef PRESERVEDIRECTORY1
		Rename `$INSTDIR\${PRESERVEDIRECTORY1}\` `$INSTDIR\~PRESERVEDIRECTORY1\`
	!endif
	!ifdef PRESERVEDIRECTORY2
		Rename `$INSTDIR\${PRESERVEDIRECTORY2}\` `$INSTDIR\~PRESERVEDIRECTORY2\`
	!endif
	!ifdef PRESERVEDIRECTORY3
		Rename `$INSTDIR\${PRESERVEDIRECTORY3}\` `$INSTDIR\~PRESERVEDIRECTORY3\`
	!endif
	!ifdef PRESERVEDIRECTORY4
		Rename `$INSTDIR\${PRESERVEDIRECTORY4}\` `$INSTDIR\~PRESERVEDIRECTORY4\`
	!endif
	!ifdef PRESERVEDIRECTORY5
		Rename `$INSTDIR\${PRESERVEDIRECTORY5}\` `$INSTDIR\~PRESERVEDIRECTORY5\`
	!endif
	!ifdef PRESERVEDIRECTORY6
		Rename `$INSTDIR\${PRESERVEDIRECTORY6}\` `$INSTDIR\~PRESERVEDIRECTORY6\`
	!endif
	!ifdef PRESERVEDIRECTORY7
		Rename `$INSTDIR\${PRESERVEDIRECTORY7}\` `$INSTDIR\~PRESERVEDIRECTORY7\`
	!endif
	!ifdef PRESERVEDIRECTORY8
		Rename `$INSTDIR\${PRESERVEDIRECTORY8}\` `$INSTDIR\~PRESERVEDIRECTORY8\`
	!endif
	!ifdef PRESERVEDIRECTORY9
		Rename `$INSTDIR\${PRESERVEDIRECTORY9}\` `$INSTDIR\~PRESERVEDIRECTORY9\`
	!endif
	!ifdef PRESERVEDIRECTORY10
		Rename `$INSTDIR\${PRESERVEDIRECTORY10}\` `$INSTDIR\~PRESERVEDIRECTORY10\`
	!endif
	
	;=== Remove main directories if necessary
	!ifdef REMOVEAPPDIRECTORY
		RMDir /r `$INSTDIR\App`
	!endif
	!ifdef REMOVEDATADIRECTORY
		RMDir /r `$INSTDIR\Data`
	!endif
	!ifdef REMOVEOTHERDIRECTORY
		RMDir /r `$INSTDIR\Other`
	!endif
	
	;=== Remove specific directories
	!ifdef REMOVEDIRECTORY1
		RMDir /r `$INSTDIR\${REMOVEDIRECTORY1}`
	!endif
	!ifdef REMOVEDIRECTORY2
		RMDir /r `$INSTDIR\${REMOVEDIRECTORY2}`
	!endif
	!ifdef REMOVEDIRECTORY3
		RMDir /r `$INSTDIR\${REMOVEDIRECTORY3}`
	!endif
	!ifdef REMOVEDIRECTORY4
		RMDir /r `$INSTDIR\${REMOVEDIRECTORY4}`
	!endif
	!ifdef REMOVEDIRECTORY5
		RMDir /r `$INSTDIR\${REMOVEDIRECTORY5}`
	!endif
	!ifdef REMOVEDIRECTORY6
		RMDir /r `$INSTDIR\${REMOVEDIRECTORY6}`
	!endif
	!ifdef REMOVEDIRECTORY7
		RMDir /r `$INSTDIR\${REMOVEDIRECTORY7}`
	!endif
	!ifdef REMOVEDIRECTORY8
		RMDir /r `$INSTDIR\${REMOVEDIRECTORY8}`
	!endif
	!ifdef REMOVEDIRECTORY9
		RMDir /r `$INSTDIR\${REMOVEDIRECTORY9}`
	!endif
	!ifdef REMOVEDIRECTORY10
		RMDir /r `$INSTDIR\${REMOVEDIRECTORY10}`
	!endif
	
	;=== Remove specific files
	!ifdef REMOVEFILE1
		Delete `$INSTDIR\${REMOVEFILE1}` 
	!endif
	!ifdef REMOVEFILE2
		Delete `$INSTDIR\${REMOVEFILE2}` 
	!endif
	!ifdef REMOVEFILE3
		Delete `$INSTDIR\${REMOVEFILE3}` 
	!endif
	!ifdef REMOVEFILE4
		Delete `$INSTDIR\${REMOVEFILE4}` 
	!endif
	!ifdef REMOVEFILE5
		Delete `$INSTDIR\${REMOVEFILE5}` 
	!endif
	!ifdef REMOVEFILE6
		Delete `$INSTDIR\${REMOVEFILE6}` 
	!endif
	!ifdef REMOVEFILE7
		Delete `$INSTDIR\${REMOVEFILE7}` 
	!endif
	!ifdef REMOVEFILE8
		Delete `$INSTDIR\${REMOVEFILE8}` 
	!endif
	!ifdef REMOVEFILE9
		Delete `$INSTDIR\${REMOVEFILE9}` 
	!endif
	!ifdef REMOVEFILE10
		Delete `$INSTDIR\${REMOVEFILE10}` 
	!endif
	
	;=== Rename the preserved directories back to their proper names
	!ifdef PRESERVEDIRECTORY1
		${GetParent} `$INSTDIR\${PRESERVEDIRECTORY1}\` $R0
		CreateDirectory $R0
		Rename `$INSTDIR\~PRESERVEDIRECTORY1\` `$INSTDIR\${PRESERVEDIRECTORY1}\`
	!endif
	!ifdef PRESERVEDIRECTORY2
		${GetParent} `$INSTDIR\${PRESERVEDIRECTORY2}\` $R0
		CreateDirectory $R0
		Rename `$INSTDIR\~PRESERVEDIRECTORY2\` `$INSTDIR\${PRESERVEDIRECTORY2}\`
	!endif
	!ifdef PRESERVEDIRECTORY3
		${GetParent} `$INSTDIR\${PRESERVEDIRECTORY3}\` $R0
		CreateDirectory $R0
		Rename `$INSTDIR\~PRESERVEDIRECTORY3\` `$INSTDIR\${PRESERVEDIRECTORY3}\`
	!endif
	!ifdef PRESERVEDIRECTORY4
		${GetParent} `$INSTDIR\${PRESERVEDIRECTORY4}\` $R0
		CreateDirectory $R0
		Rename `$INSTDIR\~PRESERVEDIRECTORY4\` `$INSTDIR\${PRESERVEDIRECTORY4}\`
	!endif
	!ifdef PRESERVEDIRECTORY5
		${GetParent} `$INSTDIR\${PRESERVEDIRECTORY5}\` $R0
		CreateDirectory $R0
		Rename `$INSTDIR\~PRESERVEDIRECTORY5\` `$INSTDIR\${PRESERVEDIRECTORY5}\`
	!endif
	!ifdef PRESERVEDIRECTORY6
		${GetParent} `$INSTDIR\${PRESERVEDIRECTORY6}\` $R0
		CreateDirectory $R0
		Rename `$INSTDIR\~PRESERVEDIRECTORY6\` `$INSTDIR\${PRESERVEDIRECTORY6}\`
	!endif
	!ifdef PRESERVEDIRECTORY7
		${GetParent} `$INSTDIR\${PRESERVEDIRECTORY7}\` $R0
		CreateDirectory $R0
		Rename `$INSTDIR\~PRESERVEDIRECTORY7\` `$INSTDIR\${PRESERVEDIRECTORY7}\`
	!endif
	!ifdef PRESERVEDIRECTORY8
		${GetParent} `$INSTDIR\${PRESERVEDIRECTORY8}\` $R0
		CreateDirectory $R0
		Rename `$INSTDIR\~PRESERVEDIRECTORY8\` `$INSTDIR\${PRESERVEDIRECTORY8}\`
	!endif
	!ifdef PRESERVEDIRECTORY9
		${GetParent} `$INSTDIR\${PRESERVEDIRECTORY9}\` $R0
		CreateDirectory $R0
		Rename `$INSTDIR\~PRESERVEDIRECTORY9\` `$INSTDIR\${PRESERVEDIRECTORY9}\`
	!endif
	!ifdef PRESERVEDIRECTORY10
		${GetParent} `$INSTDIR\${PRESERVEDIRECTORY10}\` $R0
		CreateDirectory $R0
		Rename `$INSTDIR\~PRESERVEDIRECTORY10\` `$INSTDIR\${PRESERVEDIRECTORY10}\`
	!endif
	
	;=== Rename the preserved files back to their proper names
	!ifdef PRESERVEFILE1
		${GetParent} `$INSTDIR\${PRESERVEFILE1}` $R0
		CreateDirectory $R0
		Rename `$INSTDIR\~PRESERVEFILE1` `$INSTDIR\${PRESERVEFILE1}`
	!endif
	!ifdef PRESERVEFILE2
		${GetParent} `$INSTDIR\${PRESERVEFILE2}` $R0
		CreateDirectory $R0
		Rename `$INSTDIR\~PRESERVEFILE2` `$INSTDIR\${PRESERVEFILE2}`
	!endif
	!ifdef PRESERVEFILE3
		${GetParent} `$INSTDIR\${PRESERVEFILE3}` $R0
		CreateDirectory $R0
		Rename `$INSTDIR\~PRESERVEFILE3` `$INSTDIR\${PRESERVEFILE3}`
	!endif
	!ifdef PRESERVEFILE4
		${GetParent} `$INSTDIR\${PRESERVEFILE4}` $R0
		CreateDirectory $R0
		Rename `$INSTDIR\~PRESERVEFILE4` `$INSTDIR\${PRESERVEFILE4}`
	!endif
	!ifdef PRESERVEFILE5
		${GetParent} `$INSTDIR\${PRESERVEFILE5}` $R0
		CreateDirectory $R0
		Rename `$INSTDIR\~PRESERVEFILE5` `$INSTDIR\${PRESERVEFILE5}`
	!endif
	!ifdef PRESERVEFILE6
		${GetParent} `$INSTDIR\${PRESERVEFILE6}` $R0
		CreateDirectory $R0
		Rename `$INSTDIR\~PRESERVEFILE6` `$INSTDIR\${PRESERVEFILE6}`
	!endif
	!ifdef PRESERVEFILE7
		${GetParent} `$INSTDIR\${PRESERVEFILE7}` $R0
		CreateDirectory $R0
		Rename `$INSTDIR\~PRESERVEFILE7` `$INSTDIR\${PRESERVEFILE7}`
	!endif
	!ifdef PRESERVEFILE8
		${GetParent} `$INSTDIR\${PRESERVEFILE8}` $R0
		CreateDirectory $R0
		Rename `$INSTDIR\~PRESERVEFILE8` `$INSTDIR\${PRESERVEFILE8}`
	!endif
	!ifdef PRESERVEFILE9
		${GetParent} `$INSTDIR\${PRESERVEFILE9}` $R0
		CreateDirectory $R0
		Rename `$INSTDIR\~PRESERVEFILE9` `$INSTDIR\${PRESERVEFILE9}`
	!endif
	!ifdef PRESERVEFILE10
		${GetParent} `$INSTDIR\${PRESERVEFILE10}` $R0
		CreateDirectory $R0
		Rename `$INSTDIR\~PRESERVEFILE10` `$INSTDIR\${PRESERVEFILE10}`
	!endif
	
	;=== BEGIN: PRE-INSTALL CODE ===
	;This will be executed before the app is installed.  Useful for cleaning up files no longer used.
	!ifmacrodef CustomCodePreInstall
		!insertmacro CustomCodePreInstall
	!endif
	;=== END: PRE-INSTALL CODE ===
	
	!ifndef PLUGININSTALLER
		File "..\..\*.*"
	!endif
	SetOutPath $INSTDIR\App
	File /r "..\..\App\*.*"
	SetOutPath $INSTDIR\Other
	File /r "..\..\Other\*.*"
	CreateDirectory "$INSTDIR\Data"
	
	;=== BEGIN: POST-INSTALL CODE ===
	;This will be executed after the app is installed.  Useful for updating configuration files.
	!ifmacrodef CustomCodePostInstall
		!insertmacro CustomCodePostInstall
	!endif	
	;=== END: POST-INSTALL CODE ===
	
	;=== Refresh PortableApps.com Menu (not final version)
	${GetParent} `$INSTDIR` $0
	;=== Check that it exists at the right location
	DetailPrint '$(checkforplatform)'
	IfFileExists `$0\PortableApps.com\PortableAppsPlatform.exe` "" TheEnd
		;=== Check that it's the real deal so we aren't hanging with no response
		MoreInfo::GetProductName `$0\PortableApps.com\PortableAppsPlatform.exe`
		Pop $1
		StrCmp $1 "PortableApps.com Platform" "" TheEnd
		MoreInfo::GetCompanyName `$0\PortableApps.com\PortableAppsPlatform.exe`
		Pop $1
		StrCmp $1 "PortableApps.com" "" TheEnd
		
		;=== Check that it's running
		FindProcDLL::FindProc "PortableAppsPlatform.exe"
		StrCmp $R0 "1" "" TheEnd
		
		;=== Send message for the Menu to refresh
		DetailPrint '$(refreshmenu)'
		IfFileExists `$0\PortableApps.com\App\PortableAppsPlatform.exe` SendOldRefreshMessage
			StrCpy $2 'PortableApps.comPlatformWindowMessageToRefresh$0\PortableApps.com\PortableAppsPlatform.exe'
			System::Call "user32::RegisterWindowMessage(t r2) i .r3"
			SendMessage 65535 $3 0 0 /TIMEOUT=1
			Goto TheEnd
		SendOldRefreshMessage:
			StrCpy $2 'PortableApps.comPlatformWindowMessageToRefresh$0\PortableApps.com\App\PortableAppsPlatform.exe'
			System::Call "user32::RegisterWindowMessage(t r2) i .r3"
			SendMessage 65535 $3 0 0 /TIMEOUT=1
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
			StrCmp "${OPTIONALSECTIONNOTSELECTEDAPPINFOSUFFIX}" "" SecUpdateAppInfoTheEnd
				ReadINIStr $0 "$INSTDIR\App\AppInfo\appinfo.ini" "Version" "DisplayVersion"
				WriteINIStr "$INSTDIR\App\AppInfo\appinfo.ini" "Version" "DisplayVersion" "$0 ${OPTIONALSECTIONNOTSELECTEDAPPINFOSUFFIX}"
			Goto SecUpdateAppInfoTheEnd

		SecUpdateAppInfoOptionalSelected:
			StrCmp "${OPTIONALSECTIONSELECTEDAPPINFOSUFFIX}" "" SecUpdateAppInfoTheEnd
				ReadINIStr $0 "$INSTDIR\App\AppInfo\appinfo.ini" "Version" "DisplayVersion"
				WriteINIStr "$INSTDIR\App\AppInfo\appinfo.ini" "Version" "DisplayVersion" "$0 ${OPTIONALSECTIONSELECTEDAPPINFOSUFFIX}"
		SecUpdateAppInfoTheEnd:
	SectionEnd

	!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
		!insertmacro MUI_DESCRIPTION_TEXT ${MAINSECTIONIDX} "${MAINSECTIONDESCRIPTION}"
		!insertmacro MUI_DESCRIPTION_TEXT ${OPTIONALSECTIONIDX} "${OPTIONALSECTIONDESCRIPTION}"
	!insertmacro MUI_FUNCTION_DESCRIPTION_END
!endif