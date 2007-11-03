
         ooooo                     oo     o  o               oo     o
        M"   "Mo                    Mo  oM"  M                Mo  oM"
       M           oM""Mo  oM""Mo    "Mo"    Mo"""o  oM""Mo    "Mo"
       M    """M   MooooMM MooooMM   oMMo    M    "M M    M    oMMo
       "o     oM   M    o  M    o   oM  Mo   M    oM M    M   oM  Mo
        "MoooM"M   "MooM"  "MooM"  M"    "M  M"ooo"  "MooM"  M"    "M



| ACERCA DE...
| ~~~~~~~~~~~~

Geexbox es un software que convierte tu ordenador en una máquina de
reproducir DivX. De hecho, es un CD auto-arrancable que permite ver
películas o escuchar música. Soporta multitud de formatos, tales como avi,
mpeg, divx, ogm, rm, mp3, ogg, dvd, vcd y cdda. También soporta algunos
mandos a distancia IR y las salidas de TV de ciertas tarjetas. Este archivo
contiene los ficheros necesarios para generar una imagen iso personalizada
de GeexBoX.


| REQUISITOS
| ~~~~~~~~~~

Para generar una iso GeeXboX, necesitas:
  - un sistema GNU/Linux con mkisofs y mkzftree.
  - un sistema MAC OS X con mkisofs y mkzftree.
  - un sistema windows.

Para instalar GeeXboX, necesitarás:
  - un sistema GNU/Linux con syslinux.

Para construir GeeXboX, te harán falta algunas utilidades clásicas:
  - un sistema GNU/Linux funcionando.
  - el compilador gcc para el lenguaje C.
  - GNU make.
  - el comando patch.
  - el ensamblador nasm.
  - bzip2 y gzip.
  - mkfs.ext2 y mkfs.vfat.
  - la utilidad de descarga wget (no necesaria para todo el paquete GeeXboX).
  - mkisofs y mkzftree para construir la imagen iso.
  - mkzftree para comprimir los archivos en la imagen iso.
  - cdrecord (para tostar la iso).

Y también 1.2 GB de espacio libre en disco.


| PERSONALIZACIÓN
| ~~~~~~~~~~~~~~~

Cuando GeeXboX está instalado en un disco duro o es generado, resulta muy
fácil personalizarlo.

Puedes añadir algunos códecs propietarios como rv9 o wmv9, copiándolos
en el directorio iso/GEEXBOX/codecs. Puedes encontrar estos códecs en el
paquete http://www.geexbox.org/releases/extra-codecs-nonfree.tar.gz

También puedes modificar muchas opciones a través de la edición de algunos
ficheros de texto.

* Idioma :
    Puedes seleccionar fácilmente tu idioma de menú favorito siguiendo estos pasos:

    - Usuarios GNU/Linux:
    Edita el script generator.sh y cambia la línea (cerca del principio):
    "MENU_LANG=en" a tu idioma preferido.
    Por ejemplo: para ponerlo en español, cambiala a "MENU_LANG=es".

    - Usuarios Microsoft Windows:
    Cuando arrancas, generator.exe te da una lista de selección que te
    permitirá escoger tu lenguaje preferido.

    NOTA: Esto no tiene efecto en el lenguaje del DVD (mira en la sección de
    MPlayer).

    Si tu idioma no está disponible, puedes traducir el menú a tu propio
    idioma, lo que conlleva la creación de los archivos
    language/menu_LANG.conf y language/help_LANG.txt, asi como añadir LANG
    a language/lang.conf.

