[Back to table of contents](../index.md)
[Back to Action00](../actions/action00.md)

# Action00 - Airport Tiles

```c++
// NOTE: These numbers are mostly made up rubbish for the sake of demonstrating the format of each property.
properties<AirportTiles, 0x008D> // Action00
{
    // instance_id: 0x008D
    {
    }
```

#### Property details

The properties allowed inside the instance block are as follows (see [Airport tiles](https://newgrf-specs.tt-wiki.net/wiki/Action0/Airport_Tiles) for more details). Unless otherwise specified, the formats of the property values are simple integers:

| Name | NFO | Size | Description |
|-|-|-|-|
| `substitute_tile_id`   | 0x08 | B |  |
| `aiport_tile_override` | 0x09 | B |  |
| `callback_flags`       | 0x0E | B |  |
| `animation_info`       | 0x0F | W |  |
| `animation_speed`      | 0x10 | B |  |
| `animation_triggers`   | 0x11 | B |  |

## Links

- [Airport tiles NewGRF specifications](https://newgrf-specs.tt-wiki.net/wiki/Action0/Airport_Tiles)    

