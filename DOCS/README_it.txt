
         ooooo                     oo     o  o               oo     o
        M"   "Mo                    Mo  oM"  M                Mo  oM"
       M           oM""Mo  oM""Mo    "Mo"    Mo"""o  oM""Mo    "Mo"
       M    """M   MooooMM MooooMM   oMMo    M    "M M    M    oMMo
       "o     oM   M    o  M    o   oM  Mo   M    oM M    M   oM  Mo
        "MoooM"M   "MooM"  "MooM"  M"    "M  M"ooo"  "MooM"  M"    "M



| INFORMAZIONI
| ~~~~~~~~~~~~

GeeXboX è una sorta di software per "divx box". Infatti, è un boot CD
stand-alone che permette di vedere film o ascoltare musica. Supporta molti
formati, come avi, mpeg, divx, ogm, rm, mp3, ogg, dvd, vcd e cdda.
Supporta inoltre qualche telecomando IR e il TV-Out per qualche scheda grafica.
Questo archivio contiene gli script che servono per creare un'immagine iso di
GeeXboX.


| REQUISITI
| ~~~~~~~~~

Per creare una iso di GeeXboX, occorre una delle seguenti configurazioni :
  - un sistema GNU/Linux con mkisofs e mkzftree.
  - un sistema MAC OS X con mkisofs e mkzftree.
  - un sistema windows.

Per installare GeeXboX, occorre :
  - un sistema GNU/Linux con syslinux.

