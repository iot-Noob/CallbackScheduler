/*
  MicrotaskScheduler - Basic Demo
  Shows basic task scheduling with LED blinking and Serial output
*/

#include <MicrotaskScheduler.h>

void blinkLed() {
  static bool ledState = false;
  digitalWrite(LED_BUILTIN, ledState);
  ledState = !ledState;
  Serial.println(ledState ? "🔵 LED ON" : "⚫ LED OFF");
}

void printHello() {
  Serial.println("👋 Hello from scheduled task!");
}

void printCounter() {
  static int counter = 0;
  Serial.print("🔢 Counter: ");
  Serial.println(counter++);
}

void setup() {
  Serial.begin(9600);
  while (!Serial); // Wait for Serial
  
  pinMode(LED_BUILTIN, OUTPUT);
  
  Serial.println("🚀 MicrotaskScheduler Basic Demo");
  Serial.println("=================================");
  
  // Schedule tasks
  Scheduler.enqueue(blinkLed, 500, true, true);     // Blink LED every 500ms
  Scheduler.enqueue(printHello, 2000, true, true);  // Print every 2 seconds
  Scheduler.enqueue(printCounter, 1000, true, true); // Counter every 1 second
  
  Serial.println("✅ Tasks scheduled:");
  Serial.println("   - LED blink every 500ms");
  Serial.println("   - Hello message every 2s");
  Serial.println("   - Counter every 1s");
  Serial.println("🎯 Starting scheduler...");
}

void loop() {
  Scheduler.run(); // Run the scheduler
  delay(10); // Small delay to prevent watchdog issues
}
