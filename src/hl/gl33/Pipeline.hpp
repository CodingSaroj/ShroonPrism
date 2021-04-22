#pragma once

#include <cstring>

#include "ErrorReporter.hpp"

#include "hl/api/Pipeline.hpp"

#include "hl/gl33/Core.hpp"

#include <spirv_cross/spirv_glsl.hpp>

namespace Shroon
{
    namespace Prism
    {
        namespace HL
        {
            class GL33Pipeline
            {
            public:
                static void Create(Pipeline & pl, PipelineSpecification & spec);
                static void Destroy(Pipeline & pl);

                static void ApplyStateChanges(Pipeline & pl);
            };
        }
    }
}
