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
    - The type of feature (`Canals` in the example) for which the sprites are created. One of the following enumeration values is possible. There are other types of feature in this enumeration, such as `Bridges`, but these are the ones used in Action01 records (see [features](../sundries/features.md)):
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
        - `RailTypes`       (value = 0x10)
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

The `sprite_id` blocks represent realsprites (actual images). See [real sprites](../graphics/real_sprite.md) for details.

## TODO 

TODO what about recolour sprites? Can they appear in an Action01?
TODO is 24 bpp a possible option? i.e. assume alpha is 0xFF?
TODO **yagl** should check that the feature type is one of those which can have sprites, e.g. not `Bridges`, for example.
TODO what are the standard numbers of spritesets and sprites in them for each type of feature?

## Links

The block structure described above is directly related to the corresponding record in the NewGRF specifications:

- [NewGRF Action01 specifications](https://newgrf-specs.tt-wiki.net/wiki/Action1)
- [YAGL feature enumeration](../sundries/features.md)
- [YAGL real sprites](../graphics/real_sprite.md)
