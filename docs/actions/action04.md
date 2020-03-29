[Back to table of contents](../index.md)

# Action 04

Action04 (`strings`) is used to define strings, e.g vehicle, house or industry names

When making new vehicle graphics, you also need to name the new vehicles, or they'll show up with their original name from TTD. However, custom vehicle names assigned by the player in-game (or for TTDPatch also via TTD's vehicle.dat) will always take precendence.


```bash
// Record #24
strings<Trains, en_GB, 0xD08F*> // <feature, language, first_id> Action04, English (GB)
{
    /*0xD08F*/ "{black}Service: {yellow}local passengers{new-line}{black}Axle Weight: {yellow}low (class B){new-line}{black}Length: {yellow}3 units (1.7){new-line}{black}Loading Time: {yellow}{sd} day{ext plural-list 0xFF80}{ext list-value 0x0001}{ext list-default}s{ext end-list}";
    /*0xD090*/ "{black}Service: {yellow}local passengers{new-line}{black}Axle Weight: {yellow}low (class B){new-line}{black}Length: {yellow}2 units (1.2){new-line}{black}Loading Time: {yellow}{sd} day{ext plural-list 0xFF80}{ext list-value 0x0001}{ext list-default}s{ext end-list}";
    /*0xD091*/ "{black}Service: {yellow}executive{new-line}{black}Axle Weight: {yellow}low (class B){new-line}{black}Length: {yellow}1 unit (0.5){new-line}{black}Loading Time: {yellow}{sd} day{ext plural-list 0xFF80}{ext list-value 0x0001}{ext list-default}s{ext end-list}";
    ... omitted - more strings
}
```

The block name `strings` is decorated with the following information (in angle brackets):

- The type of feature (`Trains` in the example) for which the strings are created. See [features](../sundries/features.md).

- The short code (`en_GB` in the example) for the language to which the strings relate. See [languages](../sundries/language_codes.md).

- The OpenTTD string ID of the first string that is being replaced. A star indicates that the string ID should be stored as a WORD (2-byte value). IDs are otherwise stored as bytes or extended byte depending on their values. [This feature was added to preserve the information from the decode GRF, but is not strictly necessary for **yagl** to work: it could just always used the WORD format and save some fuss.]

The block contains any number of strings presented in a readable format (all control codes are replaced with human-readable text in braces). See [string control codes](../sundries/string_codes.md).

In the example, each string is preceded with an inline comment which indicates the ID of the string that is being replaced. This is there as a guide and is not actually necessary. Comments are ignored when encoding YAGL.


## Links

The block structure described above is directly related to the corresponding record in the NewGRF specifications:

- [NewGRF Action04 specifications](https://newgrf-specs.tt-wiki.net/wiki/Action4)
- [NewGRF string control codes](https://newgrf-specs.tt-wiki.net/wiki/StringCodes)

- [YAGL feature enumeration](../sundries/features.md)
- [YAGL string control codes](../sundries/string_codes.md)
- [YAGL language codes](../sundries/language_codes.md)
