[Back to table of contents](index.md)

# YAGL syntax

YAGL is a simple block structured script. Every record (i.e. pseudo-sprite) in the GRF is represented by a single block, which contains properties and maybe nested sub-blocks. A typical block is basically an identifying name indicating its type followed by a a pair of braces. As follows:

```bash
block_name
{
    // Block contents
}
```

A block may be "decorated" with information about the particular instance of that type of block:

```bash
block_name<value1, value2, ...> // One or more values giving extra data about this block
{
    // Block contents
}
```

## Syntax elements

There are a small number of different kinds of elements which may appear in a YAGL file. These are easy to distinguish:

**Identifiers**: An identifier is a built-in name or keyword for something which **yagl** already understands. These are used for block names, property names, enumeration names, and so on. An identifer is any sequence of alphanumeric characters, and may include underscores, which does not begin with a digit. Identifiers are represented by the regular expression `[a-zA-Z_][a-zA-Z_0-9]*`. `block_name`, `expression`, `switch`, `Trains` and `ranges` are all examples of identifiers. `123something` is not an identifier because it begins with a digit. `Something456` is a valid identifier.

**Numbers**: Numbers are used to represent the values of properties and other things. There are four supported number formats: decimal, octal, hexadecimal and binary. The four formats can be used interchangeably. Just use whatever is most convenient for your purposes:

- A decimal (base 10) number is any sequence of numeric characters which does not begin with a zero. Decimals are represented by the regular expression `[1-9][0-9]*`. `123` and `456` are decimal numbers.

- An octal (base 8) number is any sequence of numeric digits less than 8 which begins with a zero. Octals are represented by the regular expression `0[0-7]*`. `012` is octal for decimal `10`. Digits `8` and `9` are not allowed in octal numbers.

- A hexadecimal (base 16) number is any sequence of digits or the letters A-F in upper or lower case (these represent values 10, 11, 12, 13, 14 and 15, respectively). Hex numbers are prefixed with `0x` to avoid ambiguity. Hex numbers are represented by the regular expression `0x[A-Fa-f0-9]*`.  `0x12` is hexadecimal for decimal `18`.

- A binary (base 2) number is any sequence of zeroes or ones. Binary numbers are prefixed with `0b` to avoid ambiguity. Binary numbers are represented by the regular expression `0b[01]*`. `0b101010` is binary for decimal `42`.

**Strings**: strings are user-provided names, descriptions, translations and many othes things intended to be used in game. Strings are sequences of any characters which are delimited by double quotes. `"Hello, World!"`, `"Concrete Bridge"` and `"There is something funny going on in Mordor..."` are all examples of strings. 

**Symbols**: Pretty much everything else is a symbol. These are single or double non-alphanumeric characters such as `;` or `>>`. Symbols are used to give structure to the data, delimit records, lists, and so on. Quite a large number of symbols are used in YAGL.

**Comments**: A comment is any sequence of characters which follows `//` until the end of the current line. This is exactly the same as comments in C++ and a number of other languages. Comments may also begin with `/*`, in which case all the following characters, including line breaks, are part of the comment, until the terminating sequence `*/` is seen. Comments have no significance at all for **yagl**: they are simply ignored.

**Whitespace**: Spaces, new lines and tabs are all regarded as white space. Whitespace is used to separate and layout the other elements, just as in most programming languages. Whitespace has no significance at all for **yagl** and is completely ignored.


## A real example

```bash
// Record #2
grf // Action08
{
    grf_id: "\xFB\xFB\x06\x01";
    version: GRF8;
    name: "Dutch Trainset 2.1.0";
    description: "{lt-gray}Dutch Trains for OpenTTD {new-line}{black}First vehicle: 1839.{new-line}{new-line}(c)Dutch Trainset Team {new-line}License: GPLv2 or higher. {new-line}See readme for details.";
}
```

In this example, which represents an Action08 record, we have:

- `grf`, `grf_id`, `version`, `GRF8`, `name` and `description` are all identifiers.
- `"\xFB\xFB\x06\x01"` is a string representation of the GRF's GRFID. 
- Note that a string can contain byte values directly. These are written as two hex digits preceded by `\x`. (similar to the regular hex prefix `0x` but less ambiguous in this context).
- `"Dutch Trainset 2.1.0"` is another string.
- `{`, `}`, `:` and `;` are symbols.
- There are no numbers in this in this example.

The various elements are organised to represent the particular structure of an Action08 record. There are four properties:

- Property `grf_id` is given the value `"\xFB\xFB\x06\x01"`. GRFIDS are represented internally as four byte numbers (double words), but can be represented in YAGL as numbers, strings or identifiers (they are a special case in this regard). In this string represention, the four bytes in the dword are `0xFB`, `0xFB`, `0x06` and `0x01`.

- Property `version` is given the value `GRF8`. This is an enumeration value representing the NewGRF specs version used to create the GRF. Versions are generally backward compatible.

- Property `name` is given the value `"Dutch Trainset 2.1.0"`. This is the names of the GRF as it will appears in OpenTTD.

- Property `description` is also given a value. Note in this case that the string contains substrings such as `{lt-gray}` and `{new-line}`. These represent string control codes. **yagl** translates this human readable format into the binary representatin that OpenTTD uses. Strings may contain any unicode characters you wish. It is not necessary to prefix unicode strings with capital thorn as mentioned in the specs: **yagl** determines automatically whether it is necessary.

Take a look at [Action08](https://newgrf-specs.tt-wiki.net/wiki/Action8) to see how the properties described here relate to the specification.

For completeness, the following code shows the same record in NFO. 

```bash
    2 * 152	 08 08 "��" 06 01 "Dutch Trainset 2.1.0" 00 89 "Dutch "
	 "Trains for OpenTTD " 0D 98 "First vehicle: "
	 "1839." 0D 0D "(c)Dutch Trainset Team " 0D "License: "
	 "GPLv2 or higher. " 0D "See readme for "
	 "details." 00
```

 - The number of the NFO record is shown as 2. Records in YAGL are not specifically numbered. 
 - The length is shown as 152 bytes. This is not required in the YAGL, which automatically calculates the length of the binary data that goes into the GRF. 
 - We can see that the `{new-line}` control codes are directly represented by the hex value `0D` (NFO doesn't use the `0x` prefixes). We can also see that the `{lt-gray}` code is directly represented by `89`. See [String codes](https://newgrf-specs.tt-wiki.net/wiki/StringCodes) for details of string codes in NFO
 - All the other items match - there is a direct correspondence between the YAGL and the NFO.

The purpose of the other documentation is really just to show how each type of record is related to the NewGRF specs, and to give details of enumerations and other identifiers, such as all the string code replacements.
