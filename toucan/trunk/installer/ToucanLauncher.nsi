;Copyright (C) 2004-2008 John T. Haller

;Website: http://PortableApps.com/Toucan

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

!define NAME "Toucan"
!define FRIENDLYNAME "Toucan"
!define APP "Toucan"
!define VER "2.0.6.0"
!define WEBSITE "PortableApps.com/Toucan"

;=== Program Details
Name "${NAME}"
OutFile "..\..\${NAME}.exe"
Caption "${FRIENDLYNAME} | PortableApps.com"
Icon "..\..\App\AppInfo\appicon.ico"
VIProductVersion "${VER}"
VIAddVersionKey ProductName "${FRIENDLYNAME}"
VIAddVersionKey CompanyName "PortableApps.com"
VIAddVersionKey LegalCopyright "Steven Lamerton"
VIAddVersionKey FileDescription "${FRIENDLYNAME}"
VIAddVersionKey FileVersion "${VER}"
VIAddVersionKey ProductVersion "${VER}"
VIAddVersionKey InternalName "${FRIENDLYNAME}"
VIAddVersionKey LegalTrademarks "PortableApps.com is a Trademark of Rare Ideas, LLC."
VIAddVersionKey OriginalFilename "${NAME}.exe"
;VIAddVersionKey PrivateBuild ""
;VIAddVersionKey SpecialBuild ""

;=== Include
;(Standard NSIS)
!include TextFunc.nsh
!insertmacro GetParameters

;=== Runtime Switches
CRCCheck On
WindowIcon Off
SilentInstall Silent
AutoCloseWindow True
RequestExecutionLevel user

Section "Main"
	${GetParameters} $0
	StrCmp $0 "" LaunchWithoutParameters LaunchWithParameters
	
	LaunchWithoutParameters:
		SetOutPath $EXEDIR\App\toucan
		ExecShell "open" "$EXEDIR\App\toucan\Toucan.exe" "" SW_HIDE
		Goto TheEnd

	LaunchWithParameters:
		SetOutPath $EXEDIR
		ExecShell "open" "$EXEDIR\App\toucan\Toucan.exe" "$0"

	TheEnd:
SectionEnd
