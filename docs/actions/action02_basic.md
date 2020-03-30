[Back to table of contents](../index.md)
[Back to Action02](../actions/action02.md)        

# Action 02 (basic)

Action2 (`sprite_groups`) is used to group sets of sprites from the previous Action01 together, and make them accessible by a variational or random Action02 (chain) or an Action03.

Do not skip an Action02 using Action09 (unless it skips the whole file). Action02 must not be skipped by Action09 or TTDPatch will most likely crash. Skip or modify Action03 instead. Skipping an Action02 with an Action07 has no effect. 

The following example includes a few comments for guidance generated automatically when a GRF is decoded. The YAGL format for this record is as follows:

```bash
// Record #8905
sprite_groups<Trains, 0xF1> // Action02 basic
{
    primary_spritesets: [ 0x0000 ];
    secondary_spritesets: [ 0x0000 ];
}
```

#### Properties

The block name is `sprite_groups` and this is decorated with two values:
- The type of the feature whose graphics are being assigned (`Trains` in the example). See [features](../sundries/features.md) for the list of possibilities.
- The set ID to be associated with this Action02 record.

The properties inside the block are:

| Name | Description |
|-|-|
| `primary_spritesets` | The square brackets contain a set of space-separated IDs of Action01 sprite sets. |
| `secondary_spritesets` | The square brackets contain a set of space-separated IDs of Action01 sprite sets. |

Each feature has two different graphics ranges, but their exact meaning differs from feature to feature. For example, for vehicles, the first set is used while moving, and the second while loading/unloading in a station.

## Links

The block structure described above is directly related to the corresponding record in the NewGRF specifications:

- [NewGRF Action02 specifications](https://newgrf-specs.tt-wiki.net/wiki/Action2)
- [YAGL feature enumeration](../sundries/features.md)

