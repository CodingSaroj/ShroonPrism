#include "Pass.hpp"

namespace Shroon
{
    namespace Prism
    {
        namespace HL
        {
            void GL33Pass::Create(Pass & pass, PassSpecification & spec)
            {
                for (auto & subpassSpec : spec.SubpassSpecs)
                {
                    if (!subpassSpec.TargetPipeline)
                    {
                        ErrorReporter(SHRN_PRISM_LEVEL_FATAL, "OpenGL3.3::Pass", "Subpass must specify a pipeline to use.");
                        return;
                    }
                }

                pass.m_Spec = spec;
            }

            void GL33Pass::Destroy(Pass & pass)
            {
            }
        }
    }
}
