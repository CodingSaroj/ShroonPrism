#pragma once

#include "hl/api/Mesh.hpp"

#include "hl/gl45/Core.hpp"

namespace Shroon
{
    namespace Prism
    {
        namespace HL
        {
            class GL45Mesh
            {
            public:
                static void Create(Mesh & mesh, MeshSpecification & spec);
                static void Destroy(Mesh & mesh);
            };
        }
    }
}
