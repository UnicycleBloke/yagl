[Back to table of contents](../index.md)


# Software architecture

The software architecture is closely modeled on the structure of a GRF file. The following sections give an overview of the GRF contents to help make sense of the code.

The format of a GRF file is pretty simple: it is nothing more than a long list of variable length records. Some of the records are pseudo-sprites (Action01, Action02, ... in the NewGRF specs) containing information about how various aspects of the game should behave, which sprites to draw, and many other things. The rest of the records are binary blobs containing mostly images (RealSprites), but sometimes sound effects. The GRF file comes in two distinct formats: Container1 and Container2. 

## Container1 format 

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

## Container2 format

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

## Software representation

The software design mimics the structure of a Container2 format file. When reading a Container1 file, it automatically generates the necessary SpriteIndex records, and invents IDs for the sprites. 

```bash
NewGRFData
  - std::vector<Record>
      - Action14Record
      - Action08Record
      - ... 
      - Action01Record 
          - std::vector<Record>
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

- The basic structure of the GRF file is captured in a NewGRFData object. This holds a vector of "top-level" records. 
- Some types of records, such as Action01Record, are basically containers/parents for sprites (i.e. SpriteIndexRecords), sound effects, and one or two others. 
- The sprites are not placed at the top-level, but held as children of the containing record, in another vector.
- The graphics section is represented by a map which associates each sprite index with a vector of one or more sprite records.

Each type of record is represented by a distinct class which is derived from a common base class. Each type implements methods to read/write itself from/to a file stream. 
- There are two read methods: 
  - one decodes the binary format for the GRF record (`read()`); 
  - the other parses the text format for the corresponding YAGL record (`parse()`). 
- There are also two write methods: 
  - one encode the binary format for the GRF record (`write()`); 
  - the other prints the text format for the corresponding YAGL record (`print()`). 
  
In some places, records are broken up recursively into collections of smaller data structures. Those structures often implement their own `read()`, `write()`, `print()` and `parse()` methods. Using Object Orientation is not everyone's cup of tea, but it makes serialisation code much simpler. 

And that's pretty much the whole program in a nutshell.

## Parsing YAGL

The YAGL script is run through a lexer to create a list of tokens representing strings, numbers, identifiers and assorted special symbols such as colons, braces, etc. Whitespace and comments are removed. 

The list of tokens is then de-serialised (in the `parse()` methods) to create an in-memory representation (as described above), rather than parsed in the traditional sense. There is no explicit language definition in terms of abstract production rules. Instead, the list of tokens is interpreted recursively with local lookup tables and other objects. This is analogous to the way in which the binary GRF file is read to create the in-memory representation. Reading the YAGL in this way probably results in more verbose code, but seemed like the right approach in this case. 

Perhaps it makes sense to regard the in-memory representation itself as being an abstract syntax tree: it certainly contains all the information necessary to generate either a binary GRF file or the equivalent YAGL script. But the way it is created has more in common with de-serialisation than rules-based parsing.

One of the goals for the parsing is to allow each different type of property to have its own specific mini-parser (the code calls these Descriptors). The idea is that, for example, every bit field can be represented with a series of property-specific identifiers ORed together (e.g. `climate: Arctic | Temperate`). It is simple to maintain a particular descriptor without affecting any other part of the YAGL de-serialisation. Descriptors can be created to represent any data type, but common ones are numbers, enumerations, bitfields, GRF "labels" (32-bit ID used for many things), and array of numbers and other things. Special case descriptors include snow lines and bridge tables.

Descriptors are also used for print the value of a property when generating YAGL. 

### GRF Strings

Strings in GRF files can be encoded in either not-quite-Latin1 or not-quite-UTF8 (prefixed with an upper case thorn character: **Þ**). In both cases the strings may contain binary control codes which have special functions when the strings are rendered in-game. 

Strings are converted to a human-readable pure-UTF8 format when generating YAGL. All the control codes are replaced with escaped string names (e.g. `"{blue}This text is blue. {red}This text is red."`). Braces are used to escape control sequences, and a double open brace is used to create a literal single open brace. Double quotes are escaped with `{dq}` as this symbol is used to delimit strings in YAGL. Double quotes are not special characters in GRF strings.

The string conversion is done in two stages. 

- Both Latin1 and UTF8 strings first are converted to UTF16. This unifies the two string types and makes life a little simple as a `char16_t` type can represent every code point in the Basic Multilingual Plane (we ignore the supplementary planes for now), and the control codes are represented more cleanly by private-use codepoints, **U+E0xx**. The **Þ** prefix is removed.

- The second stage replaces the control codes and their arguments with string representations, and then the whole thing is converted to UTF8. 

This procedure might be overkill, but the primary goal was correct function. It can be refactored later. 

Parsing a string is more or less the same procedure in reverse. The UTF16 intermediate string is checked to see if it contains any characters (other than control codes) which are not Latin1. This determines whether the GRF string is encoded as Latin1 or UTF8. UTF8 strings are prefixed with a **Þ** character.

Note that the non-unique nature of string encoding means it is very likely that using **yagl** to decode and immediately re-encode a GRF will result in different binary files, even though they are semantically identical - a bit of a pain for testing.

### Assorted details

- The map of RealSprites is walked to generate spritesheets before writing out the YAGL, this is because each sprite's position in the spritesheet needs to be calculated before the YAGL can be printed. Some sprites include both RGBA and palette information (a mask). These result in two images in different spritesheets.

- Some of the pseudo-sprites have very simple flat data structures, and are quite simple to read and write. Others such as, for example, Action14, are more complicated. In such cases there may be additional classes or structures intended to break down the task of reading and writing the data.

- Aside from strings, one or two other features have non-unique encodings, such as extended bytes. There may be cases in which encoded RealSprites have a different format due to being chunked or no chunked. 
