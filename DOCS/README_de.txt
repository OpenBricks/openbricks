
         ooooo                     oo     o  o               oo     o
        M"   "Mo                    Mo  oM"  M                Mo  oM"
       M           oM""Mo  oM""Mo    "Mo"    Mo"""o  oM""Mo    "Mo"
       M    """M   MooooMM MooooMM   oMMo    M    "M M    M    oMMo
       "o     oM   M    o  M    o   oM  Mo   M    oM M    M   oM  Mo
        "MoooM"M   "MooM"  "MooM"  M"    "M  M"ooo"  "MooM"  M"    "M



| ÜBER GEEXBOX
| ~~~~~~~~~~~~

GeeXboX ist eine Art "Divx Box" Software. Genaugenommen ist es eine bootfähige
CD, welche es erlaubt, Filme anzuschauen oder Musik zu hören. Es unterstützt
viele Formate wie zum Beispiel avi, mpeg, divx, ogm, rm, mp3, ogg, dvd, vcd und
 cdda. 
GeeXboX unterstützt zudem einige Infrarot-Fernbedienungen und den TV-Ausgang
einiger Grafikkarten.
Dieses Archiv enthält die nötigen Scripte um ein eigenes ISO-Image von GeeXboX
zu erstellen.


| ERFORDERNISSE
| ~~~~~~~~~~~~~

Um ein GeeXboX ISO zu erstellen, benötigen Sie eines der folgenden Systeme:
  - ein GNU/Linux System mit mkisofs und mkzftree.
  - ein MAC OS X System mit mkisofs und mkzftree.
  - ein Windows System.

Um GeeXboX zu installieren benötigen Sie:
  - ein GNU/Linux System mit syslinux.

Um Ihre eigene GeeXboX zu erstellen benötigen Sie einige klassische Werkzeuge:
  - ein laufendes GNU/Linux System.
  - denn gcc C Compiler.
  - GNU make
  - das Kommando patch.
  - den nasm Assembler.
  - bzip2 und gzip.
  - mkfs.ext2 und mkfs.vfat
  - das wget Download-Werkzeug (wird nicht für das komplette GeeXboX Paket
    benötigt).
  - mkisofs und mkzftree um das ISO-Image zu erstellen.
  - mkzftree um die Dateien des ISO-Images zu komprimieren.
  - cdrecord (um das ISO-Image zu brennen).

Zudem benötigen Sie mindestens 500 MB freien Speicher auf Ihrer Festplatte.


| PERSONALISIERUNG
| ~~~~~~~~~~~~~~~~

Mit dem Generator ist es sehr einfach GeeXboX zu personalisieren.

Sie können zum Beispiel die Unterstützung einiger proprietäre Codecs wie rv9
oder wmv9 hinzufügen, indem Sie die Codecs einfach in das Verzeichnis
GEEXBOX/codecs kopieren. Diese Codecs finden Sie im Paket, welches unter
http://www.mplayerhq.hu/MPlayer/releases/codecs/ erhältlich ist.

Folgende Dateien aus diesem Paket sind nützlich:
  * Real Codecs (werden benötigt um beispielsweise rv9 abzuspielen):
    atrc.so.6.0, cook.so.6.0, sipr.so.6.0, drv3.so.6.0, drv4.so.6.0.
  * Windows Media Codecs (wmv9/wma9): wma9dmod.dll und wmv9dmod.dll.

Sie können aber auch andere Einstellungen verändern, indem Sie einfach einige
Text-Dateien editieren.

* Sprache:
    Sie können die Menu-Sprache ändern, indem Sie die Datei GEEXBOX/etc/lang
    editieren. Diese Einstellung hat keinen Effekt auf die Sprache der DVD
    (siehe Kapitel MPlayer). Falls Ihre Sprache nicht existiert, können Sie das
    Menu übersetzen, indem Sie die Dateien GEEXBOX/etc/mplayer/menu_LANG.conf
    und GEEXBOX/usr/share/mplayer/help_LANG.txt bearbeiten.

