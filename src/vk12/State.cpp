#include "State.hpp"

namespace Shroon
{
    namespace Prism
    {
        static const uint32_t MAX_FRAMES_IN_FLIGHT = 2;

        static const std::vector<const char *> s_ValidationLayers{
            "VK_LAYER_KHRONOS_validation"
        };

        static const std::vector<const char *> s_DeviceExtensions{
            "VK_KHR_swapchain"
        };

        GLFWwindow * VK12State::s_Window;

        VkInstance VK12State::s_Instance;
        VkSurfaceKHR VK12State::s_Surface;
        VkPhysicalDevice VK12State::s_PhysicalDevice;
        VkDevice VK12State::s_Device;
        VkSwapchainKHR VK12State::s_Swapchain;
        VkFormat VK12State::s_SwapFormat;
        VkExtent2D VK12State::s_SwapExtent;

        VkQueue VK12State::s_GraphicsQueue;
        VkQueue VK12State::s_PresentQueue;

        VkRenderPass VK12State::s_RenderPass;

        std::vector<VkImage> VK12State::s_Images;
        std::vector<VkImageView> VK12State::s_ImageViews;
        std::vector<VkFramebuffer> VK12State::s_Framebuffers;

        VkCommandPool VK12State::s_CommandPool;
        std::vector<VkCommandBuffer> VK12State::s_CommandBuffers;

        VK12State::QueueFamilyIndices VK12State::s_Indices;

        std::vector<VkSemaphore> VK12State::s_ImageAvailableSemaphores;
        std::vector<VkSemaphore> VK12State::s_RenderFinishedSemaphores;
        std::vector<VkFence> VK12State::s_InFlighFences;
        std::vector<VkFence> VK12State::s_ImagesInFlight;
        size_t VK12State::s_CurrentFrame;

        Framebuffer VK12State::s_UserFramebuffer;

        void VK12State::CreateInstance()
        {
            VkApplicationInfo appInfo{};
            appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            appInfo.pApplicationName = "";
            appInfo.applicationVersion = VK_MAKE_VERSION(1, 2, 0);
            appInfo.pEngineName = "No Engine";
            appInfo.engineVersion = VK_MAKE_VERSION(1, 2, 0);
            appInfo.apiVersion = VK_API_VERSION_1_2;

            VkInstanceCreateInfo instanceInfo{};
            instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            instanceInfo.pApplicationInfo = &appInfo;

            uint32_t glfwExtensionCount = 0;
            const char ** glfwExtensions = nullptr;

            glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

            instanceInfo.enabledExtensionCount = glfwExtensionCount;
            instanceInfo.ppEnabledExtensionNames = glfwExtensions;

        #ifndef NDEBUG
            instanceInfo.enabledLayerCount = s_ValidationLayers.size();
            instanceInfo.ppEnabledLayerNames = s_ValidationLayers.data();
        #else
            instanceInfo.enabledLayerCount = 0;
        #endif

            if (vkCreateInstance(&instanceInfo, nullptr, &s_Instance) != VK_SUCCESS)
            {
                ErrorReporter(SHRN_PRISM_LEVEL_FATAL, "Vulkan1.2", "Failed to create instance.");
            }
        }

        void VK12State::CreateSurface()
        {
            if (glfwCreateWindowSurface(s_Instance, s_Window, nullptr, &s_Surface) != VK_SUCCESS)
            {
                ErrorReporter(SHRN_PRISM_LEVEL_FATAL, "Vulkan1.2", "Failed to create surface.");
            }
        }

        void VK12State::InitializePhysicalDevice()
        {
            uint32_t physicalDeviceCount = 0;
            vkEnumeratePhysicalDevices(s_Instance, &physicalDeviceCount, nullptr);

            if (physicalDeviceCount == 0 )
            {
                ErrorReporter(SHRN_PRISM_LEVEL_FATAL, "Vulkan1.2", "Failed to find GPU(s) with vulkan support.");
            }

            std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);

