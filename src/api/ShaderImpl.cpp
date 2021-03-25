#include "api/Shader.hpp"
#include "gl33/Shader.hpp"
#include "gl45/Shader.hpp"

namespace Shroon
{
    namespace Prism
    {
        void( * Shader::CreateFn)(Shader &, ShaderSpecification) = 0;
        void( * Shader::DestroyFn)(Shader &) = 0;

        void( * Shader::BindUniformBlockFn)(Shader &, std::string, uint32_t) = 0;

        void APIManager::InitShaderAPI()
        {
            switch (s_GraphicsAPI)
            {
                case API::GL33:
                {
                    Shader::CreateFn            = GL33Shader::Create;
                    Shader::DestroyFn           = GL33Shader::Destroy;
                    Shader::BindUniformBlockFn  = GL33Shader::BindUniformBlock;

                    break;
                }

                case API::GL45:
                {
                    Shader::CreateFn            = GL45Shader::Create;
                    Shader::DestroyFn           = GL45Shader::Destroy;
                    Shader::BindUniformBlockFn  = GL45Shader::BindUniformBlock;

                    break;
                }
            }
        }
    }
}