* MPlayer:
    Dies ist der Ort, an dem Sie die meisten Einstellungen und Veränderungen
    vornehmen können. Die Einstellungen sind in der Datei 
    packages/MPlayer/mplayer.conf enthalten.
    Es ist zum Beispiel möglich, die Schriftgrösse des OSD zu verändern
    (subfont-text-scale).
    Sie können aber auch viele andere Einstellungen wie zum Beispiel die DVD
    Standard-Sprache verändern (zum Beispiel: alang=fr,en). Die beste Übersicht
    über alle Einstellung finden Linux-Benutzer in der manpage
    (man -l build/MPlayer-*/DOCS/mplayer.1). Weitere Informationen finden Sie
    auch in der MPlayer Dokumentation (in build/MPlayer-*/DOCS oder unter
    http://mplayerhq.hu/DOCS/).
    Eine andere Datei, die sie vielleicht editieren wollen, ist
    packages/MPlayer/menu.conf. Sie können einzelne Menupunkte entfernen oder
    Übersetzungen machen.
    Die letzte interessante Datei ist packages/MPlayer/build. Sie enthält die
    Optionen, welche für das Erstellen von MPlayer gesetzt sind.

* TV-Ausgang:
    Für das Aktivieren des TV-Ausgangs werden mehrere kleinen Programme für
    jeweils einen Hersteller benutzt. Wir verwenden im Moment atitvout für ATI
    Karten, s3switch für S3 Karten und nvtv für nVidia Karten (unterstützt zum
    Teil ebenfalls Intel i810 und 3dfx Karten). Die Einstellungen dieser
    Programme werden über die Datei config/tvsettings gemacht. Sie können den
    TV-Standard wählen (PAL, NTSC) und einige spezifische Optionen für nvtv
    vornehmen.

* Lirc:
    Sie können eine unterstützte Fernbedienung wählen, indem Sie die Datei
    GEEXBOX/etc/remote editieren. Falls Sie eine ATI Remote Wonder verwenden,
    welche nicht von Lirc unterstützt wird, müssen Sie nichts verändern, da
    sie standardmässig von GeeXboX unterstützt wird. Falls Sie die
    Tastenbelegung ihrer Fernbedienung verändern möchten, so editieren Sie die
    Datei GEEXBOX/etc/lirc/lircrc_REMOTE.

* Netzwerk :
    Die Netzwerkeinstellungen werden in der Datei GEEXBOX/etc/network
    vorgenommen. Hier können Sie beispielsweise die IP-Adresse festlegen
    (Standardmässig wird DHCP verwendet und falls dies nicht funktioniert,
    wird die IP 192.168.0.54 verwendet).
    Sie können aber auch einen Login-Name und eine Passwort festlegen, welches
    für die Windows-Freigaben verwendet wird (standardmässig wird nur zu
    anonymen Freigaben verbunden).


| ERSTELLEN EINER ISO-DATEI
| ~~~~~~~~~~~~~~~~~~~~~~~~~

Bitte widmen Sie einen Augenblick der oben beschriebenen Personalisierung
bevor Sie mit der Erstellung einer ISO-Datei beginnen.

Anschliessend können Sie unter Linux die ISO-Datei mit dem folgenden Kommando
erstellen:
  ./generator.sh
oder unter Windows mit dem Starten der Datei
  generator.exe


| INSTALLATION
| ~~~~~~~~~~~~

Als erstes benötigen Sie eine FAT16 Partition mit ungefähr 16 MB freiem
Speicherplatz.

Dann können Sie die Installation unter Linux starten, indem Sie folgendes
Kommando eingeben:
  ./installator.sh
Beantworten Sie anschliessend alle Fragen. Seien Sie bei diesem Vorgang sehr
vorsichtig! Lesen Sie jede Frage zweimal und unterbrechen Sie die Installation
wenn Sie eine Frage nicht verstehen.


| PXE BOOT
| ~~~~~~~~

Ja, GeeXboX ist fähig über ein Netzwerk ohne Festplatte zu starten!
Um dies zu erreichen benötigen Sie folgendes:
 - ein DHCP Server
 - ein TFTP Server
 - ein NFS Server
 - ein PXE fähiger Computer :-)

Zuerst müssen Sie den DHCP Server konfigurieren, damit dieser die PXE Boot
Informationen schickt. Im Folgenden eine Beispiel mit isc dhcp:

allow booting;
allow bootp;

subnet 192.168.0.0 netmask 255.255.255.0 {
  range 192.168.0.128 192.168.0.192;
  option subnet-mask 255.255.255.0;
  option broadcast-address 192.168.0.255;
  next-server 192.168.0.1;
  filename "/tftpboot/GEEXBOX/boot/pxelinux.0";
}

Die next-server Option ist die Adresse des TFTP Servers.

Anschliessend konfigurieren Sie Ihren TFTP Server (wie zum Beispiel atftpd)
so, dass er das Verzeichnis /tftpboot zur Verfügung stellt. Das Verzeichnis
/tftpboot muss einen vollständigen GEEXBOX Stammbaum enthalten. Sie können
dazu zum Beispiel den Inhalt eine GeeXboX CD von Linux aus MIT DER AKTIVIERTEN
TRANSPARENTEN DEKOMPRESSION DER CDROM! (um dies zu überprüfen schauen Sie auf
den Inhalt der sbin/init im GeeXboX Dateibaum und prüfen, ob diese Datei
keinen Müll enthält).
Falls Sie GeeXboX selber aus den Sourcen kompiliert haben, so können Sie einen
Dateibaum ganz einfach mit make pxe erstellen.

Danach sollten Sie die Datei /tftpboot/GEEXBOX/boot/pxelinux.cfg/default
editieren, so dass nfsroot zum richtigen NFS Pfad des GEEXBOX Dateibaumes
zeigt.

Am Schluss müssen Sie noch NFS so konfigurieren, dass der GEEXBOX Dateibaum
exportiert wird. Dies erreichen Sie, indem Sie der Datei /etc/exports in etwa
folgendes hinzufügen:

/tftpboot/GEEXBOX (ro)

und der Datei /etc/hosts.allow folgendes hinzufügen:

ALL: ALL

Damit ist die Konfiguration abgeschlossen. Starten Sie den PXE Computer und
schauen Sie was passiert.


| KOMPILIERUNG
| ~~~~~~~~~~~~

Bevor Sie beginnen, lesen Sie bitte das anschliessende Konfigurations-Kapitel
(mindestens die allgemeinen Einstellungen).

Anschliessen können Sie das ISO-Image einfach mit folgendem Kommando erstellen:
  make
oder Sie brennen das ISO-Image sogleich:
  make burn

Wenn Sie fertig sind, können Sie Speicherplatz schaffen, indem Sie nicht mehr benötigte Dateien löschen:
  make clean
oder alle Dateien inklusive der heruntergeladenen Dateien löschen:
  make distclean

Es gibt noch einige Kommandos, die für fortgeschrittene Benutzer gedacht sind:
  scripts/get package        # lädt ein Paket herunter
  scripts/unpack package     # entpackt und bereitet das Paket vor
  scripts/build package      # kompliert das Paket
  scripts/install package    # installiert das Paket nach $INSTALL
  scripts/clean package      # reinigt den Dateibaum des Paketes
  make exec                  # startet GeeXboX in einem Käfig
                             # WARNUNG: Dies ist eine experimentelle Funktion
                             # Benutzung auf eigene Gefahr.

Falls Sie eine veränderte Version von GeeXboX erstellt haben, so können Sie
sehr einfach ein tar.bz2 Archiv davon erstellen:
  make dist
oder ein vollständiges Archiv (inklusive allen Sourcen):
  make fulldist
oder einen GeeXboX Generator mit:
  make generator
oder eine GeeXboX Installationsroutine mit:
  make installator


| KONFIGURATION
| ~~~~~~~~~~~~~

* Allgemeine Einstellungen:
    Dies ist der erste Schritt, den Sie vor dem Kompilieren von GeeXboX machen
    sollten. Alle Einstellungen stehen in der Datei config/options, welche
    sich selbst erklären sollte. Dort können Sie die CPU-Familie und das Thema
    wählen, und ob sie TrueType Schriften wollen oder nicht. Sie sollten zudem
    die Konfiguration des CD-Brenners anpassen, sofern sie die ISO-Datei
    direkt brennen wollen.

