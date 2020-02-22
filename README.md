# Yet Another GRF Language 

**yagl** is a command line tool, and a script language (called YAGL for maximal ambiguity), for working with OpenTTD and TTDPatch GRF files, as described in the [NewGRF specs](https://newgrf-specs.tt-wiki.net/wiki/Main_Page). It is basically a codec which performs either of the following operations:

1. Read a GRF file into a memory data structure, and then write out a YAGL script containing a textual representation of its contents, together with one or more sprite sheets (PNG format) containing the images present in the GRF. Sound effects are also written to files.
2. Read a YAGL file and any associated sprite sheets (and sound effects) into a memory data structure (the same one as above), and then write out a GRF file containing the binary representation of the YAGL and the encoded sprites which conforms to the NewGRF specs.

If you perform Step 1, and then perform Step 2 without first making any changes to the YAGL or the sprite sheets, then the re-generated GRF should in principle be identical to the original. There are a few valid reasons why there might be discrepancies, such as the elimination of duplicate properties in Action00, re-ordering properties (the order is not significant), variant string encodings, and so on. But they should certainly be *semantically* identical.

A GRF is basically just a long list of records of different types. A YAGL script is also just a long list of records of different types. Each record in the YAGL should be semantically identical to the corresponding record in the GRF from which it was created. It's just that while the GRF record is binary data, the YAGL record is human-readable text. For example, the following output is used to represent an Action08 record, extracted from FIRS:

```bash
grf // Action08
{
    grf_id: "\xF1%\x00\x07";
    version: 8;
    name: "FIRS Industry Replacement Set 3.0.12";
    description: "{orange}FIRS Industry Replacement Set{new-line}{black}Brought
        to you by andythenorth, FooBar, planetmaker, Terkhen and Yexo, with help 
        from many others. {lt-gray}See readme for more information, including 
        how to use FIRS. {black}License: GPL v2.";
}
```

Though this looks superficially similar to NML, it is definitely *not* NML. For example, the NML `grf` block includes other items, such as GRF options, which are actually placed into Action14 records in the GRF file. And there is currently no attempt to pull out string translations into separate language files. Some features of NML do map quite simply onto the underlying pseudosprite records; others not so much. 

The goal here is not to decompile NML, which is probably impossible to do well, but to create something which is hopefully  more readable than the equivalent NFO. 

## Road map

Although theoretically complete in terms of the basic functionality, more could be done:

- Most properties in Action00 records are represented by simple numbers (presented in hex, but decimal, octal and binary are supported). Many of these are enumerations which could be replaced with text representations of the permitted values. Many others are bitfields for each each supported bit could be replaced by a text represention. This would make the YAGL more readable.

- There is as yet no documentation. It would be nice if **yagl** itself could generate annotated versions of the various YAGL records it can print. 

- The software currently terminates as soon as there is a fault while parsing the YAGL. It would be good to implement a more fault tolerant design. This would not accept poorly formatted YAGL, but could form the basis for a linter. 

- It might be worth generating NFO which is compatible with **grfcodec**. 


## Building **yagl**

The build system is created with **CMake**, and can perform the actual build with **make**, **ninja**, or some other tool for which a generator exists. `CMakeLists.txt` specifies at least **CMake** version 3.10. This is the lowest version for which a build was attempted. Earlier versions may also work.

**yagl** is written in C++, and makes use of at least some C++17 features, notably `<filesystem>`. This affects the compiler versions which can be used.  


### Building on Linux 

Starting in some folder on your machine, execute the following commands in a terminal window:

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

The software has so far only been compiled with **g++** on Linux. Pretty much any reasonably recent Linux-like environment should be fine. The software has been built and run on the following configurations:
- Ubuntu 16.04 LTS; CMake 3.15.0; g++ 9.2.1
- Windows 10 Subsystem for Linux; Ubuntu 18.04 LTS; CMake 3.10; g++ 8.2.0

It should also build with **g++ 7.4**, though this required a little conditional compilation around `<filesystem>`, which was not officially supported until **g++ 8.0**. Not recommended.

Though this has not been tried, the software seems very likely to build with **clang**, if this is preferred. The toolchain can be edited by running **ccmake**.


#### Build dependencies

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

Aside from **g++**, the build also makes use of **python** for a pre-build step to create the version number from the git repository: it runs `yagl_version.py` to create `yagl_version.h`. I found it necessary to manually create the link `/usr/bin/python` to the binary `/usr/bin/python3`. This may or may not be an issue on your system. 

### Building on Windows

TODO

## Licence

**yagl** is licensed under GPL version 3 or later. See the COPYING file for details. 

## Running **yagl**

**To decode a GRF file into YAGL, run the following command:**

```bash
./yagl --decode [<options>] <grf_file> [<directory>] 
```

This creates a sub-directory with the given name (this defaults to `sprites`), and decodes the given GRF file. The YAGL script file, any sprite sheets, and any sound effects in the GRF, are all placed into this folder. The GRF's stem is used in the names of the output files. For example `my_mod.grf` leads to the creation of `sprites/my_mod.yagl`, `sprites/my_mod.XXX.png`. The sprite sheets names are also extended to include colour depth and zoom level. Sound effects include their own file names within the GRF: `sprites/some_sound.wav`.

The GRF file name may include a directory path, such as `foo/bar/mod.grf`. In this case, the sprites folder is created relative to the folder containing the GRF file: `foo/bar/sprites`. 

The decoder should throw an exception and terminate as soon as it detects data in the binary input stream that does not match its expectations, hopefully with some useful indicator of the problem.

**To encode a YAGL script into a GRF file, run the following command:**

```bash
./yagl --encode [<options>] <grf_file> [<directory>]  
```

This checks for the existence of a sub-directory with the given name (defaults to `sprites`), and then checks for the existence of a YAGL script file with the same stem as the given output GRF file name. The YAGL script file is encoded to create a GRF file, and pulls in the relevant sections of the spritesheets and sound effects.  

The encoder will throw an exception and terminate as soon as it detects tokens in the text input stream that do not match its expectations.

**yagl supports a number of options:**

Both long and short version of each option are supported.

- **--decode, -d**: as described above.
- **--encode, -e**: as described above.
- **--hexdump, -x**: reads the GRF into memory as for **--decode**, and then dumps a hex representation somewhat similar to NFO (it is *not* NFO). The purpose is to help analyse differences between original and re-created GRF files.
- **--palette, -p \<index\>**: choose the initial palette for the GRF. 
  - This setting will be overridden if a value is set in Action14 in a "PALS" element.
  - Permitted index values are:
    - 1: DOS palette - this is the default
    - 2: Windows palette
    - 3: DOS Toyland palette
    - 4: Windows Toyland palette
- **--width, -w \<num\>**: sets the maximum width in pixels of sprite sheets.
  - This defaults to 800, and is capped at 65,535.
  - The image may be wider, if the widest sprite would not fit. 
  - This option is ignored when encoding a GRF.
- **--height, -h \<num\>**: sets the maximum height in pixels of sprite sheets.
  - This defaults to 16,000, and is capped at 65,535. 
  - The image may be taller, if the sprites in the last row would not fit.
  - The sprites are divided into multiple sprite sheets if their combined height exceeds this.
  - This option is ignored when encoding a GRF.
- **--version, -v**: displays the version of the **yagl** executable.
  - The rest of the command line is ignored when this option is present. 
- **--help**: displays this help in the console.   

## Software architecture

The software architecture is closely modeled on the structure of a GRF file. The following sections give an overview of the GRF contents to help make sense of the code.

The format of a GRF file is pretty simple: it is nothing more than a long list of variable length records. Some of the records are pseudo-sprites (Action01, Action02, ... in the NewGRF specs) containing information about how various aspects of the game should behave, which sprites to draw, and many other things. The rest of the records are binary blobs containing mostly images (RealSprites), but sometimes sound effects. The GRF file comes in two distinct formats: Container1 and Container2. 

### Container1 format 

Container1 files intersperse the RealSprites among the pseudo-sprites. They directly follow the pseudo-sprite to which they "belong". For example, Action01 defines M sets of N sprites each. The Action01 record is followed by MxN RealSprite records. Action01 can be thought of as a notional container, owner or parent of those MxN sprites. The Action01 is a parent record, and the RealSprites are its children. Action05, Action0A and one or others follow the same pattern. Action11 is similar, but its child records are sound effects. We can represent this as a two-tier structure as follows: child records are indented under their parents:

```bash
Action14
Action08
...
Action01 (2 sets of 3 sprites each, say - 6 sprites follow)
  - RealSprite
  - RealSprite
  - RealSprite
  - RealSprite
  - RealSprite
  - RealSprite
Action02
...
```	

### Container2 format

Container2 files introduce a new type of record which contains the index of a real sprite. This is used in containers such as Action01 in place of RealSprites, and the RealSprites are moved to the end of the GRF. [Sound effects can also be moved to this section.] This indirection makes it possible to provide two or more distinct images for a given sprite index, being rendered at different zoom levels and/or with different colour depths. For each sprite index, there is a contiguous array of RealSprites in the GRF file rather than just one. 

Container1 does not support this feature, having a single palette-based sprite per sprite index (in fact it doesn't even really have sprite indices in the GRF).

```bash
// Data section
Action14
Action08
...
Action01 (2 sets of 3 sprites each, say - 6 sprite indices follow)
  - SpriteIndex 1
  - SpriteIndex 2
  - SpriteIndex 3
  - SpriteIndex 4
  - SpriteIndex 5
  - SpriteIndex 6
Action02
...

// Graphics section
RealSprite 1 (8bpp normal)
RealSprite 1 (8bpp x2)
RealSprite 1 ... 
RealSprite 2 (8bpp normal)
RealSprite 2 (8bpp x2)
...
RealSprite 6 (8bpp normal)
RealSprite 6 (8bpp x2)
RealSprite 6 (8bpp x4)
...
```

It seems that, at least in theory, this format would allow the same sprites to be used in multiple places in the GRF, by having two or more sprite index records wrapping the same sprite index. It is not known whether this feature is ever actually used.

For full details, read `grf.txt` which can be found in the **grfcodec** [repository](https://github.com/OpenTTD/grfcodec).

### Software representation

The software design mimics the structure of a Container2 format file. When reading a Container1 file, it automatically generates the necessary SpriteIndex records, and invents IDs for the sprites.

```bash
NewGRFData
  - vector<Record>
      - Action14Record
      - Action08Record
      - ... 
      - Action01Record - only the parent record is present in the vector
          - SpriteIndexRecord (index = 1)
          - SpriteIndexRecord (index = 2)
          - ...
      - Action02Record
      - ...
		
  - map<sprite_ID, vector<RealSpriteRecord>>		 
      - 1: vector<RealSpriteRecord>
          - RealSprite 1 8bpp zoom normal
          - RealSprite 1 32bpp zoom x2
          - ... 
      - 2: vector<RealSpriteRecord>
          - RealSprite 2 8bpp zoom normal
          - RealSprite 2 32bpp zoom x2
          - ... 
      - ...		
```

Each type of record is represented by a distinct class which is derived from a common base class. Each type implements methods to read/write itself from/to a file stream. 
- There are two read methods: 
  - one decodes the binary format for the GRF record (`read()`); 
  - the other parses the text format for the corresponding YAGL record (`parse()`). 
- There are also two write methods: 
  - one encode the binary format for the GRF record (`write()`); 
  - the other prints the text format for the corresponding YAGL record (`print()`). 
  
In some places, records are broken up recursively into collections of smaller data structures. Those structures often implement their own `read()`, `write()`, `print()` and `parse()` methods. Using Object Orientation is not everyone's cup of tea, but it makes serialisation code much simpler. 

And that's pretty much the whole program in a nutshell.

**Parsing YAGL**

The YAGL script is run through a lexer to create a list of tokens representing strings, numbers, identifiers and assorted special symbols such as colons, braces, etc. Whitespace and comments are removed. 

The list of tokens is then de-serialised (in the `parse()` methods) to create an in-memory representation (as described above), rather than parsed in the traditional sense. There is no explicit language definition in terms of abstract production rules. Instead, the list of tokens is interpreted recursively with local lookup tables and other objects. This is analogous to the way in which the binary GRF file is read to create the in-memory representation. Reading the YAGL in this way probably results in more verbose code, but seemed like the right approach in this case. 

Perhaps it makes sense to regard the in-memory representation itself as being an abstract syntax tree: it certainly contains all the information necessary to generate either a binary GRF file or the equivalent YAGL script. But the way it is created has more in common with de-serialisation than rules-based parsing.

One of the goals for the parsing is to allow each different type of property to have its own specific mini-parser (the code calls these Descriptors). The idea is that, for example, every bit field can be represented with a series of property-specific identifiers ORed together (e.g. `climate: Arctic | Temperate`). It is simple to maintain a particular descriptor without affecting any other part of the YAGL de-serialisation. Descriptors can be created to represent any data type, but common ones are numbers, enumerations, bitfields, GRF "labels" (32-bit ID used for many things), and array of numbers and other things. Special case descriptors include snow lines and bridge tables.

Descriptors are also used for print the value of a property when generating YAGL. 

**GRF Strings**

Strings in GRF files can be encoded in either not-quite-Latin1 or not-quite-UTF8 (prefixed with an upper case thorn character: **Þ**). In both cases the strings may contain binary control codes which have special functions when the strings are rendered in-game. 

Strings are converted to a human-readable pure-UTF8 format when generating YAGL. All the control codes are replaced with escaped string names (e.g. `"{blue}This text is blue. {red}This text is red."`). Braces are used to escape control sequences, and a double open brace is used to create a literal single open brace. Double quotes are escaped with `{dq}` as this symbol is used to delimit strings in YAGL. Double quotes are not special characters in GRF strings.

The string conversion is done in two stages. 

- Both Latin1 and UTF8 strings first are converted to UTF16. This unifies the two string types and makes life a little simple as a `char16_t` type can represent every code point in the Basic Multilingual Plane (we ignore the supplementary planes for now), and the control codes are represented more cleanly by private-use codepoints, **U+E0xx**. The **Þ** prefix is removed.

- The second stage replaces the control codes and their arguments with string representations, and then the whole thing is converted to UTF8. 

This procedure might be overkill, but the primary goal was correct function. It can be refactored later. 

Parsing a string is more or less the same procedure in reverse. The UTF16 intermediate string is checked to see if it contains any characters (other than control codes) which are not Latin1. This determines whether the GRF string is encoded as Latin1 or UTF8. UTF8 strings are prefixed with a **Þ** character.

Note that the non-unique nature of string encoding means it is very likely that using **yagl** to decode and immediately re-encode a GRF will result in different binary files, even though they are semantically identical - a bit of a pain for testing.

**Assorted details**

- The map of RealSprites is walked to generate spritesheets before writing out the YAGL, this is because each sprite's position in the spritesheet needs to be calculated before the YAGL can be printed. Some sprites include both RGBA and palette information (a mask). These result in two images in different spritesheets.

- Some of the pseudo-sprites have very simple flat data structures, and are quite simple to read and write. Others such as, for example, Action14, are more complicated. In such cases there may be additional classes or structures intended to break down the task of reading and writing the data.

- Aside from strings, one or two other features have non-unique encodings, such as extended bytes. There may be cases in which encoded RealSprites have a different format due to being chunked or no chunked. 


***TODO Much more information...***