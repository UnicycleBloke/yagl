[Back to table of contents](../index.md)

# Action 01

Action01 (`sprite_sets`) is used to create sets of sprites for 1-2-3 chains. Each record contains 1 or more spritesets, each containing 1 or more sprites. Each spriteset must contain the same number of sprites. Each sprite contains at least one sprite sheet reference, but should contain additional references for the various zoom levels the sprite supports. 

The following example includes a few comments for guidance generated automatically when a GRF is decoded. The YAGL format for this record is as follows:

```bash
// Record #2901
sprite_sets<Canals, 0x0000> // <feature, first_set> Action01
{
    sprite_set // 0x0000
    {
        sprite_id<0x00000B57>
        {
            [38, 19, -5, 0], normal, c8bpp, "sprites\zbase_extra-8bpp-normal-0.png", [490, 4770];
            [37, 19, -4, 0], normal, c32bpp | chunked, "sprites\zbase_extra-32bpp-normal-0.png", [656, 4126];
            [149, 75, -20, 0], zin4, c32bpp | chunked, "sprites\zbase_extra-32bpp-zin4-3.png", [428, 10355];
            [75, 38, -10, 0], zin2, c32bpp | chunked, "sprites\zbase_extra-32bpp-zin2-0.png", [613, 13634];
        }
        sprite_id<0x00000B58>
        {
            [36, 18, -3, 13], normal, c8bpp, "sprites\zbase_extra-8bpp-normal-0.png", [538, 4770];
            [36, 18, -3, 14], normal, c32bpp | chunked, "sprites\zbase_extra-32bpp-normal-0.png", [703, 4126];
            [141, 71, -12, 57], zin4, c32bpp | chunked, "sprites\zbase_extra-32bpp-zin4-3.png", [587, 10355];
            [71, 36, -6, 28], zin2, c32bpp | chunked, "sprites\zbase_extra-32bpp-zin2-0.png", [698, 13634];
        }
        ... omitted - more sprites
    }
    sprite_set // 0x0001
    {
        sprite_id<0x00000B63>
        {
            [38, 19, -5, 0], normal, c8bpp, "sprites\zbase_extra-8bpp-normal-0.png", [86, 4920];
            [37, 19, -4, 0], normal, c32bpp | chunked, "sprites\zbase_extra-32bpp-normal-0.png", [232, 4175];
            [149, 75, -20, 0], zin4, c32bpp | chunked, "sprites\zbase_extra-32bpp-zin4-3.png", [10, 10612];
            [75, 38, -10, 0], zin2, c32bpp | chunked, "sprites\zbase_extra-32bpp-zin2-0.png", [436, 13724];
        }
        sprite_id<0x00000B64>
        {
            [36, 18, -3, 13], normal, c8bpp, "sprites\zbase_extra-8bpp-normal-0.png", [134, 4920];
            [36, 18, -3, 14], normal, c32bpp | chunked, "sprites\zbase_extra-32bpp-normal-0.png", [279, 4175];
            [141, 71, -12, 57], zin4, c32bpp | chunked, "sprites\zbase_extra-32bpp-zin4-3.png", [169, 10612];
            [71, 36, -6, 28], zin2, c32bpp | chunked, "sprites\zbase_extra-32bpp-zin2-0.png", [521, 13724];
        }
        ... omitted - more sprites
    }
    ... omitted - more sprite sets
}
```

#### Block name 

- The block name is `sprite_sets`.

- The "decoration" in angle brackets after the block name includes:
    - The type of feature (`Canals` in the example) for which the sprites are created. One of the following enumeration values is possible. There are other types of feature in this enumeration, such as `Bridges`, but these are the ones used in Action01 records:
        - `Trains`          (value = 0x00)
        - `Vehicles`        (value = 0x01)
        - `Ships`           (value = 0x02)
        - `Aircraft`        (value = 0x03)
        - `Stations`        (value = 0x04)
        - `Canals`          (value = 0x05)
        - `Houses`          (value = 0x07)
        - `IndustryTiles`   (value = 0x09)
        - `Industries`      (value = 0x0A)
        - `Cargos`          (value = 0x0B)
        - `Airports`        (value = 0x0D)
        - `Objects`         (value = 0x0F)
        - `Railtypes`       (value = 0x10)
        - `AirportTiles`    (value = 0x11)
        - `RoadTypes`       (value = 0x12)
        - `TramTypes`       (value = 0x13)
    - The index of the first spriteset (`0x0000` in the example). A non-zero index indicates that this is an extended format Action01 record.

#### Spriteset blocks

- Inside the block are any number of sub-blocks named `sprite_set`. 
    - The comment after each one is just to aid the reader, added when **yagl** decodes a GRF. Comments are ignored when encoding. 
    - The numbers of the spritesets (in the comments) simply increment from the first index. 
    - In this example, three spritesets are shown (truncated list).

- Each spriteset contains any number of individual sprites (sub-blocks prefixed with `sprite_id`). 
    - Note that each spriteset *must* contain the same number of sprites. **yagl** checks for this when reading the YAGL, and will display an error. It doesn't know what the correct number of sprites should be, only that the sets differ.
    - In this example, each spriteset shows two sprites (truncated lists).

- Note that the example is only an extract, and the numbers of spritesheets and sprites may not be realistic.


#### Real sprite block

- The `sprite_id` blocks represent realsprites (actual images), and have the following features:
    - The actual ID of the sprite in hex (in the angle bracket decoration). This value also appears in the sprite sheets so you can identify the images.
      - The first sprite shown has ID 0xB57. IDs are double words, so are shown with 8 hex digits. The preceding zeroes are optional.
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


#### Example of zoom levels in a single sprite

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


#### Example of masks used with a 32bpp sprite

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

## TODO 

TODO what about recolour sprites? Can they appear in an Action01?
TODO is 24 bpp a possible option? i.e. assume alpha is 0xFF?
TODO **yagl** should check that the feature type is one of those which can have sprites, e.g. not `Bridges`, for example.
TODO what are the standard numbers of spritesets and sprites in them for each type of feature?

## Links

The block structure described above is directly related to the corresponding record in the NewGRF specifications:

- [Action01 NewGRF specifications](https://newgrf-specs.tt-wiki.net/wiki/Action1)
