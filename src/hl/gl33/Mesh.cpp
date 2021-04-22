#include "Mesh.hpp"

namespace Shroon
{
    namespace Prism
    {
        namespace HL
        {
            void GL33Mesh::Create(Mesh & mesh, MeshSpecification & spec)
            {
                mesh.m_VertexCount = spec.VertexCount;

                glGenVertexArrays(1, mesh.m_Handle.AsPtrOf<GLuint>());
                glBindVertexArray(mesh.m_Handle.As<GLuint>());

                mesh.m_Buffers.resize(spec.BufferDescriptions.size());

                for (size_t i = 0; i < spec.BufferDescriptions.size(); i++)
                {
                    mesh.m_Buffers[i].Create(spec.BufferDescriptions[i].first);

                    auto & desc = spec.BufferDescriptions[i].second;

                    for (auto & attrDesc : desc.AttributeDescriptions)
                    {
                        glVertexAttribPointer(
                            attrDesc.Location,
                            attrDesc.ComponentCount,
                            attrDesc.Type.Type,
                            GL_FALSE,
                            attrDesc.Stride,
                            reinterpret_cast<const void *>(attrDesc.Offset)
                        );

                        glEnableVertexAttribArray(attrDesc.Location);
                        glVertexAttribDivisor(attrDesc.Location, desc.Rate == VertexBufferInputDescription::InputRatePerVertex ? 0 : 1);
                    }
                }
            }

            void GL33Mesh::Destroy(Mesh & mesh)
            {
                for (auto & buf : mesh.m_Buffers)
                {
                    buf.~Buffer();
                }

                glDeleteVertexArrays(1, mesh.m_Handle.AsPtrOf<GLuint>());
            }
        }
    }
}
