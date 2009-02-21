
         ooooo                     oo     o  o               oo     o
        M"   "Mo                    Mo  oM"  M                Mo  oM"
       M           oM""Mo  oM""Mo    "Mo"    Mo"""o  oM""Mo    "Mo"
       M    """M   MooooMM MooooMM   oMMo    M    "M M    M    oMMo
       "o     oM   M    o  M    o   oM  Mo   M    oM M    M   oM  Mo
        "MoooM"M   "MooM"  "MooM"  M"    "M  M"ooo"  "MooM"  M"    "M



| ABOUT
| ~~~~~

GeeXboX is a Media Center Linux distribution. In fact, it is a stand-alone boot
CD which allows you to watch movies or listen to music. It supports many
formats, such as avi, mpeg, divx, ogm, rm, mp3, ogg, dvd, vcd and cdda.
It also supports some IR remote controllers and TV-Out for some graphic cards.
This archive contains the needed scripts to rebuild an iso image of GeeXboX.


| REQUIREMENTS
| ~~~~~~~~~~~~

To generate a GeeXboX iso, you will need either :
  - a GNU/Linux system with mkisofs and mkzftree.
  - a MAC OS X system with mkisofs and mkzftree.
  - a Windows system.

To build the GeeXboX, you will need some classic tools :
  - a working GNU/Linux system.
  - the gcc C compiler.
  - GNU make
  - the patch command.
  - the nasm assembler.
  - bzip2 and gzip.
  - the wget download tool (not needed for the full GeeXboX package).
  - mkisofs and mkzftree to build the iso image.
  - mkzftree to compress files on the iso image.
  - cdrecord (to burn the iso image).

And also 2 GB of hard disk free space.


| PERSONALIZATION
| ~~~~~~~~~~~~~~~

When the GeeXboX is installed on a hard drive or generated,
it is quite easy to personalize it.

You can add some proprietary codecs such a rv9 or wmv9, simply by adding
them in the GEEXBOX/codecs directory. You can find those codecs in the
package on http://www.geexbox.org/releases/extra-codecs-nonfree.tar.gz

You can also modify many options. This can be done simply by editing some
text files.

* Firmware auto-loader :

    GeeXboX supports many free drivers but unfortunately some of them may
    require an additionnal proprietary firmware (non-free and so not included
    in GeeXboX ISO) to work properly. The firmware is a binary file that is
    uploaded to the device (PCI or USB) when the driver is loaded. For each
    driver, the firmware has to be a specific file with a pre-determined name.

    If you have reluctant hardware that require additionnal firmware (in most
    of the cases, it only concerns WiFi and DVB cards), you'll need to check
    on the Internet for your Linux driver page to see how to check the
    firmware file. Most of the time, you'll get a direct download link.
    Sometimes, you'll need to extract it from your Windows drivers archive
    that was shipped with your hardware.

    Once you get the firmware file, simply put it into the /firmwares
    directory. You can put in as many firmwares as you want, and GeeXboX will
    automatically find them and use them with the right drivers if needed.
    The only thing you then have to take care is to have the good firmwares
    file and to generate a new ISO.

    Please note that you can also use the GeeXboX ISO generator to
    automatically grab firmwares for you. You can select all of the extra
    binary firmware files you want the generator to add for you. Simply note
    that this feature will require you to have an Internet access when using
    the GeeXboX generator.

