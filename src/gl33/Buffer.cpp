#include "Buffer.hpp"

namespace Shroon
{
    namespace Prism
    {
        void GL33Buffer::Create(Buffer & buf, BufferSpecification spec)
        {
            buf.m_Type = spec.Type;
            buf.m_Size = spec.Size;

            if (buf.m_Type == BufferType::Uniform)
            {
                static uint32_t lastBindingPoint = 0;

                glGenBuffers(1, reinterpret_cast<GLuint *>(&buf.m_Handle));
                glBindBuffer(buf.m_Type, buf.m_Handle);
                glBufferData(buf.m_Type, buf.m_Size, nullptr, GL_DYNAMIC_DRAW);
                glBindBufferBase(buf.m_Type, lastBindingPoint, buf.m_Handle);

                buf.m_UniformBlockBindingPoint = lastBindingPoint++;
            }
            else
            {
                glGenBuffers(1, reinterpret_cast<GLuint *>(&buf.m_Handle));
                glBindBuffer(buf.m_Type, buf.m_Handle);
                glBufferData(buf.m_Type, buf.m_Size, spec.Data, GL_STATIC_DRAW);
            }
        }

        void GL33Buffer::Destroy(Buffer & buf)
        {
            glDeleteBuffers(1, reinterpret_cast<GLuint *>(&buf.m_Handle));
        }

        uint32_t GL33Buffer::GetUniformBlockBindingBoint(Buffer & buf)
        {
            return buf.m_UniformBlockBindingPoint;
        }

        void GL33Buffer::Write(Buffer & buf, void * data, size_t size, size_t offset)
        {
            glBindBuffer(buf.m_Type, buf.m_Handle);
            glBufferSubData(buf.m_Type, size, offset, data);
        }

        void GL33Buffer::SetVertexAttrib(Buffer & buf, uint32_t index, VertexAttribType type, size_t componentCount, size_t stride, size_t offset)
        {
            glBindBuffer(buf.m_Type, buf.m_Handle);
            glVertexAttribPointer(index, componentCount, type, GL_FALSE, stride, (const void *)offset);
            glEnableVertexAttribArray(index);
        }
    }
}
