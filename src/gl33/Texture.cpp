#include "Texture.hpp"

namespace Shroon
{
    namespace Prism
    {
        void GL33Texture::Create(Texture & tex, TextureSpecification spec)
        {
            tex.m_Spec = spec;

            glGenTextures(1, reinterpret_cast<GLuint *>(&tex.m_Handle));
            glBindTexture(spec.Type, tex.m_Handle);

            if (spec.InternalFormat == TextureFormat::DEPTH24_STENCIL8)
            {
                glTexImage2D(spec.Type, 0, GL_DEPTH24_STENCIL8, spec.Width, spec.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, spec.Data);
            }
            else
            {
                glTexImage2D(spec.Type, 0, spec.InternalFormat, spec.Width, spec.Height, 0, spec.Format, GL_UNSIGNED_BYTE, spec.Data);
            }


            glTexParameteri(spec.Type, GL_TEXTURE_MIN_FILTER, spec.MinFilter);
            glTexParameteri(spec.Type, GL_TEXTURE_MAG_FILTER, spec.MagFilter);
            glTexParameteri(spec.Type, GL_TEXTURE_WRAP_S, spec.WrapS);
            glTexParameteri(spec.Type, GL_TEXTURE_WRAP_T, spec.WrapT);

            if (spec.UseMipmaps)
            {
                glGenerateMipmap(spec.Type);
            }
        }

        void GL33Texture::Destroy(Texture & tex)
        {
            glDeleteTextures(1, reinterpret_cast<GLuint *>(&tex.m_Handle));
        }

        void GL33Texture::Use(Texture & tex, uint32_t slot)
        {
            glActiveTexture(GL_TEXTURE0 + slot);
            glBindTexture(tex.m_Spec.Type, tex.m_Handle);
        }
    }
}
