
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

Y también 500 megas de espacio libre en disco.


| PERSONALIZACIÓN
| ~~~~~~~~~~~~~~~

Cuando GeeXboX está instalado en un disco duro o es generado, resulta muy
fácil personalizarlo.

Puedes añadir algunos códecs propietarios como rv9 o wmv9, copiándolos
en el directorio iso/GEEXBOX/codecs. Puedes encontrar estos códecs en el
paquete http://www.mplayerhq.hu/MPlayer/releases/codecs/ .

Ficheros útiles en el paquete de códecs:
  * Códecs de Real (usados para reroducir ficheros rv9 por ejemplo) : atrc.so.6.0,
     cook.so.6.0, sipr.so.6.0, drv3.so.6.0, drv4.so.6.0.
  * Códecs de Windows Media (wmv9/wma9) : wma9dmod.dll y wmv9dmod.dll.

También puedes modificar muchas opcionesa través de la edición de algunos
ficheros de texto.

* Idioma :
    Puedes seleccionar fácilmente tu idioma de menú favorito editando
    el fichero GEEXBOX/etc/lang. Esto no tiene efecto en el lenguaje del DVD
    (mira en la sección de MPlayer). Si tu idioma no está disponible, puedes
    traducir el menú a tu propio idioma, lo que conlleva la creación de los
    archivos GEEXBOX/etc/mplayer/menu_LANG.conf y
    GEEXBOX/usr/share/mplayer/help_LANG.txt.

* MPlayer :
    Aquí es donde puedes hacer la mayor parte de las configuraciones y ajustes.
    Las opciones se encuentran en el fichero packages/MPlayer/mplayer.conf.
    Quizás quieras modificar opciones como tamaño de fuente del OSD
    (subfont-text-scale). También puedes añadir muchas opciones como idioma
    por defecto para DVD (ej: alang=es,en). El mejor sitio para encontrar
    estas opciones es en la pagina man de MPlayer, para usuarios de linux
    (man -l build/MPlayer-*/DOCS/mplayer.1). Tambien deberias echar un vistazo
    a la documentación de MPlayer (http://mplayerhq.hu/DOCS/).
    Otro archivo que quizá quieras modificar es packages/MPlayer/menu.conf.
    Puedes quitar elementos del menú que no necesites, o, por ejemplo,
    traducirlos a tu propio idioma.
    El ultimo archivo que puedes mirar es packages/MPlayer/build, que contiene
    la selección de opciones que están compiladas en MPlayer.

* Salida de TV :
    La activación de la salida de TV se consigue con la ayuda de múltiples
    programas dedicados a las diferentes marcas de tarjetas.
    Actualmente usamos atitvout para las tarjetas Ati, s3switch para las
    tarjetas S3 y nvtv para las tarjetas Nvidia (y posiblemente las intel
    i810 y las 3dfx). La configuracion de estos programas se realiza en
    config/tvout. Ahí puedes seleccionar el estandar de TV que quieras
    (pal para España, ntsc en Iberoamérica...) y modificar otras opciones
    específicas de nvtv.

* Lirc :
    Puedes elegir uno de los controladores soportados editando el archivo
    GEEXBOX/etc/remote. Lirc no puede controlar ATI Rempote Wonder y está
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


| GENERACIÓN
| ~~~~~~~~~~

Primero hecha un vistazo a la sección de personalización detallada más arriba.

Para generar la iso únicamente tienes que ejecutar en Linux
   ./generator.sh

o
  generator.bat
bajo windows.


| INSTALACIÓN
| ~~~~~~~~~~~

Lo primero que necesitas es crear una partición FAT16 con unos 16 MB libres.

A partir de aquí, puedes instalar GeeXboX bajo Linux ejecutando
  ./installator.sh
Responde a todas las preguntas y ten mucho cuidado con ellas. Léelas dos veces
y, si no entiendes alguna, para la instalación.


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
  scripts/get package        # descargar el paquete
  scripts/unpack package     # desempaqueta y prepara el paquete
  scripts/build package      # construye el paquete
  scripts/install package    # instala el paquete con el prefijo $INSTALL
  scripts/clean package      # limpia el árbol de fuentes del paquete
  make exec                  # lanza directamente GeeXboX en una jaula
                             # AVISO: esta característica es áltamente experimental
                             # úsala a tu propio riesgo.

Si has hecho una versión modificada de GeeXboX, puedes construir fácilmente un
pequeño tar.bz2 con:
  make dist
o un tar completo (conteniendo todos los fuentes) con:
  make fulldist
o un generador geexbox con:
  make generator
o un instalador geexbox con:
  make installator


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
    MPlayer, y puedes encontrar un listado en
    build/MPlayer-*/DOCS/documentation.html#commands.


| HACKING
| ~~~~~~~

El primer sitio donde debes mirar es en el script de inicialización.
De hecho, existen dos scripts de inicialización. El primero está en
packages/initrd/linuxrc, aunque no deberías necesitar modificarlo.
El segundo se encuentra en config/init y es donde puedes personalizar.

Lo siguiente que podría interesarte es la creación de un nuevo "paquete".
Un paquete consiste en una gran cantiad de scripts que han de seguir algunas
reglas. Todos los scripts deben funcionar sobre un directorio llamado como
el programa que quieres empaquetar, dentro del directorio packages.
Una lista de scripts que puedes crear:
 - url: sólo una lista de urls para conseguir los fuentes del programa.
 - unpack: qué hacer después de desempaquetar los fuentes. Por ejemplo,
            puedes modificar algunos ficheros de configuración. Ésto no
            incluye aplicar parches.
 - need_build: es llamado cuando el paquete ha terminado de compilarse, con
            el objetivo de estar seguros de que no necesita volver a cons-
            truirse. Debe borrar el archivo .stamps/"nombre del paquete"/build,
            si el paquete debe ser recompilado.
 - build: todos los pasos necesarios para compilar el programa.
 - install: todos los pasos necesarios para instalar el programa. El prefijo
            de instalación debe ser $INSTALL.

Cuando un archivo de las url se llama patch-nombre_del_programa-... se aplica
automáticamente a los fuentes desempaquetados del programa.

Además, debes recordar que el software que corre en GeeXboX debe ser compilado
con el wrapper gcc de uClibc.

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
