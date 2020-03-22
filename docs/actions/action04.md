[Back to table of contents](../index.md)

# Action 04

Action04 (`strings`) is used to define strings, e.g vehicle, house or industry names

When making new vehicle graphics, you also need to name the new vehicles, or they'll show up with their original name from TTD. However, custom vehicle names assigned by the player in-game (or for TTDPatch also via TTD's vehicle.dat) will always take precendence.


```bash
// Record #24
strings<Trains, en_GB, 0xD08F*> // <feature, language, first_id> Action04, English (GB)
{
    /*0xD08F*/ "{black}Service: {yellow}local passengers{new-line}{black}Axle Weight: {yellow}low (class B){new-line}{black}Length: {yellow}3 units (1.7){new-line}{black}Loading Time: {yellow}{sd} day{ext plural-list 0xFF80}{ext list-value 0x0001}{ext list-default}s{ext end-list}";
    /*0xD090*/ "{black}Service: {yellow}local passengers{new-line}{black}Axle Weight: {yellow}low (class B){new-line}{black}Length: {yellow}2 units (1.2){new-line}{black}Loading Time: {yellow}{sd} day{ext plural-list 0xFF80}{ext list-value 0x0001}{ext list-default}s{ext end-list}";
    /*0xD091*/ "{black}Service: {yellow}executive{new-line}{black}Axle Weight: {yellow}low (class B){new-line}{black}Length: {yellow}1 unit (0.5){new-line}{black}Loading Time: {yellow}{sd} day{ext plural-list 0xFF80}{ext list-value 0x0001}{ext list-default}s{ext end-list}";
    ... omitted - more strings
}
```

The block name `strings` is decorated with the following information (in angle brackets):
- The type of feature (`Trains` in the example) for which the strings are created.
- The short code (`en_GB` in the example) for the language to which the strings relate. 
- The OpenTTD string ID of the first string that is being replaced. A star indicates that the string ID should be stored as a WORD (2-byte value). IDs are otherwise stored as bytes or extended byte depending on their values. [This feature was added to preserve the information from the decode GRF, but is not strictly necessary for **yagl** to work: it could just always used the WORD format and save some fuss.]

The block contains any number of strings presented in a readable format (all control codes are replaced with human-readable text in braces). 

In the example, each string is preceded with an inline comment which indicates the ID of the string that is being replaced. This is there as a guide and is not actually necessary. Comments are ignored when encoding YAGL.

**Features**

| Feature | Value |
|---------|-------|
| `Trains`          | 0x00 |
| `Vehicles`        | 0x01 |
| `Ships`           | 0x02 |
| `Aircraft`        | 0x03 |
| `Stations`        | 0x04 |
| `Canals`          | 0x05 |
| `Bridges`         | 0x06 |
| `Houses`          | 0x07 |
| `GlobalSettings`  | 0x08 |
| `IndustryTiles`   | 0x09 |
| `Industries`      | 0x0A |
| `Cargos`          | 0x0B |
| `SoundEffects`    | 0x0C |
| `Airports`        | 0x0D |
| `Signals`         | 0x0E |
| `Objects`         | 0x0F |
| `Railtypes`       | 0x10 |
| `AirportTiles`    | 0x11 |
| `RoadTypes`       | 0x12 |
| `TramTypes`       | 0x13 |

**Language codes**

