[Back to table of contents](../index.md)

# Action 08

Action08 (`grf`) defines GRFID, name and description for a GRF.  

Action08 has to be one of the first records in the YAGL file. It is however valid to have other records before it, except those that define new sprites or vehicle (etc.) properties. Actions 06, 07, 09, 0B, 0C, 0D (except resource management), 10 and 14 are valid before an Action08. 

The following example includes a few comments for guidance generated automatically when a GRF is decoded. The YAGL format for this record is as follows:


```bash
// Record #2
grf // Action08
{
    grf_id: "\xFB\xFB\x06\x01";
    version: GRF8;
    name: "Dutch Trainset v7393 (474ff0e257af)";
    description: "{lt-gray}Dutch Trains for OpenTTD {new-line}{black}First vehicle: 1839.{new-line}{new-line}
        (c)Dutch Trainset Team {new-line}License: GPLv2 or higher. {new-line}See readme for details.";
}
```

#### Properties

The block name is `grf` and contains the following properties:

| Name | Description |
|-|-|
| `grf_id`      | This is a series of 4 bytes which gives a (hopefully) unique identifier to the GRF. It is a convention for the GRF creator to include their initials and a set identifier, but you can do whatever you like. The value can be represented as a number or a string (see [GRF labels](../sundries/grf_labels.md) for more details). The example shows the four byte values represented as hex characters in a string. See [Action08](https://newgrf-specs.tt-wiki.net/wiki/Action8) for details of reserved ranges. |
| `version`     | This should be the graphics version that your GRF file is written for. This is the version of the GRF format, not the version of your GRF! |
| `name`        | This short field should contain the name of the set. The name must be short because it should fit on one line of the GRF Status window. See [string codes](../sundries/string_codes.md). |
| `description` | This is a longer description of the set. Typically it contains the name of the author and a copyright notice or other attribution. The description may be quite long, but should be short enough to fit in the description part of the GRF Status window, or a part of it may be cut off. See [string codes](../sundries/string_codes.md). |

The version number corresponds to the version of the NewGRF specifications used when creating the GRF. **yagl** is theoretically capable of reading older versions, but may not correctly write older versions. 

## Links

The block structure described above is directly related to the corresponding record in the NewGRF specifications:

- [NewGRF Action08 specifications](https://newgrf-specs.tt-wiki.net/wiki/Action8)
- [YAGL string control codes](../sundries/string_codes.md)
- [YAGL GRF labels](../sundries/grf_labels.md)
