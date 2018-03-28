#!/usr/bin/env python

import sys
import pspice as S

pout = sys.stdout.write



S.furnsh('kernels.list')

# Test et2lst, using MARS at 10 degress longitude

pout("Test utc2et: ")
et = S.utc2et("2012-02-21T01:32:31")
pout(str(et))
pout("  OK\n")

pout("Test et2lst: ")
pout(str(S.et2lst(et, 499, 10.0*0.0175, "PLANETOCENTRIC")) )
pout("  OK\n")

pout("Test etcal:  ")
s = S.etcal(et)
pout(s)
pout("  OK\n")



