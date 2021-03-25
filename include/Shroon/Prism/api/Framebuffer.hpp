#pragma once

#include <vector>

#include "API.hpp"
#include "APIManager.hpp"

#include "api/Texture.hpp"
#include "api/Pipeline.hpp"
#include "api/Pass.hpp"

namespace Shroon
{
    namespace Prism
    {
        struct FramebufferSpecification
        {
            bool Depth = true;
            bool Stencil = true;
            uint32_t Width;
            uint32_t Height;
            Pass TargetPass;
            std::vector<TextureSpecification> ColorAttachments;
            TextureSpecification DepthStencilAttachment;
        };

        class Framebuffer
        {
        public:
            inline void Create(FramebufferSpecification spec)
            {
                CreateFn(*this, spec);
            }

            inline void Destroy()
            {
                DestroyFn(*this);
            }

            inline Handle GetRaw()
            {
                return m_Handle;
            }

            inline Texture GetColorAttachment(size_t index = 0)
            {
                return m_ColorAttachments[index];
            }

            inline Texture GetDepthStencilAttachment()
            {
                return m_DepthStencilAttachment;
            }

            inline uint32_t GetWidth()
            {
                return m_Spec.Width;
            }

            inline uint32_t GetHeight()
            {
                return m_Spec.Height;
            }

            inline void Resize(uint32_t width, uint32_t height)
            {
                ResizeFn(*this, width, height);
            }

            inline void Enable()
            {
                EnableFn(*this);
            }

            inline void Disable()
            {
                DisableFn(*this);
            }

            inline void SetClearColor(float r, float g, float b, float a)
            {
                SetClearColorFn(*this, r, g, b, a);
            }

            inline void SetClearDepth(float depth)
            {
                SetClearDepthFn(*this, depth);
            }

            inline void SetClearStencil(uint32_t stencil)
            {
                SetClearStencilFn(*this, stencil);
            }

            inline void ClearBuffers()
            {
                ClearBuffersFn(*this);
            }

            inline void ReadPixelsColor(uint32_t colorAttachmentIndex, uint32_t x, uint32_t y, uint32_t width, uint32_t height, void * data)
            {
                ReadPixelsColorFn(*this, colorAttachmentIndex, x, y, width, height, data);
            }

            inline void ReadPixelsDepth(uint32_t x, uint32_t y, uint32_t width, uint32_t height, void * data)
            {
                ReadPixelsDepthFn(*this, x, y, width, height, data);
            }

            inline void ReadPixelsStencil(uint32_t x, uint32_t y, uint32_t width, uint32_t height, void * data)
            {
                ReadPixelsStencilFn(*this, x, y, width, height, data);
            }

            inline void Blit(Handle dstFB, uint32_t srcX1, uint32_t srcY1, uint32_t srcX2, uint32_t srcY2, uint32_t dstX1, uint32_t dstY1, uint32_t dstX2, uint32_t dstY2)
            {
                BlitFn(*this, dstFB, srcX1, srcY1, srcX2, srcY2, dstX1, dstY1, dstX2, dstY2);
            }

        private:
            Handle m_Handle;
            FramebufferSpecification m_Spec;
            std::vector<Texture> m_ColorAttachments;
            Texture m_DepthStencilAttachment;

            static void( * CreateFn)(Framebuffer &, FramebufferSpecification);
            static void( * DestroyFn)(Framebuffer &);

            static void( * ResizeFn)(Framebuffer &, uint32_t, uint32_t);

            static void( * EnableFn)(Framebuffer &);
            static void( * DisableFn)(Framebuffer &);

            static void( * SetClearColorFn)(Framebuffer &, float, float, float, float);
            static void( * SetClearDepthFn)(Framebuffer &, float);
            static void( * SetClearStencilFn)(Framebuffer &, uint32_t);
            static void( * ClearBuffersFn)(Framebuffer &);

            static void( * ReadPixelsColorFn)(Framebuffer &, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, void *);
            static void( * ReadPixelsDepthFn)(Framebuffer &, uint32_t, uint32_t, uint32_t, uint32_t, void *);
            static void( * ReadPixelsStencilFn)(Framebuffer &, uint32_t, uint32_t, uint32_t, uint32_t, void *);
            static void( * BlitFn)(Framebuffer &, Handle, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);

            friend class APIManager;

            friend class GL33Framebuffer;
            friend class GL45Framebuffer;
            friend class VK12Framebuffer;
        };
    }
}