* Firmware auto-loader :
    GeeXboX soporta muchos controladores libres pero, desafortunadamente,
    algunos requieren firmware propietario (no libre y por tanto no incluido
    en GeeXboX ISO) para funcionar correctamente. El firmware es un fichero
    binario que es enviado por el dispositivo (PCI o USB) cuando se carga el
    controlador. Por cada controlador el firmware tiene un fichero
    especifico con un nombre predeterminado.

    Si tienes hardware que requiere firmware adicional (suelen ser tarjetas
    WIFI y sintonizadoras de TDT), necesitaras buscar en la página del
    fabricante los ficheros de firmware. La mayor parte de los casos
    encontraras un link para descargarlos en la sección de drivers para
    Linux. Algunas veces puedes necesitar extraer los ficheros de los
    drivers para windows del CD de drivers que venia con el hardware.

    Una vez que tengas el fichero del firmware, simplemente ponlo en el
    directorio /firmwares. Puedes poner tantos firmware como quieras y
    GeeXboX automáticamente buscará y usará los ficheros correctos cuando
    se necesiten. Solo has de tener la precaución de tener el fichero de
    firmware correcto y generar una nueva ISO.

    El GeeXboX ISO generator puede descargar los firmwares automáticamente.
    Puedes seleccionar todos los ficheros de firmware y el generador los
    añadirá. Pero para que esta característica funcione debes tener acceso a
    internet en el PC que ejecutas el GeeXboX generator.

