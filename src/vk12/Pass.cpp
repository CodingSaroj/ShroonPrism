#include "Pass.hpp"

namespace Shroon
{
    namespace Prism
    {
        void VK12Pass::Create(Pass & pass, PassSpecification spec)
        {
            std::vector<VkAttachmentDescription> colorAttachments;
            std::vector<VkSubpassDescription> subpassDescriptions;
            std::vector<VkSubpassDependency> subpassDeps;

            for (uint32_t i = 0; i < spec.SubpassSpecs.size(); i++)
            {
                VkAttachmentLoadOp loadOp = spec.SubpassSpecs[i].ClearFramebuffer ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD;

                VkSampleCountFlagBits sampleCount;

                switch (spec.SubpassSpecs[i].SampleCount)
                {
                    case 1: { sampleCount = VK_SAMPLE_COUNT_1_BIT; break; }
                    case 2: { sampleCount = VK_SAMPLE_COUNT_1_BIT; break; }
                    case 4: { sampleCount = VK_SAMPLE_COUNT_4_BIT; break; }
                    case 8: { sampleCount = VK_SAMPLE_COUNT_8_BIT; break; }
                    case 16: { sampleCount = VK_SAMPLE_COUNT_16_BIT; break; }
                    case 32: { sampleCount = VK_SAMPLE_COUNT_32_BIT; break; }
                    case 64: { sampleCount = VK_SAMPLE_COUNT_64_BIT; break; }
                }

                VkAttachmentDescription & colorAttachment = colorAttachments.emplace_back();
                colorAttachment.format = VK12State::s_SwapFormat;
                colorAttachment.samples = sampleCount;
                colorAttachment.loadOp = loadOp;
                colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
                colorAttachment.stencilLoadOp = spec.SubpassSpecs[i].UseStencilBuffer ? loadOp : VK_ATTACHMENT_LOAD_OP_DONT_CARE;
                colorAttachment.stencilStoreOp = spec.SubpassSpecs[i].UseStencilBuffer ? VK_ATTACHMENT_STORE_OP_STORE : VK_ATTACHMENT_STORE_OP_DONT_CARE;
                colorAttachment.initialLayout = spec.SubpassSpecs[i].UsePreviousOutput ? VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL : VK_IMAGE_LAYOUT_UNDEFINED;
                colorAttachment.finalLayout = spec.SubpassSpecs[i].Offscreen ? VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL : VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

                VkAttachmentReference colorAttachmentRef{};
                colorAttachmentRef.attachment = 0;
                colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

                VkSubpassDescription & subpassDesc = subpassDescriptions.emplace_back();
                subpassDesc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
                subpassDesc.colorAttachmentCount = 1;
                subpassDesc.pColorAttachments = &colorAttachmentRef;
            }

            VkRenderPassCreateInfo passInfo{};
            passInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
            passInfo.attachmentCount = colorAttachments.size();
            passInfo.pAttachments = colorAttachments.data();
            passInfo.subpassCount = subpassDescriptions.size();
            passInfo.pSubpasses = subpassDescriptions.data();

            for (uint32_t i = 0; i < spec.SubpassSpecs.size(); i++)
            {
                VkSubpassDependency & subpassDep = subpassDeps.emplace_back();
                subpassDep.srcSubpass = i == 0 ? VK_SUBPASS_EXTERNAL : i - 1;
                subpassDep.dstSubpass = i;
                subpassDep.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                subpassDep.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                subpassDep.srcAccessMask = 0;
                subpassDep.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            }

            passInfo.dependencyCount = subpassDeps.size();
            passInfo.pDependencies = subpassDeps.data();

            if (vkCreateRenderPass(VK12State::s_Device, &passInfo, nullptr, pass.m_Handle.AsPtrOf<VkRenderPass>()) != VK_SUCCESS)
            {
                ErrorReporter(SHRN_PRISM_LEVEL_FATAL, "Vulkan1.2", "Failed to create pass.");
            }
        }

        void VK12Pass::Destroy(Pass & pass)
        {
            vkDestroyRenderPass(VK12State::s_Device, pass.m_Handle.As<VkRenderPass>(), nullptr);
        }
    }
}
