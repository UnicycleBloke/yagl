[Back to table of contents](../index.md)

# Real sprite block

```bash
sprite_id<0x000000C3>
{
    // [<xdim, ydim, xoff, yoff>], <zoom>, <colour>, <sprite-sheet>, [<xpos, ypos>];
    [64, 31, -31, 0], normal, c8bpp | chunked, 
        "sprites\zbase_arctic-8bpp-normal-0.png", [158, 949];
}
```

The `sprite_id` blocks represent realsprites (actual images), and have the following features:
- The actual ID of the sprite in hex (in the angle bracket decoration). This value also appears in the sprite sheets so you can identify the images. IDs are double words, so are shown with 8 hex digits. The preceding zeroes are optional.
- The size and offset of the sprite in square brackets: `[<xdim, ydim, xoff, yoff>]`.
- The zoom level: one of `normal`, `zin2`, `zin4`, `zout2`, `zout4` or `zout8`.
- The colour depth: one of `c8bpp`, `c32bpp`,  
    - which is optionally ORed together with `chunked` and/or `mask` (and/or `no_crop` - not supported at the moment). A chunked sprite uses an extra compression algorithm which chops out most transparent pixels. A mask contains additional data which is used for glass effects other things. The `no_crop` option relates to a **grfcodec** feature.
- The name of the spritesheet file containin the image data. This can be anything, but **yagl** generates files for each colour depth and zoom level when it decodes a GRF. 
- The position of the sprite within the spritesheet in square brackets: `[<xpos, ypos>]`.
- Don't forget the commas and the terminating semicolon.

The layout here differs a bit from NFO and NML but has more or less the same function. 

The real sprite is represented as a block because it may contain more than one image definition. This is how we represent different zoom levels and colour depths for a given sprite ID. This might seem a bit verbose in many cases. The whitespace is not important, so a sprite can be represented in a single line as follows, if that helps:

```bash
sprite_id<0x00000B57> { [38, 19, -5, 0], normal, c8bpp, "sprites\zbase_extra-8bpp-normal-0.png", [490, 4770]; }
```

Note that when **yagl** reads the sprite's pixels from the spritesheet, it checks for any pure white pixels among them, and prints a warning if there are any. It also checks (and warns) for any non-pure white pixels in a one-pixel-wide border around the sprite. These tests are intended to highlight any possible misalignments or size discrepancies, or odd pixels within the body of the image which are accidentally set as pure white.


## Example of zoom levels in a single sprite

This is an extract from zbase_arctic.grf.

```bash
// Record #195
sprite_id<0x000000C3>
{
    [64, 31, -31, 0], normal, c8bpp | chunked, 
        "sprites\zbase_arctic-8bpp-normal-0.png", [158, 949];
    [64, 32, -31, 0], normal, c32bpp | chunked, 
        "sprites\zbase_arctic-32bpp-normal-0.png", [380, 946];
    [256, 128, -127, 0], zin4, c32bpp | chunked, 
        "sprites\zbase_arctic-32bpp-zin4-0.png", [10, 13366];
    [128, 64, -63, 0], zin2, c32bpp | chunked, 
        "sprites\zbase_arctic-32bpp-zin2-0.png", [424, 3168];
}
```


## Example of masks used with a 32bpp sprite

Note that when and image has mask data, this is stored in a separate 8bpp sprite sheet. The image file and position within that file are added to the normal sprite definition.

```bash
// Record #196
sprite_id<0x000000C4>
{
    [33, 22, -31, -6], normal, c8bpp, 
        "sprites\zbase_arctic-8bpp-normal-0.png", [232, 949];
    [43, 27, -38, -9], normal, c32bpp | mask | chunked, 
        "sprites\zbase_arctic-32bpp-normal-0.png", [454, 946], 
        "sprites\zbase_arctic-mask-normal-0.png", [191, 10];
    [181, 118, -154, -34], zin4, c32bpp | mask | chunked, 
        "sprites\zbase_arctic-32bpp-zin4-0.png", [276, 13366], 
        "sprites\zbase_arctic-mask-zin4-0.png", [10, 155];
    [86, 54, -76, -17], zin2, c32bpp | mask | chunked, 
        "sprites\zbase_arctic-32bpp-zin2-0.png", [562, 3168], 
        "sprites\zbase_arctic-mask-zin2-0.png", [332, 10];
}
```

## Links

- [NewGRF RealSprite specifications](https://newgrf-specs.tt-wiki.net/wiki/RealSprites)

