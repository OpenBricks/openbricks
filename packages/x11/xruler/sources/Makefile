LIBS=-lXpm -lXext -lX11 

xruler: main.o
	$(CC) $(CFLAGS) -o xruler main.o $(LIBS)

install: xruler
	mkdir -p $(DESTDIR)/usr/bin
	cp -P xruler $(DESTDIR)/usr/bin
	
clean:
	rm -f main.o xruler
