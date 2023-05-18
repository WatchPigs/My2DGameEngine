#pragma once

#include "GameObject.h"
#include "AssetManager.h"
#include "json.hpp"

#include <vector>
#include <DirectXColors.h>
#include <string>
#include <functional>

namespace Engine
{
	namespace Renderer
	{
		//DirectX::XMVECTORF32 ClearColor = DirectX::Colors::Black;

		//bool bShutdown = false;

		class Renderable
		{
		public:
			WeakPtr<GameObject> m_Object;
			SpritePtr m_Sprite;

			Renderable(const SmartPtr<GameObject>& i_Object, SpritePtr& i_Sprite);
			void SetSprite(SpritePtr& i_NewSprite);
		};

		typedef SmartPtr<Renderable> RenderablePtr;
		typedef WeakPtr<Renderable> WeakRenderablePtr;

		//std::vector<RenderablePtr> AllRenderables;
		//std::vector<RenderablePtr> NewRenderables;

		//Mutex NewRenderablesMutex;

		void SetClearColor(DirectX::XMVECTORF32 i_ClearColor);
		RenderablePtr AddRenderable(const SmartPtr<GameObject>& i_GameObject, SpritePtr i_pSprite);
		void AddRenderable(const SmartPtr<GameObject>& i_GameObject, std::vector<uint8_t>& i_SpriteTextureData);
		void AddRenderableAsync(SmartPtr<GameObject>& i_GameObject, const std::string& i_SpriteFilename);
		void CreateRenderable(SmartPtr<GameObject>& i_GameObject, nlohmann::json& i_JSON);
		void CheckForNewRenderables();
		void Render(float i_dt);
		void Tick(float i_dt);

		void Init();
		void Shutdown();

		//Bootstrapper RendererBootstrapper(std::bind(Init), std::bind(Shutdown));
	}
}

