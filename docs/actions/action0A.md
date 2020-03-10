[Back to table of contents](../index.md)

# Action 0A 

Action0A (`replacement_sprite_set`) modifies TTD's sprites. Unlike Action01 and Action05, which add new sprites for new features or vehicles, Action0A modifies TTD's built-in sprites. The YAGL format for this record is as follows:

```bash
replacement_sprite_sets // Action0A
{
    replacement_sprite_set<0x09E1> // <first_sprite>
    {
        // Replace sprite 0x09E1:
        sprite_id<0x00000009>
        {
            [64, 31, -31, 0], normal, c8bpp | chunked, "sprites/Tutorial_Bridge_00-8bpp-normal-0.png", [10, 10];
        }
        // Replace sprite 0x09E2:
        sprite_id<0x0000000A>
        {
            [64, 31, -31, 0], normal, c8bpp | chunked, "sprites/Tutorial_Bridge_00-8bpp-normal-0.png", [84, 10];
        }
        // Replace sprite 0x09E3:
        sprite_id<0x0000000B>
        {
            [64, 39, -31, -8], normal, c8bpp | chunked, "sprites/Tutorial_Bridge_00-8bpp-normal-0.png", [158, 10];
        }
        // ... omitted
    }
    replacement_sprite_set<0x0A24> // <first_sprite>
    {
        // Replace sprite 0x0A24:
        sprite_id<0x00000021>
        {
            [43, 9, 0, 11], normal, c8bpp | chunked, "sprites/Tutorial_Bridge_00-8bpp-normal-0.png", [134, 108];
        }
    }
    replacement_sprite_set<0x1100> // <first_sprite>
    {
        // Replace sprite 0x1100:
        sprite_id<0x00000022>
        {
            [64, 31, -31, 0], normal, c8bpp | chunked, "sprites/Tutorial_Bridge_00-8bpp-normal-0.png", [187, 108];
        }
        // Replace sprite 0x1101:
        sprite_id<0x00000023>
        {
            [64, 31, -31, 0], normal, c8bpp | chunked, "sprites/Tutorial_Bridge_00-8bpp-normal-0.png", [261, 108];
        }
        // Replace sprite 0x1102:
        sprite_id<0x00000024>
        {
            [64, 39, -31, -8], normal, c8bpp | chunked, "sprites/Tutorial_Bridge_00-8bpp-normal-0.png", [335, 108];
        }
        // ... omitted
    }
    // ... omitted
}
```

The format is almost identical to that for [Action01](action01.md), so please look a that for details. Aside from block names, the key difference to note is that the replacement sprite sets do not all have to have the same number of sprites. The outer block is not decorated with the type of feature to which the sprites relate.

## Links
- [Action0A NewGRF specifications](https://newgrf-specs.tt-wiki.net/wiki/ActionA)
