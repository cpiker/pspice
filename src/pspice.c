/****************************************************************************/
/* Python wrapper for some of the spice lib, please extend as needed.       */
/*                                                                          */
/* Hopefully there is distutils setup.py and you don't have to compile and  */
/* and link by hand.  However if that is not the case, or setup.py is       */
/* broken,  instructions on building and installing this python extension   */
/* are included here.                                                       */
/*                                                                          */
/*    chris-piker@uiowa.edu 2006-10-5                                       */
/*                                                                          */
/* DOWNLOAD                                                                 */
/* ========                                                                 */
/* Get the NAIF cspice toolkit appropriate to your compiler and platform:   */
/*                                                                          */
/*     http://naif.jpl.nasa.gov/naif/toolkit.html                           */
/*                                                                          */
/* The cspice toolkit is now separate from the old fortran spice toolkit,   */
/* so make sure you get cspice.  These insturctions assume the cc compiler  */
/* is used, but others work as well.  Its best to use the same compiler to  */
/* build the extenstion as was used to build python.                        */
/*                                                                          */
/* BUILD                                                                    */
/* =====                                                                    */
/* To build this wrapper, you must know the python include directory and    */
/* the NAIF - cspice include directory.                                     */
/*                                                                          */
/* PYTHONINC example: /usr/local/include/python2.3                          */
/* CSPICEINC example: $HOME/cspice/include                                  */
/*                                                                          */
/* Then issue a command similar to:                                         */
/*                                                                          */
/*   cc -v -I$PYTHONINC -I$(CSPICEINC) -c -o spice.o spice.c                */
/*                                                                          */
/* To link this wrapper, you must know the python library directory and the */
/* NAIF - cspice lib directory.  If you have a static version of the python */
/* library, you will have to inform the linker on what libraries libpthon.a */
/* uses.  To find out what these are you can issue:                         */
/*                                                                          */
/*     ldd `which python`                                                   */
/*                                                                          */
/* The output from ldd can be INVALUABLE in determining the PYTHONLIB       */
/* setting below if you have to link against static python lib.             */
/*                                                                          */
/* PYTHONLIB (static): -L/usr/local/python2.3/config -lpython2.3.a \        */
/*         -lresolv -lsocket -lnsl -lrt -ldl -lpthread -lmp -laio -lpthread */
/*                                                                          */
/* PYTHONLIB (dynamic): -L/usr/local/lib -lpython2.3                        */
/*                                                                          */
/* CSPICELIB example (static): $(HOME)/cspice/lib/cspice.a                  */
/*                                                                          */
/* Then issue a command similar to:                                         */
/*                                                                          */
/*   gcc -Wall -shared spice.o $(CSPICELIB) $(PYTHONLIB) -lm -o spice.so    */
/*                                                                          */
/* INSTALL                                                                  */
/* =======                                                                  */
/* Copy spice.so to the python site-packages directory.  To make spice.so   */
/* part of a package, just copy it into a package directory.  For example   */
/* in our work site we have an "ipds" directory under site-packages and the */
/* shared object is copied to a directory such as:                          */
/*                                                                          */
/*    /usr/local/lib/python2.3/site-packages/ipds/spice.so                  */
/*                                                                          */
/* NOTE: python package directories always have a file named __init__.py,   */
/* even if __init__.py is a zero length file.  This is how python knows     */
/* that something is a package directory.  See "Distributing Python         */
/* Modules" in the python documentation (at http://www.python.org) for      */
/* more information on making python packages.                              */
/*                                                                          */
/* USAGE                                                                    */
/* =====                                                                    */
/* In your python script issue:                                             */
/*                                                                          */
/*   import spice                                                           */
/*   help(spice)                                                            */
/*                                                                          */
/* Or if you placed spice.so in the package directory ipds:                 */
/*                                                                          */
/*   import ipds.spice                                                      */
/*   help(ipds.spice)                                                       */
/*                                                                          */
/* NOTE: In the cspice toolkit most return values are pass using the        */
/* the function argument lists.  Since this is not at all pythonic(tm)      */
/* these wrappers return data as standard return values.  If more than one  */
/* return values is required a tuple is returned with all return items in   */
/* same order as they appear in the cspice function's argument list.  See   */
/* individual function help for more information.                           */
/*                                                                          */
/****************************************************************************/

#include <Python.h>

#include <SpiceUsr.h>
#include <numpy/arrayobject.h>


/*****************************************************************************/
/* Excerpt from SpiceZdf.h on types:                                         */
/*                                                                           */
/* The SPICE typedefs map in an arguably natural way to ANSI C types:        */
/*                                                                           */
/*    SpiceBoolean -> int                                                    */
/*    SpiceChar    -> char                                                   */
/*    SpiceDouble  -> double                                                 */
/*    SpiceInt     -> int or long                                            */
/*    ConstX       -> const X  (X = any of the above types)                  */
/*                                                                           */
/*****************************************************************************/


/*****************************************************************************/
/* Spice Error to exception bouncer */

static PyObject* g_pSpiceError;

static void* spice_SetException(void){
	int iNext;
	char sBuff[1841];
	
	getmsg_c("SHORT", 41, sBuff);
	iNext = strlen(sBuff);
	sBuff[iNext] = ' ';
	iNext++;
	getmsg_c("LONG", 1841-iNext , sBuff+iNext);
	
	PyErr_SetString(g_pSpiceError, sBuff);
	return NULL;
}

/*****************************************************************************/
/* reset */

char spicehelp_reset[] =
 "Reset the internal error status to a value of \"no error.\"  A call to reset\n"
 "is required if spice routines are to be used after a spice.error has been\n"
 "thrown.  Otherwise all spice calls will just throw exceptions.\n"
 "Returns: None"
;

static PyObject* spice_reset(PyObject* self, PyObject* args){
	
	if(!PyArg_ParseTuple(args, ":reset"))
		return NULL;
	
	reset_c();
	if( failed_c() ) return spice_SetException();
		
	Py_INCREF(Py_None);
	return Py_None;
}

