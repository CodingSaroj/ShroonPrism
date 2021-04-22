#include "Buffer.hpp"

namespace Shroon
{
    namespace Prism
    {
        namespace HL
        {
            void GL33Buffer::Create(Buffer & buf, BufferSpecification & spec)
            {
                static uint32_t s_LastUBOBindPoint = 0;

                buf.m_Type = spec.Type;
                buf.m_Size = spec.Size;
                buf.m_UBOBindPoint = s_LastUBOBindPoint++;

                glGenBuffers(1, buf.m_Handle.AsPtrOf<GLuint>());
                glBindBuffer(buf.m_Type, buf.m_Handle.As<GLuint>());
                glBufferData(buf.m_Type, buf.m_Size, spec.Data, spec.Dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

                if (buf.m_Type == BufferType::Uniform)
                {
                    glBindBufferBase(GL_UNIFORM_BUFFER, buf.m_UBOBindPoint, buf.m_Handle.As<GLuint>());
                }
            }

            void GL33Buffer::Destroy(Buffer & buf)
            {
                glDeleteBuffers(1, buf.m_Handle.AsPtrOf<GLuint>());
            }

            void GL33Buffer::Write(Buffer & buf, void * data, size_t size, size_t offset)
            {
                glBindBuffer(buf.m_Type, buf.m_Handle.As<GLuint>());
                glBufferSubData(buf.m_Type, size, offset, data);
            }
        }
    }
}
