[Back to table of contents](../index.md)
[Back to Action00](../actions/action00.md)
[Back to Action00 Vehicles](../actions/action00_vehicles.md)

# Action00 - Aircraft

```c++
// NOTE: These numbers are mostly made up rubbish for the sake of demonstrating the format of each property.
properties<Aircraft, 0x008D> // Action00
{
    // instance_id: 0x008D
    {
    }
```

#### Property details

The properties allowed inside the instance block are as follows (see [Aircraft](https://newgrf-specs.tt-wiki.net/wiki/Action0/Vehicles/Planes) for more details). Unless otherwise specified, the formats of the property values are simple integers:

| Name | NFO | Size | Description |
|-|-|-|-|
| `sprite_id`                    | 0x08 | B     |  |
| `is_helicopter`                | 0x09 | B     | `true` or `false`. |
| `is_large`                     | 0x0A | B     | `true` or `false`. |
| `cost_factor`                  | 0x0B | B     |  |
| `speed_8_mph`                  | 0x0C | B     |  |
| `acceleration`                 | 0x0D | B     |  |
| `running_cost_factor`          | 0x0E | B     |  |
| `passenger_capacity`           | 0x0F | W     |  |
| `mail_capacity`                | 0x11 | B     |  |
| `sound_effect_type`            | 0x12 | B     |  |
| `refit_cargo_types`            | 0x13 | D     |  |
| `callback_flags_mask`          | 0x14 | B     |  |
| `refit_cost`                   | 0x15 | B     |  |
| `retire_vehicle_early`         | 0x16 | B     |  |
| `miscellaneous_flags`          | 0x17 | B     |  |
| `refittable_cargo_classes`     | 0x18 | W     |  |
| `non_refittable_cargo_classes` | 0x19 | W     |  |
| `long_introduction_date`       | 0x1A | D     | A date in the form `date(YYYY/M/D)`, which must be no earlier than year 0 (long date). |
| `sort_purchase_list`           | 0x1B | B*    |  |
| `custom_cargo_aging_period`    | 0x1C | W     |  |
| `always_refittable_cargos`     | 0x1D | B n*B | A list of indices into the cargo translation table. |
| `never_refittable_cargos`      | 0x1E | B n*B | A list of indices into the cargo translation table. |
| `aircraft_range`               | 0x1F | W     |  |

## Links

- [Aircraft NewGRF specifications](https://newgrf-specs.tt-wiki.net/wiki/Action0/Vehicles/Planes )          
