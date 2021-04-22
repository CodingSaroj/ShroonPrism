#include "hl/api/Texture.hpp"
#include "hl/gl33/Texture.hpp"
#include "hl/gl45/Texture.hpp"

namespace Shroon
{
    namespace Prism
    {
        namespace HL
        {
            void( * Texture::CreateFn)(Texture &, TextureSpecification &) = 0;
            void( * Texture::DestroyFn)(Texture &) = 0;

            uint32_t TextureType::Texture2D = 0;

            uint32_t TextureFormat::RED = 0;
            uint32_t TextureFormat::RED_INT = 0;
            uint32_t TextureFormat::RGB = 0;
            uint32_t TextureFormat::RGBA = 0;
            uint32_t TextureFormat::DEPTH24_STENCIL8 = 0;

            uint32_t TextureFilter::Linear = 0;
            uint32_t TextureFilter::Nearest = 0;

            uint32_t TextureWrap::Repeat = 0;
            uint32_t TextureWrap::MirroredRepeat = 0;

            void APIManager::InitTextureAPI()
            {
                switch (s_GraphicsAPI)
                {
                    case API::GL33:
                    {
                        Texture::CreateFn   = GL33Texture::Create;
                        Texture::DestroyFn  = GL33Texture::Destroy;

                        TextureType::Texture2D              = GL_TEXTURE_2D;

                        TextureFormat::RED              = GL_RED;
                        TextureFormat::RED_INT          = GL_RED_INTEGER;
                        TextureFormat::RGB              = GL_RGB;
                        TextureFormat::RGBA             = GL_RGBA;
                        TextureFormat::DEPTH24_STENCIL8 = GL_DEPTH24_STENCIL8;

                        TextureFilter::Linear   = GL_LINEAR;
                        TextureFilter::Nearest  = GL_NEAREST;

                        TextureWrap::Repeat         = GL_REPEAT;
                        TextureWrap::MirroredRepeat = GL_MIRRORED_REPEAT;

                        break;
                    }

                    case API::GL45:
                    {
                        Texture::CreateFn   = GL45Texture::Create;
                        Texture::DestroyFn  = GL45Texture::Destroy;

                        TextureType::Texture2D              = GL_TEXTURE_2D;

                        TextureFormat::RED              = GL_RED;
                        TextureFormat::RED_INT          = GL_RED_INTEGER;
                        TextureFormat::RGB              = GL_RGB;
                        TextureFormat::RGBA             = GL_RGBA;
                        TextureFormat::DEPTH24_STENCIL8 = GL_DEPTH24_STENCIL8;

                        TextureFilter::Linear   = GL_LINEAR;
                        TextureFilter::Nearest  = GL_NEAREST;

                        TextureWrap::Repeat         = GL_REPEAT;
                        TextureWrap::MirroredRepeat = GL_MIRRORED_REPEAT;

                        break;
                    }
                }
            }
        }
    }
}
