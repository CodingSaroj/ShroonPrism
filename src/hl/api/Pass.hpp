#pragma once

#include <vector>

#include "hl/API.hpp"
#include "hl/APIManager.hpp"

#include "hl/api/Pipeline.hpp"

namespace Shroon
{
    namespace Prism
    {
        namespace HL
        {
            struct PassSpecification
            {
                struct SubpassSpecification
                {
                    bool ClearFramebuffer = true;
                    bool UseDepthStencil = false;

                    bool UsePreviousOutput = false;
                    bool Offscreen = false;

                    uint8_t SampleCount = 1;

                    Pipeline * TargetPipeline = nullptr;
                    PipelineState TargetPipelineState;
                };

                std::vector<SubpassSpecification> SubpassSpecs;
            };

            class Pass
            {
            public:
                inline ~Pass()
                {
                    DestroyFn(*this);
                }

                inline void Create(PassSpecification & spec)
                {
                    CreateFn(*this, spec);
                }

                inline Handle GetRaw()
                {
                    return m_Handle;
                }

            private:
                Handle m_Handle;
                PassSpecification m_Spec;

                static void( * CreateFn)(Pass &, PassSpecification &);
                static void( * DestroyFn)(Pass &);

                friend class APIManager;

                friend class GL33Pass;
                friend class GL45Pass;
                friend class ES20Pass;
                friend class ES30Pass;

                friend class GL33CommandBuffer;
                friend class CommandBuffer;
                friend class GL45CommandBuffer;
                friend class ES20CommandBuffer;
                friend class ES30CommandBuffer;
            };
        }
    }
}
