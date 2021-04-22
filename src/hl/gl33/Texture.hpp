#pragma once

#include "hl/API.hpp"
#include "hl/api/Texture.hpp"

#include "hl/gl33/Core.hpp"

namespace Shroon
{
    namespace Prism
    {
        namespace HL
        {
            class GL33Texture
            {
            public:
                static void Create(Texture & tex, TextureSpecification & spec);
                static void Destroy(Texture & tex);
            };
        }
    }
}
