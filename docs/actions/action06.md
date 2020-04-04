[Back to table of contents](../index.md)

# Action 06

Action06 (`modify_next`) allows modifying the contents of the following record. It uses the values of the GRF parameters and writes them into the data of the following record, overwriting the bytes that are present in the GRF file already. 

The following example includes a few comments for guidance generated automatically when a GRF is decoded. The YAGL format for this record is as follows:

```bash
// Record #241
modify_next // Action06
{
    // modification(value, num_bytes, offset, add?);
    modification(parameter[0x6C], 1, 8, false);
    modification(parameter[0x6B], 1, 9, false);
}
```

This record type is a little bit problematic for YAGL because the following record is written in YAGL rather than NFO, so it is not at all obvious what the offsets and sizes should be for the ranges of bytes that get overwritten. **yagl** has a `-x` option which dumps a GRF to a hex format somewhat similar to NFO. Using this should make it possible to encode your GRF, and then dump the hex version to work out what the modifications should be. This is not very satisfactory, but it will do for now.

The following example show the hex that is dumped for record #241 above and, more to the point, the following record #242:

```bash
Record #241
modify_next
06 6C 01 FF 08 00 6B 01 FF 09 00 FF 

Record #242
properties
00 08 01 02 FF 0F 00 08 00 00 
```

The block name `modify_next` is not decorated with additional information, and the block contains any number of `modification` items. Each modification has the following form:

```bash
modification(<parameter>, <num_bytes>, <offset>, <add_value>);
```

- `<parameter>` is the number of one of the GRF parameters. Its value may have been set by a previous operation in the GRF. The modification is not carried out if the parameter has not been defined yet. 
- `<num_bytes>` is how many bytes of the parameter to use. If this is larger than 4 (the size of a parameter), the bytes of the following parameter are used. In that case, all required parameters must be defined or no modification will be done. 
- `<offset>` is the index of the first byte in the following sprite to modify. The counting starts with 0 at the action byte and can go up to the length of the sprite. It is not possible to add data at the end of the sprite.
- `<add_value>` is a boolean value (`true` or `false`). If true, the parameter is *added* to the original data rather than replacing it. This is useful especially when allocating sprites using the GRF Resource Management, because one typically allocates more than one sprite, but the parameter can only hold a single number, the first sprite allocated.

## Links

The block structure described above is directly related to the corresponding record in the NewGRF specifications:

- [NewGRF Action06 specifications](https://newgrf-specs.tt-wiki.net/wiki/Action6)
