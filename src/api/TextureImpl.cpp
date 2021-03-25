#include "api/Texture.hpp"
#include "vk12/Texture.hpp"
#include "gl33/Texture.hpp"
#include "gl45/Texture.hpp"

namespace Shroon
{
    namespace Prism
    {
        void( * Texture::CreateFn)(Texture &, TextureSpecification) = 0;
        void( * Texture::DestroyFn)(Texture &) = 0;

        void( * Texture::UseFn)(Texture &, uint32_t) = 0;

        uint32_t TextureType::Texture2D = 0;

        uint32_t TextureFormat::RED = 0;
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
                    Texture::CreateFn   = GL45Texture::Create;
                    Texture::DestroyFn  = GL45Texture::Destroy;
                    Texture::UseFn      = GL45Texture::Use;

                    TextureType::Texture2D              = GL_TEXTURE_2D;

                    TextureFormat::RED              = GL_RED;
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
                    Texture::UseFn      = GL45Texture::Use;

                    TextureType::Texture2D              = GL_TEXTURE_2D;

                    TextureFormat::RED              = GL_RED;
                    TextureFormat::RGB              = GL_RGB;
                    TextureFormat::RGBA             = GL_RGBA;
                    TextureFormat::DEPTH24_STENCIL8 = GL_DEPTH24_STENCIL8;

                    TextureFilter::Linear   = GL_LINEAR;
                    TextureFilter::Nearest  = GL_NEAREST;

                    TextureWrap::Repeat         = GL_REPEAT;
                    TextureWrap::MirroredRepeat = GL_MIRRORED_REPEAT;

                    break;
                }

                case API::VK12:
                {
                    Texture::CreateFn   = VK12Texture::Create;
                    Texture::DestroyFn  = VK12Texture::Destroy;

                    TextureType::Texture2D  = VK_IMAGE_TYPE_2D;

                    TextureFormat::RED              = VK_FORMAT_R32_SFLOAT;
                    TextureFormat::RGB              = VK_FORMAT_R8G8B8_SRGB;
                    TextureFormat::RGBA             = VK_FORMAT_R8G8B8A8_SRGB;
                    TextureFormat::DEPTH24_STENCIL8 = VK_FORMAT_D24_UNORM_S8_UINT;

                    TextureFilter::Linear   = VK_FILTER_LINEAR;
                    TextureFilter::Nearest  = VK_FILTER_NEAREST;

                    TextureWrap::Repeat         = VK_SAMPLER_ADDRESS_MODE_REPEAT;
                    TextureWrap::MirroredRepeat = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;

                    break;
                }
            }
        }
    }
}
