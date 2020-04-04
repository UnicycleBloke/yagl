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


#### [Building yagl on Linux](docs/build_linux.md) 

#### [Building yagl on Windows](docs/build_windows.md) 

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


Read the [main documentation](docs/index.md) here.
