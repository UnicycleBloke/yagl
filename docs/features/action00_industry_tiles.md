[Back to table of contents](../index.md)
[Back to Action00](../actions/action00.md)

# Action00 - Industry Tiles

```c++
// NOTE: These numbers are mostly made up rubbish for the sake of demonstrating the format of each property.
properties<IndustryTiles, 0x0023> // Action00
{
    // instance_id: 0x0023
    {
        substitute_building_id: 0x12;
        industry_tile_override: 0x13;
        tile_acceptance1: 0x1014;
        tile_acceptance2: 0x0115;
        tile_acceptance3: 0x2016;
        land_shape_flags: 0x17;
        callback_flags: 0x18;
        animation_info: 0x3019;
        animation_speed: 0x20;
        callback_25_triggers: 0x21;
        special_flags: 0x22;
        cargo_acceptance_list: [ {0x02: 0x12} {0x03: 0x23} ];
    }
}
```

#### Property details

The properties allowed inside the instance block are as follows (see [Industry tiles](https://newgrf-specs.tt-wiki.net/wiki/Action0/Industry_Tiles) for more details). Unless otherwise specified, the formats of the property values are simple integers:

| Name | NFO | Size | Description |
|-|-|-|-|
| `substitute_building_id` | 0x08 | B |  |
| `industry_tile_override` | 0x09 | B |  |
| `tile_acceptance1`       | 0x0A | W |  |
| `tile_acceptance2`       | 0x0B | W |  |
| `tile_acceptance3`       | 0x0C | W |  |
| `land_shape_flags`       | 0x0D | B |  |
| `callback_flags`         | 0x0E | B |  |
| `animation_info`         | 0x0F | W |  |
| `animation_speed`        | 0x10 | B |  |
| `callback_25_triggers`   | 0x11 | B |  |
| `special_flags`          | 0x12 | B |  |
| `cargo_acceptance_list`  | 0x13 | B | An array of cargo acceptance pairs in the form `[ {<cargo>: <amount>}  {<cargo>: <amount>} ... ]`. |

## Links

- [Industry tiles NewGRF specifications](https://newgrf-specs.tt-wiki.net/wiki/Action0/Industry_Tiles)   
