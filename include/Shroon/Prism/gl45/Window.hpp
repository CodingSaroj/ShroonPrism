#pragma once

#include "ErrorReporter.hpp"

#include "api/Window.hpp"

#include <glad/glad-gl45.h>
#include <GLFW/glfw3.h>

namespace Shroon
{
    namespace Prism
    {
        class GL45Window
        {
        public:
            static void Create(Window & window, WindowSpecification spec);
            static void Destroy(Window & window);

            static Handle GetRaw(Window & window);
            static uint32_t GetWidth(Window & window);
            static uint32_t GetHeight(Window & window);

            static void Update(Window & window, float clearColorR, float clearColorG, float clearColorB, float clearColorA, void( * updateFn)(void *), void * userData);
        };
    }
}
