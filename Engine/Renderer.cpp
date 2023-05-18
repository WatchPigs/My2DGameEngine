#include "Renderer.h"
#include "Pointers.hpp"
#include "GLib.h"
#include "GameObjectFactory.h"
#include "Mutex.h"
#include "ScopeLock.h"
#include "Threading.h"
#include "JobSystem.h"
#include "ProcessFileJob.h"
#include "ConsolePrint.h"
#include "StartupShutdown.h"

#include <vector>
#include <DirectXColors.h>

namespace Engine
{
	namespace Renderer
	{
		bool bShutdown = false;
		Mutex NewRenderablesMutex;

		std::vector<RenderablePtr> AllRenderables;
		std::vector<RenderablePtr> NewRenderables;

		DirectX::XMVECTORF32 ClearColor = DirectX::Colors::Black;

		Renderable::Renderable(const SmartPtr<GameObject>& i_Object, SpritePtr& i_Sprite) :
			m_Object(i_Object),


			m_Sprite(i_Sprite)
		{}

		void Renderable::SetSprite(SpritePtr& i_NewSprite)
		{
			m_Sprite = i_NewSprite;
		}

		void SetClearColor(DirectX::XMVECTORF32 i_ClearColor)
		{
			ClearColor = i_ClearColor;
		}

		RenderablePtr AddRenderable(const SmartPtr<GameObject>& i_GameObject, SpritePtr i_pSprite)
		{
			RenderablePtr NewRenderable(new Renderable(i_GameObject, i_pSprite));

			if (InMainThread())
			{
				AllRenderables.push_back(NewRenderable);
			}
			else
			{
				ScopeLock Lock(NewRenderablesMutex);
				NewRenderables.push_back(NewRenderable);
			}

			return NewRenderable;
		}

		void AddRenderable(const SmartPtr<GameObject>& i_GameObject, std::vector<uint8_t>& i_SpriteTextureData)
		{
			if (bShutdown || !i_GameObject)
				return;

			assert(!i_SpriteTextureData.empty());

			GLib::Texture* pTexture = GLib::CreateTexture(&i_SpriteTextureData[0], i_SpriteTextureData.size());

			if (pTexture == nullptr)
			{
				DEBUG_PRINT("Failed to create texture.");
			}
			else
			{
				unsigned int width = 0;
				unsigned int height = 0;
				unsigned int depth = 0;

				// Get the dimensions of the texture. We'll use this to determine how big it is on screen
				bool result = GLib::GetDimensions(*pTexture, width, height, depth);
				assert(result == true);
				assert((width > 0) && (height > 0));

				// Define the sprite edges
				GLib::SpriteEdges	Edges = { -float(width / 2.0f), float(height), float(width / 2.0f), 0.0f };
				GLib::SpriteUVs	UVs = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f } };
				GLib::RGBA					Color = { 255, 255, 255, 255 };

				// Create the sprite
				GLib::Sprite* pSprite = GLib::CreateSprite(Edges, 0.1f, Color, UVs, pTexture);
				if (pSprite)
				{
					AddRenderable(i_GameObject, SpritePtr(pSprite));
				}

				GLib::Release(pTexture);
			}

			DEBUG_PRINT("Finished Renderable");
		}

		void AddRenderableAsync(SmartPtr<GameObject>& i_GameObject, const std::string& i_SpriteFilename)
		{
			ProcessFileAsync(i_SpriteFilename, [i_GameObject, i_SpriteFilename](std::vector<uint8_t> i_FileContents)
				{
					if (i_FileContents.empty())
						DEBUG_PRINT("Failed to load sprite file texture %s. No renderable will be created.", i_SpriteFilename.c_str());
					else
						AddRenderable(i_GameObject, i_FileContents);
				},
				JobSystem::GetCurrentThreadActiveJobData()
					);
		}

		void CreateRenderable(SmartPtr<GameObject>& i_GameObject, nlohmann::json& i_JSON)
		{
			assert(i_JSON["sprite_texture"].is_string());

			std::string SpriteFilename = i_JSON["sprite_texture"];

			AddRenderableAsync(i_GameObject, SpriteFilename);
		}

		void CheckForNewRenderables()
		{
			ScopeLock Lock(NewRenderablesMutex);
			for (auto& m : NewRenderables)
			{
				AllRenderables.push_back(m);
			}
			NewRenderables.clear();
		}

		void Render(float i_dt)
		{
			GLib::BeginRendering(ClearColor);
			GLib::Sprites::BeginRendering();

			size_t count = AllRenderables.size();

			for (size_t i = 0; i < count; )
			{
				SmartPtr<GameObject> Object = AllRenderables[i]->m_Object.AcquireOwnership();

				if (Object)
				{
					if (AllRenderables[i]->m_Sprite)
					{
						Vector2 ObjectPosition = Object->GetPosition();

						GLib::Render(*(AllRenderables[i]->m_Sprite), { ObjectPosition.x(), ObjectPosition.y() });
					}
					++i;
				}
				else
				{
					if (i < (count - 1))
						AllRenderables[i] = std::move(AllRenderables.back());

					AllRenderables.pop_back();
					--count;
				}
			}

			assert(count == AllRenderables.size());

			GLib::Sprites::EndRendering();
			GLib::EndRendering();
		}

		void Tick(float i_dt)
		{
			CheckForNewRenderables();
			Render(i_dt);
		}

		void Init()
		{
			using namespace std::placeholders;

			RegisterComponentCreator("renderable", std::bind(CreateRenderable, _1, _2));
		}

		void Shutdown()
		{
			bShutdown = true;

			DeregisterComponentCreator("renderable");

			{
				ScopeLock Lock(NewRenderablesMutex);

				NewRenderables.clear();
				NewRenderables.shrink_to_fit();
			}

			AllRenderables.clear();
			AllRenderables.shrink_to_fit();
		}

		Bootstrapper RendererBootstrapper(std::bind(Init), std::bind(Shutdown));
	}
}
