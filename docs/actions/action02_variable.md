[Back to table of contents](../index.md)
[Back to Action02](../actions/action02.md)        

# Action 02 (variable)

Action02 (`switch`) is used to select one of several (other) Action02 entries based on a calculation using in-game variables. This provides a sophisticated way of deciding what graphics to use in particular circumstances.

The following example includes a few comments for guidance generated automatically when a GRF is decoded. The YAGL format for this record is as follows:

```bash
// Record #3203
switch<Trains, 0xFC, PrimaryDWord> // Action02 variable
{
    expression:
    {
        value1 = variable[0xF2] & 0x000000FF;
    };
    ranges:
    {
        0x00000001: 0x00FD;
        0x00000002: 0x00FB;
        0x00000003: 0x00F5;
        0x00000004: 0x00FA;
    };
    default: 0x00FC;
}
```

#### Properties

The block name is `switch` and this is decorated with three values:
- The type of the feature whose graphics are being assigned (`Trains` in the example). See [features](../sundries/features.md) for the list of possibilities.
- The set ID to be associated with this Action02 record (`0xFC` in the example).
- One of the following enumeration:
  - `PrimaryByte`: NFO value 0x81.
  - `RelatedByte`: NFO value 0x82.
  - `PrimaryWord`: NFO value 0x85.
  - `RelatedWord`: NFO value 0x86.
  - `PrimaryDWord`: NFO value 0x89.
  - `RelatedDWord`: NFO value 0x8A.
  - These select either the primary object or its "related" object, and whether to use a byte, word or double word of the relevant variable. See [Variable Action02](https://newgrf-specs.tt-wiki.net/wiki/VariationalAction2) for details.

The properties inside the block are:

| Name | Description |
|-|-|
|`expression`| A mathematical expression which calculates a value used to perform the selection based on ranges. This is described in more detail below. |
|`ranges`| A set of ranges which map the result of the expression to a particular Action02 ID. Each entry has the form `<value>: <set-id>;` or `<start-value>..<end-value>: <set-id>;`. The latter form allows multiple expression values to choose the same Action02 set ID. If the set ID value has the high bit set (i.e. the value is at least `0x8000`), then it is a callback result. |
|`default`| The Action02 set ID which is selected in if nothing else matches. |

#### Expressions

The basic expression has the form:

```bash
value1 = variable[<var-num>] >> <shift-num> & <and-mask> + <add-value> / <div-value>;
```

- `var-num` is the index of the variable to be used in the calculation.
- `shift-num` may be zero, in which case `>> <shift-num>` may be omitted.
- `add-value` may be zero, in which case `+ <add-value>` may be omitted. The `add-value` is only used in conjunction with the following `div-value`.
- `div-value` may be zero, in which case `/ <div-value>` may be omitted. The operator can also be `%` rather than `/`.

More details can be gleaned from [Variable Action02](https://newgrf-specs.tt-wiki.net/wiki/VariationalAction2).

Expressions may be combined into arbitrarily long chains, for example:

```bash
value1 = variable[0x1A] & 0x0000D06C;

value2 = variable[0x1A] & 0x00000010;
value1 = ShiftLeft(value1, value2);

value2 = variable[0x1A] & 0x00000080;
value1 = TempStore(value1, value2);

...
```

In this case `value1` is calculated as described above. Then `value2` is calculated in the same way. After that, the two values are combined using an operator, and the result is typically placed into `value1` in preparation for the subsequent calculation. The final `value1` in the chain is used for the range selection.

The following joining operations are possible:

| Name | Value | Description | |
|-|-|-|-|
| `Addition`           | 0x00 | `value1 = value1 + value2` ||
| `Subtraction`        | 0x01 | `value1 = value1 - value2` ||
| `SignedMin`          | 0x02 | `value1 = min(value1, value2)` | Note 1 |
| `SignedMax`          | 0x03 | `value1 = max(value1, value2)` | Note 1 |
| `UnsignedMin`        | 0x04 | `value1 = min(value1, value2)` | Note 2 |
| `UnsignedMax`        | 0x05 | `value1 = max(value1, value2)` | Note 2 |
| `SignedDiv`          | 0x06 | `value1 = value1 / value2` | Note 1 |
| `SignedMod`          | 0x07 | `value1 = value1 % value2` | Note 1 |
| `UnsignedDiv`        | 0x08 | `value1 = value1 / value2` | Note 2 |
| `UnsignedMod`        | 0x09 | `value1 = value1 % value2` | Note 2 |
| `Multiply`           | 0x0A | `value1 = value1 * value2` ||
| `BitwiseAnd`         | 0x0B | `value1 = value1 & value2` ||
| `BitwiseOr`          | 0x0C | `value1 = value1 | value2` ||
| `BitwiseXor`         | 0x0D | `value1 = value1 ^ value2` ||
| `TempStore`          | 0x0E | `temp_storage[value2] = value1` | Note 4 |
| `Assign`             | 0x0F | `value1 = value2` ||
| `PermStore`          | 0x10 | `permanent_storage[value2] = value1` | Note 5 |
| `RotateRight`        | 0x11 | `value1 = value1 rotate-right value2` ||
| `SignedCmp`          | 0x12 | `value1` set to 0 if `value1 < value2`, 1 if `value1 == value2` and 2 if `value1 > value2` |  Note 1. |
| `UnsignedCmp`        | 0x13 | `value1` set to 0 if `value1 < value2`, 1 if `value1 == value2` and 2 if `value1 > value2`| Note 2. |
| `ShiftLeft`          | 0x14 | `value1 = value1 << value2` | Note 3 |
| `UnsignedShiftRight` | 0x15 | `value1 = value1 >> value2` | Notes 2 and 3 |
| `SignedShiftRight`   | 0x16 | `value1 = value1 >> value2` | Notes 1 and 3 |

1. `value1` and `value2` are both considered signed.
2. `value1` and `value2` are both considered unsigned.
3. `value2` should be in the range 0 to 31.
4. Temporary storage (variable 0x7D).
5. Permanent storage (variable 0x7C).

## Links

The block structure described above is directly related to the corresponding record in the NewGRF specifications:

- [NewGRF Action02 specifications](https://newgrf-specs.tt-wiki.net/wiki/VarAction2Advanced)
- [YAGL feature enumeration](../sundries/features.md)