#pragma once

#include "hl/API.hpp"
#include "hl/APIManager.hpp"

#include "hl/api/Mesh.hpp"
#include "hl/api/Texture.hpp"

#include <spirv_cross/spirv_glsl.hpp>

namespace Shroon
{
    namespace Prism
    {
        namespace HL
        {
            struct PrimitiveType
            {
                uint32_t Type = Triangles;

                static uint32_t Points;
                static uint32_t Lines;
                static uint32_t LineStrip;
                static uint32_t Triangles;
                static uint32_t TriangleStrip;

                PrimitiveType() = default;

                inline PrimitiveType(uint32_t type)
                    : Type(type)
                {
                }

                inline operator uint32_t()
                {
                    return Type;
                }
            };

            struct FillType
            {
                uint32_t Type = Fill;

                static uint32_t Wireframe;
                static uint32_t Fill;

                FillType() = default;

                inline FillType(uint32_t type)
                    : Type(type)
                {
                }

                inline operator uint32_t()
                {
                    return Type;
                }
            };

            struct CullFace
            {
                uint32_t Type = Back;

                static uint32_t Front;
                static uint32_t Back;

                CullFace() = default;

                inline CullFace(uint32_t type)
                    : Type(type)
                {
                }

                inline operator uint32_t()
                {
                    return Type;
                }
            };

            struct UniformStructureNode
            {
                spirv_cross::SPIRType Type;
                std::string Name;
                uint32_t    Size;

                std::vector<UniformStructureNode> Members;
            };

            struct PipelineState
            {
                enum PipelineStateFlags : uint16_t
                {
                    DepthTestBit   = 0x0001,
                    StencilTestBit = 0x0002,
                    BlendingBit    = 0x0004,
                    MultisampleBit = 0x0008,
                    FaceCullingBit = 0x0010,
                };

                PipelineStateFlags Flags;
                PrimitiveType Primitive;
                FillType Fill;
                CullFace Cull;
            };

            struct PipelineSpecification
            {
                std::vector<VertexBufferInputDescription> VertexBufferInputDescriptions;

                std::vector<uint32_t> * VertexShader = nullptr;
                std::vector<uint32_t> * GeometryShader = nullptr;
                std::vector<uint32_t> * FragmentShader = nullptr;
            };

            class Pipeline
            {
            public:
                inline ~Pipeline()
                {
                    DestroyFn(*this);
                }

                inline void Create(PipelineSpecification & spec)
                {
                    CreateFn(*this, spec);
                }

                inline Handle GetRaw()
                {
                    return m_Handle;
                }

                inline PipelineSpecification GetSpec()
                {
                    return m_Spec;
                }

                inline const std::unordered_map<std::string, UniformStructureNode> & GetUniformBufferLayouts()
                {
                    return m_UniformStructures;
                }

                inline void ClearFlag(PipelineState::PipelineStateFlags mask)
                {
                    m_State.Flags = static_cast<PipelineState::PipelineStateFlags>(m_State.Flags & ~mask);
                }

                inline void SetFlag(PipelineState::PipelineStateFlags mask)
                {
                    m_State.Flags = static_cast<PipelineState::PipelineStateFlags>(m_State.Flags | mask);
                }

                inline void SetPrimitiveType(PrimitiveType type)
                {
                    m_State.Primitive = type;
                }

                inline void SetFillType(FillType type)
                {
                    m_State.Fill = type;
                }

                inline void SetCullFace(CullFace face)
                {
                    m_State.Cull = face;
                }

                inline void SetState(PipelineState state)
                {
                    m_State = state;

                    ApplyStateChangesFn(*this);
                }

                inline void ApplyStateChanges()
                {
                    ApplyStateChangesFn(*this);
                }

            private:
                Handle m_Handle;
                PipelineState m_State;
                PipelineSpecification m_Spec;

                std::unordered_map<std::string, UniformStructureNode> m_UniformStructures;

                static void( * CreateFn)(Pipeline &, PipelineSpecification &);
                static void( * DestroyFn)(Pipeline &);

                static void( * ApplyStateChangesFn)(Pipeline &);

                friend class APIManager;

                friend class GL33Pipeline;
                friend class GL45Pipeline;
                friend class ES20Pipeline;
                friend class ES30Pipeline;
            };
        }
    }
}