Per compilare GeeXboX, occorrono dei tools classici :
  - un sistema GNU/Linux funzionante.
  - il compilatore C gcc.
  - GNU make
  - il comando patch.
  - l'assembler nasm.
  - bzip2 e gzip.
  - mkfs.ext2 e mkfs.vfat
  - il tool di download wget (non occorre per il pacchetto competo di GeeXboX).
  - mkisofs e mkzftree per creare l'immagine iso.
  - mkzftree per comprimere i file nell'immagine iso.
  - cdrecord (per masterizzare l'immagine).

E anche 500 MB di spazio libero su disco.


| PERSONALIZZAZIONE
| ~~~~~~~~~~~~~~~~~

Quando GeeXboX è installato su disco o generato,
è facile da personalizzare.

E' possibile aggiugnere alcuni codecs proprietari come rv9 o wmv9,
semplicemente aggiugnendoli nella cartella GEEXBOX/codecs. Si possono trovare
questi codecs nel pacchetto:
http://www.geexbox.org/releases/geexbox-extra-codecs-nonfree.tar.gz

E' possibile inoltre modificare molte opzioni. Questo è possibile modificando
semplicemente qualche file di testo.

* Linguaggio :
    E' possibile selezionare il linguaggio di menu preferito seguendo la
    seguente procedura:

    - Utenti GNU/Linux:
    Modificare lo script generator.sh prima di eseguirlo, cambiando la linea
    verso l'inizio del file: "MENU_LANG=en" con il linguaggio preferito.
    Esempio: se sei italiano, cambia la linea in "MENU_LANG=it".

    - Utenti Microsoft Windows:
    Quando viene eseguito, generator.exe permette di selezionare la lingua
    preferita tramite una lista a tendina.

    NB : Questa modifica non ha effetti sul linguaggio del DVD (leggere la
    sezione MPlayer section).

    Per la lista dei linguaggi disponibili sfogliare la directory "language".
    Se la tua lingua non è disponibile, puoi tradurre il menù nella tua lingua.
    Questo comporta la creazione dei file language/menu_LANG.conf e
    language/help_LANG.txt.

* MPlayer :
    In questa sezione è possibile effettuare la maggior parte della
    configurazione e tweaking. Le opzioni sono salvate nel file
    packages/MPlayer/mplayer.conf. E' possibile modificare opzioni come la
    dimensione del font dell'OSD (subfont-text-scale).
    E' possibile inoltre aggiungere molte opzioni come la lingua predefinita
    per il DVD (es: alang=it,en).
    Il posto migliore per trovare tutte le opzioni, per utenti linux, è la
    manpage di MPlayer (man -l build/MPlayer-*/DOCS/mplayer.1).
    Inoltre è consigliato visitare la documentazione di MPlayer
    (in build/MPlayer-*/DOCS o al link http://mplayerhq.hu/DOCS/).
    un altro file che è possibile modificare è packages/MPlayer/menu.conf.
    E' possibile rimuovere gli elementi non utilizzati, oppure è possibile
    tradurli nella propria lingua.
    L'ultimo file utile per la configurazione è packages/MPlayer/build che
    contiene la selezione di opzioni che sono incluse in MPlayer.

* uscita TV :
    L'abilitazione del TV-Out è possibile con l'aiuto di alcune piccole
    applicazioni dedicate alle varie marche di schede video. Attualmente viene
    utilizzato atitvout per le schede ATI, s3switch per le schede S3 e nvtv per
    le schede nVidia cards (e, ove possibile, schede intel i810 e 3dfx).
    La configurazione di questi programmi è effettuata in config/tvout.
    In questo file è possibile scegliere lo standard TV (pal, ntsc...) ed è
    inoltre possibile modificare le opzioni specifiche per nvtv.

    Si può definire l'aspetto (4:3 o 16:9) in questo file alla linea:

    TVOUT_ASPECT="4:3"

    Questo parametro verrà utilizzato sia per il TVOut che per i monitor.
    E' possibile specificare anche l'altezza/larghezza dello schermo e le
    frequenze orizzontale e verticale, in caso di schermi non standard, come
    WideScreens or videoproiettori. Questo può essere fatto modificando il file
    /etc/mplayer/mplayer.conf. I parametri predefiniti sono indicati di seguito
    (decommentare le linee relative alle frequenze se si vogliono utilizzare):

    screenw=800
    screenh=600
    #monitor-hfreq=31.5k-50k
    #monitor-vfreq=50-90

* Lirc :
    E' possibile scegliere uno dei telecomandi supportati modificando il file
    GEEXBOX/etc/remote. Fare attenzione a scegliere anche il corrispondente
    ricevitore ir nello stesso file di configurazione. Se si vuole modificare la
    configurazione dei tasti del proprio telecomando, modificare il file
    GEEXBOX/etc/lirc/lircrc_TELECOMANDO.

* rete :
    E' possibile configuare la rete nel file GEEXBOX/etc/network.
    Si può scegliere l'indirizzo IP utilizzato da GeeXboX (il predefinito è di
    utilizzare DHCP e se non funziona, utilizzare l'ip 192.168.0.54).
    E' possibile inoltre specificare un login e una password che sarnno usati
    per connettersi con le condivisioni windows (predefinito: anonimo).
    E' inoltre possibile dichiarare i mount NFS in GEEXBOX/etc/nfs.

* wifi :
    Come impostazioni predefinite, GeeXboX cerca di rilevare automaticamente
    le impostazioni di rete. Se sulla macchina sono installate sia una scheda
    di rete (NIC) e una scheda WiFi, solo quest'ultima sarà impostata.
    Per impostare correttamente la rete, può essere necessario modificare il
    file /etc/network.

    In questo file ci sono 4 linee riguardanti le schede wireless :
    * PHY_TYPE="auto"      # Tipo fisico di rete (auto|ethernet|wifi)
    * WIFI_MODE="managed"  # metodo di utilizzo Wifi (managed|ad-hoc)
    * WIFI_WEP=""          # chiave WEP Wifi
    * WIFI_ESSID="any"     # SSID Wifi

    Con queste linee è possibile configurare la gran parte delle impostazioni.
    E' possibile lasciare la rilevazione automatica oppure forzare l'uso della
    scheda ethernet o dell'adattatore WiFi.
    Allo stesso modo, è possibile scegliere tra il metodo managed e ad-hoc e
    definire la propria chiave WEP e SSID.

* gateway :
    GeeXboX supporta l'accesso a Internet. Nel caso in cui tu abbia una
    connessione Internet, è possibile condividerla con il proprio multimedia
    box, utilizzando un router o un gateway. Per fare ciò, è necessario
    definire l'indirizzo IP del gateway nel file /etc/network.

    * GATEWAY=""     # Gateway IP ("" per DHCP o nessuna connesione internet)

* configuraqzione tv :
    GeeXboX supporta le entrate e i sintonizzatori TV. Il sistema cercherà di
    rilevare automaticamente la scheda e il sintonizzatore. E' possibile
    forzare le impostazioni e saltare i tentativi di rilevamento. Modificare il
    file /etc/tvcard come descritto :

 #TV CARD/TUNER Model (AUTO for autodetection or look at the following urls)
 #http://www.linuxhq.com/kernel/v2.6/2/Documentation/video4linux/CARDLIST.bttv
 #http://www.linuxhq.com/kernel/v2.6/2/Documentation/video4linux/CARDLIST.tuner

    TV_CARD=AUTO
    TV_TUNER=AUTO
    TVIN_STANDARD=pal

    Lasciare il parametro AUTO se si vuole mantenere il rilevazione automatica,
    oppure sostiruirlo con il numero del tipo di scheda e sintonizzatore in
    possesso, in base alle informazioni dei precedenti URL. Prestare molta
    attenzione: per forzare il tipo si scheda e sintonizzatore occorre
    conoscere l' ESATTO MODELLO del proprio hardware.

    Successivamente, dovrebbe essere possibile utilizzare l'input TV (Composito
    e S-VHS) della propria scheda TV. Allo stesso modo, sarà possibile usare il
    sintonizzatore per guardare la TV. Per fare ciò, è necessario specificare
    la regione di appartenenza e la frequenza dei canali TV che si vogliono
    visualizzare. Anche questa volta basta modificare il file /etc/tvcard :

    # TV Channels
    # Syntax : CHAN="Channel Frequency:Channel Title"
    # Example :
    # CHAN="29:France 2"
    # CHAN="K08:Canal +"
    # TV Channels List
    # Available : france, europe-east, europe-west, us-bcast, us-cable
    CHANLIST=france

    Prestare attenzione nell'usare la stessa sintassi descritta sopra, affinchè
    i canali TV siano presenti nel menu principale.

* configurazione audio :
    GeeXboX supporta sia l'ourput audio analogico che digitale attraverso i
    connettori jack o RCA SPDIF. L'uscia predefinita è quella analogica.
    E' possibile cambiare tale impostaizone modificando il file /etc/audio :

    # Output using SPDIF (yes/no), otherwise ANALOG output
    SPDIF=no

    Ricorda che è necessario impostare l'uscita su SPDIF se si vuole collegare
    la scheda audio ad un amplificatore esterno per decodificare gli stream
    AC3/DTS (utilizzando il passthrough mode).

* Scheda DXR3/Hollywood+ :
    Gli utenti con questo tipo di scheda di decompressione hardware NON hanno
    bisogno di una scheda video o audio per utilizzare GeeXboX. Per contro, è
    possibile utilizzare solo l'uscita TV con le schede DXR3 (niente monitor).
    E' necessario specificare il corretto stadard (PAL/NTSC) nel file /etc/tvout
    e il tipo di uscita audio da utilizzare (Analogica o SPDIF) in /etc/audio.

| CREAZIONE
| ~~~~~~~~~

Prima di tutto, leggere la sezione di personalizzaizone qui sopra.

Una vota personalizzata, è possibile crare la iso sotto Linux, lanciando
  ./generator.sh
o sotto windows, lanciando
  generator.exe


| INSTALLAZIONE
| ~~~~~~~~~~~~~

Inizialmente è necessario creare una partizione FAT16 con circa 16 MB di spazio
libero.

E' possibile installare GeeXboX sotto linux lanciando
  ./installator.sh
E rispondendo alle domande poste dal programma. Ponete molta attenzione durante
questi passaggi.
Leggere due volte ogni domanda e uscire dall'installazione se non si capisce
una domanda.

| PXE BOOT
| ~~~~~~~~

Sì, GeeXboX può avviarsi dalla rete da una stazione denza disco!
Per raggiungere lo scopo è necessario:
 - un server DHCP
 - un server TFTP
 - un server NFS
 - una stazione che può utilizzare PXE :-)
