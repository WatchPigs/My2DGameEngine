#include "StartupShutdown.h"
#include "Physics.h"
#include "Renderer.h"
#include "Collision.h"

namespace Engine
{
	static std::vector<std::function<void()>>& OnStartup()
	{
		if (pOnStartup == nullptr)
		{
			pOnStartup = new std::vector<std::function<void()>>();
		}

		return *pOnStartup;
	}

	static std::vector<std::function<void()>>& OnShutdown()
	{
		if (pOnShutdown == nullptr)
		{
			pOnShutdown = new std::vector<std::function<void()>>();
		}

		return *pOnShutdown;
	}

	Bootstrapper::Bootstrapper(std::function<void()> i_OnStartup, std::function<void()> i_OnShutdown)
	{
		if (i_OnStartup)
			OnStartup().push_back(i_OnStartup);

		if (i_OnShutdown)
			OnShutdown().push_back(i_OnShutdown);
	}

	float GetFrameTime()
	{
		static Timing::tick_t lastFrameStartTick = 0;

		float FrameTime;

		Timing::tick_t currentFrameStartTick = Timing::GetCurrentTickCounter();

		FrameTime = (lastFrameStartTick == 0) ? (1.0f / 60.0f) : (Timing::GetTimeDiff_ms(lastFrameStartTick, currentFrameStartTick) / 1000.0f);

		lastFrameStartTick = currentFrameStartTick;

		//return IsDebuggerPresent() ? (1.0f / 60.0f) : FrameTime;
		return FrameTime;
	}

	void Startup()
	{
		if (pOnStartup)
		{
			for (auto iter : *pOnStartup)
			{
				iter();
			}

			delete pOnStartup;
		}
	}

	bool OpenWindow(HINSTANCE i_hInstance, int i_CmdShow, const char* i_pWindowName, WORD i_IconID, unsigned int i_WindowWidth, unsigned int i_WindowHeight)
	{

		bGLibInitialized = GLib::Initialize(i_hInstance, i_CmdShow, i_pWindowName, i_IconID, i_WindowWidth, i_WindowHeight, true);
		return bGLibInitialized;
	}

	void Run(std::function<bool()> i_Update)
	{
		bool bDone = false;

		do
		{
			GLib::Service(bDone);
			if (!bDone)
			{
				float dt = GetFrameTime();
				Physics::Tick(dt);
				Collision::Tick(dt);

				if (i_Update());
				else break;

				//Renderer::SetClearColor(Collision::FoundCollisionLastTick() ? DirectX::Colors::Red : DirectX::Colors::Blue);
				Renderer::SetClearColor(DirectX::Colors::Blue);

				Renderer::Tick(dt);
			}
		} while (bDone == false);
	}

	void Shutdown()
	{
		if (pOnShutdown)
		{
			for (auto iter : *pOnShutdown)
			{
				iter();
			}

			delete pOnShutdown;
		}

		if (bGLibInitialized)
			GLib::Shutdown();
	}

}