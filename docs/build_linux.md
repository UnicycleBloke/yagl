[Back to table of contents](index.md)


# Building on Linux

Starting in some folder on your machine, execute the following commands in a terminal window. This assumes that you have installed the build dependencies (see below):

```bash
git clone https://github.com/UnicycleBloke/yagl.git
cd yagl
mkdir build
cd build
cmake ..
make -j
./yagl -d <your_grf_file.grf>
```

If you prefer **ninja** for building:

```bash
cmake -G"Ninja" ..
ninja
./yagl -d <your_grf_file.grf>
```

Pretty much any reasonably recent Linux-like environment should be fine. The software has been built and run on the following configurations:
- Ubuntu 16.04 LTS; CMake 3.15.0; g++ 9.2.1
- Ubuntu 22.04 LTS
- Windows 10 Subsystem for Linux; Ubuntu 18.04 LTS; CMake 3.10; g++ 8.2.0
- Windows 10 Subsystem for Linux; Ubuntu 24.04 LTS; CMake 3.28.3; g++ 13.2.0
- Windows 10 Subsystem for Linux; Debian GNU/Linux; CMake 3.25.1; g++ 12.2.0

It should also build with **g++ 7.4**, though this required a little conditional compilation around `<filesystem>`, which was not officially supported until **g++ 8.0**. Not recommended.

Though this has not been tried, the software seems very likely to build with **clang**, if this is preferred. The toolchain can be edited by running **ccmake**.

## Build dependencies

The only binary library dependency is **libpng**, which is used for reading and writing spritesheets. You may need to install the headers for this, as follows:

```bash
sudo apt-get install libpng-dev
```
`libpng` apparently depends on `zlib`. I did not have a problem, but you may need to install this, too:

```bash
sudo apt-get install zlib1g-dev
```

The following header only libraries are included in the source tree along with their licences:
- **png++**: a C++ wrapper around the libpng API: https://www.nongnu.org/pngpp/.
- **cxxopts**: a C++ command line option parser: https://github.com/jarro2783/cxxopts.
