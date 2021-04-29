#pragma once

#include "ErrorReporter.hpp"

#include "hl/api/Pass.hpp"

#include "hl/es30/Core.hpp"

namespace Shroon
{
    namespace Prism
    {
        namespace HL
        {
            class ES30Pass
            {
            public:
                static void Create(Pass & pass, PassSpecification & spec);
                static void Destroy(Pass & pass);
            };
        }
    }
}
