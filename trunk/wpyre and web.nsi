; Web based Parallel Yafray Rendering Environment

; The name of the installer
Name "Web based Parallel Yafray Rendering Environment"

; The file to write
;OutFile "Web based Parallel Yafray Rendering Environment.exe"
OutFile "wpyre and web.exe"

; The default installation directory
InstallDir C:\wpyre

;--------------------------------
Page license
MiscButtonText "Back" "Next" "Cancel" "Close"

LicenseText "Readme£º""Install"
LicenseData Readme.txt

Page instfiles

;--------------------------------
Section "" ;No components page, name is not important

SetOutPath $INSTDIR\Main
File Main\*.exe Main\*.dll Main\blank.png Main\neederconf.txt Main\waiterconf.txt

SetOutPath $INSTDIR\Main\Codecs
File Main\Codecs\*.dll

SetOutPath $INSTDIR\Main\Formats
File Main\Formats\*.dll

SetOutPath $INSTDIR\Web
File Web\*.php Web\*.txt Web\*.html

SetOutPath $INSTDIR\Yafray
File Yafray\*.*


SetOutPath $INSTDIR\Test
File Test\*.*


SetOutPath $INSTDIR\Manual
File /r Manual\*.*

SectionEnd ; end the section

