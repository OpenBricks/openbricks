@echo off

REM Menu language (bg/br/ca/cs/de/en/es/et/fi/fr/gr/he/hu/it/ko/nl/no/pl/ro/ru/sk/sr/sv/zh_CN/zh_TW)
REM This have no effect on DVD language.
REM See iso/GEEXBOX/etc/mplayer/mplayer.conf
set LANG=en

REM Subtitle font (bg/br/ca/cs/de/en/es/et/fi/fr/gr/he/hu/it/ko/nl/no/pl/ro/ru/sk/sr/sv/zh_CN/zh_TW)
REM Can also be set to a charset code (iso-8859-{1,2,7,8,9}/cp125{0,1}/koi8r/gb2312/big5/euc-kr)
REM when empty default is to LANG
set SUB_FONT=

REM Remote to Use (pctv/logitech/hauppauge/realmagic/creative/leadtek/RM-S6/
REM                RX-V850/animax/askey/avermedia/packard_bell/atiusb/LG/D-10)
set REMOTE=atiusb

REM Remote receiver to use (animax/askey/avermedia/atiusb/creative/hauppauge/
REM                         homemade/irman/leadtek/logitech/pctv/realmagic)
set RECEIVER=atiusb


REM You should not have to modify the rest of this file

echo ""
echo "######################################################################"
echo "## You should NOT use this generator.bat. Use generator.exe instead ##"
echo "######################################################################"
echo ""

set MENU_FONT=
if %LANG%==no set MENU_FONT=iso-8859-1
if %LANG%==hu set MENU_FONT=iso-8859-2
if %LANG%==gr set MENU_FONT=iso-8859-7
if %LANG%==he set MENU_FONT=iso-8859-8
if %LANG%==sr set MENU_FONT=cp1250
if %LANG%==bg set MENU_FONT=cp1251
if %LANG%==ru set MENU_FONT=koi8r
if %LANG%==ko set MENU_FONT=euc-kr
if %LANG%==zh_CN set MENU_FONT=gb2312
if %LANG%==zh_TW set MENU_FONT=big5

if "X%SUB_FONT%"=="X" set SUB_FONT=%LANG%
set SUBFONT=iso-8859-1
if %SUB_FONT%==iso-8859-2 set SUBFONT=iso-8859-2
if %SUB_FONT%==cs set SUBFONT=iso-8859-2
if %SUB_FONT%==hu set SUBFONT=iso-8859-2
if %SUB_FONT%==pl set SUBFONT=iso-8859-2
if %SUB_FONT%==ro set SUBFONT=iso-8859-2
if %SUB_FONT%==sk set SUBFONT=iso-8859-2
if %SUB_FONT%==iso-8859-7 set SUBFONT=iso-8859-7
if %SUB_FONT%==gr set SUBFONT=iso-8859-7
if %SUB_FONT%==iso-8859-8 set SUBFONT=iso-8859-8
if %SUB_FONT%==he set SUBFONT=iso-8859-8
if %SUB_FONT%==iso-8859-9 set SUBFONT=iso-8859-9
if %SUB_FONT%==cp1250 set SUBFONT=cp1250
if %SUB_FONT%==sr set SUBFONT=cp1250
if %SUB_FONT%==cp1251 set SUBFONT=cp1251
if %SUB_FONT%==bg set SUBFONT=cp1251
if %SUB_FONT%==koi8r set SUBFONT=koi8r
if %SUB_FONT%==ru set SUBFONT=koi8r
if %SUB_FONT%==euc-kr set SUBFONT=euc-kr
if %SUB_FONT%==ko set SUBFONT=euc-kr
if %SUB_FONT%==gb2312 set SUBFONT=gb2312
if %SUB_FONT%==zh_CN set SUBFONT=gb2312
if %SUB_FONT%==big5 set SUBFONT=big5
if %SUB_FONT%==zh_TW set SUBFONT=big5
set SUB_FONT=%SUBFONT%

