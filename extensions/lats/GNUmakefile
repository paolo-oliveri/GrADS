#
# GNU makefile for sample Dynamically Linked UDFs.
#

export PODS=liblats.pod
export XCFLAGS=-DUSELATS=1 -DGOT_NETCDF
export EXTRAS=galats.o lats.o  latsgrib.o  latsgribmap.o  latsint.o  \
              latsnc.o  latssd.o latsstat.o  latstime.o \
              fgutil.o fgbds.o ShaveMantissa.o
export GSFS=lats4d.gs

OS := $(shell uname -o)
ifeq ($(ARCH),AIX)
      XLDFLAGS :=
else
ifeq ($(OS),Cygwin)
   XLDFLAGS := "-lnetcdf"
endif
endif


%:
	@$(MAKE) -f ../gex.mk $@ CSRC="liblats.c" XLDFLAGS="-lnetcdf"

html:
	./lats4d.sh -h | ./mkhtml.pl -t >  lats4d.html
	./lats4d.sh -h | ./mkhtml.pl    >> lats4d.html
	@$(MAKE) -f ../gex.mk html CSRC="liblats.c"

VERSION:
	./lats4d.sh -h | grep '(Version' | cut -d' ' -f12 > VERSION

BASEN = lats4d-$(shell cat VERSION)

dist: html VERSION
	/bin/rm -rf $(BASEN)
	mkdir -p $(BASEN)
	cp ChangeLog README NEWS lats4d.html \
           lats4d.gs lats4d.sh $(BASEN)
	cp INSTALL.txt $(BASEN)/INSTALL # so that make install works
	tar cvfz $(BASEN).tar.gz $(BASEN)
 
