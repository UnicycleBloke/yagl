[Back to table of contents](../index.md)
[Back to Action00](../actions/action00.md)

# Action00 - Vehicles

The four classes of vehicles (Trains, Aircraft, Ships and RoadVehicles) share a common set of properties.

```c++
// NOTE: These numbers are mostly made up rubbish for the sake of demonstrating the format of each property.
properties<Ships, 0x0023> // Action00
{
    // instance_id: 0x0023
    {
        // These properties are common to all four vehicle types.
        introduction_date: date(1925/3/5);
        reliability_decay_speed: 18;
        vehicle_life_years: 36;
        model_life_years: 53;
        climate_availability: Temperate | Arctic | Tropical | Toyland;
        loading_speed: 0x27;
    }
}
```

#### Property details

The properties allowed inside the instance block are as follows (see [Vehicles](https://newgrf-specs.tt-wiki.net/wiki/Action0/Vehicles) for more details). Unless otherwise specified, the formats of the property values are simple integers:

| Name | NFO | Size | Description |
|-|-|-|-|
| `introduction_date`       | 0x00 | W | A date in the form `date(YYYY/M/D)`, which must be no earlier than 1920 (short date). |
| `reliability_decay_speed` | 0x02 | B |  |
| `vehicle_life_years`      | 0x03 | B |  |
| `model_life_years`        | 0x04 | B |  |
| `climate_availability`    | 0x06 | B | A bitfield containing values from the set: [`Temperate`, `Arctic`, `Tropical`, `Toyland`] ORed together. |
| `loading_speed`           | 0x07 | B |  |

For details of the feature-specific properties, follow these links:

- [Trains](../features/action00_trains.md)          
- [Ships](../features/action00_ships.md)           
- [Aircraft](../features/action00_aircraft.md)        
- [RoadVehicles](../features/action00_road_vehicles.md)   

## Links

- [Vehicles NewGRF specifications](https://newgrf-specs.tt-wiki.net/wiki/Action0/Vehicles)  
  - [Trains NewGRF specifications](https://newgrf-specs.tt-wiki.net/wiki/Action0/Vehicles/Trains)          
  - [Road vehicles NewGRF specifications](https://newgrf-specs.tt-wiki.net/wiki/Action0/Vehicles/RoadVehicles)      
  - [Ships NewGRF specifications](https://newgrf-specs.tt-wiki.net/wiki/Action0/Vehicles/Ships)           
  - [Aircraft NewGRF specifications](https://newgrf-specs.tt-wiki.net/wiki/Action0/Vehicles/Planes)        
