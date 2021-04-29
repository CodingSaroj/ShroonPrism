#pragma once

#include "hl/api/Mesh.hpp"

#include "hl/es30/Core.hpp"

namespace Shroon
{
    namespace Prism
    {
        namespace HL
        {
            class ES30Mesh
            {
            public:
                static void Create(Mesh & mesh, MeshSpecification & spec);
                static void Destroy(Mesh & mesh);
            };
        }
    }
}