* MPlayer :
    Aquí es donde puedes hacer la mayor parte de las configuraciones y
    ajustes. Las opciones se encuentran en el fichero
    packages/MPlayer/config/mplayer.conf. Quizás quieras modificar opciones
    como tamaño de fuente del OSD (subfont-text-scale).

    También puedes añadir muchas opciones como idioma por defecto para DVD
    (ej: alang=es,en). El mejor sitio para encontrar estas opciones es en la
    pagina man de MPlayer, para usuarios de linux
    (man -l build/MPlayer-*/DOCS/mplayer.1). Tambien deberias echar un vistazo
    a la documentación de MPlayer (http://mplayerhq.hu/DOCS/).

    Otro archivo que quizá quieras modificar es packages/i18n-texts/config/menu.conf.
    Puedes quitar elementos del menú que no necesites, o, por ejemplo, traducirlos
    a tu propio idioma.

    El ultimo archivo que puedes mirar es packages/MPlayer/build, que contiene
    la selección de opciones que están compiladas en MPlayer.

* Salida de TV :
    La activación de la salida de TV se consigue con la ayuda de múltiples
    programas dedicados a las diferentes marcas de tarjetas.
    Actualmente usamos atitvout para las tarjetas Ati, s3switch para las
    tarjetas S3 y nvtv para las tarjetas Nvidia (y posiblemente las intel
    i810 y las 3dfx). La configuración de estos programas se realiza en
    packages/tvout/config/tvout.

    Ahí puedes seleccionar el estandar de TV que quieras (PAL para España,
    NTSC en Iberoamérica...) y modificar otras opciones específicas de nvtv.

    # TV Output Standard (ntsc/pal/secam)
    TVOUT_STANDARD=pal

    Tambien puedes defineir el aspectos de tu televisor (4:3 o 16:9) cambiando
    esta línea:

    TVOUT_ASPECT="4:3"

    Este parametro se usará pata TVOut y para el monitor. Tammbien puedes
    especificar la resolución y las frecuencias horizontal y vertical en caso
    de que tengas un dispositivo inusual, como un proyector o HDTV. Para esto
    edita el fichero /etc/mplayer/mplayer.conf. Los parametros por defecto se
    muestran abajo (quita el comentario '#' de las líneas de frecuencia si deseas
    cambiarla)

    screenw=800
    screenh=600
    #monitor-hfreq=31.5k-50k
    #monitor-vfreq=50-90


* Lirc :
    Puedes elegir uno de los controladores soportados editando el archivo
    GEEXBOX/etc/remote. Lirc no puede controlar ATI Remote Wonder y está
    siempre activado, asi que no tienes que modificar nada para usarlo.
    Si quieres cambiar la asignación de teclas para el mando a distancia,
    mira en el fichero GEEXBOX/etc/lirc/lircrc_REMOTE.

* Red :
    Puedes configurar la red en el archivo GEEXBOX/etc/network.
    Ahí puedes escoger la dirección IP usada por GeeXboX (por defecto
    se usa DHCP, y si no funciona, la IP cambia a 192.168.0.54.). También
    puedes establecer un usuario y contraseña para conectarse a recursos
    compartidos en máquinas windows (por defecto se conecta únicamente a los
    recursos que permitan conexiones anónimas).
    Tambien puedes declarar algunos NFS mounts en GEEXBOX/etc/nfs.

* Wifi :
    Por defecto, GeeXboX intentara autodetectar la configuración de la red.
    Si tienes una tarjeta Ethernet y una tarjeta WiFi, solo la última será
    configurada. Puedes modificar el fichero /etc/network para especificar
    tu configuración de red.

    en este fichero, 4 líneas configuran la tarjeta WiFi:
    * PHY_TYPE="auto"      # Tipo de red (auto|ethernet|wifi)
    * WIFI_MODE="managed"  # Modo de trabajo Wifi (managed|ad-hoc)
    * WIFI_WEP=""          # Wifi WEP key
    * WIFI_ESSID="any"     # Wifi SSID

* Gateway :
    GeeXboX soporta acceso a Internet. En el caso de que tengas una conexión
    a Internet puedes compartirla con tu caja multimedia usando un router.
    Solamente necesitas definir la ip del Gateway (normalmente el router) en
    /etc/network.

    * GATEWAY=""     # Gateway IP ("" for DHCP or no internet connection)

* DNS server :
    Para acceder a Internet, GeeXboX necesita la dirección IP del servidor DNS.
    Si no usas un servidor DHCP, puedes especificar esta IP en /etc/network.

    * DNS_SERVER=""              # DNS Server IP ("" for DHCP or none)

* Configuración de TV:
	GeeXboX soporta entradas de TV y sintonizadoras. El sistema detectará
	automáticamente la tarjeta y sintonizador que tienes instalados. Puedes forzar
	la configuración y eliminar la autodetección. Para ello modifica el fichero
	/etc/tvcard

    #TV CARD/TUNER Model (AUTO for autodetection or look at the following urls)
    #http://www.linuxhq.com/kernel/v2.6/2/Documentation/video4linux/CARDLIST.bttv
    #http://www.linuxhq.com/kernel/v2.6/2/Documentation/video4linux/CARDLIST.tuner

    TV_CARD=AUTO
    TV_TUNER=AUTO
    TVIN_STANDARD=pal

    Debes dejar el parámetro AUTO si quieres conservar la autodetección, o
    reemplazalo por el número de tu tarjeta y sintonizador. Dicho número lo
    encontraras en las direeciones del comentario. Se cuidadoso: antes de forzar
    un tipo de tarjeta y sintonizador debes saber las REFERENCIONAS EXACTAS de
    tu hardware.

    Una vez hecho podras usar las entradas de TV (Vídeo compuesto y S-VHS)
    de tu tarjeta de TV. Del mismo modo puedes usar el sintonizador para ver la TV.
    Para ello debes definir la región en la que vives y debajo añadir la frecuencia
    y nombre de los canales que quieras sintonizar. De nuevo solo tendras que 
    editar el fichero /etc/tvcard

    # TV Channels
    # Syntax : CHAN="Número de canal:Nombre del canal"
    # Example :
    # CHAN="26:France 2"
    # CHAN="K08:Canal +"
    # TV Channels List
    # Available : france, europe-east, europe-west, us-bcast, us-cable
    CHANLIST=france

    Por favor, se cuidadoso cuando edites los canals y usa la misma sintaxis que
    en el ejemplo. El nombre de los canales se mostrará en el menú.
    NOTA: Durante la instalación el programa te permitira buscar los canales
    automáticamente, pero no te permitirá ordenarlos.

* Configuración de Sonido :
    GeeXbox soporta salida de audio analógico a traves del JACK de la tarjeta de
    sonido. Tambien puede sacarte audio digital por los conectores RCA o SPDIF
    (si es que lo tienes). Por defecto esta configurado para audio analógico.
    Puedes cambiar la configuración editando el fichero /etc/audio

    # Output using SPDIF (yes/no), otherwise ANALOG output
    SPDIF=no

    Si quieres conectar tu GeeXbox a un amplificador externo para decodificar
    AC3 o DTS debes poner "SPDIF=yes"


* Post-procesado de Vídeo :
    Post-Processing es un sistema por software para mejorar la calidad de la imagen.
    Esto consume un montón de potencia de CPU pero la imagen se verá bastante mejor.
    Usando los filtros internos del MPlayer, GeeXbox puede hacer deblocking
    horizontal y vertical, deringing y  auto luminance para mejorar la imagen. Por
    defecto el post-procesado esta dehabilitado para poder funcionar en equipos
    antiguos. Puedes habilitarlo editando el fichero /etc/mplayer/mplayer.conf
    Simplemente elimina el '#' de la tercera línea.

    # Set Post Processing (h deblock, v deblock, dering, auto luminance)
    # Consumes CPU power, disabled for low configs, uncomment to enable it.
    #vf=pp=hb:a/vb:a/dr:a/al:a


* Tarjetas DXR3/Hollywood+ :
    Usuarios con estas tarjetas de descompresión por hardware NO NECESITAN tener
    una tarjeta de video ni tarjeta de sonido para usar GeeXbox. Desgraciadamente
    solo el puede ser usado el conector tvout (no podras poner monitor). Deberas
    poner la norma de TV (PAL/NTSC) en el fichero /etc/tvout y la salida de sonido
    deseada (Analógica o SPDIF) en el fichero /etc/audio.


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

* Recording Capabilities :
    GeeXboX allows you to record the stream you're watching and even
    proceed with live pause (a.k.a. Time-Shifting). Unfortunately, this
    operation is CPU-time consuming (realtime encoding and playback of a
    stream) and so, you may require a fairly recent computer to be able
    to fully make use of this feature.

    The control menu comes with a recorder-dedicated sub-menu that allows you
    to monitor the recording status (on/off), check for the recorded files
    destination, but also start/stop the record process and select the
    encoding profile you want to use. The record process can also be started
    (or stopped) from the keyboard, using the "i" key, or from the remote
    controller.

    There's unfortunately a requirement to the use of recording capabilities :
    you need to be able to store the recorded files somewhere. This can be
    an internal/external hard drive or a remote NFS/Samba share. The only
    thing that matters is that the filesystem has to be Linux-writable (i.e.
    NTFS disk aren't supported). Be aware while recording that your disk will
    be remounted R/W to allow file storage. Once the record process has ended,
    GeeXboX should remount your disk as R/O for safety measure but this might
    not work. It is then highly recommended when making use of the recording
    capabilities that you perform a clean shutdown of the computer to avoid a
    potential filesystem or data corruption. We won't take any responsibility
    for data loss.

    The recorded files destination path has to be specified at installation or
    ISO generation in the /etc/recorder configuration file. Simply replace the
    following setting with your own one :

    SAVE_PATH="/tmp"

    The default behavior is to save to /tmp but as it's a RAMdisk, GeeXboX is
    smart enough to not proceed so. Considering that, there's no way GeeXboX
    will try to record or store files to your computer unless you'd
    explicitely specify a destination path.

    In the same way, you can also specify the default encoding profile to be
    used when recording. You can always change to another one at runtime
    through the recorder option menu. A few profiles are available (see the
    /etc/mplayer/mencoder.conf file for a complete list and details) and they
    all feature different level of quality or encoding speed. You might choose
    one over another, according to your CPU power. According to the input
    stream (low-resolution WebTV, regular analog TV, digital HDTV ...), one
    profile might fit better than another. Below is a short non-exhaustive
    list of available encoding profiles :

     - MPEG 1
     - MPEG 2
     - Video CD (PAL/NTSC)
     - Super Video CD (PAL/NTSC)
     - DVD Video (PAL/NTSC)
     - Stream Dump (can be safely used on DVB MPEG-TS streams for example
       to avoid a re-encoding process).

    The default encoding profile has to be specified in the /etc/recorder
    configuration file, as below :

    RECORD_PROFILE="mpeg1"

    Unless you know exactly what you're doing, it is highly recommended that
    you use the ISO generator to select the default profile.

    When watching a TV stream and starting the record process, GeeXboX image
    might freeze a few seconds (time for MEncoder to start recording) and will
    start playing back the currently recording file. If you're CPU is too
    slow, the playback will lag, waiting for the frames to be encoded. If this
    happens, you only have 2 solutions :

     - use a more powerful HTPC.
     - use a fastest (i.e. less quality) encoding profile.

    When stopping the recording process, GeeXboX will stop MEncoder and will
    reload the original input stream.


| GENERACIÓN
| ~~~~~~~~~~

Primero echa un vistazo a la sección de personalización detallada más arriba.

Para generar la iso únicamente tienes que ejecutar en Linux:
   ./generator.sh

o en Windows
  generator.exe


| INSTALACIÓN
| ~~~~~~~~~~~

Lo primero que necesitas es crear una partición FAT16/32 o EXT2/3 con al menos
16 MB libres. (Recomendamos usar EXT3)

A partir de aquí, puedes instalar GeeXboX bajo Linux ejecutando
  ./installator.sh
Responde a todas las preguntas y ten mucho cuidado con ellas. Léelas dos veces
y, si no entiendes alguna, para la instalación.

Pero la forma más facil de instalar GeeXboX es arrancar desde el CD y teclear
"install" en el menu del isolinux.


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
directory and copy a full GEEXBOX tree in this directory. For example
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


| COMPILANDO
| ~~~~~~~~~~

Lo primero es echar un vistazo a la sección de configuración, más arriba (al
menos, la parte general y la que se refiere al tostado en CD).

A continuación, puedes crear la iso básicamente con:
  make
O puedes quemarla directamente en CD con:
  make burn

Cuando se haya terminado, puedes ahorrar espacio de disco limpiando el árbol
de compilación:
  make clean
o haciendo una limpieza total, incluso borrando las fuentes descargadas:
  make distclean

Existen también otros comandos más avanzados si quieres enredar en las tripas
de GeeXboX:
  scripts/get package          # descargar el paquete
  scripts/unpack package       # desempaqueta y prepara el paquete
  scripts/build package        # construye el paquete
  scripts/install package      # instala el paquete con el prefijo $INSTALL
  scripts/clean package        # limpia el árbol de fuentes del paquete
  scripts/clean --full package # límpia el paquete construido y sus fuentes

Si has hecho una versión modificada de GeeXboX, puedes construir fácilmente un
pequeño tar.bz2 con:
  make dist
o un tar completo (conteniendo todos los fuentes) con:
  make fulldist
o un generador geexbox con:
  make generator
o un instalador geexbox con:
  make installator
o un directorio para arranque remoto con :
  make pxe


| CONFIGURACIÓN
| ~~~~~~~~~~~~~

* Opciones globales:
    Éste es el primer apartado en el que debes fijarte antes de intentar
    compilar GeeXboX. Se encuentra en el archivo config/options, y debería
    explicarse por sí mismo. En este fichero puedes seleccionar la familia
    de tu CPU, el tema a usar, y si quieres utilizar fuentes truetype o no.
    Además, deberías modificar la configuración de la grabadora de CD para
    poder grabar directamente la ISO.

* Linux:
    El archivo packages/linux/linux.conf es una configuración clásica de Linux.
    Puedes editarlo a mano, o también puedes ejecutar scripts/unpack linux
    y hacer make menuconfig -C build/linux-* (o utilizar el método que
    prefieras en vez de menuconfig). Es conveniente que hagas una copia de
    seguridad de build/linux-*/.config en packages/linux/linux.conf.
    Lo más "difícil" que puede ocurrir es mantener tamaño del kernel lo
    suficientemente reducido como para que quepa en una imagen de un disquete
    de arranque.

* Lirc:
    Lirc te permite controlar GeeXboX a través de un mando a distancia. Lo
    primero que debes hacer es escoger el archivo que describe tu mando en
    concreto en build/lirc-*/remotes (despues de hacer scripts/unpack lirc) y
    añadirlo a packages/lirc/install. A partir de aquí, escoge un dispositivo
    (por defecto es /dev/ttyS0, o COM1) y el driver lirc y colócalos en un
    archivo con el nombre packages/lirc/lircd_$REMOTE. Entonces podrás escoger
    la asignación de teclas en packages/lirc/lircrc_$REMOTE. En cada asignación
    tienes que seleccionar un botón (coge los nombres del archivo de definición
    del mando a distancia) y asociarle una acción. Esta acción pertenece a
    MPlayer, y puedes encontrar un listado en el fichero
    build/MPlayer-*/DOCS/documentation.html#commands.


| HACKING
| ~~~~~~~

El primer sitio donde debes mirar es en el script de inicialización.
De hecho, existen dos scripts de inicialización. El primero está en
packages/initrd/linuxrc, aunque no deberías necesitar modificarlo.
El segundo se encuentra en config/init y es donde puedes personalizar.

Lo siguiente que podría interesarte es la creación de un nuevo "paquete".
Un paquete consiste en una gran cantidad de scripts que han de seguir algunas
reglas. Todos los scripts deben funcionar sobre un directorio llamado como
el programa que quieres empaquetar, dentro del directorio packages.

Una lista de scripts que puedes crear:
 - url: sólo una lista de urls para conseguir los fuentes del programa.
 - unpack: qué hacer después de desempaquetar los fuentes. Por ejemplo,
            puedes modificar algunos ficheros de configuración. Ésto no
            incluye aplicar parches.
 - need_build: es llamado cuando el paquete ha terminado de compilarse, con
            el objetivo de estar seguros de que no necesita volver a construirse.
            Debe borrar el archivo .stamps/"nombre del paquete"/build,
            si el paquete debe ser recompilado.
 - build: todos los pasos necesarios para compilar el programa.
 - install: todos los pasos necesarios para instalar el programa. El prefijo
            de instalación debe ser $INSTALL.

Tambien, en el directorio principal del paquete puedes poner estas carpetas :
 - config : Donde estan todos los ficheros de configuración. Estos ficheros pueden
            ser usados para construir un script para compilar el paquete o por un
            script de instalacion para ser puestos en el directorio /etc
 - scripts : Este subdirectorio contiene los runtime initialization scripts
             relativos al paquete actual, los cuales seran instalados por el script
             de instalación.
 - patches : Este subdirectorio contiene los  parches que seran aplicados a las
             fuentes cuando se descompriman.
 - sources : Si hay ficheros en este subdirectorio, seran copiados automáticamente
             en el arbol del paquete.
 - init.d : contiene scripts de inicialización que serán lanzados en el momento de
            la ejecución.

Cuando un archivo de las url se llama patch-nombre_del_programa-... se aplica
automáticamente a los fuentes desempaquetados del programa.

Además, debes recordar que el software que corre en GeeXboX debe ser compilado
con el wrapper gcc de uClibc. Afortunadamente, el GeeXboX toolchain puede construir
automáticamente todos los paquetes añadidos usando el uClibc gcc wrapper por si
mismo.

Finalmente, el mejor modo de hacer un paquete es observar cómo están hechos
otros paquetes.


| LICENCIA
| ~~~~~~~~

Todos los programas usados por GeeXbox estan protegidos por sus respectivas
licencias. Todos ellos son software libre, y la mayoría están cubiertos por la
Licencia Pública General GNU (GPL).
El propio GeeXboX, incluyendo todos los scripts que son utilizados en el
proceso de construcción y compilación, están cubiertos por la Licencia Pública
General GNU (GPL).
