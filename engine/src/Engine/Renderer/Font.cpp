#include <dxpch.h>
#include <glad/glad.h>

#include <Engine/Core/Assert.h>
#include <Engine/Core/Log.h>
#include <Engine/Core/ResourceLoader.h>
#include <Engine/Renderer/Font.h>

#include <filesystem>
#include <string>

namespace dyxide
{
	Font::Font(const std::string& path)
	{
		FT_Library ft;
		FT_Error errorInitLib = FT_Init_FreeType(&ft);
		DYXIDE_ASSERT(!static_cast<bool>(errorInitLib), "Could not init FreeType Library with error code " << static_cast<int>(errorInitLib));

		FT_Face face;
		FT_Error errorNewFace = FT_New_Face(ft, path.c_str(), 0, &face);
		DYXIDE_ASSERT(!static_cast<bool>(errorNewFace), "Failed to load font with error code " << static_cast<int>(errorNewFace));

		FT_Set_Pixel_Sizes(face, 0, 48);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

		for (unsigned char c = 0; c < 128; c++)
		{
			// Load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER) != 0)
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

	Ref<Font> Font::Create(const std::string& path)
	{
		return CreateRef<Font>(path);
	}

	Ref<Font> Font::GetDefault()
	{
		return ResourceLoader::LoadFont("/fonts/opensans/OpenSans-Regular.ttf");
	}
}
