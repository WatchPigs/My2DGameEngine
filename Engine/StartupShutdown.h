#pragma once

#include <Windows.h>
#include <functional>
#include <vector>

#include "Timing.h"
#include "GLib.h"

namespace Engine
{
	static bool bGLibInitialized = false;

	static std::vector<std::function<void()>>* pOnStartup;
	static std::vector<std::function<void()>>* pOnShutdown;

	static std::vector<std::function<void()>>& OnStartup();
	static std::vector<std::function<void()>>& OnShutdown();

	class Bootstrapper
	{
	public:
		Bootstrapper(std::function<void()> i_OnStartup, std::function<void()> i_OnShutdown);
	};

	float GetFrameTime();

	void Startup();
	bool OpenWindow(HINSTANCE i_hInstance, int i_CmdShow, const char* i_pWindowName, WORD i_IconID, unsigned int i_WindowWidth, unsigned int i_WindowHeight);
	void Run(std::function<bool()> i_Update);
	void Shutdown();
}

