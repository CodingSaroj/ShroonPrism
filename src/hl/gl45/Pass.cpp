#include "Pass.hpp"

namespace Shroon
{
    namespace Prism
    {
        namespace HL
        {
            void GL45Pass::Create(Pass & pass, PassSpecification & spec)
            {
                for (auto & subpassSpec : spec.SubpassSpecs)
                {
                    if (!subpassSpec.TargetPipeline)
                    {
                        ErrorReporter(SHRN_PRISM_LEVEL_FATAL, "OpenGL4.5", "Subpass must specify a pipeline to use.");
                        return;
                    }
                }

                pass.m_Spec = spec;
            }

            void GL45Pass::Destroy(Pass & pass)
            {
            }
        }
    }
}
