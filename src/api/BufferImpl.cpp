#include "api/Buffer.hpp"
#include "vk12/Buffer.hpp"
#include "gl33/Buffer.hpp"
#include "gl45/Buffer.hpp"

namespace Shroon
{
    namespace Prism
    {
        void( * Buffer::CreateFn)(Buffer &, BufferSpecification) = 0;
        void( * Buffer::DestroyFn)(Buffer &) = 0;

        uint32_t( * Buffer::GetUniformBlockBindingPointFn)(Buffer &) = 0;

        void( * Buffer::WriteFn)(Buffer &, void *, size_t, size_t) = 0;

        void( * Buffer::SetVertexAttribFn)(Buffer &, uint32_t, VertexAttribType, size_t, size_t, size_t) = 0;

        uint32_t BufferType::VertexArray = 0;
        uint32_t BufferType::VertexArrayIndex = 0;
        uint32_t BufferType::Uniform = 0;

        uint32_t VertexAttribType::Byte = 0;
        uint32_t VertexAttribType::UByte = 0;
        uint32_t VertexAttribType::Short = 0;
        uint32_t VertexAttribType::UShort = 0;
        uint32_t VertexAttribType::Int = 0;
        uint32_t VertexAttribType::UInt = 0;
        uint32_t VertexAttribType::Float = 0;

        void APIManager::InitBufferAPI()
        {
            switch (s_GraphicsAPI)
            {
                case API::GL33:
                {
                    Buffer::CreateFn                        = GL33Buffer::Create;
                    Buffer::DestroyFn                       = GL33Buffer::Destroy;
                    Buffer::GetUniformBlockBindingPointFn   = GL33Buffer::GetUniformBlockBindingBoint;
                    Buffer::WriteFn                         = GL33Buffer::Write;
                    Buffer::SetVertexAttribFn               = GL33Buffer::SetVertexAttrib;

                    BufferType::VertexArray         = GL_ARRAY_BUFFER;
                    BufferType::VertexArrayIndex    = GL_ELEMENT_ARRAY_BUFFER;
                    BufferType::Uniform             = GL_UNIFORM_BUFFER;

                    VertexAttribType::Byte      = GL_BYTE;
                    VertexAttribType::UByte     = GL_UNSIGNED_BYTE;
                    VertexAttribType::Short     = GL_SHORT;
                    VertexAttribType::UShort    = GL_UNSIGNED_SHORT;
                    VertexAttribType::Int       = GL_INT;
                    VertexAttribType::UInt      = GL_UNSIGNED_INT;
                    VertexAttribType::Float     = GL_FLOAT;

                    break;
                }

                case API::GL45:
                {
                    Buffer::CreateFn                        = GL45Buffer::Create;
                    Buffer::DestroyFn                       = GL45Buffer::Destroy;
                    Buffer::GetUniformBlockBindingPointFn   = GL45Buffer::GetUniformBlockBindingBoint;
                    Buffer::WriteFn                         = GL45Buffer::Write;
                    Buffer::SetVertexAttribFn               = GL45Buffer::SetVertexAttrib;

                    BufferType::VertexArray         = GL_ARRAY_BUFFER;
                    BufferType::VertexArrayIndex    = GL_ELEMENT_ARRAY_BUFFER;
                    BufferType::Uniform             = GL_UNIFORM_BUFFER;
                    BufferType::TransferSrc         = GL_COPY_READ_BUFFER;
                    BufferType::TransferDst         = GL_COPY_WRITE_BUFFER;

                    VertexAttribType::Byte      = GL_BYTE;
                    VertexAttribType::UByte     = GL_UNSIGNED_BYTE;
                    VertexAttribType::Short     = GL_SHORT;
                    VertexAttribType::UShort    = GL_UNSIGNED_SHORT;
                    VertexAttribType::Int       = GL_INT;
                    VertexAttribType::UInt      = GL_UNSIGNED_INT;
                    VertexAttribType::Float     = GL_FLOAT;

                    break;
                }

                case API::VK12:
                {
                    Buffer::CreateFn                        = VK12Buffer::Create;
                    Buffer::DestroyFn                       = VK12Buffer::Destroy;
                    Buffer::GetUniformBlockBindingPointFn   = VK12Buffer::GetUniformBlockBindingBoint;
                    Buffer::WriteFn                         = VK12Buffer::Write;
                    Buffer::SetVertexAttribFn               = VK12Buffer::SetVertexAttrib;

                    BufferType::VertexArray         = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
                    BufferType::VertexArrayIndex    = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
                    BufferType::Uniform             = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
                    BufferType::TransferSrc         = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
                    BufferType::TransferDst         = VK_BUFFER_USAGE_TRANSFER_DST_BIT;

                    break;
                };
            }
        }
    }
}
