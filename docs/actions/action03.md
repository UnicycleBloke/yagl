[Back to table of contents](../index.md)

# Action 03

Action 3 (`feature_graphics`) assigns graphics sets to feature IDs (for vehicles, stations, houses, industries, ...). Graphics are referred to by the heads of chains of Action02s. 

The following example includes a few comments for guidance generated automatically when a GRF is decoded. The YAGL format for this record is as follows:

```bash
// Record #640
feature_graphics<Trains> // Action03
{
    livery_override: false;
    default_set_id: 0x00FB;
    feature_ids: [ 0x0086 ];
    cargo_types:
    {
        // <cargo_type>: <cargo_id>;
        0xFF: 0x00F8;
    };
}
```

#### Properties

The block name is `feature_graphics` and this is decorated with the type of the feature whose graphics are being assigned (`Trains` in the example). See [features](../sundries/features.md) for the list of possibilities.

| Name | Description |
|-|-|
| `livery_override` | The boolean value indicates whether the Action03 record is a livery override. The default is false, so the property can be omitted in that case. |
| `default_set_id` | Default Action02 set ID to use if none of the other items leads to a match |
| `feature_ids` | IDs of zero or more items of the feature type, such as `Trains`. These specify the feature instances to which the graphics are being assigned. If there is more than one item, then all the items will get the same graphics. If there are zero items, this creates a generic feature-specific definition not associated with any particular item.  At the moment, this is used for generic callbacks. The list of features IDs are space separated and delimited by square brackets: `[ 0x1234 0x1235 ]`. |
| `cargo_types` | A list of cargo-type - set ID pairs. Each gives the cargo-type for which the set ID applies.  If the item is built to use this type of cargo, or if it is refitted for it, the given set-ID is used as its graphics. If the grf file has installed a cargo translation table, the cargo type here refers to the cargo with the label in the given slot of the translation table. If defined, cargo-type FF is used for graphics shown in the purchase or construction window.| 
 

## Links

The block structure described above is directly related to the corresponding record in the NewGRF specifications:

- [NewGRF Action03 specifications](https://newgrf-specs.tt-wiki.net/wiki/Action3)
- [YAGL feature enumeration](../sundries/features.md)

