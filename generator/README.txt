
         ooooo                     oo     o  o               oo     o
        M"   "Mo                    Mo  oM"  M                Mo  oM"
       M           oM""Mo  oM""Mo    "Mo"    Mo"""o  oM""Mo    "Mo"
       M    """M   MooooMM MooooMM   oMMo    M    "M M    M    oMMo
       "o     oM   M    o  M    o   oM  Mo   M    oM M    M   oM  Mo
        "MoooM"M   "MooM"  "MooM"  M"    "M  M"ooo"  "MooM"  M"    "M

                             Generator Version



| ABOUT
| ~~~~~

GeeXboX is a kind of "divx box" software. In fact, it is a stand-alone boot
CD which allows you to watch movies or listen to music. It supports many 
formats, such as avi, mpeg, divx, ogm, rm, mp3, ogg, dvd, vcd and cdda. 
It also supports some IR remote controllers and TV-Out for some graphic cards. 
This archive contain the needed files to generate a personalized iso image
of the GeeXboX.


| REQUIREMENTS
| ~~~~~~~~~~~~

To generate a GeeXboX iso, you will need either:
  - a GNU/Linux system with mkisofs and mkzftree.
  - a windows system.


| PERSONALIZATION
| ~~~~~~~~~~~~~~~

The aim of the generator version is to easily personalize the GeeXboX.

The most interesting thing you can do with it is to build a GeeXboX image
with a full movie on it. The movie will be played automatically at boot.
You just have to put your movies or sound files and playlists in the iso
directory.

You can also add some proprietary codecs such a rv9 or wmv9, simply by adding
them in the iso/GEEXBOX/codecs directory. You can find those codecs in the
package http://www2.mplayerhq.hu/MPlayer/releases/codecs/extralite.tar.bz2.
Useful files in it are :
  * Real codecs (used to play rv9 for exemple) : atrc.so.6.0, cook.so.6.0,
      sipr.so.6.0, drv2.so.6.0, drv3.so.6.0, drv4.so.6.0.
  * Windows Media codecs (wmv9/wma9) : wma9dmod.dll and wmv9dmod.dll.

You can also modify many options. This can be done simply by editing some
text files.

* Language :
    You can easily select your preferred menu language by editing the file
    generator.sh or generator.bat (depending on your OS). This as no effect
    on DVD language (see the MPlayer section). If your language is not
    available, you can translate the menu in your own language. This simply
    involve the creation of language/menu_LANG.conf and language/help_LANG.txt.

* Lirc :
    You can choose one of the supported remote controller by editing the file
    generator.sh or generator.bat (depending on your OS). The ATI Remote Wonder
    is not managed by lirc and is allways activated, so you don't have to
    modify anything for it. If you want to modify the key binding of your
    remote controler, look at the file lirc/lircrc_REMOTE.

* MPlayer :
    This is where you can do most of the configuration and tweaking.
    The options take place in the file iso/GEEXBOX/etc/mplayer/mplayer.conf.
    You may want to modify options such as OSD font size (subfont-text-scale).
    You can also add lots of options such as DVD default language
    (eg: alang=fr,en). The best place to find them is the MPlayer manpage
    for Linux users (man -l build/MPlayer-*/DOCS/mplayer.1). You should also
    have a look at the MPlayer documentation (http://mplayerhq.hu/DOCS/).

* tvout :
    Enabling the TV-Out is achieved with the help of multiple small programs
    dedicated to different video card brands. Currently we use atitvout
    for ATI cards, s3switch for S3 cards and nvtv for nVidia cards (and
    possibly intel i810 and 3dfx cards). The configuration of those programs
    is done in iso/GEEXBOX/etc/tvsettings. There you can choose the TV
    standard you want (pal, ntsc...) and you can also modify specific
    options for nvtv.

* network :
    You can configure the network in the file iso/GEEXBOX/etc/network.
    There you can chose the IP address used by the GeeXboX (default is to
    use DHCP and if it don't work, fall back to 192.168.0.54.
    You can also specify a login and a password which will be used to connect
    to windows shares (default will only connect to anonymous shares).


| GENERATION
| ~~~~~~~~~~

First have a look at the personalization section just above.

Then basically you can generate the iso under Linux by launching
  ./generator.sh
or under windows by launching
  generator.bat


| LICENSE
| ~~~~~~~

All the programs used by the GeeXboX are protected by their respective
license. They all are free software and most of them are covered by the
GNU General Public License.
The GeeXboX itself, meaning all the scripts which are used in the building
process, are covered by the GNU General Public License.


| AUTHOR
| ~~~~~~

Aurelien Jacobs  <aurel@geexbox.org>


| SPECIAL THANKS
| ~~~~~~~~~~~~~~

Benjamin Zores  <ben@geexbox.org>  for his patches, testings and the website.


| THANKS
| ~~~~~~

Serial Cleaner   <serial.cleaner@gmx.net>  for his setcd patch and hauppauge
                                          remote support.
Hervé Urbain     <dersie.urbain@wanadoo.fr>  for his logitech remote support.
David Legrand    <david@pcinpact.com>  for lending us his ATI Remote Wonder.
Mickaël Beugnier <Mitch@no-log.org>  for the GeeXboX logo and bootlogo design.
Andrighetto Riccardo  <geexbox@truzzone.it> for his Italian translation.
Plom             <tbb.plom@libertysurf.fr>  for his leadtek remote support.
Eva Mikulcikova  <evmi@seznam.cz>  for his czech and slovak translations.
The whole free software community, and especially the whole MPlayer team.
