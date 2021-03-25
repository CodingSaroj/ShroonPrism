#pragma once

#include "ErrorReporter.hpp"

#include "api/Framebuffer.hpp"

#include "gl33/Core.hpp"

namespace Shroon
{
    namespace Prism
    {
        class GL33Framebuffer
        {
        public:
            static void Create(Framebuffer & fb, FramebufferSpecification spec);
            static void Destroy(Framebuffer & fb);

            static void Resize(Framebuffer & fb, uint32_t width, uint32_t height);

            static void Enable(Framebuffer & fb);
            static void Disable(Framebuffer & fb);

            static void SetClearColor(Framebuffer & fb, float r, float g, float b, float a);
            static void SetClearDepth(Framebuffer & fb, float depth);
            static void SetClearStencil(Framebuffer & fb, uint32_t stencil);
            static void ClearBuffers(Framebuffer & fb);

            static void ReadPixelsColor(Framebuffer & fb, uint32_t colorAttachmentIndex, uint32_t x, uint32_t y, uint32_t width, uint32_t height, void * data);
            static void ReadPixelsDepth(Framebuffer & fb, uint32_t x, uint32_t y, uint32_t width, uint32_t height, void * data);
            static void ReadPixelsStencil(Framebuffer & fb, uint32_t x, uint32_t y, uint32_t width, uint32_t height, void * data);
            static void Blit(Framebuffer & fb, Handle dstFB, uint32_t srcX1, uint32_t srcY1, uint32_t srcX2, uint32_t srcY2, uint32_t dstX1, uint32_t dstY1, uint32_t dstX2, uint32_t dstY2);
        };
    }
}
