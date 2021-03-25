#pragma once

#include <string>

#include "API.hpp"
#include "APIManager.hpp"

#include "api/Handle.hpp"
#include "api/Framebuffer.hpp"

namespace Shroon
{
    namespace Prism
    {
        struct WindowSpecification
        {
            bool Resizable = false;
            uint32_t Width = 800;
            uint32_t Height = 600;
            std::string Title = "Prism Window";
        };

        class Window
        {
        public:
            using CharCallbackFn = void( * )(unsigned int);
            using KeyCallbackFn = void( * )(uint32_t, bool, bool);
            using MouseButtonCallbackFn = void( * )(uint32_t, bool, bool);
            using CursorPosCallbackFn = void( * )(double, double);
            using ScrollCallbackFn = void( * )(double, double);

            using ResizeCallbackFn = void( * )(uint32_t, uint32_t);

            inline void Create(WindowSpecification spec)
            {
                CreateFn(*this, spec);
            }

            inline void Destroy()
            {
                DestroyFn(*this);
            }

            inline Handle GetRaw()
            {
                return GetRawFn(*this);
            }

            inline uint32_t GetWidth()
            {
                return GetWidthFn(*this);
            }

            inline uint32_t GetHeight()
            {
                return GetHeightFn(*this);
            }

            inline void SetTitle(const std::string & title)
            {
                SetTitleFn(*this, title);
            }

            inline void SetCharCallback(CharCallbackFn callback)
            {
                SetCharCallbackFn(*this, callback);
            }

            inline void SetKeyCallback(KeyCallbackFn callback)
            {
                SetKeyCallbackFn(*this, callback);
            }

            inline void SetMouseButtonCallback(MouseButtonCallbackFn callback)
            {
                SetMouseButtonCallbackFn(*this, callback);
            }

            inline void SetCursorPosCallback(CursorPosCallbackFn callback)
            {
                SetCursorPosCallbackFn(*this, callback);
            }

            inline void SetScrollCallback(ScrollCallbackFn callback)
            {
                SetScrollCallbackFn(*this, callback);
            }

            inline void SetResizeCallback(ResizeCallbackFn callback)
            {
                SetResizeCallbackFn(*this, callback);
            }

            inline void Update(Framebuffer & src, void( * updateFn)(void *), void * userData)
            {
                UpdateFn(*this, src, updateFn, userData);
            }

        private:
            Handle m_Handle;

            CharCallbackFn m_CharCallback = nullptr;
            KeyCallbackFn m_KeyCallback = nullptr;
            MouseButtonCallbackFn m_MouseButtonCallback = nullptr;
            CursorPosCallbackFn m_CursorPosCallback = nullptr;
            ScrollCallbackFn m_ScrollCallback = nullptr;
            ResizeCallbackFn m_ResizeCallback = nullptr;

            static void( * CreateFn)(Window &, WindowSpecification);
            static void( * DestroyFn)(Window &);

            static Handle( * GetRawFn)(Window &);
            static uint32_t( * GetWidthFn)(Window &);
            static uint32_t( * GetHeightFn)(Window &);

            static void( * SetTitleFn)(Window &, const std::string &);

            static void( * SetCharCallbackFn)(Window &, CharCallbackFn);
            static void( * SetKeyCallbackFn)(Window &, KeyCallbackFn);
            static void( * SetMouseButtonCallbackFn)(Window &, MouseButtonCallbackFn);
            static void( * SetCursorPosCallbackFn)(Window &, CursorPosCallbackFn);
            static void( * SetScrollCallbackFn)(Window &, ScrollCallbackFn);

            static void( * SetResizeCallbackFn)(Window &, ResizeCallbackFn);

            static void( * UpdateFn)(Window &, Framebuffer &, void( * )(void *), void *);

            friend class APIManager;

            friend class GLFWWindow;
        };
    }
}
