/* spice help ckgp */

char spicehelp_ckgp[] = 
 "Get pointing (attitude) for a specified spacecraft clock time.\n"
 "\n"
 "4 Arguments:\n"
 "   id (int) - NAIF ID of instrument, spacecraft, or structure.\n"
 "\n"
 "   sclkdp (float) - The encoded spacecraft clock time for which pointing\n"
 "      is being requested.  The routines scencd and sce2c respectively\n"
 "      convert spacecraft clock strings and ephemeris time to encoded\n"
 "      spacecraft clock.\n"
 "\n"
 "   tol (float) - The time tolerance in ticks, the units of encoded\n"
 "      spacecraft clock time.  The routine sctiks converts a\n"
 "      spacecraft clock tolerance duration from its character string\n"
 "      representation to ticks.\n"
 "\n"
 "   ref (string) - The desired reference frame for the returned pointing.\n"
 "\n"
 "2 Returns:\n"
 "   None - If a record was not found to satisfy the request None is\n"
 "     otherwise a 2-tuple of the following items is returned:\n"
 "\n"
 "   cmat (3X3 tuple) - is a rotation matrix that transforms components of a\n"
 "      vector  expressed in the frame specified by `ref' to components\n"
 "      expressed in the frame tied to the instrument, spacecraft, or\n"
 "      other structure at time `clkout' (see below).\n"
 "\n"
 "      NOTE: cmat is in ROW-MAJOR order, thus cmat[0][2] is the 1st row\n"
 "      and the 3rd column.  The row index is first, column index is second.\n" 
 "\n"
 "      Thus, if a vector v has components x,y,z in the `ref' reference\n"
 "      frame, then v has components x',y',z' in the instrument fixed frame\n"
 "      at time `clkout':\n"
 "\n"
 "            [ x' ]     [          ] [ x ] \n"
 "            | y' |  =  |   cmat   | | y | \n"
 "            [ z' ]     [          ] [ z ] \n"
 "\n"
 "      If you know x', y', z', use the transpose of the C-matrix to\n"
 "      determine x, y, z as follows: \n"
 "\n"
 "            [ x ]      [          ]T    [ x' ] \n"
 "            | y |  =   |   cmat   |     | y' | \n"
 "            [ z ]      [          ]     [ z' ] \n"
 "                       (Transpose of cmat) \n"
 "\n"
 "   clkout (float) - the encoded spacecraft clock time associated with\n"
 "       the returned C-matrix. This value may differ from the requested\n"
 "       time, but never by more than the input tolerance `tol'.\n"
;

char spicehelp_axisar[] = 
 "Construct a rotation matrix that rotates vectors by a specified angle about\n"
 "a specified axis. \n"
 "\n"
 "2 Arguments:\n"
 "   axis (3-elements) - The (x, y, z) values representing a vector in\n"
 "      cartesian space.  The axis item may also be anything else that has 3\n"
 "      indexed components such as a tuple, list, 1-dimensional array,\n"
 "      single column matrix or single row maxtrix.\n"
 "\n"
 "   angle - The rotaiton angle in radians. Not. that all spice rotation\n"
 "      angles follow the \"right hand rule\", so counterclockwise rotations\n"
 "      about an axis have positive measure.\"\n"
 "\n"
 "1 Return:\n"
 "   rotation - A 3 by 3 numpy array of floating point rotation coefficients.\n"
 "      Array indicies are row-major.  Thus the first index indicates the\n"
 "      matrix row, the second is the column, i.e. M[row][column].\n"
 "\n"
 ;
 
 
char spicehelp_m2q[] = 
"Find a unit quaternion corresponding to a specified rotation matrix.\n"
"\n"
"1 Argument:\n"
"   rotation (3x3 elements) - A rotation matrix, may be any 3x3 element item\n"
"      such as a 3-tuple of 3-tuple, a two dimensional array, or matrix\n"
"\n"
"1 Return:\n"
"   quaternion - A unit-length SPICE-style quaternion representing the\n"
"      rotation.  The quaternion is a rank 1, 4-element array.  With the\n"
"      following correspondence to the rotation matrix.\n"
"\n"
"      If 'r' rotates vectors in the counterclockwise sense by an angle of\n"
"      'ϴ' radians about a unit vector 'a', where:\n"
"\n"
"         0 ≤ θ ≤ π\n"
"\n"
"      then letting h = θ/2,\n"
"\n"
"         q = ( cos(h), sin(h)a₁,  sin(h)a₂,  sin(h)a₃ ).\n"
"\n"
"      The restriction that 'θ' must be in the range [0, π] determines the\n"
"      output quaternion 'q' uniquely except when θ = π; in this special case,\n"
"      both of the quaternions:\n"
"\n"
"         q = ( 0, a₁, a₂, a₃)\n"
"\n"
"      and\n"
"\n"
"         q = ( 0, -a₁, -a₂, -a₃)\n"
"\n"
"      are possible outputs.\n"
;

		
char spicehelp_pxform[] = 
"Return the matrix that transforms position vectors from one specified frame\n"
"   to another at a specified epoch.\n"
"\n"
"3 Arguments:\n"
"   from (string) - The name of a reference from in which a position vector\n"
"      is known\n"
"\n"
"   to (string) - The name of a reference frame in which it is desired to\n"
"      represent a position vector.\n"
"\n"
"   et (float) - is the epoch in ephemeris seconds past the epoch of J2000\n"
"      (TDB) at which the position transformation matrix 'rotate' should be\n"
"      evaluated.\n"
"\n"
"1 Return:\n"
"   rotate (3x3 elements) - the matrix that transforms position vectors from\n"
"      the reference frame 'from' to the frame 'to' at epoch 'et'.\n"
"\n"
"      If (x, y, z) is a position relative to the frame 'from' then the vector\n"
"      ( x', y', z') is the same position relative to the frame 'to' at epoch\n" 
"      'et'.  Here the vector ( x', y', z') is defined by the equation:\n"
"\n"
"          -   -       -        -     -  -\n"
"         | x'  |     |          |   | x  |\n"
"         | y'  |  =  |  rotate  |   | y  |\n"
"         | z'  |     |          |   | z  |\n"
"          -   -       -        -     -  - \n"
;
		
		
		
		
		
		

















