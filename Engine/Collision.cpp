#include "Collision.h"

#include "GameObject.h"
#include "GameObjectFactory.h"
#include "StartupShutdown.h"
#include "Matrix3.h"
#include "Matrix4.h"
#include "Vector3.h"
#include "Mutex.h"
#include "ScopeLock.h"

#include "json.hpp"

namespace Engine
{
	namespace Collision
	{
		struct AABB
		{
			Vector2 m_Center;
			Vector2 m_Extents;
		};

		struct CollisionCheckData
		{
			SmartPtr<GameObject>	m_GameObject;
			Matrix3					m_ObjectToWorld;
			Vector2					m_BBCenterInWorld;
		};

		struct Collideable
		{
			WeakPtr<GameObject>		m_GameObject;
			AABB					m_BoundingBox;
			CollisionCallback_t		m_CollisionCallback;

			CollisionCheckData		m_CachedCheckData;
		};

		struct CollisionPair
		{
			Collideable*			m_pCollideables[2];
			float					m_CollisionTime;
		};

		std::vector<Collideable> NewCollideables;
		Mutex NewCollideablesMutex;

		std::vector<Collideable> AllCollideables;

		bool bShutdown = false;
		bool bFoundCollisionThisTick = false;

		bool CheckForCollisions(float i_dt);
		void CheckForNewCollideables();

		static Vector2 Vec2(const nlohmann::json& i_JSON)
		{
			assert(i_JSON.is_array() && (i_JSON.size() == 2));

			return Vector2(i_JSON[0], i_JSON[1]);
		}

		void CreateCollidableFromJSON(SmartPtr<GameObject>& i_GameObject, nlohmann::json& i_JSON)
		{
			AABB NewAABB;

			NewAABB.m_Center = Vec2(i_JSON["offset"]);
			NewAABB.m_Extents = Vec2(i_JSON["extents"]);

			{
				ScopeLock Lock(NewCollideablesMutex);
				NewCollideables.push_back({ i_GameObject, NewAABB });
			}
		}

		void CreateCollidable(SmartPtr<GameObject>& i_GameObject, const Vector2& i_Center, const Vector2& i_Extents, const CollisionCallback_t& i_CollisionCallback)
		{
			AABB NewAABB;

			NewAABB.m_Center = i_Center;
			NewAABB.m_Extents = i_Extents;

			{
				ScopeLock Lock(NewCollideablesMutex);
				NewCollideables.push_back({ i_GameObject, NewAABB, i_CollisionCallback });
			}
		}

		void SetCollisionCollidaable(SmartPtr<GameObject>& i_GameObject, const CollisionCallback_t& i_Callback)
		{
			CheckForNewCollideables();

			for (auto& c : AllCollideables)
			{
				if (c.m_GameObject == i_GameObject)
				{
					c.m_CollisionCallback = i_Callback;
					break;
				}
			}
		}

		void Init()
		{
			using namespace std::placeholders;

			RegisterComponentCreator("collideable", std::bind(CreateCollidableFromJSON, _1, _2));
		}

		void Shutdown()
		{
			bShutdown = true;

			DeregisterComponentCreator("collideable");

			{
				ScopeLock Lock(NewCollideablesMutex);

				NewCollideables.clear();
				NewCollideables.shrink_to_fit();
			}

			NewCollideables.clear();
			NewCollideables.shrink_to_fit();
		}

		Bootstrapper RendererBootstrapper(std::bind(Init), std::bind(Shutdown));

		void CheckForNewCollideables()
		{
			ScopeLock Lock(NewCollideablesMutex);
			for (auto& c : NewCollideables)
			{
				AllCollideables.push_back(c);
			}

			NewCollideables.clear();
		}

		void CacheCollisionCheckData()
		{
			size_t count = AllCollideables.size();

			for (size_t i = 0; i < count; )
			{
				Collideable& c = AllCollideables[i];

				c.m_CachedCheckData.m_GameObject = c.m_GameObject.AcquireOwnership();
				if (c.m_CachedCheckData.m_GameObject)
				{
					c.m_CachedCheckData.m_ObjectToWorld = Matrix3::CreateTranslationCV(c.m_CachedCheckData.m_GameObject->GetPosition());
					c.m_CachedCheckData.m_BBCenterInWorld = c.m_CachedCheckData.m_ObjectToWorld.TransformPointCV(c.m_BoundingBox.m_Center);
					++i;
				}
				else
				{
					if (i < (count - 1))
						AllCollideables[i] = std::move(AllCollideables.back());

					AllCollideables.pop_back();
					--count;
				}
			}

			assert(count == AllCollideables.size());
		}

