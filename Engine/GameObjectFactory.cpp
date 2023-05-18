#include "GameObjectFactory.h"
#include "File.h"
#include "StartupShutdown.h"
#include "ProcessFileJob.h"
#include "World.h"

#include <functional>

namespace Engine
{
	static Vector2 Vec2(const nlohmann::json& i_JSON)
	{
		assert(i_JSON.is_array() && i_JSON.size() == 2);

		return Vector2(i_JSON[0], i_JSON[1]);
	}

	SmartPtr<GameObject> CreateGameObject(const std::string& i_JSONFilename)
	{
		SmartPtr<GameObject> NewObject;

		std::vector<uint8_t> Contents = LoadFile(i_JSONFilename);

		if (!Contents.empty())
		{
			nlohmann::json JSON = nlohmann::json::parse(Contents);
			NewObject = CreateGameObjectFromJSON(JSON);
		}

		return NewObject;
	}

	SmartPtr<GameObject> CreateGameObjectFromJSON(nlohmann::json& i_JSONData)
	{
		using json = nlohmann::json;

		const Vector2 InitialPosition(Vec2(i_JSONData["initial_position"]));

		SmartPtr<GameObject> NewGameObject = GameObject::Create(InitialPosition);

		if (i_JSONData.contains("controller"))
		{
			assert(i_JSONData["controller"].is_object());
			assert(i_JSONData["controller"]["type"].is_string());
			assert(i_JSONData["controller"]["initializer"].is_object());


			auto ControllerCreator = ControllerCreators.find(i_JSONData["controller"]["type"]);
			if (ControllerCreator != ControllerCreators.end())
			{
				ControllerCreator->second(NewGameObject, i_JSONData["controller"]["initializer"]);
			}
		}

		if (i_JSONData.contains("components"))
		{
			assert(i_JSONData["components"].is_object());

			for (json::iterator it = i_JSONData["components"].begin(); it != i_JSONData["components"].end(); ++it)
			{
				const std::string& ComponentName = it.key();

				auto ComponentCreator = ComponentCreators.find(ComponentName);
				if (ComponentCreator != ComponentCreators.end())
				{
					ComponentCreator->second(NewGameObject, it.value());
				}
			}

		}

		AddGameObjectToWorld(NewGameObject);

		return NewGameObject;
	}

	static void CreateGameObjectFromJSONData(std::vector<uint8_t> i_JSONData, bool i_bIsCBOR, std::function<void(SmartPtr<GameObject>&)> i_OnCreated)
	{

		if (!i_JSONData.empty())
		{
			using json = nlohmann::json;

			json JSON = i_bIsCBOR ? json::from_cbor(i_JSONData) : json::parse(i_JSONData);

			SmartPtr<GameObject> NewObject = CreateGameObjectFromJSON(JSON);

			if (i_OnCreated)
				i_OnCreated(NewObject);
		}
	}

	void CreateGameObjectAsync(const std::string& i_JSONFilename, std::function<void(SmartPtr<GameObject>&)> i_OnCreated, JobSystem::JobStatus* i_pStatus)
	{
		using namespace std::placeholders;
		ProcessFileAsync(i_JSONFilename, std::bind(CreateGameObjectFromJSONData, _1, false, i_OnCreated), { JobSystem::GetDefaultQueue(), i_pStatus });
	}

	void RegisterComponentCreator(const std::string& i_ComponentName, std::function<void(SmartPtr<GameObject>&, nlohmann::json&)> i_ComponentCreator)
	{
		ComponentCreators.insert({ i_ComponentName, i_ComponentCreator });
	}

	void DeregisterComponentCreator(const std::string& i_ComponentName)
	{
		auto creator = ComponentCreators.find(i_ComponentName);
		if (creator != ComponentCreators.end())
			ComponentCreators.erase(creator);
	}

	void RegisterControllerCreator(const std::string& i_ControllerName, std::function<void(SmartPtr<GameObject>&, nlohmann::json&)> i_ControllerCreator)
	{
		ControllerCreators.insert({ i_ControllerName, i_ControllerCreator });
	}

	void DeregisterControllerCreator(const std::string& i_ControllerName)
	{
		auto creator = ControllerCreators.find(i_ControllerName);
		if (creator != ControllerCreators.end())
			ControllerCreators.erase(creator);
	}

	//void GameObjectFactory::Shutdown()
	//{
	//	ComponentCreators.clear();
	//	ControllerCreators.clear();
	//}

	namespace GameObjectFactory
	{
		void Shutdown()
		{
			ComponentCreators.clear();
			ControllerCreators.clear();
		}
		Bootstrapper GameObjectFactoryBootstrapper(std::function<void()>(), std::bind(Shutdown));
	}
}