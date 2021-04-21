#pragma once

#include <chrono>

#include "ErrorReporter.hpp"

#include "hl/api/Framebuffer.hpp"

#include "hl/gl45/Core.hpp"

namespace Shroon
{
    namespace Prism
    {
        namespace HL
        {
            class GL45Framebuffer
            {
            public:
                static void Create(Framebuffer & fb, FramebufferSpecification & spec);
                static void Destroy(Framebuffer & fb);

                static void Resize(Framebuffer & fb, uint32_t width, uint32_t height);

                static void ReadPixelsColor(Framebuffer & fb, uint32_t colorAttachmentIndex, uint32_t x, uint32_t y, uint32_t width, uint32_t height, void * data);
                static void ReadPixelsDepth(Framebuffer & fb, uint32_t x, uint32_t y, uint32_t width, uint32_t height, void * data);
                static void ReadPixelsStencil(Framebuffer & fb, uint32_t x, uint32_t y, uint32_t width, uint32_t height, void * data);
                static void Blit(Framebuffer & fb, Handle dstFB, uint32_t srcX1, uint32_t srcY1, uint32_t srcX2, uint32_t srcY2, uint32_t dstX1, uint32_t dstY1, uint32_t dstX2, uint32_t dstY2);
            };
        }
    }
}
