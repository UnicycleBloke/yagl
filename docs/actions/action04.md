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
- The type of feature (`Trains` in the example) for which the strings are created.
- The short code (`en_GB` in the example) for the language to which the strings relate. 
- The OpenTTD string ID of the first string that is being replaced. A star indicates that the string ID should be stored as a WORD (2-byte value). IDs are otherwise stored as bytes or extended byte depending on their values. [This feature was added to preserve the information from the decode GRF, but is not strictly necessary for **yagl** to work: it could just always used the WORD format and save some fuss.]

The block contains any number of strings presented in a readable format (all control codes are replaced with human-readable text in braces). 

In the example, each string is preceded with an inline comment which indicates the ID of the string that is being replaced. This is there as a guide and is not actually necessary. Comments are ignored when encoding YAGL.

**Features**

**Language codes**
 | Value | Code     | Name |
 |-------|----------|------|
 | 0x00  | `en_US`   | "English (US)" |
 | 0x01  | `en_GB`   | "English (GB)" |
 | 0x02  | `de_DE`   | "German" |
 | 0x03  | `fr_FR`   | "French" |
 | 0x04  | `es_ES`   | "Spanish" |
 | 0x05  | `eo_XX`   | "Esperanto" |
 | 0x06  | `io_XX`   | "Ido" |     
 | 0x07  | `ru_RU`   | "Russian" |
 | 0x08  | `ga_IE`   | "Irish" |   
 | 0x09  | `mt_MT`   | "Maltese" |
 | 0x0A  | `ta_IN`   | "Tamil" |
 | 0x0B  | `cv_RU`   | "Chuvash" | 
 | 0x0C  | `zh_TW`   | "Chinese (Traditional)" |
 | 0x0D  | `sr_BA`   | "Serbian" |
 | 0x0E  | `nn_NO`   | "Norwegian (Nynorsk)" |
 | 0x0F  | `cy_GB`   | "Welsh" |
 | 0x10  | `be_BY`   | "Belarusian" |
 | 0x11  | `mr_IN`   | "Marathi" |
 | 0x12  | `fo_FO`   | "Faroese" |
 | 0x13  | `gd_GB`   | "Scottish Gaelic" | 
 | 0x14  | `ar_EG`   | "Arabic (Egypt)" |
 | 0x15  | `cs_CZ`   | "Czech" |
 | 0x16  | `sk_SK`   | "Slovak" |
 | 0x18  | `bg_BG`   | "Bulgarian" |
 | 0x1B  | `af_ZA`   | "Afrikaans" |
 | 0x1E  | `el_GR`   | "Greek" |
 | 0x1F  | `nl_NL`   | "Dutch" |
 | 0x21  | `eu_ES`   | "Basque" |
 | 0x22  | `ca_ES`   | "Catalan" |
 | 0x23  | `de_LU`   | "Luxembourgish" | 
 | 0x24  | `hu_HU`   | "Hungarian" |
 | 0x26  | `mk_MK`   | "Macedonian" |
 | 0x27  | `it_IT`   | "Italian" |
 | 0x28  | `ro_RO`   | "Romanian" |
 | 0x29  | `is_IS`   | "Icelandic" |
 | 0x2A  | `lv_LV`   | "Latvian" |
 | 0x2B  | `lt_LT`   | "Lithuanian" |
 | 0x2C  | `sl_SI`   | "Slovenian" |
 | 0x2D  | `da_DK`   | "Danish" |
 | 0x2E  | `sv_SE`   | "Swedish" |
 | 0x2F  | `nb_NO`   | "Norwegian (Bokmal)" |
 | 0x30  | `pl_PL`   | "Polish" |
 | 0x31  | `gl_ES`   | "Galician" |
 | 0x32  | `fy_NL`   | "Frisian" |
 | 0x33  | `uk_UA`   | "Ukrainian" |
 | 0x34  | `et_EE`   | "Estonian" |
 | 0x35  | `fi_FI`   | "Finnish" |
 | 0x36  | `pt_PT`   | "Portuguese" |
 | 0x37  | `pt_BR`   | "Brazilian Portuguese" |
 | 0x38  | `hr_HR`   | "Croatian" |
 | 0x39  | `ja_JP`   | "Japanese" |
 | 0x3A  | `ko_KR`   | "Korean" |
 | 0x3C  | `ms_MY`   | "Malay" |
 | 0x3D  | `en_AU`   | "English (AU)" |
 | 0x3E  | `tr_TR`   | "Turkish" |
 | 0x42  | `th_TH`   | "Thai" |
 | 0x54  | `vi_VN`   | "Vietnamese" |
 | 0x55  | `es_MX`   | "Mexican Spanish" |
 | 0x56  | `zh_CN`   | "Chinese (Simplified)" |
 | 0x5A  | `id_ID`   | "Indonesian" |
 | 0x5C  | `ur_PK`   | "Urdu" |
 | 0x61  | `he_IL`   | "Hebrew" |
 | 0x62  | `fa_IR`   | "Persian" |
 | 0x66  | `latin`   | "Latin" | 
 | 0x7F  | `default` | "Default" |

**Control codes**

**Extended control codes**

## Links

The block structure described above is directly related to the corresponding record in the NewGRF specifications:

- [Action04 NewGRF specifications](https://newgrf-specs.tt-wiki.net/wiki/Action4)
