#pragma once

#include <Engine/Renderer/Mesh.h>

#include <string>
#include <vector>

namespace dyxide
{
	class Model
	{
	public:
		Model(std::vector<Mesh> meshes);
		~Model();

		std::vector<Mesh> GetMeshes() { return m_Meshes; }

		static Ref<Model> Create(const std::string& path);

	private:
		std::vector<Mesh> m_Meshes;
	};
}