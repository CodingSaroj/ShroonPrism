#pragma once

#include "ErrorReporter.hpp"

#include "hl/api/Pass.hpp"

#include "hl/gl33/Core.hpp"

namespace Shroon
{
    namespace Prism
    {
        namespace HL
        {
            class GL33Pass
            {
            public:
                static void Create(Pass & pass, PassSpecification & spec);
                static void Destroy(Pass & pass);
            };
        }
    }
}
