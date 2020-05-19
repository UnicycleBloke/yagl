[Back to table of contents](../index.md)
[Back to Action00](../actions/action00.md)
[Back to Action00 Vehicles](../actions/action00_vehicles.md)

# Action00 - Road Vehicles

```c++
// NOTE: These numbers are mostly made up rubbish for the sake of demonstrating the format of each property.
```

#### Property details

The properties allowed inside the instance block are as follows (see [Road vehicles](https://newgrf-specs.tt-wiki.net/wiki/Action0/Vehicles/RoadVehicles) for more details). Unless otherwise specified, the formats of the property values are simple integers:

| Name | NFO | Size | Description |
|-|-|-|-|
| `speed_2_kmh`                  | 0x08 | B     |  | 
| `running_cost_factor`          | 0x09 | B     |  | 
| `running_cost_base`            | 0x0A | D     |  | 
| `sprite_id`                    | 0x0E | B     |  | 
| `cargo_capacity`               | 0x0F | B     |  | 
| `cargo_type`                   | 0x10 | B     |  | 
| `cost_factor`                  | 0x11 | B     |  | 
| `sound_effect_type`            | 0x12 | B     |  | 
| `power_10_hp`                  | 0x13 | B     |  | 
| `weight_quarter_tons`          | 0x14 | B     |  | 
| `speed_half_kmh`               | 0x15 | B     |  | 
| `refit_cargo_types`            | 0x16 | D     |  | 
| `callback_flags_mask`          | 0x17 | B     |  | 
| `coeff_of_tractive_effort`     | 0x18 | B     |  | 
| `coeff_of_air_drag`            | 0x19 | B     |  | 
| `refit_cost`                   | 0x1A | B     |  | 
| `retire_vehicle_early`         | 0x1B | B     |  | 
| `miscellaneous_flags`          | 0x1C | B     |  | 
| `refittable_cargo_classes`     | 0x1D | W     |  | 
| `non_refittable_cargo_classes` | 0x1E | W     |  | 
| `long_introduction_date`       | 0x1F | D     | A date in the form `date(YYYY/M/D)`, which must be no earlier than year 0 (long date). | 
| `sort_purchase_list`           | 0x20 | B*    |  | 
| `visual_effect`                | 0x21 | B     |  | 
| `custom_cargo_aging_period`    | 0x22 | W     |  | 
| `shorten_vehicle`              | 0x23 | B     |  | 
| `always_refittable_cargos`     | 0x24 | B n*B | A list of indices into the cargo translation table. | 
| `never_refittable_cargos`      | 0x25 | B n*B | A list of indices into the cargo translation table. | 

## Links

- [Road vehicles NewGRF specifications](https://newgrf-specs.tt-wiki.net/wiki/Action0/Vehicles/RoadVehicles)          
