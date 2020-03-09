[Back to table of contents](../index.md)

# Action 01

Action01 (`sprite_sets`) is used to create sets of sprites for 1-2-3 chains. Each record contains 1 or more spritesets, each containing 1 or more sprites. Each spriteset must contain the same number of sprites. The YAGL format for this record is as follows:

```bash
// Record #1025
sprite_sets<Trains, 0x0000> // <feature, first_set> Action01
{
    sprite_set // 0x0000
    {
        sprite_id<0x00000403>
        {
            [8, 19, -3, -11], normal, c8bpp, "sprites\dutchtrains-8bpp-normal-0.png", [312, 349];
        }
        sprite_id<0x00000404>
        {
            [18, 14, -10, -8], normal, c8bpp, "sprites\dutchtrains-8bpp-normal-0.png", [330, 349];
        }
        // ... omitted
    }
    sprite_set // 0x0001
    {
        sprite_id<0x0000040B>
        {
            [8, 19, -3, -11], normal, c8bpp, "sprites\dutchtrains-8bpp-normal-0.png", [532, 349];
        }
        sprite_id<0x0000040C>
        {
            [18, 14, -10, -8], normal, c8bpp, "sprites\dutchtrains-8bpp-normal-0.png", [550, 349];
        }
        // ... omitted
    }
    sprite_set // 0x0002
    {
        sprite_id<0x00000413>
        {
            [8, 19, -3, -11], normal, c8bpp, "sprites\dutchtrains-8bpp-normal-0.png", [752, 349];
        }
        sprite_id<0x00000414>
        {
            [18, 14, -10, -8], normal, c8bpp, "sprites\dutchtrains-8bpp-normal-0.png", [770, 349];
        }
        // ... omitted
    }
    // ... omitted
}
```

#### Block name 

- The block name is `sprite_sets`.

- The "decoration" in angle brackets after the block name includes:
    - The type of feature (`Trains` in the example) for which the sprites are created. One of the following enumeration values is possible:
        - `Trains` 
        - `Vehicles` 
        - `Ships` 
        - `Aircraft` 
        - `Stations` 
        - `Canals` 
        - `Houses` 
        - `IndustryTiles` 
        - `Industries` 
        - `Cargos` 
        - `Airports` 
        - `Objects` 
        - `Railtypes` 
        - `AirportTiles` 
        - `RoadTypes` 
        - `TramTypes` 
    - The index of the first spriteset (`0x0000` in the example). A non-zero index indicates that this is an extended format Action01 record.

#### Spriteset blocks

- Inside the block are any number of sub-blocks named `sprite_set`. 
    - The comment after each one is just to aid the reader, added when **yagl** decodes a GRF. Comments are ignored when encoding. 
    - The numbers of the spritesets (in the comments) simply increment from the first index. 
    - In this example, three spritesets are shown (truncated list).

- Each spriteset contains any number of individual sprites (sub-blocks prefixed with `sprite_id`). 
    - Note that each spriteset must contain the same number of sprites. **yagl** checks for this when reading the YAGL, and will display an error. It doesn't know what the correct number of sprites should be, only that the sets differ.
    - In this example, each spriteset shows two sprites (truncated lists).

- Note that the example is only an extract, and the numbers of spritesheets and sprites may not be realistic.

#### Real sprite block

- The `sprite_id` blocks represent realsprites (actual images), and have the following features:
    - The actual ID of the sprite in hex (in the angle bracket decoration). 
      - The first sprite shown has ID 0x403. IDs are double words, so are shown with 8 hex digits. The preceding zeroes are optional.
    - The size and offset of the sprite in square brackets: `[<xdim, ydim, xoff, yoff>]`.
    - The zoom level: one of `normal`, `zin2`, `zin4`, `zout2`, `zout4` or `zout8`.
    - The colour depth: one of `c8bpp`, `c32bpp` or  `mask`,  
        - optionally ORed together with `chunked` (and/or `no_crop` - not supported at the moment) 
    - The name of the spritesheet file containin the image data. This can be anything, but **yagl** generates files for each colour depth and zoom level when it decodes a GRF. 
    - The position of the sprite within the spritesheet in square brackets: `[<xpos, ypos>]`.
    - Don't forget the commas and the terminating semicolon.

The layout here differs a bit from NFO and NML but has more or less the same function. 

The real sprite is represented as a block because it may contain more than one image definition. This is how we represent different zoom levels and colour depths for a given sprite ID. This might seem a bit verbose in many cases. The whitespace is not important, so a sprite can be represented in a single line as follows, if that helps:

```bash
        sprite_id<0x00000406> { [19, 16, -3, -8], normal, c8bpp, "sprites/dutchtrains-8bpp-normal-0.png", [393, 349]; }
```

Note that when **yagl** reads the sprite's pixels from the spritesheet, it checks for any pure white pixels among them, and prints a warning if there are any. It also checks (and warns) for any non-pure white pixels in a one-pixel-wide border around the sprite. These tests are intended to highlight any possible misalignments or size discrepancies. 

## TODO 

TODO what about recolour sprites? Can they appear in an Action01?
TODO 32 bpp sprites which also contain masks. 
TODO is 24 bpp a possible option? i.e. assume alpha is 0xFF?
TODO **yagl** should check that the feature type is one of those which can have sprites, e.g. not `Bridges`, for example.
TODO what are the standard numbers of spritesets and sprites in them for each type of feature?

## Links

The block structure described above is directly related to the corresponding record in the NewGRF specifications:

- [Action01 NewGRF specifications](https://newgrf-specs.tt-wiki.net/wiki/Action1)
