#pragma once

#include "hl/API.hpp"
#include "hl/api/Texture.hpp"

#include "hl/es30/Core.hpp"

namespace Shroon
{
    namespace Prism
    {
        namespace HL
        {
            class ES30Texture
            {
            public:
                static void Create(Texture & tex, TextureSpecification & spec);
                static void Destroy(Texture & tex);
            };
        }
    }
}