Configurare il server dhcp affinchè invii le informazioni di boot PXE. Ecco un
esempio con isc dhcp :

allow booting;
allow bootp;

subnet 192.168.0.0 netmask 255.255.255.0 {
  range 192.168.0.128 192.168.0.192;
  option subnet-mask 255.255.255.0;
  option broadcast-address 192.168.0.255;
  next-server 192.168.0.1;
  filename "/tftpboot/GEEXBOX/boot/pxelinux.0";
}

L'opzione next-server è l'indirizzo del server TFTP.
Configurare il server TFTP (ad esempio atftpd) affichè pubblichi la cartella
/tftpboot e copiare l'intera direcroty GEEXBOX in questa cartella. Ad esempio,
si può copiare il contenuto di un CD GeeXboX da un sistema linux CON "CDROM
TRANSPARENT DECOMPRESSION" ABILITATA !! (per verificare questo, controllare il
file sbin/init nella directory GeeXboX e verificare che non contenga sporcizia)
Se è stato compilato GeeXboX dai sorgenti, è possibile anche generare la
directory GEEXBOX con make pxe.

Modificare il file /tftpboot/GEEXBOX/boot/pxelinux.cfg/default
per impostare nfsroot al giusto percorso NFS della directory GEEXBOX.

Infine impostare NFS per esportare la directory GEEXBOX tramite /etc/exports
contenente una riga simile alla seguente :

/tftpboot/GEEXBOX (ro)

e /etc/hosts.allow contenente qualcosa come:

ALL: ALL

Dovrebbe funzionare. Avviare la stazione PXE e vedere quello che accade.


| COMPILAZIONE
| ~~~~~~~~~~~~

Prima di tutto è consigliabile controllare la sezione di configurazione subito
sotto (almeno la parte generale).

Dopo di ciò, compilare l'immagine iso con :
  make
Oppure è possibile masterizzare direttamente l'immagine con :
  make burn

