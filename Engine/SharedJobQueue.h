#pragma once

#include <Windows.h>

#include "JobSystem.h"

#include "HashedString.h"

#include <functional>
#include <queue>
#include <string>

namespace Engine
{
	namespace JobSystem
	{
		struct QueuedJob
		{
			JobData					Data;
			std::string				Name;
			std::function<void()>	Function;

			QueuedJob(const JobData& i_Data, const std::string& i_Name, std::function<void()> i_Function) :
				Data(i_Data),
				Name(i_Name),
				Function(i_Function)
			{}
		};

		class SharedJobQueue
		{
		public:
			SharedJobQueue(const std::string& i_QueueName);

			bool Add(QueuedJob* i_pJob);
			bool HasJobs() const;
			struct QueuedJob* GetWhenAvailable();
			void StartingJob(QueuedJob* i_pJob);
			void FinishedJob();

			void RequestShutdown();
			bool ShutdownRequested() const { return m_bShutdownRequested; }

			std::string GetName() const { return m_Name; }
			static JobData GetCurrentThreadActiveJobData();
		private:
			SharedJobQueue(const SharedJobQueue&) = delete;
			SharedJobQueue& operator=(const SharedJobQueue&) = delete;

			std::string m_Name;
			std::queue<struct QueuedJob*>	m_Jobs;
			CONDITION_VARIABLE m_WakeAndCheck;
			mutable CRITICAL_SECTION m_QueueAccess;

			uint32_t m_JobsRunning;
			bool m_bShutdownRequested;
		};

	} // namespace JobSystem
} // namespace Engine