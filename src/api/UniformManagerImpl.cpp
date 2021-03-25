#include "api/UniformManager.hpp"
#include "gl33/UniformManager.hpp"
#include "gl45/UniformManager.hpp"

namespace Shroon
{
    namespace Prism
    {
        std::unordered_map<std::string, Buffer> UniformManager::m_UniformBlocks;

        void( * UniformManager::DestroyFn)() = 0;

        uint32_t( * UniformManager::GetBindingPointFn)(const std::string &) = 0;

        void( * UniformManager::CreateUniformBlockFn)(const std::string &, uint32_t) = 0;
        void( * UniformManager::UpdateUniformBlockFn)(const std::string &, void *, uint32_t, uint32_t) = 0;

        void APIManager::InitUniformManagerAPI()
        {
            switch (s_GraphicsAPI)
            {
                case API::GL33:
                {
                    UniformManager::DestroyFn               = GL45UniformManager::Destroy;
                    UniformManager::GetBindingPointFn       = GL45UniformManager::GetBindingPoint;
                    UniformManager::CreateUniformBlockFn    = GL45UniformManager::CreateUniformBlock;
                    UniformManager::UpdateUniformBlockFn    = GL45UniformManager::UpdateUniformBlock;

                    break;
                }

                case API::GL45:
                {
                    UniformManager::DestroyFn               = GL45UniformManager::Destroy;
                    UniformManager::GetBindingPointFn       = GL45UniformManager::GetBindingPoint;
                    UniformManager::CreateUniformBlockFn    = GL45UniformManager::CreateUniformBlock;
                    UniformManager::UpdateUniformBlockFn    = GL45UniformManager::UpdateUniformBlock;

                    break;
                }
            }
        }
    }
}