* Linux :
    Dies ist die klassische Linux-Konfiguration (packages/linux/linux.conf).
    Sie können diese Datei entweder von Hand oder mit einem
    scripts/unpack linux und einem anschliessenden
    make menuconfig -C build/linux-* (oder ihrer bevorzugten Methode anstatt
    von menuconfig) anpassen. Danach sollten Sie Ihre Konfigurationsdatei
    build/linux-*/.config nach packages/linux/linux.conf sichern.

* Lirc :
    Lirc erlaubt Ihnen GeeXboX mit einer Fernbedienung zu steuern. Zuerst
    müssen Sie die zur ihrer Fernbedienung passende Datei aus
    build/lirc-*/remotes (nachdem Sie ein scripts/unpack lirc gemacht haben)
    auswählen und diese zur Datei packages/lirc/install hinzufügen. Danach
    sollten Sie die Gerätedatei (Standard ist /dev/ttyS0 (COM1)) und den Lirc
    Treiber auswählen und dies in die Datei packages/lirc/lircd_$REMOTE
    hinzufügen. Danach können Sie die Tasten in der Datei
    packages/lirc/lircrc_$REMOTE belegen. Für jede Belegung müssen Sie einen
    Knopf (entnehmen Sie die Namen der Knöpfe der Fernbedienungs-Defintion-
    Datei) mit einer passenden Aktion assoziieren. Die Aktion ist eine der
    MPlayer Aktionen (sie finden eine Liste der möglichen Aktionen in der
    HTML-Datei build/MPlayer-*/DOCS/documentation.html#commands).


| VERÄNDERUNGEN 
| ~~~~~~~~~~~~~

Das Erste was Sie anschauen sollten, ist das Initialisierungs-Script. 
Eigentlich sind es zwei Initialisierungs-Scripte. Das Erste ist 
packages/initrd/linuxrc welches Sie aber wahrscheinlich nicht zu verändern
brauchen. Das Zweite ist config/init, wo Sie Ihre Personalisierungen
einbringen können.

Das nächste was Sie interessieren könnte, ist die Erstellung eines neuen
"Paketes". Ein Paket ist nur eine Reihe von Scripten, die einigen Regeln
folgen müssen. Alle Scripte müssen sich in einem Verzeichnis befinden mit dem
gleichen Namen wie das Programm, das Sie "packen" wollen, dieses Verzeichnis
wiederum im packages Verzeichnis.

Hier ist eine Liste mit den Scripten, die Sie erstellen sollten:
 - url : nur eine Liste der URL's, wo man die Programm-Sourcen herunterladen
         kann.
 - unpack : enhält was nach dem Auspacken des Paketes gemacht wird. Zum
            Beispiel das Modifizieren von Konfigurations-Dateien. Dieses
            Script führt jedoch keine Patches durch.
 - need_build : wird aufgerufen falls das Paket schon kompiliert wurde, um
                sicherzugehen, dass es nicht noch einmal kompiliert werden
                muss. Es sollte die Datei .stamps/"package name"/build
                entfernen, falls das Paket nochmals kompiliert werden muss.
 - build : enthält alle Schritte um das Programm zu kompilieren.
 - install : enthält alle Schritte um das Programm installieren zu können. Das
             Prefix der Installtion sollte $INSTALL sein.

Wenn die Datei url einen Dateinamen der Form patch-programmname-... enthält,
wird der Patch automatisch auf die entpackten Sourcen ausgeführt.

Beachten Sie zudem, dass Software, die auf der GeeXboX laufen soll, mit dem
uClibc gcc Wrapper kompiliert werden muss.

Es bleibt nur noch zu sagen, dass der beste Weg ein eigenes Paket zu erstellen
der ist, einen Blick auf andere Pakete zu werfen.


| LIZENZ
| ~~~~~~~

Alle Programme die von GeeXboX benutzt werden sind durch ihre eigene Lizenz
geschützt. Sie alle sind freie Software und die meisten Programme stehen unter
der GNU General Public License.
GeeXboX selber, gemeint sind alle Scripte die für die Komilierung benutzt
werden, stehen unter der GNU General Public License.

