#include <dxpch.h>
#include <Engine/Core/Entity.h>

namespace dyxide
{
	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{
	}
}