#include <CallbackScheduler.h>

// Scheduler for different types of callbacks
CallbackScheduler<void(*)()> voidScheduler;
CallbackScheduler<void(*)(int)> intScheduler;
CallbackScheduler<void(*)(String)> stringScheduler;

void simpleTask() {
  static int count = 0;
  Serial.print("Simple task count: ");
  Serial.println(count++);
}

void parameterizedTask(int value) {
  Serial.print("Parameterized task received: ");
  Serial.println(value);
}

void stringTask(String message) {
  Serial.print("String task: ");
  Serial.println(message);
}

class TaskManager {
private:
  CallbackScheduler<void(*)()> classScheduler;
  int instanceData = 100;

public:
  void start() {
    // Capture 'this' in lambda to access instance data
    classScheduler.enqueue([this]() {
      Serial.print("Class method task, instance data: ");
      Serial.println(instanceData);
      instanceData += 10;
    }, 1500, true, true);
  }
  
  void run() {
    classScheduler.run();
  }
};

TaskManager taskManager;

void setup() {
  Serial.begin(115200);
  Serial.println("CallbackScheduler Advanced Patterns Demo");
  
  // 1. Simple void function
  voidScheduler.enqueue(simpleTask, 1000, true, true);
  
  // 2. Function with parameters
  intScheduler.enqueue(parameterizedTask, 2000, true, true);
  
  // 3. Lambda with capture
  int counter = 0;
  voidScheduler.enqueue([&counter]() {
    Serial.print("Lambda with capture, counter: ");
    Serial.println(counter++);
  }, 3000, true, true);
  
  // 4. String parameter function
  stringScheduler.enqueue(stringTask, 4000, true, true);
  
  // 5. Class-based tasks
  taskManager.start();
  
  Serial.println("All advanced pattern tasks scheduled!");
}

void loop() {
  // Run all schedulers
  voidScheduler.run();
  intScheduler.run(42); // Pass parameter for int tasks
  stringScheduler.run("Hello World"); // Pass parameter for string tasks
  taskManager.run();
  
  delay(50);
}