#include <CallbackScheduler.h>

CallbackScheduler<void(*)()> scheduler;
int taskCount = 0;

void createTask(int id) {
  scheduler.enqueue([id]() {
    Serial.print("Task ");
    Serial.print(id);
    Serial.print(" executed at: ");
    Serial.println(millis());
  }, 1000, true, true); // Run every second forever
}

void listTasks() {
  Serial.println("\n=== Current Tasks ===");
  for(int i = 0; i < 5; i++) { // Assuming max 5 tasks for demo
    if(scheduler.is_task_enable(i)) {
      Serial.print("Task ");
      Serial.print(i);
      Serial.println(": 🟢 ENABLED");
    }
  }
  Serial.println("=====================");
}

void setup() {
  Serial.begin(115200);
  Serial.println("CallbackScheduler Interactive Control Demo");
  Serial.println("Commands:");
  Serial.println("  'c' - Create new task");
  Serial.println("  'l' - List all tasks");
  Serial.println("  'd0' - Disable task 0");
  Serial.println("  'e0' - Enable task 0");
  Serial.println("  'x0' - Delete task 0");
  Serial.println("  's' - Show task status");
}

void loop() {
  scheduler.run();
  
  // Handle serial commands
  if(Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    
    if(command == "c") {
      createTask(taskCount++);
      Serial.println("New task created!");
    }
    else if(command == "l") {
      listTasks();
    }
    else if(command == "s") {
      Serial.print("Total tasks created: ");
      Serial.println(taskCount);
    }
    else if(command.startsWith("d")) {
      int taskId = command.substring(1).toInt();
      if(scheduler.disable_task(taskId)) {
        Serial.print("Disabled task ");
        Serial.println(taskId);
      }
    }
    else if(command.startsWith("e")) {
      int taskId = command.substring(1).toInt();
      if(scheduler.enable_task(taskId)) {
        Serial.print("Enabled task ");
        Serial.println(taskId);
      }
    }
    else if(command.startsWith("x")) {
      int taskId = command.substring(1).toInt();
      if(scheduler.delete_task(taskId)) {
        Serial.print("Deleted task ");
        Serial.println(taskId);
      }
    }
  }
  
  delay(50);
}