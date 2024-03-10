#include <dxpch.h>
#include <glad/glad.h>

#include <Engine/Core/Assert.h>
#include <Engine/Core/Log.h>
#include <Engine/Renderer/Font.h>

#include <filesystem>
#include <string>

namespace dyxide
{
	Font::Font(const std::string& filepath)
	{
		FT_Library ft;
		DYXIDE_ASSERT(FT_Init_FreeType(&ft), "Could not init FreeType Library");

		FT_Face face;
		DYXIDE_ASSERT(FT_New_Face(ft, (GetResourceDir() + filepath).c_str(), 0, &face), "Failed to load font");

		FT_Set_Pixel_Sizes(face, 0, 48);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

		for (unsigned char c = 0; c < 128; c++)
		{
			// Load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				DYXIDE_ERROR("Failed to load Glyph");
				continue;
			}

			TextureSpecification spec;
			spec.Width = face->glyph->bitmap.width;
			spec.Height = face->glyph->bitmap.rows;
			spec.Format = ImageFormat::RGB8;
			spec.GenerateMips = false;

			// Store character for later use
			Character character = {
				Texture2D::Create(spec),
				static_cast<char>(c),
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				face->glyph->advance.x
			};

			m_Characters.insert(std::pair<char, Character>(c, character));
		}
	}

	Ref<Font> Font::GetDefault()
	{
		return CreateRef<Font>("fonts/opensans/OpenSans-Regular.ttf");
	}
}
