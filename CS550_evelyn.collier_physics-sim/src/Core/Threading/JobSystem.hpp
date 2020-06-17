#pragma once


#include <mutex>
#include <thread>
#include <unordered_map>

#include "Core/Threading/Worker.hpp"

class JobSystem
{
public:

  JobSystem();
  ~JobSystem();
  void Initialize();
  void WaitThisThread(JobHandle& aJobHandle);
  void Update();

  // TODO(Evan): Finish FunctionDelegate to allow lambda support

  JobHandle QueueJobThisThread(std::function<void*(JobHandle&)>&& aJob)
  {
    Job* newJob = new Job(std::move(aJob));
    QueueJobInternal(newJob);
    return JobHandle(newJob);
  }

  JobHandle QueueJobThisThread(std::function<void*(JobHandle&)>& aJob, JobHandle& aParentHandle)
  {
    Job* newJob = new Job(std::move(aJob), aParentHandle);
    QueueJobInternal(newJob);
    return JobHandle(newJob);
  }
private:
  void QueueJobInternal(Job* aJob);

  Worker::WorkerID mForegroundWorker;
  std::unordered_map<Worker::WorkerID, Worker*> mPool;
  bool mAsync;
};

