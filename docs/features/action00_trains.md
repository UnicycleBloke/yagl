[Back to table of contents](../index.md)
[Back to Action00](../actions/action00.md)
[Back to Action00 Vehicles](../actions/action00_vehicles.md)

# Action00 - Trains

```c++
// NOTE: These numbers are mostly made up rubbish for the sake of demonstrating the format of each property.
properties<Trains, 0x008D> // Action00
{
    // instance_id: 0x008D
    {
        long_introduction_date: date(1914/1/1);
        model_life_years: 88;
        retire_vehicle_early: 0x30;
        vehicle_life_years: 40;
        loading_speed: 0x05;
        cost_factor: 0x1D;
        running_cost_factor: 0x18;
        speed_kmh: 111;
        power: 779;
        cargo_capacity: 1;
        weight_tons: 105;
        weight_high_byte: 0x00;
        coeff_of_tractive_effort: 0x13;
        coeff_of_air_drag: 0x00;
        reliability_decay_speed: 20;
        always_refittable_cargos: [ 0x00 ];
        refit_cargo_types: 0x00000000;
        miscellaneous_flags: 0x00;
        refit_cost: 0x00;
        track_type: 1;
        ai_special_flag: false;
        running_cost_base: 0x00004C30;
        engine_traction_type: 0x00;
        visual_effect: effect(SteamPuffs, 0x08, Disable);
        climate_availability: Temperate | Arctic | Tropical | Toyland;
        sprite_id: 0xFD;
        is_dual_headed: false;
        shorten_vehicle: 0x00;
        power_from_each_wagon: 0x0000;
        mask_for_var_42: 0x00;
    }
```

#### Property details

The properties allowed inside the instance block are as follows (see [Trains](https://newgrf-specs.tt-wiki.net/wiki/Action0/Vehicles/Trains) for more details). Unless otherwise specified, the formats of the property values are simple integers:

| Name | NFO | Size | Description |
|-|-|-|-|
| `track_type`                   | 0x08 | B     |  |
| `ai_special_flag`              | 0x08 | B     | `true` or `false`. |
| `speed_kmh`                    | 0x09 | W     |  |
| `power`                        | 0x0B | W     |  |
| `running_cost_factor`          | 0x0D | B     |  |
| `running_cost_base`            | 0x0E | D     |  |
| `sprite_id`                    | 0x12 | B     |  |
| `is_dual_headed`               | 0x13 | B     | `true` or `false`. |
| `cargo_capacity`               | 0x14 | B     |  |
| `cargo_type`                   | 0x15 | B     |  |
| `weight_tons`                  | 0x16 | B     |  |
| `cost_factor`                  | 0x17 | B     |  |
| `ai_engine_rank`               | 0x18 | B     |  |
| `engine_traction_type`         | 0x19 | B     |  |
| `sort_purchase_list`           | 0x1A | B/B*  |  |
| `power_from_each_wagon`        | 0x1B | W     |  |
| `refit_cost`                   | 0x1C | B     |  |
| `refit_cargo_types`            | 0x1D | D     |  |
| `callback_flags_mask`          | 0x1E | B     |  |
| `coeff_of_tractive_effort`     | 0x1F | B     |  |
| `coeff_of_air_drag`            | 0x20 | B     |  |
| `shorten_vehicle`              | 0x21 | B     |  |
| `visual_effect`                | 0x22 | B     | Visual effect in the form `effect(SteamPuffs, 0x08, Disable)`. The first parameter is one of the the enumeration `Default`, `SteamPuffs`, `DieselFumes`, `ElectricSparks`, `DisableEffect`, `SteamCallback`, `DieselCallback`, `ElectricCallback`. The second parameter is an integer giving the position of the effect. The third parameter is `Enable` or `Disable`, and controls the wagon power. |
| `weight_from_wagons`           | 0x23 | B     |  |
| `weight_high_byte`             | 0x24 | B     |  |
| `mask_for_var_42`              | 0x25 | B     |  |
| `retire_vehicle_early`         | 0x26 | B     |  |
| `miscellaneous_flags`          | 0x27 | B     |  |
| `refittable_cargo_classes`     | 0x28 | W     |  |
| `non_refittable_cargo_classes` | 0x29 | W     |  |
| `long_introduction_date`       | 0x2A | D     | A date in the form `date(YYYY/M/D)`, which must be no earlier than year 0 (long date). |
| `custom_cargo_aging_period`    | 0x2B | W     |  |
| `always_refittable_cargos`     | 0x2C | B n*B | A list of indices into the cargo translation table. |
| `never_refittable_cargos`      | 0x2D | B n*B | A list of indices into the cargo translation table. |

## Links

- [Trains NewGRF specifications](https://newgrf-specs.tt-wiki.net/wiki/Action0/Vehicles/Trains)          
