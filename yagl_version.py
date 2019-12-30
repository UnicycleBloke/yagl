# Possibly replace this file with bash script or something to remove
# build dependency on Python.

import os

stream = os.popen('git describe')
version = stream.read().strip()

# This is supposed to place version.h at the project top level - the parent
# folder of the build folder. 
outfile = open("../yagl_version.h", "w+")
outfile.write("// This file is generated in a pre-build step by version.py\n")
outfile.write("#pragma once\n")
outfile.write("constexpr const char* str_yagl_version = \"%s\";" % (version))

