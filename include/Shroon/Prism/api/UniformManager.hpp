#pragma once

#include <unordered_map>

#include "API.hpp"
#include "APIManager.hpp"
#include "api/Buffer.hpp"

namespace Shroon
{
    namespace Prism
    {
        class UniformManager
        {
        public:
            static inline void Destroy()
            {
                DestroyFn();
            }

            static inline uint32_t GetBindingPoint(const std::string & name)
            {
                return GetBindingPointFn(name);
            }

            static inline void CreateUniformBlock(const std::string & name, uint32_t size)
            {
                CreateUniformBlockFn(name, size);
            }

            static inline void UpdateUniformBlock(const std::string & name, void * data, uint32_t size, uint32_t offset = 0)
            {
                UpdateUniformBlockFn(name, data, size, offset);
            }

        private:
            static std::unordered_map<std::string, Buffer> m_UniformBlocks;

            static void( * DestroyFn)();

            static uint32_t( * GetBindingPointFn)(const std::string &);

            static void( * CreateUniformBlockFn)(const std::string &, uint32_t);
            static void( * UpdateUniformBlockFn)(const std::string &, void *, uint32_t, uint32_t);

            friend class APIManager;

            friend class GL33UniformManager;
            friend class GL45UniformManager;
        };
    }
}
