[Back to table of contents](../index.md)
[Back to Action02](../actions/action02.md)        

# Action 02 (sprite layout)

Action02 (`sprite_layout`) defines a spritelayout for a tile. This is used for the following feature types: `Houses`, `IndustryTiles`, `Objects` and `AirportTiles`. There are three formats available in the underlying [NewGRF specifications](https://newgrf-specs.tt-wiki.net/wiki/Action2/Sprite_Layout), but these are rolled into a single format in YAGL. The underlying format is automatically determined from the contents of the record:

- Basic format for a single groundsprite and a single building sprite.
- Extended format for combining multiple sprites.
- Advanced format with register offsets.

The following example includes a few comments for guidance generated automatically when a GRF is decoded. The YAGL format for this record is as follows:

```bash
// Record #17
sprite_layout<Objects, 0x03> // Action02 random
{
    ground_sprite<0x00000F8D>
    {
    }
    building_sprite<0x80008003>
    {
        offset: 0x00, 0x00, 0x00;
        extent: 0x0F, 0x0F, 0x00;
    }
}
```

The following example from the FIRS GRF is more advanced:

```bash
// Record #3790
sprite_layout<IndustryTiles, 0xF3> // Action02 random
{
    ground_sprite<0x00000000>
    {
    }
    child_sprite<0xC0008000>
    {
        offset: 0x00, 0x00;
        registers: 
        {
            hide_sprite: 0x80;
            sprite_offset: 0x81;
        }
    }
    child_sprite<0xC0008001>
    {
        offset: 0x00, 0x00;
        registers: 
        {
            hide_sprite: 0x82;
            sprite_offset: 0x83;
        }
    }

    ... // More omitted.

    building_sprite<0x00008000>
    {
        offset: 0x00, 0x00, 0x00;
        extent: 0x01, 0x10, 0x06;
        registers: 
        {
            hide_sprite: 0x88;
            sprite_offset: 0x89;
            offset_z: 0x8A;
        }
    }
    building_sprite<0x00008000>
    {
        offset: 0x00, 0x00, 0x00;
        extent: 0x10, 0x01, 0x06;
        registers: 
        {
            hide_sprite: 0x8B;
            sprite_offset: 0x8C;
            offset_z: 0x8D;
        }
    }

    ... // More omitted.
}
```

#### Properties

The block name is `sprite_layout` and this is decorated with two values:
- The type of the feature whose graphics are being assigned (`Objects` in the first example). See [features](../sundries/features.md).
- The set ID to be associated with this Action02 record (`0x03` in the first example).

Within the block there are a number of sub-blocks. There may be a single `ground_sprite` block and any number of `building_sprite` or `child_sprite` blocks. Each sub-block is decorated with a double word value for the ground sprite or building sprite it represents.

**Ground sprite details** 

The only item which can be found in the `ground_sprite` block is an optional set of flags and associated register values: the `registers` property. See below for details.

**Building sprite details**

The building sprite includes two properties which define a bounding box:

- `offset` is an array of three values representing `<xoffset>, <yoffset>, <zoffset>` for the bounding box.
- `extent` is an array of three values representing `<xextent>, <yextent>, <zextent>` for the bounding box.
- In addition, the optional `registers` property may also be present. See below for details.

**Child sprite details**

A `child_sprite` is a special case of `building_sprite` which shares the bounding box of the previous item. A single `building_sprite` can be followed by zero or more `child_sprite` blocks. They can also be used to modify the `ground_sprite`. 

The `child_sprite` include a single property related to the bounding box:

- `offset`, an array of two value which represent `<xpixeloffset>, <ypixeloffset>`.
- In addition, the optional `registers` property may also be present. See below for details. 

**Registers**

The `registers` sub-block represents a set of flags and associated register values. Each flag is represented by a keyword, and is followed by a register index. The set of flags bits have two slightly different interpretations, depending on what type of block they are defined inside.

1. Inside a `building_sprite` block:

| Flag name | Value | Description |
|-|-|-|
| `hide_sprite`    | 0x01 | Skip bounding box including child sprites. |          
| `sprite_offset`  | 0x02 | Add offset to sprite, disable default usage of construction stage or railtype-offset |          
| `palette_offset` | 0x04 | Add offset to recoloursprite. |          
| `palette_act01`  | 0x08 | Recolour sprite is from Action 1 (will be affected by the same construction stage resp. railtype-offsets as sprites, unless bit 2 is set). |         
| `offset_x`       | 0x10 | Add offset for `<xoffset>`. Setting only offset_x, sets offset_y to zero. |    
| `offset_y`       | 0x10 | Add offset for `<yoffset>`. Setting only offset_y, sets offset_x to zero. |    
| `offset_z`       | 0x20 | Add offset for `<zoffset>`. |    
| `sprite_var10`   | 0x40 | Resolve sprite with Variable 10 set to a specific value. |         
| `palette_var10`  | 0x80 | Resolve recoloursprite with Variable 10 set to a specific value. |         

2. Inside a `ground_sprite` or `child_sprite` block:

| Flag name | Value | Description |
|-|-|-|
| `hide_sprite`    | 0x01 | Skip sprite. |          
| `sprite_offset`  | 0x02 | As above. |          
| `palette_offset` | 0x04 | As above. |          
| `palette_act01`  | 0x08 | As above. |         
| `offset_x`       | 0x10 | Add offset for `<xpixeloffset>` |    
| `offset_y`       | 0x20 | Add offset for `<ypixeloffset>` |    
| `sprite_var10`   | 0x40 | As above. |         
| `palette_var10`  | 0x80 | As above. |         

## Links

The block structure described above is directly related to the corresponding record in the NewGRF specifications:

- [Action02 NewGRF specifications](https://newgrf-specs.tt-wiki.net/wiki/Action2/Sprite_Layout)
- [YAGL feature enumeration](../sundries/features.md)
