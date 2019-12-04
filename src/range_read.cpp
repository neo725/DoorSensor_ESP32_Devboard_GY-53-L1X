#include "range_read.h"

SensorResult InitialEmptySensorResult() {
  SensorResult result;

  result.range_in_mm = 0;
  result.measurement_level = -1;
  result.measurement_mode = -1;
  result.range_status = -1;

  return result;
}

SensorResult ReadSensor(HardwareSerial* sensorSerial) {
  SensorResult result =
    InitialEmptySensorResult();

  //uint8_t buffer[BYTES_PER_FRAME];
  uint8_t *buffer = new uint8_t[BYTES_PER_FRAME];

  // Wait 'till there are 8 Bytes (define by BYTES_PER_FRAME) waiting
  //while (Serial.available() < BYTES_PER_FRAME) {}
  while (sensorSerial->available() < BYTES_PER_FRAME) delay(1);

  //Serial.readBytes(buffer, BYTES_PER_FRAME);
  sensorSerial->readBytes(buffer, BYTES_PER_FRAME);

  // sensorSerial->end();

  // delete sensorSerial;

  // struct SensorResult result = CreateEmptySensorResult();
  // CreateEmptySensorResult(result);

  result.value = buffer[6];
/*
Ex: bytes per a frame
[5A, 5A, 15, 03, 0A, 20, 06, FC]

Distance = (0x0A << 8) | 0x20 = 2592 mm (距離值 2.592 m)
RangeStatus = (0x06 >> 4) & 0x0F = 0 (距離值可靠)
    0 : Valid, Ranging measurement is valid
    1 : 表示周圍環境光影響 Sigma_Fail, Raised if sigma estimator check is above the internal defined threshold
    2 : 表示返回的信號弱 Signal_Fail, Raised if signal value is below the internal defined threshold
    4 : 表示超出測量量程 OutOfBounds_Fail, Raised when phase is out of bounds
    5 : 表示硬體故障 Hardware_Fail, Raised in case of HW or VCSEL failure
    7 : 表示周圍環境有干擾噪音 Wrap_Target_Fail, Wrapper Target, not matching phases
    8 : 內部運算發生錯誤 Processing_Fail, Internal algorithm or overflow
    14 : 無效的測量 Range_Invalid, The reported range is invalid
Time = (0x06 >> 2) & 0x0F = 1 (測量時間為 110ms)
    0: 55ms
    1: 110ms (default)
    2: 200ms
    3: 300ms
Mode = (0x06 & 0x03) = 2 (測量模式為中距離模式，量程為 50 - 4000 mm)
*/

  result.range_in_mm = buffer[4] << 8 | buffer[5];
  result.range_status = buffer[6] >> 4 & 0x0F; // 量測狀態，= 0 表示距離值可靠
  result.measurement_level = buffer[6] >> 2 & 0x0F; // 量測距離類型
  result.measurement_mode = buffer[6] & 0x03; // 量測模式

  return result;
}