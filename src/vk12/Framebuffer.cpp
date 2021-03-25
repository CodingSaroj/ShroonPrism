#include "Framebuffer.hpp"

namespace Shroon
{
    namespace Prism
    {
        void VK12Framebuffer::Create(Framebuffer & fb, FramebufferSpecification spec)
        {
            std::vector<VkImageView> attachments;

            for (auto & texSpec : spec.ColorAttachments)
            {
                Texture texture;
                texture.Create(texSpec);

                fb.m_ColorAttachments.emplace_back(texture);
                attachments.emplace_back(texture.GetRaw().As<VkImageView>());
            }

            if (spec.Depth || spec.Stencil)
            {
                Texture texture;
                texture.Create(spec.DepthStencilAttachment);

                fb.m_DepthStencilAttachment = texture;
                attachments.emplace_back(texture.GetRaw().As<VkImageView>());
            }

            VkFramebufferCreateInfo fbInfo{};
            fbInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            fbInfo.renderPass = spec.TargetPass.GetRaw().As<VkRenderPass>();
            fbInfo.attachmentCount = attachments.size();
            fbInfo.pAttachments = attachments.data();
            fbInfo.width = spec.Width;
            fbInfo.height = spec.Height;
            fbInfo.layers = 1;

            if (vkCreateFramebuffer(VK12State::s_Device, &fbInfo, nullptr, fb.m_Handle.AsPtrOf<VkFramebuffer>()) != VK_SUCCESS)
            {
                ErrorReporter(SHRN_PRISM_LEVEL_FATAL, "Vulkan1.2", "Failed to create framebuffer.");
            }
        }

        void VK12Framebuffer::Destroy(Framebuffer & fb)
        {
            vkDestroyFramebuffer(VK12State::s_Device, fb.m_Handle.As<VkFramebuffer>(), nullptr);

            for (auto & attachment : fb.m_ColorAttachments)
            {
                attachment.Destroy();
            }

            fb.m_DepthStencilAttachment.Destroy();
        }

        void VK12Framebuffer::Resize(Framebuffer & fb, uint32_t width, uint32_t height)
        {
            Destroy(fb);

            fb.m_Spec.Width = width;
            fb.m_Spec.Height = height;

            Create(fb, fb.m_Spec);
        }
    }
}
