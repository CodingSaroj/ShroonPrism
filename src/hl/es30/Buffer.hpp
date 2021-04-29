#pragma once

#include "hl/API.hpp"
#include "hl/api/Buffer.hpp"

#include "hl/es30/Core.hpp"

namespace Shroon
{
    namespace Prism
    {
        namespace HL
        {
            class ES30Buffer
            {
            public:
                static void Create(Buffer & buf, BufferSpecification & spec);
                static void Destroy(Buffer & buf);

                static void Write(Buffer & buf, void * data, size_t size, size_t offset);
            };
        }
    }
}
