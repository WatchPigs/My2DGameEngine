#include "ConsolePrint.h"
#include "GameObjectFactory.h"
#include "World.h"
#include "StartupShutdown.h"
#include "Input.h"
#include "JobStatus.h"
#include "Physics.h"
#include "Renderer.h"
#include "Collision.h"

#include <Windows.h>

#include <assert.h>
#include "Utilities.h"
#include <string>

int WINAPI wWinMain(_In_ HINSTANCE i_hInstance, _In_opt_ HINSTANCE i_hPrevInstance, _In_ LPWSTR i_lpCmdLine, _In_ int i_nCmdShow)
{
	Engine::Startup();

	enum KeyCode { W, A, S, D, I, J, K, L, Q };
	bool keyStates[8] = { 0 };
	bool bQuit = false;

	Engine::RegisterOnKeyChange([&keyStates, &bQuit](unsigned int i_VKeyID, bool i_bIsDown)
		{
			using namespace Engine;
			switch (i_VKeyID)
			{
			case 87:
				keyStates[W] = i_bIsDown;
				break;
			case 65:
				keyStates[A] = i_bIsDown;
				break;
			case 83:
				keyStates[S] = i_bIsDown;
				break;
			case 68:
				keyStates[D] = i_bIsDown;
				break;
			case 73:
				keyStates[I] = i_bIsDown;
				break;
			case 74:
				keyStates[J] = i_bIsDown;
				break;
			case 75:
				keyStates[K] = i_bIsDown;
				break;
			case 76:
				keyStates[L] = i_bIsDown;
				break;
			case 81:
				bQuit = i_bIsDown;
			}
		}
	);

	if (Engine::OpenWindow(i_hInstance, i_nCmdShow, "My2DGame", -1, 1024, 768))
	{
		using namespace Engine;

		SmartPtr<GameObject> GameObject1;
		SmartPtr<GameObject> GameObject2;

		JobSystem::JobStatus *Status = new JobSystem::JobStatus;

		CreateGameObjectAsync("data\\GoodGuy.json", [&GameObject1](SmartPtr<GameObject>& i_TheGameObject)
			{
				i_TheGameObject->SetPosition(Engine::Vector2(-200, -82));
				GameObject1 = i_TheGameObject;
				DEBUG_PRINT("GameObjectFinished");

			},
			Status);

		CreateGameObjectAsync("data\\BadGuy.json", [&GameObject2](SmartPtr<GameObject>& i_TheGameObject)
			{
				i_TheGameObject->SetPosition(Engine::Vector2(200, -118));
				GameObject2 = i_TheGameObject;
				DEBUG_PRINT("GameObjectFinished");

			},
			Status);

		Status->WaitForZeroJobsLeft();
		DEBUG_PRINT("Finished Waiting");
		delete Status;

		Engine::Run([&GameObject1, &GameObject2, &keyStates, &bQuit]() -> bool
			{
				Physics::WeakMoveablePtr GameObject1MoveablePtr = Physics::GetMoveableForGameObject(GameObject1);
				Physics::MoveablePtr GameObject1Moveable = GameObject1MoveablePtr.AcquireOwnership();

				Physics::WeakMoveablePtr GameObject2MoveablePtr = Physics::GetMoveableForGameObject(GameObject2);
				Physics::MoveablePtr GameObject2Moveable = GameObject2MoveablePtr.AcquireOwnership();

				Vector2 horizentalForce = Vector2(100.0f, 0.0f);
				Vector2 verticalForce = Vector2(0.0f, 100.0f);
				
				Physics::ApplyForceToMoveable(GameObject1Moveable, Vector2::Zero);

				if (keyStates[W]) Physics::ApplyForceToMoveable(GameObject1Moveable, verticalForce);
				if (keyStates[A]) Physics::ApplyForceToMoveable(GameObject1Moveable, -horizentalForce);
				if (keyStates[S]) Physics::ApplyForceToMoveable(GameObject1Moveable, -verticalForce);
				if (keyStates[D]) Physics::ApplyForceToMoveable(GameObject1Moveable, horizentalForce);

				Physics::ApplyForceToMoveable(GameObject2Moveable, Vector2::Zero);

				Vector2 gameObject2ToGameObject1 = (GameObject1->GetPosition() - GameObject2->GetPosition()).Normalized();
				Vector2 traceForce = 30.0f * gameObject2ToGameObject1;

				Physics::ApplyForceToMoveable(GameObject2Moveable, traceForce);

				Collision::SetCollisionCollidaable(GameObject1, [&GameObject2, &bQuit](WeakPtr<GameObject>& otherObject)
					{
						if (otherObject == GameObject2) bQuit = true;
						DEBUG_PRINT("Collide");
					}
				);

				if (bQuit)
				{
					Engine::Shutdown();
					return false;
				}
				if (!GameObject2) assert(false);

				return true;
			}
		);
	}

	return 0;
}

void GameEnd()
{

}