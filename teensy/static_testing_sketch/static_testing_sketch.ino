#include "SparkFun_Qwiic_XM125_Arduino_Library.h"
#include <Arduino.h>
#include "DistanceReader.h"

SparkFunXM125Distance radarSensor;

// for python
bool measuring = false;

uint8_t i2cAddress = SFE_XM125_I2C_ADDRESS;
uint32_t startVal = 0;
uint32_t endVal = 0;
uint32_t numDistances = 9;
uint32_t calibrateNeeded = 0;
uint32_t measDistErr = 0;
uint32_t errorStatus = 0;
int16_t temperature = 0;

// Write to variables
uint32_t distancePeakStrength0 = 0;
uint32_t distancePeakStrength1 = 0;
uint32_t distancePeakStrength2 = 0;
uint32_t distancePeakStrength3 = 0;
uint32_t distancePeakStrength4 = 0;
uint32_t distancePeakStrength5 = 0;
uint32_t distancePeakStrength6 = 0;
uint32_t distancePeakStrength7 = 0;
uint32_t distancePeakStrength8 = 0;
uint32_t distancePeakStrength9 = 0;
uint32_t distancePeak0 = 0;
uint32_t distancePeak1 = 0;
uint32_t distancePeak2 = 0;
uint32_t distancePeak3 = 0;
uint32_t distancePeak4 = 0;
uint32_t distancePeak5 = 0;
uint32_t distancePeak6 = 0;
uint32_t distancePeak7 = 0;
uint32_t distancePeak8 = 0;
uint32_t distancePeak9 = 0;


void setup()
{
    // Start serial, Initialize Sensor
    Serial.begin(115200);
    Serial.println("Test Session Beginning: a121 absolute distance benchmarking");
    Serial.println("");
    Wire.begin();
    if (radarSensor.begin(i2cAddress, Wire) == 1) {
        Serial.println("Begin I2C Connection");
    }
    else {
        Serial.println("Device failed to setup - Freezing code.");
        while (1)
            ;
    }
    // Apply all configurations
    configureSensor(radarSensor, errorStatus, startVal, endVal);

    // get temperature
    if (radarSensor.getTemperature(temperature) != 0) {
        print("Temperature reading failed");
    }
    println("Tempurature: ");
    print(temperature);

    // Poll detector status until busy bit is cleared
    if (radarSensor.busyWait() != 0) {
        Serial.print("Busy wait error");
    }

    // Check detector status
    radarSensor.getDetectorErrorStatus(errorStatus);
    if (errorStatus != 0) {
        Serial.print("Detector status error: ");
        Serial.println(errorStatus);
    }
    Serial.println("Measuring Status: ");
    Serial.print(measuring)
    delay(1000);
}

void loop()
{

    if (Serial.available()) {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim();
        if (cmd == "START") measuring = true;
        else if (cmd == "STOP") measuring = false;
    }

    if (measuring) {

        // Initialize readings
        checkErrorsAndStart(radarSensor, errorStatus, measDistErr, calibrateNeeded);

        uint32_t distances[10];
        int32_t strengths[10];

        for (int i = 0; i < 10; i++) {
            (radarSensor.*distanceGetters[i])(distances[i]);
            (radarSensor.*strengthGetters[i])(strengths[i]);
        }

        for (int i = 0; i < 10; i++) {
            if (distances[i] != 0) {
                Serial.print("Object Detected: Peak");
                Serial.print(i);
                Serial.print(" Distance: ");
                Serial.print(distances[i]);
                Serial.print(" mm");

                Serial.print("  Strength: ");
                Serial.println(strengths[i]);
            }
        }   

    }
    // Half a second delay for easier readings
    delay(500);
}
