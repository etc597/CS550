#pragma once

#include <mutex>
#include <deque>

#include "Core/Threading/Job.hpp"

//TODO (Evan): Replace JobQueue with circular array
// less locks and more performant

class JobQueue
{
public:
  ~JobQueue();
  void Push(Job* aJob);
  Job* Pop();
  Job* Steal();
  void Flush();
private:
  std::mutex mQueueLock;
  std::deque<Job*> mQueue;
};

