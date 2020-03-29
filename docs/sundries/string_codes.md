[Back to table of contents](../index.md)

# Strings and string codes

Strings in NewGRF files are in one of two basic formats: Latin-1 and UTF8. 

- Latin-1 strings contain only characters that are found in English and a few other languages: each character is one byte.

- UTF8 strings can contains any characters (code points) from the Basic Multilingual Plane, and can be used to represent any unicode string: each code point is two bytes, and is encoded in UTF8 as a series of one or more bytes. 

Where NFO and NML require an upper case thorn character (`"Þ"`) at the beginning to indicate that a string is encoded in UTF8, **yagl** makes this determination automatically. **yagl** internally treats evert string as if it might need to be encoded as UTF8, but then checks whether it actually contains any characters that would actually require this encoding. The thorn is prepended to UTF8 encoded strings as OpenTTD expects.

Strings may contain control codes to alter the text colour and to do a number of other things. All the control codes are entered into the string in a human-readable format escaped with braces. For example `"Hello, {new-line}World!"` and `"{red}Red text, {white}White text and {blue}Blue text."`. Some control codes obtain values from an in-game stack, but this does not affect the strings in YAGL. A few control codes have one or two arguments, which are entered as byte values follow the name of the code. The following sections list all the permitted control codes.

A couple of special cases:

- Strings are delimited by double quotes in YAGL. If you actually want a double quote character inside your string, use the code `"{dq}"`. It's code point value is 0x22, which is the Latin-1 and UTF8 code for a double quote character.

- Control codes are delimited by braces in YAGL. If you actually want braces inside your string, use the sequence `"{{"` where you want a single `"{"`. This prevents the open brace being treated as the start of a control code, and the closing brace, if any, will be treated as a normal character. Braces cannot be used in Latin-1 strings because their code point values are 0x7B and 0x7D, which are already assigned to other control codes. They print regular glyphs for UTF8 strings, so **yagl** forces strings containing braces to be encoded as UTF8.

## Control codes

Any arguments are placed inside the braces with name of the control code. For example, `"{xy-offs 0x10 0x12}"`.

| Value | Text | Args | Description |
|-------|------|----------|-------------|
| 0x01 | `{x-off}`         | 1 | X offset in next byte of string (variable space) |
| 0x0D | `{new-line}`      | 0 | New line |
| 0x0E | `{small-font}`    | 0 | Set small font size |
| 0x0F | `{large-font}`    | 0 | Set large font size |
| 0x1F | `{xy-offs}`       | 2 | X and Y offsets in next two bytes of string |
| 0x22 | `{dq}`            | 0 | Double quote |
| 0x7B | `{sd}`            | 0 | Print signed dword |
| 0x7C | `{sw}`            | 0 | Print signed word |
| 0x7D | `{sb}`            | 0 | Print signed byte |
| 0x7E | `{uw}`            | 0 | Print unsigned word |
| 0x7F | `{sd-currency}`   | 0 | Print dword in currency units |
| 0x80 | `{substring1}`    | 0 | Print substring (text ID from stack) |
| 0x81 | `{substring2}`    | 2 | Print substring (text ID in next 2 bytes of string) |
| 0x82 | `{d-m-year}`      | 0 | Print date (day, month, year) (based on year 1920) |
| 0x83 | `{m-year}`        | 0 | Print short date (month and year) (based on year 1920) |
| 0x84 | `{sw-speed}`      | 0 | Print signed word in speed units |
| 0x85 | `{discard}`       | 0 | Discard next word from stack |
| 0x86 | `{rotate}`        | 0 | Rotate down top 4 words on stack |
| 0x87 | `{sw-litres}`     | 0 | Print signed word in litres |
| 0x88 | `{blue}`          | 0 | Blue |
| 0x89 | `{lt-gray}`       | 0 | Light Gray |
| 0x8A | `{gold}`          | 0 | Light Orange ('Gold') |
| 0x8B | `{red}`           | 0 | Red |
| 0x8C | `{purple}`        | 0 | Purple |
| 0x8D | `{gray-green}`    | 0 | Gray-Green |
| 0x8E | `{orange}`        | 0 | Orange |
| 0x8F | `{green}`         | 0 | Green |
| 0x90 | `{yellow}`        | 0 | Yellow |
| 0x91 | `{lt-green}`      | 0 | Light Green |
| 0x92 | `{red-brown}`     | 0 | Red-Brown |
| 0x93 | `{brown}`         | 0 | Brown |
| 0x94 | `{white}`         | 0 | White |
| 0x95 | `{lt-blue}`       | 0 | Light Blue |
| 0x96 | `{dk-gray}`       | 0 | Dark Gray |
| 0x97 | `{mauve}`         | 0 | Mauve (grayish purple) |
| 0x98 | `{black}`         | 0 | Black |
| 0x99 | `{switch-cc}`     | 1 | Switch to company colour that follows in next byte (enabled by enhancegui) |
| 0x9A | `{ext}`           | 0 | Extended format code in next byte: |
| 0x9E | `{euro}`          | 0 | Euro character |
| 0x9F | `{Y-umlaut}`      | 0 | Capital Y umlaut |
| 0xA0 | `{scroll-up}`     | 0 | Scroll button up |
| 0xAA | `{scroll-down}`   | 0 | Scroll button down |
| 0xAC | `{tick}`          | 0 | Tick mark |
| 0xAD | `{x}`             | 0 | X mark |
| 0xAF | `{scroll-right}`  | 0 | Scroll button right |
| 0xB4 | `{train}`         | 0 | Train symbol |
| 0xB5 | `{truck}`         | 0 | Truck symbol |
| 0xB6 | `{bus}`           | 0 | Bus symbol |
| 0xB7 | `{plane}`         | 0 | Plane symbol |
| 0xB8 | `{ship}`          | 0 | Ship symbol |
| 0xB9 | `{super-1}`       | 0 | Superscript -1 |
| 0xBC | `{small-up}`      | 0 | Small scroll button up |
| 0xBD | `{small-down}`    | 0 | Small scroll button down |

