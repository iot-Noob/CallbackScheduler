#ifndef CallbackScheduler_h
#define CallbackScheduler_h

#include <LinkedList.h>
#include <Arduino.h>

// Board detection function
 int getMaxTasks() {
  // === CHIP-LEVEL DETECTION ===
  
  // ATtiny Family
  #if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    if (Serial) Serial.println("ATtiny25/45/85 (256-512B RAM)");
    return 3;
  #elif defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    if (Serial) Serial.println("ATtiny24/44/84 (256-512B RAM)");
    return 3;
  #elif defined(__AVR_ATtiny13__) || defined(__AVR_ATtiny13A__)
    if (Serial) Serial.println("ATtiny13 (64B RAM) - NOT RECOMMENDED");
    return 1;
  #elif defined(__AVR_ATtiny167__)
    if (Serial) Serial.println("ATtiny167 (512B RAM)");
    return 3;

  // ATmega Family
  #elif defined(__AVR_ATmega328P__)
    if (Serial) Serial.println("ATmega328P (Arduino UNO/Nano) - 2KB RAM");
    return 8;
  #elif defined(__AVR_ATmega32U4__)
    if (Serial) Serial.println("ATmega32U4 (Leonardo/Micro) - 2.5KB RAM");
    return 10;
  #elif defined(__AVR_ATmega2560__)
    if (Serial) Serial.println("ATmega2560 (Arduino Mega) - 8KB RAM");
    return 30;

  // === BOARD-LEVEL DETECTION ===
  
  // ATtiny Boards
  #elif defined(ARDUINO_AVR_ATTINY85)
    if (Serial) Serial.println("ATtiny85 - 512B RAM");
    return 3;
  #elif defined(ARDUINO_AVR_ATTINY84)
    if (Serial) Serial.println("ATtiny84 - 512B RAM");
    return 3;

  // Standard Arduino AVR
  #elif defined(ARDUINO_AVR_UNO)
    if (Serial) Serial.println("Arduino UNO - 2KB RAM");
    return 8;
  #elif defined(ARDUINO_AVR_NANO)
    if (Serial) Serial.println("Arduino NANO - 2KB RAM");
    return 8;  
  #elif defined(ARDUINO_AVR_MEGA2560)
    if (Serial) Serial.println("Arduino MEGA 2560 - 8KB RAM");
    return 30;
  #elif defined(ARDUINO_AVR_LEONARDO)
    if (Serial) Serial.println("Arduino Leonardo - 2.5KB RAM");
    return 10;
  #elif defined(ARDUINO_AVR_MICRO)
    if (Serial) Serial.println("Arduino Micro - 2.5KB RAM");
    return 10;
  #elif defined(ARDUINO_AVR_PRO) 
    if (Serial) Serial.println("Arduino Pro Mini - 2KB RAM");
    return 8;

  // Adafruit Boards
  #elif defined(ARDUINO_AVR_GEMMA)
    if (Serial) Serial.println("Adafruit Gemma - 512B RAM");
    return 3;
  #elif defined(ARDUINO_AVR_TRINKET3)
    if (Serial) Serial.println("Adafruit Trinket 3V - 512B RAM");
    return 3;
  #elif defined(ARDUINO_AVR_TRINKET5)
    if (Serial) Serial.println("Adafruit Trinket 5V - 2.5KB RAM");
    return 8;

  // Modern Arduino Boards
  #elif defined(ARDUINO_AVR_NANO_EVERY)
    if (Serial) Serial.println("Arduino Nano Every - 6KB RAM");
    return 20;
  #elif defined(ARDUINO_SAMD_NANO_33_IOT)
    if (Serial) Serial.println("Arduino Nano 33 IoT - 32KB RAM");
    return 40;

  // ESP Boards
  #elif defined(ESP8266)
    if (Serial) Serial.println("ESP8266 - 80KB RAM");
    return 50;
  #elif defined(ESP32)
    if (Serial) Serial.println("ESP32 - 520KB RAM");
    return 100;
  #elif defined(ARDUINO_ESP32S3_DEV)
    if (Serial) Serial.println("ESP32-S3 - 512KB RAM");
    return 100;

  // ARM Boards
  #elif defined(ARDUINO_SAM_DUE)
    if (Serial) Serial.println("Arduino Due - 96KB RAM");
    return 80;
  #elif defined(ARDUINO_SAMD_ZERO)
    if (Serial) Serial.println("Arduino Zero - 32KB RAM");
    return 40;
  #elif defined(ARDUINO_SAMD_MKRZERO)
    if (Serial) Serial.println("MKR Zero - 32KB RAM");
    return 40;

  // RP2040 Boards
  #elif defined(ARDUINO_RASPBERRY_PI_PICO)
    if (Serial) Serial.println("Raspberry Pi Pico - 264KB RAM");
    return 60;

  // === FALLBACK DETECTION ===
  #elif defined(__AVR__)
    // Generic AVR fallback with RAM detection
    #if defined(RAMEND)
      #if RAMEND <= 0x25F
        if (Serial) Serial.println("Small AVR (<=512B RAM)");
        return 3;
      #elif RAMEND <= 0x4FF  
        if (Serial) Serial.println("Medium AVR (1KB RAM)");
        return 6;
      #else
        if (Serial) Serial.println("Large AVR (2KB+ RAM)");
        return 8;
      #endif
    #else
      if (Serial) Serial.println("Unknown AVR Board");
      return 5;
    #endif

  #else
    if (Serial) Serial.println("Unknown Architecture");
    return 10;
  #endif
}

