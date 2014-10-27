#
# Makefile for the Video Disk Recorder
#
# See the main source file 'vdr.c' for copyright information and
# how to reach the author.
#
# $Id: Makefile 2.52 2013/02/17 11:08:04 kls Exp $

.DELETE_ON_ERROR:

# Compiler flags:

CC       ?= gcc
CFLAGS   ?= -g -O3 -Wall

CXX      ?= g++
CXXFLAGS ?= -g -O3 -Wall -Werror=overloaded-virtual -Wno-parentheses

CDEFINES  = -D_GNU_SOURCE
CDEFINES += -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE

LIBS      = -ljpeg -lpthread -ldl -lcap -lrt $(shell pkg-config --libs freetype2 fontconfig)
INCLUDES ?= $(shell pkg-config --cflags freetype2 fontconfig)

# Directories:

CWD       ?= $(shell pwd)
LSIDIR    ?= $(CWD)/libsi
PLUGINDIR ?= $(CWD)/PLUGINS

DESTDIR   ?=
VIDEODIR  ?= /data/tv/tv0
CONFDIR   ?= /etc/vdr
CACHEDIR  ?= /var/cache/vdr

PREFIX    ?= /usr
BINDIR    ?= $(PREFIX)/bin
INCDIR    ?= $(PREFIX)/include
LIBDIR    ?= $(PREFIX)/lib/vdr
LOCDIR    ?= $(PREFIX)/share/locale
MANDIR    ?= $(PREFIX)/share/man
PCDIR     ?= $(PREFIX)/lib/pkgconfig
RESDIR    ?= $(PREFIX)/share/vdr

# Source documentation

DOXYGEN  ?= /usr/bin/doxygen
DOXYFILE  = Doxyfile

# User configuration

-include Make.config

# Mandatory compiler flags:

CFLAGS   += -fPIC
CXXFLAGS += -fPIC

# Common include files:

ifdef DVBDIR
CINCLUDES += -I$(DVBDIR)
endif

# Object files

SILIB    = $(LSIDIR)/libsi.a

OBJS = audio.o channels.o ci.o config.o cutter.o device.o diseqc.o dvbdevice.o dvbci.o\
       dvbplayer.o dvbspu.o dvbsubtitle.o eit.o eitscan.o epg.o filetransfer.o filter.o font.o i18n.o interface.o keys.o\
       lirc.o menu.o menuitems.o nit.o osdbase.o osd.o pat.o player.o plugin.o\
       receiver.o recorder.o recording.o remote.o remux.o ringbuffer.o sdt.o sections.o shutdown.o\
       skinclassic.o skinlcars.o skins.o skinsttng.o sourceparams.o sources.o spu.o status.o svdrp.o themes.o thread.o\
       timers.o tools.o transfer.o vdr.o videodir.o

OBJS += iconpatch.o

OBJS += vdrttxtsubshooks.o

DEFINES  += $(CDEFINES)
INCLUDES += $(CINCLUDES)

ifdef HDRDIR
HDRDIR   := -I$(HDRDIR)
endif
ifndef NO_KBD
DEFINES += -DREMOTE_KBD
endif
ifdef REMOTE
DEFINES += -DREMOTE_$(REMOTE)
endif
ifdef VDR_USER
DEFINES += -DVDR_USER=\"$(VDR_USER)\"
endif
ifdef BIDI
INCLUDES += $(shell pkg-config --cflags fribidi)
DEFINES += -DBIDI
LIBS += $(shell pkg-config --libs fribidi)
endif

ifdef LIVEBUFFER
DEFINES += -DUSE_LIVEBUFFER
OBJS += livebuffer.o
endif

LIRC_DEVICE ?= /var/run/lirc/lircd

DEFINES += -DLIRC_DEVICE=\"$(LIRC_DEVICE)\"
DEFINES += -DVIDEODIR=\"$(VIDEODIR)\"
DEFINES += -DCONFDIR=\"$(CONFDIR)\"
DEFINES += -DCACHEDIR=\"$(CACHEDIR)\"
DEFINES += -DRESDIR=\"$(RESDIR)\"
DEFINES += -DPLUGINDIR=\"$(LIBDIR)\"
DEFINES += -DLOCDIR=\"$(LOCDIR)\"

# The version numbers of VDR and the plugin API (taken from VDR's "config.h"):

VDRVERSION = $(shell sed -ne '/define VDRVERSION/s/^.*"\(.*\)".*$$/\1/p' config.h)
APIVERSION = $(shell sed -ne '/define APIVERSION/s/^.*"\(.*\)".*$$/\1/p' config.h)

all: vdr i18n plugins

# Implicit rules:

%.o: %.c
	$(CXX) $(CXXFLAGS) -c $(DEFINES) $(INCLUDES) -o $@ $<

# Dependencies:

MAKEDEP = $(CXX) -MM -MG
DEPFILE = .dependencies
$(DEPFILE): Makefile
	@$(MAKEDEP) $(DEFINES) $(INCLUDES) $(OBJS:%.o=%.c) > $@