            vkEnumeratePhysicalDevices(s_Instance, &physicalDeviceCount, physicalDevices.data());

            for (const auto & device : physicalDevices)
            {
                s_PhysicalDevice = device;
                break;
            }

            if (s_PhysicalDevice == VK_NULL_HANDLE)
            {
                ErrorReporter(SHRN_PRISM_LEVEL_FATAL, "Vulkan1.2", "Failed to find suitable GPU.");
            }
        }

        void VK12State::CreateLogicalDevice()
        {
            uint32_t queueFamilyCount = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(s_PhysicalDevice, &queueFamilyCount, nullptr);

            std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);

            vkGetPhysicalDeviceQueueFamilyProperties(s_PhysicalDevice, &queueFamilyCount, queueFamilyProperties.data());

            uint32_t i = 0;

            for (const auto & prop : queueFamilyProperties)
            {
                if (prop.queueFlags & VK_QUEUE_GRAPHICS_BIT)
                {
                    s_Indices.GraphicsFamily = i;
                }

                VkBool32 presentSupport = false;

                vkGetPhysicalDeviceSurfaceSupportKHR(s_PhysicalDevice, i, s_Surface, &presentSupport);

                if (presentSupport)
                {
                    s_Indices.PresentFamily = i;
                }

                if (s_Indices.IsComplete())
                {
                    break;
                }

                i++;
            }

            std::vector<VkDeviceQueueCreateInfo> queueInfos;
            std::set<uint32_t> uniqueQueueFamilies{s_Indices.GraphicsFamily.value(), s_Indices.PresentFamily.value()};

            float queuePriority = 1.0f;

            for (uint32_t queueFamily : uniqueQueueFamilies)
            {
                VkDeviceQueueCreateInfo queueInfo;
                queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                queueInfo.queueFamilyIndex = s_Indices.GraphicsFamily.value();
                queueInfo.queueCount = 1;
                queueInfo.pQueuePriorities = &queuePriority;

                queueInfos.emplace_back(queueInfo);
            }

            VkPhysicalDeviceFeatures deviceFeatures{};

            VkDeviceCreateInfo deviceInfo{};
            deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
            deviceInfo.queueCreateInfoCount = queueInfos.size();
            deviceInfo.pQueueCreateInfos = queueInfos.data();

            deviceInfo.pEnabledFeatures = &deviceFeatures;

            deviceInfo.enabledExtensionCount = s_DeviceExtensions.size();
            deviceInfo.ppEnabledExtensionNames = s_DeviceExtensions.data();

        #ifndef NDEBUG
            deviceInfo.enabledLayerCount = s_ValidationLayers.size();
            deviceInfo.ppEnabledLayerNames = s_ValidationLayers.data();
        #else
            deviceInfo.enabledLayerCount = 0;
        #endif

            if (vkCreateDevice(s_PhysicalDevice, &deviceInfo, nullptr, &s_Device) != VK_SUCCESS)
            {
                ErrorReporter(SHRN_PRISM_LEVEL_FATAL, "Vulkan1.2", "Failed to create logical device.");
            }