/*****************************************************************************/
/* Version Info */

char spicehelp_tkvrsn[] = 
 "Given an item such as the Toolkit or an entry point name, return the\n"
 "latest version string.\n"
 "1 Argument:\n"
 "  string (optional) - Item for which a version string is desired.\n"
 "       Currently, the only item supported is 'toolkit' and it will\n"
 "       return the toolkit version number.  Any other item will return\n"
 "       'No version found.'\n" 
 "1 Return:\n"
 "  string - Version info string.\n"
;

static PyObject* spice_tkvrsn(PyObject* self, PyObject* args){
	char* sItem = "TOOLKIT";
	/*const char* sVer;*/
	
	if(!PyArg_ParseTuple(args, "|z:tkvrsn", &sItem))
		return NULL;

	return Py_BuildValue("s", tkvrsn_c(sItem));
}

/*****************************************************************************/
/* KERNEL HANDLING                                                           */
/*****************************************************************************/


/*****************************************************************************/
/* clpool */

char spicehelp_clpool[] =
 "Remove all variables from the kernel pool.\n"
 "   Returns: None"
;

static PyObject* spice_clpool(PyObject* self, PyObject* args){
	
	if(!PyArg_ParseTuple(args, ":clpool")) /* arg list should be null */
		return NULL;
		
	clpool_c();
	if (failed_c()) return spice_SetException();
	
	Py_INCREF(Py_None);  /* Return the one and only None */
	return Py_None;
}

/*****************************************************************************/
/* ldpool */

char spicehelp_ldpool[] = 
 "Load the variables contained in a NAIF ASCII kernel file into the kernel pool.\n"
 "1 Argument:\n"
 "   string - Name of the kernel file. \n"
 "Returns: None"
;

static PyObject* spice_ldpool(PyObject* self, PyObject* args){
	const char* sFile;
	
	if(!PyArg_ParseTuple(args, "s:ldpool", &sFile))
		return NULL;
	
	ldpool_c(sFile);
	if( failed_c() ) return spice_SetException();
		
	Py_INCREF(Py_None);
	return Py_None;
}

/*****************************************************************************/
/* furnsh */
char spicehelp_furnsh[] = 
 "Load a SPICE meta-kernel file into a program.\n"
 "1 Argument:\n"
 "   string - Name of the meta kernel file. \n"
 "Returns: None"
;

static PyObject* spice_furnsh(PyObject* self, PyObject* args){
	const char* sFile;
	
	if(!PyArg_ParseTuple(args, "s:furnsh", &sFile))
		return NULL;

	furnsh_c(sFile);
	if( failed_c() ) return spice_SetException();
	
	Py_INCREF(Py_None);
	return Py_None;
}

/*****************************************************************************/
/* unload */

char spicehelp_unload[] = 
 "Unload a particular kernel or all the kernels referenced in a meta-kernel.\n"
 "This function is the inverse of furnsh.\n"
 "1 Argument:\n"
 "   string - Name of the kernel or meta-kernel file.\n"
 "Returns: None"
;

static PyObject* spice_unload(PyObject* self, PyObject* args){
	const char* sFile;
	
	if(!PyArg_ParseTuple(args, "s:unload", &sFile))
		return NULL;

	unload_c(sFile);
	if( failed_c() ) return spice_SetException();
	
	Py_INCREF(Py_None);
	return Py_None;
}

char spicehelp_kclear[] = 
"Clear the KEEPER system:  unload all kernels, clear the kernel pool, and\n"
"re-initialize the system.\n"
"\n"
"This entry point allows you re-initialize the KEEPER system with a single\n"
"call.  The KEEPER system is the kernel management system underlying the set\n" 
"of CSPICE APIs:\n"
"\n"
"   furnsh\n"
"   ktotal\n"
"   kdata\n"
"   kinfo\n"
"   kclear\n"
"   unload\n"
"\n"
"This routine unloads all kernels from their kernel-type-specific kernel\n"
"management subsystems (SPKBSR, CKBSR, etc.), clears the kernel pool, clears\n"
"KEEPER's internal file database, and re-sets the watch status for the kernel\n"
"variables used to load kernels via meta-kernels.\n"
"\n"
"No Arguments\n:"
"\n"
"No returns - Operates by side effects, see notes above.\n"
;

static PyObject* spice_kclear(PyObject* self, PyObject* args)
{
	if(!PyArg_ParseTuple(args, ":kclear")) return NULL;
		
	kclear_c();
	if( failed_c() ) return spice_SetException();
	
	Py_INCREF(Py_None);
	return Py_None;
};	
				
/*****************************************************************************/
/* ktotal */

char spicehelp_ktotal[] = 
 "Get the current number of kernels that have been loaded of a specified type\n"
 "\n"
 "1 Argument:\n"
 "   string - Kind of kernels to count, a space separated string consisting\n"
 "      of one or more of the following words:\n"
 "      SPK  --- All SPK files are counted in the total.\n"
 "      CK   --- All CK files are counted in the total.\n"
 "      PCK  --- All binary PCK files are counted in the total.\n"
 "      EK   --- All EK files are counted in the total.\n"
 "      TEXT --- All text kernels that are not meta-text kernels are included\n"
 "               in the total.\n"
 "      META --- All meta-text kernels are counted in the total.\n"
 "      ALL  --- Every type of kernel is counted in the total.\n"
 "\n"
 "   exampe input value: 'SPK PCK CK'\n"
 "\n"
 "Returns: int"
;

static PyObject* spice_ktotal(PyObject* self, PyObject* args){
	const char* sKinds;
	SpiceInt nKerns;
	
	if(!PyArg_ParseTuple(args, "s:ktotal", &sKinds))
		return NULL;
	
	nKerns = 0;
	ktotal_c(sKinds, &nKerns);
	if( failed_c())
		return spice_SetException();
	else
#if(defined(CSPICE_ALPHA_DIGITAL_UNIX)||defined(CSPICE_SUN_SOLARIS_64BIT_GCC)||defined(CSPICE_PC_LINUX_64BIT_GCC))
		return Py_BuildValue("i", nKerns);	
#else
		return Py_BuildValue("l", nKerns);
#endif		
}

