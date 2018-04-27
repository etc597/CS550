#pragma once
#include <atomic>
#include <memory>
#include <functional>

#include "Core/Threading/JobHandle.hpp"

class Job
{
public:
  Job(std::function<void*(JobHandle&)>&& aDelegate);
  Job(std::function<void*(JobHandle&)>&& aDelegate, JobHandle& aParentHandle);
  ~Job();

  JobHandle GetParentHandle();
  bool HasCompleted() const;
  float Progress() const;
  bool IsDeletable() const;

  void* GetReturn();
  void Invoke();

  void IncrementJobs();
  void DecrementJobs();

  void Abandon();
  std::weak_ptr<bool> GetAbandonedHandle();

protected:
  Job* mParentJob;
  std::function<void*(JobHandle&)> mDelegate;
  void* mReturn;
  std::atomic<int> mTotalJobs;
  std::atomic<int> mUnfinishedJobs;
  std::shared_ptr<bool> mAbandoned;
};
