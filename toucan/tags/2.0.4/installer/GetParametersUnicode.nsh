Function GetParametersUnicode
	; This function handles getting parameters when the EXE's path contains CJK characters
	; It replaces the built-in NSIS GetParameters function
	; GetParameters
	; input, none
	; output, top of stack (replaces, with e.g. whatever)
	; modifies no other variables. 

	Push $R0
	Push $R1
	Push $R2
	Push $R3

	StrCpy $R2 1
	StrLen $R3 $CMDLINE

	;Check for quote or space
	StrCpy $R0 $CMDLINE $R2
	StrCmp $R0 '"' 0 +3
		StrCpy $R1 '"'
		Goto loop
	StrCpy $R1 " "

	loop:
		IntOp $R2 $R2 + 1
		StrCpy $R0 $CMDLINE 1 $R2
		StrCmp $R0 $R1 get
		StrCmp $R2 $R3 get
		Goto loop
  
	get:
		IntOp $R2 $R2 + 1
		StrCpy $R0 $CMDLINE 1 $R2
		StrCmp $R0 " " get
		StrCpy $R0 $CMDLINE "" $R2

	Pop $R3
	Pop $R2
	Pop $R1
	Exch $R0
FunctionEnd

!macro GetParametersUnicode RETURN
	Call GetParametersUnicode
	Pop ${RETURN}
!macroend

!define GetParametersUnicode "!insertmacro GetParametersUnicode"