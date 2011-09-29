#!/bin/sh

DISTRO=dummy
VERSION=dummy
ARCH=dummy

yesnoquestion() {
  local REPLY
  local QUESTION="$1"
  until [ "$REPLY" = y -o "$REPLY" = yes ] || [ "$REPLY" = n -o "$REPLY" = no ]; do
    read -p "$QUESTION (yes/no) ? " REPLY
  done

  [ "$REPLY" = y -o "$REPLY" = yes ]
}

abort_install() {
  echo "The installation has been aborted."
  read -p "Press any key to reboot the system..." DUMMY
  reboot
}

echo "$DISTRO $ARCH $VERSION installator"
echo "----------------------------"
echo

echo "This program installs $DISTRO on your computer."
echo "WARNING: the contents of the selected disk will be erased!"
echo

yesnoquestion "Begin the install" || abort_install

echo "Ok, let's get started."
echo "Here's a list of the hard drives I could find on your system:"
disklist=""
rdev=`cat /proc/mounts | grep '/\.root' | cut -f1 -d\ | cut -f3 -d/`
for i in /sys/block/*; do
  dev=`basename $i`

  [ "$dev" =  loop ] && continue
  [ "$dev" = ram ] && continue
  [ "$dev" = "$rdev" ] && continue
  [ -r /sys/block/$dev/device/type ] || continue
  [ `cat /sys/block/$dev/device/type` -eq 0 ] || continue
  unset vendor model size
  [ -r /sys/block/$dev/device/vendor ] &&
    vendor=`cat /sys/block/$dev/device/vendor`
  [ -r /sys/block/$dev/device/model ] &&
    model=`cat /sys/block/$dev/device/model`
  if [ -r /sys/block/$dev/size ]; then
    size=`cat /sys/block/$dev/size`
    size=$((size*512))
    size=$((size/1000))
    size=$((size/1000))
    if [ $size -ge 1000 ]; then
      size=$((size/1000))
      size="${size} GB"
    else
      size="${size} MB"
    fi
  else
    size=""
  fi
  echo "${dev}: $vendor $model $size"
  [ -z "$disklist" ] && disklist=$dev || disklist="${disklist}/${dev}"
done

if [ -z "$disklist" ]; then
  echo "Couldn't find any hard drives!"
  abort_install
fi

until [ -n "$DISK" ] && echo "$disklist" | grep -q "$DISK"; do
  read -p "Where to you want to install $DISTRO ($disklist) ? " DISK
done

echo "----------- WARNING --------------"
echo "$DISTRO will be installed on $DISK"
echo "All the contents of $DISK will be erased!"

yesnoquestion "Are you sure you want to continue" || abort_install

echo "Let me say that again: ALL the contents of $DISK will be ERASED"
yesnoquestion "Are you REALLY sure you want to continue" || abort_install

echo "Repartitioning..." 
target_dev="/dev/$DISK"
dd if=/dev/zero of=$target_dev bs=512 count=4
/bin/echo -e 'o\nn\np\n1\n1\n\na\n1\nw\n' | fdisk $target_dev
target_part=${target_dev}1
echo "Formatting..." 
/usr/sbin/mkfs.ext4 -L $DISTRO -m 0 $target_part
/usr/sbin/tune2fs -c 0 -i 0 $target_part
mkdir -p /tmp/installator
/sbin/blkid -o udev $target_part > /tmp/installator/blkid
. /tmp/installator/blkid
echo "Installing $DISTRO..."
target="/tmp/installator/target"
mkdir -p $target
mount $target_part $target
cp -PR /.squashfs/* $target
cat > $target/etc/fstab <<EOF
proc /proc proc defaults 0 0
UUID=${ID_FS_UUID} / ext4 relatime,errors=remount-ro 0 1
EOF
echo "Installing the kernel..."
mkdir -p $target/boot
cp -P /.root/vmlinuz $target/boot/
cp -P /.root/initrd $target/boot/
echo "Installing the boot loader..."
cp -P /.root/isolinux/splash.png $target/boot/
cp -P /.root/isolinux/vesamenu.c32 $target/boot/
cp -P /.root/isolinux/help.msg $target/boot/
cat > $target/boot/syslinux.cfg <<EOF
DEFAULT /boot/vesamenu.c32
PROMPT 0

TIMEOUT 20

MENU BACKGROUND /boot/splash.png
MENU TITLE Welcome to $DISTRO $ARCH $VERSION (C) 2002-2011
MENU VSHIFT 11
MENU ROWS 6
MENU TABMSGROW 15
MENU CMDLINEROW 14
MENU HELPMSGROW 16
MENU TABMSG Press [Tab] to edit options, [F1] for boot options.
MENU COLOR sel 7;37;40 #e0000000 #fa833b all
MENU COLOR border 30;44 #00000000 #00000000 none

LABEL geexbox
  MENU LABEL Start $DISTRO ...
  MENU DEFAULT
  KERNEL /boot/vmlinuz
  APPEND initrd=/boot/initrd root=UUID=${ID_FS_UUID} rootfs=flat quiet loglevel=3

MENU SEPARATOR

LABEL debug
  MENU LABEL Start in debugging mode ...
  KERNEL /boot/vmlinuz
  APPEND initrd=/boot/initrd root=UUID=${ID_FS_UUID} rootfs=flat emergency

F1 help.msg #00000000
EOF

/sbin/extlinux --install $target/boot
umount $target
cat /usr/share/syslinux/mbr.bin > $target_dev

echo "The installation has been completed!"
read -p "Press any key to reboot the system..." DUMMY
reboot

