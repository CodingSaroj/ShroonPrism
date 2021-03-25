#include "api/Framebuffer.hpp"
#include "vk12/Framebuffer.hpp"
#include "gl33/Framebuffer.hpp"
#include "gl45/Framebuffer.hpp"

namespace Shroon
{
    namespace Prism
    {
        void( * Framebuffer::CreateFn)(Framebuffer &, FramebufferSpecification) = 0;
        void( * Framebuffer::DestroyFn)(Framebuffer &) = 0;

        void( * Framebuffer::ResizeFn)(Framebuffer &, uint32_t, uint32_t) = 0;

        void( * Framebuffer::EnableFn)(Framebuffer &) = 0;
        void( * Framebuffer::DisableFn)(Framebuffer &) = 0;

        void( * Framebuffer::SetClearColorFn)(Framebuffer &, float, float, float, float) = 0;
        void( * Framebuffer::SetClearDepthFn)(Framebuffer &, float) = 0;
        void( * Framebuffer::SetClearStencilFn)(Framebuffer &, uint32_t) = 0;
        void( * Framebuffer::ClearBuffersFn)(Framebuffer &) = 0;

        void( * Framebuffer::ReadPixelsColorFn)(Framebuffer &, uint32_t colorAttachmentIndex, uint32_t x, uint32_t y, uint32_t width, uint32_t height, void * data) = 0;
        void( * Framebuffer::ReadPixelsDepthFn)(Framebuffer &, uint32_t x, uint32_t y, uint32_t width, uint32_t height, void * data) = 0;
        void( * Framebuffer::ReadPixelsStencilFn)(Framebuffer &, uint32_t x, uint32_t y, uint32_t width, uint32_t height, void * data) = 0;
        void( * Framebuffer::BlitFn)(Framebuffer &, Handle, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t) = 0;

        void APIManager::InitFramebufferAPI()
        {
            switch (s_GraphicsAPI)
            {
                case API::GL33:
                {
                    Framebuffer::CreateFn               = GL33Framebuffer::Create;
                    Framebuffer::DestroyFn              = GL33Framebuffer::Destroy;
                    Framebuffer::ResizeFn               = GL33Framebuffer::Resize;
                    Framebuffer::EnableFn               = GL33Framebuffer::Enable;
                    Framebuffer::DisableFn              = GL33Framebuffer::Disable;
                    Framebuffer::SetClearColorFn        = GL33Framebuffer::SetClearColor;
                    Framebuffer::SetClearDepthFn        = GL33Framebuffer::SetClearDepth;
                    Framebuffer::SetClearStencilFn      = GL33Framebuffer::SetClearStencil;
                    Framebuffer::ClearBuffersFn         = GL33Framebuffer::ClearBuffers;
                    Framebuffer::ReadPixelsColorFn      = GL33Framebuffer::ReadPixelsColor;
                    Framebuffer::ReadPixelsDepthFn      = GL33Framebuffer::ReadPixelsDepth;
                    Framebuffer::ReadPixelsStencilFn    = GL33Framebuffer::ReadPixelsStencil;
                    Framebuffer::BlitFn                 = GL33Framebuffer::Blit;

                    break;
                }

                case API::GL45:
                {
                    Framebuffer::CreateFn               = GL45Framebuffer::Create;
                    Framebuffer::DestroyFn              = GL45Framebuffer::Destroy;
                    Framebuffer::ResizeFn               = GL45Framebuffer::Resize;
                    Framebuffer::EnableFn               = GL45Framebuffer::Enable;
                    Framebuffer::DisableFn              = GL45Framebuffer::Disable;
                    Framebuffer::SetClearColorFn        = GL45Framebuffer::SetClearColor;
                    Framebuffer::SetClearDepthFn        = GL45Framebuffer::SetClearDepth;
                    Framebuffer::SetClearStencilFn      = GL45Framebuffer::SetClearStencil;
                    Framebuffer::ClearBuffersFn         = GL45Framebuffer::ClearBuffers;
                    Framebuffer::ReadPixelsColorFn      = GL45Framebuffer::ReadPixelsColor;
                    Framebuffer::ReadPixelsDepthFn      = GL45Framebuffer::ReadPixelsDepth;
                    Framebuffer::ReadPixelsStencilFn    = GL45Framebuffer::ReadPixelsStencil;
                    Framebuffer::BlitFn                 = GL45Framebuffer::Blit;

                    break;
                }

                case API::VK12:
                {
                    Framebuffer::CreateFn               = VK12Framebuffer::Create;
                    Framebuffer::DestroyFn              = VK12Framebuffer::Destroy;
                    Framebuffer::ResizeFn               = VK12Framebuffer::Resize;
                    Framebuffer::EnableFn               = GL45Framebuffer::Enable;
                    Framebuffer::DisableFn              = GL45Framebuffer::Disable;
                    Framebuffer::SetClearColorFn        = GL45Framebuffer::SetClearColor;
                    Framebuffer::SetClearDepthFn        = GL45Framebuffer::SetClearDepth;
                    Framebuffer::SetClearStencilFn      = GL45Framebuffer::SetClearStencil;
                    Framebuffer::ClearBuffersFn         = GL45Framebuffer::ClearBuffers;
                    Framebuffer::ReadPixelsColorFn      = GL45Framebuffer::ReadPixelsColor;
                    Framebuffer::ReadPixelsDepthFn      = GL45Framebuffer::ReadPixelsDepth;
                    Framebuffer::ReadPixelsStencilFn    = GL45Framebuffer::ReadPixelsStencil;
                    Framebuffer::BlitFn                 = GL45Framebuffer::Blit;

                    break;
                }
            }
        }
    }
}
