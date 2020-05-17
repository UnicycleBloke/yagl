[Back to table of contents](../index.md)
[Back to Action00](../actions/action00.md)

# Action00 - Airports

```c++
// NOTE: These numbers are mostly made up rubbish for the sake of demonstrating the format of each property.
properties<Airports, 0x0023> // Action00
{
    // instance_id: 0x0023
    {
        airport_override_id: 0x12;
        airport_layouts:  
        {
            layout<North>
            {
                old_tile(1, 3, 0x35); // Only 8 bits
                new_tile(5, 3, 0x2341);
                clearance(2, -3);
            }
            layout<South>
            {
                old_tile(1, 3, 0x35); // Only 8 bits
                new_tile(5, 3, 0x2341);
                clearance(-2, -3);
            }
        };
        years_available: [ 1900 1950 ];
        compatible_ttd_airport: 0x12;
        catchment_area: 0x09;
        noise_level: 0x11;
        airport_name_id: 0x1234;
        maintenance_cost_factor: 0x5678;
    }
}
```

#### Property details

The properties allowed inside the instance block are as follows (see [Airports](https://newgrf-specs.tt-wiki.net/wiki/Action0/Airports) for more details). Unless otherwise specified, the formats of the property values are simple integers:

| Name | NFO | Size | Description |
|-|-|-|-|
| `airport_override_id`     | 0x08 | B   |  | 
| `airport_layouts`         | 0x0A | V   | See below. | 
| `years_available`         | 0x0C | 2*W | A pair of years (first and last) in the form: `[ 1920 1970 ]`. | 
| `compatible_ttd_airport`  | 0x0D | B   |  | 
| `catchment_area`          | 0x0E | B   |  | 
| `noise_level`             | 0x0F | B   |  | 
| `airport_name_id`         | 0x10 | W   |  | 
| `maintenance_cost_factor` | 0x11 | W   |  | 

#### Airport layouts

- The airport layouts property is a block containing a list of individual airport layout sub-blocks. 
- Each sub-block has the name `layout`, and is decorated with the orientation of this particular layout.
- The orientation is one of the enumeration: `North`, `South`, `East` and `West`.
- Each layout contains a list of items which describe the individual tiles in the layout. There are three types:
  - `old_tile(<x-offset>, <y-offset>, <tile_id>);`: The tile_id must be a byte value. The offsets cannot be negative.
  - `new_tile(<x-offset>, <y-offset>, <tile_id>);`: The tile_id may be a word value.The offsets cannot be negative.
  - `clearance(<x-offset>, <y-offset>);`: Just clear the land. Offsets may be negative.

TODO there seems little value in splitting new and old tiles. Maybe just choose depending on the value of the tile ID. The current code preserves what it finds in the GRF.

## Links

- [Airports NewGRF specifications](https://newgrf-specs.tt-wiki.net/wiki/Action0/Airports)                 
