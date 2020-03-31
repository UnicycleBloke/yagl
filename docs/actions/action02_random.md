[Back to table of contents](../index.md)
[Back to Action02](../actions/action02.md)        

# Action 02 (random)

Action02 (`random_switch`) is used to create randomised results which pick one of several (other) Action02 entries.

The following example includes a few comments for guidance generated automatically when a GRF is decoded. The YAGL format for this record is as follows:

```bash
// Record #10210
random_switch<Trains, 0xF4, Related> // Action02 random
{
    triggers: 0x00;
    rand_bit: 0x00;
    set_ids: // set_id: probability;
    {
        0x00F0: 1;
        0x00FC: 1;
    };
}
```

#### Properties

The block name is `random_switch` and this is decorated with three or four values:
- The type of the feature whose graphics are being assigned (`Trains` in the example). See [features](../sundries/features.md) for the list of possibilities.
- The set ID to be associated with this Action02 record (`0xF4` in the example).
- A value which indicates how to randomise. This may be one of the following enumeration value:
  - `Object`: Use to randomise the object (vehicle, station, building, industry, object) based on its own triggers and bits. 
  - `Related`: Use to randomise the object based on its "related" object. 
  - `Consist`: Use to randomise the vehicle based on any vehicle in the consist. 
- If the third value is `Consist`, then there is a fourth value. This describes how to determine which vehicle in the consist to use for randomisation, and can be one of the following enumeration values:
  - `BackwardFromVehicle`: Count back (away from the engine), starting at this vehicle.
  - `ForwardFromVehicle`: Count forward (toward the engine), starting at this vehicle. 
  - `BackwardFromEngine`: Count back, starting at the engine.
  - `BackwardFromSameID`: Count back, starting at the first vehicle in this chain of vehicles with the same ID, as for vehicle variable 41.

The properties inside the block are:

| Name | Description |
|-|-|
| `triggers` | This is a bit mask of triggers which cause re-randomising. Normally, any matching trigger causes the graphics to be randomised again, but if you add 80 to the bitmask, the re-randomising only happens if all triggers have occurred. Trigger bits are feature-specific. See [NewGRF specs]((https://newgrf-specs.tt-wiki.net/wiki/RandomAction2)). |
| `rand_bit` | Setting randbit determines the first bit to be re-randomized, as well as basing the random graphics on. The total number of bits used is the 2-logarithm of nrand below (e.g., for nrand=16, 4 bits are used). **yagl** will report an error if this is not the case. |
| `set_ids`  | Action02 IDs to randomly choose from. Each pair of numbers represents an Action02 ID and a duplication count: `<set-id>: <count>;`. Giving an ID a larger count than others increases the probability that it will be selected. The counters should sum to a power of two. |

## Links

The block structure described above is directly related to the corresponding record in the NewGRF specifications:

- [NewGRF random Action02 specifications](https://newgrf-specs.tt-wiki.net/wiki/RandomAction2)
- [YAGL feature enumeration](../sundries/features.md)

