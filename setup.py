#!/usr/bin/env python

import sys
import os
import os.path
from distutils.core import setup, Extension

sSpiceInc = os.getenv("CSPICE_INC")
if sSpiceInc == None:
	raise Exception("""Set the CSPICE_INC to point to your NAIF c_spice top level
include directory.  i.e. the one that contains SpiceUsr.h""")


sNatLibDir = os.getenv("INST_NAT_LIB")
if sNatLibDir == None:
	raise Exception("Set INST_NAT_LIB to the directory containing libcspice.so")

#sSpiceLib = os.getenv("CSPICE_LIB")
#if sSpiceLib == None:
#	raise Exception("""Set the CSPICE_LIB to point to your NAIF c_spice top library
#object.  i.e. the cspice.a file.""")

# Correct for another CSW packager error:
if sys.platform.lower().startswith('sunos'):
	# Solaris 8 setup, use this since spice is compiled for solaris 8

	os.environ['CC'] = '/opt/SUNWspro/bin/cc'
	os.environ['LDSHARED'] = '/opt/SUNWspro/bin/cc'

	setup(
		name="pspice",
		version="0.4",
		scripts=['scripts/spiceid'],
		ext_modules=[ 
			Extension(
				"pspice", 
				["src/pspice.c"],
				extra_objects=[sSpiceLib],
				include_dirs=[sSpiceInc],
				libraries = ["m"],
				extra_link_args = ['-G']
			) 
		], 
	   description="Python NAIF c_spice Wrapper",
		author="Chris Piker",
		author_email="chris-piker@uiowa.edu",
		url="https://saturn.physics.uiowa.edu/svn/util/python/trunk/pspice/"
	)

elif sys.platform.lower().startswith('win'):
	# Linux setup, may work on windows, who knows
	setup(
		name="pspice",
		version="0.4",
		scripts=['scripts/spiceid'],
		ext_modules=[ 
			Extension(
				"pspice", 
				["src/pspice.c"],
				extra_objects=[sSpiceLib],
				include_dirs=[sSpiceInc],
				libraries = ["m"],
				library_dirs = [ os.path.dirname(sys.executable) ]
			) 
		], 
	   description="Python NAIF c_spice Wrapper",
		author="Chris Piker",
		author_email="chris-piker@uiowa.edu",
		url="https://saturn.physics.uiowa.edu/svn/util/python/trunk/pspice/"
	)
	
else:
	# Linux setup, may work on windows, who knows
	setup(
		name="pspice",
		version="0.4",
		scripts=['scripts/spiceid'],
		ext_modules=[ 
			Extension(
				"pspice", 
				["src/pspice.c"],
				#extra_objects=[sSpiceLib],
				include_dirs=[sSpiceInc],
				library_dirs = [sNatLibDir],
				libraries = ["m", "cspice"]
			) 
		], 
	   description="Python NAIF c_spice Wrapper",
		author="Chris Piker",
		author_email="chris-piker@uiowa.edu",
		url="https://saturn.physics.uiowa.edu/svn/util/python/trunk/pspice/"
	)