/*****************************************************************************/
/* kdata */

/*  FIGURE OUT A WAY TO AUTOMATICALLY ALLOCATE SUFFICENT MEMORY TO READ ALL
    OUTPUT FROM kdata_c AND kinfo_c BEFORE PROCEEDING ! */

/*
char spicehelp_kdata[] = 
 "Return data for the nth kernel that is among a list of specified 
 "   kernel types. 
 "

static PyObject* spice_kdata
*/


/*****************************************************************************/
/* bodvrd */

char spicehelp_bodvrd[] = 
 "Return float values from the kernel pool\n"
 "2 Manditory arguments:\n"
 "  string - name or id (as a string) for a body, not case sensitive\n"
 "  string - short name of a body value, such as 'RADII' not BODY599_RADII'\n"
 "1 Optional argument:\n"
 "  int - size of internal array for kernel values defaults to 26\n"
 "1 Return:\n"
 "  list - a python list of the returned variables\n"
;

static PyObject* spice_bodvrd(PyObject* self, PyObject* args){
	const char* sBod;
	const char* sVar;
	int nMaxVals = 26;
	int i;
	
	SpiceInt nRetVals;
	double* pdVals;
	PyObject* pList;

	if(!PyArg_ParseTuple(args, "ss|i:bodvrd", &sBod, &sVar, &nMaxVals))
		return NULL;

	pdVals = (double*)PyMem_Malloc(abs(nMaxVals) * sizeof(double) );
	
	bodvrd_c(sBod, sVar, nMaxVals, &nRetVals, pdVals);
	if( failed_c() ){
		PyMem_Free(pdVals);
		return spice_SetException();
	}
	
	/* Make a python list of the returned values */
	pList = PyList_New(nRetVals);
	for(i =0; i< nRetVals; i++){
		PyList_SetItem(pList, i, PyFloat_FromDouble(pdVals[i]));
	}
	
	PyMem_Free(pdVals);
	return pList;
}

/*****************************************************************************/
/* Body Names to NAIF codes                                                  */
/*****************************************************************************/

char spicehelp_bodn2c[] = 
 "Translate the name of a body or object to the corresponding SPICE integer\n"
 "ID code.\n"
 "1 Argument:\n"
 "  string - Body name to be translated into a SPICE ID code.\n"
 "1 Return:\n"
 "  int - SPICE integer ID code for the named body."
;

static PyObject* spice_bodn2c(PyObject* self, PyObject* args){
	int nFound;
	SpiceInt nID;
	const char* sName;
	char sErr[80];
	
	
	if(!PyArg_ParseTuple(args, "s:bodn2c", &sName))
		return NULL;
	
	bodn2c_c(sName, &nID, &nFound);
	if( failed_c() )
		return spice_SetException();
	
	if(nFound == SPICEFALSE){
		strcpy(sErr, "Unknown body name: ");
		strncat(sErr, sName, 57); 
		sErr[79] = '\0';
		PyErr_SetString(g_pSpiceError, sErr);
		return NULL;
	}
#if(defined(CSPICE_ALPHA_DIGITAL_UNIX)||defined(CSPICE_SUN_SOLARIS_64BIT_GCC)||defined(CSPICE_PC_LINUX_64BIT_GCC))
	return Py_BuildValue("i", nID);
#else
	return Py_BuildValue("l", nID);
#endif	
}


char spicehelp_bodc2n[] = 
 "Translate the SPICE integer code of a body into a common name for that body.\n"
 "1 Argument:\n"
 "  int - Integer ID code to be translated into a name.\n"
 "1 Return:\n"
 "  string - A common name for the body\n"
;

static PyObject* spice_bodc2n(PyObject* self, PyObject* args){
	int nFound;
	SpiceInt nID;
	char sErr[80];
	char sName[40];

#if(defined(CSPICE_ALPHA_DIGITAL_UNIX)||defined(CSPICE_SUN_SOLARIS_64BIT_GCC)||defined(CSPICE_PC_LINUX_64BIT_GCC))	
	if(!PyArg_ParseTuple(args, "i:bodc2n", &nID))
#else
	if(!PyArg_ParseTuple(args, "l:bodc2n", &nID))
#endif
	return NULL;
	
	bodc2n_c(nID, 40, sName, &nFound);
	if( failed_c() )
		return spice_SetException();
	
	if(nFound == SPICEFALSE){
#if(defined(CSPICE_ALPHA_DIGITAL_UNIX)||defined(CSPICE_SUN_SOLARIS_64BIT_GCC)||defined(CSPICE_PC_LINUX_64BIT_GCC))	
		snprintf(sErr, 80, "Unknown body id: %i ", nID);
#else
		snprintf(sErr, 80, "Unknown body id: %li ", nID);
#endif
		sErr[79] = '\0';
		PyErr_SetString(g_pSpiceError, sErr);
		return NULL;
	}
	
	return Py_BuildValue("s", sName);
}
		
/*****************************************************************************/
/* TIME FUNCTIONS                                                            */
/*****************************************************************************/

#include "help_time.c"

/* utc2et */
static PyObject* spice_utc2et(PyObject* self, PyObject* args){
	const char* sUtc;
	double dSec;
	
	if(!PyArg_ParseTuple(args, "s:utc2et", &sUtc))
		return NULL;
		
	dSec = 0;
	utc2et_c(sUtc, &dSec);
	if( failed_c() )
		return spice_SetException();
	else
		return Py_BuildValue("d", dSec);
}


