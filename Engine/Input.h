#pragma once

#include "GLib.h"
#include "StartupShutdown.h"

#include <functional>
#include <vector>

namespace Engine
{
	//std::vector<std::function<void(unsigned int, bool)>> KeyChangeCallbacks;

	void RegisterOnKeyChange(std::function<void(unsigned int, bool)> i_OnKeyChange);

	namespace Input
	{
		void Init();

		void Shutdown();

		//Bootstrapper InputBootstrapper(std::bind(Init), std::bind(Shutdown));
	}
}