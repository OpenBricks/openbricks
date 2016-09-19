#!/bin/sh

. config/options

put_str() {
  printf "$*\n"
}

get_path () {
echo $(dirname "$1")
}

search_meta () {
 SECTIONS="admin drivers filesystem firmware fonts games host-packages libs misc multimedia nodejs net perl proto python sound system utils x11"
  local temp_meta=null
  for section in $SECTIONS ; do
    [ -r $ROOT/$PACKAGES/$section/$1/meta ] && temp_meta=$ROOT/$PACKAGES/$section/$1/meta
  done
  # check for platform-specific package overlay
  PLATFORM_META="$PLATFORM_PACKAGES/$1/meta"
  [ -r "$PLATFORM_META" ] && temp_meta="$PLATFORM_META"

  # check for machine-specific package overlay
  MACHINE_META="$MACHINE_PACKAGES/$1/meta"
  [ -r "$MACHINE_META" ] && temp_meta="$MACHINE_META"

  # check for flavour-specific package overlay
  FLAVOUR_META="$FLAVOUR_PACKAGES/$1/meta"
  [ -r "$FLAVOUR_META" ] && temp_meta="$FLAVOUR_META"
  
  echo $temp_meta
}

if [ x"$1" = "x" ] ; then
  put_str "Usage: $0 <pkg_name|specific-meta_file> [update]\n"
  exit 0
fi

if echo $1 | grep -q meta  ; then
  dir_pkg_meta=$(get_path "$1")
  package=$(basename $dir_pkg_meta)
else
  dir_pkg_meta=$(get_path $(search_meta "$1"))
  package=$1
fi

if ! [ -f $dir_pkg_meta/meta ] ; then
  put_str "no meta file for $1"
  exit 0
fi

. $dir_pkg_meta/meta

if [ -z $PKG_NAME ] ; then
  put_str "setting \$PKG_NAME to $package"
  PKG_NAME=$package
  . $dir_pkg_meta/meta
fi

if [ -n "$PKG_PARENT" ] ; then
  put_str " Please, check manually this package, \$PKG_PARENT is set ..."
  exit 1
fi

if [ -n "$PKG_URL_PROTO" ]; then
  REPO_BASENAME=$PKG_NAME-$PKG_VERSION
  FILES=$REPO_BASENAME.tar.bz2
else
  [ -f "$PACKAGE_URL" ] && \
    FILES=`sed 's%.*/\(.*\)\$%\1%' $PACKAGE_URL`

  [ -n "$PKG_URL" ] && \
      FILES=`echo $PKG_URL | sed 's%.*/\(.*\)\$%\1%'`
fi

src="$FILES"
if [ -z $src ] ; then
put_str "no source for $package"
exit 0
fi

put_str "Checking $package with file $src"

if [ -d "$ROOT/$SOURCES/$package" ] ; then
  put_str "found directory $ROOT/$SOURCES/$package"
#  cd $ROOT/$SOURCES/$package
  if [ -f "$ROOT/$SOURCES/$package/$src" ] ; then
    put_str "Computing md5 sum for $src "
    MD5=$(md5sum "$ROOT/$SOURCES/$package/$src" | cut -d ' ' -f 1)
    if [ -z "$PKG_MD5" ] ; then
      echo "PKG_MD5=\"$MD5\"" >> $dir_pkg_meta/meta
      put_str " Added PKG_MD5=$MD5 to $dir_pkg_meta/meta"
    else
      put_str "PKG_MD5 value already set, comparing ..."
      if [ "$MD5" != "$PKG_MD5" ]; then
        if [ x"$2" = "xupdate" ] ; then
          sed -i $dir_pkg_meta/meta -e "s%^PKG_MD5=.*%PKG_MD5=\"${MD5}\"%"
        else
          put_str "    it differs, please check"
        fi
      else
        put_str "    Same value"
      fi
    fi
  else
  put_str "$src not found"
#  ls  -al $ROOT/$SOURCES/$package
  fi
#  cd -
else
  echo "Directory $ROOT/$SOURCES/$package does not exist"
fi