		void ReleaseCachedCollisionCheckData()
		{
			for (auto& c : AllCollideables)
			{
				c.m_CachedCheckData.m_GameObject = nullptr;
			}
		}

		void Tick(float i_dt)
		{
			CheckForNewCollideables();

			bFoundCollisionThisTick = CheckForCollisions(i_dt);
		}

		bool FoundCollisionLastTick()
		{
			return bFoundCollisionThisTick;
		}

		bool DetectCrossTimes(float i_Center, float i_Extent, float i_Point, float i_Travel, float& o_tEnter, float& o_tExit)
		{
			assert(i_Extent > 0.0f);

			float i_Start = i_Center - i_Extent;
			float i_End = i_Center + i_Extent;

			if (IsZero(i_Travel))
			{
				if ((i_Point < i_Start) || (i_Point > i_End))
					return false;
				else
				{
					o_tEnter = 0.0f;
					o_tExit = 1.0f;
					return true;
				}
			}
			o_tEnter = (i_Start - i_Point) / i_Travel;
			o_tExit = (i_End - i_Point) / i_Travel;

			if (o_tEnter > o_tExit)
				Swap(o_tEnter, o_tExit);

			return !((o_tEnter >= 1.0f) || (o_tExit <= 0.0f));
		}

		bool CheckCollision2D(const Collideable& i_Collideable1, const Collideable& i_Collideable2, float i_dt, float& o_tCollision)
		{
			const SmartPtr<GameObject>& i_GameObject1 = i_Collideable1.m_CachedCheckData.m_GameObject;
			const SmartPtr<GameObject>& i_GameObject2 = i_Collideable2.m_CachedCheckData.m_GameObject;

			const CollisionCheckData& i_CachedData1 = i_Collideable1.m_CachedCheckData;
			const CollisionCheckData& i_CachedData2 = i_Collideable2.m_CachedCheckData;

			float tEnter = 0.0f;
			float tExit = 1.0f;

			Vector2 Obj2RelVel = i_GameObject2->GetVelocity() - i_GameObject1->GetVelocity();
			Vector2 Obj2Travel = Obj2RelVel * i_dt;

			// Obj1 X
			{
				float Obj1BBCenterOnAxis = i_CachedData1.m_BBCenterInWorld.x();
				float Obj2BBCenterOnAxis = i_CachedData2.m_BBCenterInWorld.x();

				float Obj2ProjectedExtents = fabs(i_Collideable2.m_BoundingBox.m_Extents.x());

				float Obj1ExpandedExtents = i_Collideable1.m_BoundingBox.m_Extents.x() + Obj2ProjectedExtents;

				float Obj2TravelAlongAxis = Obj2Travel.x();

				float axisEnter = 0.0f;
				float axisExit = 1.0f;

				if (!DetectCrossTimes(Obj1BBCenterOnAxis, Obj1ExpandedExtents, Obj2BBCenterOnAxis, Obj2TravelAlongAxis, axisEnter, axisExit))
					return false;

				if (axisEnter > tEnter)
					tEnter = axisEnter;
				if (axisExit < tExit)
					tExit = axisExit;
			}
			// Obj1 Y
			{
				float Obj1BBCenterOnAxis = i_CachedData1.m_BBCenterInWorld.y();
				float Obj2BBCenterOnAxis = i_CachedData2.m_BBCenterInWorld.y();

				float Obj2ProjectedExtents = fabs(i_Collideable2.m_BoundingBox.m_Extents.y());

				float Obj1ExpandedExtents = i_Collideable1.m_BoundingBox.m_Extents.y() + Obj2ProjectedExtents;

				float Obj2TravelAlongAxis = Obj2Travel.y();

				float axisEnter = 0.0f;
				float axisExit = 1.0f;

				if (!DetectCrossTimes(Obj1BBCenterOnAxis, Obj1ExpandedExtents, Obj2BBCenterOnAxis, Obj2TravelAlongAxis, axisEnter, axisExit))
					return false;

				if (axisEnter > tEnter)
					tEnter = axisEnter;
				if (axisExit < tExit)
					tExit = axisExit;
			}

			Vector2 Obj1RelVel = i_GameObject1->GetVelocity() - i_GameObject2->GetVelocity();
			Vector2 Obj1Travel = Obj1RelVel * i_dt;

			// Obj2 X
			{
				float Obj2BBCenterOnAxis = i_CachedData2.m_BBCenterInWorld.x();
				float Obj1BBCenterOnAxis = i_CachedData1.m_BBCenterInWorld.x();

				float Obj1ProjectedExtents = fabs(i_Collideable1.m_BoundingBox.m_Extents.x());

				float Obj2ExpandedExtents = i_Collideable2.m_BoundingBox.m_Extents.x() + Obj1ProjectedExtents;

				float Obj1TravelAlongAxis = Obj1Travel.x();

				float axisEnter = 0.0f;
				float axisExit = 1.0f;

				if (!DetectCrossTimes(Obj2BBCenterOnAxis, Obj2ExpandedExtents, Obj1BBCenterOnAxis, Obj1TravelAlongAxis, axisEnter, axisExit))
					return false;

				if (axisEnter > tEnter)
					tEnter = axisEnter;
				if (axisExit < tExit)
					tExit = axisExit;
			}

			// Obj2 Y
			{
				float Obj2BBCenterOnAxis = i_CachedData2.m_BBCenterInWorld.y();
				float Obj1BBCenterOnAxis = i_CachedData1.m_BBCenterInWorld.y();

				float Obj1ProjectedExtents = fabs(i_Collideable1.m_BoundingBox.m_Extents.y());

				float Obj2ExpandedExtents = i_Collideable2.m_BoundingBox.m_Extents.y() + Obj1ProjectedExtents;

				float Obj1TravelAlongAxis = Obj1Travel.y();

				float axisEnter = 0.0f;
				float axisExit = 1.0f;

				if (!DetectCrossTimes(Obj2BBCenterOnAxis, Obj2ExpandedExtents, Obj1BBCenterOnAxis, Obj1TravelAlongAxis, axisEnter, axisExit))
					return false;

				if (axisEnter > tEnter)
					tEnter = axisEnter;
				if (axisExit < tExit)
					tExit = axisExit;
			}

			o_tCollision = tEnter;
			return tEnter < tExit;
		}

