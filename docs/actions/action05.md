[Back to table of contents](../index.md)

# Action 05

Action05 (`replacement_sprite_set`) defines base graphics specific sets. Action05 is used to define graphics for several new features of OpenTTD and TTDPatch. This is similar to [Action01](action01.md). 

```bash
replace_sprites<Aqueducts> // Action05
{
    sprite_id: 0x00000113
    {
        [62, 39, -30, -14], normal, 8bpp, "sprites/zbase_extra-8bpp-normal-0.png", [10, 10];
        [64, 45, -32, -13], normal, 32bpp|chunked, "sprites/zbase_extra-32bpp-normal-0.png", [10, 10];
        [256, 179, -128, -51], zin4, 32bpp|chunked, "sprites/zbase_extra-32bpp-zin4-0.png", [10, 10];
        [128, 90, -64, -26], zin2, 32bpp|chunked, "sprites/zbase_extra-32bpp-zin2-0.png", [10, 10];
    }
    sprite_id: 0x00000114
    {
        [62, 39, -30, -14], normal, 8bpp, "sprites/zbase_extra-8bpp-normal-0.png", [82, 10];
        [64, 45, -32, -13], normal, 32bpp|chunked, "sprites/zbase_extra-32bpp-normal-0.png", [84, 10];
        [256, 179, -128, -51], zin4, 32bpp|chunked, "sprites/zbase_extra-32bpp-zin4-0.png", [276, 10];
        [128, 90, -64, -26], zin2, 32bpp|chunked, "sprites/zbase_extra-32bpp-zin2-0.png", [148, 10];
    }
    ... 
```

void Action05Record::print(std::ostream& os, const SpriteZoomMap& sprites, uint16_t indent) const
{
    os << pad(indent) << RecordName(record_type()) << "<";
    os << NewFeatureName(m_sprite_type) << ", ";
    os << to_hex(m_offset) << "> // <new_feature_type, offset>  Action05\n";
    os << pad(indent) << "{\n";

    uint16_t num_sprites = num_sprites_to_write();
    for (uint16_t index = 0; index < num_sprites; ++index)
    {
        os << pad(indent + 4) << "// Replace " << NewFeatureName(m_sprite_type); 
        os << " sprite " << to_hex<uint16_t>(m_offset + index) << "\n";
        print_sprite(index, os, sprites, indent + 4);
    }

    os << pad(indent) << "}\n";
}


## Links
- [Action05 NewGRF specifications](https://newgrf-specs.tt-wiki.net/wiki/Action5)
