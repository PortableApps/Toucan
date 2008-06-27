;Copyright 2007 John T. Haller

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

;EXCEPTION: Can be used with non-GPLed apps distributed by PortableApps.com

;=== BEGIN: BASIC INFORMATION
!define NAME "PortableApps.com Updater"
!define SHORTNAME "PortableAppsUpdater"
!define VERSION "0.9.0.9"
!define FILENAME "PA_UPDATER_1.0_Alpha9"
!define CHECKRUNNING "PortableAppsUpdater.exe"
!define CLOSENAME "PortableApps Updater"
!define ADDONSDIRECTORYPRESERVE "NONE"
!define PORTABLEAPPSINSTALLERVERSION "0.9.9.0"
!define INSTALLERCOMMENTS "For additional details, visit PortableApps.com"
!define INSTALLERADDITIONALTRADEMARKS "" ;end this entry with a period and a space if used
!define INSTALLERLEGALCOPYRIGHT "PortableApps.com and contributors"
;!define LICENSEAGREEMENT "eula.rtf"
; NOTE: For no license agreement, comment out the above line by placing a semicolon at the start of it
;=== END: BASIC INFORMATION

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

;=== Program Icon
Icon "..\..\App\AppInfo\appicon.ico"

;=== Icon & Stye ===
!define MUI_ICON "..\..\App\AppInfo\appicon.ico"
BrandingText "PortableApps.com - Your Digital Life, Anywhereô"

;=== Pages
!define MUI_WELCOMEFINISHPAGE_BITMAP "PortableApps.comInstaller.bmp"
!define MUI_WELCOMEPAGE_TITLE "${NAME}"
!define MUI_WELCOMEPAGE_TEXT "$(welcome)"
!insertmacro MUI_PAGE_WELCOME
!ifdef LICENSEAGREEMENT
	!define MUI_LICENSEPAGE_RADIOBUTTONS
	!insertmacro MUI_PAGE_LICENSE "${LICENSEAGREEMENT}"
!endif
!define MUI_PAGE_CUSTOMFUNCTION_LEAVE LeaveDirectory
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!define MUI_FINISHPAGE_TEXT "$(finish)"
;!define MUI_FINISHPAGE_RUN "$INSTDIR\PortableAppsUpdater.exe"
!insertmacro MUI_PAGE_FINISH

;=== Languages
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "French"
!insertmacro MUI_LANGUAGE "German"
!insertmacro MUI_LANGUAGE "Italian"
!insertmacro MUI_LANGUAGE "Portuguese"
!insertmacro MUI_LANGUAGE "Spanish"
!insertmacro MUI_LANGUAGE "Japanese"

!insertmacro MUI_RESERVEFILE_LANGDLL

LangString welcome ${LANG_ENGLISH} "This wizard will guide you through the installation of ${NAME}.\r\n\r\nIf you are upgrading an existing installation of ${NAME}, please close it before proceeding.\r\n\r\nClick Next to continue."
LangString finish ${LANG_ENGLISH} "${NAME} has been installed on your device.\r\n\r\nClick Finish to close this wizard."
LangString runwarning ${LANG_ENGLISH} "All instances of ${CLOSENAME} will close for update, cause it can not be upgraded while it is running."
LangString invaliddirectory ${LANG_ENGLISH} "The destination folder you selected is invalid.  Please choose a valid folder."
LangString notenoughspace ${LANG_ENGLISH} "The device you have selected to install to does not have enough free space for this app."
LangString checkforplatform ${LANG_ENGLISH} "Checking for PortableApps.com Platform"
LangString refreshmenu ${LANG_ENGLISH} "Refreshing PortableApps.com Menu"

