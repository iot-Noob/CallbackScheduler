#ifndef SCHEDULER_IMPL_H
#define SCHEDULER_IMPL_H

#include "scheduler.h"

// Template implementations
template<typename F>
MicrotaskQueue<F>::MicrotaskQueue() : MAX_TASKS(getMaxTasks()) {
  if (Serial) {
    Serial.print("Max tasks configured: ");
    Serial.println(MAX_TASKS);
  }
}

template<typename F>
bool MicrotaskQueue<F>::shouldRun(unsigned long scheduled, unsigned long current) {
  return (current - scheduled) < 0x7FFFFFFFUL;
}

template<typename F>
void MicrotaskQueue<F>::enqueue(F func, unsigned long delayMs, bool enable, bool run_forever) {
  unsigned long now = millis();
  int tts = tasks.size();
  
  if(tts >= MAX_TASKS) {
    if(Serial) {
      Serial.println("Task size exceeded");  // Fixed typo
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

template<typename F>
template<typename... Args>
void MicrotaskQueue<F>::run(Args... args) {
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
        // No i++ - next task moved to current position
      }
    } else {
      i++;
    }
  }
}

// Other method implementations...
template<typename F>
bool MicrotaskQueue<F>::enable_task(int index) {
  if (index < 0 || index >= tasks.size()) {
    if (Serial) Serial.print("❌ enable_task: Index out of range: "); Serial.println(index);
    return false;
  }
  tasks.get(index)->enabled = true;
  if (Serial) { Serial.print("🟢 Enabled task at index "); Serial.println(index); }
  return true;
}

template<typename F>
bool MicrotaskQueue<F>::disable_task(int index) {
  if (index < 0 || index >= tasks.size()) {
    if (Serial) Serial.print("❌ disable_task: Index out of range: "); Serial.println(index);
    return false;
  }
  tasks.get(index)->enabled = false;
  if (Serial) { Serial.print("🔴 Disabled task at index "); Serial.println(index); }
  return true;
}

template<typename F>
bool MicrotaskQueue<F>::delete_task(int index) {
  if (index < 0 || index >= tasks.size()) {
    if (Serial) Serial.print("❌ delete_task: Index out of range: "); Serial.println(index);
    return false;
  }
  Task<F>* task = tasks.get(index);
  tasks.remove(index);
  delete task;
  if (Serial) { Serial.print("🗑️ Deleted task at index "); Serial.println(index); }
  return true;
}

template<typename F>
bool MicrotaskQueue<F>::is_task_enable(int index) {
  if (index < 0 || index >= tasks.size()) return false;
  return tasks.get(index)->enabled;
}

template<typename F>
MicrotaskQueue<F>::~MicrotaskQueue() {
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

#endif