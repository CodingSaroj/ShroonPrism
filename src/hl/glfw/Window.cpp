#include "Window.hpp"

namespace Shroon
{
    namespace Prism
    {
        namespace HL
        {
            std::string APIString;

            static void GLMsgCallback(unsigned source, unsigned type, unsigned id, unsigned severity, int length, const char * message, const void * userParam)
            {
                switch(severity)
                {
                    case GL_DEBUG_SEVERITY_HIGH:            ErrorReporter(SHRN_PRISM_LEVEL_ERROR, APIString, message); break;
                    case GL_DEBUG_SEVERITY_MEDIUM:          ErrorReporter(SHRN_PRISM_LEVEL_ERROR, APIString, message); break;
                    case GL_DEBUG_SEVERITY_LOW:             ErrorReporter(SHRN_PRISM_LEVEL_WARNING, APIString, message); break;
                    case GL_DEBUG_SEVERITY_NOTIFICATION:    ErrorReporter(SHRN_PRISM_LEVEL_INFO, APIString, message); break;
                }
            }

            void ErrorCallback(int errcode, const char * description)
            {
                ErrorReporter(SHRN_PRISM_LEVEL_ERROR, APIString + "::Window", description);
            }

            void GLFWWindow::CharCallback(GLFWwindow * glfwWindow, unsigned int codepoint)
            {
                Window & window = *reinterpret_cast<Window *>(glfwGetWindowUserPointer(glfwWindow));

                if (window.m_CharCallback)
                {
                    window.m_CharCallback(codepoint);
                }
            }

            void GLFWWindow::KeyCallback(GLFWwindow * glfwWindow, int key, int scancode, int action, int mod)
            {
                Window & window = *reinterpret_cast<Window *>(glfwGetWindowUserPointer(glfwWindow));

                if (window.m_KeyCallback)
                {
                    window.m_KeyCallback(key, action == GLFW_PRESS ? true : false, action == GLFW_REPEAT ? true : false);
                }
            }

            void GLFWWindow::MouseButtonCallback(GLFWwindow * glfwWindow, int button, int action, int mode)
            {
                Window & window = *reinterpret_cast<Window *>(glfwGetWindowUserPointer(glfwWindow));

                if (window.m_MouseButtonCallback)
                {
                    window.m_MouseButtonCallback(button, action == GLFW_PRESS ? true : false, action == GLFW_REPEAT ? true : false);
                }
            }

            void GLFWWindow::CursorPosCallback(GLFWwindow * glfwWindow, double x, double y)
            {
                Window & window = *reinterpret_cast<Window *>(glfwGetWindowUserPointer(glfwWindow));

                if (window.m_CursorPosCallback)
                {
                    window.m_CursorPosCallback(x, y);
                }
            }

            void GLFWWindow::ScrollCallback(GLFWwindow * glfwWindow, double x, double y)
            {
                Window & window = *reinterpret_cast<Window *>(glfwGetWindowUserPointer(glfwWindow));

                if (window.m_ScrollCallback)
                {
                    window.m_ScrollCallback(x, y);
                }
            }

            void GLFWWindow::ResizeCallback(GLFWwindow * glfwWindow, int width, int height)
            {
                Window & window = *reinterpret_cast<Window *>(glfwGetWindowUserPointer(glfwWindow));

                if (window.m_ResizeCallback)
                {
                    window.m_ResizeCallback(width, height);
                }
            }

