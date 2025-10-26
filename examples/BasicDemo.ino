#include <CallbackScheduler.h>

// Create a scheduler instance
CallbackScheduler<void(*)()> scheduler;

void task1() {
  Serial.println("Task 1 executed");
}

void task2() {
  Serial.println("Task 2 executed");
}

void setup() {
  Serial.begin(115200);
  Serial.println("CallbackScheduler Basic Demo Started");
  
  // Schedule task1 to run once after 1000ms
  scheduler.enqueue(task1, 1000, true, false);
  
  // Schedule task2 to run forever every 2000ms
  scheduler.enqueue(task2, 2000, true, true);
  
  Serial.println("Tasks scheduled:");
  Serial.println("- Task 1: One-time after 1 second");
  Serial.println("- Task 2: Repeating every 2 seconds");
}

void loop() {
  scheduler.run();
  delay(10); // Small delay to prevent watchdog issues
}