
         ooooo                     oo     o  o               oo     o
        M"   "Mo                    Mo  oM"  M                Mo  oM"
       M           oM""Mo  oM""Mo    "Mo"    Mo"""o  oM""Mo    "Mo"
       M    """M   MooooMM MooooMM   oMMo    M    "M M    M    oMMo
       "o     oM   M    o  M    o   oM  Mo   M    oM M    M   oM  Mo
        "MoooM"M   "MooM"  "MooM"  M"    "M  M"ooo"  "MooM"  M"    "M



| ABOUT
| ~~~~~

GeeXboX is a kind of "divx box" software. In fact, it is a stand-alone boot
CD which allows you to watch movies or listen to music. It supports many 
formats, such as avi, mpeg, divx, ogm, rm, mp3, ogg, dvd, vcd and cdda. 
It also supports some IR remote controllers and TV-Out for some graphic cards. 
This archive contain the needed scripts to rebuild an iso image of the GeeXboX.


| REQUIREMENTS
| ~~~~~~~~~~~~

To generate a GeeXboX iso, you will need either :
  - a GNU/Linux system with mkisofs and mkzftree.
  - a MAC OS X system with mkisofs and mkzftree.
  - a windows system.

To install the GeeXboX, you will need :
  - a GNU/Linux system with syslinux.

To build the GeeXboX, you will need some classic tools :
  - a working GNU/Linux system.
  - the gcc C compiler.
  - GNU make
  - the patch command.
  - the nasm assembler.
  - bzip2 and gzip.
  - mkfs.ext2 and mkfs.vfat
  - the wget download tool (not needed for the full GeeXboX package).
  - mkisofs and mkzftree to build the iso image.
  - mkzftree to compress files on the iso image.
  - cdrecord (to burn the iso image).

And also 1.2 GB of hard disk free space.


| PERSONALIZATION
| ~~~~~~~~~~~~~~~

When the GeeXboX is installed on a hard drive or generated, 
it is quite easy to personalize it.

You can add some proprietary codecs such a rv9 or wmv9, simply by adding
them in the GEEXBOX/codecs directory. You can find those codecs in the
package on http://www.mplayerhq.hu/MPlayer/releases/codecs/ .

Useful files in it are :
  * Real codecs (used to play rv9 for exemple) : atrc.so.6.0, cook.so.6.0,
      sipr.so.6.0, drv3.so.6.0, drv4.so.6.0.
  * Windows Media codecs (wmv9/wma9) : wma9dmod.dll and wmv9dmod.dll.

You can also modify many options. This can be done simply by editing some
text files.

* Language :
    You can easily select your preferred menu language by following
    the procedure below :

    - GNU/Linux users:
    Edit the generator.sh script before running it and change the line
    near the top: "LANG=en" to your preferred language.
    For example: if you're French, change it to "LANG=fr".

    - Microsoft Windows users:
    When started, generator.exe gives you a combo list that allows you
    to choose your preferred language.

    NB : This has no effect on DVD language (see the MPlayer section).

    For the list of available languages please see the "language" directory.
    If your language is not available, you can translate the menu
    in your own language. This simply involves the creation of
    language/menu_LANG.conf and language/help_LANG.txt,
    and adding LANG to language/lang.conf.

