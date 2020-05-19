[Back to table of contents](../index.md)
[Back to Action00](../actions/action00.md)

# Action00 -TramTypes

```c++
// NOTE: These numbers are mostly made up rubbish for the sake of demonstrating the format of each property.
properties<TramTypes, 0x0000> // Action00
{
    // instance_id: 0x0000
    {
        tramtype_label: "AUZB";
        introduction_date: date(0/1/1);
        tram_type_name_id: 0xDC00;
        toolbar_caption_id: 0xDC01;
        dropdown_text_id: 0xDC02;
        window_caption_id: 0xDC03;
        autoreplace_text_id: 0xDC04;
        new_engine_text_id: 0xDC05;
        powered_roadtypes: [ "ROAD" "FUEL" "ASP1" "ASP2" "HWAY" ];
        speed_limit: 0x003C;
        sort_order: 0x10;
    }
}
```

#### Property details

The properties allowed inside the instance block are as follows (see [Tram types](https://newgrf-specs.tt-wiki.net/wiki/Action0/Tramtypes) for more details). Unless otherwise specified, the formats of the property values are simple integers:

| Name | NFO | Size | Description |
|-|-|-|-|
| `tramtype_label`          | 0x08 | D     | GRF label such as `"ELEC"`. See [GRF labels](../sundries/grf_labels.md). |
| `toolbar_caption_id`      | 0x09 | W     |   |
| `dropdown_text_id`        | 0x0A | W     |   |
| `window_caption_id`       | 0x0B | W     |   |
| `autoreplace_text_id`     | 0x0C | W     |   |
| `new_engine_text_id`      | 0x0D | W     |   |
| `powered_tramtypes`       | 0x0F | B n*D | List of GRF labels such as `["ELEC", "MAGL"]`. |
| `tramtype_flags`          | 0x10 | B     |   |
| `construction_costs`      | 0x13 | W     |   |
| `speed_limit`             | 0x14 | W     |   |
| `minimap_colour`          | 0x16 | B     |   |
| `introduction_date`       | 0x17 | D     | A date in the form `date(YYYY/M/D)`, which must be no earlier than year 0 (long date). |
| `required_tramtypes`      | 0x18 | B n*D | List of GRF labels. |
| `introduced_tramtypes`    | 0x19 | B n*D | List of GRF labels. |
| `sort_order`              | 0x1A | B     |   |
| `tram_type_name_id`       | 0x1B | W     |   |
| `maintenance_cost_factor` | 0x1C | W     |   |
| `alternate_tramtypes`     | 0x1D | B n*D | List of GRF labels. |

## Links

- [Tram types NewGRF specifications](https://newgrf-specs.tt-wiki.net/wiki/Action0/Tramtypes)
- [GRF labels](../sundries/grf_labels.md)
