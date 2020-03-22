[Back to table of contents](../index.md)

# Action 12

Action12 (`unicode_glyphs`) is used to define or re-define glyphs in the unicode fonts that OpenTTD used when it renders strings. Any code point in the Unicode Basic Multilingual Plane (BMP) can be defined. That is, code points in the range 0x0000 to 0xFFFF. 

The following example includes a few comments for guidance generated automatically when a GRF is decoded. The YAGL format for this record is as follows:

```bash
// Record #348
unicode_glyphs // Action12
{
    range<NormalProp, 0x007B> // <font, base_char>
    {
        // Replace character 0x0000007B
        sprite_id<0x0000015E>
        {
            [5, 13, 0, -2], normal, c8bpp | no_crop, "sprites\zbase_extra-8bpp-normal-0.png", [320, 205];
        }
        // Replace character 0x0000007C
        sprite_id<0x0000015F>
        {
            [3, 13, 0, -2], normal, c8bpp | no_crop, "sprites\zbase_extra-8bpp-normal-0.png", [335, 205];
        }
        // Replace character 0x0000007D
        sprite_id<0x00000160>
        {
            [5, 13, 0, -2], normal, c8bpp | no_crop, "sprites\zbase_extra-8bpp-normal-0.png", [348, 205];
        }
        // Replace character 0x0000007E
        sprite_id<0x00000161>
        {
            [7, 13, 0, -2], normal, c8bpp | no_crop, "sprites\zbase_extra-8bpp-normal-0.png", [363, 205];
        }
    }
    ... omitted - more ranges
}
```

#### Replacement spriteset blocks

The main block name is `unicode_glyphs`. 

- Inside the main block are any number of sub-blocks named `range`.
    - Each `range` block name is decorated with the font type and the code point of the first character defined within the range (in angle brackets). The font type is one of the following enumeration values: 
        - `NormalProp`  (value = 0) Normal proportional font
        - `SmallProp`   (value = 1) Small proportional font
        - `LargeProp`   (value = 2) Large proportional font
        - `NormalFixed` (value = 3) Normal fixed font
    - Each `range` block contains any number of individual sprites (sub-blocks prefixed with `sprite_id`). See [Action01](action01.md) for details of these.

In other respects, the content of this record is similar to [Action01](action01.md), except that there is no concept of sprite sets. Please look there for more information. 

## Links

The block structure described above is directly related to the corresponding record in the NewGRF specifications:

- [Action12 NewGRF specifications](https://newgrf-specs.tt-wiki.net/wiki/Action12)

