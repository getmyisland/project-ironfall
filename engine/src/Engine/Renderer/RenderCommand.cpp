#include <dxpch.h>
#include <Engine/Renderer/RenderCommand.h>

namespace dyxide
{
	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}