            vkGetDeviceQueue(s_Device, s_Indices.GraphicsFamily.value(), 0, &s_GraphicsQueue);
            vkGetDeviceQueue(s_Device, s_Indices.PresentFamily.value(), 0, &s_PresentQueue);
        }

        void VK12State::CreateSwapchain()
        {
            SwapchainSupportDetails details;

            uint32_t formatCount = 0;
            vkGetPhysicalDeviceSurfaceFormatsKHR(s_PhysicalDevice, s_Surface, &formatCount, nullptr);

            if (formatCount != 0)
            {
                details.Formats.resize(formatCount);

                vkGetPhysicalDeviceSurfaceFormatsKHR(s_PhysicalDevice, s_Surface, &formatCount, details.Formats.data());
            }

            uint32_t presentModeCount = 0;
            vkGetPhysicalDeviceSurfacePresentModesKHR(s_PhysicalDevice, s_Surface, &presentModeCount, nullptr);

            if (presentModeCount != 0)
            {
                details.Formats.resize(presentModeCount);

                vkGetPhysicalDeviceSurfacePresentModesKHR(s_PhysicalDevice, s_Surface, &presentModeCount, details.PresentModes.data());
            }

            VkSurfaceFormatKHR format;

            {
                bool set = false;

                for (const auto & fmt : details.Formats)
                {
                    if (fmt.format == VK_FORMAT_B8G8R8_SRGB
                        && fmt.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
                    {
                        format = fmt;
                        set = true;
                        break;
                    }
                }

                if (!set)
                {
                    format = details.Formats[0];
                }
            }

            VkPresentModeKHR presentMode;

            {
                bool set = false;

                for (const auto & mode : details.PresentModes)
                {
                    if (mode == VK_PRESENT_MODE_FIFO_KHR)
                    {
                        presentMode = mode;
                        break;
                    }
                }

                if (!set)
                {
                    presentMode = details.PresentModes[0];
                }
            }

            VkExtent2D extent;

            if (details.Capabilities.currentExtent.width != UINT32_MAX)
            {
                extent = details.Capabilities.currentExtent;
            }
            else
            {
                int width, height;

                glfwGetFramebufferSize(s_Window, &width, &height);

                VkExtent2D actualExtent{static_cast<uint32_t>(width), static_cast<uint32_t>(height)};

                actualExtent.width = std::max(details.Capabilities.maxImageExtent.width, std::min(details.Capabilities.maxImageExtent.width, actualExtent.width));
                actualExtent.height = std::max(details.Capabilities.maxImageExtent.height, std::min(details.Capabilities.maxImageExtent.height, actualExtent.height));

                extent = actualExtent;
            }

            uint32_t imageCount = details.Capabilities.minImageCount + 1;

            if (details.Capabilities.maxImageCount > 0
                && imageCount > details.Capabilities.maxImageCount)
            {
                imageCount = details.Capabilities.maxImageCount;
            }

            VkSwapchainCreateInfoKHR swapchainInfo{};
            swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
            swapchainInfo.surface = s_Surface;
            swapchainInfo.minImageCount = imageCount;
            swapchainInfo.imageFormat = format.format;
            swapchainInfo.imageColorSpace = format.colorSpace;
            swapchainInfo.imageExtent = extent;
            swapchainInfo.imageArrayLayers = 1;
            swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

            uint32_t queueFamilyIndices[]{s_Indices.GraphicsFamily.value(), s_Indices.PresentFamily.value()};

            if (s_Indices.GraphicsFamily != s_Indices.PresentFamily)
            {
                swapchainInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
                swapchainInfo.queueFamilyIndexCount = 2;
                swapchainInfo.pQueueFamilyIndices = queueFamilyIndices;
            }
            else
            {
                swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
                swapchainInfo.queueFamilyIndexCount = 0;
                swapchainInfo.pQueueFamilyIndices = nullptr;
            }

            swapchainInfo.preTransform = details.Capabilities.currentTransform;
            swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

            swapchainInfo.presentMode = presentMode;
            swapchainInfo.clipped = VK_TRUE;
            swapchainInfo.oldSwapchain = VK_NULL_HANDLE;

            if (vkCreateSwapchainKHR(s_Device, &swapchainInfo, nullptr, &s_Swapchain) != VK_SUCCESS)
            {
                ErrorReporter(SHRN_PRISM_LEVEL_FATAL, "Vulkan1.2", "Failed to create swapchain.");
            }

            imageCount = 0;
            vkGetSwapchainImagesKHR(s_Device, s_Swapchain, &imageCount, nullptr);

            s_Images.resize(imageCount);

            vkGetSwapchainImagesKHR(s_Device, s_Swapchain, &imageCount, s_Images.data());

            s_SwapFormat = format.format;
            s_SwapExtent = extent;
        }

        void VK12State::CreateImageViews()
        {
            s_ImageViews.resize(s_Images.size());

            for (size_t i = 0; i < s_Images.size(); i++)
            {
                VkImageViewCreateInfo imageViewInfo{};
                imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
                imageViewInfo.image = s_Images[i];

                imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
                imageViewInfo.format = s_SwapFormat;

                imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
                imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
                imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
                imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

                imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                imageViewInfo.subresourceRange.baseArrayLayer = 0;
                imageViewInfo.subresourceRange.layerCount = 1;
                imageViewInfo.subresourceRange.baseMipLevel = 0;
                imageViewInfo.subresourceRange.levelCount = 1;

                if (vkCreateImageView(s_Device, &imageViewInfo, nullptr, &s_ImageViews[i]) != VK_SUCCESS)
                {
                    ErrorReporter(SHRN_PRISM_LEVEL_FATAL, "Vulkan1.2", "Failed to create image view.");
                }
            }
        }

        void VK12State::CreateRenderPass()
        {
            VkAttachmentDescription colorAttachment{};
            colorAttachment.format = s_SwapFormat;
            colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
            colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

            VkAttachmentReference colorAttachmentRef{};
            colorAttachmentRef.attachment = 0;
            colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

            VkSubpassDescription subpass{};
            subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
            subpass.colorAttachmentCount = 1;
            subpass.pColorAttachments = &colorAttachmentRef;

            VkRenderPassCreateInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
            renderPassInfo.attachmentCount = 1;
            renderPassInfo.pAttachments = &colorAttachment;
            renderPassInfo.subpassCount = 1;
            renderPassInfo.pSubpasses = &subpass;
 
            VkSubpassDependency dependency{};
            dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
            dependency.dstSubpass = 0;
            dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            dependency.srcAccessMask = 0;
            dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

            renderPassInfo.dependencyCount = 1;
            renderPassInfo.pDependencies = &dependency;

            if (vkCreateRenderPass(s_Device, &renderPassInfo, nullptr, &s_RenderPass) != VK_SUCCESS)
            {
                ErrorReporter(SHRN_PRISM_LEVEL_FATAL, "Vulkan1.2", "Failed to create render pass!");
            }
        }

        void VK12State::CreateFramebuffers()
        {
            s_Framebuffers.resize(s_ImageViews.size());

            for (uint32_t i = 0; i < s_ImageViews.size(); i++)
            {
                VkImageView * attachments = &s_ImageViews[i];

                VkFramebufferCreateInfo framebufferInfo{};
                framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
                framebufferInfo.renderPass = s_RenderPass;
                framebufferInfo.attachmentCount = 1;
                framebufferInfo.pAttachments = attachments;
                framebufferInfo.width = s_SwapExtent.width;
                framebufferInfo.height = s_SwapExtent.height;
                framebufferInfo.layers = 1;

                if (vkCreateFramebuffer(s_Device, &framebufferInfo, nullptr, &s_Framebuffers[i]) != VK_SUCCESS)
                {
                    ErrorReporter(SHRN_PRISM_LEVEL_FATAL, "Vulkan1.2", "Failed to create swapchain framebuffer.");
                }
            }
        }

        void VK12State::CreateCommandPool()
        {
            VkCommandPoolCreateInfo poolInfo{};
            poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            poolInfo.queueFamilyIndex = s_Indices.GraphicsFamily.value();
            poolInfo.flags = 0;

            if (vkCreateCommandPool(s_Device, &poolInfo, nullptr, &s_CommandPool) != VK_SUCCESS)
            {
                ErrorReporter(SHRN_PRISM_LEVEL_FATAL, "Vulkan1.2", "Failed to create command pool.");
            }
        }

        void VK12State::CreateCommandBuffers()
        {
            s_CommandBuffers.resize(s_Framebuffers.size());

            VkCommandBufferAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocInfo.commandPool = s_CommandPool;
            allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            allocInfo.commandBufferCount = s_CommandBuffers.size();
            
            if (vkAllocateCommandBuffers(s_Device, &allocInfo, s_CommandBuffers.data()) != VK_SUCCESS)
            {
                ErrorReporter(SHRN_PRISM_LEVEL_FATAL, "Vulkan1.2", "Failed to allocate command buffer.");
            }
            
            for (size_t i = 0; i < s_CommandBuffers.size(); i++)
            {
                VkCommandBufferBeginInfo beginInfo{};
                beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
                beginInfo.flags = 0;
                beginInfo.pInheritanceInfo = nullptr;
            
                if (vkBeginCommandBuffer(s_CommandBuffers[i], &beginInfo) != VK_SUCCESS)
                {
                    ErrorReporter(SHRN_PRISM_LEVEL_FATAL, "Vulkan1.2", "Failed to begin recording command buffer.");
                }

                VkRenderPassBeginInfo renderPassInfo{};
                renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
                renderPassInfo.renderPass = s_RenderPass;
                renderPassInfo.framebuffer = s_Framebuffers[i];
                renderPassInfo.renderArea.offset = {0, 0};
                renderPassInfo.renderArea.extent = s_SwapExtent;

                VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
                renderPassInfo.clearValueCount = 1;
                renderPassInfo.pClearValues = &clearColor;

                vkCmdBeginRenderPass(s_CommandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

                VkImageBlit blit;
                blit.srcOffsets->x = 0;
                blit.srcOffsets->y = 0;
                blit.srcOffsets->z = 0;
                blit.dstOffsets->x = 0;
                blit.dstOffsets->y = 0;
                blit.dstOffsets->z = 0;

                vkCmdBlitImage(
                    s_CommandBuffers[i],
                    s_UserFramebuffer.GetColorAttachment().GetRaw().As<VkImage>(),
                    VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                    s_Images[i],
                    VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                    1, &blit, VK_FILTER_NEAREST
                );

                vkCmdEndRenderPass(s_CommandBuffers[i]);

                if (vkEndCommandBuffer(s_CommandBuffers[i]) != VK_SUCCESS)
                {
                    ErrorReporter(SHRN_PRISM_LEVEL_FATAL, "Vulkan1.2", "Failed to record command buffer.");
                }
            }
        }

        void VK12State::CreateSyncObjects()
        {
            s_ImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
            s_RenderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
            s_InFlighFences.resize(MAX_FRAMES_IN_FLIGHT);
            s_ImagesInFlight.resize(s_Images.size(), VK_NULL_HANDLE);

            VkSemaphoreCreateInfo semaphoreInfo{};
            semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

            VkFenceCreateInfo fenceInfo{};
            fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
            fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

            for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
            {
                if (vkCreateSemaphore(s_Device, &semaphoreInfo, nullptr, &s_ImageAvailableSemaphores[i]) != VK_SUCCESS
                    || vkCreateSemaphore(s_Device, &semaphoreInfo, nullptr, &s_RenderFinishedSemaphores[i]) != VK_SUCCESS
                    || vkCreateFence(s_Device, &fenceInfo, nullptr, &s_InFlighFences[i]) != VK_SUCCESS)
                {
                    ErrorReporter(SHRN_PRISM_LEVEL_FATAL, "Vulkan1.2", "Failed to create synchronization objects.");
                }
            }
        }

        void VK12State::Initialize()
        {
            CreateInstance();
            CreateSurface();
            InitializePhysicalDevice();
            CreateLogicalDevice();
            CreateSwapchain();
            CreateImageViews();
            CreateRenderPass();
            CreateFramebuffers();
            CreateCommandPool();
            CreateCommandBuffers();
            CreateSyncObjects();
        }

        void VK12State::Cleanup()
        {
            for (VkCommandBuffer commandBuffer : s_CommandBuffers)
            {
                vkFreeCommandBuffers(s_Device, s_CommandPool, 1, &commandBuffer);
            }

            vkDestroyCommandPool(s_Device, s_CommandPool, nullptr);

            for (auto imgView : s_ImageViews)
            {
                vkDestroyImageView(s_Device, imgView, nullptr);
            }

            vkDestroySwapchainKHR(s_Device, s_Swapchain, nullptr);
            vkDestroyDevice(s_Device, nullptr);
            vkDestroySurfaceKHR(s_Instance, s_Surface, nullptr);
            vkDestroyInstance(s_Instance, nullptr);
        }

        void VK12State::Run(Framebuffer fb)
        {
            if (fb.GetRaw() != s_UserFramebuffer.GetRaw())
            {
                s_UserFramebuffer = fb;

                for (VkCommandBuffer commandBuffer : s_CommandBuffers)
                {
                    vkFreeCommandBuffers(s_Device, s_CommandPool, 1, &commandBuffer);
                }

                CreateCommandBuffers();
            }

            vkWaitForFences(s_Device, 1, &s_InFlighFences[s_CurrentFrame], VK_TRUE, UINT64_MAX);

            uint32_t imageIndex = 0;

            VkResult result = vkAcquireNextImageKHR(s_Device, s_Swapchain, UINT64_MAX, s_ImageAvailableSemaphores[s_CurrentFrame], VK_NULL_HANDLE, &imageIndex);

            if (result == VK_ERROR_OUT_OF_DATE_KHR)
            {
                return;
            }
            else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
            {
                ErrorReporter(SHRN_PRISM_LEVEL_FATAL, "Vulkan1.2", "Failed to acquire swapchain image.");
            }

            if (s_ImagesInFlight[imageIndex] != VK_NULL_HANDLE)
            {
                vkWaitForFences(s_Device, 1, &s_InFlighFences[imageIndex], VK_TRUE, UINT64_MAX);
            }

            s_ImagesInFlight[imageIndex] = s_InFlighFences[s_CurrentFrame];

            VkSubmitInfo submitInfo{};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

            VkSemaphore * waitSemaphores = &s_ImageAvailableSemaphores[s_CurrentFrame];
            VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
            submitInfo.waitSemaphoreCount = 1;
            submitInfo.pWaitSemaphores = waitSemaphores;
            submitInfo.pWaitDstStageMask = waitStages;
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &s_CommandBuffers[imageIndex];

            VkSemaphore signalSemaphores[] = {s_RenderFinishedSemaphores[s_CurrentFrame]};

            submitInfo.signalSemaphoreCount = 1;
            submitInfo.pSignalSemaphores = signalSemaphores;

            vkResetFences(s_Device, 1, &s_InFlighFences[s_CurrentFrame]);

            if (vkQueueSubmit(s_GraphicsQueue, 1, &submitInfo, s_InFlighFences[s_CurrentFrame]) != VK_SUCCESS)
            {
                ErrorReporter(SHRN_PRISM_LEVEL_FATAL, "Vulkan1.2", "Failed to submit draw command buffer.");
            }

            VkPresentInfoKHR presentInfo{};
            presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
            presentInfo.waitSemaphoreCount = 1;
            presentInfo.pWaitSemaphores = signalSemaphores;

            VkSwapchainKHR swapchains[] = {s_Swapchain};

            presentInfo.swapchainCount = 1;
            presentInfo.pSwapchains = swapchains;
            presentInfo.pImageIndices = &imageIndex;

            result = vkQueuePresentKHR(s_GraphicsQueue, &presentInfo);

            if (result == VK_ERROR_OUT_OF_DATE_KHR)
            {
                return;
            }
            else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
            {
                ErrorReporter(SHRN_PRISM_LEVEL_FATAL, "Vulkan1.2", "Failed to acquire swapchain image.");
            }

            vkQueueWaitIdle(s_GraphicsQueue);

            s_CurrentFrame = (s_CurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
        }
    }
}
