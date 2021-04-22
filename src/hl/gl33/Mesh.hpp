#pragma once

#include "hl/api/Mesh.hpp"

#include "hl/gl33/Core.hpp"

namespace Shroon
{
    namespace Prism
    {
        namespace HL
        {
            class GL33Mesh
            {
            public:
                static void Create(Mesh & mesh, MeshSpecification & spec);
                static void Destroy(Mesh & mesh);
            };
        }
    }
}
