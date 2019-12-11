// #include <Arduino.h>
#include <HardwareSerial.h>
// #include <Wire.h>

#include <stdint.h>

#define BYTES_PER_FRAME 8

/*
measurement_level :
0 : Long distance,    0 ~ 2 M,    T = 35 ms,  +- 4cm
1 : Fast measurement, 0 ~ 1.2 M,  T = 22 ms
2 : High precision,   0 ~ 1.2 M,  T = 200 ms, +- 1cm
3 : General,          0 ~ 1.2 M,  T = 35 ms,  +- 2cm
 */

typedef struct MeansurementResult {
    int range_in_mm;
    int measurement_level;
    int measurement_mode;
    int range_status;

    bool ready;

    uint8_t value;
} SensorResult;

SensorResult InitialEmptySensorResult();
SensorResult ReadSensor(HardwareSerial*);