
         ooooo                     oo     o  o               oo     o
        M"   "Mo                    Mo  oM"  M                Mo  oM"
       M           oM""Mo  oM""Mo    "Mo"    Mo"""o  oM""Mo    "Mo"
       M    """M   MooooMM MooooMM   oMMo    M    "M M    M    oMMo
       "o     oM   M    o  M    o   oM  Mo   M    oM M    M   oM  Mo
        "MoooM"M   "MooM"  "MooM"  M"    "M  M"ooo"  "MooM"  M"    "M

                             Versión Generator



| SOBRE GEEXBOX
| ~~~~~

Geexbox es un software que convierte tu ordenador en una máquina de
reproducir DivX. De hecho, es un CD auto-arrancable que permite ver
películas o escuchar música. Soporta multitud de formatos, tales como avi,
mpeg, divx, ogm, rm, mp3, ogg, dvd, vcd y cdda. También soporta algunos
mandos a distancia por IR y salidas de TV para algunas tarjetas. Este archivo
contiene los ficheros necesarios para generar una imagen iso personalizada
de GeexBoX.


| REQUISITOS
| ~~~~~~~~~~

Para generar una iso GeeXboX, necesitas o:
  - un sistema GNU/Linux con mkisofs y mkzftree.
  - un sistema windows.


| PERSONALIZACIÓN
| ~~~~~~~~~~~~~~~

El objetivo de esta versión generadora es la de personalizar facilmente GeeXboX.

Lo más interesante que puedes hacer con esta versión es la de construir
una imagen GeeXboX incluyendo una película. Esta película se reproducirá 
automáticamente al arrancar. Sólo tienes que poner tus películas o ficheros
de sonido y listas de reproducción en el directorio iso.

También puedes añadir algunos códecs propietarios como rv9 o wmv9, simplemente
añadiéndolos en el directorio iso/GEEXBOX/codecs. Puedes encontrar estos códecs
en el paquete http://www2.mplayerhq.hu/MPlayer/releases/codecs/extralite.tar.bz2.
Ficheros útiles :
  
  * Códecs de Real (usados para reroducir ficheros rv9 por ejemplo) : atrc.so.6.0,
     cook.so.6.0, sipr.so.6.0, drv2.so.6.0, drv3.so.6.0, drv4.so.6.0.
  
  * Códecs Windows Media (wmv9/wma9) : wma9dmod.dll and wmv9dmod.dll.

También puedes modificar muchas opciones. Esto se hace editando algunos ficheros
de texto.

* Idioma :
    Puedes seleccionar facilmente tu idioma preferido para el menú editando
    el fichero generator.sh o generator.bat (dependiendo de tu SO). Esto no 
    tiene efecto en el lenguaje del DVD (ver sección del MPlayer). Si tu idioma
    no está disponible, puedes traducir el menú a tu propio idioma simplemente
    creando los archivos language/menu_LANG.conf y language/help_LANG.txt.
	
* Lirc :
    Puedes elegir uno de los controladores soportados editando el fichero
    generator.sh o generator.bat (dependiendo de tu SO). El ATI Remote Wonder
    no es controlado por lirc y esta siempre activado, asi que no tienes que
    modificar nada para su uso. Si quieres modificar la asignación a las teclas
    para tu controlador remoto, mira en el fichero lirc/lircrc_REMOTE.

* MPlayer :
    Aquí es donde puedes hacer la mayor parte de las configuraciones y ajustes.
    Las opciones se encuentran en el fichero iso/GEEXBOX/etc/mplayer/mplayer.conf.
    Quizás quieras modificar opciones como tamaño de fuente del OSD (subfont-text-scale).
    También puedes añadir muchas opciones como idioma por defecto del DVD 
    (ej: alang=es,en). El mejor sitio para encontrarlas es en la pagina man
    de MPlayer, para usuarios de linux (man -l build/MPlayer-*/DOCS/mplayer.1)
    Tambien deberias echar un vistazo a la documentación de Mplayer
    (http://mplayerhq.hu/DOCS/).

* Salida de TV :
    Activando la Salida de TV se consigue con la ayuda de múltiples
    programas dedicados a las diferentes marcas de tarjetas.
    Actualmente usamos atitvout para las tarjetas Ati, s3switch para
    las tarjetas S3 y nvtv para las tarjetas Nvidia (y posiblemente
    las intel i810 y las 3dfx. 
    La configuracion de estos programas se realiza en    
    iso/GEEXBOX/etc/tvsettings. Ahí puedes elegir el estandar de TV
    que quieras (pal para España, ntsc...) y puedes modificar opciones específicas
    para nvtv.

* Red :
    Puedes configurar la red en el fichero iso/GEEXBOX/etc/network.
    Ahí puedes escoger la dirección IP usada por GeeXboX ( por defecto
    usa DHCP y si no funciona, pasa a 192.168.0.54.)
    También puedes establecer un login y una clave que se usará para
    conectarse a maquinas windows ( por defecto se conecta anónimamente)


| GENERACIÓN
| ~~~~~~~~~~

Primero hecha un vistazo a la sección de personalización detallada más arriba.

Para generar la iso únicamente tienes que ejecutar en Linux
   ./generator.sh

o bajo windows ejecutando
  generator.bat


| LICENCIA
| ~~~~~~~~

Todos los programas usados por GeeXbox estan protegidos por sus respectivas
licencias. Todos ellos son software libre y muchos estan cubiertos por la
Licencia Pública General GNU

GeeXbox, entendiendose por todos los scripts que son usados durante el proceso
de construcción del mismo, está cubierto por la Licencia Pública General GNU
General Public License (GPL).
 

| AUTOR
| ~~~~~~

Aurelien Jacobs  <aurel@geexbox.org>


| GRACIAS ESPECIALES
| ~~~~~~~~~~~~~~~~~~

Benjamin Zores  <ben@geexbox.org>  por sus parches, pruebas, y la página web.


| GRACIAS
| ~~~~~~

Serial Cleaner   <serial.cleaner@gmx.net>  por su parche setcd y soporte para
                                           hauppauge.                                          
Hervé Urbain     <dersie.urbain@wanadoo.fr>  por su soporte para control remoto Logitech.
David Legrand    <david@pcinpact.com>  por prestarnos su ATI Remote Wonder.
Mickaël Beugnier <Mitch@no-log.org>  por el logo de GeexBoX y el diseño del logo de arranque. 
Andrighetto Riccardo  <geexbox@truzzone.it> por su traducción al italiano.
Plom             <tbb.plom@libertysurf.fr>  por su soporte para el control remoto leadtek
Eva Mikulcikova  <evmi@seznam.cz>  por sus traducciones al checo y al eslovaco.
La entera comunidad del software libre, y especialmente a todo el equipo de MPLayer.


