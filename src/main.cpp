#include <Arduino.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include <SPI.h>
#include <ThreadController.h>
#include <WiFi.h>

#include "RangeSensorThreadClass.h"
// #include "range_read.h"
#include "i2cscannning.h"
#include "main.h"

#include "OLEDDisplay.h"
#include "SSD1306Wire.h"
#include "SSD1306.h"

/*
NodeMCU-32S (ESP-WROOM-32)
NodeMCU-32S DevBoard

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

// WiFi Setting in Header file (main.h)   // the Wifi radio's status

// void drawText(int16_t x, int16_t y, String text){
//   //display.setTextAlignment(TEXT_ALIGN_LEFT);

//   //display.setFont(ArialMT_Plain_10);
//   display.drawString(x, y, text);
// }

void clearDisplay() {
  display.clear();
}
void drawText(uint16_t rowIndex, int16_t columnIndex, String text) {
  int16_t x = columnIndex * 50;
  int16_t y = rowIndex * 10;

  // display.drawString(x, 50, "WIFI : " + String(WIFI_CONNECTED));
  //drawText(x, y, text);
  display.drawString(x, y, text);
}

void drawText(String text) {
  drawText(0, 0, text);
}

// void drawString(uint16_t rowIndex = 0, int16_t columnIndex = 0, bool text) {
//   drawText(rowIndex, columnIndex, String(text));
// }

// void drawString(uint16_t rowIndex = 0, int16_t columnIndex = 0, int16_t text) {
//   drawText(rowIndex, columnIndex, String(text));
// }

// void drawString(uint16_t rowIndex = 0, int16_t columnIndex = 0, int8_t text) {
//   drawText(rowIndex, columnIndex, String(text));
// }

void drawSensor(SensorResult sensor, int16_t columnIndex = 0) {
    // create more fonts at http://oleddisplay.squix.ch/
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    
    int16_t x = 0;

    if (columnIndex > 1) {
      display.setFont(ArialMT_Plain_10);
      String range_in_mm = (String)sensor.range_in_mm;
      //String range_in_mm = (String)sensor.RangeInMM;
      range_in_mm = String(columnIndex) + " : " + range_in_mm;

      drawText(0, columnIndex, range_in_mm);
    }

    // String is_ready = sensor.ready ? "Ready" : "Not Ready !!";
    // is_ready = "0 : " + is_ready;
    // drawString(sensor, 0, 0, is_ready);

    display.setFont(ArialMT_Plain_10);
    String range_in_mm = (String)sensor.range_in_mm;
    //String range_in_mm = (String)sensor.RangeInMM;
    range_in_mm = "1 : " + range_in_mm;
    drawText(x, 1, range_in_mm);

    //display.setFont(ArialMT_Plain_16);
    display.setFont(ArialMT_Plain_10);
    String measurement_level = (String)sensor.measurement_level;
    // String measurement_level = (String)sensor.MeasurementLevel;
    measurement_level = "2 : " + measurement_level;
    drawText(x, 2, measurement_level);

    //display.setFont(ArialMT_Plain_24);
    display.setFont(ArialMT_Plain_10);
    String measurement_mode = (String)sensor.measurement_mode;
    // String measurement_mode = (String)sensor.MeasurementMode;
    measurement_mode = "3 : " + measurement_mode;
    drawText(x, 3, measurement_mode);

    display.setFont(ArialMT_Plain_10);
    String range_status = (String)sensor.range_status;
    // String range_status = (String)sensor.RangeStatus;
    range_status = "4 : " + range_status;
    display.drawString(x, 4, range_status);


    //display.drawString(x, 50, "WIFI : " + String(WIFI_CONNECTED));
}

void timerCallback() {
  // sensorController.run();
}

void setup() {
  Serial.begin(115200);

  while (!Serial) delay(1);

  delay(10);

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

// // check for the presence of the shield:
//   if (WiFi.status() == WL_NO_SHIELD) {
//     Serial.println("WiFi shield not present");
//     // don't continue:
//     while (true);
//   }

  // String fv = WiFi.firmwareVersion();
  // if (fv != "1.1.0") {
  //   Serial.println("Please upgrade the firmware");
  // }

  // // attempt to connect to Wifi network:
  // while (status != WL_CONNECTED) {
  //   Serial.print("Attempting to connect to WPA SSID: ");
  //   Serial.println(ssid);
  //   // Connect to WPA/WPA2 network:
  //   status = WiFi.begin(ssid, pass);

  //   WIFI_CONNECTED = true;

  //   // wait 10 seconds for connection:
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
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  // Serial.println("");
  // Serial.println("WiFi connected");
  // Serial.println("IP address: ");
  // Serial.println(WiFi.localIP());

  drawText(0, 5, "WIFI : " + String(ssid) + ", IP : " + WiFi.localIP());
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