#pragma once

#include <cstdint>

#include "API.hpp"
#include "APIManager.hpp"

#include "api/Handle.hpp"

namespace Shroon
{
    namespace Prism
    {
        struct TextureType
        {
            uint32_t Type = Texture2D;

            static uint32_t Texture2D;

            TextureType() = default;

            inline TextureType(uint32_t type)
                : Type(type)
            {
            }

            inline operator uint32_t()
            {
                return Type;
            }
        };

        struct TextureFormat
        {
            uint32_t Type = RGBA;

            static uint32_t RED;
            static uint32_t RED_INT;
            static uint32_t RGB;
            static uint32_t RGBA;

            static uint32_t DEPTH24_STENCIL8;

            TextureFormat() = default;

            inline TextureFormat(uint32_t type)
                : Type(type)
            {
            }

            inline operator uint32_t()
            {
                return Type;
            }
        };

        struct TextureFilter
        {
            uint32_t Type = Linear;

            static uint32_t Nearest;
            static uint32_t Linear;

            TextureFilter() = default;

            inline TextureFilter(uint32_t type)
                : Type(type)
            {
            }

            inline operator uint32_t()
            {
                return Type;
            }
        };

        struct TextureWrap
        {
            uint32_t Type = Repeat;

            static uint32_t Repeat;
            static uint32_t MirroredRepeat;

            TextureWrap() = default;

            inline TextureWrap(uint32_t type)
                : Type(type)
            {
            }

            inline operator uint32_t()
            {
                return Type;
            }
        };
        struct TextureSpecification
        {
            bool Multisample = false;
            bool UseMipmaps = false;

            void * Data = nullptr;
            uint32_t Width;
            uint32_t Height;

            TextureType Type;

            TextureFormat Format;
            TextureFormat InternalFormat;

            TextureFilter MinFilter;
            TextureFilter MagFilter;

            TextureWrap WrapS;
            TextureWrap WrapT;
        };

        class Texture
        {
        public:
            inline void Create(TextureSpecification spec)
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

            inline TextureType GetType()
            {
                return m_Spec.Type;
            }

            inline TextureFormat GetFormat()
            {
                return m_Spec.InternalFormat;
            }

            inline TextureSpecification GetSpec()
            {
                return m_Spec;
            }

            inline void Use(uint32_t slot = 0)
            {
                UseFn(*this, slot);
            }

        private:
            Handle m_Handle;
            Handle m_MemoryHandle;
            TextureSpecification m_Spec;

            static void( * CreateFn)(Texture &, TextureSpecification);
            static void( * DestroyFn)(Texture &);

            static void( * UseFn)(Texture &, uint32_t);

            friend class APIManager;

            friend class GL33Texture;
            friend class GL45Texture;
            friend class VK12Texture;
        };
    }
}
