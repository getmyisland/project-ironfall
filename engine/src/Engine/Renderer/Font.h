#pragma once

#include <Engine/Core/Common.h>
#include <Engine/Renderer/Texture.h>

#include <string>
#include <unordered_map>

namespace dyxide
{
	struct MSDFData;

	class Font
	{
	public:
		Font(const std::string& path);
		~Font();

		const MSDFData* GetMSDFData() const { return m_Data; }
		Ref<Texture2D> GetAtlasTexture() const { return m_AtlasTexture; }

		static Ref<Font> Create(const std::string& path);
		static Ref<Font> GetDefault();

	private:
		MSDFData* m_Data;
		Ref<Texture2D> m_AtlasTexture;
	};

}
