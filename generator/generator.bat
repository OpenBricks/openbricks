@echo off

REM Menu langage (bg/br/cat/cz/de/en/es/fi/fr/it/nl/pl/ro/ru/se/sk)
REM This have no effect on DVD language.
REM See iso/GEEXBOX/etc/mplayer/mplayer.conf
REM For ru, you will need to replace the ttf file in
REM iso/GEEXBOX/usr/share/mplayer/font by a KOI8R ttf font.
set LANG=en

REM Remote to Use (pctv/logitech/hauppauge/realmagic/creative/leadtek/RM-S6/
REM                RX-V850/animax)
set REMOTE=pctv


REM You should not have to modify the rest of this file

if not exist ziso goto nothingtoremove
echo "Removing old compressed tree..."
if exist %windir%\command\deltree.exe deltree /y ziso >nul
if not exist %windir%\command\deltree.exe rmdir /q /s ziso >nul
:nothingtoremove

echo "Applying settings..."
copy language\help_%LANG%.txt iso\GEEXBOX\usr\share\mplayer\help.txt >nul
copy language\menu_%LANG%.conf iso\GEEXBOX\etc\mplayer\menu.conf >nul
set FONT=iso-8859-1
if %LANG%==cz set FONT=iso-8859-2
if %LANG%==pl set FONT=iso-8859-2
if %LANG%==sk set FONT=iso-8859-2
if %LANG%==ru set FONT=koi8r
if %LANG%==bg set FONT=windows-1251
copy font\%FONT%\* iso\GEEXBOX\usr\share\mplayer\font >nul

copy lirc\lircrc_%REMOTE% iso\GEEXBOX\etc\lircrc >nul
copy lirc\lircd_%REMOTE% iso\GEEXBOX\etc\lircd >nul
copy lirc\lircd_%REMOTE%.conf iso\GEEXBOX\etc\lircd.conf >nul

echo "Building compressed tree..."
md ziso >nul
win32\mkzftree iso\GEEXBOX ziso\GEEXBOX >nul
if exist %windir%\command\deltree.exe deltree /y ziso\GEEXBOX\boot\* iso\GEEXBOX\usr\share\mplayer\help.txt iso\GEEXBOX\etc\mplayer\menu.conf iso\GEEXBOX\usr\share\mplayer\font\font.desc iso\GEEXBOX\usr\share\mplayer\font\*.raw iso\GEEXBOX\etc\lirc* >nul
if not exist %windir%\command\deltree.exe del /f /q ziso\GEEXBOX\boot\* iso\GEEXBOX\usr\share\mplayer\help.txt iso\GEEXBOX\etc\mplayer\menu.conf iso\GEEXBOX\usr\share\mplayer\font\font.desc iso\GEEXBOX\usr\share\mplayer\font\*.raw iso\GEEXBOX\etc\lirc*
copy iso\GEEXBOX\boot\* ziso\GEEXBOX\boot >nul
echo "Copying additionnal files..."
xcopy /d /e iso\* ziso >nul

echo "Generating iso..."
win32\mkisofs -quiet -no-pad -V GEEXBOX -volset GEEXBOX -P "The GeeXboX team (http://www.geexbox.org/)" -p "The GeeXboX team (http://www.geexbox.org/)" -A "MKISOFS ISO 9660/HFS FILESYSTEM BUILDER" -z -f -D -r -J -b GEEXBOX/boot/isolinux.bin -c GEEXBOX/boot/boot.catalog -sort sort -no-emul-boot -boot-load-size 4 -boot-info-table ziso > geexbox-custom.iso

echo "Cleaning tree..."
if exist %windir%\command\deltree.exe deltree /y ziso >nul
if not exist %windir%\command\deltree.exe rmdir /q /s ziso >nul

echo.
echo **** Your customized GeeXboX iso is ready ****
echo.
