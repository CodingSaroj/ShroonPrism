#include "Buffer.hpp"

namespace Shroon
{
    namespace Prism
    {
        void VK12Buffer::Create(Buffer & buf, BufferSpecification spec)
        {
            VkBuffer stagingBuffer;
            VkDeviceMemory stagingBufferMemory;

            {
                VkBufferCreateInfo bufferInfo{};
                bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
                bufferInfo.size = spec.Size;
                bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
                bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

                if (vkCreateBuffer(VK12State::s_Device, &bufferInfo, nullptr, &stagingBuffer) != VK_SUCCESS)
                {
                    ErrorReporter(SHRN_PRISM_LEVEL_FATAL, "Vulkan1.2", "Failed to create buffer.");
                }

                VkMemoryRequirements memRequirements;

                vkGetBufferMemoryRequirements(VK12State::s_Device, stagingBuffer, &memRequirements);

                VkMemoryAllocateInfo allocInfo{};
                allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
                allocInfo.allocationSize = memRequirements.size;
                allocInfo.memoryTypeIndex = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

                if (vkAllocateMemory(VK12State::s_Device, &allocInfo, nullptr, &stagingBufferMemory) != VK_SUCCESS)
                {
                    ErrorReporter(SHRN_PRISM_LEVEL_FATAL, "Vulkan1.2", "Failed to allocate memory.");
                }

                vkBindBufferMemory(VK12State::s_Device, stagingBuffer, stagingBufferMemory, 0);

                void * memory;

                vkMapMemory(VK12State::s_Device, stagingBufferMemory, 0, spec.Size, 0, &memory);

                memcpy(memory, spec.Data, spec.Size);

                vkUnmapMemory(VK12State::s_Device, stagingBufferMemory);
            }

            VkBufferCreateInfo bufferInfo{};
            bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferInfo.size = spec.Size;
            bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | spec.Type;
            bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

            if (vkCreateBuffer(VK12State::s_Device, &bufferInfo, nullptr, buf.m_Handle.AsPtrOf<VkBuffer>()) != VK_SUCCESS)
            {
                ErrorReporter(SHRN_PRISM_LEVEL_FATAL, "Vulkan1.2", "Failed to create buffer.");
            }

            VkMemoryRequirements memRequirements;

            vkGetBufferMemoryRequirements(VK12State::s_Device, buf.m_Handle.As<VkBuffer>(), &memRequirements);

            VkMemoryAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocInfo.allocationSize = memRequirements.size;
            allocInfo.memoryTypeIndex = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

            if (vkAllocateMemory(VK12State::s_Device, &allocInfo, nullptr, buf.m_MemoryHandle.AsPtrOf<VkDeviceMemory>()) != VK_SUCCESS)
            {
                ErrorReporter(SHRN_PRISM_LEVEL_FATAL, "Vulkan1.2", "Failed to allocate memory.");
            }

            vkBindBufferMemory(VK12State::s_Device, buf.m_Handle.As<VkBuffer>(), buf.m_MemoryHandle.As<VkDeviceMemory>(), 0);

            vkFreeMemory(VK12State::s_Device, stagingBufferMemory, nullptr);
            vkDestroyBuffer(VK12State::s_Device, stagingBuffer, nullptr);
        }

        void VK12Buffer::Destroy(Buffer & buf)
        {
            vkFreeMemory(VK12State::s_Device, buf.m_MemoryHandle.As<VkDeviceMemory>(), nullptr);
            vkDestroyBuffer(VK12State::s_Device, buf.m_Handle.As<VkBuffer>(), nullptr);
        }

        uint32_t VK12Buffer::GetUniformBlockBindingBoint(Buffer & buf)
        {
            return buf.m_UniformBlockBindingPoint;
        }

        void VK12Buffer::Write(Buffer & buf, void * data, size_t size, size_t offset)
        {
            void * memory;

            vkMapMemory(VK12State::s_Device, buf.m_MemoryHandle.As<VkDeviceMemory>(), offset, size, 0, &memory);

            memcpy(memory, data, offset);

            vkUnmapMemory(VK12State::s_Device, buf.m_MemoryHandle.As<VkDeviceMemory>());
        }

        void VK12Buffer::SetVertexAttrib(Buffer & buf, uint32_t index, VertexAttribType type, size_t componentCount, size_t stride, size_t offset)
        {
        }
    }
}
