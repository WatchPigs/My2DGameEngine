#pragma once

#include "Pointers.hpp"
#include "GameObject.h"
#include "json.hpp"
#include "Vector2.h"
#include "JobStatus.h"

#include <string>


namespace Engine
{
	static std::map<std::string, std::function<void(SmartPtr<GameObject>&, nlohmann::json&)>> ControllerCreators;
	static std::map<std::string, std::function<void(SmartPtr<GameObject>&, nlohmann::json&)> >  ComponentCreators;

	static Vector2 Vec2(const nlohmann::json& i_JSON);

	SmartPtr<GameObject> CreateGameObject(const std::string& i_JSONFilename);
	SmartPtr<GameObject> CreateGameObjectFromJSON(nlohmann::json& i_JSONData);
	static void CreateGameObjectFromJSONData(std::vector<uint8_t> i_JSONData, bool i_bIsCBOR, std::function<void(SmartPtr<GameObject>&)> i_OnCreated);
	void CreateGameObjectAsync(const std::string& i_JSONFilename, std::function<void(SmartPtr<GameObject>&)> i_OnCreated, JobSystem::JobStatus* i_pStatus);


	void RegisterComponentCreator(const std::string& i_ComponentName, std::function<void(SmartPtr<GameObject>&, nlohmann::json&)> i_ComponentCreator);
	void DeregisterComponentCreator(const std::string& i_ComponentName);

	void RegisterControllerCreator(const std::string& i_ControllerName, std::function<void(SmartPtr<GameObject>&, nlohmann::json&)> i_ControllerCreator);
	void DeregisterControllerCreator(const std::string& i_ControllerName);

	namespace GameObjectFactory
	{
		static void Shutdown();
	}
}