LangString welcome ${LANG_FRENCH} "Cet assistant vous guidera tout au long de l'installation de ${NAME}.\r\n\r\nSi vous mettez ‡ jour une installation existante de ${NAME}, veuillez l'arrÍter avant de continuer.\r\n\r\nCliquez sur Next pour continuer."
LangString finish ${LANG_FRENCH} "${NAME} a ÈtÈ installÈ sur votre disque.\r\n\r\nCliquez sur Finish pour fermer cet assistant."
LangString runwarning ${LANG_FRENCH} "Veuillez fermer toutes les instances de ${CLOSENAME} et cliquez ensuite sur OK. L'application portable ne peut pas Ítre mise ‡ jour tant qu'il fonctionne."
LangString invaliddirectory ${LANG_FRENCH} "Le dossier de destination que vous avez sÈlectionnÈ est invalide. Veuillez choisir un dossier valide."
LangString notenoughspace ${LANG_FRENCH} "Le disque que vous avez sÈlectionnÈ pour l'installation, ne contient pas assez d'espace libre pour cette application."
LangString checkforplatform ${LANG_FRENCH} "Recherche de la Plateforme PortableApps.com"
LangString refreshmenu ${LANG_FRENCH} "Actualisation du Menu PortableApps.com"

LangString welcome ${LANG_GERMAN} "Dieser Assistent wird Sie bei der Installation von ${NAME} unterst¸tzen.\r\n\r\nWenn Sie eine existierende Installation von ${NAME} aktualisieren wollen, schliessen Sie sie bitte, bevor Sie fortfahren.\r\n\r\nKlicken Sie bitte auf Weiter, um fortzusetzen."
LangString finish ${LANG_GERMAN} "${NAME} wurde auf Ihrem Laufwerk installiert.\r\n\r\nBitte Beenden klicken, um diesen Assistenten zu verlassen."
LangString runwarning ${LANG_GERMAN} "Alle Instanzen von ${CLOSENAME} werden geschlossen, wenn Sie auf OK klicken. Die Anwendung kann nicht aktualisiert werden, solange sie l‰uft."
LangString invaliddirectory ${LANG_GERMAN} "Das ausgew‰hlte Zielverzeichnis ist ung¸ltig!.  Bitte w‰hlen Sie ein g¸ltiges Verzeichnis."
LangString notenoughspace ${LANG_GERMAN} "Das Laufwerk, welches Sie zur Installation ausgew‰hlt haben, hat nicht genug freien Speicherplatz f¸r diese Anwendung."
LangString checkforplatform ${LANG_GERMAN} "PortableApps.com Platform ¸berpr¸fen"
LangString refreshmenu ${LANG_GERMAN} "PortableApps.com Men¸ neu laden"

LangString welcome ${LANG_ITALIAN} "Questa procedura vi guider‡ durante l'installazione di PortableApps.\r\n\r\nSe state aggiornando una installazione gi‡ esistente di PortableApss, vi invitiamo a chiuderla prima di procedere.\r\n\r\nPremere  su Continua per proseguire."
LangString finish ${LANG_ITALIAN} "${NAME} Ë stato installato sulla vostra periferica.\r\n\r\nPremere Fine per terminare questa procedura."
LangString runwarning ${LANG_ITALIAN} "Vi invitiamo a chiudere tutte le sessioni aperte di ${CLOSENAME} e quindi premere su OK.  L'applicazione portatile non puÚ essere aggiornata mentre Ë in esecuzione."
LangString invaliddirectory ${LANG_ITALIAN} "La cartella di destinazione selezionata non Ë valida.  Vi preghiamo di selezionarne una valida."
LangString notenoughspace ${LANG_ITALIAN} "La periferica che avete selezionato per l'installazione non ha spazio libero sufficiente per questa applicazione."
LangString checkforplatform ${LANG_ITALIAN} "Controllo della Piattaforma PortableApps.com"
LangString refreshmenu ${LANG_ITALIAN} "Aggiornamento del Men˘ PortableApps.com"


