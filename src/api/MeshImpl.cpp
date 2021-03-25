#include "api/Mesh.hpp"
#include "vk12/Mesh.hpp"
#include "gl33/Mesh.hpp"
#include "gl45/Mesh.hpp"

namespace Shroon
{
    namespace Prism
    {
        void( * Mesh::CreateFn)(Mesh &, MeshSpecification) = 0;
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

                case API::VK12:
                {
                    Mesh::CreateFn  = VK12Mesh::Create;
                    Mesh::DestroyFn = VK12Mesh::Destroy;

                    break;
                }
            }
        }
    }
}
