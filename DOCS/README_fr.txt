
         ooooo                     oo     o  o               oo     o
        M"   "Mo                    Mo  oM"  M                Mo  oM"
       M           oM""Mo  oM""Mo    "Mo"    Mo"""o  oM""Mo    "Mo"
       M    """M   MooooMM MooooMM   oMMo    M    "M M    M    oMMo
       "o     oM   M    o  M    o   oM  Mo   M    oM M    M   oM  Mo
        "MoooM"M   "MooM"  "MooM"  M"    "M  M"ooo"  "MooM"  M"    "M




| AVANT PROPOS
| ~~~~~~~~~~~~

La GeeXboX est une distribution Linux de type Media Center. Il s'agit d'un CD
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

Pour construire votre propre GeeXboX, vous nécessiterez les outils
classiques :
  - un système GNU/Linux opérationnel.
  - Le compilateur C GCC.
  - GNU make
  - La commande patch.
  - L'assembleur nasm.
  - bzip2 et gzip.
  - L'outil de téléchargement wget (non nécessaire pour le paquetage
    GeeXboX complet).
  - mkisofs et mkzftree pour construire l'image ISO.
  - mkzftree pour compresser les fichiers de l'image ISO.
  - cdrecord (pour graver l'image).

Et environ 2 Go d'espace disque disponible.


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
http://www.geexbox.org/releases/geexbox-extra-codecs-nonfree.tar.gz

Vous pouvez modifier d'autres options en éditant simplement des fichiers
textes.

* Chargement des firmwares :

    La GeeXboX supporte de nombreux pilotes de périphériques mais
    malheureusement, certains d'entre eux requierent un firmware binaire
    additionnel propriétaire (i.e. non-libre et donc non-inclus dans la
    GeeXboX). Le firmware est un fichier binaire qui est envoyé au
    périphérique (PCI ou USB) lorsque le pilote se charge. Pour chaque pilote,
    le firmware se doit d'être un fichier spécifique avec un nom
    pré-déterminé.

    Si vous disposez de matériel à problème qui peut nécessiter la présence
    d'un firmware additionnel (dans la plupart des cas, il s'agit de cartes
    WiFi ou DVB), vous devrez chercher sur Internet une page concernant le
    support de périphérique sous Linux et l'utilisation du firmware.
    La plupart du temps, vous tomberez sur un lien de téléchargement direct.
    Des fois, il vous sera nécessaire d'extraire ce firmware depuis une
    archive de pilotes pour Windows qui ont été fourni par le fabricant du
    périphérique.

    Une fois que vous disposerez du fichier de firmware pour votre carte,
    déposez le simplement dans le répertoire /firmwares. Vous pouvez y stocker
    autant de firmwares que vous voudrez, la GeeXboX essaiera d'elle même de
    les charger lorsqu'un pilote en aura besoin. La seule chose dont vous ayez
    à vous soucier et de disposer des bons fichiers de firmwares avec les bons
    noms avant de regénérer une nouvelle ISO.

    Veuillez noter que vous pouvez également utiliser le générateur d'ISO de
    la GeeXboX qui vous permettra de récupérer les firmwares pour vous.
    Vous pouvez l'utiliser pour sélectionner tous les fichiers de firmware que
    vous souhaiter et le générateur les téléchargera et ajoutera pour vous.
    Assurez vous juste de disposer d'une connexion à Internet lors de
    l'utilisation du générateur.

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

    Activer la sortie TV se fait au moyen de nombreux petits utilitaires
    dédiés chacun à une marque carte graphique. Nous utilisons actuellement
    atitvout pour les cartes ATI, s3switch pour les cartes S3 et nvtv pour les
    cartes nVidia (ce qui peut aussi marcher abec les cartes i810 et 3dfx). La
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

* WiFi :

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

* Passerelle :

    La GeeXboX supporte l'accès à Internet. Définissez simplement l'adresse IP
    de la passerelle dans le fichier /etc/network

    GATEWAY=""     # Gateway IP ("" for DHCP or no internet connection)

