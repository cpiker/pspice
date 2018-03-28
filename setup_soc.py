#!/usr/bin/env python

import sys
import os
import os.path
from distutils.core import setup, Extension

#sSpiceDir = os.getenv("CSPICE_DIR")
#if sSpiceDir == None:
#	raise Exception("""Set the CSPICE_DIR to point to your NAIF c_spice top level
#installation directory.  i.e. the one that contains include/ and lib/.""")

# Hacked up version since Dan split up the cspice stuff instead of keeping
# it under one directory.


# Linux setup, may work on windows, who knows
setup(
	name="pspice",
	version="0.2",
	scripts=['scripts/spiceid'],
	ext_modules=[ 
		Extension(
			"pspice", 
			["src/pspice.c"],
			extra_objects=['/usr/local/lib/cspice.a'],
			include_dirs=['/usr/local/include/cspice'],
		) 
	], 
   description="Python NAIF c_spice Wrapper",
	author="Chris Piker",
	author_email="chris-piker@uiowa.edu",
	url="https://saturn.physics.uiowa.edu/svn/util/python/trunk/pspice/"
)
