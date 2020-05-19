[Back to table of contents](../index.md)
[Back to Action00](../actions/action00.md)

# Action00 - Houses

```c++
// NOTE: These numbers are mostly made up rubbish for the sake of demonstrating the format of each property.
properties<Houses, 0x0023> // Action00
{
    // instance_id: 0x0023
    {
        substitute_building_id: 0x01;
        building_flags: 0x02;
        years_available: [ 1931 1967 ];
        population: 0x20;
        mail_multiplier: 0x03;
        passenger_acceptance: 0x04;
        mail_acceptance: 0x05;
        goods_etc_acceptance: 0x06;
        la_rating_decrease: 0x0107;
        removal_cost_multiplier: 0x08;
        building_name_id: 0xC109;
        availability_mask: 0x1234;
        callback_flags: 0x0A;
        override_byte: 0x0B;
        refresh_multiplier: 0x0C;
        four_random_colours: [ 0x0D 0x0E 0x0F 0x10 ];
        appearance_probability: 0x12;
        extra_flags: 0x19;
        animation_frames: 0x20;
        animation_speed: 0x21;
        building_class: 0x22;
        callback_flags_2: 0x23;
        accepted_cargo_types: [ 0x01 0x03 0x05 0x07 ];
        minimum_life_years: 0x01C9;
        accepted_cargo_list: [ 0x02 0x04 0x06 ];
        long_minimum_year: 1931;
        long_maximum_year: 1967;
        tile_acceptance_list: [ {0x01: 0x12} {0x02: 0x13} ];
    }
}
```

#### Property details

The properties allowed inside the instance block are as follows (see [Houses](https://newgrf-specs.tt-wiki.net/wiki/Action0/Houses) for more details). Unless otherwise specified, the formats of the property values are simple integers:

| Name | NFO | Size | Description |
|-|-|-|-|
| `substitute_building_id`  | 0x08 | B     |  |
| `building_flags`          | 0x09 | B     |  |
| `years_available`         | 0x0A | 2*B   | An array of two years in the form `[ <first> <last> ]`. Years must be no earlier than 1920. |
| `population`              | 0x0B | B     |  |
| `mail_multiplier`         | 0x0C | B     |  |
| `passenger_acceptance`    | 0x0D | B     |  |
| `mail_acceptance`         | 0x0E | B     |  |
| `goods_etc_acceptance`    | 0x0F | B     |  |
| `la_rating_decrease`      | 0x10 | W     |  |
| `removal_cost_multiplier` | 0x11 | B     |  |
| `building_name_id`        | 0x12 | W     |  |
| `availability_mask`       | 0x13 | W     |  |
| `callback_flags`          | 0x14 | B     |  |
| `override_byte`           | 0x15 | B     |  |
| `refresh_multiplier`      | 0x16 | B     |  |
| `four_random_colours`     | 0x17 | 4*B   | An array of four colours in the form `[ <clr> <clr> <clr> <clr> ]`. | 
| `appearance_probability`  | 0x18 | B     |  |
| `extra_flags`             | 0x19 | B     |  |
| `animation_frames`        | 0x1A | B     |  |
| `animation_speed`         | 0x1B | B     |  |
| `building_class`          | 0x1C | B     |  |
| `callback_flags_2`        | 0x1D | B     |  |
| `accepted_cargo_types`    | 0x1E | 4*B   | An array of four cargo types in the form `[ <cargo> <cargo> <cargo> <cargo> ]`. | 
| `minimum_life_years`      | 0x1F | W     |  |
| `accepted_cargo_list`     | 0x20 | B n*B | An array of cargo types in the form `[ <cargo> <cargo> <cargo> ]`. |
| `long_minimum_year`       | 0x21 | W     | Any year since 0000. |
| `long_maximum_year`       | 0x22 | W     | Any year since 0000. |
| `tile_acceptance_list`    | 0x23 | V     | An array of cargo acceptance pairs in the form `[ {<cargo>: <amount>}  {<cargo>: <amount>} ... ]`. | 

## Links

- [Houses NewGRF specifications](https://newgrf-specs.tt-wiki.net/wiki/Action0/Houses)          