// Task structure - EXACTLY as you had it
template<typename F>
struct Task {
  F f;
  unsigned long runAt;
  bool enabled;
  bool run_forever;
  unsigned long fix_delay;
};

// MicrotaskQueue class - EXACTLY as you had it
template<typename F>
class CallbackScheduler {
private:
  LinkedList<Task<F>*> tasks;
  const int MAX_TASKS;

public:
  // Constructor - EXACTLY as you had it
  CallbackScheduler() : MAX_TASKS(getMaxTasks()) {
    if (Serial) {
      Serial.print("Max tasks configured: ");
      Serial.println(MAX_TASKS);
    }
  }

  // Timing check - EXACTLY as you had it
  bool shouldRun(unsigned long scheduled, unsigned long current) {
    return (current - scheduled) < 0x7FFFFFFFUL;
  }

  // Enqueue task - EXACTLY as you had it
  void enqueue(F func, unsigned long delayMs = 0, bool enable = true, bool run_forever = false) {
    unsigned long now = millis();
    int tts = tasks.size();
  
    if(tts >= MAX_TASKS) {
      if(Serial) {
        Serial.println("tasx size exceed");  // Keeping your original message
      }
      return;
    }
    Task<F>* newtask = new Task<F>;
    newtask->f = func;
    newtask->fix_delay = delayMs;
    newtask->enabled = enable;
    newtask->run_forever = run_forever;
    newtask->runAt = now + delayMs;
    tasks.add(newtask);
  }

  // Run tasks - EXACTLY as you had it
  template<typename... Args>
  void run(Args... args) {
    unsigned long now = millis();

    if (tasks.size() == 0) return;

    int i = 0;
    while (i < tasks.size()) {
      Task<F>* task = tasks.get(i);

      if (!task->enabled) {
        i++;
        continue;
      }
 
      if (shouldRun(task->runAt, now)) {
        task->f(args...);

        if (task->run_forever) {
          task->runAt = now + task->fix_delay;
          i++;
        } else {
          tasks.remove(i);
          delete task;
          task = nullptr;  // Your original logic preserved
        }
      } else {
        i++;
      }
    }
  }

  // Enable task - EXACTLY as you had it
  bool enable_task(int index) {
    if (index < 0 || index >= tasks.size()) {
      if (Serial) {
        Serial.print("❌ enable_task: Index out of range: ");
        Serial.println(index);
      }
      return false;
    }

    Task<F>* t = tasks.get(index);
    t->enabled = true;

    if (Serial) {
      Serial.print("🟢 Enabled task at index ");
      Serial.println(index);
    }
    return true;
  }

  // Disable task - EXACTLY as you had it
  bool disable_task(int index) {
    if (index < 0 || index >= tasks.size()) {
      if (Serial) {
        Serial.print("❌ disable_task: Index out of range: ");
        Serial.println(index);
      }
      return false;
    }

    Task<F>* t = tasks.get(index);
    t->enabled = false;

    if (Serial) {
      Serial.print("🔴 Disabled task at index ");
      Serial.println(index);
    }
    return true;
  }

  // Delete task - EXACTLY as you had it
  bool delete_task(int index) {
    if (index < 0 || index >= tasks.size()) {
      if (Serial) {
        Serial.print("❌ delete_task: Index out of range: ");
        Serial.println(index);
      }
      return false;
    }

    Task<F>* task = tasks.get(index);
    tasks.remove(index);
    delete task;
    task = nullptr;  // Your original logic preserved
    if (Serial) {
      Serial.print("🗑️ Deleted task at index ");
      Serial.println(index);
    }
    return true;
  }

  // Check if task is enabled - EXACTLY as you had it
  bool is_task_enable(int index) {
    if (index < 0 || index >= tasks.size()) return false;
    return tasks.get(index)->enabled;
  }

  // Destructor - EXACTLY as you had it
  ~CallbackScheduler() {
    if (Serial) {
      Serial.print("🧹 Destructor cleaning ");
      Serial.print(tasks.size());
      Serial.println(" tasks");
    }

    while (tasks.size() > 0) {
      delete tasks.get(0);
      tasks.remove(0);
    }
  }
};

 
#endif