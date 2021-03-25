#pragma once

#include "api/UniformManager.hpp"

#include "gl45/Core.hpp"

namespace Shroon
{
    namespace Prism
    {
        class GL45UniformManager
        {
        public:
            static void Destroy();

            static uint32_t GetBindingPoint(const std::string & name);

            static void CreateUniformBlock(const std::string & name, uint32_t size);
            static void UpdateUniformBlock(const std::string & name, void * data, uint32_t size, uint32_t offset);
        };
    }
}
