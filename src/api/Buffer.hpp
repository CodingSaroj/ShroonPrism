#pragma once

#include <iostream>

#include "API.hpp"
#include "APIManager.hpp"

#include "api/Handle.hpp"

namespace Shroon
{
    namespace Prism
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

        struct VertexAttribType
        {
            uint32_t Type = Float;

            static uint32_t Byte;
            static uint32_t UByte;
            static uint32_t Short;
            static uint32_t UShort;
            static uint32_t Int;
            static uint32_t UInt;
            static uint32_t Float;

            VertexAttribType() = default;

            inline VertexAttribType(uint32_t type)
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
            void * Data;
            size_t Size;
        };

        class Buffer
        {
        public:
            inline void Create(BufferSpecification spec)
            {
                CreateFn(*this, spec);
            }

            inline void Destroy()
            {
                DestroyFn(*this);
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

            inline uint32_t GetUniformBlockBindingPoint()
            {
                return GetUniformBlockBindingPointFn(*this);
            }

            inline void Write(void * data, size_t start, size_t size)
            {
                WriteFn(*this, data, start, size);
            }

            inline void SetVertexAttrib(uint32_t index, VertexAttribType type, size_t componentCount, size_t stride, size_t offset = 0)
            {
                SetVertexAttribFn(*this, index, type, componentCount, stride, offset);
            }

        private:
            Handle m_Handle;
            Handle m_MemoryHandle;
            BufferType m_Type;
            size_t m_Size;
            uint32_t m_UniformBlockBindingPoint;

            static void( * CreateFn)(Buffer &, BufferSpecification);
            static void( * DestroyFn)(Buffer &);

            static uint32_t( * GetUniformBlockBindingPointFn)(Buffer &);

            static void( * WriteFn)(Buffer &, void *, size_t, size_t);

            static void( * SetVertexAttribFn)(Buffer &, uint32_t, VertexAttribType, size_t, size_t, size_t);

            friend class APIManager;

            friend class GL33Buffer;
            friend class GL45Buffer;
            friend class VK12Buffer;
        };
    }
}
