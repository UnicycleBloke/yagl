# ///////////////////////////////////////////////////////////////////////////////
# // Copyright 2019 Alan Chambers (unicycle.bloke@gmail.com)
# //
# // This file is part of yagl.
# //
# // yagl is free software: you can redistribute it and/or modify
# // it under the terms of the GNU General Public License as published by
# // the Free Software Foundation, either version 3 of the License, or
# // (at your option) any later version.
# //
# // yagl is distributed in the hope that it will be useful,
# // but WITHOUT ANY WARRANTY; without even the implied warranty of
# // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# // GNU General Public License for more details.
# //
# // You should have received a copy of the GNU General Public License
# // along with yagl. If not, see <https://www.gnu.org/licenses/>.
# ///////////////////////////////////////////////////////////////////////////////

# TODO Possibly replace this file with bash script or something to remove
# build dependency on Python.

import os

stream = os.popen('git describe')
version = stream.read().strip()

# This is supposed to place version.h at the project top level - the parent
# folder of the build folder. Just having an include might possibly fall foul of
# the one definition rule, but it seems to be OK so far. Maybe using constexpr makes
# this a non-issue? Easy to fix if it becomes a problem.

# TODO compare contents with existing file to avoid unnecessary recompilation? Seems
# to work anyway...
outfile = open("../yagl_version.h", "w+")
outfile.write("// This file is generated in a pre-build step by yagl_version.py\n")
outfile.write("#pragma once\n")
outfile.write("constexpr const char* str_yagl_version = \"%s\";" % (version))

