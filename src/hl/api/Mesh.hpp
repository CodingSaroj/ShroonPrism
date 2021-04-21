#pragma once

#include <vector>

#include "hl/API.hpp"
#include "hl/APIManager.hpp"

#include "hl/api/Buffer.hpp"

namespace Shroon
{
    namespace Prism
    {
        namespace HL
        {
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

            struct VertexBufferInputDescription
            {
                struct VertexAttributeDescription
                {
                    VertexAttribType Type;
                    uint32_t Location;
                    size_t ComponentCount;
                    size_t Stride;
                    size_t Offset;
                };

                enum InputRate
                {
                    InputRatePerVertex,
                    InputRatePerInstance
                } Rate;

                std::vector<VertexAttributeDescription> AttributeDescriptions;
            };

            struct MeshSpecification
            {
                uint32_t VertexCount;
                std::vector<std::pair<BufferSpecification, VertexBufferInputDescription>> BufferDescriptions;
            };

            class Mesh
            {
            public:
                inline ~Mesh()
                {
                    DestroyFn(*this);
                }

                inline void Create(MeshSpecification & spec)
                {
                    CreateFn(*this, spec);
                }

                inline Handle GetRaw()
                {
                    return m_Handle;
                }

                inline size_t GetVertexCount()
                {
                    return m_VertexCount;
                }

            private:
                Handle m_Handle;
                uint32_t m_VertexCount;
                std::vector<Buffer> m_Buffers;

                static void( * CreateFn)(Mesh &, MeshSpecification & spec);
                static void( * DestroyFn)(Mesh &);

                friend class APIManager;

                friend class GL33Mesh;
                friend class GL45Mesh;
                friend class ES20Mesh;
                friend class ES30Mesh;
            };
        }
    }
}