TODO is this list complete and correct?

 | Value | Code     | Name |
 |-------|----------|------|
 | 0x00  | `en_US`   | English (US) |
 | 0x01  | `en_GB`   | English (GB) |
 | 0x02  | `de_DE`   | German |
 | 0x03  | `fr_FR`   | French |
 | 0x04  | `es_ES`   | Spanish |
 | 0x05  | `eo_XX`   | Esperanto |
 | 0x06  | `io_XX`   | Ido |     
 | 0x07  | `ru_RU`   | Russian |
 | 0x08  | `ga_IE`   | Irish |   
 | 0x09  | `mt_MT`   | Maltese |
 | 0x0A  | `ta_IN`   | Tamil |
 | 0x0B  | `cv_RU`   | Chuvash | 
 | 0x0C  | `zh_TW`   | Chinese (Traditional) |
 | 0x0D  | `sr_BA`   | Serbian |
 | 0x0E  | `nn_NO`   | Norwegian (Nynorsk) |
 | 0x0F  | `cy_GB`   | Welsh |
 | 0x10  | `be_BY`   | Belarusian |
 | 0x11  | `mr_IN`   | Marathi |
 | 0x12  | `fo_FO`   | Faroese |
 | 0x13  | `gd_GB`   | Scottish Gaelic | 
 | 0x14  | `ar_EG`   | Arabic (Egypt) |
 | 0x15  | `cs_CZ`   | Czech |
 | 0x16  | `sk_SK`   | Slovak |
 | 0x18  | `bg_BG`   | Bulgarian |
 | 0x1B  | `af_ZA`   | Afrikaans |
 | 0x1E  | `el_GR`   | Greek |
 | 0x1F  | `nl_NL`   | Dutch |
 | 0x21  | `eu_ES`   | Basque |
 | 0x22  | `ca_ES`   | Catalan |
 | 0x23  | `de_LU`   | Luxembourgish | 
 | 0x24  | `hu_HU`   | Hungarian |
 | 0x26  | `mk_MK`   | Macedonian |
 | 0x27  | `it_IT`   | Italian |
 | 0x28  | `ro_RO`   | Romanian |
 | 0x29  | `is_IS`   | Icelandic |
 | 0x2A  | `lv_LV`   | Latvian |
 | 0x2B  | `lt_LT`   | Lithuanian |
 | 0x2C  | `sl_SI`   | Slovenian |
 | 0x2D  | `da_DK`   | Danish |
 | 0x2E  | `sv_SE`   | Swedish |
 | 0x2F  | `nb_NO`   | Norwegian (Bokmal) |
 | 0x30  | `pl_PL`   | Polish |
 | 0x31  | `gl_ES`   | Galician |
 | 0x32  | `fy_NL`   | Frisian |
 | 0x33  | `uk_UA`   | Ukrainian |
 | 0x34  | `et_EE`   | Estonian |
 | 0x35  | `fi_FI`   | Finnish |
 | 0x36  | `pt_PT`   | Portuguese |
 | 0x37  | `pt_BR`   | Brazilian Portuguese |
 | 0x38  | `hr_HR`   | Croatian |
 | 0x39  | `ja_JP`   | Japanese |
 | 0x3A  | `ko_KR`   | Korean |
 | 0x3C  | `ms_MY`   | Malay |
 | 0x3D  | `en_AU`   | English (AU) |
 | 0x3E  | `tr_TR`   | Turkish |
 | 0x42  | `th_TH`   | Thai |
 | 0x54  | `vi_VN`   | Vietnamese |
 | 0x55  | `es_MX`   | Mexican Spanish |
 | 0x56  | `zh_CN`   | Chinese (Simplified) |
 | 0x5A  | `id_ID`   | Indonesian |
 | 0x5C  | `ur_PK`   | Urdu |
 | 0x61  | `he_IL`   | Hebrew |
 | 0x62  | `fa_IR`   | Persian |
 | 0x66  | `latin`   | Latin | 
 | 0x7F  | `default` | Default |

**Control codes**

Any arguments are placed inside the braces with name of the control code.

| Value | Text | Args | Description |
|-------|------|----------|-------------|
| 0x01 | {x-off}         | 1 | X offset in next byte of string (variable space) |
| 0x0D | {new-line}      | 0 | New line |
| 0x0E | {small-font}    | 0 | Set small font size |
| 0x0F | {large-font}    | 0 | Set large font size |
| 0x1F | {xy-offs}       | 2 | X and Y offsets in next two bytes of string |
| 0x22 | {dq}            | 0 | Double quote |
| 0x7B | {sd}            | 0 | Print signed dword |
| 0x7C | {sw}            | 0 | Print signed word |
| 0x7D | {sb}            | 0 | Print signed byte |
| 0x7E | {uw}            | 0 | Print unsigned word |
| 0x7F | {sd-currency}   | 0 | Print dword in currency units |
| 0x80 | {substring1}    | 0 | Print substring (text ID from stack) |
| 0x81 | {substring2}    | 2 | Print substring (text ID in next 2 bytes of string) |
| 0x82 | {d-m-year}      | 0 | Print date (day, month, year) (based on year 1920) |
| 0x83 | {m-year}        | 0 | Print short date (month and year) (based on year 1920) |
| 0x84 | {sw-speed}      | 0 | Print signed word in speed units |
| 0x85 | {discard}       | 0 | Discard next word from stack |
| 0x86 | {rotate}        | 0 | Rotate down top 4 words on stack |
| 0x87 | {sw-litres}     | 0 | Print signed word in litres |
| 0x88 | {blue}          | 0 | Blue |
| 0x89 | {lt-gray}       | 0 | Light Gray |
| 0x8A | {gold}          | 0 | Light Orange ('Gold') |
| 0x8B | {red}           | 0 | Red |
| 0x8C | {purple}        | 0 | Purple |
| 0x8D | {gray-green}    | 0 | Gray-Green |
| 0x8E | {orange}        | 0 | Orange |
| 0x8F | {green}         | 0 | Green |
| 0x90 | {yellow}        | 0 | Yellow |
| 0x91 | {lt-green}      | 0 | Light Green |
| 0x92 | {red-brown}     | 0 | Red-Brown |
| 0x93 | {brown}         | 0 | Brown |
| 0x94 | {white}         | 0 | White |
| 0x95 | {lt-blue}       | 0 | Light Blue |
| 0x96 | {dk-gray}       | 0 | Dark Gray |
| 0x97 | {mauve}         | 0 | Mauve (grayish purple) |
| 0x98 | {black}         | 0 | Black |
| 0x99 | {switch-cc}     | 1 | Switch to company colour that follows in next byte (enabled by enhancegui) |
| 0x9A | {ext}           | 0 | Extended format code in next byte: |
| 0x9E | {euro}          | 0 | Euro character |
| 0x9F | {Y-umlaut}      | 0 | Capital Y umlaut |
| 0xA0 | {scroll-up}     | 0 | Scroll button up |
| 0xAA | {scroll-down}   | 0 | Scroll button down |
| 0xAC | {tick}          | 0 | Tick mark |
| 0xAD | {x}             | 0 | X mark |
| 0xAF | {scroll-right}  | 0 | Scroll button right |
| 0xB4 | {train}         | 0 | Train symbol |
| 0xB5 | {truck}         | 0 | Truck symbol |
| 0xB6 | {bus}           | 0 | Bus symbol |
| 0xB7 | {plane}         | 0 | Plane symbol |
| 0xB8 | {ship}          | 0 | Ship symbol |
| 0xB9 | {super-1}       | 0 | Superscript -1 |
| 0xBC | {small-up}      | 0 | Small scroll button up |
| 0xBD | {small-down}    | 0 | Small scroll button down |

