Vintage Wine Authentication
```
api Sommelier::verify_vintage(
    bottle_id: String(regex="^[A-Z]{2}-[0-9]{4}-[A-Z]{3}$"), // "FR-1945-BRD"
    chemical_analysis: Array<Float(min=0.0, max=100.0), len=12>, // Alcohol%, pH, etc
    uv_light_response: Bool // Fluorescence under UV
) -> Enum["GENUINE", "FORGERY", "INCONCLUSIVE"];
```

Antarctic Weather Station Alerts
```
api Meteorology::check_blizzard_risk(  
    station_id: String(regex="^ANT-[0-9]{3}$"),           // "ANT-042"  
    wind_speed_kmh: Float(min=0.0, max=300.0),  
    visibility_m: Float(min=0.0, max=10000.0)  
) -> Enum["SAFE", "WARNING", "CRITICAL"];  
```

Industrial Sensor Calibration
```
api IoT::calibrate_sensor(  
    sensor_id: UUID(version=4),                   // RFC 4122 UUIDv4  
    calibration_points: Array<Float(min=-200.0, max=1000.0), len=10>,  
    tolerance: Float(min=0.1, max=100.0)         // Percentage  
) -> Struct<{status: Enum["OK", "OUT_OF_RANGE"], adjustments: Array<Float>}>;  
```

DNA Sequence Pattern Matching
```
api Bioinformatics::find_pattern(  
    dna_sequence: DNAString("ACGT"),  
    min_length: Int(3..50),  
    max_mutations: Int(0..5)  
) -> PatternLocations;  
```