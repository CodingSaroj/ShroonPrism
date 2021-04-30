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
                    case API::ES30: APIString = "OpenGLES3.0"; break;
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
                else if (s_GraphicsAPI == API::ES30)
                {
                    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
                    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
                    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
                    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
                }

                glfwSetErrorCallback(ErrorCallback);

                window.m_Handle = glfwCreateWindow(spec.Width, spec.Height, spec.Title.c_str(), nullptr, nullptr);

                if (!window.m_Handle.As<GLFWwindow *>())
                {
                    // Emscripten uses GLFW which does not support glfwGetError().
                #if __EMSCRIPTEN__
                    ErrorReporter(SHRN_PRISM_LEVEL_FATAL, "GLFW::Window", "Failed to create window.");
                #else
                    const char * msg;
                    glfwGetError(&msg);

                    ErrorReporter(SHRN_PRISM_LEVEL_FATAL, "GLFW::Window", "Failed to create window.");
                    ErrorReporter(SHRN_PRISM_LEVEL_FATAL, "GLFW::Window", msg);
                    exit(1);
                #endif
                }

                if (s_GraphicsAPI == API::GL33 || s_GraphicsAPI == API::GL45)
                {
                    glfwMakeContextCurrent(window.m_Handle.As<GLFWwindow *>());

                    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
                    {
                        ErrorReporter(SHRN_PRISM_LEVEL_FATAL, APIString, "Failed to load GL functions.");
                        exit(1);
                    }
                }
                else if (s_GraphicsAPI == API::ES30)
                {
                    glfwMakeContextCurrent(window.m_Handle.As<GLFWwindow *>());

                    if (!gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress))
                    {
                        ErrorReporter(SHRN_PRISM_LEVEL_FATAL, APIString, "Failed to load GLES functions.");
                        exit(1);
                    }
                }

                glfwSetWindowUserPointer(window.m_Handle.As<GLFWwindow *>(), &window);

                glfwSetCharCallback(window.m_Handle.As<GLFWwindow *>(), CharCallback);
                glfwSetKeyCallback(window.m_Handle.As<GLFWwindow *>(), KeyCallback);
                glfwSetMouseButtonCallback(window.m_Handle.As<GLFWwindow *>(), MouseButtonCallback);
                glfwSetCursorPosCallback(window.m_Handle.As<GLFWwindow *>(), CursorPosCallback);
                glfwSetScrollCallback(window.m_Handle.As<GLFWwindow *>(), ScrollCallback);

                glfwSetFramebufferSizeCallback(window.m_Handle.As<GLFWwindow *>(), ResizeCallback);

                if (s_GraphicsAPI == API::GL33 || s_GraphicsAPI == API::GL45 || s_GraphicsAPI == API::ES30)
                {
                    const GLubyte * version = glGetString(GL_VERSION);
                    const GLubyte * vendor = glGetString(GL_VENDOR);
                    const GLubyte * renderer = glGetString(GL_RENDERER);
                    
                    ErrorReporter(SHRN_PRISM_LEVEL_INFO, APIString, "Version: " + std::string(reinterpret_cast<const char *>(version)) + ".");
                    ErrorReporter(SHRN_PRISM_LEVEL_INFO, APIString, "Vendor: " + std::string(reinterpret_cast<const char *>(vendor)) + ".");
                    ErrorReporter(SHRN_PRISM_LEVEL_INFO, APIString, "Renderer: " + std::string(reinterpret_cast<const char *>(renderer)) + ".");

                    if (GLAD_GL_ARB_debug_output || GLAD_GL_KHR_debug)
                    {
                        if (s_GraphicsAPI == API::GL45)
                        {
                            glEnable(GL_DEBUG_OUTPUT);
                            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
                            glDebugMessageCallback(GLMsgCallback, nullptr);
                            
                            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
                        }
                        else if (GLAD_GL_ARB_debug_output && (s_GraphicsAPI == API::GL33 || s_GraphicsAPI == API::ES30))
                        {
                            glEnable(GL_DEBUG_OUTPUT);
                            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
                            glDebugMessageCallbackARB(GLMsgCallback, nullptr);
                            
                            glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
                        }
                        else if (GLAD_GL_KHR_debug && (s_GraphicsAPI == API::GL33 || s_GraphicsAPI == API::ES30))
                        {
                            glEnable(GL_DEBUG_OUTPUT);
                            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_KHR);
                            glDebugMessageCallbackKHR(GLMsgCallback, nullptr);
                            
                            glDebugMessageControlKHR(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
                        }
                    }
                    else
                    {
                        ErrorReporter(SHRN_PRISM_LEVEL_WARNING, APIString, "GL_ARB_debug_output or GL_KHR_debug is required for debugging but is not available.");
                    }
                }
            }

            void GLFWWindow::Destroy(Window & window)
            {
                glfwDestroyWindow(window.m_Handle.As<GLFWwindow *>());

                glfwTerminate();
            }

            Handle GLFWWindow::GetRaw(Window & window)
            {
                return window.m_Handle.As<GLFWwindow *>();
            }

            uint32_t GLFWWindow::GetWidth(Window & window)
            {
                int width;

                glfwGetWindowSize(window.m_Handle.As<GLFWwindow *>(), &width, nullptr);

                return width;
            }

            uint32_t GLFWWindow::GetHeight(Window & window)
            {
                int height;

                glfwGetWindowSize(window.m_Handle.As<GLFWwindow *>(), nullptr, &height);

                return height;
            }

            void GLFWWindow::SetTitle(Window & window, const std::string & title)
            {
                glfwSetWindowTitle(window.m_Handle.As<GLFWwindow *>(), title.c_str());
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
                window.m_IsRunning = true;

                struct UpdateData
                {
                    Window & This;
                    Framebuffer & Src;
                    void( * UpdateFn)(void *);
                    void * UserData;
                } updateData{window, src, updateFn, userData};
                
                auto update = [](void * data)
                {
                    UpdateData & dt = *reinterpret_cast<UpdateData *>(data);

                #ifdef __EMSCRIPTEN__
                    if (!dt.This.m_IsRunning)
                    {
                        emscripten_cancel_main_loop();
                    }
                #endif

                    glfwPollEvents();

                    glfwSetTime(0.0);

                    auto s = std::chrono::steady_clock::now();

                    dt.UpdateFn(dt.UserData);

                    if (s_GraphicsAPI == API::GL33 || s_GraphicsAPI == API::GL45 || s_GraphicsAPI == API::ES30)
                    {
                        dt.Src.Blit(0u,
                            0, 0, dt.Src.GetWidth() - 1, dt.Src.GetHeight() - 1,
                            0, 0, dt.Src.GetWidth() - 1, dt.Src.GetHeight() - 1
                        );

                        glfwSwapBuffers(dt.This.m_Handle.As<GLFWwindow *>());
                    }

                    auto e1 = std::chrono::steady_clock::now();

                    while (glfwGetTime() < 0.0167) {}

                    auto e2 = std::chrono::steady_clock::now();

                    auto d1 = e1 - s;
                    auto d2 = e2 - s;

                    ErrorReporter(SHRN_PRISM_LEVEL_INFO, "Window",
                        "Frametime: " +
                        std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(d2).count()) + "ms\t" +
                        "Render + Presentation time: " +
                        std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(d1).count()) + "ms."
                    );
                };

            #ifdef __EMSCRIPTEN__
                emscripten_set_main_loop_arg(update, &updateData, 0, EM_TRUE);
            #else
                while (!glfwWindowShouldClose(window.m_Handle.As<GLFWwindow *>()) && window.m_IsRunning)
                {
                    update(&updateData);
                }
            #endif
            }
        }
    }
}