/* str2et */
static PyObject* spice_str2et(PyObject* self, PyObject* args){
	const char* sTime = NULL;
	double dSec = 0.0;
	
	if(!PyArg_ParseTuple(args, "s:str2et", &sTime))
		return NULL;
	
	str2et_c(sTime, &dSec);
	if( failed_c() )
		return spice_SetException();
	else
		return Py_BuildValue("d", dSec);
}

/* sce2c */
static PyObject* spice_sce2c(PyObject* self, PyObject* args){
	SpiceInt sc;
	SpiceDouble et;
	SpiceDouble sclkdp;
	
	et = 0;
	
	if(!PyArg_ParseTuple(args, "id:sce2c", &sc, &et))
		return NULL;
		
	sce2c_c(sc, et, &sclkdp);
	if( failed_c() )
		return spice_SetException();
	else
		return Py_BuildValue("d", sclkdp);
}

/* scs2e */
static PyObject* spice_scs2e(PyObject* self, PyObject* args){
	int nCraft;
	const char* sSclk;
	double dSec;

	if(!PyArg_ParseTuple(args, "is:scs2e", &nCraft, &sSclk))
		return NULL;
	
	scs2e_c(nCraft, sSclk, &dSec);
	if( failed_c() )
		return spice_SetException();
	else
		return Py_BuildValue("f", dSec);
}


/* sce2s */
static PyObject* spice_sce2s(PyObject* self, PyObject* args){
	int nCraft;
	double dEphemeris;
	char sSclk[40];
	
	if(!PyArg_ParseTuple(args, "id:sce2s", &nCraft, &dEphemeris))
		return NULL;
		
	sce2s_c(nCraft, dEphemeris, 40, sSclk);
	if( failed_c() )
		return spice_SetException();
	else
		return Py_BuildValue("s", sSclk);
}


/* et2utc */
static PyObject* spice_et2utc(PyObject* self, PyObject* args){
	double et;
	const char* format;
	int prec;
	char utcstr[40];
	
	if(!PyArg_ParseTuple(args, "dsi:et2utc", &et, &format, &prec))
		return NULL;
	
	et2utc_c(et, format, prec, 40, utcstr);
	if( failed_c() )
		return spice_SetException();
	else
		return Py_BuildValue("s", utcstr);
}

/* etcal */
static PyObject* spice_etcal(PyObject* self, PyObject* args)
{
	double et;
	char sCal[52];
	

	if(!PyArg_ParseTuple(args, "d:etcal", &et))
		return NULL;
	
	etcal_c(et, 49, sCal);
	if( failed_c() )
		return spice_SetException();
	else
		return Py_BuildValue("s", sCal);
}
	
/* scencd */
static PyObject* spice_scencd(PyObject* self, PyObject* args){
	SpiceInt sc;
	ConstSpiceChar* sclkch;
	SpiceDouble sclkdp;
	
	if(!PyArg_ParseTuple(args, "is:scencd", &sc, &sclkch))
		return NULL;
		
	scencd_c(sc, sclkch, &sclkdp);
	if( failed_c() )
		return spice_SetException();
	else
		return Py_BuildValue("d", sclkdp);
}

/* sctiks */
static PyObject* spice_sctiks(PyObject* self, PyObject* args){
	SpiceInt sc;
	ConstSpiceChar* clkstr;
	SpiceDouble ticks;
	
	if(!PyArg_ParseTuple(args, "is:sctiks", &sc, &clkstr))
		return NULL;
		
	scencd_c(sc, clkstr, &ticks);
	if( failed_c() )
		return spice_SetException();
	else
		return Py_BuildValue("d", ticks);
}

/* deltet */
static PyObject* spice_deltet(PyObject* self, PyObject* args){
	SpiceDouble epoch;
	ConstSpiceChar* eptype;
	SpiceDouble delta;
   
	if(!PyArg_ParseTuple(args, "ds:deltet", &epoch, &eptype))
		return NULL;
	
	deltet_c(epoch, eptype, &delta);
	if( failed_c() )
		return spice_SetException();
	else
		return Py_BuildValue("d", delta);
}

/*****************************************************************************/
/* POSITION FUNCTIONS                                                        */
/*****************************************************************************/

#include "help_position.c"

static PyObject* spice_spkez(PyObject* self, PyObject* args){
	int targ;
	double et;
	const char* ref;
	const char* abcorr;
	int obs;
	double t[6];
	double lt;
	/*PyObject* pList;*/

	if(!PyArg_ParseTuple(args, "idssi:spkez", &targ, &et, &ref, &abcorr, &obs))
		return NULL;
	
	spkez_c(targ, et, ref, abcorr, obs, t, &lt);
	
	if( failed_c() )
		return spice_SetException();
	else
		return Py_BuildValue("(dddddd)d", t[0],t[1],t[2],t[3],t[4],t[5], lt);
}

static PyObject* spice_spkezr(PyObject* self, PyObject* args){
	const char* starg;
	double et;
	const char* ref;
	const char* abcorr;
	const char* sobs;
	double t[6];
	double lt;
	/*PyObject* pList;*/

	if(!PyArg_ParseTuple(args, "sdsss:spkezr", &starg, &et, &ref, &abcorr, &sobs))
		return NULL;
	
	spkezr_c(starg, et, ref, abcorr, sobs, t, &lt);
	
	if( failed_c() )
		return spice_SetException();
	else
		return Py_BuildValue("(dddddd)d", t[0],t[1],t[2],t[3],t[4],t[5], lt);
}

static PyObject* spice_spkpos(PyObject* self, PyObject* args){
	const char* targ;
	double      et;
	const char* frm;
	const char* corr;
	const char* obs;
	double      ptarg[3];
	double      lt;
	
	if(!PyArg_ParseTuple(args,"sdsss:spkpos", &targ, &et, &frm, &corr, &obs))
		return NULL;
	
	spkpos_c(targ, et, frm, corr, obs, ptarg, &lt);
	
	if( failed_c() )
		return spice_SetException();
	else
		return Py_BuildValue("(ddd)d", ptarg[0], ptarg[1], ptarg[2], lt);
}


