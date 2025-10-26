/*
  MicrotaskScheduler - Interactive Control
  Control tasks dynamically via Serial commands
*/

#include <MicrotaskScheduler.h>

bool task2Enabled = true;
bool task3Enabled = true;

void task1() {
  Serial.println("⏰ Task 1 - Running every 1s");
}

void task2() {
  Serial.println("🎯 Task 2 - Running every 2s");
}

void task3() {
  Serial.println("⭐ Task 3 - Running every 3s");
}

void printHelp() {
  Serial.println();
  Serial.println("🎮 Interactive Controls:");
  Serial.println("1 - Enable/Disable Task 2");
  Serial.println("2 - Enable/Disable Task 3"); 
  Serial.println("d - Delete Task 3");
  Serial.println("s - Show task status");
  Serial.println("h - Show this help");
  Serial.println();
}

void showStatus() {
  Serial.println();
  Serial.println("📊 Task Status:");
  Serial.println("Task 1: 🟢 ALWAYS RUNNING");
  Serial.print("Task 2: ");
  Serial.println(task2Enabled ? "🟢 ENABLED" : "🔴 DISABLED");
  Serial.print("Task 3: ");
  Serial.println(task3Enabled ? "🟢 ENABLED" : "🔴 DISABLED");
  Serial.println();
}

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  Serial.println("🎮 MicrotaskScheduler Interactive Demo");
  Serial.println("======================================");
  
  // Schedule tasks
  Scheduler.enqueue(task1, 1000, true, true);  // Always running
  Scheduler.enqueue(task2, 2000, true, true);  // Controllable
  Scheduler.enqueue(task3, 3000, true, true);  // Controllable
  
  printHelp();
  showStatus();
}

void loop() {
  Scheduler.run();
  
  // Handle Serial commands
  if (Serial.available()) {
    char command = Serial.read();
    
    switch(command) {
      case '1':
        task2Enabled = !task2Enabled;
        Scheduler.enableTask(1); // Enable/disable task at index 1
        Serial.println(task2Enabled ? "🟢 Task 2 ENABLED" : "🔴 Task 2 DISABLED");
        break;
        
      case '2':
        task3Enabled = !task3Enabled;
        Scheduler.enableTask(2); // Enable/disable task at index 2
        Serial.println(task3Enabled ? "🟢 Task 3 ENABLED" : "🔴 Task 3 DISABLED");
        break;
        
      case 'd':
        Scheduler.deleteTask(2);
        Serial.println("🗑️ Task 3 DELETED");
        task3Enabled = false;
        break;
        
      case 's':
        showStatus();
        break;
        
      case 'h':
        printHelp();
        break;
        
      default:
        Serial.println("❓ Unknown command. Press 'h' for help.");
    }
  }
  
  delay(10);
}
