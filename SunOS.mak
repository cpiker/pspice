# Common Definitions #########################################################

# Pick a default install location, if user doesn't have one defiend
ifeq ($(PREFIX),)
PREFIX=$(HOME)
endif

ETC=$(PREFIX)/etc
DOC=$(PREFIX)/doc
BIN=$(PREFIX)/bin
SRC=$(PREFIX)/src
LIB=$(PREFIX)/lib

ifeq ($(PYVER),)
PYVER=$(shell python -c "import sys; print '.'.join( sys.version.split()[0].split('.')[:2] )")
endif

PYLIB=$(LIB)/python$(PYVER)

INC=$(PREFIX)/include
SHARE=$(PREFIX)/share

DATE=$(shell date "+%Y-%m-%d")
ARCH=$(shell uname -s).$(shell uname -m)

BUILD=build.$(ARCH)

##############################################################################
# Explicit Rules

build: 
	python$(PYVER) setup.py build

install:
	python$(PYVER) setup.py install --prefix=$(PREFIX) --install-purelib=$(PYLIB)  --install-platlib=$(PYLIB)

clean:
	rm -r -f build

test:
	echo "Unit tests have not been defined"
