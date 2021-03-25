#include "Mesh.hpp"

namespace Shroon
{
    namespace Prism
    {
        void VK12Mesh::Create(Mesh & mesh, MeshSpecification spec)
        {
            static uint64_t s_LastMeshHandle = 0;

            mesh.m_Handle = s_LastMeshHandle++;
            mesh.m_Spec = spec;
            mesh.m_Buffers.resize(spec.Buffers.size());

            for (uint32_t i = 0; i < mesh.m_Buffers.size(); i++)
            {
                mesh.m_Buffers[i].Create(spec.Buffers[i].Spec);

                if (spec.Buffers[i].IsVertexAttrib)
                {
                    mesh.m_Buffers[i].SetVertexAttrib(
                        spec.Buffers[i].AttribIndex,
                        spec.Buffers[i].AttribType,
                        spec.Buffers[i].AttribComponentCount,
                        spec.Buffers[i].AttribStride,
                        spec.Buffers[i].AttribOffset
                    );
                }
            }
        }

        void VK12Mesh::Destroy(Mesh & mesh)
        {
            for (auto & buffer : mesh.m_Buffers)
            {
                buffer.Destroy();
            }
        }
    }
}