/* recpgr - retangular to planetographic */
static PyObject* spice_recpgr(PyObject* self, PyObject* args){
	const char* sBod;
	
	double dIn[3];

	double dRequ;
	double dFlatting;
	
	double dLong;
	double dLat;
	double dAlt;
	
	if(!PyArg_ParseTuple(args, "s(ddd)dd:recpgr", &sBod, dIn, dIn+1, dIn+2,
	                     &dRequ, &dFlatting))
		return NULL;
	
	recpgr_c(sBod, dIn, dRequ, dFlatting, &dLong, &dLat, &dAlt);
	
	if( failed_c() )
		return spice_SetException();
	else
		return Py_BuildValue("(ddd)", dLong, dLat, dAlt);
}


/* reclat - rectangular to latitudinal coordinates */ 
static PyObject* spice_reclat(PyObject* self, PyObject* args){
	 double rIn[3];
	 double rOut[3];
	 
	 if(!PyArg_ParseTuple(args, "(ddd):reclat", rIn, rIn+1, rIn+2))
		 return NULL;
	 
	 reclat_c(rIn, rOut, rOut+1, rOut+2);
	
	if( failed_c() )
		return spice_SetException();
	else
		return Py_BuildValue("(ddd)", rOut[0], rOut[1], rOut[2]);
}

/*****************************************************************************/
/* GENERAL BODY FUNCTIONS                                                    */
/*****************************************************************************/

/*****************************************************************************/
/* lspcn - Planetocentric longitude of the sun */

char spicehelp_lspcn[] = 
"Compute L_s, the planetocentric longitude of the sun, as seen from a\n"
"specified body.\n"
"\n"
"3 ARGUMENTS\n"
"  body   (string)  Name of central body\n"
"  et     (float)   Epoch in seconds past J2000 TDB\n"
"  abcorr (string)  Aberration correction\n"
"\n"
"1 RETURN (float)\n"
"\n"
"  The function returns the planetocentric longitude of the sun, often called\n"
"  'L_s', for the specified body at the specified time. This is the longitude\n"
"  of the body-sun vector in a right-handed frame whose basis vectors are\n"
"  defined as follows:\n"
"\n"
"    - The positive Z direction is given by the instantaneous angular velocity\n"
"      vector of the orbit of the body about the sun. \n"
"\n"
"    - The positive X direction is that of the cross product of the \n"
"      instantaneous north spin axis of the body with the positive Z direction.\n"
"\n"
"    - The positive Y direction is Z x X.\n"
"\n"
"  Units are radians; the range is 0 to 2*pi.  Longitudes are positive to the\n"
"  east.\n"
"\n";

static PyObject* spice_lspcn(PyObject* self, PyObject* args)
{
	const char* sBody = NULL;
	SpiceDouble et = 0.0;
	const char* sAbCorr = NULL;	
	
	SpiceDouble dLong = 0.0;
	
	if(!PyArg_ParseTuple(args, "sds:lspcn", &sBody, &et, &sAbCorr))
		return NULL;
	
	dLong = lspcn_c(sBody, et, sAbCorr);
	
	if( failed_c() )
		return spice_SetException();
	else
		return Py_BuildValue("d", dLong);
}

/*****************************************************************************/
/* SURFACE POINT FUNCTIONS                                                   */
/*****************************************************************************/

#include "help_surface.c"

/*****************************************************************************/
/* et2lst - Local Solar Time of a surface point */

static PyObject* spice_et2lst(PyObject* self, PyObject* args){

	SpiceDouble et;
	SpiceInt body;
	SpiceDouble lon;
	const char* type;
	
	SpiceInt  hour, min, sec;
	SpiceChar time[51];	
	SpiceChar ampm[51];
	
	if(!PyArg_ParseTuple(args, "dids:et2lst", &et, &body, &lon, &type))
		return NULL;
	
	et2lst_c(et, body, lon, type, 51, 51, &hour, &min, &sec, time, ampm);
	
	if( failed_c() )
		return spice_SetException();
	else
		return Py_BuildValue("(iiiss)", hour, min, sec, time, ampm);
}

/*****************************************************************************/
/* subslr - Sub-solar point calculation */

static PyObject* spice_subslr(PyObject* self, PyObject* args)
{
	/* Input */
	const char* sMethod = NULL;
	const char* sTarg = NULL;
	SpiceDouble et = 0.0;
	const char* sFixRef = NULL;
	const char* sAbCorr = NULL;
	const char* sObserver = NULL;
	
	/* Output */
	SpiceDouble pLocSlrPt[3] = {0.0};
	SpiceDouble etSlrPt = 0.0;
	SpiceDouble pObs2SlrPtVec[3] = {0.0};
	
	if(!PyArg_ParseTuple(args, "ssdsss:subslr", &sMethod, &sTarg, &et, 
			               &sFixRef, &sAbCorr, &sObserver))
		return NULL;
	
	subslr_c(sMethod, sTarg, et, sFixRef, sAbCorr, sObserver,
			   pLocSlrPt, &etSlrPt, pObs2SlrPtVec);
	
	if( failed_c() )
		return spice_SetException();
	
	return Py_BuildValue("(ddd)d(ddd)",
			               pLocSlrPt[0], pLocSlrPt[1], pLocSlrPt[2],
			               etSlrPt, 
			               pObs2SlrPtVec[0], pObs2SlrPtVec[1], pObs2SlrPtVec[2]);
}

/*****************************************************************************/
/* subpnt - Sub-observer point calculation */

