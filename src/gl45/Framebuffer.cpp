#include "Framebuffer.hpp"

namespace Shroon
{
    namespace Prism
    {
        void GL45Framebuffer::Create(Framebuffer & fb, FramebufferSpecification spec)
        {
            fb.m_Spec = spec;

            glGenFramebuffers(1, reinterpret_cast<GLuint *>(&fb.m_Handle));
            glBindFramebuffer(GL_FRAMEBUFFER, fb.m_Handle);

            fb.m_ColorAttachments.resize(spec.ColorAttachments.size());

            for (size_t i = 0; i < fb.m_ColorAttachments.size(); i++)
            {
                fb.m_ColorAttachments[i].Create(spec.ColorAttachments[i]);

                glFramebufferTexture2D(
                    GL_FRAMEBUFFER,
                    GL_COLOR_ATTACHMENT0 + i,
                    spec.ColorAttachments[i].Type,
                    fb.m_ColorAttachments[i].GetRaw(),
                    0
                );
            }

            if (spec.Depth || spec.Stencil)
            {
                fb.m_DepthStencilAttachment.Create(spec.DepthStencilAttachment);

                glFramebufferTexture2D(
                    GL_FRAMEBUFFER,
                    (spec.Depth && spec.Stencil) ? GL_DEPTH_STENCIL_ATTACHMENT : (spec.Depth ? GL_DEPTH_ATTACHMENT : GL_STENCIL_ATTACHMENT),
                    spec.DepthStencilAttachment.Type,
                    fb.m_DepthStencilAttachment.GetRaw(),
                    0
                );
            }

            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            {
                ErrorReporter(SHRN_PRISM_LEVEL_ERROR, "OpenGL4.5::Framebuffer", "Framebuffer incomplete!");
            }

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        void GL45Framebuffer::Destroy(Framebuffer & fb)
        {
            for (auto & attachment : fb.m_ColorAttachments)
            {
                attachment.Destroy();
            }

            if (fb.m_Spec.Depth || fb.m_Spec.Stencil)
            {
                fb.m_DepthStencilAttachment.Destroy();
            }

            glDeleteFramebuffers(1, reinterpret_cast<GLuint *>(&fb.m_Handle));
        }

        void GL45Framebuffer::Resize(Framebuffer & fb, uint32_t width, uint32_t height)
        {
            for (auto & attachment : fb.m_ColorAttachments)
            {
                auto spec = attachment.GetSpec();
                spec.Width = width;
                spec.Height = height;

                attachment.Destroy();
                attachment.Create(spec);
            }

            if (fb.m_Spec.Depth || fb.m_Spec.Stencil)
            {
                auto spec = fb.m_DepthStencilAttachment.GetSpec();
                spec.Width = width;
                spec.Height = height;

                fb.m_DepthStencilAttachment.Destroy();
                fb.m_DepthStencilAttachment.Create(spec);
            }

            fb.m_Spec.Width = width;
            fb.m_Spec.Height = height;
        }

        void GL45Framebuffer::Enable(Framebuffer & fb)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, fb.m_Handle);
            glViewport(0, 0, fb.m_Spec.Width, fb.m_Spec.Height);
        }

        void GL45Framebuffer::Disable(Framebuffer & fb)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        void GL45Framebuffer::SetClearColor(Framebuffer & fb, float r, float g, float b, float a)
        {
            glClearColor(r, g, b, a);
        }

        void GL45Framebuffer::SetClearDepth(Framebuffer & fb, float depth)
        {
            glClearDepth(depth);
        }

        void GL45Framebuffer::SetClearStencil(Framebuffer & fb, uint32_t stencil)
        {
            glClearStencil(stencil);
        }

        void GL45Framebuffer::ClearBuffers(Framebuffer & fb)
        {
            glClear(GL_COLOR_BUFFER_BIT | (fb.m_Spec.Depth ? GL_DEPTH_BUFFER_BIT : 0) | (fb.m_Spec.Stencil ? GL_STENCIL_BUFFER_BIT : 0));
        }

        void GL45Framebuffer::ReadPixelsColor(Framebuffer & fb, uint32_t colorAttachmentIndex, uint32_t x, uint32_t y, uint32_t width, uint32_t height, void * data)
        {
            Texture & color = fb.m_ColorAttachments[colorAttachmentIndex];

            glBindFramebuffer(GL_FRAMEBUFFER, fb.m_Handle);
            glBindTexture(color.GetType(), color.GetRaw());
            glReadPixels(x, y, width, height, color.GetFormat(), GL_UNSIGNED_BYTE, data);
            glBindTexture(color.GetType(), 0);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        void GL45Framebuffer::ReadPixelsDepth(Framebuffer & fb, uint32_t x, uint32_t y, uint32_t width, uint32_t height, void * data)
        {
            if (fb.m_Spec.Depth)
            {
                glBindFramebuffer(GL_FRAMEBUFFER, fb.m_Handle);
                glReadPixels(x, y, width, height, GL_DEPTH_COMPONENT, GL_FLOAT, data);
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
            }
        }

        void GL45Framebuffer::ReadPixelsStencil(Framebuffer & fb, uint32_t x, uint32_t y, uint32_t width, uint32_t height, void * data)
        {
            if (fb.m_Spec.Stencil)
            {
                glBindFramebuffer(GL_FRAMEBUFFER, fb.m_Handle);
                glReadPixels(x, y, width, height, GL_STENCIL_INDEX, fb.m_Spec.Depth ? GL_UNSIGNED_BYTE : fb.m_DepthStencilAttachment.GetFormat().Type, data);
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
            }
        }

        void GL45Framebuffer::Blit(Framebuffer & fb, Handle dstFB, uint32_t srcX1, uint32_t srcY1, uint32_t srcX2, uint32_t srcY2, uint32_t dstX1, uint32_t dstY1, uint32_t dstX2, uint32_t dstY2)
        {
            glBindFramebuffer(GL_READ_FRAMEBUFFER, fb.m_Handle);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dstFB);

            glBlitFramebuffer(
                srcX1, srcY1, srcX2, srcY2,
                dstX1, dstY1, dstX2, dstY2,
                GL_COLOR_BUFFER_BIT | (fb.m_Spec.Depth ? GL_DEPTH_BUFFER_BIT : 0) | (fb.m_Spec.Stencil ? GL_STENCIL_BUFFER_BIT : 0),
                GL_NEAREST
            );
        }
    }
}