* Configuration TV :

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
    # Syntax : CHAN="Channel Frequency:Channel Title"
    # Example :
    # CHAN="29:France 2"
    # CHAN="K08:Canal +"
    # TV Channels List
    # Available : france, europe-east, europe-west, us-bcast, us-cable
    CHANLIST=france

    Faites attention lors de l'édition des canaux TV et veillez à utiliser
    la même syntaxe que décrit ci-dessus et les canaux TV devraient
    apparaître dans le menu principal.

* Configuration Audio :

    La GeeXboX supporte à la fois la restitution audio via la sortie
    analogique ou numérique, en utilisant les connecteurs classiques JACK
    ou RCA SPDIF.
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
    consommer une partie du temps processeur afin de rendre l'image plus
    belle. Via l'utilisation des filtres internes à MPlayer, la GeeXboX vous
    permet de minimiser les effets de blocs horizontaux et verticaux, les
    effets d'anneaux de dégradés et de corriger automatiquement la luminosité
    de votre film. Par défaut, le Post-Processing est désactivé, pour éviter
    de saccader sur de petites configurations matérielles. Il vous est
    possible de l'activer très simplement en éditant le fichier
    /etc/mplayer/mplayer.conf :

    # Set Post Processing (h deblock, v deblock, dering, auto luminance)
    # Consumes CPU power, disabled for low configs, uncomment to enable it.
    #vf=pp=hb:a/vb:a/dr:a/al:a

