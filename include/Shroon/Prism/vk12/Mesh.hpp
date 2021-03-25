#pragma once

#include "api/Mesh.hpp"

#include "vk12/Core.hpp"
#include "vk12/Buffer.hpp"

namespace Shroon
{
    namespace Prism
    {
        class VK12Mesh
        {
        public:
            static void Create(Mesh & mesh, MeshSpecification spec);
            static void Destroy(Mesh & mesh);
        };
    }
}
