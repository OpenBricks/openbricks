
         ooooo                     oo     o  o               oo     o
        M"   "Mo                    Mo  oM"  M                Mo  oM"
       M           oM""Mo  oM""Mo    "Mo"    Mo"""o  oM""Mo    "Mo"
       M    """M   MooooMM MooooMM   oMMo    M    "M M    M    oMMo
       "o     oM   M    o  M    o   oM  Mo   M    oM M    M   oM  Mo
        "MoooM"M   "MooM"  "MooM"  M"    "M  M"ooo"  "MooM"  M"    "M




| AVANT PROPOS
| ~~~~~~~~~~~~

La GeeXboX est une sorte "divx box" logicielle. En fait, il s'agit d'un CD
bootable qui vous permet de regarder des films ou d'écouter de la musique.
Il supporte de nombreux formats tels que avi, mpeg, divx, ogm, rm, mp3, ogg,
dvd, vcd et cdda. GeeXboX supporte aussi certains types de télécommandes
infra-rouge et les sorties TV de certaines cartes graphiques.
Cette archive contient tous les fichiers nécessaires pour générer une image
iso personnalisée de la GeeXboX


| PREREQUIS
| ~~~~~~~~~

Pour générer une iso GeeXboX vous devez posséder l'un des sytémes suivants:
  - GNU/Linux avec mkisofs et mkzftree.
  - MAC OS X avec mkisofs et mkzftree.
  - Windows.

Pour installer la GeeXboX, vous aurez besoin :
  - un système GNU/Linux avec SysLinux.

Pour construire votre propre GeeXboX, vous nécessiterez les outils classiques :
  - un système GNU/Linux opérationnel.
  - Le compilateur C GCC.
  - GNU make
  - La commande patch.
  - L'assembleur nasm.
  - bzip2 et gzip.
  - mkfs.ext2 et mkfs.vfat
  - L'outil de téléchargement wget (non nécessaire pour le paquetage 
  GeeXboX complet).
  - mkisofs et mkzftree pour construire l'image ISO.
  - mkzftree pour compresser les fichiers de l'image ISO.
  - cdrecord (pour graver l'image).

Et environ 1.2 Go d'espace disque disponible.


| PERSONALISATION
| ~~~~~~~~~~~~~~~

Le but du générateur est de personnaliser facilement sa GeeXboX.

La chose la plus interessante que vous puissiez faire et de générer une image
comportant une vidéo complète qui sera lu automatiquement au boot. Vous avez
juste à copier vos vidéos (ou vos fichiers sons) et vos playlists dans le
repertoire iso.

Vous pouvez aussi ajouter des codecs propriétaires comme le rv9 ou le wmv9, en
les copiant simplement dans le répertoire iso/GEEXBOX/codecs. Ces codecs se
trouvent ici :
http://www.mplayerhq.hu/MPlayer/releases/codecs/

Les fichiers utiles sont les suivants:

  * Codecs Real (pour le rv9 par exemple) : atrc.so.6.0, cook.so.6.0,
      sipr.so.6.0, drv3.so.6.0 et drv4.so.6.0.
  * Codecs Windows Media (wmv9/wma9) : wma9dmod.dll et wmv9dmod.dll.

Vous pouvez modifier d'autres options en éditant simplement des fichiers
textes.

* Langue :
    Choisissez la langue de vos menus en éditant le fichier generator.sh ou
    generator.bat (en fonction de votre OS). Cela n'as pas d'effets sur la
    langue du DVD (voir section MPlayer). Si votre langue n'est pas disponible,
    vous pouvez toujours traduire un menu. Il suffit de re-créer les fichiers
    language/menu_LANG.conf and language/help_LANG.txt.

* MPlayer :
    C'est ici que se font la plupart des configurations et modifications.
    Les options se situent dans le fichier packages/MPlayer/mplayer.conf
    Il est possible de changer des options comme la taille des police de l'OSD
    (subfont-text-scale) et beaucoup d'autres choses telles que la langue par
    défaut de lecture des DVD (ex: alang=fr,en). La meilleur façon de
    comprendre ces options est, pour les utilisateurs de Linux, de se référer
    au MAN de MPlayer (man -l build/MPlayer-*/DOCS/mplayer.1). D'autres
    informations sont aussi disponibles sur la documentation officielle
    (http://mplayerhq.hu/DOCS/).
    Il peut également être intéressant de modifier le fichier 
    packages/MPlayer/menu.conf. Vous pouvez les menus qui vous semblent 
    inutiles, ou les traduire dans d'autres langues par exemple. Enfin, le 
    dernier intéressant est packages/MPlayer/build, qui contient la sélection
    d'options de compilation de MPlayer.

* Sortie TV :
    Activer la sortie TV se fait au moyen de nombreux petits utilitaires dédiés
    chacun à une marque carte graphique. Nous utilisons actuellement atitvout
    pour les cartes ATI, s3switch pour les cartes S3 et nvtv pour les cartes
    nVidia (ce qui peut aussi marcher abec les cartes i810 et 3dfx). La
    configuration de ces programmes se fait dans iso/GEEXBOX/etc/tvout.
    Vous pouvez y choisir le standard que vous utilisez (pal, secam...) et y
    modifier les options spécifiques de nvtv.

    # TV Output Standard (ntsc/pal/secam)
    TVOUT_STANDARD=pal

    Vous pouvez également définir le rapport d'image de sortie (mode 4:3 ou
    16:9) via la ligne :

    TVOUT_ASPECT="4:3"

    Ce paramètre est utilisé à la fois pour la sortie TV et pour l'affichage
    classique sur moniteurs CRT ou TFT. Vous pouvez également définir les
    valeurs de hauteur et largeur (en pixels) pour l'affichage, ainsi que les
    fréquences de rafraichissement horizontal et vertical, dans le cas où vous
    utiliseriez un écran panoramique ou encore un rétro-projecteur. Ceci peut
    etre fait en modifiant le contenu du fichier /etc/mplayer/mplayer.conf.
    Les paramètres suivants sont donnés par défaut (veuillez décommenter les
    lignes liées à la fréquence de refraichissement si vous souhaiter les
    utiliser ) :

    screenw=800
    screenh=600
    #monitor-hfreq=31.5k-50k
    #monitor-vfreq=50-90

* Lirc :
    Choisissez la télécommande supporté en éditant le fichier generator.sh ou
    generator.bat (en fonction de votre OS). Faite également attention à
    bien choisir le récepteur infrarouge correspondant dans le même fichier.
    Si vous désirez modifier le mappage des touches reportez vous au fichier
    lirc/lircrc_REMOTE.

* Réseau :
    Le réseau est configurable au niveau du fichier iso/GEEXBOX/etc/network.
    Ici vous reglerez l'adresse IP de la GeeXboX (qui par défaut cherche un
    serveur DCHP ou prend l'IP 192.168.0.54 si elle n'en trouve pas). Il est
    aussi possible de lui assigner un login est un mot de passe (par défaut,
    la GeeXboX ne peut se connecter que sur des partages anonymes).
    Vous pouvez aussi déclarer des montages NFS dans GEEXBOX/etc/nfs.

