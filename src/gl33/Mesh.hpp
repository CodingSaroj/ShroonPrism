#pragma once

#include "api/Mesh.hpp"

#include "gl33/Core.hpp"

namespace Shroon
{
    namespace Prism
    {
        class GL33Mesh
        {
        public:
            static void Create(Mesh & mesh, MeshSpecification spec);
            static void Destroy(Mesh & mesh);
        };
    }
}