**Extended control codes**

Any arguments are placed inside the braces with name of the control code.

| Value | Text | Args | Description |
|-------|------|----------|-------------|
| 0x00 | {64-currency0}  | 0 | Display 64-bit value from stack in currency units |
| 0x01 | {64-currency1}  | 0 | Display 64-bit value from stack in currency units |
| 0x02 | {ignore-colour} | 0 | Ignore next colour byte. Multiple instances will skip multiple colour bytes. |
| 0x03 | {push-w}        | 2 | WORD Push WORD onto the textref stack |
| 0x04 | {unprint-b}     | 1 | BYTE Un-print the previous BYTE characters. | 
| 0x05 | {internal5}     | 0 | For internal use only. Not valid in GRF files. |
| 0x06 | {b-hex}         | 0 | Print byte in hex |
| 0x07 | {w-hex}         | 0 | Print word in hex |
| 0x08 | {d-hex}         | 0 | Print dword in hex |
| 0x09 | {internal9}     | 0 | For internal use only. Usage in NewGRFs will most likely crash TTDPatch. |
| 0x0A | {internalA}     | 0 | For internal use only. Usage in NewGRFs will most likely crash TTDPatch. |
| 0x0B | {64-hex}        | 0 | Print 64-bit value in hex |
| 0x0C | {station}       | 0 | Print name of station with id in next textrefstack word |
| 0x0D | {uw-tonnes}     | 0 | Print unsigned word in tonnes |
| 0x0E | {gender}        | 1 | Set gender of string, NewGRF internal ID in next byte. Must be first in a string. |
| 0x0F | {case}          | 1 | Select case for next substring, NewGRF internal ID in next byte |
| 0x10 | {list-value}    | 1 | Begin choice list value, NewGRF internal ID in next byte |
| 0x11 | {list-default}  | 0 | Begin choice list default |
| 0x12 | {end-list}      | 0 | End choice list |
| 0x13 | {gender-list}   | 1 | Begin gender choice list, stack offset of substring to get gender from in next byte |
| 0x14 | {case-list}     | 0 | Begin case choice list |
| 0x15 | {plural-list}   | 1 | Begin plural choice list, stack offset of value to get plural for in next byte |
| 0x16 | {dw-date}       | 0 | Print dword as date (day, month, year) (based on year 0) |
| 0x17 | {dw-short-date} | 0 | Print dword as short date (month and year) (based on year 0) |
| 0x18 | {uw-hp}         | 0 | Print unsigned word in horse power |
| 0x19 | {uw-volume}     | 0 | Print unsigned word as short volume |
| 0x1A | {uw-weight}     | 0 | Print unsigned word as short weight |

## Links

The block structure described above is directly related to the corresponding record in the NewGRF specifications:

- [Action04 NewGRF specifications](https://newgrf-specs.tt-wiki.net/wiki/Action4)
