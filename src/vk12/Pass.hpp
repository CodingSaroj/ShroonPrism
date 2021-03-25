#pragma once

#include <vector>

#include "api/Pass.hpp"

#include "vk12/Core.hpp"
#include "vk12/State.hpp"

namespace Shroon
{
    namespace Prism
    {
        class VK12Pass
        {
        public:
            void Create(Pass & pass, PassSpecification spec);
            void Destroy(Pass & pass);
        };
    }
}
