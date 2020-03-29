[Back to table of contents](../index.md)

# Action 05

Action05 (`replacement_sprite_set`) defines base graphics specific sets. Action05 is used to define graphics for several new features of OpenTTD and TTDPatch. 

The following examples include a few comments for guidance generated automatically when a GRF is decoded. The YAGL format for this record is as follows:

**Example: replacement_sprite_set with real sprites:** 
```bash
// Record #273
replace_sprites<Aqueducts, 0x0000> // <new_feature_type, offset>  Action05
{
    // Replace Aqueducts sprite 0x0000
    sprite_id<0x00000113>
    {
        [62, 39, -30, -14], normal, c8bpp, "sprites\zbase_extra-8bpp-normal-0.png", [10, 10];
        [64, 45, -32, -13], normal, c32bpp | chunked, "sprites\zbase_extra-32bpp-normal-0.png", [10, 10];
        [256, 179, -128, -51], zin4, c32bpp | chunked, "sprites\zbase_extra-32bpp-zin4-0.png", [10, 10];
        [128, 90, -64, -26], zin2, c32bpp | chunked, "sprites\zbase_extra-32bpp-zin2-0.png", [10, 10];
    }
    // Replace Aqueducts sprite 0x0001
    sprite_id<0x00000114>
    {
        [62, 39, -30, -14], normal, c8bpp, "sprites\zbase_extra-8bpp-normal-0.png", [82, 10];
        [64, 45, -32, -13], normal, c32bpp | chunked, "sprites\zbase_extra-32bpp-normal-0.png", [84, 10];
        [256, 179, -128, -51], zin4, c32bpp | chunked, "sprites\zbase_extra-32bpp-zin4-0.png", [276, 10];
        [128, 90, -64, -26], zin2, c32bpp | chunked, "sprites\zbase_extra-32bpp-zin2-0.png", [148, 10];
    }
    ... 
}
```

**Example: replacement_sprite_set with recolour sprites:** 
```bash
// Record #16
replace_sprites<TwoCompanyColour, 0x0000> // <new_feature_type, offset>  Action05
{
    // Replace TwoCompanyColour sprite 0x0000
    recolour_sprite
    {
        0x50..0x57: 0xC6..0xCD;
    }
    // Replace TwoCompanyColour sprite 0x0001
    recolour_sprite
    {
        0x50..0x57: 0xC6..0xCD;
        0xC6..0xCD: 0x60..0x67;
    }
    ...
    // Replace TwoCompanyColour sprite 0x000C
    recolour_sprite
    {
        0x50..0x57: 0xC6..0xCD;
        0xC6: 0x40;
        0xC7..0xCC: 0xC0..0xC5;
        0xCD: 0x27;
    }
    ...
}
```

#### Block name 

- The block name is `replace_sprites`.

- The "decoration" in angle brackets after the block name includes:
    - The type of feature (`Aqueducts` in the example) for which the sprites are created. One of the following enumeration values is possible (see [new features](../sundries/new_features.md)):
        - `PreSignal`        (value = 0x04)
        - `Catenary`         (value = 0x05)
        - `Foundations`      (value = 0x06)
        - `TTDPatchGUI`      (value = 0x07)
        - `Canals`           (value = 0x08)
        - `OneWayArrows`     (value = 0x09)
        - `TwoCompanyColour` (value = 0x0A)
        - `TramTracks`       (value = 0x0B)
        - `SnowyTrees`       (value = 0x0C)
        - `CoastTiles`       (value = 0x0D)
        - `NewSignals`       (value = 0x0E)
        - `SlopeTrackMarks`  (value = 0x0F)
        - `AirportExtra`     (value = 0x10)
        - `RoadStops`        (value = 0x11)
        - `Aqueducts`        (value = 0x12) 
        - `AutoRail`         (value = 0x13)
        - `Flags`            (value = 0x14)
        - `OpenTTDGUI`       (value = 0x15)
        - `AirportPreview`   (value = 0x16)
        - `RailTypeTunnel`   (value = 0x17)
        - `ExtraAllBlack`    (value = 0x18)
    - The offset of the first sprite in the relevant block of sprites (`0x0000` in the example).

In other respects, the content of this record is similar to [Action01](action01.md), except that there is no concept of sprite sets. Please look there for more information. 

#### Recolour sprite block

Rather than an actual image sprite, a `replace_sprites` block can also include recolour sprites. These originated when OpenTTD supported only 8bpp images, and provide a map which shuffles the colours around the 256-entry palette. TODO how do they affect 32bpp images?

The `recolour_sprite` block contains a series of ranges which specify how modify the colours. Each range has one of two formats: `<to_index>: <from_index>;` or `<to_begin..to_end>: <from_begin..from_end>;`. The numbers are all indices in the range 0 to 255 (0x00 to 0xFF). The second format is an abbreviation. That is:

```bash
0x50..0x53: 0xC6..0xC9;
```

is equivalent to: 

```bash
0x50: 0xC6;
0x51: 0xC7;
0x52: 0xC8;
0x53: 0xC9;
```

## Links

The block structure described above is directly related to the corresponding record in the NewGRF specifications:

- [NewGRF Action05 specifications](https://newgrf-specs.tt-wiki.net/wiki/Action5)
- [YAGL new feature enumeration](../sundries/new_features.md)
- [YAGL real sprites](../graphics/real_sprite.md)
- [YAGL recolour sprites](../graphics/recolour_sprite.md)