;thanks to ptm/lflcorreia
LangString welcome ${LANG_PORTUGUESE} "Este assistente ir· gui·-lo na instalaÁ„o da aplicaÁ„o ${NAME}.\r\n\r\nSe est· a actualizar uma instalaÁ„o existente de ${NAME}, por favor feche-a antes de continuar.\r\n\r\nClique em Seguinte para continuar."
LangString finish ${LANG_PORTUGUESE} "${NAME} foi instalado no seu dispositivo.\r\n\r\nClique em Terminar para fechar este assistente."
LangString runwarning ${LANG_PORTUGUESE} "Por favor feche todas as inst‚ncias de ${CLOSENAME} e depois clique em OK. A aplicaÁ„o port·til n„o pode ser actualizada enquanto est· a ser executada."
LangString invaliddirectory ${LANG_PORTUGUESE} "A pasta de destino que seleccionou È inv·lida. Por favor escolha uma pasta v·lida."
LangString notenoughspace ${LANG_PORTUGUESE} "O dispositivo que seleccionou para a instalaÁ„o n„o tem espaÁo livre suficiente para esta aplicaÁ„o."
LangString checkforplatform ${LANG_PORTUGUESE} "A localizar a Plataforma PortableApps.com"
LangString refreshmenu ${LANG_PORTUGUESE} "A recarregar o Menu PortableApps.com"

;thanks to Aciago/Spanish team on pa.com
LangString welcome ${LANG_SPANISH} "Este asistente lo guiar· en la instalaciÛn de ${NAME}.\r\n\r\nSi Usted est· actualizando una versiÛn ya existente de ${NAME}, por favor, ciÈrrela antes de proseguir.\r\n\r\nHaga clic en Siguiente para continuar."
LangString finish ${LANG_SPANISH} "${NAME} ha sido instalado en su dispositivo.\r\n\r\nPresione Finalizar para cerrar este asistente."
LangString runwarning ${LANG_SPANISH} "Por favor, cierre todos las instancias de ${CLOSENAME} y luego haga clic en OK. No se puede actualizar dicho programa port·til mientras se est· en ejecuciÛn."
LangString invaliddirectory ${LANG_SPANISH} "La carpeta de destino no es v·lida. Por favor, escoja otra carpeta."
LangString notenoughspace ${LANG_SPANISH} "No hay suficiente espacio libre en el dispositivo seleccionado para instalar la aplicaciÛn."
LangString checkforplatform ${LANG_SPANISH} "Buscando la Plataforma de PortableApps.com"
LangString refreshmenu ${LANG_SPANISH} "Recargando el Men˙ de PortableApps.com"

LangString welcome ${LANG_JAPANESE} "Ç±ÇÃÉEÉBÉUÅ[ÉhÇÕ PortableApps ÇÃÉCÉìÉXÉgÅ[ÉãÇ∆ÉAÉbÉvÉOÉåÅ[ÉhÇÇ≤àƒì‡ÇµÇ‹Ç∑ÅB\r\n\r\nä˘ë∂ÇÃ PortableApps ÇÉAÉbÉvÉOÉåÅ[ÉhÇµÇƒÇ¢ÇÈèÍçáÅAêiçsëOÇ…ï¬Ç∂ÇƒÇ≠ÇæÇ≥Ç¢ÅB\r\n\r\nÉAÉNÉeÉBÉuÇ»ÉlÉbÉgÉèÅ[ÉNê⁄ë±Ç™Ç†ÇË PortableApps Updater Ç™Ç®égÇ¢ÇÃÉtÉ@ÉCÉAÉEÉHÅ[ÉãÇ…ÇÊÇ¡ÇƒÉuÉçÉbÉNÇ≥ÇÍÇƒÇ¢Ç»Ç¢Ç±Ç∆Çämé¿Ç…ÇµÇƒÇ≠ÇæÇ≥Ç¢ÅB\r\n\r\në±çsÇ∑ÇÈÇ…ÇÕ [éüÇ÷] ÇÉNÉäÉbÉNÇµÇ‹Ç∑ÅB"
LangString finish ${LANG_JAPANESE} "${NAME} ÇÕÇ®égÇ¢ÇÃÉfÉoÉCÉXÇ…ÉCÉìÉXÉgÅ[ÉãÇ≥ÇÍÇƒÇ¢Ç‹Ç∑ÅB\r\n\r\nÇ±ÇÃÉEÉBÉUÅ[ÉhÇï¬Ç∂ÇÈÇ…ÇÕ [äÆóπ] ÇÉNÉäÉbÉNÇµÇ‹Ç∑ÅB"
LangString runwarning ${LANG_JAPANESE} "Ç∑Ç◊ÇƒÇÃ ${CLOSENAME} Çï¬Ç∂ÇƒéüÇ… [OK] ÇÉNÉäÉbÉNÇµÇƒÇ≠ÇæÇ≥Ç¢ÅB É|Å[É^ÉuÉã ÉAÉvÉäÉPÅ[ÉVÉáÉìÇÕãNìÆíÜÇ…ÉAÉbÉvÉOÉåÅ[ÉhÇ≈Ç´Ç‹ÇπÇÒÅB"
LangString invaliddirectory ${LANG_JAPANESE} "ëIëÇµÇΩÉCÉìÉXÉgÅ[ÉãêÊÇÃÉtÉHÉãÉ_ÇÕïsê≥Ç≈Ç∑ÅB óLå¯Ç»ÉtÉHÉãÉ_ÇëIëÇµÇƒÇ≠ÇæÇ≥Ç¢ÅB"
LangString notenoughspace ${LANG_JAPANESE} "ÉCÉìÉXÉgÅ[ÉãÇ…ëIëÇµÇΩÉfÉoÉCÉXÇ…Ç±ÇÃÉAÉvÉäÉPÅ[ÉVÉáÉìÇ…è\ï™Ç»ãÛÇ´óÃàÊÇ™Ç†ÇËÇ‹ÇπÇÒÅB"
LangString checkforplatform ${LANG_JAPANESE} "PortableApps.com Platform ÇÉ`ÉFÉbÉNÇµÇƒÇ¢Ç‹Ç∑"
LangString refreshmenu ${LANG_JAPANESE} "PortableApps.com Menu ÇçXêVÇµÇƒÇ¢Ç‹Ç∑"


