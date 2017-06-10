#!/bin/sh

# $1 is target

# In case it's the first time we try the build
mkdir -p /project/sources /project/stamps /project/build.host/.cache build/build.host build/config /project/.ccache /project/.ssh

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






