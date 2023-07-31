; includes
    !include MUI2.nsh
    !include WinMessages.nsh
    !define /IfNDef EM_SHOWBALLOONTIP 0x1503
    !define /IfNDef EM_HIDEBALLOONTIP 0x1504

; settings
    Name "Geode"
    OutFile "GeodeInstaller.exe"
    Unicode true
    InstallDir "$PROGRAMFILES32\Steam\steamapps\common\Geometry Dash" ; set default path to the most common one

; ui settings
    !define MUI_ABORTWARNING
    !define MUI_LANGDLL_ALLLANGUAGES
    !define MUI_FINISHPAGE_NOAUTOCLOSE
    !define MUI_UNFINISHPAGE_NOAUTOCLOSE
    !define MUI_ICON logo_inst.ico
    !define MUI_UNICON logo_uninst.ico
    !define MUI_WELCOMEFINISHPAGE_BITMAP banner.bmp
    !define MUI_UNWELCOMEFINISHPAGE_BITMAP banner.bmp

; remember language
    !define MUI_LANGDLL_REGISTRY_ROOT "HKCU" 
    !define MUI_LANGDLL_REGISTRY_KEY "Software\Geode"
    !define MUI_LANGDLL_REGISTRY_VALUENAME "Installer Language"

; pages
    !insertmacro MUI_PAGE_WELCOME
    !insertmacro MUI_PAGE_LICENSE "..\..\LICENSE.txt"
    !define MUI_PAGE_CUSTOMFUNCTION_PRE FindAndApplyGamePath
    !insertmacro MUI_PAGE_DIRECTORY
    !insertmacro MUI_PAGE_INSTFILES
    !insertmacro MUI_PAGE_FINISH

    !insertmacro MUI_UNPAGE_WELCOME
    !insertmacro MUI_UNPAGE_CONFIRM
    !insertmacro MUI_UNPAGE_INSTFILES
    !insertmacro MUI_UNPAGE_FINISH

; languages
    !insertmacro MUI_LANGUAGE "English"
    !insertmacro MUI_LANGUAGE "French"
    !insertmacro MUI_LANGUAGE "German"
    !insertmacro MUI_LANGUAGE "Spanish"
    !insertmacro MUI_LANGUAGE "SpanishInternational"
    !insertmacro MUI_LANGUAGE "SimpChinese"
    !insertmacro MUI_LANGUAGE "TradChinese"
    !insertmacro MUI_LANGUAGE "Japanese"
    !insertmacro MUI_LANGUAGE "Korean"
    !insertmacro MUI_LANGUAGE "Italian"
    !insertmacro MUI_LANGUAGE "Dutch"
    !insertmacro MUI_LANGUAGE "Danish"
    !insertmacro MUI_LANGUAGE "Swedish"
    !insertmacro MUI_LANGUAGE "Norwegian"
    !insertmacro MUI_LANGUAGE "NorwegianNynorsk"
    !insertmacro MUI_LANGUAGE "Finnish"
    !insertmacro MUI_LANGUAGE "Greek"
    !insertmacro MUI_LANGUAGE "Russian"
    !insertmacro MUI_LANGUAGE "Portuguese"
    !insertmacro MUI_LANGUAGE "PortugueseBR"
    !insertmacro MUI_LANGUAGE "Polish"
    !insertmacro MUI_LANGUAGE "Ukrainian"
    !insertmacro MUI_LANGUAGE "Czech"
    !insertmacro MUI_LANGUAGE "Slovak"
    !insertmacro MUI_LANGUAGE "Croatian"
    !insertmacro MUI_LANGUAGE "Bulgarian"
    !insertmacro MUI_LANGUAGE "Hungarian"
    !insertmacro MUI_LANGUAGE "Thai"
    !insertmacro MUI_LANGUAGE "Romanian"
    !insertmacro MUI_LANGUAGE "Latvian"
    !insertmacro MUI_LANGUAGE "Macedonian"
    !insertmacro MUI_LANGUAGE "Estonian"
    !insertmacro MUI_LANGUAGE "Turkish"
    !insertmacro MUI_LANGUAGE "Lithuanian"
    !insertmacro MUI_LANGUAGE "Slovenian"
    !insertmacro MUI_LANGUAGE "Serbian"
    !insertmacro MUI_LANGUAGE "SerbianLatin"
    !insertmacro MUI_LANGUAGE "Arabic"
    !insertmacro MUI_LANGUAGE "Farsi"
    !insertmacro MUI_LANGUAGE "Hebrew"
    !insertmacro MUI_LANGUAGE "Indonesian"
    !insertmacro MUI_LANGUAGE "Mongolian"
    !insertmacro MUI_LANGUAGE "Luxembourgish"
    !insertmacro MUI_LANGUAGE "Albanian"
    !insertmacro MUI_LANGUAGE "Breton"
    !insertmacro MUI_LANGUAGE "Belarusian"
    !insertmacro MUI_LANGUAGE "Icelandic"
    !insertmacro MUI_LANGUAGE "Malay"
    !insertmacro MUI_LANGUAGE "Bosnian"
    !insertmacro MUI_LANGUAGE "Kurdish"
    !insertmacro MUI_LANGUAGE "Irish"
    !insertmacro MUI_LANGUAGE "Uzbek"
    !insertmacro MUI_LANGUAGE "Galician"
    !insertmacro MUI_LANGUAGE "Afrikaans"
    !insertmacro MUI_LANGUAGE "Catalan"
    !insertmacro MUI_LANGUAGE "Esperanto"
    !insertmacro MUI_LANGUAGE "Asturian"
    !insertmacro MUI_LANGUAGE "Basque"
    !insertmacro MUI_LANGUAGE "Pashto"
    !insertmacro MUI_LANGUAGE "ScotsGaelic"
    !insertmacro MUI_LANGUAGE "Georgian"
    !insertmacro MUI_LANGUAGE "Vietnamese"
    !insertmacro MUI_LANGUAGE "Welsh"
    !insertmacro MUI_LANGUAGE "Armenian"
    !insertmacro MUI_LANGUAGE "Corsican"
    !insertmacro MUI_LANGUAGE "Tatar"
    !insertmacro MUI_LANGUAGE "Hindi"

    !insertmacro MUI_RESERVEFILE_LANGDLL

    Function .onInit
        !insertmacro MUI_LANGDLL_DISPLAY
    FunctionEnd
    Function un.onInit
        !insertmacro MUI_UNGETLANGUAGE
    FunctionEnd

