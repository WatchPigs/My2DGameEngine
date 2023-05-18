#include "GameObject.h"

namespace Engine
{
	SmartPtr<GameObject> GameObject::Create(const Vector2& i_InitialPosition, const Vector2& i_InitialVelocity)
	{
		return SmartPtr<GameObject>(new GameObject(i_InitialPosition, i_InitialVelocity));
	}

	GameObject::GameObject(const Vector2& i_InitialPosition, const Vector2& i_InitialVelocity) :
		m_Position(i_InitialPosition),
		m_Velocity(i_InitialVelocity)
	{}

	Vector2 GameObject::GetPosition() const
	{
		return m_Position;
	}

	void GameObject::SetPosition(const Vector2& i_Position)
	{
		m_Position = i_Position;
	}

	Vector2 GameObject::GetVelocity() const
	{
		return m_Velocity;
	}

	void GameObject::SetVelocity(const Vector2& i_Velocity)
	{
		m_Velocity = i_Velocity;
	}
}