## Extended control codes

Any arguments are placed inside the braces with name of the control code. For example, `"{ext b-hex}"` or `"{ext push-w 0x34 0x12}"`.

| Value | Text | Args     | Description |
|-------|------|----------|-------------|
| 0x00 | `{64-currency0}`  | 0 | Display 64-bit value from stack in currency units |
| 0x01 | `{64-currency1}`  | 0 | Display 64-bit value from stack in currency units |
| 0x02 | `{ignore-colour}` | 0 | Ignore next colour byte. Multiple instances will skip multiple colour bytes. |
| 0x03 | `{push-w}`        | 2 | WORD Push WORD onto the textref stack |
| 0x04 | `{unprint-b}`     | 1 | BYTE Un-print the previous BYTE characters. | 
| 0x05 | `{internal5}`     | 0 | For internal use only. Not valid in GRF files. |
| 0x06 | `{b-hex}`         | 0 | Print byte in hex |
| 0x07 | `{w-hex}`         | 0 | Print word in hex |
| 0x08 | `{d-hex}`         | 0 | Print dword in hex |
| 0x09 | `{internal9}`     | 0 | For internal use only. Usage in NewGRFs will most likely crash TTDPatch. |
| 0x0A | `{internalA}`     | 0 | For internal use only. Usage in NewGRFs will most likely crash TTDPatch. |
| 0x0B | `{64-hex}`        | 0 | Print 64-bit value in hex |
| 0x0C | `{station}`       | 0 | Print name of station with id in next textrefstack word |
| 0x0D | `{uw-tonnes}`     | 0 | Print unsigned word in tonnes |
| 0x0E | `{gender}`        | 1 | Set gender of string, NewGRF internal ID in next byte. Must be first in a string. |
| 0x0F | `{case}`          | 1 | Select case for next substring, NewGRF internal ID in next byte |
| 0x10 | `{list-value}`    | 1 | Begin choice list value, NewGRF internal ID in next byte |
| 0x11 | `{list-default}`  | 0 | Begin choice list default |
| 0x12 | `{end-list}`      | 0 | End choice list |
| 0x13 | `{gender-list}`   | 1 | Begin gender choice list, stack offset of substring to get gender from in next byte |
| 0x14 | `{case-list}`     | 0 | Begin case choice list |
| 0x15 | `{plural-list}`   | 1 | Begin plural choice list, stack offset of value to get plural for in next byte |
| 0x16 | `{dw-date}`       | 0 | Print dword as date (day, month, year) (based on year 0) |
| 0x17 | `{dw-short-date}` | 0 | Print dword as short date (month and year) (based on year 0) |
| 0x18 | `{uw-hp}`         | 0 | Print unsigned word in horse power |
| 0x19 | `{uw-volume}`     | 0 | Print unsigned word as short volume |
| 0x1A | `{uw-weight}`     | 0 | Print unsigned word as short weight |


## Links 

- [NewGRF string control codes](https://newgrf-specs.tt-wiki.net/wiki/StringCodes)
