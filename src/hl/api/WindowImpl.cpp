#include "hl/api/KeyCode.hpp"
#include "hl/api/MouseCode.hpp"
#include "hl/api/Window.hpp"
#include "hl/glfw/Window.hpp"

namespace Shroon
{
    namespace Prism
    {
        namespace HL
        {
            namespace ModifierCode
            {
                uint32_t Control;
                uint32_t Shift;
                uint32_t Alt;
                uint32_t Super;
            }

            namespace KeyCode
            {
                uint32_t NumLock;
                uint32_t Num0;
                uint32_t Num1;
                uint32_t Num2;
                uint32_t Num3;
                uint32_t Num4;
                uint32_t Num5;
                uint32_t Num6;
                uint32_t Num7;
                uint32_t Num8;
                uint32_t Num9;
                uint32_t NumAdd;
                uint32_t NumSubtract;
                uint32_t NumMultiply;
                uint32_t NumDivide;
                uint32_t NumEnter;
                uint32_t NumEqual;
                uint32_t NumDecimal;

                uint32_t Alpha0;
                uint32_t Alpha1;
                uint32_t Alpha2;
                uint32_t Alpha3;
                uint32_t Alpha4;
                uint32_t Alpha5;
                uint32_t Alpha6;
                uint32_t Alpha7;
                uint32_t Alpha8;
                uint32_t Alpha9;
                uint32_t A;
                uint32_t B;
                uint32_t C;
                uint32_t D;
                uint32_t E;
                uint32_t F;
                uint32_t G;
                uint32_t H;
                uint32_t I;
                uint32_t J;
                uint32_t K;
                uint32_t L;
                uint32_t M;
                uint32_t N;
                uint32_t O;
                uint32_t P;
                uint32_t Q;
                uint32_t R;
                uint32_t S;
                uint32_t T;
                uint32_t U;
                uint32_t V;
                uint32_t W;
                uint32_t X;
                uint32_t Y;
                uint32_t Z;

                uint32_t Space;
                uint32_t GraveAccent;
                uint32_t Minus;
                uint32_t Equal;
                uint32_t LeftBracket;
                uint32_t RightBracket;
                uint32_t Backslash;
                uint32_t Semicolon;
                uint32_t Apostrophe;
                uint32_t Comma;
                uint32_t Period;
                uint32_t Slash;

                uint32_t F1;
                uint32_t F2;
                uint32_t F3;
                uint32_t F4;
                uint32_t F5;
                uint32_t F6;
                uint32_t F7;
                uint32_t F8;
                uint32_t F9;
                uint32_t F10;
                uint32_t F11;
                uint32_t F12;

                uint32_t Escape;
                uint32_t Backspace;
                uint32_t Tab;
                uint32_t CapsLock;
                uint32_t Enter;
                uint32_t LeftControl;
                uint32_t LeftShift;
                uint32_t LeftSuper;
                uint32_t LeftAlt;
                uint32_t RightControl;
                uint32_t RightShift;
                uint32_t RightSuper;
                uint32_t RightAlt;
                uint32_t Menu;
                uint32_t Home;
                uint32_t End;
                uint32_t Insert;
                uint32_t Delete;
                uint32_t PageUp;
                uint32_t PageDown;
                uint32_t PrintScreen;

                uint32_t UpArrow;
                uint32_t DownArrow;
                uint32_t LeftArrow;
                uint32_t RightArrow;
            }

            namespace MouseCode
            {
                uint32_t LeftButton;
                uint32_t MiddleButton;
                uint32_t RightButton;
            }

            void( * Window::CreateFn)(Window &, WindowSpecification) = 0;
            void( * Window::DestroyFn)(Window &) = 0;

            Handle( * Window::GetRawFn)(Window &) = 0;
            uint32_t( * Window::GetWidthFn)(Window &) = 0;
            uint32_t( * Window::GetHeightFn)(Window &) = 0;

            void( * Window::SetTitleFn)(Window &, const std::string &) = 0;

            void( * Window::SetCharCallbackFn)(Window &, CharCallbackFn) = 0;
            void( * Window::SetKeyCallbackFn)(Window &, KeyCallbackFn) = 0;
            void( * Window::SetMouseButtonCallbackFn)(Window &, MouseButtonCallbackFn) = 0;
            void( * Window::SetCursorPosCallbackFn)(Window &, CursorPosCallbackFn) = 0;
            void( * Window::SetScrollCallbackFn)(Window &, ScrollCallbackFn) = 0;

            void( * Window::SetResizeCallbackFn)(Window &, ResizeCallbackFn) = 0;

            void( * Window::UpdateFn)(Window &, Framebuffer &, void( * )(void *), void *) = 0;

