[Back to table of contents](../index.md)
[Back to Action00](../actions/action00.md)

# Action00 - Objects

```c++
// NOTE: These numbers are mostly made up rubbish for the sake of demonstrating the format of each property.
properties<Objects, 0x0023> // Action00
{
    // instance_id: 0x0023
    {
        class_label: "ABCD";
        class_text_id: 0xC099;
        object_text_id: 0xC09A;
        climate_availability: Arctic | Toyland;
        size_xy: 0x44;
        cost_factor: 0x12;
        introduction_date: date(1800/1/1);
        end_of_life_date: date(2000/1/1);
        object_flags: 0x1234;
        animation_info: 0x2345;
        animation_speed: 0x23;
        animation_triggers: 0x1357;
        removal_cost_factor: 0x45;
        callback_flags: 0x1020;
        building_height: 0x34;
        number_of_views: 0x45;
        number_on_creation: 0x56;
    }
}
```

#### Property details

The properties allowed inside the instance block are as follows (see [Objects](https://newgrf-specs.tt-wiki.net/wiki/Action0/Objects) for more details). Unless otherwise specified, the formats of the property values are simple integers:

| Name | NFO | Size | Description |
|-|-|-|-|
| `class_label`          | 0x08 | D | GRF label such as `"TOWR"`. See [GRF labels](../sundries/grf_labels.md). |
| `class_text_id`        | 0x09 | W |  |
| `object_text_id`       | 0x0A | W |  |
| `climate_availability` | 0x0B | B | A bitfield containing values from the set: [`Temperate`, `Arctic`, `Tropical`, `Toyland`] ORed together. |
| `size_xy`              | 0x0C | B |  |
| `cost_factor`          | 0x0D | B |  |
| `introduction_date`    | 0x0E | D | A date in the form `date(YYYY/M/D)`, which must be no earlier than year 0 (long date). |
| `end_of_life_date`     | 0x0F | D | A date in the form `date(YYYY/M/D)`, which must be no earlier than year 0 (long date). |
| `object_flags`         | 0x10 | W |  |
| `animation_info`       | 0x11 | W |  |
| `animation_speed`      | 0x12 | B |  |
| `animation_triggers`   | 0x13 | W |  |
| `removal_cost_factor`  | 0x14 | B |  |
| `callback_flags`       | 0x15 | W |  |
| `building_height`      | 0x16 | B |  |
| `number_of_views`      | 0x17 | B |  |
| `number_on_creation`   | 0x18 | B |  |

## Links

- [Objects NewGRF specifications](https://newgrf-specs.tt-wiki.net/wiki/Action0/Objects)         
- [GRF labels](../sundries/grf_labels.md)