static PyObject* spice_subpnt(PyObject* self, PyObject* args){

	/* Input */
	const char* sMethod = NULL;
	const char* sTarg = NULL;
	SpiceDouble et = 0.0;
	const char* sFixRef = NULL;
	const char* sAbCorr = NULL;
	const char* sObserver = NULL;
	
	/* Output */
	SpiceDouble pLocSubPt[3] = {0.0};
	SpiceDouble etSubPt = 0.0;
	SpiceDouble pObs2SubPtVec[3] = {0.0};
	
	if(!PyArg_ParseTuple(args, "ssdsss:subpnt", &sMethod, &sTarg, &et, 
			               &sFixRef, &sAbCorr, &sObserver))
		return NULL;
	
	subpnt_c(sMethod, sTarg, et, sFixRef, sAbCorr, sObserver,
			   pLocSubPt, &etSubPt, pObs2SubPtVec);
	
	if( failed_c() )
		return spice_SetException();
	
	return Py_BuildValue("(ddd)d(ddd)",
			               pLocSubPt[0], pLocSubPt[1], pLocSubPt[2],
			               etSubPt, 
			               pObs2SubPtVec[0], pObs2SubPtVec[1], pObs2SubPtVec[2]);
}

static PyObject* spice_ilumin(PyObject* self, PyObject* args)
{
	/* Input */
	const char* sMethod = NULL;
	const char* sTarget = NULL;
	double      rEt = 0.0;
	const char* sFixRef = NULL;
	const char* sAbCorr = NULL;
	const char* sObserver = NULL;
	double      spoint[3] = {0.0};
	
	/* output */
	double rTrgEpc = 0.0;
	double rObs2SurfPt[3] = {0.0};
	double rPhase = 0.0, rSolar = 0.0, rEmissn = 0.0;
	
	if(!PyArg_ParseTuple(args, "ssdsss(ddd):subpnt", &sMethod, &sTarget, 
			               &rEt, &sFixRef, &sAbCorr, &sObserver, spoint, 
			               spoint + 1, spoint + 2))
		return NULL;
	
	
	ilumin_c(sMethod, sTarget, rEt, sFixRef, sAbCorr, sObserver, spoint,
	         &rTrgEpc, rObs2SurfPt, &rPhase, &rSolar, &rEmissn);
	
	if( failed_c() )
		return spice_SetException();
	
	return Py_BuildValue("d(ddd)ddd", rTrgEpc, rObs2SurfPt[0], rObs2SurfPt[0],
			               rObs2SurfPt[0], rPhase, rSolar, rEmissn);
}

/*****************************************************************************/
/* ROTATION FUNCTIONS                                                        */
/*****************************************************************************/

#include "help_rotations.c"

/* rotation matrix from axis and angle */
static PyObject* spice_axisar(PyObject* self, PyObject* args)
{
	PyObject* ppoAxis = NULL;
	double rAngle = 0.0;
	PyObject* pnaAxis = NULL;
	double* pAxis = NULL;
	double  axisBuf[3];
	double  pMatrix[3][3] = {{0.0}};
	PyObject* pnaMatrix = NULL;
	int i;
	
	if(!PyArg_ParseTuple(args, "Od:axisar", &ppoAxis, &rAngle)) return NULL;
	
	if( (pnaAxis = PyArray_FROM_OTF(ppoAxis, NPY_DOUBLE, NPY_IN_ARRAY)) == NULL)
		return NULL;

	/* Try for simple 1-Dim 3-element objects */
	if(PyArray_NDIM(pnaAxis) == 1){
		if(PyArray_DIM(pnaAxis, 0) == 3) pAxis = (double*)PyArray_DATA(pnaAxis);
	}
	else{
		/* Handle a 3-element matrix */
		if(PyArray_NDIM(pnaAxis) == 2){
			/* Row vector */
			if((PyArray_DIM(pnaAxis, 0) == 1) && (PyArray_DIM(pnaAxis, 1) == 3)){
				for(i = 0; i<3; ++i)
					axisBuf[i] = *((double*) PyArray_GETPTR2(pnaAxis, 0, i) ); 
				pAxis = axisBuf;
			}
		
			/* Column vector */
			if((PyArray_DIM(pnaAxis, 0) == 3) && (PyArray_DIM(pnaAxis, 1) == 1)){
				for(i = 0; i<3; ++i)
					axisBuf[i] = *((double*) PyArray_GETPTR2(pnaAxis, i, 0) );
				pAxis = axisBuf;
			}
		}
	}
	
	if(pAxis == NULL){
		PyErr_SetString(PyExc_ValueError, "axis argument must have 3, and only 3, components");
		Py_DECREF(pnaAxis);
		return NULL;	
	}
	
	
	axisar_c(pAxis, rAngle, pMatrix);
	Py_DECREF(pnaAxis);
	
	if( failed_c() ) return spice_SetException();
	
	npy_intp dims[2] = {3,3};
	
	pnaMatrix = (PyObject*)PyArray_SimpleNew(2, dims, NPY_DOUBLE);
	
	memcpy(PyArray_DATA(pnaMatrix), pMatrix, 9*sizeof(double));	
	return pnaMatrix;
}

/* Rotation matrix to quaternion */

static PyObject* spice_m2q(PyObject* self, PyObject* args){
	PyObject* ppoRot = NULL;
	PyObject* pnaRot = NULL;
	double* pRot = NULL;
	double aQuat[4] = {0.0};
	PyObject* pnaQuat = NULL;
		
	if(!PyArg_ParseTuple(args, "O:m2q", &ppoRot)) return NULL;
	
	/* make sure the thing in 3X3 */
	if( (pnaRot = PyArray_FROM_OTF(ppoRot, NPY_DOUBLE, NPY_IN_ARRAY)) == NULL)
		return NULL;
	
	if((PyArray_NDIM(pnaRot) != 2)||(PyArray_DIM(pnaRot, 0) != 3)||
		(PyArray_DIM(pnaRot, 0) != 3)){
		PyErr_SetString(PyExc_ValueError, "rotation argument shape is not 3 x 3.");
		Py_DECREF(pnaRot);
		return NULL;	
	}
	
	pRot = (double*)PyArray_DATA(pnaRot);
	
	m2q_c(pRot, aQuat);
	Py_DECREF(pnaRot);
	
	if( failed_c() ) return spice_SetException();
	
	npy_intp dims = 4;
	pnaQuat = (PyObject*)PyArray_SimpleNew(1, &dims, NPY_DOUBLE);
	memcpy(PyArray_DATA(pnaQuat), aQuat, 4*sizeof(double));
	
	return pnaQuat;
}	

