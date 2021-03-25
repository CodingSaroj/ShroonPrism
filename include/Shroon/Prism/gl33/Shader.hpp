#pragma once

#include "api/Shader.hpp"

#include "gl45/Core.hpp"

#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

namespace Shroon
{
    namespace Prism
    {
        class GL33Shader
        {
        public:
            static void Create(Shader & shader, ShaderSpecification spec);
            static void Destroy(Shader & shader);

            static void BindUniformBlock(Shader & shader, std::string name, uint32_t bindingPoint);
        };
    }
}
