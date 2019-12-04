#include "RangeSensorThreadClass.h"

HardwareSerial sensorSerial(1);

// // must have, because this class inherit from Thread
// void RangeThreadClass::run(void) {
//   RunMeansurement();

//   runned();
// }

// Constructors
RangeThreadClass::RangeThreadClass(int sensorIndex) {
    // RangeThreadClass::SensorIndex = sensorIndex;
    sensorIndex = sensorIndex == 1 ? 1 : 2;

    SensorIndex = sensorIndex;

		// sensorSerial = RangeThreadClass::GetSensorSerial();

    // HardwareSerial sensorSerial(sensorIndex);

    // if (sensorIndex == 1) {
    //     sensorSerial.begin(9600, SERIAL_8N1, RXD1, TXD1);
    // }
    // else {
    //     sensorSerial.begin(9600, SERIAL_8N1, RXD2, TXD2);
    // }
    // sensorSerial.setRxBufferSize(BYTES_PER_FRAME);

    // while (!sensorSerial) delay(1);

    Ready = true;
}

// DeConstructors
RangeThreadClass::~RangeThreadClass() {
}

// void RangeThreadClass::RunMeansurement() {
//   // while(true) {
//   //   delay(LOOP_DELAY / 2);

//     SensorResult result = ReadSensor(sensorSerial);

//     RangeInMM = -1;
//     MeasurementLevel = result.measurement_level;
//     MeasurementMode = result.measurement_mode;
//     RangeStatus = result.range_status;

//     if (result.range_status == 0) {
//       RangeInMM = result.range_in_mm;
//     }
//   // }
// }