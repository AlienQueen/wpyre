; Web based Parallel Yafray Rendering Environment

; The name of the installer
Name "Web based Parallel Yafray Rendering Environment"

; The file to write
OutFile "wpyre source.exe"

; The default installation directory
InstallDir C:\wpyre\Source

;--------------------------------
Page license
MiscButtonText "后退" "下一步" "取消" "关闭"

LicenseText "说明：""安装"
LicenseData Readme.txt

Page instfiles

;--------------------------------
Section "" ;No components page, name is not important

SetOutPath $INSTDIR
File *.txt *.nsi

SetOutPath $INSTDIR\Main
File Main\*.exe Main\*.dll Main\*.c Main\*.cpp Main\compile.bat Main\final-tga.bat Main\neederconf.txt Main\waiterconf.txt Main\makefile.vc Main\wpyre.rc Main\share.ico Main\cmd.exe.lnk Main\blank.png

SetOutPath $INSTDIR\Main\Expat-2.0.0
File /r Main\Expat-2.0.0\*.*

SetOutPath $INSTDIR\Main\DevIL
File /r Main\DevIL\*.*

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


SetOutPath $INSTDIR\Beautify
File /r Beautify\*.*

SetOutPath $INSTDIR\Manual
File /r Manual\*.*

SectionEnd ; end the section
