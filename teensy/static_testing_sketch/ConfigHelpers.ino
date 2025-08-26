/* Helpers for sensor config */
#include "SparkFun_Qwiic_XM125_Arduino_Library.h"

// --- Configuration constants ---
const uint32_t START_DIST = 30;    // mm
const uint32_t END_DIST = 1000;  // mm
const uint32_t MAX_STEP_LENGTH = 0;  // 0 = auto based on profile
const uint32_t MAX_PROFILE  = XM125_DISTANCE_PROFILE5;
const bool     CLOSE_RANGE_LEAKAGE_CANCELLATION = true;
const uint32_t SIGNAL_QUALITY = 15700;   // factor of 1000 from exp tool
const uint32_t THRESHOLD_METHOD = XM125_DISTANCE_CFAR;
const uint32_t PEAKSORTING_METHOD = XM125_DISTANCE_CLOSEST;
const uint32_t REFLECTOR_SHAPE = XM125_DISTANCE_PLANAR; // or GENERIC
const uint32_t NUM_FRAMES_REC_THRESH = 20;
const uint32_t FIXED_AMP_THRESH_VAL = 100000;
const uint32_t FIXED_STRENGTH_THRESH_VAL = 0;
const uint32_t THRESHOLD_SENSITIVITY = 500; // factor of 1000

// error helper
void checkError(const char* name, int err) {
  if (err != 0) {
    Serial.print(name);
    Serial.print(" error: ");
    Serial.println(err);
  }
}

// config wrapper
void configureSensor(SparkFunXM125Distance &radarSensor, uint32_t &errorStatus, uint32_t &startVal, uint32_t &endVal) {
  // reset presets
  radarSensor.setCommand(SFE_XM125_DISTANCE_RESET_MODULE);
  radarSensor.busyWait();

  // check error/busy bits
  radarSensor.getDetectorErrorStatus(errorStatus);
  if (errorStatus != 0) {
    Serial.print("Detector status error: ");
    Serial.println(errorStatus);
  }
  delay(100);

  // set start
  checkError("Start", radarSensor.setStart(START_DIST));
  radarSensor.getStart(startVal);
  Serial.print("Start Val: ");
  Serial.println(startVal);

  // set end
  checkError("End", radarSensor.setEnd(END_DIST));
  radarSensor.getEnd(endVal);
  Serial.print("End Val: ");
  Serial.println(endVal);

  // other configs
  checkError("Threshold Sensitivity", radarSensor.setThresholdSensitivity(THRESHOLD_SENSITIVITY));
  checkError("Fixed Amp Thresh", radarSensor.setFixedAmpThreshold(FIXED_AMP_THRESH_VAL));
  checkError("Fixed Strength Thresh", radarSensor.setFixedStrengthThresholdValue(FIXED_STRENGTH_THRESH_VAL));
  checkError("Max Step", radarSensor.setMaxStepLength(MAX_STEP_LENGTH));
  checkError("Max Profile", radarSensor.setMaxProfile(MAX_PROFILE));
  checkError("Leakage Cancel", radarSensor.setCloseRangeLeakageCancellation(CLOSE_RANGE_LEAKAGE_CANCELLATION));
  checkError("Signal Quality", radarSensor.setSignalQuality(SIGNAL_QUALITY));
  checkError("Threshold Method", radarSensor.setThresholdMethod(THRESHOLD_METHOD));
  checkError("Peak Sorting", radarSensor.setPeakSorting(PEAKSORTING_METHOD));
  checkError("Reflector Shape", radarSensor.setReflectorShape(REFLECTOR_SHAPE));
  checkError("Num Frames", radarSensor.setNumFramesRecordedThreshold(NUM_FRAMES_REC_THRESH));

  // apply config
  if (radarSensor.setCommand(SFE_XM125_DISTANCE_APPLY_CONFIGURATION) != 0) {
    radarSensor.getDetectorErrorStatus(errorStatus);
    if (errorStatus != 0) {
      Serial.print("Detector status error: ");
      Serial.println(errorStatus);
    }
    Serial.println("Configuration application error");
  }
}
