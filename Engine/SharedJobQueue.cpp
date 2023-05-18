#include "SharedJobQueue.h"

#include <assert.h>

#include "Console\ConsolePrint.h"
#include "AtomicOperations.h"

namespace Engine
{
	namespace JobSystem
	{
		SharedJobQueue::SharedJobQueue(const std::string& i_Name) :
			m_Name(i_Name),
			m_WakeAndCheck(CONDITION_VARIABLE_INIT),
			m_JobsRunning(0),
			m_bShutdownRequested(false)
		{
			InitializeCriticalSection(&m_QueueAccess);
		}

		bool SharedJobQueue::Add(struct QueuedJob* i_pJob)
		{
			assert(i_pJob);
			bool bAdded = false;

			EnterCriticalSection(&m_QueueAccess);
			if (m_bShutdownRequested == false)
			{
				if (i_pJob->Data.pJobStatus)
					i_pJob->Data.pJobStatus->AddJob();

				m_Jobs.push(i_pJob);
				bAdded = true;
			}
			LeaveCriticalSection(&m_QueueAccess);

			if (bAdded)
				WakeConditionVariable(&m_WakeAndCheck);

			return bAdded;
		}

		bool SharedJobQueue::HasJobs() const
		{
			EnterCriticalSection(&m_QueueAccess);
			bool bFinished = m_Jobs.empty() && (m_JobsRunning == 0);
			LeaveCriticalSection(&m_QueueAccess);

			return !bFinished;
		}

		__declspec(thread) QueuedJob* pJobRunningOnThisThread = nullptr;

		JobData SharedJobQueue::GetCurrentThreadActiveJobData()
		{
			return pJobRunningOnThisThread ? pJobRunningOnThisThread->Data : JobData();
		}

		void SharedJobQueue::StartingJob(QueuedJob* i_pJob)
		{
			pJobRunningOnThisThread = i_pJob;
			AtomicIncrement(m_JobsRunning);

		}
		void SharedJobQueue::FinishedJob()
		{
			assert(pJobRunningOnThisThread);

			if (pJobRunningOnThisThread->Data.pJobStatus)
				pJobRunningOnThisThread->Data.pJobStatus->FinishJob();

			pJobRunningOnThisThread = nullptr;

			AtomicDecrement(m_JobsRunning);
		}

		struct QueuedJob* SharedJobQueue::GetWhenAvailable()
		{
			EnterCriticalSection(&m_QueueAccess);

			if (m_Jobs.empty() && (m_bShutdownRequested == false))
			{
				BOOL result = SleepConditionVariableCS(&m_WakeAndCheck, &m_QueueAccess, INFINITE);
				assert(result != 0);

				if (m_bShutdownRequested == true)
				{
					LeaveCriticalSection(&m_QueueAccess);
					return nullptr;
				}
			}

			struct QueuedJob* pJob = nullptr;

			if (!m_Jobs.empty())
			{
				pJob = m_Jobs.front();
				m_Jobs.pop();
			}

			LeaveCriticalSection(&m_QueueAccess);

			return pJob;
		}

		void SharedJobQueue::RequestShutdown()
		{
			m_bShutdownRequested = true;
			WakeAllConditionVariable(&m_WakeAndCheck);
		}

	} // namespace JobSystem
} // namespace Engine
