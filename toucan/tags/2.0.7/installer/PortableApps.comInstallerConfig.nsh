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

;OVERVIEW: This file is used to hold configuration data for the PortableApps.comInstaller
;It should be placed in the Other\Source directory along with PortableApps.comInstaller.nsi.
;All configuration data is contained within it.  If any custom pre-install, post-install or 
;cleanup when the optional features are not selected is needed, it can be included in the
;PortableApps.comInstallerCustom.nsi which is detailed within that file.

;LAST UPDATED FOR INSTALLER VERSION: 0.12.x

;== Basic Information.  Basic information about the portable app
!define NAME "Toucan"
!define SHORTNAME "Toucan"
!define VERSION "2.0.7.0"
!define FILENAME "Toucan_2.0.7"
!define CHECKRUNNING "Toucan.exe"
!define CLOSENAME "Toucan"
!define ADDONSDIRECTORYPRESERVE "NONE" ;use "NONE" for no plugins directory or relative to install directory: App\AppName\plugins
!define INSTALLERCOMMENTS "For additional details, visit PortableApps.com"
!define INSTALLERADDITIONALTRADEMARKS "" ;end this entry with a period and a space if used
!define INSTALLERMULTILINGUAL "true" ;uncomment this entry for multilingual installers

!ifndef INSTALLERMULTILINGUAL
	;== Single language installer
	!define INSTALLERLANGUAGE "English" ;one of the NSIS pre-defined languages supported: English, French, Italian, German, Japanese, SimpChinese
!else
	;== Multilingual installer, uncomment the lanuages used
	!define USES_ENGLISH "true"
	;!define USES_AFRIKAANS "true"
	;!define USES_ALBANIAN "true"
	;!define USES_ARABIC "true"
	;!define USES_BASQUE "true"
	;!define USES_BELARUSIAN "true"
	;!define USES_BOSNIAN "true"
	;!define USES_BRETON "true"
	!define USES_BULGARIAN "true"
	;!define USES_CATALAN "true"
	;!define USES_CROATIAN "true"
	!define USES_CZECH "true"
	;!define USES_DANISH "true"
	!define USES_DUTCH "true"
	!define USES_ESTONIAN "true"
	;!define USES_FARSI "true"
	;!define USES_FINNISH "true"
	!define USES_FRENCH "true"
	;!define USES_GALICIAN "true"
	!define USES_GERMAN "true"
	;!define USES_GREEK "true"
	;!define USES_HEBREW "true"
	!define USES_HUNGARIAN "true"
	;!define USES_ICELANDIC "true"
	;!define USES_INDONESIAN "true"
	;!define USES_IRISH "true"
	!define USES_ITALIAN "true"
	;!define USES_JAPANESE "true"
	!define USES_KOREAN "true"
	;!define USES_KURDISH "true"
	;!define USES_LATVIAN "true"
	;!define USES_LITHUANIAN "true"
	;!define USES_LUXEMBOURGISH "true"
	;!define USES_MACEDONIAN "true"
	;!define USES_MALAY "true"
	;!define USES_MONGOLIAN "true"
	;!define USES_NORWEGIAN "true"
	;!define USES_NORWEGIANNYNORSK "true"
	!define USES_POLISH "true"
	!define USES_PORTUGUESE "true"
	!define USES_PORTUGUESEBR "true"
	;!define USES_ROMANIAN "true"
	!define USES_RUSSIAN "true"
	;!define USES_SERBIAN "true"
	;!define USES_SERBIANLATIN "true"
	;!define USES_SIMPCHINESE "true"
	;!define USES_SLOVAK "true"
	;!define USES_SLOVENIAN "true"
	!define USES_SPANISH "true"
	;!define USES_SPANISHINTERNATIONAL "true"
	;!define USES_SWEDISH "true"
	;!define USES_THAI "true"
	!define USES_TRADCHINESE "true"
	!define USES_TURKISH "true"
	;!define USES_UKRAINIAN "true"
	;!define USES_UZBECK "true"
	;!define USES_WELSH "true"
