#!/bin/sh

# Menu language (bg/br/ca/cs/de/en/es/et/fi/fr/he/hu/it/nl/no/pl/ro/ru/sk/sr/sv)
# This have no effect on DVD language. See iso/GEEXBOX/etc/mplayer/mplayer.conf
LANG=en

# Subtitle font (bg/br/ca/cs/de/en/es/et/fi/fr/he/hu/it/nl/no/pl/ro/ru/sk/sr/sv)
# Can also be set to a charset code (iso-8859-{1,2,7,8,9}/cp125{0,1}/koi8r/gb2312/big5)
# when empty default is to LANG
SUB_FONT=

# Remote to Use (pctv/logitech/hauppauge/realmagic/creative/leadtek/RM-S6/
#                RX-V850/animax/askey/avermedia/packard_bell/atiusb/LG/D-10)
REMOTE=atiusb

# Remote receiver to use (animax/askey/avermedia/atiusb/creative/hauppauge/
#                         homemade/irman/leadtek/logitech/pctv/realmagic)
RECEIVER=atiusb


# You should not have to modify the rest of this file

if [ -z "`which mkisofs`" -o -z "`which mkzftree`" ]; then
  echo ""
  echo "**** You need to have mkisofs and mkzftree installed ****"
  echo ""
  exit 1
fi

if [ -d ./iso -a -d ./lirc -a -d ./language ]; then
  TMPDIR="."
  GEEXBOX_DIR="."
elif [ -d /usr/share/geexbox-generator ]; then
  TMPDIR="/tmp/geexbox-generator"
  GEEXBOX_DIR="/usr/share/geexbox-generator"
else
  echo ""
  echo "**** GeeXboX files not found ****"
  echo ""
  exit 1
fi
OUTPUT=geexbox-`cat $GEEXBOX_DIR/VERSION`-$LANG.iso
W32CODECS_DIR=/usr/lib/win32

if [ -e /etc/geexbox-generator.conf ]; then
  . /etc/geexbox-generator.conf;
fi

if [ -e ~/.geexbox-generator ]; then
  . ~/.geexbox-generator.conf;
fi

. $GEEXBOX_DIR/language/lang.conf
. $GEEXBOX_DIR/language/lang.funcs

[ -d $W32CODECS_DIR ] && W32CODECS_USAGE=" [-w|--with-w32codecs]"

usage()
{
cat << EOF
Usage:
    $0 [-h|--help]$W32CODECS_USAGE [-o file.iso]

EOF
exit 1
}

while [ $# -ne 0 ]; do
  case $1 in
    --help|-h)
      usage
      ;;
    --with-w32codecs|-w)
      [ -d /usr/lib/win32 ] && W32CODECS=1
      ;;
    -o)
      OUTPUT=$2
      shift
      ;;
    *)
      echo "Invalid option: $1" >&2
      exit 1
      ;;
  esac
  shift || true
done

MENU_FONT=`lang2menufont $LANG`
SUB_FONT=`subfont2font $SUB_FONT`
[ -z "$SUB_FONT" ] && exit 1

for font in $MENU_FONT $SUB_FONT; do
  if [ ! -f $GEEXBOX_DIR/font/$font/font.desc ]; then
    echo ""
    echo "**** $font font is missing ****"
    echo "**** Please visit the README - EXTRA SUBTITLE FONTS section ****"
    echo ""
    exit 1
  fi
done

for file in menu_$LANG.conf help_$LANG.txt; do
  if [ ! -f $GEEXBOX_DIR/language/$file ]; then
    echo ""
    echo "**** $file language file is missing ****"
    echo ""
    exit 1
  fi
done

if [ $TMPDIR = "." ]; then
  rm -rf $TMPDIR/ziso
else
  rm -rf $TMPDIR
fi
mkdir -p $TMPDIR/ziso
[ ! -d $TMPDIR/iso ] && cp -r $GEEXBOX_DIR/iso $TMPDIR/iso

echo $LANG > $TMPDIR/iso/GEEXBOX/etc/lang
cp $GEEXBOX_DIR/language/help_$LANG.txt $TMPDIR/iso/GEEXBOX/usr/share/mplayer/
cp $GEEXBOX_DIR/language/menu_$LANG.conf $TMPDIR/iso/GEEXBOX/etc/mplayer/
cp $GEEXBOX_DIR/language/lang.conf $TMPDIR/iso/GEEXBOX/etc/

echo $SUB_FONT > $TMPDIR/iso/GEEXBOX/etc/subfont
cp -r $GEEXBOX_DIR/font/$SUB_FONT $TMPDIR/iso/GEEXBOX/usr/share/mplayer/font/
if [ -n "$MENU_FONT" -a "$MENU_FONT" != "$SUB_FONT" ]; then
  cp -r $GEEXBOX_DIR/font/$MENU_FONT $TMPDIR/iso/GEEXBOX/usr/share/mplayer/font/
fi

cp $GEEXBOX_DIR/lirc/lircrc_$REMOTE $TMPDIR/iso/GEEXBOX/etc/lircrc
cp $GEEXBOX_DIR/lirc/lircd_$RECEIVER $TMPDIR/iso/GEEXBOX/etc/lircd
cp $GEEXBOX_DIR/lirc/lircd_$REMOTE.conf $TMPDIR/iso/GEEXBOX/etc/lircd.conf
[ -n "$W32CODECS" ] && ln -s $W32CODECS_DIR/* $TMPDIR/iso/GEEXBOX/codecs/
mkzftree $TMPDIR/iso/GEEXBOX $TMPDIR/ziso/GEEXBOX
rm -f $TMPDIR/iso/GEEXBOX/usr/share/mplayer/help.txt
rm -f $TMPDIR/iso/GEEXBOX/etc/mplayer/menu.conf
for i in $TMPDIR/iso/GEEXBOX/usr/share/mplayer/font/*/; do
  [ -d $i ] && rm -rf $i
done
rm -f $TMPDIR/iso/GEEXBOX/etc/lirc*

cp -rf $TMPDIR/iso/GEEXBOX/boot/* $TMPDIR/ziso/GEEXBOX/boot
for i in $TMPDIR/iso/*; do
  [ "$i" != $TMPDIR/iso/GEEXBOX ] && ln -s "../$i" $TMPDIR/ziso
done

mkisofs -quiet -no-pad -V GEEXBOX -volset GEEXBOX -publisher "The GeeXboX team (www.geexbox.org)" -p "The GeeXboX team (www.geexbox.org)" -A "MKISOFS ISO 9660/HFS FILESYSTEM BUILDER" -z -f -D -r -J -b GEEXBOX/boot/isolinux.bin -c GEEXBOX/boot/boot.catalog -sort $GEEXBOX_DIR/sort -no-emul-boot -boot-load-size 4 -boot-info-table $TMPDIR/ziso > $OUTPUT

if [ $TMPDIR = "." ]; then
  rm -rf $TMPDIR/ziso
else
  rm -rf $TMPDIR
fi

echo ""
echo "**** Your customized GeeXboX iso is ready ****"
echo ""
