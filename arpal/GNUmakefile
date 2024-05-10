#
# Driver Makefile for CMI UDFs
# 20220627, Paolo Oliveri
#

.SILENT:

SHELL = /bin/bash
PYTHON = python2

ARCH := $(shell uname -s)
MACH := $(shell uname -m)
ifeq ($(ARCH),Darwin)
   OS := MacOSX
else
ifeq ($(ARCH),FreeBSD)
   OS := FreeBSD
else
   OS := $(shell uname -o)
endif
endif

bintype=# On windows this will be set to c, nc, nc4, dods, hdf, nam
bindir := $(shell mkdir -p $(GADDIR); cd $(GADDIR); pwd)
gexdir = $(bindir)/gex/$(bintype)

ifeq ($(OS),Cygwin)
	bintype=
endif
export bintype bindir gexdir

# For pytests
HERE := $(shell pwd)

PYTHONPATH := $(HERE)/../pytests/lib/grads

ifeq ($(ARCH),Darwin)
   DYLD_LIBRARY_PATH := $(shell cd $(GADDIR)/gex; pwd):$(DYLD_LIBRARY_PATH)
else
   LD_LIBRARY_PATH := $(shell cd $(GADDIR)/gex; pwd):$(LD_LIBRARY_PATH)
endif

SUBDIRS = gex gsf data shape

#---------------------------  t a r g e t s   --------------------------------

% :
	@ t=$@; argv="$(SUBDIRS)" ;\
          for d in $$argv; do                    \
            ( cd $$d                            ;\
              echo -n "Making $$t for extension <$$d> ... ";\
              $(MAKE) -e  $$t ;\
              echo ok ) \
          done

install : force
	@/bin/mkdir -p $(bindir) $(gexdir)
	@ t=$@; argv="$(SUBDIRS)" ;\
          for d in $$argv; do                    \
            ( cd $$d                            ;\
              echo -n "Making $$t for extension <$$d> ... ";\
              $(MAKE) -e  $$t ;\
              echo ok ) \
          done
	@/bin/rm -rf $(gexdir)/udxt
	@/bin/cat $(gexdir)/*.udxt > $(gexdir)/udxt 

force : ;
