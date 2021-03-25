#pragma once

#include "api/Pipeline.hpp"

#include "vk12/Core.hpp"
#include "vk12/State.hpp"

namespace Shroon
{
    namespace Prism
    {
        class VK12Pipeline
        {
        public:
            static void Create(Pipeline & pl, PipelineSpecification spec);
            static void Destroy(Pipeline & pl);

            static void Run(Pipeline & pl, std::vector<PipelineObject> & objects);
        };
    }
}
