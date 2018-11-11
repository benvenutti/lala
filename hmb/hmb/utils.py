import git
import os
import shutil
import subprocess

import hmb.config as config

def updateDependency( dep ):
    folder = getFolder( dep )
    if os.path.exists( folder ) and os.path.isdir( folder ):
    	update( dep, folder )
    else:
    	clone( dep, folder )

def getFolder( dep ) -> bool:
    return os.path.join( config.package_folder, dep.name )

def printUpdate( action, dep, folder ):
    print( action, dep.name, "(" + dep.url + ")", "@", dep.branch, "into", folder )

def update( dep, folder ):
    printUpdate( "Updating", dep, folder )

    repo = git.Repo( folder )
    if repo.is_dirty():
        repo.git.reset( "--hard" )
        repo.git.clean( "-fdx" )

    repo.remotes.origin.pull()
    repo.git.checkout( dep.branch )
    repo.remotes.origin.pull()
    repo.git.pull()

def clone( dep, folder ):
    printUpdate( "Cloning", dep, folder )
    git.Repo.clone_from( dep.url, folder, branch=dep.branch, recursive=True )

def run( cmd ) -> bool:
    return subprocess.check_call( cmd, stderr=subprocess.STDOUT, shell=True ) == 0

def printBuild( dep, folder ):
    print( "Building dependency", dep.name, "into", folder )

def printBuildError( dep, step ):
    print( "error building", dep.name, ", the following build step failed:" )
    print( "  ->", step )

def buildDependency( dep ):
    binFolder = os.path.join( getFolder( dep ), config.bin_folder )

    printBuild( dep, binFolder )

    if os.path.exists( binFolder ) and os.path.isdir( binFolder ):
        shutil.rmtree( binFolder )

    os.makedirs( binFolder )
    os.chdir( binFolder )

    for step in dep.buildSteps:
        if not run( step ):
            printBuildError( dep, step )
