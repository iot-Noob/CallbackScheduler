# **CallbackScheduler**
A lightweight, memory-safe task scheduler for Arduino that automatically detects your board and configures optimal task limits. Perfect for IoT projects, robotics, and embedded systems requiring reliable task scheduling.

### **🚀 Features**
- 🤖 Automatic Board Detection - Works with 50+ Arduino boards out of the box
- 🛡️ Memory-Safe - Prevents heap fragmentation and memory exhaustion
- ⏰ Millis Rollover Protection - Handles 49-day timer wrap-around correctly
- 🔧 Template-Based - Flexible API supporting lambdas and function pointers
- 📊 Resource Management - Automatic cleanup and bounds checking
- 🎯 Cross-Platform - Works on AVR, ESP8266, ESP32, ARM, and RP2040

### **📋 Supported Boards**
| **Board Family**       | **RAM**     | **Max Tasks** | **Notes**                          |
|------------------------|-------------|---------------|------------------------------------|
| ATtiny85/45/25         | 256–512 B   | 3             | Minimal memory usage               |
| Arduino UNO / Nano     | 2 KB        | 8             | Classic AVR boards                 |
| Arduino Mega           | 8 KB        | 30            | Large AVR projects                 |
| ESP8266                | 80 KB       | 50            | IoT applications                   |
| ESP32                  | 520 KB      | 100+          | High-performance projects          |
| Arduino Zero           | 32 KB       | 40            | ARM-based boards                   |
| Raspberry Pi Pico      | 264 KB      | 60            | RP2040 platform                    |
| *—*                    | *—*         | *—*           | *Full board detection list covers 50+ variants with automatic fallbacks* |

### **🔧 Installation**
#### **Arduino IDE**
1. Download the latest release
2. In Arduino IDE: `Sketch → Include Library → Add .ZIP Library`
3. Select the downloaded ZIP file

#### **PlatformIO**
```ini
lib_deps =
    https://github.com/yourusername/CallbackScheduler.git
```
### **💡 Quick Start**
```cpp
#include <CallbackScheduler.h>

CallbackScheduler<void(*)()> scheduler;

void blinkLed() {
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

void printMessage() {
  Serial.println("Hello from scheduled task!");
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Schedule tasks
  scheduler.enqueue(blinkLed, 500, true, true);    // Blink every 500ms
  scheduler.enqueue(printMessage, 1000, true, true); // Print every second
}

void loop() {
  scheduler.run();  // Keep this in your main loop
  delay(10);
}
```
### **📚 API Reference**
#### **Basic Scheduling**
```cpp
// Schedule a one-time task
scheduler.enqueue(taskFunction, 5000);  // Runs once after 5 seconds

// Schedule a recurring task  
scheduler.enqueue(taskFunction, 1000, true, true);  // Runs every second forever

// Schedule with initial disable
scheduler.enqueue(taskFunction, 1000, false, true);  // Disabled by default
```