if not exist ziso goto nothingtoremove
echo "Removing old compressed tree..."
if exist %windir%\command\deltree.exe deltree /y ziso >nul
if not exist %windir%\command\deltree.exe rmdir /q /s ziso >nul
:nothingtoremove

echo "Applying settings..."
echo %LANG% > iso\GEEXBOX\etc\lang
copy language\help_%LANG%.txt iso\GEEXBOX\usr\share\mplayer\help_%LANG%.txt >nul
copy language\menu_%LANG%.conf iso\GEEXBOX\etc\mplayer\menu_%LANG%.conf >nul
copy language\lang.conf iso\GEEXBOX\etc\lang.conf >nul

echo %SUB_FONT% > iso\GEEXBOX\etc\subfont
md iso\GEEXBOX\usr\share\mplayer\font\%SUB_FONT%
copy font\%SUB_FONT%\* iso\GEEXBOX\usr\share\mplayer\font\%SUB_FONT% >nul

if "X%MENU_FONT%"=="X" set MENU_FONT=%SUB_FONT%
if %MENU_FONT%==%SUB_FONT% goto samefont
md iso\GEEXBOX\usr\share\mplayer\font\%MENU_FONT%
copy font\%MENU_FONT%\* iso\GEEXBOX\usr\share\mplayer\font\%MENU_FONT% >nul
:samefont

copy lirc\lircrc_%REMOTE% iso\GEEXBOX\etc\lircrc >nul
copy lirc\lircd_%RECEIVER% iso\GEEXBOX\etc\lircd >nul
copy lirc\lircd_%REMOTE%.conf iso\GEEXBOX\etc\lircd.conf >nul

echo "Building compressed tree..."
md ziso >nul
win32\mkzftree iso\GEEXBOX ziso\GEEXBOX >nul
if exist %windir%\command\deltree.exe deltree /y ziso\GEEXBOX\boot\* iso\GEEXBOX\usr\share\mplayer\help_%LANG%.txt iso\GEEXBOX\etc\mplayer\menu_%LANG%.conf iso\GEEXBOX\usr\share\mplayer\font\%SUB_FONT% iso\GEEXBOX\usr\share\mplayer\font\%MENU_FONT% iso\GEEXBOX\etc\subfont iso\GEEXBOX\etc\lang iso\GEEXBOX\etc\lirc* >nul
if not exist %windir%\command\deltree.exe del /f /q ziso\GEEXBOX\boot\* iso\GEEXBOX\usr\share\mplayer\help_%LANG%.txt iso\GEEXBOX\etc\mplayer\menu_%LANG%.conf iso\GEEXBOX\usr\share\mplayer\font\%SUB_FONT% iso\GEEXBOX\usr\share\mplayer\font\%MENU_FONT% iso\GEEXBOX\etc\subfont iso\GEEXBOX\etc\lang iso\GEEXBOX\etc\lirc*
copy iso\GEEXBOX\boot\* ziso\GEEXBOX\boot >nul
echo "Copying additionnal files..."
xcopy /d /e iso\* ziso >nul

echo "Generating iso..."
win32\mkisofs -quiet -no-pad -V GEEXBOX -volset GEEXBOX -P "The GeeXboX team (www.geexbox.org)" -p "The GeeXboX team (www.geexbox.org)" -A "MKISOFS ISO 9660/HFS FILESYSTEM BUILDER" -z -f -D -r -J -b GEEXBOX/boot/isolinux.bin -c GEEXBOX/boot/boot.catalog -sort sort -no-emul-boot -boot-load-size 4 -boot-info-table ziso > geexbox-custom-%LANG%.iso

echo "Cleaning tree..."
if exist %windir%\command\deltree.exe deltree /y ziso >nul
if not exist %windir%\command\deltree.exe rmdir /q /s ziso >nul

echo.
echo **** Your customized GeeXboX iso is ready ****
echo.
