/* spice help on position functions */

char spicehelp_spkpos[] = 
"Return the position of a target body relative to an observing body, optionally\n"
"corrected for light time (planetary aberration) and stellar aberration.\n"
"\n"
"The target and observer define a position vector which points from the\n"
"observer to the target.  The vminus() function may be used to reverse to\n"
"position vector\n"
"\n" 
"5 Arguments:\n"
"\n"
"   target (string) - the name of a target body.  Optionally, you may supply\n"
"      the integer ID code for the object as a string.  For example both \"MOON\"\n"
"      and \"301\" are legitimate strings that indicate the moon is the target\n"
"      body.\n"
"\n"
"   et (float) - the ephemeris time, expressed as seconds past J2000 TDB, at\n"
"      which the position of the target body relative to the observer is to be\n"
"      computed.  'et' refers to time at the observer's location.\n"
"\n"
"   frame (string) - the name of the reference frame relative to which the output\n"
"      position vector should be expressed. This may be any frame supported by\n"
"      the SPICE system, including built-in frames (documented in the Frames\n"
"      Required Reading) and frames defined by a loaded frame kernel (FK).\n"
"\n"
"      When 'frame' designates a non-inertial frame, the orientation of the frame\n"
"      is evaluated at an epoch dependent on the selected aberration correction.\n"
"      See the description of the output position vector 'postarg' for details.\n"
"\n"
"   abcorr (string) - indicates the aberration corrections to be applied to the\n" 
"      position of the target body to account for one-way light time and stellar\n"
"      aberration.  See the discussion in the NAIF doucmentation for the\n"
"      spkpos_c() function for recommendations on how to choose aberration\n"
"      corrections.\n"
"\n"
"      Neither special nor general relativistic effects are accounted for in the\n"
"      aberration corrections applied by this routine.\n"
"\n"
"      Case and blanks are not significant in the string 'abcorr'.\n"
"\n"
"      'abcorr' may be any of the following:\n"
"\n"
"         \"NONE\" - Apply no correction. Return the  geometric position of the\n"
"            target body relative to the observer.\n"
"\n"
"      The following values of 'abcorr' apply to the \"reception\" case in which\n"
"      photons depart from the target's location at the light-time corrected\n"
"      epoch: (et - lt) and *arrive* at the observer's location at 'et':\n"
"\n"
"         \"LT\" - Correct for one-way light time (also called \"planetary\n"
"           aberration\") using a Newtonian formulation. This correction yields\n"
"           the position of the target at the moment it emitted photons arriving\n"
"           at the observer at `et'.\n"
"\n"
"           The light time correction uses an iterative solution of the light\n"
"           time equation (see Particulars for details). The solution invoked by\n"
"           the \"LT\" option uses one iteration.\n"
"\n"
"        \"LT+S\" - Correct for one-way light time and stellar aberration using a\n"
"           Newtonian formulation. This option modifies the position obtained with\n"
"           the \"LT\" option to account for the observer's velocity relative to\n"
"           the solar system barycenter. The result is the apparent position of\n"
"           the target---the position as seen by the observer.\n"
"\n"
"        \"CN\" - Converged Newtonian light time correction.  In solving the light\n"
"           time equation, the \"CN\" correction iterates until the solution\n"
"           converges (three iterations on all supported platforms).\n"
"\n"
"           The \"CN\" correction typically does not substantially improve accuracy\n"
"           because the errors made by ignoring relativistic effects may be larger\n"
"           than the improvement afforded by obtaining convergence of the light\n"
"           time solution. The \"CN\" correction computation also requires a\n"
"           significantly greater number of CPU cycles than does the one-\n"
"           iteration light time correction.\n"
"\n"
"        \"CN+S\" - Converged Newtonian light time and stellar aberration\n"
"           corrections.\n"
"\n"
"     The following values of 'abcorr' apply to the \"transmission\" case in which\n"
"     photons *depart* from the observer's location at 'et' and arrive at the\n"
"     target's location at the light-time corrected epoch (et + lt):\n"
"\n"
"        \"XLT\" - \"Transmission\" case:  correct for one-way light time using a\n"
"           Newtonian formulation. This correction yields the position of the\n"
"           target at the moment it receives photons emitted from the observer's\n"
"           location at 'et'.\n"
"\n"
"        \"XLT+S\" - \"Transmission\" case:  correct for one-way light time and\n"
"           stellar aberration using a Newtonian formulation.  This option\n"
"           modifies the position obtained with the \"XLT\" option to account for\n"
"           the observer's velocity relative to the solar system barycenter. The\n"
"           computed target position indicates the direction that photons emitted\n"
"           from the observer's location must be \"aimed\" to hit the target.\n"
"\n"
"        \"XCN\" - \"Transmission\" case:  converged Newtonian light time\n"
"           correction.\n"
"\n"
"        \"XCN+S\" - \"Transmission\" case: converged Newtonian light time and\n"
"           stellar aberration corrections.\n"
"\n"
"   observer (string) - the name of an observing body.  Optionally, you may\n"
"      supply the ID code of the object as an integer string. For example, both\n"
"      \"EARTH\" and \"399\" are legitimate strings to supply to indicate the\n"
"      observer is Earth.\n"
"\n"
"2 Returns:\n"
"\n"
"   postarg (3-tuple, floats) - a Cartesian 3-vector representing the position of\n"
"      the target body relative to the specified observer. 'postarg' is corrected\n"
"      for the specified aberrations, and is expressed with respect to the\n"
"      reference frame specified by 'frame'.  The three components of 'postarg'\n"
"      represent the x-, y- and z-components of the target's position.\n"
"\n"
"      Units are always km.\n"
"\n"
"      'postarg' points from the observer's location at 'et' to the aberration-\n"
"      corrected location of the target. Note that the sense of this position\n"
"      vector is independent of the direction of radiation travel implied by the\n"
"      aberration correction.\n"
"\n"
"      Non-inertial frames are treated as follows: letting 'ltcent' be the one-\n"
"      way light time between the observer and the central body associated with\n"
"      the frame, the orientation of the frame is evaluated at (et - ltcent),\n"
"      (et + ltcent), or 'et' depending on whether the requested aberration\n"
"      correction is, respectively, for received radiation, transmitted\n"
"      radiation, or is omitted.  'ltcent' is computed using the method indicated\n"
"      by 'abcorr'.\n"
"\n"
"  lt (float) - the one-way light time between the observer and target in\n"
"       seconds.  If the target position is corrected for aberrations, then 'lt'\n"
"       is the one-way light time between the observer and the light time\n"
"       corrected target location.\n"
;


