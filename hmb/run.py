"""Help Me Build (hmb) is small tool to clone, build and expose a project dependency."""

import subprocess
import sys

import hmb.config
import hmb.utils

def checkPython3():
    if sys.version_info[0] < 3:
        raise Exception( "Python 3 or a more recent version is required." )

def main():
    for dep in hmb.config.packages:
        hmb.utils.updateDependency( dep )

        if dep.buildSteps:
            hmb.utils.buildDependency( dep )

        print( "...done" )

main()
