#
# Driver Makefile
#

.SILENT:

SHELL = /bin/bash
PYTHON = python3

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
GA2UDXT := $(GABDIR)/gex/udxt
GASCRP  := $(GABDIR)/gex/

ifeq ($(ARCH),Darwin)
   DYLD_LIBRARY_PATH := $(shell cd $(GADDIR)/gex; pwd):$(DYLD_LIBRARY_PATH)
else
   LD_LIBRARY_PATH := $(shell cd $(GADDIR)/gex; pwd):$(LD_LIBRARY_PATH)
endif

SUBDIRS = ams bjt fish hello ipc re shfilt gsf mf lats orb tle

#---------------------------  t a r g e t s   --------------------------------

% :
	@ t=$@; argv="$(SUBDIRS)" ;\
          for d in $$argv; do                    \
            ( cd $$d                            ;\
              echo -n "Making $$t for extension <$$d> ... ";\
              $(MAKE) -e  $$t ;\
              echo ok ) \
          done

# Note: it may be better to have a top ut.py for this
# thisbin : install
# 	$(GABDIR)/grads -bl

check : install
	/usr/bin/env PYTHONPATH=$(PYTHONPATH) \
	             GADSET=$(GADSET) \
	             GADDIR=$(GADDIR) \
	             GABDIR=$(GABDIR) \
	             GA2UDXT=$(GA2UDXT) \
	             GAUDPT=$(GAUDPT) \
	             GASCRP=$(GASCRP) \
	             LD_LIBRARY_PATH=$(LD_LIBRARY_PATH) \
	             $(PYTHON) ut.all.py 

bundle-check : install
	/usr/bin/env PYTHONPATH=$(PYTHONPATH) \
	             GADSET=$(GADSET) \
	             GABDIR=$(GABDIR) \
	             $(PYTHON) ut.all.py 

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
