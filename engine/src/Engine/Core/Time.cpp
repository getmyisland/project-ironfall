#include <dxpch.h>
#include <Engine/Core/Time.h>

#include <GLFW/glfw3.h>

namespace dyxide
{
    float Time::GetTime()
    {
        return glfwGetTime();
    }
}