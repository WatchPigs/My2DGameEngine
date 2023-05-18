#pragma once

#include "Pointers.hpp"
#include "GameObject.h"
#include "Vector3.h"

#include <functional>

namespace Engine
{
	namespace Collision
	{
		typedef std::function<void(WeakPtr<GameObject>&)> CollisionCallback_t;

		void CreateCollidable(SmartPtr<GameObject>& i_GameObject, const Vector2& i_Center, const Vector2& i_Extents, const CollisionCallback_t& Callback);
		void SetCollisionCollidaable(SmartPtr<GameObject>& i_GameObject, const CollisionCallback_t& i_Callback);

		void Tick(float i_dt);
		bool FoundCollisionLastTick();
	} // namespace Collision
} // namespace Engine