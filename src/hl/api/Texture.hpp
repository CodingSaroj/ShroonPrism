#pragma once

#include <cstdint>

#include "Handle.hpp"

#include "hl/API.hpp"
#include "hl/APIManager.hpp"

namespace Shroon
{
    namespace Prism
    {
        namespace HL
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
                inline ~Texture()
                {
                    DestroyFn(*this);
                }

                inline void Create(TextureSpecification & spec)
                {
                    CreateFn(*this, spec);
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

            private:
                Handle m_Handle;
                TextureSpecification m_Spec;

                static void( * CreateFn)(Texture &, TextureSpecification &);
                static void( * DestroyFn)(Texture &);

                friend class APIManager;

                friend class GL33Texture;
                friend class GL45Texture;
                friend class ES20Texture;
                friend class ES30Texture;
            };
        }
    }
}