* MPlayer :
    This is where you can do most of the configuration and tweaking.
    The options take place in the file packages/MPlayer/mplayer.conf.
    You may want to modify options such as OSD font size (subfont-text-scale).
    You can also add lots of options such as DVD default language
    (eg: alang=fr,en). The best place to find them is the MPlayer manpage
    for Linux users (man -l build/MPlayer-*/DOCS/mplayer.1). You should also
    have a look at the MPlayer documentation (in build/MPlayer-*/DOCS or at
    http://mplayerhq.hu/DOCS/).
    Another file you may modify is the packages/MPlayer/menu.conf. You can
    remove items you don't need, or you can translate it in your own language
    for example.
    Then the last file you can look at is the packages/MPlayer/build which
    contain the selection of options which are built into MPlayer.

* tvout :
    Enabling the TV-Out is achieved with the help of multiple small programs
    dedicated to different video card brands. Currently we use atitvout
    for ATI cards, s3switch for S3 cards and nvtv for nVidia cards (and
    possibly intel i810 and 3dfx cards). The configuration of those programs
    is done in config/tvout. There you can choose the TV standard you
    want (pal, ntsc...) and you can also modify specific options for nvtv.

    # TV Output Standard (ntsc/pal/secam)
    TVOUT_STANDARD=pal

    Please note that you can also define the output aspect (say 4:3 or 16:9
    display) in this file through the line :

    TVOUT_ASPECT="4:3"

    This parameter will be used both for TVOut and regular (CRT/TFT) display.
    You can also specify the desired height/width values and your display's
    horizontal and vertical frequencies in case you have unusual screen like
    WideScreens or videoprojector. This can be done, editing the
    /etc/mplayer/mplayer.conf file. Default parameters are shown below (please
    uncomment lines related to frequency if you want to use them) :

    screenw=800
    screenh=600
    #monitor-hfreq=31.5k-50k
    #monitor-vfreq=50-90

* Lirc :
    You can choose one of the supported remote controller by editing the file
    GEEXBOX/etc/remote. Also take care to chose the corresponding ir receiver
    in the same configuration file. If you want to modify the key binding of
    your remote controler, look at the file GEEXBOX/etc/lirc/lircrc_REMOTE.

* network :
    You can configure the network in the file GEEXBOX/etc/network.
    There you can chose the IP address used by the GeeXboX (default is to
    use DHCP and if it don't work, fall back to 192.168.0.54.
    You can also specify a login and a password which will be used to connect
    to windows shares (default will only connect to anonymous shares).
    You can also declare some NFS mounts in GEEXBOX/etc/nfs.

* wifi :
    By default, GeeXboX tries to autodetect your network settings.
    If you have both traditionnal NIC and WiFi cards, only the latest will
    be setup. You may have to modify the /etc/network file in order to fit
    your network's settings.

    In this one, 4 lines are related to wireless cards :
    * PHY_TYPE="auto"      # Network physical type (auto|ethernet|wifi)
    * WIFI_MODE="managed"  # Wifi working mode (managed|ad-hoc)
    * WIFI_WEP=""          # Wifi WEP key
    * WIFI_ESSID="any"     # Wifi SSID

    These lines let you configure most of the settings. You can let 
    autodetection enable or even force the use of ethernet or WiFi adapters.
    In the same way, this let you choose between the managed and the ad-hoc
    mode and let you define your WEP key and SSID.

* gateway :
    GeeXboX supports access to the Internet. In the case you've got a
    connection to the Internet, you may share it with your multimedia box,
    using a router or a gateway. For this, simply define the gateway IP
    address in the /etc/network file.

    * GATEWAY=""     # Gateway IP ("" for DHCP or no internet connection)

* tv configuration :
    GeeXboX supports TV inputs and tuners. The system hardly tries to
    autodetect the card and the tuner. You can force the settings and skip
    the autodetection try. Please modify the /etc/tvcard as described :

 #TV CARD/TUNER Model (AUTO for autodetection or look at the following urls)
 #http://www.linuxhq.com/kernel/v2.6/2/Documentation/video4linux/CARDLIST.bttv
 #http://www.linuxhq.com/kernel/v2.6/2/Documentation/video4linux/CARDLIST.tuner
 
    TV_CARD=AUTO
    TV_TUNER=AUTO
    TVIN_STANDARD=pal

    Please let the AUTO parameter whether you want to keep autodetection,
    or replace it by the number of your card and tuner types, according to
    the previous URL. Please be careful : in order to force the card and tuner
    types, you have to know the EXACT REFERENCES of your hardware.

    Once done, you should be able to use the TV inputs (Composite and S-VHS)
    of your TV card. In the same way, you can use the tuner to watch TV.
    For that, you will have to define the region you belongs to and the
    frequency of the TV channels you want to watch.
    Once again, simply edit the /etc/tvcard file :

    # TV Channels
    # Syntax : CHAN="Channel Title":"Channel Frequency"
    # Example :
    # CHAN="France 2":"26"
    # CHAN="Canal +":"K08"
    # TV Channels List
    # Available : france, europe-east, europe-west, us-bcast, us-cable
    CHANLIST=france

    Please be careful when editing channels and simply use the same syntax
    as described above and TV channels should be present in the main menu.

* audio configuration :
    GeeXboX supports both analog and digital audio output through regular JACK
    connectors or RCA SPDIF one. By default, output is set to analog. You can
    change this, by editing the /etc/audio file :

    # Output using SPDIF (yes/no), otherwise ANALOG output
    SPDIF=no

    Remember that you will need to set your output to SPDIF if you want
    to connect your soundcard to an external amplifier for decoding AC3/DTS
    streams (using passthrough mode).

* video post-processing :
    Post-Processing is a software way to get a smoother and more accurate
    render of your videos. It however consumes a lot of CPU time to magnify
    pictures but, as a result, it looks far better. Using MPlayer internal
    filters, GeeXboX lets you make h/v deblocking, deringing and auto
    luminance to enhance your image. By default, post-processing is disabled
    to avoid lagging on small configurations. You can enable it simply by
    editing the /etc/mplayer/mplayer.conf file :

    # Set Post Processing (h deblock, v deblock, dering, auto luminance)
    # Consumes CPU power, disabled for low configs, uncomment to enable it.
    #vf=pp=hb:a/vb:a/dr:a/al:a

* DXR3/Hollywood+ cards :
    Users with this kind of hardware decompression card does NOT even need
    to have a video board and sound card to use GeeXboX. On the drawbacks,
    only the tvout connector can be used with DXR3 cards (no CRT display).
    You may have to set the required image norm (PAL/NTSC) in the /etc/tvout
    file and the kind of audio output you want to use (Analog or SPDIF)
    in /etc/audio.

| GENERATION
| ~~~~~~~~~~

First have a look at the personalization section just above.

Then basically you can generate the iso under Linux by launching
  ./generator.sh
or under windows by launching
  generator.exe


| EXTRA SUBTITLE FONTS
| ~~~~~~~~~~~~~~~~~~~~

First make sure you font is not included in offical GeeXboX package,
by using the generator and looking up your font in the section above.

Most fonts are already included in offical package, some are supported
but not included because of their excessive size (Chinese fonts for example).

* General support:

  To add support for new fonts you should extract your font into font directory
  and add your font name to language/lang.conf FONTS variable.

* Chinese support:

  To include Chinese fonts in your generated geexbox iso you should get
  either the big5 or the gb2312 font from MPlayer contributed fonts:

    http://www1.mplayerhq.hu/MPlayer/contrib/fonts/chinesefonts/

  and unpack and copy into generator ./font/big5 or ./font/gb2312
  the directory which ends with `24` (font size 24)

  For example if you wish to use the 'gb2312 kai' font:

  - GNU/Linux users:
  wget http://www1.mplayerhq.hu/MPlayer/contrib/fonts/chinesefonts/gb2312-kai.tar.bz2
  tar -jxf gb2312-kai.tar.bz2
  mv gb2312-kai/gkai00mp24 ../path/to/generator/font/gb2312

  - Windows users:
  Download http://www1.mplayerhq.hu/MPlayer/contrib/fonts/chinesefonts/gb2312-kai.tar.bz2
  Open it up with WinZip
  And extract gb2312-kai\gkai00mp24 directory from the archive
  into ..\path\to\generator\font\gb2312

* Korean support:

  Similar to Chinese font support, only you can get the euc-kr font from:

    http://www1.mplayerhq.hu/MPlayer/contrib/fonts/korean/

  A good choise will be to use dotum20.tar.gz euc-kr font,
  and unpack and copy into generator ./font/euc-kr
  in similar way mention above.

| INSTALLATION
| ~~~~~~~~~~~~

First you need to create a FAT16/32 or EXT2/3 partition with at least 16 MB
of free space.

Then you can install the GeeXboX under Linux by launching
  ./installator.sh
And then responding to all the questions. Be carefull during this step.
Read twice each question and stop installing if you don't understand one
of the question.

But the easiest way to install GeeXboX is still to boot it from CD and type
"install" at isolinux boot prompt.


| PXE BOOT
| ~~~~~~~~

Yes, the GeeXboX is able to boot from the network on a diskless station !
To achieve this you will need :
 - a DHCP server
 - a TFTP server
 - a NFS server
 - a PXE capable station :-)

* Using a GNU/Linux system :
  ------------------------

First configure your dhcp server to send PXE boot info. Here is an example
with isc dhcp :

allow booting;
allow bootp;

subnet 192.168.0.0 netmask 255.255.255.0 {
  range 192.168.0.128 192.168.0.192;
  option subnet-mask 255.255.255.0;
  option broadcast-address 192.168.0.255;
  next-server 192.168.0.1;
  filename "/tftpboot/GEEXBOX/boot/pxelinux.0";
}

The next-server option is the address of the TFTP server.
Then configure your TFTP server (such as atftpd) to server the /tftpboot
directory and copy a full GEEXBOX tree in this directory. For exemple
you can copy the content of a GeeXboX CD from a linux WITH THE CDROM
TRANSPARENT DECOMPRESSION ENABLED !! (to verify this, look at the sbin/init
file in the GeeXboX tree and verify that it do not contain garbage)
If you've built the GeeXboX yourself from sources, you can also generate
the GEEXBOX tree with make pxe.

Then you should edit the file /tftpboot/GEEXBOX/boot/pxelinux.cfg/default
to set nfsroot to the right NFS path to the GEEXBOX tree.

Finaly set up your NFS to export the GEEXBOX tree with a /etc/exports
containing something like this :

/tftpboot/GEEXBOX (ro)

and a /etc/hosts.allow containing something like :

ALL: ALL

That should do the trick. Boot your PXE station and see what happen.

* Using a Microsoft Windows system :
  --------------------------------

In order to boot in PXE mode from a Windows host,
you'll need the following software :

* An TFTP and a DHCP Server
  (for example "tftpd32", available at http://tftpd32.jounin.net/)
* An NFS Server (for example "Allegro NFS server",
  available at http://opensource.franz.com/nfs/)
* A computer supporting PXE boot mode. 

Download and uncompress (No need to install) the tftpd32 folder
somewhere on your disk. In this example, lets assume it is : C:\tftpd32

Copy the complete GEEXBOX tree in the same directory (C:\tftpd32\GEEXBOX)

Start tftpd32 :
- Choose the C:\tftpd32 folder for "current directory".
- Choose the interface (network card) to be used in "server interface".
  In the example here it's 192.168.0.1
- Go in "setting" and make sure DHCP server is checked (enabled)
- In the "DHCP server" tab, fill in all the box using the following example :
  (Please refer to some documentation about DHCP all around the net
  in order to understand)
	* IP starting pool : 192.168.0.10
	* Size of pool : 10
	* Boot file : ./GEEXBOX/boot/pxelinux.0
	* WINS/DNS server : 192.168.0.254
	* Default router : 192.168.0.254
	* Mask : 255.255.255.0
	* Domain name : mydomain.net
- Click "save" to apply modifications.

First part is done, you can now boot the client computer (the one starting
GeeXboX), and will see it loading until the logo appears.
After a while it'll freeze because your NFS server is not set yet.

Install "Allegro NFS server" and fill in the boxes following these settings :

Exports tab : 
        * Add a "new name" : and call it "/tftpboot/GEEXBOX"
        * In "path" just below : choose "C:\tftp32\GEEXBOX" folder
        * In "allowed host list" , choose "all"
	* "Read write", and "read only user list", choose "root" and "everyone"
          (if you want some log just check all in the last tab)

Apply settings.

Don't forget to modify the file : C:\tftp32\GEEXBOX\boot\pxelinux.cfg\default 
and change the IP address "192.168.0.2" to "192.168.0.1" (or the one you've
previously setup).

Just start the client computer and now GeeXboX should be running fine.


| BUILDING
| ~~~~~~~~

First have a look at the configuration section just under (at least the
global part).

Then basically you can build the iso with :
  make
Or you can directly burn this iso with :
  make burn

When done, you can save disk space by cleaning the build tree :
  make clean
or by doing a full cleaning, even removing downloaded sources :
  make distclean

There's also more advanced commands if you want to hack around the GeeXboX :
  scripts/get package          # download the package
  scripts/unpack package       # unpack and prepare the package
  scripts/build package        # build the package
  scripts/install package      # install the package with the $INSTALL prefix
  scripts/clean package        # clean the package build tree
  scripts/clean --full package # clean both the package build and source trees
  make exec                    # launch directly the GeeXboX in a jail
                               # WARNING: this is a highly experimental feature
                               # use it at your own risks.

If you've made a hacked version of the GeeXboX, you can easily
build a small tar.bz2 for it with :
  make dist
or a full tar (containing all sources) with :
  make fulldist
or a geexbox generator with :
  make generator
or a geexbox installator with :
  make installator
or a pxe ready tree with :
  make pxe


| CONFIGURATION
| ~~~~~~~~~~~~~

* Global options :
    This is the first thing you should take care of before trying to build
    the GeeXboX. It stands in the file config/options, and it should be
    self explanatory.

* Linux :
    This is a classical Linux configuration (packages/linux/linux.conf).
    You can edit them by hand, or you can also do a scripts/unpack linux
    and then a make menuconfig -C build/linux-* (or use the method you prefer
    instead of menuconfig). Then you should backup your build/linux-*/.config
    into packages/linux/linux.conf.

* Lirc :
    Lirc allows you to control the GeeXboX using a remote controller. First
    you have to choose the file describing your specific remote in
    build/lirc-*/remotes (after doing a scripts/unpack lirc) and add it
    in packages/lirc/install. Then you should choose your device (default
    is /dev/ttyS0 (COM1)) and the lirc driver and put them in a file
    called packages/lirc/lircd_$REMOTE. Then you can choose your key mapping in
    the file packages/lirc/lircrc_$REMOTE. For each mapping you have to select
    a button (pike their names in the remote definition file) and associate an
    action to it. The action is one of MPlayer's actions (you can find a list
    in the html file build/MPlayer-*/DOCS/documentation.html#commands).


| HACKING
| ~~~~~~~

The first thing you should look at is the initialization script.
In fact there are two initialization scripts. The first one is in
packages/initrd/linuxrc but you shouldn't need to modify it. The
second one is in config/init and that's where you may put some
customizations.

Then the next thing which may interest you is the creation of a new
"package". A package is just a bunch of scripts which have to follow
some rules. All the scripts have to take place in a directory named as
the program you want to "package", itself in the packages directory.
Here is a list of the scripts you may create :
 - url : just a list of urls where to get the program sources.
 - unpack : what to do after unpacking the sources. For example, you can
            modify some configuration files. This does not include
            applying patches.
 - need_build : called when the package as allready been built, in order
                to be sure that it don't need to be rebuilt. It should
                remove the file .stamps/"package name"/build if the
                package need to be rebuilt.
 - build : all the steps needed to build the program.
 - install : all the steps needed to install the program. The installation
             prefix should be $INSTALL.

When a file from the urls is named patch-program_name-... it is automatically
applied to the unpacked sources of the program.

You also have to remember that software which run on the GeeXboX have to
be compiled with the uClibc gcc wrapper.

Finally, the best way to make a package is to look how other packages are done.


| LICENSE
| ~~~~~~~

All the programs used by the GeeXboX are protected by their respective
license. They all are free software and most of them are covered by the
GNU General Public License.
The GeeXboX itself, meaning all the scripts which are used in the building
process, are covered by the GNU General Public License.
