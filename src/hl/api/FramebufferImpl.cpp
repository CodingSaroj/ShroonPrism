#include "hl/api/Framebuffer.hpp"
#include "hl/gl33/Framebuffer.hpp"
#include "hl/gl45/Framebuffer.hpp"
#include "hl/es30/Framebuffer.hpp"

namespace Shroon
{
    namespace Prism
    {
        namespace HL
        {
            void( * Framebuffer::CreateFn)(Framebuffer &, FramebufferSpecification &) = 0;
            void( * Framebuffer::DestroyFn)(Framebuffer &) = 0;

            void( * Framebuffer::ResizeFn)(Framebuffer &, uint32_t, uint32_t) = 0;

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

                        Framebuffer::ReadPixelsColorFn      = GL45Framebuffer::ReadPixelsColor;
                        Framebuffer::ReadPixelsDepthFn      = GL45Framebuffer::ReadPixelsDepth;
                        Framebuffer::ReadPixelsStencilFn    = GL45Framebuffer::ReadPixelsStencil;
                        Framebuffer::BlitFn                 = GL45Framebuffer::Blit;

                        break;
                    }

                    case API::ES30:
                    {
                        Framebuffer::CreateFn               = ES30Framebuffer::Create;
                        Framebuffer::DestroyFn              = ES30Framebuffer::Destroy;

                        Framebuffer::ResizeFn               = ES30Framebuffer::Resize;

                        Framebuffer::ReadPixelsColorFn      = ES30Framebuffer::ReadPixelsColor;
                        Framebuffer::ReadPixelsDepthFn      = ES30Framebuffer::ReadPixelsDepth;
                        Framebuffer::ReadPixelsStencilFn    = ES30Framebuffer::ReadPixelsStencil;
                        Framebuffer::BlitFn                 = ES30Framebuffer::Blit;

                        break;
                    }
                }
            }
        }
    }
}
