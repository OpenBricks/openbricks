# based on http://sources.gentoo.org/cgi-bin/viewvc.cgi/gentoo-x86/dev-libs/tinyxml/files/Makefile-3

AR ?= ar
CXX ?= g++
CXXFLAGS += -Wall
RANLIB ?= ranlib
LIBDIR ?= /usr/lib
DESTDIR ?=

name = libtinyxml
major = 0
minor = 1
version = $(major).$(minor)

src = tinyxml.cpp tinyxmlparser.cpp tinyxmlerror.cpp tinystr.cpp
lo = $(addsuffix .lo,$(basename ${src}))
o = $(addsuffix .o,$(basename ${src}))

all: $(name).a $(name).so

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@

$(name).a: $(o)
	$(AR) rc $(name).a $(o)
	$(RANLIB) $(name).a

%.lo: %.cpp
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) -fPIC $< -o $@

$(name).so: $(lo)
	$(CXX) $(LDFLAGS) -fPIC -shared $(lo) -Wl,-soname,$(name).so.$(major) -o $(name).so.$(version)
	ln -s $(name).so.$(version) $(name).so.$(major)
	ln -s $(name).so.$(version) $(name).so

clean:
	-rm -f *.o *.lo *.so* *.a

install:
	install -d -m 0755  $(DESTDIR)/usr/lib
	install -m 0755 $(name).so.$(version) $(DESTDIR)/usr/lib/
	install -m 0755 $(name).so.$(major) $(DESTDIR)/usr/lib/
	install -m 0755 $(name).so $(DESTDIR)/usr/lib/
	install -d -m 0755  $(DESTDIR)/usr/lib/pkgconfig
	install -m 0644 tinyxml.pc $(DESTDIR)/usr/lib/pkgconfig
	install -d -m 0755  $(DESTDIR)/usr/include
	install -m 0644 tinyxml.h tinystr.h $(DESTDIR)/usr/include
