#pragma once

namespace Shroon
{
    namespace Prism
    {
        enum class API
        {
            GL33,
            GL45,

            VK12
        };

        extern API s_GraphicsAPI;
    }
}
