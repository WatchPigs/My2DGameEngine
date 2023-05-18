#include "World.h"
#include "StartupShutdown.h"
#include "Mutex.h"
#include "ScopeLock.h"

#include <functional>

namespace Engine
{
	std::vector<SmartPtr<GameObject>> WorldGameObjects;
	Mutex WorldGameObjectsLock;

	void AddGameObjectToWorld(SmartPtr<GameObject>& i_NewGameObject)
	{
		ScopeLock Lock(WorldGameObjectsLock);
		WorldGameObjects.push_back(i_NewGameObject);
	}

	bool RemoveGameObjectFromWorld(WeakPtr<GameObject>& i_GameObject)
	{
		SmartPtr<GameObject> GameObject = i_GameObject.AcquireOwnership();

		ScopeLock Lock(WorldGameObjectsLock);

		for (auto& iter : WorldGameObjects)
		{
			if (iter == i_GameObject)
			{
				if( iter != WorldGameObjects.back())
					std::swap(iter, WorldGameObjects.back());

				WorldGameObjects.pop_back();
				return true;
			}
		}

		return false;
	}

	namespace World
	{
		void Shutdown()
		{
			WorldGameObjects.clear();
			WorldGameObjects.shrink_to_fit();
		}
		Bootstrapper WorldBootstrapper(std::function<void()>(), std::bind(Shutdown));
	}
}