* wifi :
    Par defaut, le système tente de détecter automatiquement votre
    configuration réseau. Si vous disposez à la fois d'une carte réseau
    Ethernet classique et d'une carte WiFi, seule cette dernière sera
    configurée. Vous pourrez avoir à modifier le fichier /etc/network afin
    d'y configurer vos paramètres réseaux. Dans ce dernier, 4 lignes sont
    relatives aux cartes sans-fils :

    PHY_TYPE="auto"      # Network physical type (auto|ethernet|wifi)
    WIFI_MODE="managed"  # Wifi working mode (managed|ad-hoc)
    WIFI_WEP=""          # Wifi WEP key
    WIFI_ESSID="any"     # Wifi SSID

    Vous pouvez soit conserver la détection automatique, soit forcer
    l'activation du controleur Ethernet ou WiFi. De la même façon, ceci vous
    permettra de choisir entre le mode managed et le mode de communication dit
    ad-hoc et de définir à la fois votre clé WEP et le SSID de votre réseau.

* passerelle :
    La GeeXboX supporte l'accès à Internet. Définissez simplement l'adresse IP
    de la passerelle dans le fichier /etc/network

    GATEWAY=""     # Gateway IP ("" for DHCP or no internet connection)

* configuration TV :
    La GeeXboX supporte les entrées et tuners de cartes TV. Le système essaie
    avec peine de détecter automatiquement le type de carte et de tuners
    utilisés. Vous pouvez forcer les paramètres et ainsi éviter la tentative
    de détection automatique. Veuillez modifier le fichier /etc/tvcard
    tel qu'il suit :

