
         ooooo                     oo     o  o               oo     o
        M"   "Mo                    Mo  oM"  M                Mo  oM"
       M           oM""Mo  oM""Mo    "Mo"    Mo"""o  oM""Mo    "Mo"
       M    """M   MooooMM MooooMM   oMMo    M    "M M    M    oMMo
       "o     oM   M    o  M    o   oM  Mo   M    oM M    M   oM  Mo
        "MoooM"M   "MooM"  "MooM"  M"    "M  M"ooo"  "MooM"  M"    "M



| O produktu
| ~~~~~
GeeXboX je druh "divx box" softwaru. Jedná se o bootovací CD umoòující
sledování filmù nebo poslouchání hudby. Podporuje celou øadu formátù jako
avi, mpeg, divx, ogm, rm, mp3, ogg, dvd, vcd , cdda. Obsahuje také podporu
IR dálkovıch ovladaèù a TV vıstup na urèitıch typech grafickıch karet.
Tato distribuce obsahuje nástroje potøebné k  vytvoøení vlastního GeexboX iso souboru.



GeeXboX is a kind of "divx box" software. In fact, it is a stand-alone boot
CD which allows you to watch movies or listen to music. It supports many
formats, such as avi, mpeg, divx, ogm, rm, mp3, ogg, dvd, vcd and cdda.
It also supports some IR remote controllers and TV-Out for some graphic cards.
This archive contain the needed scripts to rebuild an iso image of the GeeXboX.


| Poadavky na systém
| ~~~~~~~~~~~~
Pro vytvoøení GeeXboX iso souboru potøebujete jedno z následujících:
  - GNU/Linux systém s  mkisofs a mkzftree.
  - MAC OS X systém s  mkisofs a mkzftree.
  - MS Windows systém.

K instalaci GeeXboX potøebujete:
  - GNU/Linux systém s syslinux.

Pro vytvoøení GeeXboXu jsou zapotøebí následující standardní nástroje:
  - funkèní GNU/Linux systém.
  - gcc C compiler.
  - GNU make
  - patch command.
  - nasm assembler.
  - bzip2 a gzip.
  - mkfs.ext2 a mkfs.vfat
  - wget download tool (není zapotøebí pokud máte plnou verzi GeeXboX balíèku).
  - mkisofs a mkzftree pro vytvoøení iso souboru.
  - mkzftree pro komprimaci souborù v  iso souboru.
  - cdrecord (pro vypálení iso souboru).

A také pøiblinì 500 MB volného diskového prosoru.


| Vlastní nastavení
| ~~~~~~~~~~~~~~~

Pokud je GeeXboX nainstalován na pevnı disk nebo generován
je velice jednoduché vytvoøit osobní nastavení.


Mùete pøidat další kodeky jako napøíklad rv9 nebo wmv9 pouhım nakopírováním
do adresáøe GEEXBOX/codecs. Tyto kodeky mùete nalézt v balíècích na
http://www.geexbox.org/releases/geexbox-extra-codecs-nonfree.tar.gz

Dále upravit velké mnoství nastavení editací textovıch konfiguraèních  souborù.


* Jazyky:
    Mùete zvolit preferovanı jazyk nabídky následujícími zpùsoby:


    - uivatelé GNU/Linux:
    Pøed spuštìním editujte øádek "MENU_LANG=en" skriptu generator.sh
    na váš preferovanı jazyk napø "MENU_LANG=cz"


    - Uivatelé Microsoft Windows :
    Pøi spuštìní generátoru vyberte v rozbalovací nabídce poadovanı jazyk.


    Poznámka : Toto nemá vliv na volbu jazyka v DVD (více v sekci o MPlayeru ).

    Pro seznam podporovanıch jazykù nahlédnìte do adresáøe "language".
    Pokud není Váš jazyk v nabídce mùete jej vytvoøit pøeloením ji existujících souborù menu
    language/menu_LANG.conf a language/help_LANG.txt,
    a pøidáním LANG do language/lang.conf.

