#pragma once

#include "api/Pipeline.hpp"

#include "gl45/Core.hpp"

namespace Shroon
{
    namespace Prism
    {
        class GL33Pipeline
        {
        public:
            static void Create(Pipeline & pl, PipelineSpecification spec);
            static void Destroy(Pipeline & pl);

            static void Run(Pipeline & pl, std::vector<PipelineObject> & objects);
        };
    }
}
