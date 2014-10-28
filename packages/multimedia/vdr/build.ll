#!/bin/sh

. config/options

require_cxx $1

$SCRIPTS/unpack linux

ACTIVE_EXTENSIONS="liemiext"
VDR_PLUGINS="`echo "$ROOTFS_PACKAGES $OPK_PACKAGES" | tr ' ' '\n' | grep 'vdr-'`" || true
for plugin in $VDR_PLUGINS; do
  VDR_EXTENSIONS=
  if [ -r $PACKAGES/$plugin/config/vdr ]; then
    . $PACKAGES/$plugin/config/vdr
    ACTIVE_EXTENSIONS="$ACTIVE_EXTENSIONS $VDR_EXTENSIONS"
  fi
done

cd $BUILD/$1*
rm -rf PLUGINS/src/dvbhddevice
#cp svdrpsend.pl svdrpsend
#cp /root/openbricks/packages/vdr/config/Make.config .
cp $ROOT/$PACKAGES/$1/config/Make.config .
cp $ROOT/$PACKAGES/$1/config/Make* .
cat >> Make.config <<EOF
PREFIX=/usr
PLUGINLIBDIR=/usr/lib/vdr/plugins
VIDEODIR=/media/video
CONFDIR=/etc/vdr
LOCDIR=/usr/share/locale
LIBS += -lintl
EOF
#patch -p1 < /$ROOT/$PACKAGES/$1/02_makefile.patch
#patch -p1 < /$ROOT/$PACKAGES/$1/04_makefile.patch

[ "$TARGET_LIBC" = "uclibc" ] && echo 'LIBS += -liconv' >>Make.config

for ext in $ACTIVE_EXTENSIONS; do
  EXTNAME=`echo $ext | tr a-z A-Z`
  echo "$EXTNAME=1" >> Make.config
  VDR_DEFINES="$VDR_DEFINES -DUSE_$EXTNAME"
done
[ -n "$VDR_DEFINES" ] && echo "DEFINES += $VDR_DEFINES" >> Make.config

echo "ENTER"
read enterKey
make && make include-dir # latter one to build extra plugins
make_install