* DXR3/Hollywood+ cards :

    Les utilisateurs de ce type de cartes de décompression n'ont pas besoin
    d'avoir une carte vidéo ou une carte son dans leur ordinateur. Parmi les
    inconvénients, on notera néanmoins que seule la sortie TV peut etre
    utilisée avec ce type de carte (pas d'affichage sur moniteur).
    Vous pouvez etre amené à définir la norme d'image souhaitée (PAL/NTSC)
    via le fichier /etc/tvout ainsi que le type de sortie audio à utiliser
    (Analogique ou SPDIF) via le fichier /etc/audio.

* Configuration de la radio :

    Si vôtre carte TV dispose d'un tuner radio FM intégré, la GeeXboX vous
    permet d'écouter la radio. Pour cela, il vous faudra modifier le fichier
    GEEXBOX/etc/radio pour mettre l'option RADIO à yes.

    Le fichier GEEXBOX/etc/radio contient aussi la définition des stations.
    Des exemples sont présents dans le fichier. Il suffira de les dé-commenter
    et de les adapter à vos stations favorites.

    Le programme d'écoute de la radio ne faisant pas partie de MPlayer, il
    y a des contrôle de volume séparés pour la radio. Notez que le volume
    contrôlé est le volume principale. Il pourra donc être nécessaire de le
    réajuster avant de lancer la TV, une vidéo ou de la musique.

* Partage de fichiers via UPnP :

    UPnP est l'acronyme de Universal Plug & Play et peut se décrire comme un
    protocole réseau permettant l'auto-découverte de périphériques ainsi que
    des services qui leur sont associés au sein de votre réseau domestique.
    La norme UPnP A/V (pour Audio/Video) définit un certains nombre de
    profiles pour des périphériques permettant le partage et la lecture de
    fichiers multimédias au sein d'un réseau. Le profile UPnP Media Server
    permet regroupe tous les périphériques capables de partager des fichiers
    à des périphériques de type UPnP Media Player ou UPnP Media Renderer,
    qui sont en mesure de les restituer.

    La GeeXboX embarque un logiciel de contrôle UPnP (UPnP Control Point) qui
    permet l'auto-découverte de tous les périphériques de type Media Server de
    votre réseau local et qui s'occupe de monter leur contenu en toute
    transparence dans un point de montage dédié (/mnt/UPnP).

    De cette manière, il vous suffit de disposer d'un PC ou autre périphérique
    possédant un logiciel compatible avec le profile UPnP Media Server pour
    que son contenu soit accessible depuis la GeeXboX. Pour plus d'infos sur
    l'UPnP ainsi que l'utilsiation de quelques serveurs,
    veuillez vous référer à la page suivante :
    http://www.geexbox.org/wiki/index.php/Accessing_to_UPnP_Contents

* Streaming réseau :

    GeeXboX permet de rajouter les listes de streams SHOUTcast Radio et TV
    tout autant que vos propres listes de lecture. (locales et distantes.)

    L'activation de SHOUTcast se déclare dans le fichier de configuration
    "GEEXBOX/etc/network". SHOUTcast TV étant succeptible de diffuser des
    streams nécessitant une inscription particulière ainsi que des streams
    à caractère pornographique, une liste noire et une liste blanche
    peuvent être complétées afin de filtrer le contenu selon vos critères
    (par défaut, les mots clefs définis pour la liste noire sont :
    "adult xxx porn ESS SWCTV SWPTV Subscription"). La liste noire et la
    liste blanche sont insensibles à la casse.

    Vous pouvez créer vos listes de lecture locales et distantes dans le
    fichier de configuration "GEEXBOX/etc/netstream" (Des exemples sont
    contenus dans ce fichier). Les listes de lecture distantes concernent
    uniquement les fichiers M3U Etendus.

    Note aux utilisateurs (Français uniquement) :
    Il est possible de receptionner un flux "FreeboxTV" grâce à ce fichier.
    Pour y parvenir, veuillez suivre la procédure suivante :
    - dé-commentez l'entrée EXTM3U ad-hoc.
    - utilisez une version de GeeXboX compilée avec le support de la
      librairie "LIVE555".
    (ces deux conditions étant nécessaires à la visualisation du flux) 

* Cartes DVB :

    La GeeXboX supporte un grand nombre de cartes DVB (Terrestre i.e. TNT,
    Cable, ATSC et Satellite) et ce, aussi bien en PCI qu'en USB. Veuillez
    cependant noter qu'il n'est pour l'instant possible d'utiliser qu'un
    unique adaptateur DVB à la fois.

    Certains péripéhriques DVB (particulièrement ceux en USB) peuvent
    nécessiter un firmware propriétaire additionnel pour fonctionner
    correctement. Il vous sera peut être utile de jeter un oeil au Wiki DVB de
    LinuxTV (http://linuxtv.org/wiki/index.php/Main_Page) pour vous assurer
    que votre carte nécessite un tel firmware ou non. Dans ce cas, le
    générateur d'ISO pourra le télécharger pour vous.

    Certains firmwares peuvent être récupérés directement depuis le site Web
    de LinuxTV (http://linuxtv.org/download/firmware/), d'autres peuvent être
    inclus dans l'archive de drivers Windows du constructeur de la carte.
    Veuillez vous référer à la documentation sur le chargement des firmwares
    dans la GeeXboX pour de plus amples informations.

    Pour chaque carte, la liste des chaînes disponibles est disponible au sein
    du menu de MPlayer. Il n'est cependant pas possible d'auto-découvrir ces
    chaînes automatiquement.

    De ce fait, la déclaration de chaînes accessibles par DVB dans la GeeXboX
    peut être fait de différentes manières :

    - utiliser une liste de chaîne existante : MPlayer a besoin d'un fichier
    channels.conf valide et fonctionnel pour faire fonctionner la DVB. Ce
    fichier peut être généré à l'aide d'utilitaire fournis par le paquetage
    dvb-apps. Utilisez simplement une distribution Linux classique où votre
    carte DVB a déjà été configurée, téléchargez l'archive linuxtv-dvb-apps
    depuis le site http://www.linuxtv.org/download/dvb/, compilez le tout et
    créez le fichier de configuration via l'utilitaire "scan", dans le format
    "zap" (qui est le format par défaut).

    Par exemple, pour une carte DVB-S (Satellite) utilisant le fournisseur
    d'accès Astra-19.2E :

      wget http://www.linuxtv.org/download/dvb/linuxtv-dvb-apps-1.1.0.tar.bz2
      tar jxvf linuxtv-dvb-apps-1.1.0.tar.bz2
      cd linuxtv-dvb-apps-1.1.0/util/scan
      make
      ./scan -x 0 dvb-s/Astra-19.2E > channels.conf

    Le drapeau "-x 0" indique que le scan ne tentera pas de se connecter aux
    chaînes cryptées (généralement payantes). Veuillez tout le temps procéder
    de la sorte, MPlayer ne gérant de toutes façons pas les chaînes
    DVB cryptées.

    Selon votre type de carte DVB, choisissez un fichier de transponders
    (fournisseur) depuis les répertoires "dvb-s", "dvb-c", "dvb-t" ou "atsc".

    Ensuite, copiez simplement le fichier channels.conf que vous venez de
    générer au sein du répertoire /etc/mplayer de l'arborescence GeeXboX
    du générateur and recompilez une ISO.

    - utiliser une liste de transpondeur existante : cette procédure est
    relativement similaire à la précédente à la différence qu'il s'agit cette
    fois de la GeeXboX qui va s'occuper de scanner les chaînes pour vous et
    ce, à chaque démarrage. Vous n'aurez ainsi plus besoin d'une autre
    distribution Linux pour générer le fichier channels.conf.

    Pour ce faire, rendez-vous simplement sur :
      http://linuxtv.org/cgi-bin/viewcvs.cgi/dvb-apps/util/scan/

    Selon votre type de carte DVB (S/T/C/ATSC), sélectionnez le bon répertoire
    et choisissez le fichier de liste de fréquences de transpondeurs qui
    correspond à vos besoins ou votre localisation géographique. Renommez
    ensuite simplement ce fichier en dvb.conf et copiez le dans le répertoire
    /etc. Puis, reconstruisez une ISO via le générateur.

    Au démarrage, si la GeeXboX détecte un fichier de transpondeur valide dans
    le fichier /etc/dvb.conf, elle l'utilisera pour scanner les chaînes DVB
    disponibles et générera le fichier /etc/mplayer/channels.conf d'elle-même.

    ATTENTION : Le scan de chaînes peut être relativement lent selon le type
    d'émetteur et le nombre de chaînes à découvrir. Ce processus est de plus
    effectué à chaque démarrage de la GeeXboX si vous l'utilisez en tant que
    LiveCD. Il est alors hautement recommandé qu'une fois le scan effectué,
    vous copiez manuellement le fichier /etc/mplayer/channels.conf qui aura
    été généré quelque part afin de pouvoir le ré-utiliser dans le générateur
    d'ISO en suivant la première méthode, ou de faire une installation sur
    disque dur.

    - utiliser l'installator : il s'agit probablement là de la méthode la plus
    simple mais recquiert une installation sur disque de la GeeXboX. Au cours
    du processus d'installation, si un périphérique compatible DVB est reconnu
    sur votre système, le script d'installation vous demandera de lui-même si
    vous souhaitez rechercher les chaînes disponibles pour votre carte DVB.

    L'installator contient la liste complète des fréquences des transpondeurs
    du site LinuxTV. De ce fait, vous n'aurez qu'à sélectionner votre type de
    carte ainsi que le transpondeur à utiliser et la GeeXboX s'occupera de
    scanner letout et de générer le fichier /etc/mplayer/channels.conf.

* Menus de Navigation DVD :

    La GeeXboX propose 2 méthodes de lecture des DVD :
     - Lecture directe du film (defaut).
     - Lecture avec support des menus de Navigation DVD (expérimental).

    La première permet une lecture quasi-assurée de la plupart des DVDs. Une
    fois inséré, le disque est automatiquement détecté et MPlayer va essayer
    de lire le chapitre qu'il jugera correspondre au film. Le principal
    avantage réside dans le fait que toutes les scènes de droit d'auteur,
    publicités ou menus seront sautées pour permettre une lecture instantannée
    du film. Néanmoins, dans certaines situations (comme les DVD avec des
    menus très sophistiqués ou encore les DVD de séries TV, contenant de
    multiples épisodes et donc chapitres), cette méthode n'est pas précise
    et vous pourriez ne pas être en mesure de lire votre film correctement.

    La seconde, bien qu'encore expérimentale avec MPlayer (mais qui a
    néanmoins de très fortes chances de fonctionner), apporte le support des
    menus de navigation DVD, vous permettant donc de le lire comme vous le
    feriez avec n'importe quel lecteur DVD du commerce, avec les avantages et
    inconvénients qui en découlent.

    Il vous est bien entendu possible de basculer d'un mode de lecture DVD
    à un autre au moyen du menu d'options de la GeeXboX, selon le DVD que vous
    êtes amener à visionner.

    Il vous est également possible de spécifier la méthode de lecture par
    défaut au moyen du générateur d'ISO de la GeeXboX.


| INSTALLATION
| ~~~~~~~~~~~~

Le plus simple est de démarrer la GeeXboX depuis le CD est de
taper "install" au prompt de démarrage.

Répondez ensuite à toutes les questions. Lisez les questions avec attention
et stoppez l'installation si vous ne comprenez pas ce que vous faites.


| BOOT PXE
| ~~~~~~~~

Oui, la GeeXboX est capable de booter depuis le réseau sur une machine
sans disque ! Pour obtenir cela il vous faudra :
 - un serveur DHCP
 - un serveur TFTP
 - un serveur NFS
 - une machine supportant le PXE :-)

* Depuis un poste GNU/Linux :
  -------------------------

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
de l'arborescence GeeXboX ne contient pas de caractères totalement
incohérents). Si vous avez compilé la GeeXboX vous même a partir des sources,
il est aussi possible de générer l'arborescence GEEXBOX avec make pxe.

