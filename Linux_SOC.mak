# Common Definitions #########################################################

# Pick a default install location, if user doesn't have one defiend
ifeq ($(PREFIX),)
PREFIX=/usr/local
endif

ETC=$(PREFIX)/etc
DOC=$(PREFIX)/doc
BIN=$(PREFIX)/bin
SRC=$(PREFIX)/src

ifeq ($(shell uname -m),x86_64)
LIB=$(PREFIX)/lib64
else
LIB=$(PREFIX)/lib
endif

PYVER=$(shell python -c "import sys; print '.'.join( sys.version.split()[0].split('.')[:2] )")


PYLIB=$(LIB)/python$(PYVER)

INC=$(PREFIX)/include
SHARE=$(PREFIX)/share

DATE=$(shell date "+%Y-%m-%d")
ARCH=$(shell uname -s).$(shell uname -m)

BUILD=build.$(ARCH)

##############################################################################
# Explicit Rules

build: 
	python setup_soc.py build

install:
	python setup_soc.py install --prefix=$(PREFIX) --install-purelib=$(PYLIB)  --install-platlib=$(PYLIB)

clean:
	rm -r -f build

test:
	echo "Unit tests have not been defined"
