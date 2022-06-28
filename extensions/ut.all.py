#!/usr/bin/env python

"""
Unit tests based oy PyUnit.
"""

# Add parent directory to python search path
# ------------------------------------------
import os
import sys
from optparse      import OptionParser   # Command-line args

import unittest

#......................................................................

sys.path.insert(0,'../pytests/lib')
from grads import GrADS

import ams
import bjt
import fish
# import gsf  # works, test do not work
# import gxyat  # DEPRECATED, removed support for gacols.h in OpenGrADS
import hello
import ipc
# import lats  # seems to work, tests do not work because missing dataset
import mf
import orb
import tle  # works, test do not work
# import shape  # DEPRECATED
# import shfilt  # works, test do not work

# Special case to avoid conflict with bult in "re" (regular expression)
sys.path.insert(0,'re')
# import utre as re  # works, test do not work

#......................................................................

def run_all_tests(verb=2,BinDir=None,DataDir=None):
    """
    Runs all tests based on the standard *model* testing file.
    """

    print("")
    print("             Testing OpenGrADS Extensions")
    print("             ----------------------------")
    print("")

#   Assemble and run the test suite
#   -------------------------------
    load = unittest.TestLoader().loadTestsFromTestCase
    TestSuite = [ load(ams.ut), 
                  load(bjt.ut), 
                  load(fish.ut),
#                   load(gsf.ut),  # works, test do not work 
#                   load(gxyat.ut), # DEPRECATED, removed support for gacols.h in OpenGrADS
                  load(hello.ut),
                  load(ipc.ut),
#                   load(lats.ut),  # seems to work, tests do not work because missing dataset 
                  load(mf.ut),
                  load(orb.ut),
                  load(tle.ut),  # works, test do not work
#                   load(re.ut),  # works, test do not work
#                   load(shape.ut),  # DEPRECATED
#                   load(shfilt.ut),  # works, test do not work
                ]
    all = unittest.TestSuite(TestSuite)

    Results = unittest.TextTestRunner(verbosity=verb).run(all)

#   Return number of errors+failures: skipped binaries do not count
#   ---------------------------------------------------------------
    if not Results.wasSuccessful(): 
        raise IOError('GrADS tests failed')
    else:
        print("Passed ALL unit tests")

#----------------------------------------------------------------

if __name__ == "__main__":

    print("")
    print("  Welcome to OpenGrADS Extensions Unit Tests")
    print("")

    # Run all tests: BinDir and DataDir passed as environment variables
    # GABDIR and GADSET
    # -----------------------------------------------------------------
    run_all_tests()
