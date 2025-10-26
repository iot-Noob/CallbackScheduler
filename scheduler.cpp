#include "scheduler.h"

int getMaxTasks() {
  // Your board detection code here (same as before)
  #if defined(ARDUINO_AVR_UNO)
    if (Serial) Serial.println("Arduino UNO - 2KB RAM");
    return 8;
  #elif defined(ESP8266)
    if (Serial) Serial.println("ESP8266 - 80KB RAM");
    return 50;
  // ... rest of your detection code
  #else
    if (Serial) Serial.println("Unknown Architecture");
    return 10;
  #endif
}