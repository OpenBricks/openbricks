
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

To generate a GeeXboX iso, you will need either:
  - a GNU/Linux system with mkisofs and mkzftree.
  - a windows system.

To install the GeeXboX, you will need:
  - a GNU/Linux system with syslinux.

To build the GeeXboX, you will need some classic tools:
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

And also 500 MB of hard disk free space.


| PERSONALIZATION
| ~~~~~~~~~~~~~~~

When the GeeXboX is installed on a hard drive or generated, 
it is quite easy to personalize it.

You can add some proprietary codecs such a rv9 or wmv9, simply by adding
them in the GEEXBOX/codecs directory. You can find those codecs in the
package http://www2.mplayerhq.hu/MPlayer/releases/codecs/extralite.tar.bz2.

Useful files in it are :
  * Real codecs (used to play rv9 for exemple) : atrc.so.6.0, cook.so.6.0,
      sipr.so.6.0, drv2.so.6.0, drv3.so.6.0, drv4.so.6.0.
  * Windows Media codecs (wmv9/wma9) : wma9dmod.dll and wmv9dmod.dll.

You can also modify many options. This can be done simply by editing some
text files.

* Language :
    You can easily select your preferred menu language by editing the file
    GEEXBOX/etc/lang. This as no effect on DVD language (see the MPlayer
    section). If your language is not available, you can translate the menu
    in your own language. This simply involve the creation of
    GEEXBOX/etc/mplayer/menu_LANG.conf and
    GEEXBOX/usr/share/mplayer/help_LANG.txt.

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
    is done in config/tvsettings. There you can choose the TV standard you
    want (pal, ntsc...) and you can also modify specific options for nvtv.

* Lirc :
    You can choose one of the supported remote controller by editing the file
    GEEXBOX/etc/remote. The ATI Remote Wonder is not managed by lirc and is
    allways activated, so you don't have to modify anything for it. If you
    want to modify the key binding of your remote controler, look at the file
    GEEXBOX/etc/lirc/lircrc_REMOTE.

* network :
    You can configure the network in the file GEEXBOX/etc/network.
    There you can chose the IP address used by the GeeXboX (default is to
    use DHCP and if it don't work, fall back to 192.168.0.54.
    You can also specify a login and a password which will be used to connect
    to windows shares (default will only connect to anonymous shares).


| CONFIGURATION
| ~~~~~~~~~~~~~

* Global options :
    This is the first thing you should take care of before trying to build
    the GeeXboX. It stands in the file config/options, and it should be
    self explanatory. There you can choose your CPU family, your theme,
    and whether you want truetype fonts or not. You should also modify your
    cd burner configuration to be able to burn directly the iso.

* Linux :
    This is a classical Linux configuration (packages/linux/linux.conf).
    You can edit them by hand, or you can also do a scripts/unpack linux
    and then a make menuconfig -C build/linux-* (or use the method you prefer
    instead of menuconfig). Then you should backup your build/linux-*/.config
    into packages/linux/linux.conf.
    The only "difficult" thing with this is to keep the kernel small enough
    to fit on the boot floppy image.

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


| GENERATION
| ~~~~~~~~~~

First have a look at the personalization section just above.

Then basically you can generate the iso under Linux by launching
  ./generator.sh
or under windows by launching
  generator.bat


| INSTALLATION
| ~~~~~~~~~~~~

First you need to create a FAT16 partition with about 16 MB of free space.

Then you can install the GeeXboX under Linux by launching
  ./installator.sh
And then responding to all the questions. Be carefull during this step.
Read twice each question and stop installing if you don't understand one
of the question.


| BUILDING
| ~~~~~~~~

First have a look at the configuration section just above (at least the
general and the burning parts).

Then basically you can build the iso with :
  make
Or you can directly burn this iso with :
  make burn

When done, you can save disk space by cleaning the build tree :
  make clean
or by doing a full cleaning, even removing downloaded sources :
  make distclean

There's also more advanced commands if you want to hack around the GeeXboX :
  scripts/get package        # download the package
  scripts/unpack package     # unpack and prepare the package
  scripts/build package      # build the package
  scripts/install package    # install the package with the $INSTALL prefix
  scripts/clean package      # clean the package source tree
  make exec                  # launch directly the GeeXboX in a jail
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