* MPlayer :
    Zde mùete provádìt nejvíce nastavení a customizací.
    Nastavení se provádìjí pomocí souboru packages/MPlayer/mplayer.conf.
    Pravdìpodobnì budete chtít zmìnit hodnoty jako napø. velikost OSD fontu.
    Mùete pøidat další parametry jako napø. vıchozí jazyk pro DVD (eg: alang=cz,en).
    Nejlepším místem pro získání parametrù je manuál MPlayeru.
    Uivatelé linuxu (man -l build/MPlayer-*/DOCS/mplayer.1). Mùete také nahlédnout na dokumentaci
    ( build/MPlayer-*/DOCS nebo na    http://mplayerhq.hu/DOCS/).
    Dalším souborem kde je dobré provádìt úpravy je packages/MPlayer/menu.conf.
    Mùete odebrat volby, které nepotøebujete nebo jej pøeloit do vašeho jazyka.
    Posledním souborem doporuèenım k editaci je  packages/MPlayer/build kterı obsahuje
    vıbìr moností MPlayeru.

* tv vıstup :
    TV vıstu je dosaen s pomocí drobnıch aplikací uršenıch pro dané typy grafickıch karet
    V souèasné dobì pouíváme  atitvout pro karty ATI, s3switch pro  karty S3  a nvtv pro karty nVidia
    (moná i karty intel i810 a karty 3dfx cards). Nastavení tìchto pomocnıch programù je provedeno
    v config/tvout. Tam mùete nastavi Váš TV standard (pal, ntsc...) a nastavit specielní volby pro nvtv.


    Zde mùete také definovat pomìr stran  (napø. 4:3 nebo 16:9) toho docílíte parametrem:

    TVOUT_ASPECT="4:3"

    Tento parametr bude pouit pro oba (TVOut i klasickı  (CRT/TFT)) vıstupy.
    V pøípadì e máte nestandardní monitor (Wide screen nebo videoprojektor) ùete nastavit
    poadovanou vıšku/šíøku stejnì jako obnovovací frekvenci.
    Parametry nastavujeme v souboru     /etc/mplayer/mplayer.conf .
    Vıchozí hodnoty jsou uvedeny níe (odkomentujte øádky u frekvencí pokud je chcete pouít) :

    screenw=800
    screenh=600
    #monitor-hfreq=31.5k-50k
    #monitor-vfreq=50-90

* Lirc :
     Mùete vybrat jeden z podporovanıch typù dálkovıch ovladaèù  editací souboru
    GEEXBOX/etc/remote. Dbejte na sprıvnı vıbìr pøijímacího zaøízení v tomté souboru
    Pokud chcete zbìnit pøiøazení tlaèítek Vašeho ovladaèe podívejte se do souboru
    GEEXBOX/etc/lirc/lircrc_REMOTE.

* Sí :
    Nastavení sítì se provádí v souboru GEEXBOX/etc/network.
    Zde mùete nastavit IP adresu pro GeeXboX (vıchozí nastavení je pouít DHCP
    a pohud proces sele, je nastavena adresa 192.168.0.54.
    Lze zde také nastavit uivatelské jméno a heslo pro pøístup ke sdílenım slokám
    systému windows (vıchozí nastavení se pøipojí pouze k anonymním sdílením).
    Parametry pro NFS se nastavují v souboru GEEXBOX/etc/nfs.

* wifi :
    GeeXboX se pokusí automaticky zjistit síové nastavení.
    Pokud máte v systému klasickou NIC a WiFi kartu pouze  poslední v øadì bude nastavena.
    Pro pøesné nastavení vyhovující Vašemu prostøedí musíte upravit soubor  /etc/network.


    Následující øádky se tıkají konfigurace WiFi :
    * PHY_TYPE="auto"      # Network physical type (auto|ethernet|wifi)
    * WIFI_MODE="managed"  # Wifi working mode (managed|ad-hoc)
    * WIFI_WEP=""          # Wifi WEP key
    * WIFI_ESSID="any"     # Wifi SSID

    Toto by mìlo postaèovat pro nastavení. Mùete zvolit autodetekci nebo dokonce zvolit pouívání
    pouze WiFi nebo ethernetu.
    Stejnım zpùsobem lze vybrat typ WiFi módu mezi  managed a  ad-hocvèetnì nastavení WEP klíèe a SSID.

* gateway :
    GeeXboX podporuje pøístup na sí internet. Pokud máte na síti dostupné internetové pøipojení
    mùete ho sdílet pro GeeXBox pouitím routeru nebo brány. Pro tuto monost editujte IP adresu brány
    v souboru /etc/network file.

    * GATEWAY=""     # IP adresa brány ("" pro  DHCP nebo ádnı pøístup na internet)

* nastavení TV :
    GeeXboX umoòuje pouití TV tunerù. Systém se snaí rozpoznat jakı televizní tuner máte.
    Mùete pøeskoèit autodetekci vepsáním parametrù Vašeho tuneru v  /etc/tvcard
    následujícím zpùsobem :

 #TV CARD/TUNER Model (AUTO pro autodetekci nebo zadejte hodnoty z následujících odkazù)
 #http://www.linuxhq.com/kernel/v2.6/2/Documentation/video4linux/CARDLIST.bttv
 #http://www.linuxhq.com/kernel/v2.6/2/Documentation/video4linux/CARDLIST.tuner

    TV_CARD=AUTO
    TV_TUNER=AUTO
    TVIN_STANDARD=pal


    Nechte parametr AUTO pokud si pøejete aby se systém pokusil rozpoznat Vaši kartu,
    nebo jej nahraïte èíslem Vaší karty v souladu z odkaz uvedenımi vıše.
    Pozor v pøípadì e nepouijete parametr AUTO musíte znát pøesnı typ Vaší karty!.

    Pokud máte nastaveno mìli by jste bıt schopni pøijímat TV signáls (Composite a S-VHS)
    na vaší  TV kartì. Stejnì jako sledovat televizní vysílání.
    Pro nalazení programù muzíte zadat region a frekvenci kanálu, kterı chcete sledovat.
    To lze provést v souboru  /etc/tvcard  :

    # TV programy
    # Syntaxe : CHAN="Frekvence:Název programu"
    # Ukázka :
    # CHAN="29:France 2"
    # CHAN="K08:Canal +"
    # TV Channels List
    # Available : france, europe-east, europe-west, us-bcast, us-cable
    CHANLIST=france

    Jednotlivé programy by se mìli objevit v základní nabídce.

* audio nastavení :
    GeeXboX podporuje analogovı  i  digitální  audio vıstup pøes klasickı JACK
    nebo   RCA SPDIF.  Vıchozí vıstup je nastavenı analogovı. Tuto hodnotu mùete zmìnit editací
    souboru /etc/audio :

    # Output using SPDIF (yes/no), otherwise ANALOG output
    SPDIF=no

    Pamatujte, e musíte nastavit SPDIF pokud chcete zvukovou kartu pøipojit
    k externímu zesilovaèi pro dekódování AC3/DTS stop.


* DXR3/Hollywood+ karty :
    Uivatelé s tímto druhem hardware ani nemusí mít video a zvukovou kartu k pouívání GeeXboXu.
    Ve reálu lze pouít pouze TV out s tìmito kartami.
    Moná bude zapotøebí nastavit poadovanou normu  (PAL/NTSC) v souboru  /etc/tvout
    fstejnì jako audio vıstup  (Analog nebo SPDIF) v  /etc/audio.

| Generování
| ~~~~~~~~~~
Nejprve si prosím pøeètìte sekci o nastavení viz vıše.

Potom jednoduše vygenerujte iso soubor v Linuxu spuštìním skriptu
  ./generator.sh
nebo ve windows aplikací
  generator.exe


| Instalace
| ~~~~~~~~~~~~

K instalaci je zapotøebí vytvoøit diskovı oddíl FAT16 s alespoò 16MB volného místa.

Pak je moné nainstalovat GeeXboX pod linuxem spuštìním skriptu
  ./installator.sh
S následnım zodpovìzením všech nastavujících dotazù. Dbejte zvıšené opatrnosti v prùbìhu instalace,
radìji pøerušte instalaci pokud si nejste jisti co dìláte.


| PXE Bootování
| ~~~~~~~~
GeeXboX je schopnı bootovat ze sítì na bezdiskové stanici.
K dosaení tohoto stavu budete potøebovat:

 -  DHCP server
 -  TFTP server
 -  NFS server
 -  PXE kompatibilní stanici :-)
Nejprve je nutné nakonfigurovat Váš DHCP server pro zasílání PXE bootovacích informací
Zde je ukázka s isc dhcp:


allow booting;
allow bootp;

subnet 192.168.0.0 netmask 255.255.255.0 {
  range 192.168.0.128 192.168.0.192;
  option subnet-mask 255.255.255.0;
  option broadcast-address 192.168.0.255;
  next-server 192.168.0.1;
  filename "/tftpboot/GEEXBOX/boot/pxelinux.0";
}
Další nastavení je adresa TFTP serveru.
Nakonfigurujte TFTP server na adresáø /tftpboot a nakopírujte všechny GeeXboX
soubory do tohoto adresáøe.
Napøíklad mùete nakopírovat obsah GeeXboX CD z linuxu  v módu TRANSPARENT DECOMPRESSION !
pro ovìøení nahlédnìte do souboru sbin/init a zkontrolujte e je struktura normální.
Pokud jste vytvoøili GeeXboX ze zrdrojovıch souborù mùete strukturu GeeXboXu vygenerovat pomocí
If you've built the GeeXboX yourself from sources, you can also generate
make pxe.

Poté mùete editovat soubor /tftpboot/GEEXBOX/boot/pxelinux.cfg/default
pro nastavení nfsroot na správnou  NFS cestu pro  GEEXBOX strukturu.

Nakonec nastavte NFS pro export GEEXBOX struktury v /etc/exports
zhruba tímto zpùsobem :

/tftpboot/GEEXBOX (ro)

 a /etc/hosts.allow asi takhle:

ALL: ALL

To by mìlo bıt vše nyní mùete zkusit nabootovat a uvidíte co se stane.


| Vytváøení ISO
| ~~~~~~~~

Nejprve prosím proètìte èást o nastavení.

Potom vytvoøte iso pomocí:
  make
Nebo soubor rovnou vypalte  :
  make burn

Kdy jste hotovi mùete GeeXboX stukturu smazat pro úsporu místa na disku:

  make clean
Nebo smazat všechny soubory a zdroje:

  make distclean

Existuje více nastavujících pøíkazù pokud se chcete v GeeXboXu troch vrtat:

  scripts/get package        # stáhnout balíèek
  scripts/unpack package     # rozbalit a pøipravit balíèek
  scripts/build package      # vytvoøí balíèek
  scripts/install package    # instalovat balíèek s $INSTALL prefix
  scripts/clean package      # vymazat adresáøovou strukturu balíèku

Pokud jste vytvoøili upravenou verzi GeeXboXu mùete vytvoøit tar.bz2 pomocí :
  make dist
nebo plnı  tar (se všemi zdrojovımi soubory) pomocí :
  make fulldist
nebo geexbox generátor pomocí :
  make generator
nebo  geexbox instalátor pomocí :
  make installator
nebo   pxe strukturu pomocí :
  make pxe


| Konfigurace
| ~~~~~~~~~~~~~

* Globální nastavení :

    Je nejdùleitìjším krokem pøed samotnım vytváøením GeeXboXu.
    Je uloeno v souboru  config/options, a jeho syntaxe je srozumitelná.
    Mùete zvolit typ Vašeho procesoru, téma, a zda chcete pouívat True Type fonty nebo ne.
    Také mùete upravit nastavení Vaší vypalovaèky aby bylo moné iso rovnou vypálit.

* Linux :
    Tohle je klasické nastavení Linuxu (packages/linux/linux.conf).
    Lze je editovat runì nebo spustit skript scripts/unpack linux
    a poté  make menuconfig -C build/linux-* (pøípadnì pouít Vámi preferovanou metodu namísto menuconfig).
    Zazálohujte /linux-*/.config do packages/linux/linux.conf.

* Lirc :
    Lirc umoòuje  ovládání GeeXboXu pomocí dálkového ovladaèe. Nejprve se pokuste nalézt
    soubor s vaším typem ovladaèe v  build/lirc-*/remotes (po provedení  scripts/unpack lirc) a
    pøidejte jej do packages/lirc/install. Potom vyberte zaøízení (vıchozí je  /dev/ttyS0 (COM1))
    a ovladaè lirc a ulote jej do souboru packages/lirc/lircd_$REMOTE. Poté zvolte nastavení tlaèítek
    v souboru packages/lirc/lircrc_$REMOTE. Pro kadé pøiøazení tlaèítka musíte vybrat jeho jméno ze souboru
    a zasociovat ho k urèité akci. Tou mùe bıt nìkterá z akcí MPlayeru (ty naleznete v souboru
    build/MPlayer-*/DOCS/documentation.html#commands).


| Úpravy
| ~~~~~~~

Jako první je dobré podívat se na inicializaèní skript.
Ve skuteènosti se jedná o skripty dva. První je v packages/initrd/linuxrc
ale ten pravdìpodobnì nebudete chtít upravovat. Druhı je  config/init a v nìm se dá
upravit nìkolik moností nastavení.

Další co by Vás mohlo zajímat je vytvoøení nového balíèku.
To není nic jiného ne spousta skriptù které sledují urèitá pravidla.
Všechny skripty musejí bıt ve stejném adresáøi jako je program kterı chcete "pøibalit"

Zde je seznam skriptù které mùete vytvoøit :
 - url : seznam adres kde lze stáhnout zdrojové soubory programù.
 - unpack : co se provede po rozbalení zdrojù, ùete napøíklad upravit konfiguraèní soubory,
            netıká se aplikací záplat.
 - need_build : pouití pokud balíèek ji byl sestaven, a pro ujištìní e nepotøebuje reebuilt.
                Dojde k odstranìní souboru .stamps/"package name"/build v pøípadì e rebuild není nutnı.

 - build :   všechny nezbytné kroky k vytvoøení programu.
 - install : všechny nezbytné kroky k instalaci  programu. Zaèátek instalace by mìl bıt $INSTALL.

Kdy je soubor z url adresy pojmenován záplata-program_name-...je automaticky záplata aplikována
na rozbalené zdrojové soubory programu.

Mìli by jste pamatovat e software na kterém GeeXboX bìí musí bıt zkompilován s
uClibc gcc wrapper.

Nakonec, nejlepší cesta k vytvoøení balíèku je podívat s edo ji hotovıch.



| Licenèní podmínky
| ~~~~~~~

Všechny programy pouité v GeeXboXu jsou chránìny jejich licencí.
Všechny jsou volnì šiøitelné a vìtšina z nich podléhá GNU licencování.
GeeXboX jako takovı. myšleno skripty pouité pro jeho vytvoøení spadají pod GNU.
