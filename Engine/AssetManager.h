#pragma once

#include "Pointers.hpp"
#include "GLib.h"
#include <map>
#include <string>

namespace Engine
{
	class ReleaseSprite
	{
	public:
		static void release(GLib::Sprite* i_ptr) { GLib::Release(i_ptr); }
	};

	typedef SmartPtr<GLib::Sprite, ReleaseSprite> SpritePtr;

	class AssetManager
	{
	public:
		static AssetManager& get_instance();
		void AddSprite(std::string name, const char* i_pFilename);
		SpritePtr FindSprite(std::string name);
		bool DeleteSprite(std::string name);

	private:
		AssetManager() = default;
		~AssetManager() = default;
		AssetManager(const AssetManager&) = delete;
		AssetManager(AssetManager&&) = delete;
		AssetManager& operator = (const AssetManager&) = delete;
		AssetManager& operator = (AssetManager&&) = delete;

		std::map<std::string, SpritePtr> m_sprites;
	};

	SpritePtr CreateSprite(const char* i_pFilename);

	void* LoadFile(const char* i_pFilename, size_t& o_sizeFile);
}
