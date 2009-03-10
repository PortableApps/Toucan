!macro CustomCodePostInstall
	StrCmp $LANGUAGE "1061" 0 +3 ;Estonian
		WriteINIStr $INSTDIR\Data\Settings.ini "General" "LanguageCode" "et_EE"
		Goto CustomCodePostInstallEnd
	StrCmp $LANGUAGE "1034" 0 +3 ;Spanish
		WriteINIStr $INSTDIR\Data\Settings.ini "General" "LanguageCode" "es_ES"
		Goto CustomCodePostInstallEnd
	StrCmp $LANGUAGE "1049" 0 +3 ;Russian
		WriteINIStr $INSTDIR\Data\Settings.ini "General" "LanguageCode" "ru_RU"
		Goto CustomCodePostInstallEnd
	StrCmp $LANGUAGE "1043" 0 +3 ;Dutch
		WriteINIStr $INSTDIR\Data\Settings.ini "General" "LanguageCode" "nl_NL"
		Goto CustomCodePostInstallEnd
	StrCmp $LANGUAGE "1031" 0 +3 ;German
		WriteINIStr $INSTDIR\Data\Settings.ini "General" "LanguageCode" "de_DE"
		Goto CustomCodePostInstallEnd
	StrCmp $LANGUAGE "1036" 0 +3 ;French
		WriteINIStr $INSTDIR\Data\Settings.ini "General" "LanguageCode" "fr_FR"
		Goto CustomCodePostInstallEnd
	StrCmp $LANGUAGE "1045" 0 +3 ;Polish
		WriteINIStr $INSTDIR\Data\Settings.ini "General" "LanguageCode" "pl_PL"
		Goto CustomCodePostInstallEnd
	StrCmp $LANGUAGE "1038" 0 +3 ;Hungarian
		WriteINIStr $INSTDIR\Data\Settings.ini "General" "LanguageCode" "hu_HU"
		Goto CustomCodePostInstallEnd
	StrCmp $LANGUAGE "1026" 0 +3 ;Bulgarian
		WriteINIStr $INSTDIR\Data\Settings.ini "General" "LanguageCode" "bg_BG"
		Goto CustomCodePostInstallEnd
	StrCmp $LANGUAGE "1042" 0 +3 ;Korean
		WriteINIStr $INSTDIR\Data\Settings.ini "General" "LanguageCode" "ko_KO"
		Goto CustomCodePostInstallEnd
	StrCmp $LANGUAGE "1040" 0 +3 ;Italian
		WriteINIStr $INSTDIR\Data\Settings.ini "General" "LanguageCode" "it_IT"
		Goto CustomCodePostInstallEnd
	StrCmp $LANGUAGE "1046" 0 +3 ;Portuguese (Brazilian)
		WriteINIStr $INSTDIR\Data\Settings.ini "General" "LanguageCode" "pt_BR"
		Goto CustomCodePostInstallEnd
	StrCmp $LANGUAGE "2070" 0 +3 ;Portuguese
		WriteINIStr $INSTDIR\Data\Settings.ini "General" "LanguageCode" "pt_PT"
		Goto CustomCodePostInstallEnd
	StrCmp $LANGUAGE "1055" 0 +3 ;Turkish
		WriteINIStr $INSTDIR\Data\Settings.ini "General" "LanguageCode" "tr_TR"
		Goto CustomCodePostInstallEnd
	StrCmp $LANGUAGE "1029" 0 +3 ;Czech
		WriteINIStr $INSTDIR\Data\Settings.ini "General" "LanguageCode" "cs_CZ"
		Goto CustomCodePostInstallEnd
	StrCmp $LANGUAGE "1028" 0 +3 ;Chinese Traditional
		WriteINIStr $INSTDIR\Data\Settings.ini "General" "LanguageCode" "zh_TW"
		Goto CustomCodePostInstallEnd
	WriteINIStr $INSTDIR\Data\Settings.ini "General" "LanguageCode" "en_GB" ;=== Fallback to English
	CustomCodePostInstallEnd:
!macroend