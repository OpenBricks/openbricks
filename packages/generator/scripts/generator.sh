#!/bin/sh

# Menu language (bg/br/ca/cs/de/en/es/et/fi/fr/gr/he/hu/it/ko/nl/no/pl/ro/ru/sk/sr/sv/zh_CN/zh_TW)
# This have no effect on DVD language. See iso/GEEXBOX/etc/mplayer/mplayer.conf
LANG=en

# Subtitle charset (bg/br/ca/cs/de/en/es/et/fi/fr/gr/he/hu/it/ko/nl/no/pl/ro/ru/sk/sr/sv/zh_CN/zh_TW)
# Can also be set to a charset code (iso-8859-{1,2,7,8,9}/cp125{0,1}/koi8r/gb2312/big5/euc-kr)
# when empty default is to LANG
SUB_CHARSET=

# Remote to Use (pctv/logitech/hauppauge/realmagic/creative/leadtek/RM-S6/
#                RX-V850/animax/askey/avermedia/packard_bell/atiusb/LG/D-10)
REMOTE=atiusb

# Remote receiver to use (animax/askey/avermedia/atiusb/creative/hauppauge/
#                         homemade/irman/leadtek/logitech/pctv/realmagic)
RECEIVER=atiusb

# Theme
THEME=

# You should not have to modify the rest of this file

if [ -z "`which mkisofs`" -o -z "`which mkzftree`" ]; then
  echo ""
  echo "**** You need to have mkisofs and mkzftree installed ****"
  echo ""
  exit 1
fi

if [ -d ./iso -a -d ./lirc -a -d ./i18n ]; then
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

if [ -e /etc/geexbox-generator.conf ]; then
  . /etc/geexbox-generator.conf;
fi

if [ -e ~/.geexbox-generator ]; then
  . ~/.geexbox-generator.conf;
fi

OUTPUT=geexbox-`cat $GEEXBOX_DIR/VERSION`-$LANG.iso
W32CODECS_DIR=/usr/lib/win32

. $GEEXBOX_DIR/i18n/lang.conf
. $GEEXBOX_DIR/i18n/lang.funcs

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

[ -z "$THEME" ] && THEME=`cat themes/default`
if [ ! -f "themes/$THEME/config" ]; then
  OTHEME=$THEME
  THEME="theme-$THEME"
  if [ ! -f "themes/$THEME/config" ]; then
    echo ""
    echo "**** GeeXboX theme '$OTHEME' not found. ****"
    echo ""
    exit 1
  fi
fi

. themes/$THEME/config
[ -z "$FONT_CHARSETS" ] && FONT_CHARSETS="iso-8859-1"

MENU_CHARSET=`lang2charset "$LANG"`
MENU_FONT=`lang2font "$LANG" menu`
SUB_CHARSET=`lang2charset "$SUB_CHARSET"`
SUB_FONT=`lang2font "$SUB_CHARSET" sub`

for i in $FONT_CHARSETS; do
  if [ "$i" = "$MENU_CHARSET" ]; then
    MENU_FONT="themefont.ttf"
    break
  fi
done

[ -z "$SUB_FONT" -o -z "$MENU_FONT" ] && exit 1

for font in $MENU_FONT $SUB_FONT; do
  if [ "$font" != "themefont.ttf" -a ! -f $GEEXBOX_DIR/i18n/fonts/$font ]; then
    echo ""
    echo "**** $font font is missing ****"
    echo "**** Please visit the README - EXTRA SUBTITLE FONTS section ****"
    echo ""
    exit 1
  fi
done

for file in menu_$LANG.conf help_$LANG.txt; do
  if [ ! -f $GEEXBOX_DIR/i18n/texts/$file ]; then
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
cp $GEEXBOX_DIR/i18n/texts/help_$LANG.txt $TMPDIR/iso/GEEXBOX/usr/share/mplayer/
cp $GEEXBOX_DIR/i18n/texts/menu_$LANG.conf $TMPDIR/iso/GEEXBOX/etc/mplayer/
cp $GEEXBOX_DIR/i18n/lang.conf $TMPDIR/iso/GEEXBOX/etc/

