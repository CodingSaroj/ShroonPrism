#include "Pass.hpp"

namespace Shroon
{
    namespace Prism
    {
        namespace HL
        {
            void ES30Pass::Create(Pass & pass, PassSpecification & spec)
            {
                for (auto & subpassSpec : spec.SubpassSpecs)
                {
                    if (!subpassSpec.TargetPipeline)
                    {
                        ErrorReporter(SHRN_PRISM_LEVEL_FATAL, "OpenGLES3.0::Pass", "Subpass must specify a pipeline to use.");
                        return;
                    }
                }

                pass.m_Spec = spec;
            }

            void ES30Pass::Destroy(Pass & pass)
            {
            }
        }
    }
}
