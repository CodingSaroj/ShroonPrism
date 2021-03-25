#include "Pipeline.hpp"

namespace Shroon
{
    namespace Prism
    {
        void GL33Pipeline::Create(Pipeline & pl, PipelineSpecification spec)
        {
            pl.m_Handle = pl.m_Shader.GetRaw();
            pl.m_Spec = spec;
            pl.m_Shader.Create(spec.PipelineShaderSpec);
        }

        void GL33Pipeline::Destroy(Pipeline & pl)
        {
            pl.m_Shader.Destroy();
        }

        void GL33Pipeline::Run(Pipeline & pl, std::vector<PipelineObject> & objects)
        {
            glCullFace(GL_BACK);

            if (pl.m_Spec.DepthTesting)
            {
                glEnable(GL_DEPTH_TEST);
                glDepthFunc(GL_LESS);
            }
            else
            {
                glDisable(GL_DEPTH_TEST);
            }

            if (pl.m_Spec.StencilTesting)
            {
                glEnable(GL_STENCIL_TEST);
            }
            else
            {
                glDisable(GL_STENCIL_TEST);
            }

            glUseProgram(pl.m_Shader.GetRaw());

            for (size_t i = 0; i < objects.size(); i++)
            {
                glPolygonMode(GL_FRONT_AND_BACK, objects[i].ObjectPolygonMode);

                if (i == 0)
                {
                    glBindVertexArray(objects[i].ObjectMesh.GetRaw());
                }
                else if (objects[i - 1].ObjectMesh.GetRaw() == objects[i].ObjectMesh.GetRaw())
                {
                    glBindVertexArray(objects[i].ObjectMesh.GetRaw());
                }

                UniformManager::UpdateUniformBlock("ObjectData", objects[i].ObjectData.data(), objects[i].ObjectData.size());

                for (auto & name : pl.m_Shader.GetRequiredUniformBlockNames())
                {
                    pl.m_Shader.BindUniformBlock(name, UniformManager::GetBindingPoint(name));
                }

                if (objects[i].ObjectGeometryMode == GeometryMode::Points
                    || objects[i].ObjectGeometryMode == GeometryMode::Lines
                    || objects[i].ObjectGeometryMode == GeometryMode::LineStrip)
                {
                    glDrawArrays(objects[i].ObjectGeometryMode, 0, objects[i].ObjectMesh.GetVertexCount());
                }
                else
                {
                    glDrawElements(objects[i].ObjectGeometryMode, objects[i].ObjectMesh.GetVertexCount(), GL_UNSIGNED_INT, 0);
                }
            }
        }
    }
}