char spicehelp_spkez[] = "\n\
NOTE: See Expanded help in spice docs for more information.\n\
\n\
Return the state (position and velocity) of a target body \n\
relative to an observing body, optionally corrected for light \n\
time (planetary aberration) and stellar aberration. \n\
\n\
5 Arguments:\n\
\n\
   int     the NAIF ID code for a target body.  The target and observer\n\
           define a state vector whose position component points from the\n\
           observer to the target.\n\
\n\
   float   the ephemeris time at observers location, expressed as\n\
           seconds past J2000 \n\
\n\
   string  the name of the reference frame relative in which the output state\n\
           vector should be expressed.  Use J2000 to get an inertial frame.\n\
\n\
   string  indicates the aberration corrections to be applied to the state of\n\
           the target body to account for one-way light time and stellar\n\
           aberration, May be any of the following: 'NONE', 'LT', 'LT+S'\n\
           'CN', 'CN+S', 'XLT', 'XLT+S', 'XCN', or 'XCN+S'.\n\
\n\
   int     the NAIF ID code for an observing body. \n\
\n\
2 RETURNS:\n\
\n\
   array(6) a Cartesian state vector representing the position and velocity of\n\
           the target body relative to the specified observer. The first \n\
           three components of `starg' represent the x-, y- and z-components\n\
           of the target's position; the last three components form the\n\
           corresponding velocity vector. \n\
\n\
           Units are always km and km/sec.\n\
\n\
   float   is the one-way light time between the observer and target in\n\
           seconds. If the target state is corrected for aberrations, then\n\
           this is the one-way light time between the observer and the light\n\
           time corrected target location.\n";

