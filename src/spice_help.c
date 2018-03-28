/* spice help -- 
 *
 *         FILE IS NOT FINISHED, JUST IGNORE FOR NOW !!!!
 *
 */

char spicehelp_spkez[] = 
 "Return the state (position and velocity) of a target body relative to an\n"
 "observing body, optionally corrected for light time (planetary aberration)\n"
 "and stellar aberration.\n"
 "5 Arguments:\n"
 "   targ        is the NAIF ID code for a target body.  The target 
 "              and observer define a state vector whose position 
 "              component points from the observer to the target.
 "
 "  et          is the ephemeris time, expressed as seconds past J2000 
 "              TDB, at which the state of the target body relative to \n"
               the observer is to be computed.  `et' refers to time at 
               the observer's location. 

   ref         is the name of the reference frame relative to which 
               the output state vector should be expressed. This may 
               be any frame supported by the SPICE system, including 
               built-in frames (documented in the Frames Required 
               Reading) and frames defined by a loaded frame kernel
               (FK). 

               When `ref' designates a non-inertial frame, the 
               orientation of the frame is evaluated at an epoch  
               dependent on the selected aberration correction. 
               See the description of the output state vector `starg' 
               for details. 

   abcorr      indicates the aberration corrections to be applied 
               to the state of the target body to account for one-way 
               light time and stellar aberration.  See the discussion 
               in the Particulars section for recommendations on  
               how to choose aberration corrections. 

               `abcorr' may be any of the following: 

                  "NONE"     Apply no correction. Return the  
                             geometric state of the target body  
                             relative to the observer.   

               The following values of `abcorr' apply to the
               "reception" case in which photons depart from the
               target's location at the light-time corrected epoch
               et-lt and *arrive* at the observer's location at
               `et':

                  "LT"       Correct for one-way light time (also 
                             called "planetary aberration") using a 
                             Newtonian formulation. This correction 
                             yields the state of the target at the 
                             moment it emitted photons arriving at 
                             the observer at `et'. 

                             The light time correction uses an
                             iterative solution of the light time 
                             equation (see Particulars for details). 
                             The solution invoked by the "LT" option 
                             uses one iteration. 

                  "LT+S"     Correct for one-way light time and 
                             stellar aberration using a Newtonian 
                             formulation. This option modifies the 
                             state obtained with the "LT" option to 
                             account for the observer's velocity 
                             relative to the solar system 
                             barycenter. The result is the apparent 
                             state of the target---the position and 
                             velocity of the target as seen by the 
                             observer. 

                  "CN"       Converged Newtonian light time 
                             correction.  In solving the light time 
                             equation, the "CN" correction iterates 
                             until the solution converges (three 
                             iterations on all supported platforms). 

                             The "CN" correction typically does not 
                             substantially improve accuracy because 
                             the errors made by ignoring 
                             relativistic effects may be larger than 
                             the improvement afforded by obtaining 
                             convergence of the light time solution. 
                             The "CN" correction computation also  
                             requires a significantly greater number 
                             of CPU cycles than does the  
                             one-iteration light time correction. 

                  "CN+S"     Converged Newtonian light time 
                             and stellar aberration corrections. 


               The following values of `abcorr' apply to the 
               "transmission" case in which photons *depart* from 
               the observer's location at `et' and arrive at the 
               target's location at the light-time corrected epoch 
               et+lt: 

                  "XLT"      "Transmission" case:  correct for 
                             one-way light time using a Newtonian 
                             formulation. This correction yields the 
                             state of the target at the moment it 
                             receives photons emitted from the 
                             observer's location at `et'. 

                  "XLT+S"    "Transmission" case:  correct for 
                             one-way light time and stellar 
                             aberration using a Newtonian 
                             formulation  This option modifies the 
                             state obtained with the "XLT" option to 
                             account for the observer's velocity 
                             relative to the solar system 
                             barycenter. The position component of 
                             the computed target state indicates the 
                             direction that photons emitted from the 
                             observer's location must be "aimed" to 
                             hit the target. 

                  "XCN"      "Transmission" case:  converged  
                             Newtonian light time correction. 

                  "XCN+S"    "Transmission" case:  converged  
                             Newtonian light time and stellar  
                             aberration corrections. 


               Neither special nor general relativistic effects are 
               accounted for in the aberration corrections applied 
               by this routine. 

               Case and blanks are not significant in the string 
               `abcorr'. 

   obs         is the NAIF ID code for an observing body. 