/* pxfrom - Get rotation matrix for translating between frames at some instant */

static PyObject* spice_pxform(PyObject* self, PyObject* args)
{
	/* Input */
	const char* sFrom = NULL;     /* input */
	const char*   sTo = NULL;
	double        rEt = 0.0;
	
	double  rot[3][3] = {{0.0}};  /* buffer */
	 
	PyObject* pnaRot  = NULL;     /* output */
	
	if(!PyArg_ParseTuple(args, "ssd:pxfrom", &sFrom, &sTo, &rEt)) return NULL;
		
	pxform_c(sFrom, sTo, rEt, rot);
	if( failed_c() ) return spice_SetException();
	
	npy_intp dims[2] = {3,3};
	pnaRot = (PyObject*)PyArray_SimpleNew(2, dims, NPY_DOUBLE);
	
	memcpy(PyArray_DATA(pnaRot), rot, 9*sizeof(double));	
	return pnaRot;
}

/* ckgp - get pointing (attitude) for a specified sclk */

static PyObject* spice_ckgp(PyObject* self, PyObject* args)
{
	SpiceInt inst;
	SpiceDouble sclkdp, tol;
	const char* ref;
	
	SpiceDouble cmat[3][3];
	SpiceDouble clkout;
	SpiceBoolean found;

#if(defined(CSPICE_ALPHA_DIGITAL_UNIX)||defined(CSPICE_SUN_SOLARIS_64BIT_GCC)||defined(CSPICE_PC_LINUX_64BIT_GCC))	
	if(!PyArg_ParseTuple(args, "idds:ckgp", &inst, &sclkdp, &tol, &ref))
#else
	if(!PyArg_ParseTuple(args, "ldds:ckgp", &inst, &sclkdp, &tol, &ref))
#endif
		return NULL;
		
	ckgp_c(inst, sclkdp, tol, ref, cmat, &clkout, &found);
	
	if( failed_c() )
		return spice_SetException();

	if(found == SPICEFALSE){
		Py_INCREF(Py_None);
		return Py_None;
	}
	
	return Py_BuildValue("( ((ddd),(ddd),(ddd)), d)", 
	                     cmat[0][0], cmat[0][1], cmat[0][2],
	                     cmat[1][0], cmat[1][1], cmat[1][2],
	                     cmat[2][0], cmat[2][1], cmat[2][2],
	                     clkout);
}

/* ckgp_T - get transpose of pointing (attitude) special pspice add on */

char spicehelp_ckgp_T[] = 
 "Same as ckgp, except cmat is transposed prior to returning.\n";

static PyObject* spice_ckgp_T(PyObject* self, PyObject* args){
	SpiceInt inst;
	SpiceDouble sclkdp, tol;
	const char* ref;
	
	SpiceDouble cmat[3][3];
	SpiceDouble clkout;
	SpiceBoolean found;

#if(defined(CSPICE_ALPHA_DIGITAL_UNIX)||defined(CSPICE_SUN_SOLARIS_64BIT_GCC)||defined(CSPICE_PC_LINUX_64BIT_GCC))	
	if(!PyArg_ParseTuple(args, "idds:ckgp_T", &inst, &sclkdp, &tol, &ref))
#else
	if(!PyArg_ParseTuple(args, "ldds:ckgp_T", &inst, &sclkdp, &tol, &ref))
#endif
		return NULL;
		
	ckgp_c(inst, sclkdp, tol, ref, cmat, &clkout, &found);
	
	if( failed_c() )
		return spice_SetException();

	if(found == SPICEFALSE){
		Py_INCREF(Py_None);
		return Py_None;
	}
	
	return Py_BuildValue("( ((ddd),(ddd),(ddd)), d)", 
	                     cmat[0][0], cmat[1][0], cmat[2][0],
	                     cmat[0][1], cmat[1][1], cmat[2][1],
	                     cmat[0][2], cmat[1][2], cmat[2][2],
	                     clkout);
}



/*****************************************************************************/
/* UTILITY FUNCTIONS                                                         */
/*****************************************************************************/

char spicehelp_vnorm[] = 
"Compute the magnitude of a double precision, 3-dimensional vector\n"
"1 Argument:\n"
"  3-tuple -- A double precision vector\n"
"1 Return:\n"
"  The function returns the magnitude of v1 calculated in a numerically\n"
"  stable way.\n"
"\n";

static PyObject* spice_vnorm(PyObject* self, PyObject* args){
	ConstSpiceDouble vIn[3];
	SpiceDouble dNorm = 0.0;
	
	if(!PyArg_ParseTuple(args, "(ddd):vnorm", &(vIn[0]), vIn + 1, vIn + 2))
		return NULL;
	
	dNorm = vnorm_c(vIn);
	
	if( failed_c() )
		return spice_SetException();
	else
		return Py_BuildValue("d", dNorm);
}

char spicehelp_vminus[] = 
 "Negate a double precision 3-dimensional vector.\n"
 "\n"
 "1 Argument:\n"
 "   3-tuple - A position vector consiting of three double precision values\n"
 "\n"
 "1 Return:\n"
 "   3-tuple - This will be the negation (additive inverse) of V1.  It is a\n"
 "      is a 3-dimensional, double precision tuple.\n";

static PyObject* spice_vminus(PyObject* self, PyObject* args){
	double vIn[3];
	double vOut[3];
	
	if(!PyArg_ParseTuple(args, "(ddd):vminus", &(vIn[0]), vIn + 1, vIn + 2))
		return NULL;
	
	vminus_c(vIn, vOut);
	
	if( failed_c() )
		return spice_SetException();
	else
		return Py_BuildValue("(ddd)", vOut[0],vOut[1],vOut[2]);
}