		bool FindCollision(float i_dt, CollisionPair& o_CollisionPair)
		{
			bool bFoundCollision = false;
			const size_t count = AllCollideables.size();

			if (count > 1)
			{
				for (size_t i = 0; i < (count - 1); i++)
				{
					for (size_t j = i + 1; j < count; j++)
					{
						float tCollision = 0.0f;

						if (CheckCollision2D(AllCollideables[i], AllCollideables[j], i_dt, tCollision))
						{
							if (!bFoundCollision || (bFoundCollision && (tCollision < o_CollisionPair.m_CollisionTime)))
							{
								o_CollisionPair.m_pCollideables[0] = &AllCollideables[i];
								o_CollisionPair.m_pCollideables[1] = &AllCollideables[j];
								o_CollisionPair.m_CollisionTime = tCollision;

								bFoundCollision = true;
							}
						}
					}
				}
			}

			return bFoundCollision;
		}

		bool CheckForCollisions(float i_dt)
		{
			bool bFoundCollision = false;

			CacheCollisionCheckData();

			CollisionPair FoundCollision;

			if (FindCollision(i_dt, FoundCollision))
			{
				assert(FoundCollision.m_pCollideables[0]);
				assert(FoundCollision.m_pCollideables[1]);

				if (FoundCollision.m_pCollideables[0]->m_CollisionCallback)
					FoundCollision.m_pCollideables[0]->m_CollisionCallback(FoundCollision.m_pCollideables[1]->m_GameObject);

				if (FoundCollision.m_pCollideables[1]->m_CollisionCallback)
					FoundCollision.m_pCollideables[1]->m_CollisionCallback(FoundCollision.m_pCollideables[0]->m_GameObject);

				bFoundCollision = true;
			}

			ReleaseCachedCollisionCheckData();

			return bFoundCollision;
		}
	}
}