#pragma once

#include "api/Mesh.hpp"

#include "gl45/Core.hpp"

namespace Shroon
{
    namespace Prism
    {
        class GL45Mesh
        {
        public:
            static void Create(Mesh & mesh, MeshSpecification spec);
            static void Destroy(Mesh & mesh);
        };
    }
}
