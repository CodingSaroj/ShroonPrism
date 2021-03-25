#pragma once

#include <set>
#include <vector>

#include "ErrorReporter.hpp"

#include "api/Framebuffer.hpp"

#include "vk12/Core.hpp"

namespace Shroon
{
    namespace Prism
    {
        class VK12State
        {
        public:
            static void Initialize();
            static void Cleanup();

            static void Run(Framebuffer fb);

        private:
            struct QueueFamilyIndices
            {
                std::optional<uint32_t> GraphicsFamily;
                std::optional<uint32_t> PresentFamily;

                inline bool IsComplete() { return GraphicsFamily.has_value() && PresentFamily.has_value(); };
            };

            struct SwapchainSupportDetails
            {
                VkSurfaceCapabilitiesKHR Capabilities;
                std::vector<VkSurfaceFormatKHR> Formats;
                std::vector<VkPresentModeKHR> PresentModes;
            };

            static GLFWwindow * s_Window;

            static VkInstance s_Instance;
            static VkSurfaceKHR s_Surface;
            static VkPhysicalDevice s_PhysicalDevice;
            static VkDevice s_Device;
            static VkSwapchainKHR s_Swapchain;
            static VkFormat s_SwapFormat;
            static VkExtent2D s_SwapExtent;

            static VkQueue s_GraphicsQueue;
            static VkQueue s_PresentQueue;

            static VkRenderPass s_RenderPass;

            // Swapchain images and framebuffers.
            static std::vector<VkImage> s_Images;
            static std::vector<VkImageView> s_ImageViews;
            static std::vector<VkFramebuffer> s_Framebuffers;

            static VkCommandPool s_CommandPool;
            static std::vector<VkCommandBuffer> s_CommandBuffers;

            static QueueFamilyIndices s_Indices;

            static std::vector<VkSemaphore> s_ImageAvailableSemaphores;
            static std::vector<VkSemaphore> s_RenderFinishedSemaphores;
            static std::vector<VkFence> s_InFlighFences;
            static std::vector<VkFence> s_ImagesInFlight;
            static size_t s_CurrentFrame;

            static Framebuffer s_UserFramebuffer;

            static void CreateInstance();
            static void CreateSurface();
            static void InitializePhysicalDevice();
            static void CreateLogicalDevice();
            static void CreateSwapchain();
            static void CreateImageViews();
            static void CreateRenderPass();
            static void CreateFramebuffers();
            static void CreateCommandPool();
            static void CreateCommandBuffers();
            static void CreateSyncObjects();

            friend class VK12Buffer;
            friend class VK12Texture;
            friend class VK12Shader;
            friend class VK12Pass;
            friend class VK12Pipeline;
            friend class VK12Framebuffer;
            friend class VK12CommandBuffer;
        };
    }
}
