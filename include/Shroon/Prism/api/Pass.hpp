#pragma once

#include <vector>

#include "Handle.hpp"

namespace Shroon
{
    namespace Prism
    {
        struct PassSpecification
        {
            struct SubpassSpecification
            {
                bool ClearFramebuffer = true;
                bool UseStencilBuffer = false;

                bool UsePreviousOutput = false;
                bool Offscreen = false;

                uint8_t SampleCount = 1;
            };

            std::vector<SubpassSpecification> SubpassSpecs;
        };

        class Pass
        {
        public:
            inline Handle GetRaw()
            {
                return m_Handle;
            }

        private:
            Handle m_Handle;

            friend class VK12Pass;
        };
    }
}
