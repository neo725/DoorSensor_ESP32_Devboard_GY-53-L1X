#include <Thread.h>
#include <HardwareSerial.h>

#include "range_read.h"

// #define LOOP_DELAY     150
//#define MAX_RANGE       2500 // in mm
#define MAX_RANGE       1500 // in mm

// #define RXD1 9
// #define TXD1 10
#define RXD1 4
#define TXD1 5

#define RXD2 16
#define TXD2 17

// HardwareSerial sensorSerial(1);

class RangeThreadClass: public Thread
{
    private:
        // Private Property
        int max_range_in_mm = 0;

    private:
        // Private method
        // void RunMeansurement(void);
        void RunMeansurement() {
            
            // SensorResult result = ReadSensor(sensorSerial);
            SensorResult result = ReadSensor(GetSensorSerial());

            RangeInMM = -1;
            MeasurementLevel = result.measurement_level;
            MeasurementMode = result.measurement_mode;
            RangeStatus = result.range_status;

            if (result.range_status == 0) {
                RangeInMM = result.range_in_mm;
            }
        };

    public:
        // Constructors
        RangeThreadClass(int cs);
        // DeConstructors
        ~RangeThreadClass(void);

    public:
        // Public Property
        int SensorIndex = 1;
        int RangeInMM = 0;
        int MeasurementLevel = 0;
        int MeasurementMode = 0;
        int RangeStatus = 0;
        bool Ready = false;

        HardwareSerial* SensorSerial;

    public:
        // Public Method
        HardwareSerial* GetSensorSerial() {
            int sensorIndex = SensorIndex;

            if (SensorSerial == NULL) {
                HardwareSerial sensorSerial(sensorIndex);

                if (sensorIndex == 1) {
                    sensorSerial.begin(9600, SERIAL_8N1, RXD1, TXD1);
                }
                else {
                    sensorSerial.begin(9600, SERIAL_8N1, RXD2, TXD2);
                }
                sensorSerial.setRxBufferSize(BYTES_PER_FRAME);

                while (!sensorSerial) delay(1);

                SensorSerial = &sensorSerial;
            }

            return SensorSerial;
        };

        void run() {
            RunMeansurement();
            
            runned();
        }
};