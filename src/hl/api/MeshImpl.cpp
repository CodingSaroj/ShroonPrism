#include "hl/api/Mesh.hpp"
#include "hl/gl33/Mesh.hpp"
#include "hl/gl45/Mesh.hpp"
#include "hl/es30/Mesh.hpp"

namespace Shroon
{
    namespace Prism
    {
        namespace HL
        {
            void( * Mesh::CreateFn)(Mesh &, MeshSpecification &) = 0;
            void( * Mesh::DestroyFn)(Mesh &) = 0;

            void APIManager::InitMeshAPI()
            {
                switch (s_GraphicsAPI)
                {
                    case API::GL33:
                    {
                        Mesh::CreateFn  = GL33Mesh::Create;
                        Mesh::DestroyFn = GL33Mesh::Destroy;

                        break;
                    }

                    case API::GL45:
                    {
                        Mesh::CreateFn  = GL45Mesh::Create;
                        Mesh::DestroyFn = GL45Mesh::Destroy;

                        break;
                    }

                    case API::ES30:
                    {
                        Mesh::CreateFn  = ES30Mesh::Create;
                        Mesh::DestroyFn = ES30Mesh::Destroy;

                        break;
                    }
                }
            }
        }
    }
}
