#pragma once

#include <cstring>

#include "ErrorReporter.hpp"

#include "hl/api/Pipeline.hpp"

#include "hl/es30/Core.hpp"

#include <spirv_cross/spirv_glsl.hpp>

namespace Shroon
{
    namespace Prism
    {
        namespace HL
        {
            class ES30Pipeline
            {
            public:
                static void Create(Pipeline & pl, PipelineSpecification & spec);
                static void Destroy(Pipeline & pl);

                static void ApplyStateChanges(Pipeline & pl);
            };
        }
    }
}