# TV CARD/TUNER Model (AUTO for autodetection or look at the following urls)
# http://www.linuxhq.com/kernel/v2.6/2/Documentation/video4linux/CARDLIST.bttv
# http://www.linuxhq.com/kernel/v2.6/2/Documentation/video4linux/CARDLIST.tuner
TV_CARD=AUTO
TV_TUNER=AUTO

    Laissez le paramètre AUTO si vous souahitez conserver la détection
    automatique, ou remplacez le par le numéro de la carte et du tuner,
    comme décrit dans les URL précédentes. Soyez attentifs : pour forcer les
    types de cartes et de tuners, vous devez connaître les REFERENCES EXACTES
    de votre matériel.

    Une fois cela fait, vous devriez être en mesure d'utiliser les entrées TV
    (Composite et S-VHS) de votre carte TV. De la même manière, vous pouvez
    utiliser le tuner pour regarder la TV. Pour cela, vous devrez définir
    votre région ainsi que les fréquences des chaînes que vous souhaitez
    visionner. Editez simplement le fichier /etc/tvcard :

    # TV Channels
    # Syntax : CHAN="Channel Title":"Channel Frequency"
    # Example :
    # CHAN="France 2":"26"
    # CHAN="Canal +":"K08"
    # TV Channels List
    # Available : france, europe-east, europe-west, us-bcast, us-cable
    CHANLIST=france

    Faites attention lors de l'édition des canaux TV et veillez à utiliser
    la même syntaxe que décrit ci-dessus et les canaux TV devraient
    apparaître dans le menu principal.

* configuration audio :
    La GeeXboX supporte à la fois la restitution audio via la sortie analogique
    ou numérique, en utilisant les connecteurs classiques JACK ou RCA SPDIF.
    Par défaut, la sortie est gérée de manière analogique. Ceci peut etre
    changé en modifiant le fichier /etc/audio :

    # Output using SPDIF (yes/no), otherwise ANALOG output
    SPDIF=no

    Souvenez vous qu'il est nécessaire de régler la sortie en mode numérique
    (SPDIF), si vous souhaitez relier votre carte son à un amplificateur hifi
    externe pour décoder des flux AC3/DTS (en utilisant le mode passthrough).

* Post-Processing vidéo :
    Le Post-Processing est un moyen logiciel pour affiner une image, en la
    rendant plus nette et plus précise. Cela a par contre l'inconvénient de
    consommer une partie du temps processeur afin de rendre l'image plus belle.
    Via l'utilisation des filtres internes à MPlayer, la GeeXboX vous permet de
    minimiser les effets de blocs horizontaux et verticaux, les effets
    d'anneaux de dégradés et de corriger automatiquement la luminosité de votre
    film. Par défaut, le Post-Processing est désactivé, pour éviter de saccader
    sur de petites configurations matérielles. Il vous est possible de
    l'activer très simplement en éditant le fichier /etc/tvout :

    # Set Post Processing (consume CPU power, disable for low configs)
    POSTPROCESS=no

* DXR3/Hollywood+ cards :
    Les utilisateurs de ce type de cartes de décompression n'ont pas besoin
    d'avoir une carte vidéo ou une carte son dans leur ordinateur. Parmi les
    inconvénients, on notera néanmoins que seule la sortie TV peut etre
    utilisée avec ce type de carte (pas d'affichage sur moniteur).
    Vous pouvez etre amené à définir la norme d'image souhaitée (PAL/NTSC)
    via le fichier /etc/tvout ainsi que le type de sortie audio à utiliser
    (Analogique ou SPDIF) via le fichier /etc/audio.


| GENERATION DE L'ISO
| ~~~~~~~~~~~~~~~~~~~

Avant tout, jetez un oeuil sur la section personalisation juste au dessus

Sous Linux, l'ISO est générée en lançant la commande suivante:
  ./generator.sh
et sous Windows:
  generator.exe


| POLICES DE CARACTERES ADDITIONELLES POUR LES SOUS-TITRES
| ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Vous pouvez être amenés à vouloir rajouter un jeu de caractère différent de
celui proposé par défaut (iso8859-1) pour les sous-titres. La première étape
est de s'assurer que la fonte n'est pas déjà inclus dans l'archive officielle
de la GeeXboX. Pour cela, utilisez le generator et regardez si la fonte ne s'y
trouve pas déjà.

La plupart des jeus de caractères sont déjà présents dans l'archive officielle
mais certains n'ont pas pu l'être en raison de leur taille excessive (les
polices asiatiques en particulier).

* Fontes génériques :

  Pour rajouter le support de nouvelles fontes, vous n'avez qu'à extraire votre
  police dans le répertoire font et ajouter le nom de votre fonte à la variable
  FONT du fichier language/lang.conf.

* Support du Chinois :

  Pour inclure les fontes Chinoise dans l'ISO, vous devez au préalable
  récupérer l'archive big5 ou gb2312 utilisée par MPlayer :

    http://www1.mplayerhq.hu/MPlayer/contrib/fonts/chinesefonts/

  et la décompresser et copier dans le répertoire ./font/big5 ou ./font/gb2312
  du generator le contenu du répertoire se terminant par `24` (taille de fonte
  valant 24)

  Par exemple, si vous souhaitez utiliser la police 'gb2312 kai' :

  - Pour les utilisateurs de systèmes GNU/Linux :
  wget http://www1.mplayerhq.hu/MPlayer/contrib/fonts/chinesefonts/gb2312-kai.tar.bz2
  tar -jxf gb2312-kai.tar.bz2
  mv gb2312-kai/gkai00mp24 ../path/to/generator/font/gb2312

  - Pour les utilisateurs de Windows :
  Téléchargez le fichier à l'adresse http://www1.mplayerhq.hu/MPlayer/contrib/fonts/chinesefonts/gb2312-kai.tar.bz2
  Ouvrez le avec WinZip ou équivalent.
  Extrayez le contenu du répertoire gb2312-kai\gkai00mp24 de l'archive vers le
  répertoire ..\path\to\generator\font\gb2312 du generator.


| INSTALLATION
| ~~~~~~~~~~~~

Avant tout, vous devez créer une partition de type FAT16/32 ou EXT2/3 d'une
taille d'environ 16 Mo minimum.

Puis, vous pouvez installer la GeeXboX depuis Linux en lançant simplement
  ./installator.sh

Répondez ensuite à toutes les questions. Lisez les questions avec attention
et stoppez l'installation si vous ne comprenez pas ce que vous faites.

Mais le plus simple reste encore de démarrer la GeeXboX depuis le CD est de
taper "install" au prompt de démarrage.

| BOOT PXE
| ~~~~~~~~

Oui, la GeeXboX est capable de booter depuis le réseau sur une machine
sans disque ! Pour obtenir cela il vous faudra :
 - un serveur DHCP
 - un serveur TFTP
 - un serveur NFS
 - une machine supportant le PXE :-)
