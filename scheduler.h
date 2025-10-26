#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <LinkedList.h>
#include <Arduino.h>

// Forward declarations
int getMaxTasks();

// Task structure
template<typename F>
struct Task {
  F f;
  unsigned long runAt;
  bool enabled;
  bool run_forever;
  unsigned long fix_delay;
};

// MicrotaskQueue class declaration
template<typename F>
class MicrotaskQueue {
private:
  LinkedList<Task<F>*> tasks;
  const int MAX_TASKS;

public:
  MicrotaskQueue();
  bool shouldRun(unsigned long scheduled, unsigned long current);
  void enqueue(F func, unsigned long delayMs = 0, bool enable = true, bool run_forever = false);
  
  template<typename... Args>
  void run(Args... args);

  bool enable_task(int index);
  bool disable_task(int index);
  bool delete_task(int index);
  bool is_task_enable(int index);
  ~MicrotaskQueue();
};

// 🔥 TEMPLATE IMPLEMENTATIONS MUST BE IN HEADER!
#include "scheduler_impl.h"

#endif