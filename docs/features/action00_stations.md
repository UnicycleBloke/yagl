[Back to table of contents](../index.md)
[Back to Action00](../actions/action00.md)

# Action00 - Airports

```c++
// NOTE: These numbers are mostly made up rubbish for the sake of demonstrating the format of each property.
properties<Stations, 0x0023> // Action00
{
    // instance_id: 0x0023
    {
        class_id: "STNS";
        sprite_layouts: 
        {
            tile<0x13245768>
            {
                sprite(0x12345678, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06);
                sprite(0x12345678, 0x01, 0x02);
            }
            tile<0x13245769>
            {
                sprite(0x12345678, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06);
                sprite(0x12345678, 0x01, 0x02);
                sprite(0x12345678, 0x03, 0x04);
            }
        };
        copy_sprite_layout_id: 0x12;
        callback_flags: 0x13;
        disabled_platform_numbers: 0x14;
        disabled_platform_lengths: 0x15;
        custom_layouts: 
        {
            layout
            {
                // Platform: B=Building, P=Plain, L=RoofLeft, R=RoofRight
                B B B B B B ; // Plaform
                B B B B B B ; // Plaform
                B B B B B B ; // Plaform
            }
            layout
            {
                // Platform: B=Building, P=Plain, L=RoofLeft, R=RoofRight
                B B P P P B B R B B ; // Plaform
                L R B P P P P B B B ; // Plaform
            }
        };
        copy_custom_layout_id: 0x23;
        little_lots_threshold: 0x1234;
        pylon_placement: 0x33;
        cargo_type_triggers: 0x12345678;
        general_flags: 0x56;
        overhead_wire_placement: 0x67;
        can_train_enter_tile: 0x45;
        animation_info: 0x4321;
        animation_speed: 0x43;
        animation_triggers: 0x1122;
    }
}
```

#### Property details

The properties allowed inside the instance block are as follows (see [Stations](https://newgrf-specs.tt-wiki.net/wiki/Action0/Stations) for more details). Unless otherwise specified, the formats of the property values are simple integers:

| Name | NFO | Size | Description |
|-|-|-|-|
| class_id                  | 0x08 | D | GRF label such as `"ELEC"`.  See [GRF labels](../sundries/grf_labels.md). |
| sprite_layouts            | 0x09 | V |  | See sprite layouts below.
| copy_sprite_layout_id     | 0x0A | B |  | 
| callback_flags            | 0x0B | B |  | 
| disabled_platform_numbers | 0x0C | B |  | 
| disabled_platform_lengths | 0x0D | B |  | 
| custom_layouts            | 0x0E | V |  | See custom layouts below.
| copy_custom_layout_id     | 0x0F | B |  | 
| little_lots_threshold     | 0x10 | W |  | 
| pylon_placement           | 0x11 | B |  | 
| cargo_type_triggers       | 0x12 | D |  | 
| general_flags             | 0x13 | B |  | 
| overhead_wire_placement   | 0x14 | B |  | 
| can_train_enter_tile      | 0x15 | B |  | 
| animation_info            | 0x16 | W |  | 
| animation_speed           | 0x17 | B |  | 
| animation_triggers        | 0x18 | W |  | 

#### Sprite layouts

- The `sprite_layouts` property is a block containing a list of individual tile layout sub-blocks. 
- Each sub-block has the name `tile`, and is decorated with the ground sprite ID (a dword).
- Each tile contains a list of items which describe the additional sprites in the layout. There are two types:
  - `sprite(<sprite_id>, <x-offset>, <y-offset>, <z-offset>, <x-extent>, <y-extent>, <z-extent>);`: This is used for sprites which define their own bounding box.
  - `sprite(<sprite_id>, <x-offset>, <y-offset>);`: This is used for sprite which share the bounding box of the previous sprite.

#### Custom layouts

TODO

## Links

- [Stations NewGRF specifications](https://newgrf-specs.tt-wiki.net/wiki/Action0/Stations)
- [GRF labels](../sundries/grf_labels.md)

