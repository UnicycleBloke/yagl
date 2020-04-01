[Back to table of contents](index.md)

# Building on Windows  

Starting in some folder on your machine, execute the following commands in a terminal window. This assumes that you have installed Microsoft Visual Studio 2019 (Community edition is fine), and placed MSBuild on the path, and have installed the build dependencies (see below):

```bash
git clone https://github.com/UnicycleBloke/yagl.git
cd yagl
mkdir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=<vcpkg-root>\scripts\buildsystems\vcpkg.cmake
    -DVCPKG_DIR=<vcpkg-root> ..
msbuild yagl.sln /property:Configuration=Release
Release\yagl.exe -d <your_grf_file.grf> 
```

Rather than use MSBuild, you can open the solution file in Visual Studio and build it from there. This will certainly be more useful for debugging, should you need to do that.

## Build dependencies

The only binary library dependency is **libpng**, which is used for reading and writing spritesheets. This is installed using **vcpkg**. Starting in some folder on your machine, execute the following commands in a terminal window:

```bash
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
bootstrap-vcpkg.bat
vcpkg install libpng:x64-windows-static
```

The following header only libraries are included in the source tree along with their licences:
- **png++**: a C++ wrapper around the libpng API: https://www.nongnu.org/pngpp/.
- **cxxopts**: a C++ command line option parser: https://github.com/jarro2783/cxxopts.

The build also makes use of **python** for a pre-build step to create the version number from the git repository: it runs `yagl_version.py` to create `yagl_version.h`. Python needs to be on your path. 

