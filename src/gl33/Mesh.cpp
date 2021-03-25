#include "Mesh.hpp"

namespace Shroon
{
    namespace Prism
    {
        void GL33Mesh::Create(Mesh & mesh, MeshSpecification spec)
        {
            mesh.m_Spec = spec;

            glGenVertexArrays(1, reinterpret_cast<GLuint *>(&mesh.m_Handle));
            glBindVertexArray(mesh.m_Handle);

            mesh.m_Buffers.resize(spec.Buffers.size());

            for (size_t i = 0; i < spec.Buffers.size(); i++)
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

            glBindVertexArray(0);
        }

        void GL33Mesh::Destroy(Mesh & mesh)
        {
            for (auto & buf : mesh.m_Buffers)
            {
                buf.Destroy();
            }

            glDeleteVertexArrays(1, reinterpret_cast<GLuint *>(&mesh.m_Handle));
        }
    }
}