-include $(DEPFILE)

# The main program:

vdr: $(OBJS) $(SILIB)
	$(CXX) $(CXXFLAGS) -rdynamic $(LDFLAGS) $(OBJS) $(LIBS) $(SILIB) -o vdr

# The libsi library:

$(SILIB):
	$(MAKE) --no-print-directory -C $(LSIDIR) CXXFLAGS="$(CXXFLAGS)" DEFINES="$(CDEFINES)" all

# pkg-config file:

.PHONY: vdr.pc
vdr.pc:
	@echo "bindir=$(BINDIR)" > $@
	@echo "mandir=$(MANDIR)" >> $@
	@echo "configdir=$(CONFDIR)" >> $@
	@echo "videodir=$(VIDEODIR)" >> $@
	@echo "cachedir=$(CACHEDIR)" >> $@
	@echo "resdir=$(RESDIR)" >> $@
	@echo "libdir=$(LIBDIR)" >> $@
	@echo "locdir=$(LOCDIR)" >> $@
	@echo "incdir=$(INCDIR)" >> $@
	@echo "plgcfg=$(PLGCFG)" >> $@
	@echo "apiversion=$(APIVERSION)" >> $@
	@echo "cflags=$(CFLAGS) $(CDEFINES) $(CINCLUDES) $(HDRDIR)" >> $@
	@echo "cxxflags=$(CXXFLAGS) $(CDEFINES) $(CINCLUDES) $(HDRDIR)" >> $@
	@echo "" >> $@
	@echo "Name: VDR" >> $@
	@echo "Description: Video Disk Recorder" >> $@
	@echo "URL: http://www.tvdr.de/" >> $@
	@echo "Version: $(VDRVERSION)" >> $@
	@echo "Cflags: \$${cflags}" >> $@

# Internationalization (I18N):