Ensuite il faut éditer le fichier /tftpboot/GEEXBOX/boot/pxelinux.cfg/default
pour faire correspondre le nfsroot au bon chemin NFS vers l'arborescence
GeeXboX.

Enfin il reste a configurer NFS pour qu'il exporte l'arborescence GEEXBOX
avec un fichier /etc/exports ressemblant à ceci :

/tftpboot/GEEXBOX (ro)

et un /etc/hosts.allow ressemblant à :

ALL: ALL

Ca devrait être bon.
Reste a booter la machine PXE et a voir ce qu'il se passe.

* Depuis un poste Microsoft Windows :
  ---------------------------------

Pour démarrer en mode PXE depuis un système Microsoft Windows, il vous faudra
les logiciels suivants :

* Un serveur TFTP et un serveur DHCP ("tftpd32" remplira cette tâche,
  il est disponible à l'adresse http://tftpd32.jounin.net/).
* Un serveur NFS (comme par exemple "Allegro NFS server", disponible à
  l'adresse http://opensource.franz.com/nfs/).
* Une machine supportant le boot en mode PXE.

Téléchargez et décompressez (aucune installation n'est requise) le dossier
tftpd32 quelque part sur votre disque, par exemple C:\tftpd32

Copiez y une arborescence GEEXBOX complète : C:\tftpd32\GEEXBOX

Lancez tftpd32 :
- Sélectionnez le dossier C:\tftpd32 pour "current directory".
- choisissez l'interface (carte réseau) à utiliser en haut dans
  "server interface". Ici dans l'exemple une carte avec l'IP 192.168.0.1
- Cliquez sur "setting" et vérifiez que "DHCP server" soit bien cochée.
- Dans l'onglet "DHCP server", remplissez les champs, en suivant cet exemple :
  (se référer à une documentation plus approfondie sur le fonctionnement
  d'un serveur DHCP pour plus de détails)
	* IP starting pool : 192.168.0.10
	* Size of pool  : 10
	* Boot file : ./GEEXBOX/boot/pxelinux.0
	* WINS/DNS server : 192.168.0.254
	* Default router : 192.168.0.254
	* Mask : 255.255.255.0
	* Domain name : mydomain.net
- Faites "save" pour appliquer les modifications.

La première partie est terminée, normalement en démarrant la machine cliente
(qui doit lancer la GeeXboX), on devrait apercevoir le chargement jusqu'à
l'affichage du logo. Le système se bloquera alors, car le serveur NFS
n'a pas encore été configuré.

Installez "Allegro NFS Server" et configurez le comme suit :

- Onglet Exports :
	* faites un "new name" : et nommez le "/tftpboot/GEEXBOX"
	* dans "path", juste en dessous : selectionnez le
          répertoire "C:\tftp32\GEEXBOX"
	* dans "allowed host list", selectionnez "all".
	* "Read write" , et "read only user list", selectionnez "root"
          et "everyone" (pour avoir un log, il suffit de tout cocher
          dans le dernier onglet).

Faites "Appliquer".

N'oubliez pas de modifier le fichier
C:\tftp32\GEEXBOX\boot\pxelinux.cfg\default et d'y changer l'adresse IP
"192.168.0.2" en "192.168.0.1" (ou toute adresse que vous aurez choisie).

Il suffit maintenant de démarrer la machine cliente pour amorçer la GeeXboX
par le réseau.


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
ou en effectant un nettoyage complet, éliminant même les
sources téléchargées :

  make distclean

Il existe également des commandes plus avancées si vous désirez effectuer
des modifications en profondeur au niveau de la GeeXboX :
  scripts/get package          # télécharge le paquetage
  scripts/unpack package       # prépare le paquetage
  scripts/build package        # compile le paquetage
  scripts/install package      # installe le paquetage dans $INSTALL
  scripts/clean package        # nettoie l'arborescence du paquetage
  scripts/clean --full package # nettoie les sources du paquetage

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
    télécommande dans build/lirc-*/remotes (après avoir effectué
    scripts/unpack lirc) et l'ajouterez à packages/lirc/install.

    Puis, choisissez votre périphérique (par défaut, il s'agit de /dev/ttyS0
    (COM1)) et le pilote lirc et mettez le tout dans un fichier nommé
    packages/lirc/lircd_$REMOTE.

    Vous pourrez ensuite choisir l'affectation des touches dans le fichier
    packages/lirc/lircrc_$REMOTE. Pour chaque affectation, vous aurez à chosir
    un bouton (choisissez leurs noms dans le fichier de définitions de la
    télécommande) et associez lui une action. L'action sera une de celle
    disponible dans MPlayer (vous pouvez trouver une liste dans le fichier
    html build/MPlayer-*/DOCS/documentation.html#commands).


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
            pouvez modifier les fichiers de configuration. Ceci ne concerne
            pas l'application de patchs.
 - need_build : appelé lorsque le paquetage a déjà été compilé, afin de
                s'assurer qu'il n'aura plus besoin d'être recompilé.
                Il devrait supprimer le fichier .stamps/"package name"/build
                si le paquetage nécessite d'être reconstruit.
 - build : l'ensemble des étapes nécessaires pour compiler le programme.
 - install : l'ensemble des étapes nécessaires à l'installation du programme.
             Le préfixe d'installation devrait être $INSTALL.

De plus, le répertoire décrivant un paquetage peut contenir de nombreux
sous-répertoires additionnels :
 - config : c'est là que sont situés tous les fichiers de configuration. Ces
            derniers peuvent être, soit utilisés par le script build pour
            compiler le paquetage, soit par le script install pour être copiés
            dans le répertoire /etc de destination.
 - scripts : ce sous-répertoire peut contenir des scripts d'initialisation lié
             au paquetage courant et qui seront installés par le script
             install
 - patches : ce sous-répertoire peut contenir des patchs destinés à être
             appliqués aux sources du paquetage, au moment du script unpack.
 - sources : si des fichiers sont présents dans ce sous-répertoire, ils seront
             automatiquement copiés dans l'arbre de compilation du paquetage.
 - init.d : contient les scripts d'initialisation qui seront éxécutés au
            chargement du système.

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
