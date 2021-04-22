#pragma once

#include <cstdint>

namespace Shroon
{
    namespace Prism
    {
        class Handle
        {
        public:
            Handle() = default;

            inline Handle(uint32_t raw)
                : m_Handle(raw)
            {
            }

            inline Handle(uint64_t raw)
                : m_Handle(raw)
            {
            }

            template <typename T>
            inline Handle(T * raw)
                : m_Handle(reinterpret_cast<uint64_t>(raw))
            {
            }

            template <typename T>
            inline T As()
            {
                return (T)m_Handle;
            }

            template <typename T>
            inline T * AsPtrOf()
            {
                return reinterpret_cast<T *>(&m_Handle);
            }

            template <typename T>
            inline T & AsRefOf()
            {
                return *reinterpret_cast<T *>(&m_Handle);
            }

            inline bool operator==(const Handle & handle)
            {
                return m_Handle == handle.m_Handle;
            }

            inline bool operator!=(const Handle & handle)
            {
                return m_Handle != handle.m_Handle;
            }

        private:
            uint64_t m_Handle;
        };
    }
}
