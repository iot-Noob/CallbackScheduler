/*
  MicrotaskScheduler - Memory Management Demo
  Shows how the library handles memory limits automatically
*/

#include <MicrotaskScheduler.h>

void memoryHeavyTask(int taskId) {
  Serial.print("💾 Memory Task ");
  Serial.print(taskId);
  Serial.println(" executed");
}

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  Serial.println("💾 MicrotaskScheduler Memory Demo");
  Serial.println("=================================");
  Serial.println("Testing automatic memory limits...");
  Serial.println();
  
  // Try to add more tasks than the board can handle
  int maxTasks = 20; // More than any board's limit
  
  for (int i = 0; i < maxTasks; i++) {
    bool success = Scheduler.enqueue([i]() { memoryHeavyTask(i); }, 1000, true, true);
    
    if (success) {
      Serial.print("✅ Task ");
      Serial.print(i);
      Serial.println(" scheduled");
    } else {
      Serial.print("🚫 Task ");
      Serial.print(i);
      Serial.println(" REJECTED - Memory limit reached");
      Serial.println("💡 Library automatically protects your board from memory exhaustion!");
      break;
    }
  }
  
  Serial.println();
  Serial.println("🎯 Scheduler running with optimized task count");
}

void loop() {
  Scheduler.run();
  delay(10);
}
