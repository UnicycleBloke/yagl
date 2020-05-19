[Back to table of contents](../index.md)
[Back to Action00](../actions/action00.md)
[Back to Action00 Vehicles](../actions/action00_vehicles.md)

# Action00 - Ships

```c++
// NOTE: These numbers are mostly made up rubbish for the sake of demonstrating the format of each property.
properties<Ships, 0x008D> // Action00
{
    // instance_id: 0x008D
    {
    }
```

#### Property details

The properties allowed inside the instance block are as follows (see [Ships](https://newgrf-specs.tt-wiki.net/wiki/Action0/Vehicles/Ships) for more details). Unless otherwise specified, the formats of the property values are simple integers:

| Name | NFO | Size | Description |
|-|-|-|-|
| `sprite_id`                    | 0x08 | B     |  |
| `is_refittable`                | 0x09 | B     | `true` or `false`. |
| `cost_factor`                  | 0x0A | B     |  |
| `speed_2_kmh`                  | 0x0B | B     |  |
| `cargo_type`                   | 0x0C | B     |  |
| `cargo_capacity`               | 0x0D | W     |  |
| `running_cost_factor`          | 0x0F | B     |  |
| `sound_effect_type`            | 0x10 | B     |  |
| `refit_cargo_types`            | 0x11 | D     |  |
| `callback_flags_mask`          | 0x12 | B     |  |
| `refit_cost`                   | 0x13 | B     |  |
| `ocean_speed_fraction`         | 0x14 | B     |  |
| `canal_speed_fraction`         | 0x15 | B     |  |
| `retire_vehicle_early`         | 0x16 | B     |  |
| `miscellaneous_flags`          | 0x17 | B     |  |
| `refittable_cargo_classes`     | 0x18 | W     |  |
| `non_refittable_cargo_classes` | 0x19 | W     |  |
| `long_introduction_date`       | 0x1A | D     | A date in the form `date(YYYY/M/D)`, which must be no earlier than year 0 (long date). |
| `sort_purchase_list`           | 0x1B | B*    |  |
| `visual_effect`                | 0x1C | B     |  |
| `custom_cargo_aging_period`    | 0x1D | W     |  |
| `always_refittable_cargos`     | 0x1E | B n*B | A list of indices into the cargo translation table. |
| `never_refittable_cargos`      | 0x1F | B n*B | A list of indices into the cargo translation table. |

## Links

- [Ships NewGRF specifications](https://newgrf-specs.tt-wiki.net/wiki/Action0/Vehicles/Ships )          
