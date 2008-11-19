;THIS IS A SPECIALIZED VERSION OF THE PORTABLEAPPS.COM INSTALLER AND SHOULD NOT BE USED WITH OTHER APPS

;Copyright 2007-2008 John T. Haller of PortableApps.com

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

!define PORTABLEAPPSINSTALLERVERSION "0.10.1.0"
!include PortableApps.comInstallerConfig.nsh

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
VIAddVersionKey LegalCopyright "PortableApps.com Installer Copyright 2007-2008 PortableApps.com."
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
;=== THESE BITS WERE ADDED AND ARE NOT YET PART OF THE PORTABLEAPPS.COM INSTALLER
!insertmacro MUI_LANGUAGE "English"
!include PortableApps.comInstallerLANG_ENGLISH.nsh
!insertmacro MUI_LANGUAGE "Dutch"
!include PortableApps.comInstallerLANG_DUTCH.nsh
!insertmacro MUI_LANGUAGE "Estonian"
!include PortableApps.comInstallerLANG_ESTONIAN.nsh
!insertmacro MUI_LANGUAGE "French"
!include PortableApps.comInstallerLANG_FRENCH.nsh
!insertmacro MUI_LANGUAGE "German"
!include PortableApps.comInstallerLANG_GERMAN.nsh
!insertmacro MUI_LANGUAGE "Russian"
!include PortableApps.comInstallerLANG_RUSSIAN.nsh
!insertmacro MUI_LANGUAGE "Spanish"
!include PortableApps.comInstallerLANG_SPANISH.nsh
!insertmacro MUI_LANGUAGE "Polish"
!include PortableApps.comInstallerLANG_POLISH.nsh
!insertmacro MUI_LANGUAGE "Hungarian"
!include PortableApps.comInstallerLANG_HUNGARIAN.nsh
!insertmacro MUI_LANGUAGE "Bulgarian"
!include PortableApps.comInstallerLANG_BULGARIAN.nsh
!insertmacro MUI_LANGUAGE "Korean"
!include PortableApps.comInstallerLANG_KOREAN.nsh

!insertmacro MUI_RESERVEFILE_LANGDLL
;=== END ADDED BITS

;=== Variables
Var FOUNDPORTABLEAPPSPATH
!ifdef MAINSECTIONTITLE
	Var OPTIONAL1DONE
!endif

;=== Custom Code
!ifdef USESCUSTOMCODE
	!include PortableApps.comInstallerCustom.nsh
!endif

Function .onInit
	;=== THESE BITS WERE ADDED AND ARE NOT YET PART OF THE PORTABLEAPPS.COM INSTALLER
	ReadEnvStr $0 "PortableApps.comLocaleID"
	StrCmp $0 "1061" SetLanguageFromEnvironment ;Estonian
	StrCmp $0 "1034" SetLanguageFromEnvironment ;Spanish
	StrCmp $0 "1049" SetLanguageFromEnvironment ;Russian
	StrCmp $0 "1043" SetLanguageFromEnvironment ;Dutch
	StrCmp $0 "1031" SetLanguageFromEnvironment ;German
	StrCmp $0 "1036" SetLanguageFromEnvironment ;French
	StrCmp $0 "1045" SetLanguageFromEnvironment ;Polish
	StrCmp $0 "1038" SetLanguageFromEnvironment ;Hungarian
	StrCmp $0 "1026" SetLanguageFromEnvironment ;Bulgarian
	StrCmp $0 "1042" SetLanguageFromEnvironment ;Korean
	StrCmp $0 "1033" SetLanguageFromEnvironment ShowLanguageSelector ;English
	
	SetLanguageFromEnvironment:
		StrCpy $LANGUAGE $0
		Goto GetCommandLineOptions

	ShowLanguageSelector:
		!insertmacro MUI_LANGDLL_DISPLAY

	GetCommandLineOptions:
	;=== END ADDED BITS
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
	
	File "..\..\*.*"
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