Quando la compilazione è completata, è possibile liberare spazio su disco
pulendo la directory di compilazione :
  make clean
oppure facendo una pulizia completa, eliminando anche i sorgenti scaricati :
  make distclean

Ci sono anche comandi più avanzati, se si vuole fare un po' di hack di GeeXboX:
  scripts/get package        # scarica il pacchetto
  scripts/unpack package     # scompatta e prepara il pacchetto
  scripts/build package      # compila il pacchettobuild
  scripts/install package    # installa il pacchetto con il prefisso $INSTALL
  scripts/clean package      # pulisce la directory dei sorgenti del pacchetto

Se avete fatto una versione modificata di GeeXboX, è possibile compilare
facilmente un piccolo tar.bz2 con :
  make dist
oppure un tar completo (contenente tutti i sorgenti) con :
  make fulldist
oppure un geexbox generator con :
  make generator
oppure un geexbox installator con :
  make installator
oppure una directory per pxe con :
  make pxe


| CONFIGURAZIONE
| ~~~~~~~~~~~~~~

* Opzioni generali :
    Questa è la prima cosa alla quale prestare attenzione prima di provare a
    compilare GeeXboX. Tali opzioni sono memorizzate nel file config/options,
    e dovrebbero spiegarsi da sole. Si può scegliere la famiglia di CPU, il
    tema, e se si vogliono font truetype o no. E' possibile modificare la
    configurazione del masterizzatore cd per poter masterizzare direttamente
    l'immagine iso.

* Linux :
    Questa è una configurazione classica di linux (packages/linux/linux.conf).
    E' possibile modificarla a mano, oppure usare anche scripts/unpack linux
    e make menuconfig -C build/linux-* (o utilizzare il metodo preferito al
    posto di menuconfig). Infine fare un backup di build/linux-*/.config in
    packages/linux/linux.conf.

* Lirc :
    Lirc permette di controllare GeeXboX usando un telecomando. Inizialmente
    occorre scegliere il file che descrive il proprio telecomando in
    build/lirc-*/remotes (dopo aver eseguito scripts/unpack lirc) e aggiungerlo
    in packages/lirc/install. Fatto questo, occorre scegliere la periferica
    (la predefinita è /dev/ttyS0 (COM1)) e il driver lirc, inserendoli in un
    file chiamato packages/lirc/lircd_$TELECOMANDO. E' possibile definire i
    tasti nel file packages/lirc/lircrc_$TELECOMANDO. Per ogni definizione
    occorre selezionare un tasto (utilizzando i nomi nel file di descrizione
    del telecomando) e associare un'azione a tale tasto. L'azione è una delle
    azioni di MPlayer's (si può trovare una lista nel file html
    build/MPlayer-*/DOCS/documentation.html#commands).


| HACKING
| ~~~~~~~

La prima cosa da controllare sono gli script di inizializzazione.
Infatti, ci sono due script di installazione. Il primo è in
packages/initrd/linuxrc, ma non ci dovrebbe essere bisogno di modificarlo.
Il secondo è in config/init ed è quello in cui è possibile aggiongere qualche
personalizzazione.

La cosa successiva che può interessare è la creazione di un nuovo "pacchetto".
Un pacchetto è un insieme di script che seguono alcune regole. Tutti gli script
devono essere memorizzati in una directory chiamata come il programma che si
vuole "impacchettare", che sta nella directory packages.
Di seguito la lista degli script da cerare :
 - url : una lista di url dove trovare i sorgenti del programma.
 - unpack : cosa fare dopo aver scompattato i sorgenti. Ad esempio, è possibile
            modificare i file di configurazione. Questo non include
            l'applicazione delle patch.
 - need_build : chiamato quando il pacchetto è già compilato, per essere sicuri
                che non necessita di ricompilazione. Dovrebbe eliminare il file
                .stamps/"nome pacchetto"/build se il pacchetto necessita di
                ricompilazione.
 - build : tutti i passaggi necessari per compilare il programma.
 - install : tutti i passaggi necessari per installare il programma. Il
             prefisso di installazione deve essere $INSTALL.

Quando un file degli url è chiamato patch-nome_programma-... è applicato
automaticamenteit ai sorgenti scompattati del programma.

Ricordare inoltre che il software utilizzato in GeeXboX deve essere compilato
con il gcc wrapper uClibc.

Se si è ancora incerti, la via migliore per creare un pacchetto è guardare come
sono fatti gli altri pacchetti.


| LICENZA
| ~~~~~~~

Tutti i programmi utilizzati da GeeXboX sono protetti dalle rispettive licenze.
Sono tutti software liberi e molti di questi sono regolati dalla GNU General
Public License.
GeeXboX per sè, intesa come tutti gli script utilizzati nel processo di
compilazione, sono regolati dalla GNU General Public License.
