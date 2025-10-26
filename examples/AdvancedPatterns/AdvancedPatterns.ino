/*
  MicrotaskScheduler - Advanced Patterns
  Shows one-time tasks, parameter passing, and complex scheduling
*/

#include <MicrotaskScheduler.h>

void oneTimeTask() {
  Serial.println("🎯 ONE-TIME TASK - This runs only once!");
}

void parameterizedTask(const char* message, int value) {
  Serial.print("📨 ");
  Serial.print(message);
  Serial.print(" | Value: ");
  Serial.println(value);
}

void chainedTask(int step) {
  Serial.print("⛓️ Chained Task - Step ");
  Serial.println(step);
  
  // Schedule next step
  if (step < 3) {
    Scheduler.enqueue([step]() { chainedTask(step + 1); }, 1000, true, false);
  }
}

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  Serial.println("🚀 MicrotaskScheduler Advanced Patterns");
  Serial.println("=======================================");
  
  // Pattern 1: One-time task
  Scheduler.enqueue(oneTimeTask, 3000, true, false);
  Serial.println("✅ Scheduled one-time task (runs in 3s)");
  
  // Pattern 2: Parameterized tasks using lambdas
  Scheduler.enqueue([]() { parameterizedTask("Hello from lambda", 42); }, 1000, true, false);
  Scheduler.enqueue([]() { parameterizedTask("Another message", 100); }, 2000, true, false);
  Serial.println("✅ Scheduled parameterized tasks");
  
  // Pattern 3: Task chaining
  Scheduler.enqueue([]() { chainedTask(1); }, 4000, true, false);
  Serial.println("✅ Scheduled chained tasks (starts in 4s)");
  
  // Pattern 4: Mixed recurring and one-time
  Scheduler.enqueue([]() { 
    static int counter = 0;
    Serial.print("🔄 Recurring task run #");
    Serial.println(counter++);
  }, 500, true, true);
  
  Serial.println("✅ Scheduled recurring counter task");
  Serial.println();
  Serial.println("📊 Watch the Serial monitor for different patterns!");
}

void loop() {
  Scheduler.run();
  delay(10);
}
