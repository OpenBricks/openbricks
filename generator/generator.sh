#!/bin/sh

# Menu langage (bg/br/cat/cz/de/en/es/fi/fr/hu/it/nl/pl/ro/ru/se/sk)
# This have no effect on DVD language. See iso/GEEXBOX/etc/mplayer/mplayer.conf
# For ru, you will need to replace the ttf file in
# iso/GEEXBOX/usr/share/mplayer/font by a KOI8R ttf font.
LANG=en

# Remote to Use (pctv/logitech/hauppauge/realmagic/creative/leadtek/RM-S6/
#                RX-V850/animax/packard_bell/atiusb)
REMOTE=atiusb


# You should not have to modify the rest of this file

if [ -z "`which mkisofs`" -o -z "`which mkzftree`" ]; then
  echo ""
  echo "**** You need to have mkisofs and mkzftree installed ****"
  echo ""
  exit 1
fi

rm -rf ziso
mkdir -p ziso
echo $LANG > iso/GEEXBOX/etc/lang
cp language/help_$LANG.txt iso/GEEXBOX/usr/share/mplayer/
cp language/menu_$LANG.conf iso/GEEXBOX/etc/mplayer/
if [ $LANG = cz -o $LANG = hu -o $LANG = pl -o $LANG = ro -o $LANG = sk ]; then
  cp -r font/iso-8859-2 iso/GEEXBOX/usr/share/mplayer/font/
elif [ $LANG = bg ]; then
  cp -r font/cp1251 iso/GEEXBOX/usr/share/mplayer/font/
elif [ $LANG = ru ]; then
  cp -r font/koi8r iso/GEEXBOX/usr/share/mplayer/font/
else
  cp -r font/iso-8859-1 iso/GEEXBOX/usr/share/mplayer/font/
fi
cp lirc/lircrc_$REMOTE iso/GEEXBOX/etc/lircrc
cp lirc/lircd_$REMOTE iso/GEEXBOX/etc/lircd
cp lirc/lircd_$REMOTE.conf iso/GEEXBOX/etc/lircd.conf
mkzftree iso/GEEXBOX ziso/GEEXBOX
rm -f iso/GEEXBOX/usr/share/mplayer/help.txt
rm -f iso/GEEXBOX/etc/mplayer/menu.conf
rm -f iso/GEEXBOX/usr/share/mplayer/font/font.desc
rm -f iso/GEEXBOX/usr/share/mplayer/font/*.raw
rm -f iso/GEEXBOX/etc/lirc*

cp -rf iso/GEEXBOX/boot/* ziso/GEEXBOX/boot
for i in iso/*; do
  [ "$i" != iso/GEEXBOX ] && ln -s "../$i" ziso
done

mkisofs -quiet -no-pad -V GEEXBOX -volset GEEXBOX -P "The GeeXboX team (www.geexbox.org)" -p "The GeeXboX team (www.geexbox.org)" -A "MKISOFS ISO 9660/HFS FILESYSTEM BUILDER" -z -f -D -r -J -b GEEXBOX/boot/isolinux.bin -c GEEXBOX/boot/boot.catalog -sort sort -no-emul-boot -boot-load-size 4 -boot-info-table ziso > geexbox-`cat VERSION`.iso

rm -rf ziso

echo ""
echo "**** Your customized GeeXboX iso is ready ****"
echo ""