* MPlayer :

    This is where you can do most of the configuration and tweaking.
    The options take place in the file packages/MPlayer/config/mplayer.conf.
    You may want to modify options such as OSD font size (subfont-text-scale).

    You can also add lots of options such as DVD default language
    (eg: alang=fr,en). The best place to find them is the MPlayer manpage
    for Linux users (man -l build/MPlayer-*/DOCS/mplayer.1). You should also
    have a look at the MPlayer documentation (in build/MPlayer-*/DOCS or at
    http://www.mplayerhq.hu/DOCS/).

    Another file you may modify is the packages/i18n-texts/config/menu.conf.
    You can remove items you don't need, or you can translate it in your
    own language for example.

    Then the last file you can look at is the packages/MPlayer/build which
    contains the selection of options which are built into MPlayer.

* TV-Out :

    Enabling the TV-Out is achieved with the help of multiple small programs
    dedicated to different video card brands. Currently we use atitvout
    for ATI cards, s3switch for S3 cards and nvtv for nVidia cards (and
    possibly intel i810 and 3dfx cards). The configuration of those programs
    is done in packages/tvout/config/tvout.

    There you can choose the TV standard you
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

* Network :

    You can configure the network in the file GEEXBOX/etc/network.
    There you can chose the IP address used by the GeeXboX (default is to
    use DHCP and if it don't work, fall back to 192.168.0.54).
    You can also specify a login and a password which will be used to connect
    to windows shares (default will only connect to anonymous shares).
    You can also declare some NFS mounts in GEEXBOX/etc/nfs.

* WiFi :

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

* Gateway :

    GeeXboX supports access to the Internet. In the case you've got a
    connection to the Internet, you may share it with your multimedia box,
    using a router or a gateway. For this, simply define the gateway IP
    address in the /etc/network file.

    * GATEWAY=""     # Gateway IP ("" for DHCP or no internet connection)

* DNS server :

    If you want to access to the Internet, GeeXboX may require a DNS server
    IP address. If you're not using a DHCP server that will do this job, you'll
    have to specify it by hand in the /etc/network file.

    * DNS_SERVER=""              # DNS Server IP ("" for DHCP or none)

* TV configuration :

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
    # Syntax : CHAN="Channel Frequency:Channel Title"
    # Example :
    # CHAN="26:France 2"
    # CHAN="K08:Canal +"
    # TV Channels List
    # Available : france, europe-east, europe-west, us-bcast, us-cable
    CHANLIST=france

    Please be careful when editing channels and simply use the same syntax
    as described above and TV channels should be present in the main menu.

* Audio configuration :

    GeeXboX supports both analog and digital audio output through regular JACK
    connectors or RCA SPDIF one. By default, output is set to analog. You can
    change this, by editing the /etc/audio file :

    # Output using SPDIF (yes/no), otherwise ANALOG output
    SPDIF=no

    Remember that you will need to set your output to SPDIF if you want
    to connect your soundcard to an external amplifier for decoding AC3/DTS
    streams (using passthrough mode).

* Video post-processing :

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

* Radio configuration :

    If your TV-card has built-in FM radio tuner, GeeXboX can support playing
    the radio. You have to modify the GEEXBOX/etc/radio file to have
    "RADIO=yes" if you want to use the radio. There is no hardware auto-
    detection, so setting this to 'yes' without a TV-card-based FM radio will
    put unnecessary menu items on your GeeXboX screen.

    The GEEXBOX/etc/radio file also has the radio station definition. There
    are a number of examples in the file, which simply include the radio
    frequency and the name of the station. Please be careful when editing
    channels and simply use the same syntax as the examples and the radio
    stations should be present in the main menu under "LISTEN RADIO".

    Since the radio application itself is not part of MPlayer, there is
    separate access to the volume. Please note that the volume that is being
    increased or decreased is Master volume. Before switching back to TV or
    video viewing, or music playing, the volume should be re-adjusted.

* UPnP File Sharing :

    UPnP stands for Universal Plug & Play and is a network protocol aiming at
    providing the autodiscovery of devices and services of your Home Network.
    The UPnP A/V (for Audio/Video) norms defines several profiles for devices
    to easily share multimedia files and lets them being played. The UPnP
    Media Server profiles represent all of the devices that are able to share
    files to devices implementing the UPnP Media Player or UPnP Media Renderer
    profiles.

    The GeeXboX embeds a UPnP Control Point software that is able to
    auto-discover all UPnP Media Servers from your Home network and it simply
    mounts their contents to a dedicated mount point (/mnt/UPnP).

    This way, you just have to share contents using a compliant UPnP Media
    Server software and they'll be automatically accessible from GeeXboX.
    For more info about UPnP and some media servers usage, please go to
    http://www.geexbox.org/wiki/index.php/Accessing_to_UPnP_Contents

* Network streaming  :

    GeeXboX now gives you the possibility to add some SHOUTcast Radio/TV
    official playlist as easily as personal playlists (local and remote).

    SHOUTcast activation is set in "GEEXBOX/etc/network" config file.
    As SHOUTcast TV sometimes may contain pornographic and
    subscription-only streams, you can define a "black list" and a "white
    list" to allow/deny any streams you choose (default keywords for "black
    list" are : "adult xxx porn ESS SWCTV SWPTV Subscription"). The
    black/white lists are case-insensitive.

    "GEEXBOX/etc/netstream" config file can be set to create local and
    remote playlists (some examples are located in this file). "Remote
    playlists" refer to "Extended M3U" files.

    For French users only :
    You can get "FreeboxTV" thanks to this file, using the following process :
    - uncomment EXTM3U ad hoc entry.
    - use a GeeXboX version that have to be compiled with "LIVE555 library"
      support (otherwise you won't be able to decode the stream).

* DVB cards :

    GeeXboX has a support for various types of DVB cards (Terrestrial, Cable,
    ATSC and Satellite) either in PCI or USB. Please note that for now, only
    one DVB adapter is usable at a time.

    Some DVB devices (especially USB ones) may require an external proprietary
    firmware to be loaded in the mean time as the driver to be usable.
    You may have a look at the LinuxTV DVB Wiki
    (http://linuxtv.org/wiki/index.php/Main_Page) to see if you're card
    requires one or not and if so, which one it is.

    Some firmwares can be fetched directly from the LinuxTV website
    (http://linuxtv.org/download/firmware/), otherwise may be included in the
    Windows drivers archive. See the GeeXboX firmware loader documentation to
    see how to get them loaded for you.

    For each card, the list of channels is available through the MPlayer's
    open menu. Nevertheless, there's no way to auto-discover these channels.

    Thus, adding the list of DVB channels to GeeXboX can be done in
    several ways :

    - using an existing channel list : MPlayer needs to have a working
    channels.conf file to use DVB. This file can be generated through the
    utilities provided by the dvb-apps package. Simply use a Linux box that
    has a DVB card configured, download the linuxtv-dvb-apps tarball from
    http://www.linuxtv.org/download/dvb/ , compile it and create the config
    file using the "scan" executable, in "zap" format (which is the default
    as of dvb-apps 1.1.0). For example, for a DVB-S (Satellite) card using
    Astra-19.2E as the provider:

      wget http://www.linuxtv.org/download/dvb/linuxtv-dvb-apps-1.1.0.tar.bz2
      tar jxvf linuxtv-dvb-apps-1.1.0.tar.bz2
      cd linuxtv-dvb-apps-1.1.0/util/scan
      make
      ./scan -x 0 dvb-s/Astra-19.2E > channels.conf

    The "-x 0" flag is here to restrict the scan to Free To Air channels only.
    Please always do so, as MPlayer do not support encrypted DVB channels.

    According to your type of DVB card, choose either a file from the "dvb-s"
    "dvb-c", "dvb-t" or "atsc" directories and ask for a scan.

    Then, simply copy the channels.conf file you've just created to
    the /etc/mplayer directory in the GeeXboX generator tree, and
    rebuild away.

    - using an existing transponder list : the procedure is highly similar to
    the one defined just before but with no scan. This time, the scan will be
    done at GeeXboX boot but this way, you do no more require another Linux
    system to generate the channels.conf file.

    To do so, you simpy have to check at :
      http://linuxtv.org/cgi-bin/viewcvs.cgi/dvb-apps/util/scan/

    According to your DVB card type (S/T/C/ATSC), check for the good directory
    and grab the transponder frequency list that fit your needs. Then simply
    rename the file to dvb.conf and copy it to /etc. Then, rebuild
    a new ISO using the generator.

    A bootup, if GeeXboX detects a valid transponder file in
    /etc/dvb.conf, it will be used to scan for DVB channels and will
    generate the /etc/mplayer/channels.conf file itself.

    WARNING : Scanning can be slow according to the number of devices to be
    scanned and will be done each time you boot GeeXboX if using it as a
    LiveCD. It is highly recommended that you do it once only, then copy the
    generated /etc/mplayer/channels.conf file somewhere else and rebuild an
    ISO using generator, following the first method or to install it on disk.

    - using installator : this is for sure the easiest solution but requires
    you to install GeeXboX to disk. During the installation process, if a
    valid DVB device is recognized by the system, the installator script will
    ask you if you want to scan for DVB channels.

    The installator contains the complete list of transponders frequencies.
    That way, you just have to select your DVB device type and the transponder
    file you want to use for GeeXboX to scan. The channels.conf file will
    then be automatically generated.

* DVD Navigation Menus :

    GeeXboX allows 2 DVD playback mode:
     - Direct movie playback (default).
     - DVD Navigation Menu support (fairly experimental).

    The first one can be safely trusted for playing any kind of DVD. Once
    inserted, the disc is automatically detected and MPlayer tries to play the
    title it consider as the movie one. The major advantage is that all
    copyright, advertisement and menu parts are skipped to allow a direct
    playback. However, in some situations (like DVDs with enhanced menus or
    most of the series DVD, containing multiple titles to be played) this
    method is inaccurate and you'll not be able to watch your DVD correctly.

    The second one, while still experimental (but has really good chances to
    works fine) supports the regular DVD Navigation Menus and let you control
    your DVD the same way you'll do it using a regular commercial DVD player.

    You can use the GeeXboX options menu to switch between the DVD playback
    methods, according to the DVD disc you're trying to watch.

    You can also make use of the GeeXboX ISO generator to select the default
    DVD playback method you want.


| INSTALLATION
| ~~~~~~~~~~~~

The easiest way to install GeeXboX is to boot it from CD and type
"install" at isolinux boot prompt.

Be carefull during this step. Read twice each question and stop installing
if you don't understand one of the questions.


| PXE BOOT
| ~~~~~~~~

Yes, the GeeXboX is able to boot from the network on a diskless station !
To achieve this you will need :
 - a DHCP server
 - a TFTP server
 - a NFS or SMB server
 - a PXE capable station :-)

* The network boot process :
  ------------------------
The DHCP server not only supplies the Geexbox client it's DHCP address lease, but
also points geexbox to tftp location to download the pxe linux loader.
On execution this pxe linux loader gets the configuration file ./pxelinux.cfg/default
from the tftpserver.
PXE boot related stuff in this config file:
-path/filename of kernel and ramdisk image to download using tftp
-additional kernel command line options, which tell Geexbox where to get files for loading 
stage 2 from.

These 2 examples show pxe boot related kernel command line options:
NFS mode:
boot=nfs nfsroot=192.168.0.1:/tftpboot/gbdev/geexbox/geexbox-pxe/GEEXBOX.i386.i386
SMB mode:
boot=smb smbroot=//192.168.0.1/geexbox smbuser=user smbpass=password


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
directory and copy a full GEEXBOX tree in this directory. For example
you can copy the content of a GeeXboX CD from a linux WITH THE CDROM
TRANSPARENT DECOMPRESSION ENABLED !! (to verify this, look at the sbin/init
file in the GeeXboX tree and verify that it do not contain garbage)
If you've built the GeeXboX yourself from sources, you can also generate
the GEEXBOX tree with make pxe.

Then you should edit the file /tftpboot/GEEXBOX/boot/pxelinux.cfg/default
to configure NFS or SMB settings as shown in example above.

When using NFS, set up your NFS to export the GEEXBOX tree with a /etc/exports
containing something like this :

/tftpboot/GEEXBOX (ro)

and a /etc/hosts.allow containing something like :

ALL: ALL


When using SMB, setup your the smb.conf file with a share something like this:

[geexbox]
   comment = Geexbox SMB boot files
   writable = no
   locking = no
   path = /tftpboot/GEEXBOX
   public = yes



* Using a Microsoft Windows system :
  --------------------------------

In order to boot in PXE mode from a Windows host,
you'll need the following software :

* A computer supporting PXE boot mode ;-)
* An TFTP and a DHCP Server
  (for example "tftpd32", available at http://tftpd32.jounin.net/)
* For stage 2 loading of files, there are 2 options: 
  -use the SMB server build into windows
  -or use an add-on NFS Server (for example "Allegro NFS server",
  available at http://opensource.franz.com/nfs/)


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
After a while it'll freeze because stage 2 loading (NFS or SMB) isn't configured
yet.


Using SMB mode:
Share the directory c:\tftpd32\GEEXBOX as geexbox (read-only will do).
Use a linux-aware editor to open
C:\tftp32\geexbox\geexbox-pxe\GEEXBOX\boot\pxelinux.cfg\default
On each boot menu entry:
  -change boot=nfs into boot=smb
  -remove nfsroot=....  entry
  -add following options
       smbroot=//192.168.0.1/geexbox   
       smbuser=username
       smbpass=password

Using NFS mode:
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

If you've made a hacked version of the GeeXboX, you can easily
build a small tar.bz2 for it with :
  make dist
or a full tar (containing all sources) with :
  make fulldist
or a geexbox generator with :
  make generator
or a pxe ready tree with :
  make pxe


| CONFIGURATION
| ~~~~~~~~~~~~~

* Global options :

    This is the first thing you should take care of before trying to build
    the GeeXboX. It stands in the file config/options, and it should be
    self explanatory.

* Linux :

    This is a classical Linux configuration (packages/linux/config/linux.conf).
    You can edit them by hand, or you can also do a scripts/unpack linux
    and then a make menuconfig -C build/linux-* (or use the method you prefer
    instead of menuconfig). Then you should backup your build/linux-*/.config
    into packages/linux/config/linux.conf.

* Lirc :

    Lirc allows you to control the GeeXboX using a remote controller. First
    you have to choose the file describing your specific remote in
    build/lirc-*/remotes (after doing a scripts/unpack lirc) and add it
    in packages/lirc/install. Then you should choose your device (default
    is /dev/ttyS0 (COM1)) and the lirc driver and put them in a file
    called packages/lirc/config/lircd_$REMOTE.

    Then you can choose your key mapping in the file
    packages/lirc/config/lircrc_$REMOTE. For each mapping you have to select
    a button (pike their names in the remote definition file) and associate an
    action to it. The action is one of MPlayer's actions (you can find a list
    in the html file build/MPlayer-*/DOCS/documentation.html#commands).


| HACKING
| ~~~~~~~

The first thing you should look at is the initialization script.
In fact there are two initialization scripts. The first one is in
packages/initrd/scripts/linuxrc but you shouldn't need to modify it.

The second one is in config/options and that's where you may put some
customizations. There you can first choose which is the architecture
(i386 or PPC) you want to build GeeXboX for. After that, you can perform
finnest tuning by specifying a certain kind of sub arch (386, pentium, athlon)
in order to optimize the distribution for your specific CPU and gets the best
perfomances off.

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

In addition, the package main directory may contain extra subdirs :
 - config : where all config files belong. These files may be used either
            by the build script to compile the package or by the install
            script to be put to the system's /etc directory.
 - scripts : this subdir may contain runtime initialization scripts related
             to the current package which should be installed by install script
 - patches : this subdir may contain patches to be applied to the sources
             of the package, at unpack time.
 - sources : if files are present here, they will be automatically copied
             into the package's build tree.
 - init.d : contains initialization scripts to be launched at runtime.

You also have to remember that software which run on the GeeXboX have to
be compiled with the uClibc gcc wrapper. Fortunately, the GeeXboX toolchain is
smart enough to automatically build all added packages using the uClibc gcc
wrapper itself.

Do not forget to ask for your package's compilation and installation by
explicitely asking for it in the scripts/stage2 build script.

Finally, the best way to make a package is to look how other packages are done.


| LICENSE
| ~~~~~~~

All the programs used by the GeeXboX are protected by their respective
license. They all are free software and most of them are covered by the
GNU General Public License.

The GeeXboX itself, meaning all the scripts which are used in the building
process, are covered by the GNU General Public License.
