[Back to table of contents](../index.md)
[Back to Action00](../actions/action00.md)

# Action00 - Cargos

```c++
// NOTE: These numbers are mostly made up rubbish for the sake of demonstrating the format of each property.
properties<Cargos, 0x0023> // Action00
{
    // instance_id: 0x0023
    {
        bit_number: 0x01;
        cargo_type_name_id: 0x0102;
        single_unit_name_id: 0x2003;
        single_unit_id: 0x1004;
        multiple_units_id: 0x0305;
        cargo_type_abbrev_id: 0x2006;
        cargo_sprite_id: 0x0707;
        single_unit_weight: 0x08;
        penalty_time_1: 0x09;
        penalty_time_2: 0x0A;
        base_price: 0x11223344;
        station_list_colour: 0xD3;
        payment_list_colour: 0xD4;
        is_freight: true;
        cargo_classes: 0x1111;
        cargo_label: "ABCD";
        town_growth_effect: 0x23;
        town_growth_multiplier: 0x1234;
        callback_flags: 0x45;
        cargo_units_id: 0xD001;
        cargo_amount_id: 0xD002;
        capacity_multiplier: 0x0102;
    }
}
```

#### Property details

The properties allowed inside the instance block are as follows (see [Cargos](https://newgrf-specs.tt-wiki.net/wiki/Action0/Cargos) for more details). Unless otherwise specified, the formats of the property values are simple integers:

| Name | NFO | Size | Description |
|-|-|-|-|
| `bit_number`             | 0x08 | B |  |
| `cargo_type_name_id`     | 0x09 | W |  |
| `single_unit_name_id`    | 0x0A | W |  |
| `single_unit_id`         | 0x0B | W |  |
| `multiple_units_id`      | 0x0C | W |  |
| `cargo_type_abbrev_id`   | 0x0D | W |  |
| `cargo_sprite_id`        | 0x0E | W |  |
| `single_unit_weight`     | 0x0F | B |  |
| `penalty_time_1`         | 0x10 | B |  |
| `penalty_time_2`         | 0x11 | B |  |
| `base_price`             | 0x12 | D |  |
| `station_list_colour`    | 0x13 | B |  |
| `payment_list_colour`    | 0x14 | B |  |
| `is_freight`             | 0x15 | B | Boolean value `true` or `false`. |
| `cargo_classes`          | 0x16 | W |  |
| `cargo_label`            | 0x17 | D | GRF label such as `"PAPR"`. See [GRF labels](../sundries/grf_labels.md). |
| `town_growth_effect`     | 0x18 | B |  |
| `town_growth_multiplier` | 0x19 | W |  |
| `callback_flags`         | 0x1A | B |  |
| `cargo_units_id`         | 0x1B | W |  |
| `cargo_amount_id`        | 0x1C | W |  |
| `capacity_multiplier`    | 0x1D | W |  |

## Links

- [Cargos NewGRF specifications](https://newgrf-specs.tt-wiki.net/wiki/Action0/Cargos)
- [GRF labels](../sundries/grf_labels.md)

