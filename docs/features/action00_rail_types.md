[Back to table of contents](../index.md)
[Back to Action00](../actions/action00.md)

# Action00 - RailTypes

```c++
// NOTE: These numbers are mostly made up rubbish for the sake of demonstrating the format of each property.
properties<RailTypes, 0x0001> // Action00
{
    // instance_id: 0x0001
    {
        railtype_label: "ELRL";
        introduction_date: date(1910/1/1);
        rail_type_name_id: 0xDC05;
        toolbar_caption_id: 0xDC06;
        dropdown_text_id: 0xDC06;
        window_caption_id: 0xDC07;
        autoreplace_text_id: 0xDC08;
        new_engine_text_id: 0xDC09;
        compatible_railtypes: [ "RAIL" "RLOW" "RMED" "RHIG" ];
        powered_railtypes: [ "ELRL" "ELOW" "EMED" "EHIG" "HSTR" ];
        alternate_railtypes: [ "ELRL" "ELOW" "EMED" "EHIG" "HSTR" ];
        construction_costs: 0x0000;
        maintenance_cost_factor: 0x0000;
        railtype_flags: 0x01;
        speed_limit: 0x0000;
        acceleration_model: 0x00;
        minimap_colour: 0x5F;
        sort_order: 0x04;
    }
}
```

#### Property details

The properties allowed inside the instance block are as follows (see [Rail types](https://newgrf-specs.tt-wiki.net/wiki/Action0/Railtypes) for more details). Unless otherwise specified, the formats of the property values are simple integers:

| Name | NFO | Size | Description |
|-|-|-|-|
| `railtype_label`          | 0x08 | D     | GRF label such as `"ELEC"`.  See [GRF labels](../sundries/grf_labels.md). |
| `toolbar_caption_id`      | 0x09 | W     | |
| `dropdown_text_id`        | 0x0A | W     | |
| `window_caption_id`       | 0x0B | W     | |
| `autoreplace_text_id`     | 0x0C | W     | |
| `new_engine_text_id`      | 0x0D | W     | |
| `compatible_railtypes`    | 0x0E | B n*D | List of GRF labels such as `["ELEC", "MAGL"]`. |
| `powered_railtypes`       | 0x0F | B n*D | List of GRF labels. |
| `railtype_flags`          | 0x10 | B     | |
| `curve_speed_multiplier`  | 0x11 | B     | |
| `station_graphics`        | 0x12 | B     | |
| `construction_costs`      | 0x13 | W     | |
| `speed_limit`             | 0x14 | W     | |
| `acceleration_model`      | 0x15 | B     | |
| `minimap_colour`          | 0x16 | B     | |
| `introduction_date`       | 0x17 | D     | A date in the form `date(YYYY/M/D)`, which must be no earlier than year 0 (long date). |
| `required_railtypes`      | 0x18 | B n*D | List of GRF labels. |
| `introduced_railtypes`    | 0x19 | B n*D | List of GRF labels. |
| `sort_order`              | 0x1A | B     | |
| `rail_type_name_id`       | 0x1B | W     | |
| `maintenance_cost_factor` | 0x1C | W     | |
| `alternate_railtypes`     | 0x1D | B n*D | List of GRF labels. |

## Links

- [Rail types NewGRF specifications](https://newgrf-specs.tt-wiki.net/wiki/Action0/Railtypes)       
- [GRF labels](../sundries/grf_labels.md)
