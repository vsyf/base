/*
 * task_runner_base.h
 * Copyright (C) 2022 youfa.song <vsyfar@gmail.com>
 *
 * Distributed under terms of the GPLv2 license.
 */

#ifndef TASK_RUNNER_BASE_H
#define TASK_RUNNER_BASE_H

#include <memory>

#include "task.h"

namespace ave {
namespace base {
class TaskRunnerBase {
 public:
  virtual void Destruct() = 0;

  // post a task to be run, return true if the task maybe run at some point int
  // the future, and false the task will not be run.
  virtual void PostTask(std::unique_ptr<Task> task) = 0;

  virtual void PostDelayedTask(std::unique_ptr<Task> task,
                               uint64_t time_us) = 0;

  virtual void PostDelayedTaskAndWait(std::unique_ptr<Task> task,
                                      uint64_t time_us,
                                      bool wait) = 0;

  // virtual bool postTaskAndReplay(const Task& task, const Task& reply);

  static TaskRunnerBase* Current();
  bool IsCurrent() const { return Current() == this; }

 protected:
  class CurrentTaskRunnerSetter {
   public:
    explicit CurrentTaskRunnerSetter(TaskRunnerBase* task_runner);
    CurrentTaskRunnerSetter(const CurrentTaskRunnerSetter&) = delete;
    CurrentTaskRunnerSetter& operator=(const CurrentTaskRunnerSetter&) = delete;
    ~CurrentTaskRunnerSetter();

   private:
    TaskRunnerBase* const previous_;
  };

  virtual ~TaskRunnerBase() = default;
};

struct TaskRunnerDeleter {
  void operator()(TaskRunnerBase* task_runner) const {
    task_runner->Destruct();
  }
};

}  // namespace base
}  // namespace ave

#endif /* !TASK_RUNNER_BASE_H */
