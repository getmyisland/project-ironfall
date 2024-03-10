#include <dxpch.h>
#include <Engine/Core/Time.h>

#include <glfw/glfw3.h>

namespace dyxide
{
    float Time::GetTime()
    {
        return glfwGetTime();
    }
}