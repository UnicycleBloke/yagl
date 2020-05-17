[Back to table of contents](../index.md)

# Action00

Action00 (`properties`) defines properties for one or more instances of a feature, such as Trains. If more than one instance of the feature is represented, then it **must** have the same set of properties defined for each one. 

The following example shows a typical example, in this case for a single train. All properties blocks have the same format, but with different feature types. Each feature type has a different set of properties, some or all of which may be listed for each instance. 

```bash
properties<Trains, 0x0087> // Action00
{
    // instance_id: 0x0087
    {
        introduction_date: date(1920/1/1);
        long_introduction_date: date(1920/1/1);
        model_life_years: 44;
        visual_effect: effect(SteamPuffs, 0x08, Disable);
        climate_availability: Temperate | Arctic | Tropical | Toyland;
        sprite_id: 0xFD;
        is_dual_headed: false;
        shorten_vehicle: 0x00;
        power_from_each_wagon: 0x0000;
        mask_for_var_42: 0x00;
    }
}
```
#### Properties

All Action00 blocks have the same format. 

The block name is `properties` and this is decorated with two values:

- The type of the feature whose properties are being assigned (`Trains` in the example). See [features](../sundries/features.md) for the list of possibilities.
- The instance ID of the first feature whose properties are being assigned (The train with ID 0x0087 in the example).

The block contains one or more instance blocks each containing a list of properties.

Each property appears in the format `<property_name>: <property_value>;`. The value for properties is typically a simple integer, but some have specialised formats. Any integers can be written as decimal, hexadecimal, octal or binary.

A given property may appear more than once in each instance block, but **yagl** does not preserve all the values: only the last value for each property is kept. As far as I am aware, there is no advantage to be gained from duplicated properties - they have been observed in some GRFs and are perhaps the result of auto-generating NML.

## Properties for specific features

Each type of feature has its own set of properties:

- [Vehicles](../features/action00_vehicles.md)        
    - [Trains](../features/action00_trains.md)          
    - [Ships](../features/action00_ships.md)           
    - [Aircraft](../features/action00_aircraft.md)        
    - [RoadVehicles](../features/action00_road_vehicles.md)          
- [Stations](../features/action00_stations.md)        
- [Canals](../features/action00_canals.md)          
- [Bridges](../features/action00_bridges.md)         
- [Houses](../features/action00_houses.md)          
- [GlobalSettings](../features/action00_global_settings.md)  
- [IndustryTiles](../features/action00_industry_tiles.md)   
- [Industries](../features/action00_industries.md)      
- [Cargos](../features/action00_cargos.md)          
- [SoundEffects](../features/action00_sound_effects.md)    
- [Airports](../features/action00_airports.md)                
- [Objects](../features/action00_objects.md)         
- [RailTypes](../features/action00_rail_types.md)       
- [AirportTiles](../features/action00_airport_tiles.md)    
- [RoadTypes](../features/action00_road_types.md)       
- [TramTypes](../features/action00_tram_types.md)       

## Links

- [Action00 NewGRF specifications](https://newgrf-specs.tt-wiki.net/wiki/Action0)  
- [YAGL feature enumeration](../sundries/features.md)