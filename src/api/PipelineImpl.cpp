#include "api/Pipeline.hpp"
#include "vk12/Pipeline.hpp"
#include "gl33/Pipeline.hpp"
#include "gl45/Pipeline.hpp"

namespace Shroon
{
    namespace Prism
    {
        void( * Pipeline::CreateFn)(Pipeline &, PipelineSpecification) = 0;
        void( * Pipeline::DestroyFn)(Pipeline &) = 0;

        void( * Pipeline::RunFn)(Pipeline &, std::vector<PipelineObject> &) = 0;

        uint32_t GeometryMode::Points = 0;
        uint32_t GeometryMode::Lines = 0;
        uint32_t GeometryMode::LineStrip = 0;
        uint32_t GeometryMode::Triangles = 0;
        uint32_t GeometryMode::TriangleStrip = 0;

        uint32_t PolygonMode::Wireframe = 0;
        uint32_t PolygonMode::Fill = 0;

        void APIManager::InitPipelineAPI()
        {
            switch (s_GraphicsAPI)
            {
                case API::GL33:
                {
                    Pipeline::CreateFn  = GL45Pipeline::Create;
                    Pipeline::DestroyFn = GL45Pipeline::Destroy;
                    Pipeline::RunFn     = GL45Pipeline::Run;

                    GeometryMode::Points        = GL_POINTS;
                    GeometryMode::Lines         = GL_LINES;
                    GeometryMode::LineStrip     = GL_LINE_STRIP;
                    GeometryMode::Triangles     = GL_TRIANGLES;
                    GeometryMode::TriangleStrip = GL_TRIANGLE_STRIP;

                    PolygonMode::Wireframe = GL_LINE;
                    PolygonMode::Fill = GL_FILL;

                    break;
                }

                case API::GL45:
                {
                    Pipeline::CreateFn  = GL45Pipeline::Create;
                    Pipeline::DestroyFn = GL45Pipeline::Destroy;
                    Pipeline::RunFn     = GL45Pipeline::Run;

                    GeometryMode::Points        = GL_POINTS;
                    GeometryMode::Lines         = GL_LINES;
                    GeometryMode::LineStrip     = GL_LINE_STRIP;
                    GeometryMode::Triangles     = GL_TRIANGLES;
                    GeometryMode::TriangleStrip = GL_TRIANGLE_STRIP;

                    PolygonMode::Wireframe = GL_LINE;
                    PolygonMode::Fill = GL_FILL;

                    break;
                }

                case API::VK12:
                {
                    Pipeline::CreateFn  = VK12Pipeline::Create;
                    Pipeline::DestroyFn = VK12Pipeline::Destroy;

                    GeometryMode::Points        = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
                    GeometryMode::Lines         = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
                    GeometryMode::LineStrip     = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
                    GeometryMode::Triangles     = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
                    GeometryMode::TriangleStrip = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;

                    PolygonMode::Wireframe = VK_POLYGON_MODE_LINE;
                    PolygonMode::Fill = VK_POLYGON_MODE_FILL;

                    break;
                }
            }
        }
    }
}
