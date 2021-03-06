#include "hl/api/Pipeline.hpp"
#include "hl/gl45/Pipeline.hpp"

namespace Shroon
{
    namespace Prism
    {
        namespace HL
        {
            void( * Pipeline::CreateFn)(Pipeline &, PipelineSpecification &) = 0;
            void( * Pipeline::DestroyFn)(Pipeline &) = 0;

            void( * Pipeline::ApplyStateChangesFn)(Pipeline &) = 0;

            uint32_t PrimitiveType::Points = 0;
            uint32_t PrimitiveType::Lines = 0;
            uint32_t PrimitiveType::LineStrip = 0;
            uint32_t PrimitiveType::Triangles = 0;
            uint32_t PrimitiveType::TriangleStrip = 0;

            uint32_t FillType::Wireframe = 0;
            uint32_t FillType::Fill = 0;

            uint32_t CullFace::Front = 0;
            uint32_t CullFace::Back = 0;

            uint32_t VertexAttribType::Byte = 0;
            uint32_t VertexAttribType::UByte = 0;
            uint32_t VertexAttribType::Short = 0;
            uint32_t VertexAttribType::UShort = 0;
            uint32_t VertexAttribType::Int = 0;
            uint32_t VertexAttribType::UInt = 0;
            uint32_t VertexAttribType::Float = 0;

            void APIManager::InitPipelineAPI()
            {
                switch (s_GraphicsAPI)
                {
//                    case API::GL33:
//                    {
//                        Pipeline::CreateFn  = GL45Pipeline::Create;
//                        Pipeline::DestroyFn = GL45Pipeline::Destroy;
//
//                        PrimitiveType::Points        = GL_POINTS;
//                        PrimitiveType::Lines         = GL_LINES;
//                        PrimitiveType::LineStrip     = GL_LINE_STRIP;
//                        PrimitiveType::Triangles     = GL_TRIANGLES;
//                        PrimitiveType::TriangleStrip = GL_TRIANGLE_STRIP;
//
//                        FillType::Wireframe  = GL_LINE;
//                        FillType::Fill       = GL_FILL;
//
//                        VertexAttribType::Byte      = GL_BYTE;
//                        VertexAttribType::UByte     = GL_UNSIGNED_BYTE;
//                        VertexAttribType::Short     = GL_SHORT;
//                        VertexAttribType::UShort    = GL_UNSIGNED_SHORT;
//                        VertexAttribType::Int       = GL_INT;
//                        VertexAttribType::UInt      = GL_UNSIGNED_INT;
//                        VertexAttribType::Float     = GL_FLOAT;
//
//                        break;
//                    }

                    case API::GL45:
                    {
                        Pipeline::CreateFn              = GL45Pipeline::Create;
                        Pipeline::DestroyFn             = GL45Pipeline::Destroy;

                        Pipeline::ApplyStateChangesFn   = GL45Pipeline::ApplyStateChanges;

                        PrimitiveType::Points        = GL_POINTS;
                        PrimitiveType::Lines         = GL_LINES;
                        PrimitiveType::LineStrip     = GL_LINE_STRIP;
                        PrimitiveType::Triangles     = GL_TRIANGLES;
                        PrimitiveType::TriangleStrip = GL_TRIANGLE_STRIP;

                        FillType::Wireframe  = GL_LINE;
                        FillType::Fill       = GL_FILL;

                        CullFace::Front = GL_FRONT;
                        CullFace::Back  = GL_BACK;

                        VertexAttribType::Byte      = GL_BYTE;
                        VertexAttribType::UByte     = GL_UNSIGNED_BYTE;
                        VertexAttribType::Short     = GL_SHORT;
                        VertexAttribType::UShort    = GL_UNSIGNED_SHORT;
                        VertexAttribType::Int       = GL_INT;
                        VertexAttribType::UInt      = GL_UNSIGNED_INT;
                        VertexAttribType::Float     = GL_FLOAT;

                        break;
                    }
                }
            }
        }
    }
}