Il faut tout d'abord configurer le server DHCP pour qu'il envoie les info de
boot PXE. Voilà un exemple de configuration avec isc dhcp :

allow booting;
allow bootp;

subnet 192.168.0.0 netmask 255.255.255.0 {
  range 192.168.0.128 192.168.0.192;
  option subnet-mask 255.255.255.0;
  option broadcast-address 192.168.0.255;
  next-server 192.168.0.1;
  filename "/tftpboot/GEEXBOX/boot/pxelinux.0";
}

L'option next-server est l'adresse du server TFTP.
Ensuite il faut configurer votre serveur TFTP (tel que atftpd) pour qu'il
serve le répertoire /tftpboot et copier une arborescence GEEXBOX complète
dans ce répertoire. Par exemple il est possible de copier le contenu d'un
CD de GeeXboX depuis un linux AVEC L'OPTION CDROM TRANSPARENT DECOMPRESSION
ACTIVE !! (pour vérifier cela, il suffit de regarder si le fichier sbin/init
de l'arborescence GeeXboX ne contient pas de caractères totalement incohérents)
Si vous avez compilé la GeeXboX vous même a partir des sources, il est aussi
possible de générer l'arborescence GEEXBOX avec make pxe.

Ensuite il faut éditer le fichier /tftpboot/GEEXBOX/boot/pxelinux.cfg/default
pour faire correspondre le nfsroot au bon chemin NFS vers l'arborescence
GeeXboX.

Enfin il reste a configurer NFS pour qu'il exporte l'arborescence GEEXBOX
avec un fichier /etc/exports ressemblant à ceci :

/tftpboot/GEEXBOX (ro)

et un /etc/hosts.allow ressemblant à :

ALL: ALL

Ca devrait être bon. Reste a booter la machine PXE et a voir ce qu'il se passe.


| COMPILATION
| ~~~~~~~~~~~

Tout d'abord, regardez la partie de configuration ci-dessous.

Typiquement, la compilation s'effectue simplement au moyen de : 
  make
Ou vous pouvez directement compiler et graver l'ISO via : 
  make burn

Une fois cela fait, vous pouvez regagner de l'espace disque en effaçant 
l'arborescence de compilation via :
  make clean
ou en effectant un nettoyage complet, éliminant même les sources téléchargées :
  make distclean

Il existe également des commandes plus avancées si vous désirez effectuer
des modifications en profondeur au niveau de la GeeXboX :
  scripts/get package          # télécharge le paquetage
  scripts/unpack package       # prépare le paquetage
  scripts/build package        # compile le paquetage
  scripts/install package      # installe le paquetage dans $INSTALL
  scripts/clean package        # nettoie l'arborescence du paquetage
  scripts/clean --full package # nettoie les sources du paquetage
  make exec                    # lance la GeeXboX dans une cellule
                               # ATTENTION: ceci est une fonction expérimentale
                               # Utilisez là à vos propres risques.

Si vous avez effectué une version modifiée de la GeeXboX, vous pouvez :
construire une archive réduite tar.bz2 via :
  make dist
ou une archive complète (avec l'intégralité des sources) au moyen de :
  make fulldist
ou construire le générateur d'ISO :
  make generator
ou encore l'installateur : 
  make installator
ou enfin une arborescence PXE :
  make pxe


| CONFIGURATION
| ~~~~~~~~~~~~~

* Options Globales :
    C'est la première chose dont vous aurez à vous soucier avant d'essayer de
    compiler la GeeXboX. Elles sont contenues dans le fichier config/options, 
    et devraient être suffisamment explicites.

* Linux :
    Il s'agit d'une configuration Linux classique (packages/linux/linux.conf).
    Vous pouvez éditer le fichier à la main, ou via scripts/unpack linux
    suivi de make menuconfig -C build/linux-* (ou utiliser votre méthode 
    préférée en lieu et place de menuconfig). Puis, vous devrez sauvegarder
    votre fichier build/linux-*/.config dans packages/linux/linux.conf.

* Lirc :
    Lirc vous permet de contrôler la GeeXboX en utilisant une télécommande.
    En premier lieu, vous aurez à choisir le fichier correspondant à votre
    télécommande dans build/lirc-*/remotes (après avoir effectué scripts/unpack
    lirc) et l'ajouterez à packages/lirc/install. Puis, choisissez votre
    périphérique (par défaut, il s'agit de /dev/ttyS0 (COM1)) et le pilote
    lirc et mettez le tout dans un fichier nommé packages/lirc/lircd_$REMOTE. 
    Vous pourrez ensuite choisir l'affectation des touches dans le fichier 
    packages/lirc/lircrc_$REMOTE. Pour chaque affectation, vous aurez à chosir
    un bouton (choisissez leurs noms dans le fichier de définitions de la
    télécommande) et associez lui une action. L'action sera une de celle
    disponible dans MPlayer (vous pouvez trouver une liste dans le fichier html
    build/MPlayer-*/DOCS/documentation.html#commands).


| MODIFICATION
| ~~~~~~~~~~~~

La première chose dont vous aurez àvous soucier concerne le script 
d'initialisation. En fait, ils sont 2. Le premier est dans 
packages/initrd/linuxrc mais vous ne devriez pas avoir besoin de le modifier.
Le second est config/init et c'est dans ce dernier que vous aurez de
probables modifications à effectuer.

Puis, vous pourrez être intéressés par l'ajout de nouveaux paquetages. Un
paquetage n'est implement qu'un ensemble de scripts qui se doivent de suivre
certaines règles. Tous les scripts se doivent d'être placés dans un
répertoire dont le nom coïncide avec celui du programme que vous désirez
ajouter, lui-même dans le répertoire packages.

Voici une liste de scripts que vous aurez à créer :
 - url : simple liste d'URLS où sont disponibles les sources.
 - unpack : que faire après avoir décompresser les sources. Par exemples, vous
            pouvez modifier les fichiers de configuration. Ceci ne concerne pas
            l'application de patchs.
 - need_build : appelé lorsque le paquetage a déjà été compilé, afin de
                s'assurer qu'il n'aura plus besoin d'être recompilé. Il devrait
                supprimer le fichier .stamps/"package name"/build si le
                paquetage nécessite d'être reconstruit.
 - build : l'ensemble des étapes nécessaires pour compiler le programme.
 - install : l'ensemble des étapes nécessaires à l'installation du programme.
             Le préfixe d'installation devrait être $INSTALL.

Lorsqu'un fichier de la liste d'URLS est nommé patch-nom_du_programme-..., il
est automatiquement appliqué aux sources du programme.

Vous devez avoir à l'esprit que les applications qui tournent sous le système
GeeXboX doivent avoir été compilé avec la librairie uClibc.

Enfin, la meilleure manière d'ajouter un paquetage est de s'inspirer de la 
façon dont les actuels sont faits.


| LICENSE
| ~~~~~~~

Tous les programmes utilisés par GeeXboX sont protégés par leurs licenses
respectives. Tous ces logiciels sont libres et, pour la plupart, protégés par
une licence GPL (License Publique Générale)
La GeeXboX elle-même, c'est à dire tous les scripts utilisé et le système
de compilation, est couvert par la licence GNU-GPL.