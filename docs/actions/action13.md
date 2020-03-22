[Back to table of contents](../index.md)

# Action 13

Translating regular strings is easy by just overwriting them with an Action04 (`strings` block), however this is not possible for translating GRF-specific strings in the 0xD000 or 0xDC00 range of IDs. Instead, these must be translated with this Action13 (`grf_strings`). 


```bash
grf_strings<"GRFX", en_US, 0xC504> // <grf_id, language, first_id> Action13, English (US)
{
    /*0xC504*/ "Roofs"; 
    /*0xC505*/ "Platform"; 
    /*0xC506*/ "Benches"; 
    /*0xC507*/ "Parking lot (front)"; 
    /*0xC508*/ "Parking lot (back)"; 
    /*0xC509*/ "Flat roofs"; 
    /*0xC50A*/ "Glass roofs"; 
    /*0xC50B*/ "Overpass"; 
    /*0xC50C*/ "Station Hall (small)"; 
    /*0xC50D*/ "Station Hall (large)"; 
    /*0xC50E*/ "Underpass"; 
}
```

The block name `grf_strings` is decorated with the following information (in angle brackets):
- The ID of the GRF (in string form) for which strings are being modified.
- The short code (`en_GB` in the example) for the language to which the strings relate. 
- The string ID of the first string that is being replaced. 

See [Action04](action04.md) for details of string codes and languages codes.

The block contains any number of strings presented in a readable format (all control codes are replaced with human-readable text in braces). 

In the example, each string is preceded with an inline comment which indicates the ID of the string that is being replaced. This is there as a guide and is not actually necessary. Comments are ignored when encoding YAGL.

## Links

The block structure described above is directly related to the corresponding record in the NewGRF specifications:

- [Action13 NewGRF specifications](https://newgrf-specs.tt-wiki.net/wiki/Action13)

