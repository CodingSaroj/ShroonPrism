#pragma once

#include <vector>

#include "API.hpp"
#include "APIManager.hpp"

#include "api/Handle.hpp"
#include "api/Buffer.hpp"

namespace Shroon
{
    namespace Prism
    {
        struct MeshSpecification
        {
            struct MeshBufferSpecification
            {
                bool IsVertexAttrib = false;
                uint32_t AttribIndex;
                VertexAttribType AttribType;
                size_t AttribComponentCount;
                size_t AttribStride;
                size_t AttribOffset;
                BufferSpecification Spec;
            };

            size_t VertexCount;
            std::vector<MeshBufferSpecification> Buffers;
        };

        class Mesh
        {
        public:
            inline void Create(MeshSpecification spec)
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

            inline size_t GetVertexCount()
            {
                return m_Spec.VertexCount;
            }

        private:
            Handle m_Handle;
            MeshSpecification m_Spec;
            std::vector<Buffer> m_Buffers;

            static void( * CreateFn)(Mesh &, MeshSpecification spec);
            static void( * DestroyFn)(Mesh &);

            friend class APIManager;

            friend class GL33Mesh;
            friend class GL45Mesh;
            friend class VK12Mesh;

            friend class GL33CommandBuffer;
            friend class GL45CommandBuffer;
            friend class VK12CommandBuffer;
        };
    }
}
