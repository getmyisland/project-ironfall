#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include <map>
#include <filesystem>

#include <Engine/Core/Common.h>
#include <Engine/Renderer/Texture.h>

#include <glm/vec2.hpp>

namespace dyxide
{
	struct Character {
		Ref<Texture2D> Texture;
		char Char;
		glm::ivec2   Size;
		glm::ivec2   Bearing;
		unsigned int Advance;
	};

	class Font
	{
	public:
		Font(const std::string& filepath);
		~Font() = default;

		std::map<char, Character> GetCharacters() const { return m_Characters; }
		FT_Face GetFace() const { return m_Face; }

		static Ref<Font> GetDefault();

	private:
		std::map<char, Character> m_Characters;
		FT_Face m_Face;
	};

}
