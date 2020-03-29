[Back to table of contents](../index.md)

# Recolour Sprites

```bash
recolour_sprite
{
    0x50..0x57: 0xC6..0xCD;
    0xC6: 0x40;
    0xC7..0xCC: 0xC0..0xC5;
    0xCD: 0x27;
}
```


Rather than an actual image sprite (i.e. a [real sprite](../graphics/real_sprite.md)), a `replace_sprites` block can also include recolour sprites. These originated when OpenTTD supported only 8bpp images, and provide a map which shuffles the colours around the 256-entry palette. TODO how do they affect 32bpp images?

The `recolour_sprite` block contains a series of ranges which specify how modify the colours. Each range has one of two formats: 
 - `<to_index>: <from_index>;`, or 
 - `<to_begin..to_end>: <from_begin..from_end>;`. 
 
The numbers are all indices in the range 0 to 255 (0x00 to 0xFF). The second format is an abbreviation. That is:

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

- [NewGRF RealSprite specifications](https://newgrf-specs.tt-wiki.net/wiki/RecolorSprites)
- [YAGL real sprites](../graphics/real_sprite.md)