!endif

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
	;!define OPTIONALSECTIONPRESELECTEDIFNONENGLISHINSTALL "true" ;Uncomment this line if you want the optional piece selected by default if the user chooses an install language other than English
!endif

;== Plugin Installer.  If this is a plugin installer (not a full app installer), uncomment this line to skip adding the app size to the free space calculation and files in root directory
;!define PLUGININSTALLER "true"

;== Remove Main Directories.  If you want to remove one of the main directories before installing (useful with upgrades) uncomment the appropriate line.
;!define REMOVEAPPDIRECTORY "true"
;!define REMOVEDATADIRECTORY "true" ;Normally this should not be used
;!define REMOVEOTHERDIRECTORY "true"

;== Preserve Specific Directories.  If you are removing directories but wish to preserve a specific directory, define them here.
;!define PRESERVEDIRECTORY1 "App\Example\Plugins"
;!define PRESERVEDIRECTORY2 ""
;!define PRESERVEDIRECTORY3 ""
;!define PRESERVEDIRECTORY4 ""
;!define PRESERVEDIRECTORY5 ""
;!define PRESERVEDIRECTORY6 ""
;!define PRESERVEDIRECTORY7 ""
;!define PRESERVEDIRECTORY8 ""
;!define PRESERVEDIRECTORY9 ""
;!define PRESERVEDIRECTORY10 ""

;== Preserve Specific Files.  If you are removing directories but wish to preserve a specific file, define them here.  Wildcards work.
;!define PRESERVEFILE1 "App\Example\Plugin.dll"
;!define PRESERVEFILE2 ""
;!define PRESERVEFILE3 ""
;!define PRESERVEFILE4 ""
;!define PRESERVEFILE5 ""
;!define PRESERVEFILE6 ""
;!define PRESERVEFILE7 ""
;!define PRESERVEFILE8 ""
;!define PRESERVEFILE9 ""
;!define PRESERVEFILE10 ""

;== Remove Specific Directories.  If you wish to remove specific directories and all files within, define them here.
;!define REMOVEDIRECTORY1 "App\Example"
;!define REMOVEDIRECTORY2 ""
;!define REMOVEDIRECTORY3 ""
;!define REMOVEDIRECTORY4 ""
;!define REMOVEDIRECTORY5 ""
;!define REMOVEDIRECTORY6 ""
;!define REMOVEDIRECTORY7 ""
;!define REMOVEDIRECTORY8 ""
;!define REMOVEDIRECTORY9 ""
;!define REMOVEDIRECTORY10 ""

;== Remove Specific Files. If you wish to remove specific files, define them here.  Wildcards are acceptable: App\Example\Example.*
;!define REMOVEFILE1 "App\Example\example.exe"
;!define REMOVEFILE2 ""
;!define REMOVEFILE3 ""
;!define REMOVEFILE4 ""
;!define REMOVEFILE5 ""
;!define REMOVEFILE6 ""
;!define REMOVEFILE7 ""
;!define REMOVEFILE8 ""
;!define REMOVEFILE9 ""
;!define REMOVEFILE10 ""

;== Custom Code.  If you need custom code pre or post install or to cleanup when optional features are not selected, uncomment the appropriate lines here and include it in the PortableApps.comInstallerCustom.nsi file.
!define USESCUSTOMCODE "true"
;!define USESCONFIGREAD "true" ;Uncomment this line if you need to use ConfigRead functions in your custom code
;!define USESCONFIGREADS "true" ;Uncomment this line if you need to use ConfigReadS functions in your custom code
;!define USESCONFIGWRITE "true" ;Uncomment this line if you need to use ConfigWrite functions in your custom code
;!define USESCONFIGWRITES "true" ;Uncomment this line if you need to use ConfigWriteS functions in your custom code
;!define USESVERSIONCOMPARE "true" ;Uncomment this line if you need to use VersionCompare functions in your custom code