import collections
import multiprocessing

cpu_count = multiprocessing.cpu_count()
package_folder = "3rd-party"
bin_folder = "bin"

dependency = collections.namedtuple('dependency', 'name url branch buildSteps')

packages = [
    dependency( 
        "libpd", 
        "https://github.com/libpd/libpd.git", 
        "master",
        [
            "cmake -DPD_UTILS=ON -DPD_EXTRA=ON ..", 
            "cmake --build . -- -j" + str( cpu_count )
        ] )
]
