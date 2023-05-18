#pragma once
#include "Vector2.h"
#include "Pointers.hpp"

namespace Engine
{
	class GameObject
	{
	public:
		static SmartPtr<GameObject> Create(const Vector2& i_InitialPosition, const Vector2& i_InitialVelocity = Vector2::Zero);
		Vector2 GetPosition() const;
		void SetPosition(const Vector2& i_Position);
		Vector2 GetVelocity() const;
		void SetVelocity(const Vector2& i_Velocity);
	private:
		GameObject(const Vector2& i_InitialPosition, const Vector2& i_InitialVelocity = Vector2::Zero);
		Vector2 m_Position;
		Vector2 m_Velocity;
	};
}