            void APIManager::InitWindowAPI()
            {
                switch (s_GraphicsAPI)
                {
                    case API::GL33:
                    case API::GL45:
                    case API::ES30:
                    {
                        ModifierCode::Control   = GLFW_MOD_CONTROL;
                        ModifierCode::Shift     = GLFW_MOD_SHIFT;
                        ModifierCode::Alt       = GLFW_MOD_ALT;
                        ModifierCode::Super     = GLFW_MOD_SUPER;

                        KeyCode::NumLock        = GLFW_KEY_NUM_LOCK;
                        KeyCode::Num0           = GLFW_KEY_KP_0;
                        KeyCode::Num1           = GLFW_KEY_KP_1;
                        KeyCode::Num2           = GLFW_KEY_KP_2;
                        KeyCode::Num3           = GLFW_KEY_KP_3;
                        KeyCode::Num4           = GLFW_KEY_KP_4;
                        KeyCode::Num5           = GLFW_KEY_KP_5;
                        KeyCode::Num6           = GLFW_KEY_KP_6;
                        KeyCode::Num7           = GLFW_KEY_KP_7;
                        KeyCode::Num8           = GLFW_KEY_KP_8;
                        KeyCode::Num9           = GLFW_KEY_KP_9;
                        KeyCode::NumAdd         = GLFW_KEY_KP_ADD;
                        KeyCode::NumSubtract    = GLFW_KEY_KP_SUBTRACT;
                        KeyCode::NumMultiply    = GLFW_KEY_KP_MULTIPLY;
                        KeyCode::NumDivide      = GLFW_KEY_KP_DIVIDE;
                        KeyCode::NumEnter       = GLFW_KEY_KP_ENTER;
                        KeyCode::NumEqual       = GLFW_KEY_KP_EQUAL;
                        KeyCode::NumDecimal     = GLFW_KEY_KP_DECIMAL;

                        KeyCode::Alpha0 = GLFW_KEY_0;
                        KeyCode::Alpha1 = GLFW_KEY_1;
                        KeyCode::Alpha2 = GLFW_KEY_2;
                        KeyCode::Alpha3 = GLFW_KEY_3;
                        KeyCode::Alpha4 = GLFW_KEY_4;
                        KeyCode::Alpha5 = GLFW_KEY_5;
                        KeyCode::Alpha6 = GLFW_KEY_6;
                        KeyCode::Alpha7 = GLFW_KEY_7;
                        KeyCode::Alpha8 = GLFW_KEY_8;
                        KeyCode::Alpha9 = GLFW_KEY_9;
                        KeyCode::A      = GLFW_KEY_A;
                        KeyCode::B      = GLFW_KEY_B;
                        KeyCode::C      = GLFW_KEY_C;
                        KeyCode::D      = GLFW_KEY_D;
                        KeyCode::E      = GLFW_KEY_E;
                        KeyCode::F      = GLFW_KEY_F;
                        KeyCode::G      = GLFW_KEY_G;
                        KeyCode::H      = GLFW_KEY_H;
                        KeyCode::I      = GLFW_KEY_I;
                        KeyCode::J      = GLFW_KEY_J;
                        KeyCode::K      = GLFW_KEY_K;
                        KeyCode::L      = GLFW_KEY_L;
                        KeyCode::M      = GLFW_KEY_M;
                        KeyCode::N      = GLFW_KEY_N;
                        KeyCode::O      = GLFW_KEY_O;
                        KeyCode::P      = GLFW_KEY_P;
                        KeyCode::Q      = GLFW_KEY_Q;
                        KeyCode::R      = GLFW_KEY_R;
                        KeyCode::S      = GLFW_KEY_S;
                        KeyCode::T      = GLFW_KEY_T;
                        KeyCode::U      = GLFW_KEY_U;
                        KeyCode::V      = GLFW_KEY_V;
                        KeyCode::W      = GLFW_KEY_W;
                        KeyCode::X      = GLFW_KEY_X;
                        KeyCode::Y      = GLFW_KEY_Y;
                        KeyCode::Z      = GLFW_KEY_Z;

                        KeyCode::Space          = GLFW_KEY_SPACE;
                        KeyCode::GraveAccent    = GLFW_KEY_GRAVE_ACCENT;
                        KeyCode::Minus          = GLFW_KEY_MINUS;
                        KeyCode::Equal          = GLFW_KEY_EQUAL;
                        KeyCode::LeftBracket    = GLFW_KEY_LEFT_BRACKET;
                        KeyCode::RightBracket   = GLFW_KEY_RIGHT_BRACKET;
                        KeyCode::Backslash      = GLFW_KEY_BACKSLASH;
                        KeyCode::Semicolon      = GLFW_KEY_SEMICOLON;
                        KeyCode::Apostrophe     = GLFW_KEY_APOSTROPHE;
                        KeyCode::Comma          = GLFW_KEY_COMMA;
                        KeyCode::Period         = GLFW_KEY_PERIOD;
                        KeyCode::Slash          = GLFW_KEY_SLASH;

                        KeyCode::F1     = GLFW_KEY_F1;
                        KeyCode::F2     = GLFW_KEY_F2;
                        KeyCode::F3     = GLFW_KEY_F3;
                        KeyCode::F4     = GLFW_KEY_F4;
                        KeyCode::F5     = GLFW_KEY_F5;
                        KeyCode::F6     = GLFW_KEY_F6;
                        KeyCode::F7     = GLFW_KEY_F7;
                        KeyCode::F8     = GLFW_KEY_F8;
                        KeyCode::F9     = GLFW_KEY_F9;
                        KeyCode::F10    = GLFW_KEY_F10;
                        KeyCode::F11    = GLFW_KEY_F11;
                        KeyCode::F12    = GLFW_KEY_F12;

                        KeyCode::Escape         = GLFW_KEY_ESCAPE;
                        KeyCode::Backspace      = GLFW_KEY_BACKSPACE;
                        KeyCode::Tab            = GLFW_KEY_TAB;
                        KeyCode::CapsLock       = GLFW_KEY_CAPS_LOCK;
                        KeyCode::Enter          = GLFW_KEY_ENTER;
                        KeyCode::LeftControl    = GLFW_KEY_LEFT_CONTROL;
                        KeyCode::LeftShift      = GLFW_KEY_LEFT_SHIFT;
                        KeyCode::LeftSuper      = GLFW_KEY_LEFT_SUPER;
                        KeyCode::LeftAlt        = GLFW_KEY_LEFT_ALT;
                        KeyCode::RightControl   = GLFW_KEY_RIGHT_CONTROL;
                        KeyCode::RightShift     = GLFW_KEY_RIGHT_SHIFT;
                        KeyCode::RightSuper     = GLFW_KEY_RIGHT_SUPER;
                        KeyCode::RightAlt       = GLFW_KEY_RIGHT_ALT;
                        KeyCode::Menu           = GLFW_KEY_MENU;
                        KeyCode::Home           = GLFW_KEY_HOME;
                        KeyCode::End            = GLFW_KEY_END;
                        KeyCode::Insert         = GLFW_KEY_INSERT;
                        KeyCode::Delete         = GLFW_KEY_DELETE;
                        KeyCode::PageUp         = GLFW_KEY_PAGE_UP;
                        KeyCode::PageDown       = GLFW_KEY_PAGE_DOWN;
                        KeyCode::PrintScreen    = GLFW_KEY_PRINT_SCREEN;

                        KeyCode::UpArrow    = GLFW_KEY_UP;
                        KeyCode::DownArrow  = GLFW_KEY_DOWN;
                        KeyCode::LeftArrow  = GLFW_KEY_LEFT;
                        KeyCode::RightArrow = GLFW_KEY_RIGHT;

                        MouseCode::LeftButton = GLFW_MOUSE_BUTTON_LEFT;
                        MouseCode::MiddleButton = GLFW_MOUSE_BUTTON_MIDDLE;
                        MouseCode::RightButton  = GLFW_MOUSE_BUTTON_RIGHT;

                        break;
                    }
                }

                switch(s_GraphicsAPI)
                {
                    case API::GL33:
                    case API::GL45:
                    case API::ES30:
                    {
                        Window::CreateFn                    = GLFWWindow::Create;
                        Window::DestroyFn                   = GLFWWindow::Destroy;
                        Window::GetRawFn                    = GLFWWindow::GetRaw;
                        Window::GetWidthFn                  = GLFWWindow::GetWidth;
                        Window::GetHeightFn                 = GLFWWindow::GetHeight;
                        Window::SetTitleFn                  = GLFWWindow::SetTitle;
                        Window::SetCharCallbackFn           = GLFWWindow::SetCharCallback;
                        Window::SetKeyCallbackFn            = GLFWWindow::SetKeyCallback;
                        Window::SetMouseButtonCallbackFn    = GLFWWindow::SetMouseButtonCallback;
                        Window::SetCursorPosCallbackFn      = GLFWWindow::SetCursorPosCallback;
                        Window::SetScrollCallbackFn         = GLFWWindow::SetScrollCallback;
                        Window::SetResizeCallbackFn         = GLFWWindow::SetResizeCallback;
                        Window::UpdateFn                    = GLFWWindow::Update;

                        break;
                    }
                }
            }
        }
    }
}
