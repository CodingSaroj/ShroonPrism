#include "Texture.hpp"

namespace Shroon
{
    namespace Prism
    {
        void VK12Texture::Create(Texture & tex, TextureSpecification spec)
        {
            Buffer stagingBuffer;

            {
                BufferSpecification bufSpec;
                bufSpec.Type = BufferType::TransferSrc;
                bufSpec.Data = spec.Data;
                bufSpec.Size = spec.Width * spec.Height * 4;

                stagingBuffer.Create(bufSpec);
            }

            VkImageCreateInfo imageInfo{};
            imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
            imageInfo.imageType = static_cast<VkImageType>(spec.Type.Type);
            imageInfo.extent.width = spec.Width;
            imageInfo.extent.height = spec.Height;
            imageInfo.extent.depth = 1;
            imageInfo.format = static_cast<VkFormat>(spec.Format.Type);
            imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
            imageInfo.tiling = VK_IMAGE_TILING_LINEAR;
            imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT
                                | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
                                | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT 
                                | VK_IMAGE_USAGE_SAMPLED_BIT;
            imageInfo.arrayLayers = 1;
            imageInfo.mipLevels = 1;
            imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            imageInfo.queueFamilyIndexCount = 1;
            imageInfo.pQueueFamilyIndices = &VK12State::s_Indices.GraphicsFamily.value();
            imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

            if (vkCreateImage(VK12State::s_Device, &imageInfo, nullptr, tex.m_Handle.AsPtrOf<VkImage>()) != VK_SUCCESS)
            {
                ErrorReporter(SHRN_PRISM_LEVEL_FATAL, "Vulkan1.2", "Failed to create image.");
            }

            VkMemoryRequirements memRequirements;
            vkGetImageMemoryRequirements(VK12State::s_Device, tex.m_Handle.As<VkImage>(), &memRequirements);

            VkMemoryAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocInfo.allocationSize = memRequirements.size;
            allocInfo.memoryTypeIndex = 0;

            if (vkAllocateMemory(VK12State::s_Device, &allocInfo, nullptr, tex.m_MemoryHandle.AsPtrOf<VkDeviceMemory>()) != VK_SUCCESS)
            {
                ErrorReporter(SHRN_PRISM_LEVEL_FATAL, "Vulkan1.2", "Failed to allocate memory for image.");
            }

            vkBindImageMemory(VK12State::s_Device, tex.m_Handle.As<VkImage>(), tex.m_MemoryHandle.As<VkDeviceMemory>(), 0);

            stagingBuffer.Destroy();
        }

        void VK12Texture::Destroy(Texture & tex)
        {
            vkFreeMemory(VK12State::s_Device, tex.m_MemoryHandle.As<VkDeviceMemory>(), nullptr);
            vkDestroyImage(VK12State::s_Device, tex.m_Handle.As<VkImage>(), nullptr);
        }
    }
}
