#include "UniformManager.hpp"

namespace Shroon
{
    namespace Prism
    {
        void GL45UniformManager::Destroy()
        {
            for (auto & block : UniformManager::m_UniformBlocks)
            {
                block.second.Destroy();
            }
        }

        uint32_t GL45UniformManager::GetBindingPoint(const std::string & name)
        {
            return UniformManager::m_UniformBlocks[name].GetUniformBlockBindingPoint();
        }

        void GL45UniformManager::CreateUniformBlock(const std::string & name, uint32_t size)
        {
            BufferSpecification spec;
            spec.Type.Type = BufferType::Uniform;
            spec.Size = size;

            UniformManager::m_UniformBlocks[name].Create(spec);
        }

        void GL45UniformManager::UpdateUniformBlock(const std::string & name, void * data, uint32_t size, uint32_t offset)
        {
            UniformManager::m_UniformBlocks[name].Write(data, offset, size);
        }
    }
}
