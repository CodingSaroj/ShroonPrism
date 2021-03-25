#pragma once

#include "ErrorReporter.hpp"

#include "api/Window.hpp"
#include "api/Framebuffer.hpp"

#include "vk12/State.hpp"

#define GLFW_INCLUDE_VULKAN
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Shroon
{
    namespace Prism
    {
        class GLFWWindow
        {
        public:
            static void Create(Window & window, WindowSpecification spec);
            static void Destroy(Window & window);

            static Handle GetRaw(Window & window);
            static uint32_t GetWidth(Window & window);
            static uint32_t GetHeight(Window & window);

            static void SetTitle(Window & window, const std::string & title);

            static void SetCharCallback(Window & window, Window::CharCallbackFn callback);
            static void SetKeyCallback(Window & window, Window::KeyCallbackFn callback);
            static void SetMouseButtonCallback(Window & window, Window::MouseButtonCallbackFn callback);
            static void SetCursorPosCallback(Window & window, Window::CursorPosCallbackFn callback);
            static void SetScrollCallback(Window & window, Window::ScrollCallbackFn callback);

            static void SetResizeCallback(Window & window, Window::ResizeCallbackFn callback);

            static void Update(Window & window, Framebuffer & src, void( * updateFn)(void *), void * userData);

        private:
            static void CharCallback(GLFWwindow * window, unsigned int codepoint);
            static void KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mod);
            static void MouseButtonCallback(GLFWwindow * window, int button, int scancode, int action);
            static void CursorPosCallback(GLFWwindow * window, double x, double y);
            static void ScrollCallback(GLFWwindow * window, double x, double y);

            static void ResizeCallback(GLFWwindow * window, int width, int height);
        };
    }
}
