#pragma once

#include "api/Framebuffer.hpp"

#include "vk12/Core.hpp"
#include "vk12/State.hpp"
#include "vk12/Texture.hpp"
#include "vk12/Pass.hpp"

namespace Shroon
{
    namespace Prism
    {
        class VK12Framebuffer
        {
        public:
            static void Create(Framebuffer & fb, FramebufferSpecification spec);
            static void Destroy(Framebuffer & fb);

            static void Resize(Framebuffer & fb, uint32_t width, uint32_t height);
        };
    }
}