; random shit
    ; https://nsis.sourceforge.io/StrReplace
    !define StrRep "!insertmacro StrRep"
    !macro StrRep output string old new
        Push `${string}`
        Push `${old}`
        Push `${new}`
        !ifdef __UNINSTALL__
            Call un.StrRep
        !else
            Call StrRep
        !endif
        Pop ${output}
    !macroend
    !macro Func_StrRep un
        Function ${un}StrRep
            Exch $R2 ;new
            Exch 1
            Exch $R1 ;old
            Exch 2
            Exch $R0 ;string
            Push $R3
            Push $R4
            Push $R5
            Push $R6
            Push $R7
            Push $R8
            Push $R9

            StrCpy $R3 0
            StrLen $R4 $R1
            StrLen $R6 $R0
            StrLen $R9 $R2
            loop:
                StrCpy $R5 $R0 $R4 $R3
                StrCmp $R5 $R1 found
                StrCmp $R3 $R6 done
                IntOp $R3 $R3 + 1 ;move offset by 1 to check the next character
                Goto loop
            found:
                StrCpy $R5 $R0 $R3
                IntOp $R8 $R3 + $R4
                StrCpy $R7 $R0 "" $R8
                StrCpy $R0 $R5$R2$R7
                StrLen $R6 $R0
                IntOp $R3 $R3 + $R9 ;move offset by length of the replacement string
                Goto loop
            done:

            Pop $R9
            Pop $R8
            Pop $R7
            Pop $R6
            Pop $R5
            Pop $R4
            Pop $R3
            Push $R0
            Push $R1
            Pop $R0
            Pop $R1
            Pop $R0
            Pop $R2
            Exch $R1
        FunctionEnd
    !macroend
    !insertmacro Func_StrRep ""
    ;!insertmacro Func_StrRep "un."

    ; https://nsis.sourceforge.io/Remove_leading_and_trailing_whitespaces_from_a_string
    Function Trim
        Exch $R1 ; Original string
        Push $R2
        Loop:
            StrCpy $R2 "$R1" 1
            StrCmp "$R2" " " TrimLeft
            StrCmp "$R2" "$\r" TrimLeft
            StrCmp "$R2" "$\n" TrimLeft
            StrCmp "$R2" "$\t" TrimLeft
            Goto Loop2
        TrimLeft:
            StrCpy $R1 "$R1" "" 1
            Goto Loop
        Loop2:
            StrCpy $R2 "$R1" 1 -1
            StrCmp "$R2" " " TrimRight
            StrCmp "$R2" "$\r" TrimRight
            StrCmp "$R2" "$\n" TrimRight
            StrCmp "$R2" "$\t" TrimRight
            Goto Done
        TrimRight:
            StrCpy $R1 "$R1" -1
            Goto Loop2
        Done:
            Pop $R2
            Exch $R1
    FunctionEnd
    !define Trim "!insertmacro Trim"
    !macro Trim ResultVar String
        Push "${String}"
        Call Trim
        Pop "${ResultVar}"
    !macroend

    ; https://nsis.sourceforge.io/Split_strings
    !macro GET_STRING_TOKEN INPUT PART
        Push $R0
        Push $R1
        Push $R2
        StrCpy $R0 -1
        IntOp $R1 ${PART} * 2
        IntOp $R1 $R1 - 1
        findStart_loop_${PART}:
            IntOp $R0 $R0 + 1
            StrCpy $R2 ${INPUT} 1 $R0
            StrCmp $R2 "" error_${PART}
            StrCmp $R2 '"' 0 findStart_loop_${PART}
            IntOp $R1 $R1 - 1
            IntCmp $R1 0 0 0 findStart_loop_${PART}
            IntOp $R1 $R0 + 1
        findEnd_loop_${PART}:
            IntOp $R0 $R0 + 1
            StrCpy $R2 ${INPUT} 1 $R0
            StrCmp $R2 "" error_${PART}
            StrCmp $R2 '"' 0 findEnd_loop_${PART}
            IntOp $R0 $R0 - $R1
            StrCpy $R0 ${INPUT} $R0 $R1
            Goto done_${PART}
        error_${PART}:
            StrCpy $R0 error
        done_${PART}:
            Pop $R2
            Pop $R1
            Exch $R0
    !macroend

