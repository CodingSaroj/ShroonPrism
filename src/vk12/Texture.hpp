#pragma once

#include "api/Buffer.hpp"
#include "api/Texture.hpp"

#include "vk12/Core.hpp"
#include "vk12/State.hpp"

namespace Shroon
{
    namespace Prism
    {
        class VK12Texture
        {
        public:
            static void Create(Texture & tex, TextureSpecification spec);
            static void Destroy(Texture & tex);
        };
    }
}
