#pragma once

#include "API.hpp"
#include "api/Buffer.hpp"

#include "gl45/Core.hpp"

namespace Shroon
{
    namespace Prism
    {
        class GL45Buffer
        {
        public:
            static void Create(Buffer & buf, BufferSpecification spec);
            static void Destroy(Buffer & buf);

            static uint32_t GetUniformBlockBindingBoint(Buffer & buf);

            static void Write(Buffer & buf, void * data, size_t size, size_t offset);

            static void SetVertexAttrib(Buffer & buf, uint32_t index, VertexAttribType type, size_t componentCount, size_t stride, size_t offset);
        };
    }
}