### **Task Management**
```cpp
// Enable/disable tasks
scheduler.enable_task(0);   // Enable task at index 0
scheduler.disable_task(1);  // Disable task at index 1

// Delete tasks
scheduler.delete_task(2);   // Remove task at index 2

// Check task status
bool enabled = scheduler.is_task_enable(0);  // Check if task is enabled
```
### **Advanced Usage**
```cpp
// Using lambdas with capture
int counter = 0;
scheduler.enqueue([&counter]() {
  Serial.print("Counter: ");
  Serial.println(counter++);
}, 1000, true, true);

// Parameterized tasks
scheduler.enqueue([]() {
  someFunction("parameter", 42);
}, 2000, true, false);

// Task chaining
void chainedTask(int step) {
  Serial.print("Step ");
  Serial.println(step);
  if (step < 5) {
    scheduler.enqueue([step]() { chainedTask(step + 1); }, 500, true, false);
  }
}
```
### **🎯 Examples**
#### **Basic LED Blinker**
```cpp
#include <CallbackScheduler.h>

CallbackScheduler<void(*)()> scheduler;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  
  scheduler.enqueue([]() {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }, 500, true, true);
}

void loop() {
  scheduler.run();
}
```
### **Interactive Serial Control**
```cpp
#include <CallbackScheduler.h>

CallbackScheduler<void(*)()> scheduler;
bool taskEnabled = true;

void setup() {
  Serial.begin(9600);
  
  scheduler.enqueue([]() { 
    Serial.println("Task running every 1 second"); 
  }, 1000, true, true);
  
  Serial.println("Press 't' to toggle task, 's' for status");
}

void loop() {
  scheduler.run();
  
  if (Serial.available()) {
    char cmd = Serial.read();
    if (cmd == 't') {
      taskEnabled = !taskEnabled;
      scheduler.enable_task(0);
      Serial.println(taskEnabled ? "Task enabled" : "Task disabled");
    }
    if (cmd == 's') {
      Serial.print("Task status: ");
      Serial.println(scheduler.is_task_enable(0) ? "ENABLED" : "DISABLED");
    }
  }
}
```
#### **Memory Management Demo**
```cpp
#include <CallbackScheduler.h>

CallbackScheduler<void(*)()> scheduler;

void setup() {
  Serial.begin(9600);
  
  // The library automatically prevents memory exhaustion
  for (int i = 0; i < 50; i++) {
    bool success = scheduler.enqueue([i]() {
      Serial.print("Task ");
      Serial.println(i);
    }, 1000, true, true);
    
    if (!success) {
      Serial.print("Safe limit reached at ");
      Serial.print(i);
      Serial.println(" tasks - memory protected!");
      break;
    }
  }
}

void loop() {
  scheduler.run();
}
```
#### **🔍 How It Works**
##### **Automatic Board Detection**
The library automatically detects your hardware at compile-time and configures optimal settings:
```cpp
// Behind the scenes - intelligent configuration
#if defined(ARDUINO_AVR_UNO)
  const int MAX_TASKS = 8;      // 2KB RAM protection
#elif defined(ESP8266)
  const int MAX_TASKS = 50;     // 80KB RAM optimization
#elif defined(ESP32)
  const int MAX_TASKS = 100;    // 520KB RAM utilization
#endif
```
### **Memory Safety Features**
- Compile-time limits based on board capabilities
- Heap fragmentation protection through task limits
- Automatic cleanup of completed tasks
- Bounds checking on all operations
### Millis Rollover Protection
```cpp
// Safe time comparison handles 49-day wrap-around
bool shouldRun(unsigned long scheduled, unsigned long current) {
  return (current - scheduled) < 0x7FFFFFFFUL;
}
```
### **⚠️ Best Practices**
### **Do's**
```cpp
// ✅ Keep Scheduler.run() in your main loop
void loop() {
  Scheduler.run();
  delay(10);  // Small delay prevents watchdog issues
}

// ✅ Use lambdas for simple tasks
Scheduler.enqueue([]() {
  // Simple task logic here
}, 1000, true, true);

// ✅ Check return values for critical tasks
if (Scheduler.enqueue(criticalTask, 1000, true, true)) {
  Serial.println("Critical task scheduled successfully");
}
```
### **Don'ts**
```cpp
// ❌ Don't use long delays in tasks
void badTask() {
  delay(5000);  // Blocks entire scheduler!
}

// ❌ Don't forget Scheduler.run()
void loop() {
  // Missing Scheduler.run() - tasks won't execute!
}

// ❌ Don't overload small boards
// Let the library handle limits automatically
```
### **🐛 Troubleshooting**
#### **Common Issues**
##### **Tasks not running?**
- Ensure `Scheduler.run()` is in your `loop()`
- Check task is enabled with `Scheduler.isTaskEnabled(index)`
- Verify Serial is initialized before task creation
#### **Memory warnings?**
- The library automatically enforces limits
- Reduce task complexity or use one-time tasks
- Monitor Serial output for limit notifications

#### **Compilation errors?**
- Ensure you have the required LinkedList library
- Check Arduino IDE board selection matches your hardware
## **📊 Technical Details**
#### **Memory Usage**
| **Component**         | **Memory Usage** |
|------------------------|------------------|
| Task structure         | ~18 bytes        |
| LinkedList overhead    | ~8 bytes         |
| Heap allocation        | ~8 bytes         |
| **Total per task**     | **~34 bytes**    |

#### **Performance Characteristics**
- O(n) execution time for task checking
- O(1) scheduling for new tasks
- Deterministic memory usage with fixed limits
- Zero memory fragmentation with proper usage
## **🔧 Advanced Configuration**
### **Custom Task Limits**
For specialized requirements, you can modify the `getMaxTasks() `function in the library source to set custom limits for your specific use case.
### **Performance Optimization**
- Keep task execution time under 1ms
- Use one-time tasks instead of recurring where possible
- Monitor Serial output for performance warnings
### **🤝 Contributing**
We welcome contributions! Please feel free to submit pull requests, report bugs, or suggest new features.
- Fork the repository
- Create a feature branch: git checkout -b new-feature
- Commit changes: git commit -am 'Add new feature'
- Push to branch: git push origin new-feature
- Submit a pull request
## **📄 License**
MIT License - feel free to use in personal and commercial projects.

## **🎉 Acknowledgments**
- Built on the reliable LinkedList library
- Inspired by Arduino's built-in timer functions
- Community tested across multiple hardware platforms

---
##### **Happy Scheduling! 🚀**
