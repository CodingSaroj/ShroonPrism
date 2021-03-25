#pragma once

#include "API.hpp"
#include "api/Texture.hpp"

#include "gl45/Core.hpp"

namespace Shroon
{
    namespace Prism
    {
        class GL45Texture
        {
        public:
            static void Create(Texture & tex, TextureSpecification spec);
            static void Destroy(Texture & tex);

            static void Use(Texture & tex, uint32_t slot);
        };
    }
}
