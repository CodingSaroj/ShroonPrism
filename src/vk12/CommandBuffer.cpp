#include "CommandBuffer.hpp"

namespace Shroon
{
    namespace Prism
    {
        void VK12CommandBuffer::Create(CommandBuffer & cb)
        {
            VkCommandBufferAllocateInfo cbInfo{};
            cbInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            cbInfo.commandPool = VK12State::s_CommandPool;
            cbInfo.commandBufferCount = 1;
            cbInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

            if (vkAllocateCommandBuffers(VK12State::s_Device, &cbInfo, cb.m_Handle.AsPtrOf<VkCommandBuffer>()) != VK_SUCCESS)
            {
                ErrorReporter(SHRN_PRISM_LEVEL_FATAL, "Vulkan1.2", "Failed to allocate command buffer.");
            }
        }

        void VK12CommandBuffer::Destroy(CommandBuffer & cb)
        {
            vkFreeCommandBuffers(VK12State::s_Device, VK12State::s_CommandPool, 1, cb.m_Handle.AsPtrOf<VkCommandBuffer>());
        }

        void VK12CommandBuffer::BeginRecording(CommandBuffer & cb)
        {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            beginInfo.flags = 0;
            beginInfo.pInheritanceInfo = nullptr;

            vkBeginCommandBuffer(cb.m_Handle.As<VkCommandBuffer>(), &beginInfo);
        }

        void VK12CommandBuffer::EndRecording(CommandBuffer & cb)
        {
            if (vkEndCommandBuffer(cb.m_Handle.As<VkCommandBuffer>()) != VK_SUCCESS)
            {
                ErrorReporter(SHRN_PRISM_LEVEL_FATAL, "Vulkan1.2", "Failed to record command buffer.");
            }
        }

        void VK12CommandBuffer::CmdBeginPass(CommandBuffer & cb, Pass pass, Framebuffer fb, float clearR, float clearG, float clearB, float clearA)
        {
            VkRenderPassBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            beginInfo.renderPass = pass.GetRaw().As<VkRenderPass>();
            beginInfo.framebuffer = fb.GetRaw().As<VkFramebuffer>();
            beginInfo.renderArea.offset = {0, 0};
            beginInfo.renderArea.extent.width = fb.GetWidth();
            beginInfo.renderArea.extent.height = fb.GetHeight();

            VkClearValue clearColor = {clearR, clearG, clearB, clearA};

            beginInfo.clearValueCount = 1;
            beginInfo.pClearValues = &clearColor;

            vkCmdBeginRenderPass(cb.m_Handle.As<VkCommandBuffer>(), &beginInfo, VK_SUBPASS_CONTENTS_INLINE);
        }

        void VK12CommandBuffer::CmdEndPass(CommandBuffer & cb)
        {
            vkCmdEndRenderPass(cb.m_Handle.As<VkCommandBuffer>());
        }

        void VK12CommandBuffer::CmdBindPipeline(CommandBuffer & cb, Pipeline pl)
        {
            vkCmdBindPipeline(cb.m_Handle.As<VkCommandBuffer>(), VK_PIPELINE_BIND_POINT_GRAPHICS, pl.GetRaw().As<VkPipeline>());
        }

        void VK12CommandBuffer::CmdBindMesh(CommandBuffer & cb, Mesh mesh)
        {
            std::vector<VkBuffer> vertexBuffers;
            vertexBuffers.reserve(mesh.m_Buffers.size());

            for (auto & buffer : mesh.m_Buffers)
            {
                if (buffer.GetType() == BufferType::VertexArray)
                {
                    vertexBuffers.emplace_back(buffer.GetRaw().As<VkBuffer>());
                }
                else
                {
                    vkCmdBindIndexBuffer(cb.m_Handle.As<VkCommandBuffer>(), buffer.GetRaw().As<VkBuffer>(), 0, VK_INDEX_TYPE_UINT32);
                }
            }

            VkDeviceSize offsets[] = {0};

            vkCmdBindVertexBuffers(cb.m_Handle.As<VkCommandBuffer>(), 0, 1, vertexBuffers.data(), offsets);
        }

        void VK12CommandBuffer::CmdCopyBuffer(CommandBuffer & cb, Buffer src, uint32_t srcOffset, Buffer dst, uint32_t dstOffset, uint32_t size)
        {
            VkBufferCopy copyInfo{};
            copyInfo.srcOffset = srcOffset;
            copyInfo.dstOffset = dstOffset;
            copyInfo.size = size;

            vkCmdCopyBuffer(cb.m_Handle.As<VkCommandBuffer>(), src.GetRaw().As<VkBuffer>(), dst.GetRaw().As<VkBuffer>(), 1, &copyInfo);
        }

        void VK12CommandBuffer::CmdDrawFn(CommandBuffer & cb, uint32_t vtxCount, uint32_t vtxStart, uint32_t instanceCount, uint32_t instanceStart)
        {
            vkCmdDraw(cb.m_Handle.As<VkCommandBuffer>(), vtxCount, instanceCount, vtxStart, instanceStart);
        }

        void VK12CommandBuffer::CmdDrawIndexedFn(CommandBuffer & cb, uint32_t vtxCount, uint32_t vtxStart, uint32_t instanceCount, uint32_t instanceStart)
        {
            vkCmdDrawIndexed(cb.m_Handle.As<VkCommandBuffer>(), vtxCount, instanceCount, vtxStart, 0, instanceStart);
        }
    }
}
