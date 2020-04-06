[Back to table of contents](../index.md)

# GRF labels

The NewGRF specifications make used of GRF labels, also known as GRF IDs, in a number of different places. Examples include cargo labels, road types, rail types, and more. They all have the same internal format, which is a double word - a four byte number. The internal value may be rendered in three different ways:

**As a number**

The clearest way to represent a four-byte number is in hexadecimal, such as `0x01020304`, but you can use any number format you wish. In the preceding example, the four bytes are `0x01`, `0x02`, `0x03` and `0x04`. The values of the bytes would be less obvious with a decimal, octal or binary number.

**As an identifier**

Some GRF labels can also be represented as an *identifier*. This is a four byte name which conforms to the rules of [YAGL syntax](../yagl_syntax.md). The name must begin with an English letter (`A-Z`) or underscore, must contain only letters, digits or underscores, and must be exactly four characters in length. Examples include `COAL`, `WOOD`, `xyz3` and `A__Z`. The four characters are internally converted to their byte values (from the Latin-1 character codes). For example `COAL` has the four bytes `0x43 (=C)`, `0x4F (=O)`, `0x41 (=A)` and `0x4C (=L)`. When presented as a number, the bytes are reversed in order (for reasons), so the number value equivalent to `COAL` is `0x4C414F43`.

**As a string**

Another way to represent a GRF label is as a four-character string. This is similar to using an identifier except that the value is shown in double quotes, and any bytes are allowed. Bytes representing printable characters appear as normal characters. Other byte values are shown as hexadecimal embedded within the string. For example `"COAL"` or `"ABC\x01"`. The string should represent exactly four characters, but the escapes for non-printable characters mean that the string itself might be longer. In the second example here the first three bytes are normal printable characters `"ABC"`, and the fourth byte has the value `0x01`. This is format more useful than identifiers because it can represent any GRF label, and more readable than plain numbers. 

## Links

- [YAGL syntax](yagl_syntax.md) - a basic introduction to core ideas.
