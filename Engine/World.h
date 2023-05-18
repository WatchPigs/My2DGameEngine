#pragma once

#include "GameObject.h"
#include "Pointers.hpp"

#include <vector>

namespace Engine
{
	//std::vector<SmartPtr<GameObject>> WorldGameObjects;
	//Mutex WorldGameObjectsLock;

	void AddGameObjectToWorld(SmartPtr<GameObject>& i_NewGameObject);
	bool RemoveGameObjectFromWorld(WeakPtr<GameObject>& i_GameObject);

	namespace World
	{
		void Shutdown();
	}

} // namespace Engine