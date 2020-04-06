[Back to table of contents](../index.md)

# Action 14

Action14 (`optional_info`) allows us to specify additional information about the GRF. Every piece of information is considered optional and non-essential, thus OpenTTD/TTDPatch will ignore unknown parts and not display any error. The action is also meant to allow interfacing non-official extensions/patches for OpenTTD.

Currently this action can be used to define:

- Translations for the name and description of the GRF. (Those from action 8)
- The palette (default (=DOS), legacy (=Windows)) the GRF needs, or whether the palette does not matter.
- The colour depth (8bpp, 32bpp) the GRF needs
- The number, format and purpose of GRF parameters. (allowed values, names, descriptions, ...)
- Version information and compatibility to older versions of the same GRF.
- Website url that can be opened in-game.

Action14 is processed while scanning for GRFs and their Action08 descriptions. This scanning stops when encountering an Action08, thus Action14 needs to appear earlier in the GRF. 

The following example includes a few comments for guidance generated automatically when a GRF is decoded. The YAGL format for this record is as follows:


```bash
optional_info // Action14
{
    INFO: 
    {
        DESC: fr_FR, "{lt-gray}Trains Hollandais pour OpenTTD{new-line}{black}Premier
            véhicule : 1839.{new-line}{new-line}(c)Dutch Trainset Team{new-line}Licence:
            GPLv2 ou plus.{new-line}Voir le readme pour pour plus de détails."; // French
        URL_: default, "http://www.tt-forums.net/viewtopic.php?t=59353"; // Default
        VRSN: [ 0xE1 0x1C 0x00 0x00 ];
        MINV: [ 0x7F 0x1C 0x00 0x00 ];
        NPAR: [ 0x06 ];
        PARA: 
        {
            0x00000000: 
            {
                NAME: default, "Use English vehicle nicknames"; // Default
                NAME: fr_FR, "Traduire les surnoms en français"; // French
                DESC: default, "Enable this to have the vehicle nicknames translated to
                    English. When disabled, the vehicle nicknames will be in Dutch."; // Default
                DESC: fr_FR, "Certaines locomotives ont des surnoms en néerlandais. Ce
                    paramètre permet de les traduire (approximativement) en français."; // French
                TYPE: [ 0x01 ];
                MASK: [ 0x00 0x00 0x01 ];
                DFLT: [ 0x00 0x00 0x00 0x00 ];
            }
            // ... more omitted
        }
        // ... more omitted
    }
    // ... more omitted
}
```

#### Properties

The block name is `optional_info` and contains any number each of three different element type `container`, `string` and `binary`. Every item is named with a [GRF label](../sundries/grf_labels.md).

| Name | Description |
|-|-|
| `string`    | The format for a string element is `<GRF label>: <language>, <text>;`, See [languages](../sundries/language_codes.md) and [string codes](../sundries/string_codes.md). An example from the code above is `NAME: fr_FR, "Traduire les surnoms en français";`. |
| `binary`    | The format for a binary element is `<GRF label>: [ <byte> <byte> ... ]`. There may be any number of bytes up to 65535, though the number is usually much smaller. An example from the code above is `MINV: [ 0x7F 0x1C 0x00 0x00 ];`. |
| `container` | The third type of element is a container for `string`, `binary` and `container` elements. These allows data to be structured hierarchically. The format for a container element is `<GRF label>: { ... more elements ... }`. An example from the code above is `PARA: { ... }`. |

#### Special cases

The previous section defined the basic format for any Action14. There are a number of special cases which OpenTTD uses to set up such things as GRF parameters. These are described in detail in the [NewGRF specifications](https://newgrf-specs.tt-wiki.net/wiki/Action14), so I won't repeat the information here. But a few comments might help:

- Strings with the same GRF label can appear more than once inside the same container. This is how different language translations are defined for a given property.
- When defining parameters, the index of each parameter is used for its GRF label. The parameter is a container of other elements which define its names and other properties.
- When defining enumeration parameters, the value of the parameter is a list of strings. Each string's GRF label is the enumerator value, and the string is the name that will appear in game. The same enumerator value may appear several times in order to define the name in multiple languages.


## Links

- [NewGRF Action14 specifications](https://newgrf-specs.tt-wiki.net/wiki/Action14)
- [YAGL language codes](../sundries/language_codes.md)
- [YAGL string control codes](../sundries/string_codes.md)