; actual code

!define BINDIR ..\..\bin\nightly

Var GamePath
Function FindGamePath
    Push $0
    Push $1
    Push $2
    Push $3
    Push $4

    # get steam path
    ReadRegStr $0 HKCU "Software\Valve\Steam" "SteamPath"
    IfErrors gamePathError
    ${StrRep} $0 $0 "/" "\"

    # read library folders
    FileOpen $1 "$0\steamapps\libraryfolders.vdf" r
    IfErrors gamePathError

    vdfReadLine:
        ClearErrors
        FileRead $1 $2
        IfErrors vdfDone
        ${Trim} $2 $2
        !insertmacro GET_STRING_TOKEN $2 1
        Pop $4
        StrCmp $4 "path" 0 vdfCheckAppId
            !insertmacro GET_STRING_TOKEN $2 2
            Pop $3 # save current library path into $3
    vdfCheckAppId:
        SetErrors
        StrCmp $4 "322170" 0 vdfReadLine
            ClearErrors # gd found in current library
    vdfDone:
        IfErrors gamePathError

    FileClose $1
    IfErrors gamePathError

    ${StrRep} $0 $3 "\\" "\"
    StrCpy $GamePath "$0\steamapps\common\Geometry Dash"

    # make sure game path is correct
    IfFileExists "$GamePath\*.exe" +2 0
    gamePathError:
        SetErrors
    Pop $4
    Pop $3
    Pop $2
    Pop $1
    Pop $0
FunctionEnd

Function FindAndApplyGamePath
    Call FindGamePath
    IfErrors 0 +3
        StrCpy $GamePath ""
        Return
    StrCpy $INSTDIR $GamePath
FunctionEnd

; https://stackoverflow.com/a/61486726
Function .onVerifyInstDir
    FindWindow $9 "#32770" "" $HWNDPARENT
    GetDlgItem $3 $9 1006 ; IDC_INTROTEXT
    LockWindow on

    ; check if there's any exe and libcocos2d.dll (GeometryDash.exe won't work because of GDPSes)
    IfFileExists $INSTDIR\*.exe 0 noGameNoLife
        IfFileExists $INSTDIR\libcocos2d.dll 0 noGameNoLife

    ; check if xinput doesn't exist or geode is already installed
    IfFileExists $INSTDIR\XInput9_1_0.dll 0 valid
        IfFileExists $INSTDIR\Geode.dll valid modLoader

    modLoader:
        IfFileExists $INSTDIR\hackpro.dll megahack other

    megahack:
        SetCtlColors $3 ff0000 transparent
        SendMessage $3 ${WM_SETTEXT} "" "STR:This path already has Mega Hack v6/v7 installed!$\r$\nGeode doesn't work with MHv6/v7.$\r$\nPlease, uninstall it before proceeding."
        LockWindow off
        Abort
        Return
    other:
        SetCtlColors $3 ff0000 transparent
        SendMessage $3 ${WM_SETTEXT} "" "STR:This path already has another mod loader installed!$\r$\nGeode doesn't work with any other mod loader.$\r$\nPlease, uninstall it before proceeding."
        LockWindow off
        Abort
        Return

    noGameNoLife:
        SetCtlColors $3 ff0000 transparent
        SendMessage $3 ${WM_SETTEXT} "" "STR:This path does not have Geometry Dash installed!"
        LockWindow off
        Abort
        Return

    valid:
        SetCtlColors $3 SYSCLR:18 SYSCLR:15
        SendMessage $3 ${WM_SETTEXT} "" "STR:$(^DirText)"
        LockWindow off
FunctionEnd

Section
    SetOutPath $INSTDIR

    File ${BINDIR}\Geode.dll
    File ${BINDIR}\Geode.pdb
    File ${BINDIR}\GeodeUpdater.exe
    File ${BINDIR}\XInput9_1_0.dll

    CreateDirectory $INSTDIR\geode
    WriteUninstaller "geode\Uninstall.exe"
SectionEnd

Section "Uninstall"
    StrCpy $INSTDIR "$INSTDIR\.." ; TODO fix hack bad !!!!!!!
    Delete $INSTDIR\Geode.dll
    Delete $INSTDIR\Geode.pdb
    Delete $INSTDIR\GeodeUpdater.exe
    Delete $INSTDIR\XInput9_1_0.dll
    RMdir /r $INSTDIR\geode
    DeleteRegKey /ifempty HKCU "Software\Geode"
SectionEnd