echo $SUB_CHARSET > $TMPDIR/iso/GEEXBOX/etc/subfont
cp -r $GEEXBOX_DIR/i18n/fonts/$SUB_FONT $TMPDIR/iso/GEEXBOX/usr/share/fonts/
if [ "$MENU_FONT" = "themefont.ttf" ]; then
  cp $GEEXBOX_DIR/themes/$THEME/*.ttf $TMPDIR/iso/GEEXBOX/usr/share/fonts/themefont.ttf
elif [ "$MENU_FONT" != "$SUB_FONT" ]; then
  cp -r $GEEXBOX_DIR/i18n/fonts/$MENU_FONT $TMPDIR/iso/GEEXBOX/usr/share/fonts/
fi

for i in $SUB_CHARSET $MENU_CHARSET; do
  grep "^$i " $GEEXBOX_DIR/i18n/iconv/charset.db | cut -d ' ' -f 2 | while read f; do
    cp -f $GEEXBOX_DIR/i18n/iconv/$f $TMPDIR/iso/GEEXBOX/usr/share/iconv/
  done
done

cp $GEEXBOX_DIR/themes/$THEME/config $TMPDIR/iso/GEEXBOX/etc/theme.conf
cp $GEEXBOX_DIR/themes/$THEME/*.ttf $TMPDIR/iso/GEEXBOX/usr/share/fonts/themefont.ttf
cp $GEEXBOX_DIR/themes/$THEME/background.avi $TMPDIR/iso/GEEXBOX/usr/share/mplayer/
[ -f $GEEXBOX_DIR/themes/$THEME/background-audio.avi ] && cp $GEEXBOX_DIR/themes/$THEME/background-audio.avi $TMPDIR/iso/GEEXBOX/usr/share/mplayer/
[ -f $GEEXBOX_DIR/themes/$THEME/grub-splash.xpm.gz ] && cp $GEEXBOX_DIR/themes/$THEME/grub-splash.xpm.gz $TMPDIR/iso/GEEXBOX/usr/share/

cp $GEEXBOX_DIR/lirc/lircrc_$REMOTE $TMPDIR/iso/GEEXBOX/etc/lircrc
cp $GEEXBOX_DIR/lirc/lircd_$RECEIVER $TMPDIR/iso/GEEXBOX/etc/lircd
cp $GEEXBOX_DIR/lirc/lircd_$REMOTE.conf $TMPDIR/iso/GEEXBOX/etc/lircd.conf
[ -n "$W32CODECS" ] && ln -s $W32CODECS_DIR/* $TMPDIR/iso/GEEXBOX/codecs/
mkzftree $TMPDIR/iso/GEEXBOX $TMPDIR/ziso/GEEXBOX
rm -f $TMPDIR/iso/GEEXBOX/usr/share/mplayer/help_$LANG.txt
rm -f $TMPDIR/iso/GEEXBOX/etc/mplayer/menu_$LANG.conf
rm -f $TMPDIR/iso/GEEXBOX/etc/lang.conf
rm -f $TMPDIR/iso/GEEXBOX/etc/lang
rm -f $TMPDIR/iso/GEEXBOX/etc/subfont
rm -f $TMPDIR/iso/GEEXBOX/etc/theme.conf
rm -rf $TMPDIR/iso/GEEXBOX/usr/share/iconv/*
rm -rf $TMPDIR/iso/GEEXBOX/usr/share/fonts/*
rm -f $TMPDIR/iso/GEEXBOX/usr/share/mplayer/background.avi
rm -f $TMPDIR/iso/GEEXBOX/usr/share/mplayer/background-audio.avi
rm -f $TMPDIR/iso/GEEXBOX/usr/share/grub-splash.xpm.gz
rm -f $TMPDIR/iso/GEEXBOX/etc/lirc*

cp -rf $TMPDIR/iso/GEEXBOX/boot/* $TMPDIR/ziso/GEEXBOX/boot
[ -f $GEEXBOX_DIR/themes/$THEME/bootsplash.dat ] && cat $GEEXBOX_DIR/themes/$THEME/bootsplash.dat >> $TMPDIR/ziso/GEEXBOX/boot/initrd.gz
[ -f $GEEXBOX_DIR/themes/$THEME/splash-isolinux.rle ] && cp $GEEXBOX_DIR/themes/$THEME/splash-isolinux.rle $TMPDIR/ziso/GEEXBOX/boot/splash.rle

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
