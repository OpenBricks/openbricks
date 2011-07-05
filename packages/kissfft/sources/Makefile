VPATH=tools/

CFLAGS += -Wall -g
CFLAGS += -I.

OBJS = kiss_fft.o kiss_fftnd.o kiss_fftndr.o kiss_fftr.o

all: libkiss_fft.a libkiss_fft.so

libkiss_fft.a: $(OBJS)
	$(AR) ruv $@ $(OBJS)

libkiss_fft.so: $(OBJS)
	$(CC) -shared -Wl,-soname,$@ -o $@ $(OBJS) -lm

install:
	mkdir -p $(DESTDIR)/usr/include
	mkdir -p $(DESTDIR)/usr/lib
	cp -P libkiss_fft.a libkiss_fft.so $(DESTDIR)/usr/lib
	cp -P kissfft.hh kiss_fft.h tools/kiss_fftnd.h tools/kiss_fftndr.h tools/kiss_fftr.h $(DESTDIR)/usr/include

clean:
	rm -f *.o libkiss_fft.a libkiss_fft.so
