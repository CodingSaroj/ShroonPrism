#pragma once

#include <cstring>

#include "api/Buffer.hpp"

#include "vk12/Core.hpp"
#include "vk12/State.hpp"

namespace Shroon
{
    namespace Prism
    {
        class VK12Buffer
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