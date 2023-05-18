#include "Input.h"

namespace Engine
{
	std::vector<std::function<void(unsigned int, bool)>> KeyChangeCallbacks;

	void RegisterOnKeyChange(std::function<void(unsigned int, bool)> i_OnKeyChange)
	{
		KeyChangeCallbacks.push_back(i_OnKeyChange);
	}

	void Input::Init()
	{
		GLib::SetKeyStateChangeCallback([](unsigned int i_VKeyID, bool i_bDown)
			{
				for (auto k : KeyChangeCallbacks)
				{
					if (k)
						k(i_VKeyID, i_bDown);
				}
			}
		);
	}

	namespace Input
	{
		void Shutdown()
		{
			KeyChangeCallbacks.clear();
			KeyChangeCallbacks.shrink_to_fit();
		}

		Bootstrapper InputBootstrapper(std::bind(Init), std::bind(Shutdown));
	}
}