PODIR     = po
LOCALEDIR = locale
I18Npo    = $(wildcard $(PODIR)/*.po)
I18Nmo    = $(addsuffix .mo, $(foreach file, $(I18Npo), $(basename $(file))))
I18Nmsgs  = $(addprefix $(LOCALEDIR)/, $(addsuffix /LC_MESSAGES/vdr.mo, $(notdir $(foreach file, $(I18Npo), $(basename $(file))))))
I18Npot   = $(PODIR)/vdr.pot

%.mo: %.po
	msgfmt -c -o $@ $<

$(I18Npot): $(wildcard *.c)
	xgettext -C -cTRANSLATORS --no-wrap --no-location -k -ktr -ktrNOOP --package-name=VDR --package-version=$(VDRVERSION) --msgid-bugs-address='<vdr-bugs@tvdr.de>' -o $@ `ls $^`

%.po: $(I18Npot)
	msgmerge -U --no-wrap --no-location --backup=none -q -N $@ $<
	@touch $@

$(I18Nmsgs): $(LOCALEDIR)/%/LC_MESSAGES/vdr.mo: $(PODIR)/%.mo
	install -D -m644 $< $@

.PHONY: i18n
i18n: $(I18Nmsgs)

install-i18n: i18n
	@mkdir -p $(DESTDIR)$(LOCDIR)
	cp -r $(LOCALEDIR)/* $(DESTDIR)$(LOCDIR)

# The 'include' directory (for plugins):

include-dir:
	@mkdir -p include/vdr
	@(cd include/vdr; for i in ../../*.h; do ln -fs $$i .; done)
	@mkdir -p include/libsi
	@(cd include/libsi; for i in ../../libsi/*.h; do ln -fs $$i .; done)

# Plugins:

plugins: include-dir vdr.pc
	@failed="";\
	noapiv="";\
	oldmakefile="";\
	for i in `ls $(PLUGINDIR)/src | grep -v '[^a-z0-9]'`; do\
	    echo; echo "*** Plugin $$i:";\
	    # No APIVERSION: Skip\
	    if ! grep -q "\$$(LIBDIR)/.*\$$(APIVERSION)" "$(PLUGINDIR)/src/$$i/Makefile" ; then\
	       echo "ERROR: plugin $$i doesn't honor APIVERSION - not compiled!";\
	       noapiv="$$noapiv $$i";\
	       continue;\
	       fi;\
	    # Old Makefile\
	    if ! grep -q "PKGCFG" "$(PLUGINDIR)/src/$$i/Makefile" ; then\
	       echo "WARNING: plugin $$i is using an old Makefile!";\
	       oldmakefile="$$oldmakefile $$i";\
	       $(MAKE) --no-print-directory -C "$(PLUGINDIR)/src/$$i" CFLAGS="$(CFLAGS) $(CDEFINES) $(CINCLUDES)" CXXFLAGS="$(CXXFLAGS) $(CDEFINES) $(CINCLUDES)" LIBDIR="$(PLUGINDIR)/lib" VDRDIR="$(CWD)" all || failed="$$failed $$i";\
	       continue;\
	       fi;\
	    # New Makefile\
	    INCLUDES="-I$(CWD)/include"\
	    $(MAKE) --no-print-directory -C "$(PLUGINDIR)/src/$$i" VDRDIR="$(CWD)" || failed="$$failed $$i";\
	    if [ -n "$(LCLBLD)" ] ; then\
	       (cd $(PLUGINDIR)/src/$$i; for l in libvdr-*.so; do install $$l $(LIBDIR)/$$l.$(APIVERSION); done);\
	       if [ -d $(PLUGINDIR)/src/$$i/po ]; then\
	          for l in `ls $(PLUGINDIR)/src/$$i/po/*.mo`; do\
	              install -D -m644 $$l $(LOCDIR)/`basename $$l | cut -d. -f1`/LC_MESSAGES/vdr-$$i.mo;\
	              done;\
	          fi;\
	       fi;\
	    done;\
	# Conclusion\
	if [ -n "$$noapiv" ] ; then echo; echo "*** plugins without APIVERSION:$$noapiv"; echo; fi;\
	if [ -n "$$oldmakefile" ] ; then\
	   echo; echo "*** plugins with old Makefile:$$oldmakefile"; echo;\
	   echo "**********************************************************************";\
	   echo "*** While this currently still works, it is strongly recommended";\
	   echo "*** that you convert old Makefiles to the new style used since";\
	   echo "*** VDR version 1.7.36. Support for old style Makefiles may be dropped";\
	   echo "*** in future versions of VDR.";\
	   echo "**********************************************************************";\
	   fi;\
	if [ -n "$$failed" ] ; then echo; echo "*** failed plugins:$$failed"; echo; exit 1; fi

clean-plugins:
	@for i in `ls $(PLUGINDIR)/src | grep -v '[^a-z0-9]'`; do $(MAKE) --no-print-directory -C "$(PLUGINDIR)/src/$$i" clean; done
	@-rm -f $(PLUGINDIR)/lib/lib*-*.so.$(APIVERSION)

# Install the files (note that 'install-pc' must be first!):

install: install-pc install-bin install-dirs install-conf install-doc install-plugins install-i18n install-includes

# VDR binary:

install-bin: vdr
	@mkdir -p $(DESTDIR)$(BINDIR)
	@cp --remove-destination vdr svdrpsend $(DESTDIR)$(BINDIR)

# Configuration files:

install-dirs:
	@mkdir -p $(DESTDIR)$(VIDEODIR)
	@mkdir -p $(DESTDIR)$(CONFDIR)
	@mkdir -p $(DESTDIR)$(CACHEDIR)
	@mkdir -p $(DESTDIR)$(RESDIR)

install-conf:
	@cp -pn *.conf $(DESTDIR)$(CONFDIR)

# Documentation:

install-doc:
	@mkdir -p $(DESTDIR)$(MANDIR)/man1
	@mkdir -p $(DESTDIR)$(MANDIR)/man5
	@gzip -c vdr.1 > $(DESTDIR)$(MANDIR)/man1/vdr.1.gz
	@gzip -c vdr.5 > $(DESTDIR)$(MANDIR)/man5/vdr.5.gz

# Plugins:

install-plugins: plugins
	@-for i in `ls $(PLUGINDIR)/src | grep -v '[^a-z0-9]'`; do\
	      $(MAKE) --no-print-directory -C "$(PLUGINDIR)/src/$$i" VDRDIR=$(CWD) DESTDIR=$(DESTDIR) install;\
	      done
	@if [ -d $(PLUGINDIR)/lib ] ; then\
	    for i in `find $(PLUGINDIR)/lib -name 'lib*-*.so.$(APIVERSION)'`; do\
	        install -D $$i $(DESTDIR)$(LIBDIR);\
	        done;\
	    fi

# Includes:

install-includes: include-dir
	@mkdir -p $(DESTDIR)$(INCDIR)
	@cp -pLR include/vdr include/libsi $(DESTDIR)$(INCDIR)

# pkg-config file:

install-pc: vdr.pc
	if [ -n "$(PCDIR)" ] ; then\
	   mkdir -p $(DESTDIR)$(PCDIR) ;\
	   cp vdr.pc $(DESTDIR)$(PCDIR) ;\
	   fi

# Source documentation:

srcdoc:
	@cat $(DOXYFILE) > $(DOXYFILE).tmp
	@echo PROJECT_NUMBER = $(VDRVERSION) >> $(DOXYFILE).tmp
	$(DOXYGEN) $(DOXYFILE).tmp
	@rm $(DOXYFILE).tmp

# Housekeeping:

clean:
	@$(MAKE) --no-print-directory -C $(LSIDIR) clean
	@-rm -f $(OBJS) $(DEPFILE) vdr vdr.pc core* *~
	@-rm -rf $(LOCALEDIR) $(PODIR)/*.mo $(PODIR)/*.pot
	@-rm -rf include
	@-rm -rf srcdoc
CLEAN: clean
distclean: clean-plugins clean
