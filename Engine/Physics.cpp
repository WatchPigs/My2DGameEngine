#include "Physics.h"
#include "Pointers.hpp"
#include "GameObjectFactory.h"
#include "StartupShutdown.h"
#include "Mutex.h"
#include "json.hpp"
#include "ScopeLock.h"
#include "Vector2.h"

namespace Engine
{
	namespace Physics
	{
		bool bShutdown = false;
		Mutex NewMoveablesMutex;

		std::vector<MoveablePtr> AllMoveables;
		std::vector<Moveable> NewMoveables;

		Moveable::Moveable(SmartPtr<GameObject>& i_Object, float i_Mass, float i_Kd) :
			m_Object(i_Object),
			m_Forces(Vector2::Zero),
			m_Acceleration(Vector2::Zero),
			m_Drag(Vector2::Zero),
			m_Mass(i_Mass),
			m_Kd(i_Kd)
		{}

		WeakMoveablePtr GetMoveableForGameObject(const SmartPtr<GameObject>& i_GameObject)
		{
			CheckForNewMoveables();

			for (auto& m : AllMoveables)
			{
				if (m->m_Object == i_GameObject)
				{
					return WeakMoveablePtr(m);
				}
			}

			return WeakPtr<Moveable>();
		}

		void AddMoveable(SmartPtr<GameObject>& i_GameObject, nlohmann::json& i_JSON)
		{
			if (!bShutdown)
			{
				assert(i_JSON["mass"].is_number_float());
				assert(i_JSON["kd"].is_number_float());

				float Mass = i_JSON["mass"];
				float Kd = i_JSON["kd"];

				{
					ScopeLock Lock(NewMoveablesMutex);
					NewMoveables.push_back({ i_GameObject, Mass, Kd });
				}
			}
		}

		void ApplyForceToMoveable(MoveablePtr& i_Moveable, const Engine::Vector2 i_Forces)
		{
			i_Moveable->m_Forces = i_Forces;
		}

		void CheckForNewMoveables()
		{
			ScopeLock Lock(NewMoveablesMutex);
			for (auto& m : NewMoveables)
			{
				AllMoveables.push_back(MoveablePtr(new Moveable(m)));
			}
			NewMoveables.clear();
		}

		void UpdateAllMoveables(float i_dt)
		{
			size_t count = AllMoveables.size();

			for (size_t i = 0; i < count;)
			{
				SmartPtr<GameObject> Object = AllMoveables[i]->m_Object.AcquireOwnership();
				if (Object)
				{
					const Vector2 CurrentVelocity = Object->GetVelocity();

					const Vector2 Drag = CurrentVelocity * -(AllMoveables[i]->m_Kd);
					const Vector2 TotalForce = AllMoveables[i]->m_Forces + Drag;

					const Vector2 Acceleration = TotalForce / AllMoveables[i]->m_Mass;

					const Vector2 NextVelocity = CurrentVelocity + (Acceleration * i_dt);
					const Vector2 AverageVelocity = (CurrentVelocity + NextVelocity) / 2.0f;

					AllMoveables[i]->m_Drag = Drag;
					AllMoveables[i]->m_Acceleration = Acceleration;

					Object->SetPosition(Object->GetPosition() + AverageVelocity * i_dt);
					Object->SetVelocity(NextVelocity);

					++i;
				}
				else
				{
					if (i < (count - 1))
						AllMoveables[i] = std::move(AllMoveables.back());

					AllMoveables.pop_back();
					--count;
				}
			}
		}

		void Tick(float i_ms)
		{
			CheckForNewMoveables();
			// todo: actually do something here.
			UpdateAllMoveables(i_ms);
		}

		void Init()
		{
			using namespace std::placeholders;

			RegisterComponentCreator("moveable", std::bind(AddMoveable, _1, _2));
		}

		void Shutdown()
		{
			bShutdown = true;

			DeregisterComponentCreator("moveable");

			AllMoveables.clear();
			AllMoveables.shrink_to_fit();

			{
				ScopeLock Lock(NewMoveablesMutex);
				NewMoveables.clear();
				NewMoveables.shrink_to_fit();
			}

		}
		Bootstrapper PhysicsBootstrapper(std::bind(Init), std::bind(Shutdown));
	}
}