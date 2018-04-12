#pragma once
#include <memory>

class JobHandle
{
  friend class Job;
public:
  JobHandle(Job* aJob);
  bool HasParentHandle() const;
  JobHandle GetParentHandle();
  bool HasCompleted() const;
  float Progress() const;
  bool WasAbandoned() const;
  bool IsEmpty() const;
  void* GetReturn();

private:
  Job * GetJob();

  Job* mJob;
  std::shared_ptr<bool> mAbandoned;
};