            void GLFWWindow::Create(Window & window, WindowSpecification spec)
            {
                switch (s_GraphicsAPI)
                {
                    case API::GL33: APIString = "OpenGL3.3"; break;
                    case API::GL45: APIString = "OpenGL4.5"; break;
                }

                glfwInit();

                glfwWindowHint(GLFW_RESIZABLE, spec.Resizable ? GLFW_TRUE : GLFW_FALSE);
                glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

                if (s_GraphicsAPI == API::GL33)
                {
                    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
                    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
                    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
                }
                else if (s_GraphicsAPI == API::GL45)
                {
                    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
                    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
                    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
                }

                glfwSetErrorCallback(ErrorCallback);

                window.m_Handle = glfwCreateWindow(spec.Width, spec.Height, spec.Title.c_str(), nullptr, nullptr);

                if (!window.m_Handle)
                {
                    const char * msg;
                    glfwGetError(&msg);

                    ErrorReporter(SHRN_PRISM_LEVEL_FATAL, "GLFW::Window", "Failed to create window.");
                    ErrorReporter(SHRN_PRISM_LEVEL_FATAL, "GLFW::Window", msg);
                    exit(1);
                }

                if (s_GraphicsAPI == API::GL33 || s_GraphicsAPI == API::GL45)
                {
                    glfwMakeContextCurrent(window.m_Handle);

                    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
                    {
                        ErrorReporter(SHRN_PRISM_LEVEL_FATAL, APIString, "Failed to load GL functions.");
                        exit(1);
                    }

                    if (!GLAD_GL_ARB_debug_output)
                    {
                        ErrorReporter(SHRN_PRISM_LEVEL_WARNING, APIString, "GL_ARB_debug_output is required for debugging but is not available.");
                    }
                }

                glfwSetWindowUserPointer(window.m_Handle, &window);

                glfwSetCharCallback(window.m_Handle, CharCallback);
                glfwSetKeyCallback(window.m_Handle, KeyCallback);
                glfwSetMouseButtonCallback(window.m_Handle, MouseButtonCallback);
                glfwSetCursorPosCallback(window.m_Handle, CursorPosCallback);
                glfwSetScrollCallback(window.m_Handle, ScrollCallback);

                glfwSetFramebufferSizeCallback(window.m_Handle, ResizeCallback);

                if (s_GraphicsAPI == API::GL33 || s_GraphicsAPI == API::GL45)
                {
                    const GLubyte * version = glGetString(GL_VERSION);
                    const GLubyte * vendor = glGetString(GL_VENDOR);
                    
                    ErrorReporter(SHRN_PRISM_LEVEL_INFO, APIString, "Version:" + std::string(reinterpret_cast<const char *>(version)) + ".");
                    ErrorReporter(SHRN_PRISM_LEVEL_INFO, APIString, "Vendor:" + std::string(reinterpret_cast<const char *>(vendor)) + ".");

                    if (s_GraphicsAPI == API::GL45)
                    {
                        glEnable(GL_DEBUG_OUTPUT);
                        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
                        glDebugMessageCallback(GLMsgCallback, nullptr);
                        
                        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
                    }
                    else if (s_GraphicsAPI == API::GL33 && GLAD_GL_ARB_debug_output)
                    {
                        glEnable(GL_DEBUG_OUTPUT);
                        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
                        glDebugMessageCallbackARB(GLMsgCallback, nullptr);
                        
                        glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
                    }
                }
            }

            void GLFWWindow::Destroy(Window & window)
            {
                glfwDestroyWindow(window.GetRaw());

                glfwTerminate();
            }

            Handle GLFWWindow::GetRaw(Window & window)
            {
                return window.m_Handle;
            }

            uint32_t GLFWWindow::GetWidth(Window & window)
            {
                int width;

                glfwGetWindowSize(window.GetRaw(), &width, nullptr);

                return width;
            }

            uint32_t GLFWWindow::GetHeight(Window & window)
            {
                int height;

                glfwGetWindowSize(window.GetRaw(), nullptr, &height);

                return height;
            }

            void GLFWWindow::SetTitle(Window & window, const std::string & title)
            {
                glfwSetWindowTitle(window.m_Handle, title.c_str());
            }

            void GLFWWindow::SetCharCallback(Window & window, Window::CharCallbackFn callback)
            {
                window.m_CharCallback = callback;
            }

            void GLFWWindow::SetKeyCallback(Window & window, Window::KeyCallbackFn callback)
            {
                window.m_KeyCallback = callback;
            }

            void GLFWWindow::SetMouseButtonCallback(Window & window, Window::MouseButtonCallbackFn callback)
            {
                window.m_MouseButtonCallback = callback;
            }

            void GLFWWindow::SetCursorPosCallback(Window & window, Window::CursorPosCallbackFn callback)
            {
                window.m_CursorPosCallback = callback;
            }

            void GLFWWindow::SetScrollCallback(Window & window, Window::ScrollCallbackFn callback)
            {
                window.m_ScrollCallback = callback;
            }

            void GLFWWindow::SetResizeCallback(Window & window, Window::ResizeCallbackFn callback)
            {
                window.m_ResizeCallback = callback;
            }

            void GLFWWindow::Update(Window & window, Framebuffer & src, void( * updateFn)(void *), void * userData)
            {
                while (!glfwWindowShouldClose(window.m_Handle))
                {
                    glfwPollEvents();

                    glfwSetTime(0.0);

                    auto s = std::chrono::steady_clock::now();

                    updateFn(userData);

                    if (s_GraphicsAPI == API::GL33 || s_GraphicsAPI == API::GL45)
                    {
                        src.Blit(0u,
                            0, 0, src.GetWidth() - 1, src.GetHeight() - 1,
                            0, 0, src.GetWidth() - 1, src.GetHeight() - 1
                        );

                        glfwSwapBuffers(window.m_Handle);
                    }

                    auto e1 = std::chrono::steady_clock::now();

                    while (glfwGetTime() < 0.0167) {}

                    auto e2 = std::chrono::steady_clock::now();

                    auto d1 = e1 - s;
                    auto d2 = e2 - s;

                    ErrorReporter(SHRN_PRISM_LEVEL_INFO, "PrismTest",
                        "Frametime: " +
                        std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(d2).count()) + "ms\t" +
                        "Render + Presentation time: " +
                        std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(d1).count()) + "ms."
                    );
                }
            }
        }
    }
}
