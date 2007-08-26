CFLAGS += -D_GNU_SOURCE
LDFLAGS += -j1

all: webgui

webgui: index.cgi state.cgi
#	$(CC) webgui.c libcgi.a -o webgui.cgi 
#	$(CC) searchfile.c libcgi.a -o searchfile.cgi  
	@chmod a+x *.cgi
	
state.cgi: state.o
	$(CC) $(CFLAGS) state.o system.o -o state.cgi
	
state.o: state.c
	$(CC) $(CFLAGS) -c state.c -o state.o
	
index.cgi: index.o browser.o system.o command.o control.o libcgi-util.o
	$(CC) $(CFLAGS) index.o browser.o system.o command.o control.o libcgi-util.o -o index.cgi

index.o: index.c
	$(CC) $(CFLAGS) -c index.c -o index.o
	
browser.o: browser.c browser.h
	$(CC) $(CFLAGS) -c browser.c -o browser.o
	
command.o: command.c command.h
	$(CC) $(CFLAGS) -c command.c -o command.o
	
control.o: control.c control.h
	$(CC) $(CFLAGS) -c control.c -o control.o
	
system.o: system.c system.h
	$(CC) $(CFLAGS) -c system.c -o system.o

libcgi-util.o: cgi-util.c cgi-util.h
	$(CC) $(CFLAGS) -fPIC cgi-util.c -c -o libcgi-util.o

install:
	@cp *.cgi /tftpboot/GEEXBOX/var/www/cgi-bin/
	@cp ../*.css /tftpboot/GEEXBOX/var/www/
	@cp -R ../img/* /tftpboot/GEEXBOX/var/www/img/

backup: install clean
	@tar -cjf ../webgui_`date -I`.tar.bz2 ../*.css ../cgi-bin/ ../img/

clean: 
	@rm -f *~ *.o *.cgi ../img/*.db ../img/big/*.db 
	
