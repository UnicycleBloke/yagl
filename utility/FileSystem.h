///////////////////////////////////////////////////////////////////////////////
// Copyright 2019 Alan Chambers (unicycle.bloke@gmail.com)
//
// This file is part of yagl.
//
// yagl is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// yagl is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with yagl. If not, see <https://www.gnu.org/licenses/>.
///////////////////////////////////////////////////////////////////////////////

// This needs to be tested. Inconvenient that the filesystem stuff
// isn't supported as cleanly as we'd like.
#ifdef __GNUC__
    #include <features.h>
    #if __GNUC_PREREQ(8,0)
        // g++ version 8.0 or later
        #include <filesystem>
        namespace fs = std::filesystem;
    #else
        // g++ version 7.x or earlier
        #include <experimental/filesystem>
        namespace fs = std::experimental::filesystem;
    #endif
#else
    // TODO similar checks for other compilers.
    #include <filesystem>
    namespace fs = std::filesystem;
#endif


