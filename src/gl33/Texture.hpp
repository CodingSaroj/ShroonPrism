#pragma once

#include "API.hpp"
#include "api/Texture.hpp"

#include "gl33/Core.hpp"

namespace Shroon
{
    namespace Prism
    {
        class GL33Texture
        {
        public:
            static void Create(Texture & tex, TextureSpecification spec);
            static void Destroy(Texture & tex);

            static void Use(Texture & tex, uint32_t slot);
        };
    }
}