char spicehelp_spkezr[] = 
"Return the state (position and velocity) of a target body relative to an\n"
"observing body, optionally corrected for light time (planetary aberration)\n"
"and stellar aberration.\n"
"\n"
"This function is essentially the same as spkez, except that the target and\n"
"observer may be specified as strings instead of integers id codes\n"
"\n"
"5 ARGUMENTS\n"
"  string  The name of a target body. Optionally, you may supply the integer\n"
"          ID code for the object as an integer string. For example both \n"
"          'MOON' and '301' are legitimate strings that indicate the moon is\n"
"          the target body.\n"
"\n"
"  float   the ephemeris time at observers location, expressed as\n"
"          seconds past J2000 \n"
"\n"
"  string  the name of the reference frame relative in which the output state\n"
"          vector should be expressed.  Use J2000 to get an inertial frame.\n"
"\n"
"  string  indicates the aberration corrections to be applied to the state of\n"
"          the target body to account for one-way light time and stellar\n"
"          aberration, May be any of the following: 'NONE', 'LT', 'LT+S'\n"
"          'CN', 'CN+S', 'XLT', 'XLT+S', 'XCN', or 'XCN+S'.\n"
"\n"
"  string  The name of the observing body, for exmaple 'Mars Express', may\n"
"          specify a NAIF id as as string.\n"
"\n";

char spicehelp_recpgr[] =
 "Convert rectangular coordinates to planetographic coordinates.\n"
 "\n"
 "4 Arguments:\n"
 "   string - Name of the body, case insensitve may use id as a string\n"
 "   3-tuple - (X,Y,Z) coordinates of a point (usually in rotating frame)\n"
 "   float - Equatorial radius of a reference spheroid.\n"
 "   float - Flattening coefficient = (Req - Rpole) / Req\n"
 "\n"
 "3 Returns:\n"
 "   float - Planetographic longitude of the input point.  Longitude\n"
 "      increases to the east for retrograde planets and the earth, sun,\n"
 "      and moon.  Longitude increases to the west for all other bodies.\n"
 "   float - Planetographic latitude of the input point in radians. eq = 0\n"
 "   float - Altitude of point above the reference spheroid\n"
 "\n"
 "WARNING: Kernel variables of the type BODY<body ID>_PGR_POSITIVE_LON\n"
 "   can be used to change the direction of positive longitude for any\n"
 "   body\n";

char spicehelp_reclat[] = 
 "Convert from rectangular coordinates to latitudinal coordinates.\n"
 "1 Argument:\n"
 "  3-tuple - (X,Y,Z) The rectangular coordinates of the input point.\n"
 "\n"
 "1 Return:\n"
 "  3-tuple - (Radius, Longitude, Latitude)\n"
 "     Radius -  Distance of the point from the origin.  The units associated\n"
 "        with Radius are those associated with the input.\n"
 "\n"
 "     Longitude - Longitude of the input point.  This is angle between the\n"
 "        prime meridian and the meridian containing `rectan'.  The direction\n"
 "        of increasing longitude is from the +X axis towards the +Y axis.\n"
 "        Longitude is output in radians in the range [-pi, pi].\n"
 "\n"
 "     Latitude - Latitude of the input point.  This is the angle from the XY\n"
 "        plane of the ray from the origin through the point.  Latitude is\n"
 "        output in radians in the range [-pi/2, pi/2].\n"
 "\n"
 "   Note 1: If the X and Y components of `rectan' are both zero, the\n"
 "      longitude\n is set to zero.\n"
 "\n" 
 "   Note 2: If `rectan' is the zero vector, longitude and latitude are both\n"
 "      set to zero.\n";
 
 
