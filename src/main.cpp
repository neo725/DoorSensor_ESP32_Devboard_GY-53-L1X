#include <Arduino.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include <ThreadController.h>

#include "RangeSensorThreadClass.h"
// #include "range_read.h"
#include "i2cscannning.h"
#include "main.h"

#include "OLEDDisplay.h"
#include "SSD1306Wire.h"
#include "SSD1306.h"

/*
NodeMCU-32S (ESP-WROOM-32)
GY-53-L1X ToF 紅外測距模組 * 2
I2C 0.96 OLED 顯示模組 * 1

Power usage :
NodeMCU-32S 500 mA
GY-53-L1X 20mA * 2 = 40 mA
OLED Display 2.5 mA

*/

// ThreadController that will controll all threads
// ThreadController sensorController = ThreadController();

HardwareSerial sensorSerial1(1);
HardwareSerial sensorSerial2(2);
// RangeThreadClass sensor1(1);

SSD1306Wire display(0x3C, SDA, SCL);

int max_range_in_mm = 0;
//int8_t currentSensorIndex = 0;

void drawText(String text) {
    display.setTextAlignment(TEXT_ALIGN_LEFT);

    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 0, text);
}

void drawSensor(SensorResult sensor, uint8_t columnIndex = 1) {
  // void drawSensor(RangeThreadClass sensor) {
    // Font Demo1

    int16_t x = (columnIndex - 1) * 50;
    
    // create more fonts at http://oleddisplay.squix.ch/
    display.setTextAlignment(TEXT_ALIGN_LEFT);

    // display.setFont(ArialMT_Plain_10);
    // String is_ready = sensor.Ready ? "Ready" : "Not Ready !!";
    // is_ready = "0 : " + is_ready;
    // display.drawString(0, 0, is_ready);

    display.setFont(ArialMT_Plain_10);
    String range_in_mm = (String)sensor.range_in_mm;
    //String range_in_mm = (String)sensor.RangeInMM;
    range_in_mm = "1 : " + range_in_mm;
    display.drawString(x, 10, range_in_mm);

    //display.setFont(ArialMT_Plain_16);
    display.setFont(ArialMT_Plain_10);
    String measurement_level = (String)sensor.measurement_level;
    // String measurement_level = (String)sensor.MeasurementLevel;
    measurement_level = "2 : " + measurement_level;
    display.drawString(x, 20, measurement_level);

    //display.setFont(ArialMT_Plain_24);
    display.setFont(ArialMT_Plain_10);
    String measurement_mode = (String)sensor.measurement_mode;
    // String measurement_mode = (String)sensor.MeasurementMode;
    measurement_mode = "3 : " + measurement_mode;
    display.drawString(x, 30, measurement_mode);

    display.setFont(ArialMT_Plain_10);
    String range_status = (String)sensor.range_status;
    // String range_status = (String)sensor.RangeStatus;
    range_status = "4 : " + range_status;
    display.drawString(x, 40, range_status);
}

void timerCallback() {
  // sensorController.run();
}

void setup() {
  Serial.begin(115200);

  while (!Serial) delay(1);

  Serial.setDebugOutput(true);

  Serial.println("Serial ready !");
  
  Serial.println("Waiting sensorSerial1(1) ready...");
  sensorSerial1.begin(9600, SERIAL_8N1, RXD1, TXD1);
  sensorSerial1.setRxBufferSize(BYTES_PER_FRAME);
  while (!sensorSerial1) delay(1);
  Serial.println("sensorSerial1(1) ready !");

  Serial.println("Waiting sensorSerial1(2) ready...");
  sensorSerial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  sensorSerial2.setRxBufferSize(BYTES_PER_FRAME);
  while (!sensorSerial2) delay(1);
  Serial.println("sensorSerial1(2) ready !");

  // while(!sensor1.Ready) delay(1);
  // sensor1.setInterval(LOOP_DELAY);

  // sensorController.add(&sensor1);

  Serial.println("Prepare wire begin...");
  Wire.begin();

  Serial.println("Wire ready !");

  //Serial.println("Serial prepare ready !");
  //I2CScanning();
  
  Serial.println("Prepare display...");
  display.init();
  display.flipScreenVertically();
  //display.setFont(ArialMT_Plain_10);

  drawText("Waiting...");

  Serial.println("[Setup] Power up delay...");
  delay(POWER_UP_DELAY);
  Serial.println("[Setup] Power up delay ready !");

  /*
		If using DueTimer...
	*/
	// Timer1.attachInterrupt(timerCallback).start(10000);

	/*
		If using TimerOne...
	*/
	// Timer1.initialize(20000);
	// Timer1.attachInterrupt(timerCallback);
	// Timer1.start();
  
}

void loop() {

  // sensorController.run();
  SensorResult result1 = ReadSensor(&sensorSerial1);
  SensorResult result2 = ReadSensor(&sensorSerial2);
  // // clear the display
  display.clear();

  // // draw the current demo method
  // //demos[demoMode]();
  drawSensor(result2, 1);
  drawSensor(result1, 2);

  display.setTextAlignment(TEXT_ALIGN_LEFT);

  display.setFont(ArialMT_Plain_10);
  display.drawString(110, 0, String(millis()));

  // // write the buffer to the display
  display.display();

  delay(LOOP_DELAY / 2);
}