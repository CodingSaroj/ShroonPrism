#include "hl/api/Buffer.hpp"
#include "hl/gl33/Buffer.hpp"
#include "hl/gl45/Buffer.hpp"
#include "hl/es30/Buffer.hpp"

namespace Shroon
{
    namespace Prism
    {
        namespace HL
        {
            void( * Buffer::CreateFn)(Buffer &, BufferSpecification &) = 0;
            void( * Buffer::DestroyFn)(Buffer &) = 0;

            void( * Buffer::WriteFn)(Buffer &, void *, size_t, size_t) = 0;

            uint32_t BufferType::VertexArray = 0;
            uint32_t BufferType::VertexArrayIndex = 0;
            uint32_t BufferType::Uniform = 0;
            uint32_t BufferType::TransferSrc = 0;
            uint32_t BufferType::TransferDst = 0;

            void APIManager::InitBufferAPI()
            {
                switch (s_GraphicsAPI)
                {
                    case API::GL33:
                    {
                        Buffer::CreateFn    = GL33Buffer::Create;
                        Buffer::DestroyFn   = GL33Buffer::Destroy;
                        Buffer::WriteFn     = GL33Buffer::Write;

                        BufferType::VertexArray         = GL_ARRAY_BUFFER;
                        BufferType::VertexArrayIndex    = GL_ELEMENT_ARRAY_BUFFER;
                        BufferType::Uniform             = GL_UNIFORM_BUFFER;
                        BufferType::TransferSrc         = GL_COPY_READ_BUFFER;
                        BufferType::TransferDst         = GL_COPY_WRITE_BUFFER;

                        break;
                    }

                    case API::GL45:
                    {
                        Buffer::CreateFn    = GL45Buffer::Create;
                        Buffer::DestroyFn   = GL45Buffer::Destroy;
                        Buffer::WriteFn     = GL45Buffer::Write;

                        BufferType::VertexArray         = GL_ARRAY_BUFFER;
                        BufferType::VertexArrayIndex    = GL_ELEMENT_ARRAY_BUFFER;
                        BufferType::Uniform             = GL_UNIFORM_BUFFER;
                        BufferType::TransferSrc         = GL_COPY_READ_BUFFER;
                        BufferType::TransferDst         = GL_COPY_WRITE_BUFFER;

                        break;
                    }

                    case API::ES30:
                    {
                        Buffer::CreateFn    = ES30Buffer::Create;
                        Buffer::DestroyFn   = ES30Buffer::Destroy;
                        Buffer::WriteFn     = ES30Buffer::Write;

                        BufferType::VertexArray         = GL_ARRAY_BUFFER;
                        BufferType::VertexArrayIndex    = GL_ELEMENT_ARRAY_BUFFER;
                        BufferType::Uniform             = GL_UNIFORM_BUFFER;
                        BufferType::TransferSrc         = GL_COPY_READ_BUFFER;
                        BufferType::TransferDst         = GL_COPY_WRITE_BUFFER;

                        break;
                    }
                }
            }
        }
    }
}
