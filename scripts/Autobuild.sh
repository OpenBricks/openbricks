#!/bin/sh

# $1 is target

# In case it's the first time we try the build
mkdir -p /project/sources /project/stamps build/config /project/.ccache-$1 /project/.ssh

create_img () {
  local name=`basename *.tar.xz .tar.xz`
  if [ -f make-sdcard ]; then
    sudo rm -f /tmp/$CONFNAME.img*
    sudo ./make-sdcard /tmp/$CONFNAME.img $name.tar.xz
    sudo chown --reference=$name.tar.xz /tmp/$CONFNAME.img*
    [ -f /tmp/$CONFNAME.img.xz ] && mv /tmp/$CONFNAME.img.xz ./$name.img.xz
    rm -f /tmp/$CONFNAME.img*
  fi
}
REPONAME=openbricks
REPO=/project/repo/checkout
CONFNAME=$1
CONFFILE=$REPO/config/defconfigs/$1.conf
DATE=$(date +%Y%m%d)

# Create a ssh key for our server
if [ -f /project/.ssh/id_rsa ] ; then
  echo "[ssh] Not generating a new file, already here"
else
  ssh-keygen -q -t rsa -N "" -f /project/.ssh/id_rsa
fi
ssh_opt="-o UserKnownHostsFile=/project/.ssh/known_hosts -o StrictHostKeyChecking=no"
echo "######## Public key #########"
cat /project/.ssh/id_rsa.pub
echo "######## Public key #########"

# Do not dowmnload if we have already the sources
ln -s /project/sources sources
ln -s /project/stamps .stamps


# Just to see...
# echo "Content of build/build.host"
# ls -al build/build.host
# echo "Content of build/build.host/.cache"
# ls -al build/build.host/.cache
# echo "Content of /project/sources"
# ls -al /project/sources
# echo "Content of /project"
# ls -al /project


echo "######################"
echo "# Starting the build #"
echo "######################"
./scripts/loadcfg $1 || exit 1
# Build with the maximum speed (maybe we should try too without MAKECFLAGS set)
DOOZER_CONCURRENCY_MAKE_LEVEL=$(echo $MAKEFLAGS |cut -d, -f2)
echo "Using DOOZER_CONCURRENCY_MAKE_LEVEL=$DOOZER_CONCURRENCY_MAKE_LEVEL : MAKEFLAGS was $MAKEFLAGS"
echo DOOZER_CONCURRENCY_MAKE_LEVEL=$DOOZER_CONCURRENCY_MAKE_LEVEL >> build/config/options-doozer
echo "DOOZER_TARGET_CCACHE=/project/.ccache-$1" >> build/config/options-doozer


if grep -q 'CONFIG_OPT_TARGET_FLAT=y' $CONFFILE; then
  sed \
      -e 's:CONFIG_OPT_TARGET_FLAT=y:# CONFIG_OPT_TARGET_FLAT is not set:' \
      -e 's:# CONFIG_OPT_TARGET_TARBALL is not set:CONFIG_OPT_TARGET_TARBALL=y:' \
       < $CONFFILE > `ls -d build/build.host/kconfig-frontends-*`/.config
else
  cp -P $CONFFILE `ls -d build/build.host/kconfig-frontends-*`/.config
fi

make silentoldconfig


make || exit 1


########################### Sending our files now #########################
# Clean packages
echo "Cleaning binaries/binaries.*"
find binaries/binaries.* -name "*-dbg_*.opk" -delete

mkdir -p /project/$REPONAME/$CONFNAME/$DATE
rm -rf /project/$REPONAME/$CONFNAME/$DATE/*

echo "Moving files to /project/$REPONAME/$CONFNAME/$DATE"
mv binaries/binaries.* /project/$REPONAME/$CONFNAME/$DATE

(cd /project/$REPONAME/$CONFNAME/$DATE/binaries.*; create_img)

scp $ssh_opt -i /project/.ssh/id_rsa -v -r /project/$REPONAME buildbot@geexbox.org:/data-snapshots/doozer-buildbot

rm -rf /project/$REPONAME/$CONFNAME/$DATE




