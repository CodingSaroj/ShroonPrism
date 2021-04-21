#pragma once

#include <iostream>

#include "Handle.hpp"

#include "hl/API.hpp"
#include "hl/APIManager.hpp"

namespace Shroon
{
    namespace Prism
    {
        namespace HL
        {
            struct BufferType
            {
                uint32_t Type = VertexArray;

                static uint32_t VertexArray;
                static uint32_t VertexArrayIndex;
                static uint32_t Uniform;
                static uint32_t TransferSrc;
                static uint32_t TransferDst;

                BufferType() = default;

                inline BufferType(uint32_t type)
                    : Type(type)
                {
                }

                inline operator uint32_t()
                {
                    return Type;
                }
            };

            struct BufferSpecification
            {
                BufferType Type;
                bool Dynamic = false;
                void * Data = nullptr;
                size_t Size = 0;
            };

            class Buffer
            {
            public:
                inline ~Buffer()
                {
                    DestroyFn(*this);
                }

                inline void Create(BufferSpecification & spec)
                {
                    CreateFn(*this, spec);
                }

                inline Handle GetRaw()
                {
                    return m_Handle;
                }

                inline BufferType GetType()
                {
                    return m_Type;
                }

                inline size_t GetSize()
                {
                    return m_Size;
                }

                inline uint32_t GetUBOBindPoint()
                {
                    return m_UBOBindPoint;
                }

                inline void Write(void * data, size_t start, size_t size)
                {
                    WriteFn(*this, data, start, size);
                }

            private:
                Handle m_Handle;

                BufferType m_Type;
                size_t m_Size;
                uint32_t m_UBOBindPoint;

                static void( * CreateFn)(Buffer &, BufferSpecification &);
                static void( * DestroyFn)(Buffer &);

                static void( * WriteFn)(Buffer &, void *, size_t, size_t);

                friend class APIManager;

                friend class GL33Buffer;
                friend class GL45Buffer;
                friend class ES20Buffer;
                friend class ES30Buffer;
            };
        }
    }
}