;=== Variables
Var FOUNDPORTABLEAPPSPATH
Var PortableApps.comLocaleID

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
				KillProcDLL::KillProc "${CHECKRUNNING}"
				;Abort
  
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
					SectionGetSize ${SectionMain} $1 ;=== Space Required for App
					${GetRoot} `$INSTDIR` $2
					${DriveSpace} `$2\` "/D=F /S=K" $3 ;=== Space Free on Device
					${GetSize} `$INSTDIR` "/M=*.* /S=0K /G=1" $4 $5 $6 ;=== Current installation size
					IntOp $7 $3 + $4 ;=== Space Free + Current Install Size
					IfFileExists `$INSTDIR\Data` "" CheckPluginsDirectory
						${GetSize} `$INSTDIR\Data` "/M=*.* /S=0K /G=1" $4 $5 $6 ;=== Size of Data directory
						IntOp $7 $7 - $4 ;=== Remove the data directory from the free space calculation

				CheckPluginsDirectory:
					StrCmp `${ADDONSDIRECTORYPRESERVE}` "NONE" CalculateSpaceLeft
						IfFileExists `$INSTDIR\${ADDONSDIRECTORYPRESERVE}` "" CalculateSpaceLeft
							${GetSize} `$INSTDIR\${ADDONSDIRECTORYPRESERVE}` "/M=*.* /S=0K /G=1" $4 $5 $6 ;=== Size of Data directory
							IntOp $7 $7 - $4 ;=== Remove the plugins directory from the free space calculation

				CalculateSpaceLeft:
					IntCmp $7 $1 NotEnoughSpaceNoUpgrade NotEnoughSpaceNoUpgrade
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

Section "!App Portable (required)"
	SetOutPath $INSTDIR
	
	;=== BEGIN: PRE-INSTALL CODE
    Delete '$INSTDIR\Help.chm'
	;=== END: PRE-INSTALL CODE
	
	File "..\..\*.*"
	SetOutPath $INSTDIR\App
	File /r "..\..\App\*.*"
	SetOutPath $INSTDIR\Other
	File /r "..\..\Other\*.*"
	CreateDirectory "$INSTDIR\Data"

	;=== BEGIN: POST-INSTALL CODE
	;=== Set Language
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
		
		;=== Set settings for use with platform
		
		WriteINIStr $INSTDIR\Data\Settings.ini "Show" "Welcome" "false"
		WriteINIStr $INSTDIR\Data\Settings.ini "Show" "License" "false"

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