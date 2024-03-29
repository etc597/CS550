#include "JobHandle.hpp"
#include "Job.hpp"

JobHandle::JobHandle(Job * aJob)
  : mJob(aJob)
  , mAbandoned(aJob ? aJob->GetAbandonedHandle().lock() : std::make_shared<bool>(false))
{
}

bool JobHandle::HasParentHandle() const
{
  return !mJob->GetParentHandle().IsEmpty();
}

JobHandle JobHandle::GetParentHandle()
{
  return *mAbandoned ? nullptr : mJob->GetParentHandle();
}

bool JobHandle::HasCompleted() const
{
  return *mAbandoned ? false : mJob->HasCompleted();
}

float JobHandle::Progress() const
{
  return *mAbandoned ? false : mJob->Progress();
}

bool JobHandle::WasAbandoned() const
{
  return *mAbandoned;
}

bool JobHandle::IsEmpty() const
{
  return !mJob;
}

void* JobHandle::GetReturn()
{
  return *mAbandoned ? nullptr : mJob->GetReturn();
}

Job * JobHandle::GetJob()
{
  return mJob;
}