char spicehelp_dpr[] = 
"Get the number of degrees per radian.\n"
"No Arguments\n"
"1 Return:\n"
"  180.0/pi, where pi is determined by the acos function.  That is,\n"
"\n"
"     dpr = 180.0 / acos ( -1.0 )\n"
"\n";

static PyObject* spice_dpr(PyObject* self, PyObject* args){
	double dDpr;
	
	dDpr = dpr_c();
	
	if( failed_c() )
		return spice_SetException();
	else
		return Py_BuildValue("d", dDpr);
}

/*****************************************************************************/
/* The method defintions */

static PyMethodDef spice_methods[] = {
	
	/* Kernel functions */
	{"tkvrsn", spice_tkvrsn, METH_VARARGS, spicehelp_tkvrsn },
	{"clpool", spice_clpool, METH_VARARGS, spicehelp_clpool },
	{"reset",  spice_reset,  METH_VARARGS, spicehelp_reset  },
	{"ldpool", spice_ldpool, METH_VARARGS, spicehelp_ldpool },
	{"furnsh", spice_furnsh, METH_VARARGS, spicehelp_furnsh },
	{"unload", spice_unload, METH_VARARGS, spicehelp_unload },
	{"ktotal", spice_ktotal, METH_VARARGS, spicehelp_ktotal },
	{"kclear", spice_kclear, METH_VARARGS, spicehelp_kclear },
	
	{"bodvrd", spice_bodvrd, METH_VARARGS, spicehelp_bodvrd },
	{"bodn2c", spice_bodn2c, METH_VARARGS, spicehelp_bodn2c },
	{"bodc2n", spice_bodc2n, METH_VARARGS, spicehelp_bodc2n },
	
	/* Time Functions */
	{"utc2et", spice_utc2et, METH_VARARGS, spicehelp_utc2et },
	{"str2et", spice_str2et, METH_VARARGS, spicehelp_str2et },
	{"et2utc", spice_et2utc, METH_VARARGS, spicehelp_et2utc },
	{"etcal",  spice_etcal,  METH_VARARGS, spicehelp_etcal  },
	{"sce2s",  spice_sce2s,  METH_VARARGS, spicehelp_sce2s  },
	{"scs2e",  spice_scs2e,  METH_VARARGS, spicehelp_scs2e  },
	{"sce2c",  spice_sce2c,  METH_VARARGS, spicehelp_sce2c  },
	{"scencd", spice_scencd, METH_VARARGS, spicehelp_scencd },
	{"sctiks", spice_sctiks, METH_VARARGS, spicehelp_sctiks },
	{"deltet", spice_deltet, METH_VARARGS, spicehelp_deltet },
	
	/* Poistion Functions */
	
	{"spkpos", spice_spkpos, METH_VARARGS, spicehelp_spkpos },
	{"spkez",  spice_spkez,  METH_VARARGS, spicehelp_spkez  },
	{"spkezr", spice_spkezr, METH_VARARGS, spicehelp_spkezr },
	{"recpgr", spice_recpgr, METH_VARARGS, spicehelp_recpgr },
	{"reclat", spice_reclat, METH_VARARGS, spicehelp_reclat },
	{"lspcn",  spice_lspcn,  METH_VARARGS, spicehelp_lspcn  },
	{"et2lst", spice_et2lst, METH_VARARGS, spicehelp_et2lst },
	{"subpnt", spice_subpnt, METH_VARARGS, spicehelp_subpnt },
	{"subslr", spice_subslr, METH_VARARGS, spicehelp_subslr },
	{"ilumin", spice_ilumin, METH_VARARGS, spicehelp_ilumin },
	
	/* Rotation Functions */
	{"axisar", spice_axisar, METH_VARARGS, spicehelp_axisar },
	{"m2q",    spice_m2q,    METH_VARARGS, spicehelp_m2q    },
	{"pxform", spice_pxform, METH_VARARGS, spicehelp_pxform },
	{"ckgp",   spice_ckgp,   METH_VARARGS, spicehelp_ckgp   },
	{"ckgp_T", spice_ckgp_T, METH_VARARGS, spicehelp_ckgp_T },
	
	{"vminus", spice_vminus, METH_VARARGS, spicehelp_vminus },
	{"vnorm",  spice_vnorm,  METH_VARARGS, spicehelp_vnorm  },
	{"dpr",    spice_dpr,    METH_NOARGS,  spicehelp_dpr    },
	{NULL, NULL, 0, NULL}
};

/*****************************************************************************/
/* Module initialization */

PyMODINIT_FUNC initpspice(void){
	
	PyObject* pMod;
	
	/* This statement is required to setup the numpy C API.  If you leave it
	 * out you WILL get SEGFAULTS! */
	import_array();

	pMod = Py_InitModule3("pspice", spice_methods,
	"Python wrappers for NAIF spice toolkit: http://naif.jpl.nasa.gov/naif/\n"
	"Cobbled together by chris-piker@uiowa.edu.  These are not offical, so\n"
	"please DON'T BUG JPL for python wrapper info!\n\n"
	"Note:  Unlike the native spice toolkit, NONE of these functions modify\n"
	"there input values, instead all results are returned as values.  Tuples\n"
	"or arrays are used to return multiple values if need be.");
	
	g_pSpiceError = PyErr_NewException("pspice.error", PyExc_StandardError, NULL);
	Py_INCREF(g_pSpiceError);
	PyModule_AddObject(pMod, "error", g_pSpiceError);
	
	/* Set spice's error handling to not output anything, our special
	exception object handles passing the info around.*/
	
	errprt_c("SET", 0, "ALL");     /* Want all error text */
	erract_c("SET", 0, "RETURN");  /* Just return on error, we'll check for it */
	errdev_c("SET", 0, "NULL");    /* Don't go printing to the screen on error */
	
}
