#pragma once

#include "GameObject.h"
#include "json.hpp"

#include <functional>
#include <vector>

namespace Engine
{
	namespace Physics
	{
		class Moveable
		{
		public:
			WeakPtr<GameObject> m_Object;
			Vector2 m_Forces;
			Vector2 m_Acceleration; // meters/second
			Vector2 m_Drag; //
			float m_Mass; // kg
			float m_Kd;

			Moveable(SmartPtr<GameObject>& i_Object, float i_Mass, float i_Kd);
		};

		typedef SmartPtr<Moveable> MoveablePtr;
		typedef WeakPtr<Moveable> WeakMoveablePtr;

		WeakMoveablePtr GetMoveableForGameObject(const SmartPtr<GameObject>& i_GameObject);
		void AddMoveable(SmartPtr<GameObject>& i_GameObject, nlohmann::json& i_JSON);
		void ApplyForceToMoveable(MoveablePtr& i_Moveable, const Engine::Vector2 i_Forces);
		void CheckForNewMoveables();
		void UpdateAllMoveables(float i_dt);
		void Tick(float i_ms);

		void Init();
		void Shutdown();
	}
}

