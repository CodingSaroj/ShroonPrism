#pragma once

#include "API.hpp"
#include "APIManager.hpp"

#include "api/Mesh.hpp"
#include "api/Texture.hpp"
#include "api/Shader.hpp"
#include "api/UniformManager.hpp"
#include "api/Pass.hpp"

namespace Shroon
{
    namespace Prism
    {
        struct GeometryMode
        {
            uint32_t Type = Triangles;

            static uint32_t Points;
            static uint32_t Lines;
            static uint32_t LineStrip;
            static uint32_t Triangles;
            static uint32_t TriangleStrip;

            GeometryMode() = default;

            inline GeometryMode(uint32_t type)
                : Type(type)
            {
            }

            inline operator uint32_t()
            {
                return Type;
            }
        };

        struct PolygonMode
        {
            uint32_t Type = Fill;

            static uint32_t Wireframe;
            static uint32_t Fill;

            PolygonMode() = default;

            inline PolygonMode(uint32_t type)
                : Type(type)
            {
            }

            inline operator uint32_t()
            {
                return Type;
            }
        };

        struct PipelineObject
        {
            Mesh ObjectMesh;
            std::vector<uint8_t> ObjectData;
        };

        struct PipelineSpecification
        {
            bool DepthTesting = false;
            bool StencilTesting = false;

            GeometryMode PipelineGeometryMode;
            PolygonMode PipelinePolygonMode;

            Pass TargetPass;
            uint32_t TargetSubpassIndex = 0;

            std::vector<uint32_t> * VertexShader = nullptr;
            std::vector<uint32_t> * TessellationControlShader = nullptr;
            std::vector<uint32_t> * TessellationEvaluationShader = nullptr;
            std::vector<uint32_t> * GeometryShader = nullptr;
            std::vector<uint32_t> * FragmentShader = nullptr;
            std::vector<uint32_t> * ComputeShader = nullptr;
        };

        class Pipeline
        {
        public:
            inline void Create(PipelineSpecification spec)
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

            inline PipelineSpecification GetSpec()
            {
                return m_Spec;
            }

            inline void Run(std::vector<PipelineObject> & objects)
            {
                RunFn(*this, objects);
            }

        private:
            Handle m_Handle;
            Handle m_LayoutHandle;
            PipelineSpecification m_Spec;
            Shader m_Shader;

            static void( * CreateFn)(Pipeline &, PipelineSpecification);
            static void( * DestroyFn)(Pipeline &);

            static void( * RunFn)(Pipeline &, std::vector<PipelineObject> &);

            friend class APIManager;

            friend class GL33Pipeline;
            friend class GL45Pipeline;
            friend class VK12Pipeline;
        };
    }
}
