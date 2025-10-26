#include <CallbackScheduler.h>

CallbackScheduler<void(*)()> scheduler;

void memoryTask() {
  Serial.print("Free memory: ");
  Serial.println(freeMemory());
}

void setup() {
  Serial.begin(115200);
  Serial.println("CallbackScheduler Memory Demo Started");
  
  // Schedule memory monitoring task to run every 3 seconds
  scheduler.enqueue(memoryTask, 3000, true, true);
  
  // Add multiple tasks to demonstrate memory usage
  for(int i = 0; i < 5; i++) {
    scheduler.enqueue([]() {
      Serial.println("Dummy task executed");
    }, 5000 + (i * 1000), true, false);
  }
}

void loop() {
  scheduler.run();
  delay(50);
}

// Helper function to check free memory (AVR specific)
#ifdef __AVR__
extern "C" char* __brkval;
int freeMemory() {
  char top;
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
}
#else
int freeMemory() {
  return 0; // Placeholder for non-AVR boards
}
#endif