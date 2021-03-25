#pragma once

#include <string>
#include <vector>

#include <cstdint>

#include "API.hpp"
#include "APIManager.hpp"
#include "ErrorReporter.hpp"

#include "api/Handle.hpp"
#include "api/Buffer.hpp"

#include <spirv_cross/spirv_cross.hpp>

namespace Shroon
{
    namespace Prism
    {
        struct UniformStructureNode
        {
            spirv_cross::SPIRType Type;
            std::string Name;
            uint32_t    Size;

            std::vector<UniformStructureNode> Members;
        };

        struct ShaderSpecification
        {
            std::vector<uint32_t> * VertexShader = nullptr;
            std::vector<uint32_t> * TessellationControlShader = nullptr;
            std::vector<uint32_t> * TessellationEvaluationShader = nullptr;
            std::vector<uint32_t> * GeometryShader = nullptr;
            std::vector<uint32_t> * FragmentShader = nullptr;
            std::vector<uint32_t> * ComputeShader = nullptr;
        };

        class Shader
        {
        public:
            inline void Create(ShaderSpecification spec)
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

            inline std::vector<UniformStructureNode> GetUniformStructures()
            {
                return m_UniformStructures;
            }

            inline std::vector<std::string> GetRequiredUniformBlockNames()
            {
                return m_RequiredUniformBlocks;
            }

            inline void BindUniformBlock(std::string name, uint32_t bindingPoint)
            {
                BindUniformBlockFn(*this, name, bindingPoint);
            }

        private:
            Handle m_Handle;
            std::vector<UniformStructureNode> m_UniformStructures;
            std::vector<std::string> m_RequiredUniformBlocks;

            static void( * CreateFn)(Shader &, ShaderSpecification);
            static void( * DestroyFn)(Shader &);

            static void( * BindUniformBlockFn)(Shader &, std::string, uint32_t);

            friend class APIManager;

            friend class GL33Shader;
            friend class GL45Shader;
            friend class VK12Shader;
        };
    }
}
