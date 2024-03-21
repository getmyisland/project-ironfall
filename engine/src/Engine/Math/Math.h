#pragma once

#include <glm/glm.hpp>

namespace dyxide
{
	class Math
	{
	public:
		static glm::vec3 Vector3Forward(glm::vec3 vec3);
		static glm::vec3 Vector3Right(glm::vec3 vec3);
		static glm::vec3 Vector3Up(glm::vec3 vec3);
	};
}