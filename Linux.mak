export INST_NAT_LIB

##############################################################################
# Explicit Rules

build: 
	python$(PYVER) setup.py build

install:
	python$(PYVER) setup.py install --prefix=$(PREFIX) --install-purelib=$(INST_HOST_LIB) \
 --install-platlib=$(INST_EXT_LIB) --install-scripts=$(INST_NAT_BIN)

clean:
	rm -r -f build
	
distclean:
	rm -r -f build

test:
	echo "Unit tests have not been defined"
